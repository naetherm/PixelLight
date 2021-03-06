/*********************************************************\
 *  File: Script.cpp                                     *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Log/Log.h>
#include "PLScriptPython/PythonContext.h"
#include "PLScriptPython/Script.h"
#include <Python/graminit.h>	// Python parse-mode flags (e.g. "file_input")


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4127) // "conditional expression is constant" (within "Py_XDECREF()" and "Py_DECREF()")


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptPython {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Script, "PLScriptPython", PLCore::Script, "Python (http://www.python.org/) script implementation")
	// Properties
	pl_properties
		pl_property("Language",	"Python")
		pl_property("Formats",	"py,PY")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(Script)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Script::Script() :
	m_pPythonModule(nullptr),
	m_pPythonDictionary(nullptr),
	m_pPythonFunction(nullptr),
	m_pPythonTuple(nullptr),
	m_nCurrentArgument(0),
	m_pPythonFunctionResult(nullptr),
	m_pPythonTableOfFunctions(nullptr)
{
	// Add a context reference
	PythonContext::AddContextReference();
}

/**
*  @brief
*    Destructor
*/
Script::~Script()
{
	// Clear the script
	Clear();

	// Remove all global functions
	RemoveAllGlobalFunctions();

	// Release a context reference
	PythonContext::ReleaseContextReference();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Script functions               ]
//[-------------------------------------------------------]
bool Script::IsGlobalFunction(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support
	if (sNamespace.GetLength())
		return false;	// Not implemented, yet. Get us out of here right now!

	// Is there a Python module dictionary? If so, request the Python function (results in borrowed reference, don't use Py_DECREF on it).
	return (m_pPythonDictionary && PyDict_GetItemString(m_pPythonDictionary, sName));
}

bool Script::AddGlobalFunction(const String &sFunction, const DynFunc &cDynFunc, const String &sNamespace)
{
	// Is there a Python module?
	if (m_pPythonModule) {
		// Error!
		return false;
	} else {
		// Add the global function
		GlobalFunction *psGlobalFunction = new GlobalFunction;
		psGlobalFunction->sFunction  = sFunction;
		psGlobalFunction->pDynFunc   = cDynFunc.Clone();
		psGlobalFunction->sNamespace = sNamespace;
		m_lstGlobalFunctions.Add(psGlobalFunction);

		// Done
		return true;
	}
}

bool Script::RemoveAllGlobalFunctions()
{
	// Is there a Python module?
	if (m_pPythonModule) {
		// Error!
		return false;
	} else {
		// Destroy the global functions
		for (uint32 i=0; i<m_lstGlobalFunctions.GetNumOfElements(); i++) {
			delete m_lstGlobalFunctions[i]->pDynFunc;
			delete m_lstGlobalFunctions[i];
		}
		m_lstGlobalFunctions.Clear();

		// Done
		return true;
	}
}

String Script::GetSourceCode() const
{
	return m_sSourceCode;
}

bool Script::SetSourceCode(const String &sSourceCode)
{
	// Clear the previous script
	Clear();

	// Backup the given source code
	m_sSourceCode = sSourceCode;

	// Is there source code?
	if (m_sSourceCode.GetLength()) {
		// Get the name of our new Python module
		const String sModuleName = Name.Get().GetLength() ? Name.Get() : "__main__";

		// Create an empty Python module (results in borrowed reference, don't use Py_DECREF on it)
		m_pPythonModule = PyImport_AddModule(sModuleName);
		if (m_pPythonModule) {
			// Get the dictionary of our module (results in borrowed reference, don't use Py_DECREF on it)
			m_pPythonDictionary = PyModule_GetDict(m_pPythonModule);
			if (m_pPythonDictionary) {
				// Are there any global functions?
				if (m_lstGlobalFunctions.GetNumOfElements()) {
					// Allocate the Python table of functions
					m_pPythonTableOfFunctions = new PyMethodDef[m_lstGlobalFunctions.GetNumOfElements()];

					// Add the global functions
					for (uint32 i=0; i<m_lstGlobalFunctions.GetNumOfElements(); i++) {
						// Get the global function
						GlobalFunction *psGlobalFunction = m_lstGlobalFunctions[i];

						// Get the current Python method definition
						PyMethodDef *pPythonMethodDefinition = &m_pPythonTableOfFunctions[i];

						// Setup the current Python method definition
						pPythonMethodDefinition->ml_name  = psGlobalFunction->sFunction;	// The name of the built-in function/method
						pPythonMethodDefinition->ml_meth  = PythonFunctionCallback;			// The C function that implements it
						pPythonMethodDefinition->ml_flags = METH_VARARGS;					// Combination of METH_xxx flags, which mostly describe the args expected by the C func
						pPythonMethodDefinition->ml_doc   = nullptr;						// The __doc__ attribute, or NULL

						// Create the new Python function object
						PyObject *pPythonFunction = PyCFunction_New(pPythonMethodDefinition, PyCapsule_New(psGlobalFunction, nullptr, nullptr));

						// Add the new Python function
						if (!AddPythonFunction(m_pPythonDictionary, psGlobalFunction->sFunction, pPythonFunction, psGlobalFunction->sNamespace)) {
							// Error!
							LogOutput(Log::Error, "Failed to add the function '" + psGlobalFunction->sFunction + "\' to the Python module '" + sModuleName + '\'');
						}

						// Remove our reference from the Python function object
						Py_DECREF(pPythonFunction);
					}
				}

				// "Run" the Python script within the dictionary in order to set the source code (results in new reference, use Py_DECREF on the result)
				PyObject *pPythonResult = PyRun_String(sSourceCode, file_input, m_pPythonDictionary, m_pPythonDictionary);
				if (pPythonResult) {
					// Remove our reference from the Python result object
					Py_DECREF(pPythonResult);

					// Done
					return true;
				} else {
					// Error!
					LogOutputWithErrorDescription(Log::Error, "Failed to run the new Python module with the name '" + sModuleName + "\' in order to set the source code", GetPythonErrorDescription());
				}
			} else {
				// Error!
				LogOutput(Log::Error, "Failed to get the dictionary of the new empty Python module with the name '" + sModuleName + '\'');
			}
		} else {
			// Error!
			LogOutput(Log::Error, "Failed to create a new empty Python module with the name '" + sModuleName + '\'');
		}

		// "m_pPythonModule", "m_pPythonDictionary" and "m_pPythonFunction" are borrowed references, don't use Py_DECREF on them
		m_pPythonModule     = nullptr;
		m_pPythonDictionary = nullptr;
		m_pPythonFunction   = nullptr;
	} else {
		// No script at all - done
		return true;
	}

	// Error!
	return false;
}

void Script::GetAssociatedFilenames(Array<String> &lstFilenames)
{
	// [TODO] Implement me
}

bool Script::Execute(const String &sSourceCode)
{
	// [TODO] Implement me
	return false;
}

void Script::GetGlobalVariables(Array<String> &lstGlobalVariables, const String &sNamespace)
{
	// [TODO] Add namespace support
	if (sNamespace.GetLength())
		return;	// Not implemented, yet. Get us out of here right now!

	// Is there a Python dictionary?
	if (m_pPythonDictionary) {
		// Iterate through the global dictionary of the Python module (results in borrowed references, don't use Py_DECREF on them)
		PyObject *pPythonKey = nullptr, *pPythonValue = nullptr;
		Py_ssize_t nPos = 0;
		while (PyDict_Next(m_pPythonDictionary, &nPos, &pPythonKey, &pPythonValue)) {
			// Is this Python object a global variable? (something like "__name__" is passing this test as well, but that's probably ok because it's just a Python build in global variable)
			if (PyString_Check(pPythonValue) || PyInt_Check(pPythonValue) || PyFloat_Check(pPythonValue)) {
				// Add the global variable to the list
				lstGlobalVariables.Add(PyString_AsString(pPythonKey));
			}
		}
	}
}

bool Script::IsGlobalVariable(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support

	return m_pPythonModule ? (PyObject_HasAttrString(m_pPythonModule, sName) != 0) : false;
}

ETypeID Script::GetGlobalVariableTypeID(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support

	// Is there a Python module?
	if (m_pPythonModule) {
		// Request the Python attribute (results in borrowed reference, don't use Py_DECREF on it)
		PyObject *pPythonAttribute = PyObject_GetAttrString(m_pPythonModule, sName);
		if (pPythonAttribute) {
			if (PyString_Check(pPythonAttribute))
				return TypeString;
			else if (PyInt_Check(pPythonAttribute))
				return TypeInt32;
			else if (PyFloat_Check(pPythonAttribute))
				return TypeFloat;
			// [TODO] Add Object* support
		}
	}

	// Error!
	return TypeInvalid;
}

String Script::GetGlobalVariable(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support

	// Is there a Python module?
	if (m_pPythonModule) {
		// Request the Python attribute (results in borrowed reference, don't use Py_DECREF on it)
		PyObject *pPythonAttribute = PyObject_GetAttrString(m_pPythonModule, sName);
		if (pPythonAttribute) {
			if (PyString_Check(pPythonAttribute))
				return PyString_AsString(pPythonAttribute);
			else if (PyInt_Check(pPythonAttribute))
				return static_cast<int32>(PyInt_AsLong(pPythonAttribute));	// [TODO] Use int32 or int64? (currently int32 is used)
			else if (PyFloat_Check(pPythonAttribute))
				return PyFloat_AsDouble(pPythonAttribute);
		}
	}

	// Error!
	return "";
}

void Script::SetGlobalVariable(const String &sName, const DynVar &cValue, const String &sNamespace)
{
	// [TODO] Add namespace support

	// Is there a Python module?
	if (m_pPythonModule) {
		// Request the Python attribute because we don't want to change it's type (results in borrowed reference, don't use Py_DECREF on it)
		PyObject *pPythonAttribute = PyObject_GetAttrString(m_pPythonModule, sName);
		if (pPythonAttribute) {
			if (PyString_Check(pPythonAttribute))
				PyObject_SetAttrString(m_pPythonModule, sName, PyString_FromString(cValue.GetString()));
			else if (PyInt_Check(pPythonAttribute))
				PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));
			else if (PyFloat_Check(pPythonAttribute))
				PyObject_SetAttrString(m_pPythonModule, sName, PyFloat_FromDouble(cValue.GetDouble()));
			// [TODO] Add Object* support
		} else {
			// Ok, this must be a new global variable
			switch (cValue.GetTypeID()) {
				case TypeVoid:																									break;	// ? Yeah, that's really funny!
				case TypeBool:		PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				case TypeDouble:	PyObject_SetAttrString(m_pPythonModule, sName, PyFloat_FromDouble(cValue.GetDouble()));		break;
				case TypeFloat:		PyObject_SetAttrString(m_pPythonModule, sName, PyFloat_FromDouble(cValue.GetDouble()));		break;
				case TypeInt:		PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				case TypeInt16:		PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				case TypeInt32:		PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				case TypeInt64:		PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;	// [TODO] TypeInt64 is currently handled just as int
				case TypeInt8:		PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				case TypeString:	PyObject_SetAttrString(m_pPythonModule, sName, PyString_FromString(cValue.GetString()));	break;
				case TypeUInt16:	PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				case TypeUInt32:	PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				case TypeUInt64:	PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;	// [TODO] TypeUInt64 is currently handled just as int
				case TypeUInt8:		PyObject_SetAttrString(m_pPythonModule, sName, PyInt_FromLong(cValue.GetInt()));			break;
				// [TODO] Add Object* support
			}
		}
	}
}

bool Script::BeginCall(const String &sFunctionName, const String &sFunctionSignature, const String &sNamespace)
{
	// [TODO] Add namespace support
	if (sNamespace.GetLength())
		return false;	// Error! Not implemented, yet. Get us out of here right now!

	// Is there a Python module dictionary?
	if (m_pPythonDictionary) {
		// Request the Python function (results in borrowed reference, don't use Py_DECREF on it)
		m_pPythonFunction = PyDict_GetItemString(m_pPythonDictionary, sFunctionName);
		if (m_pPythonFunction) {
			// Functions must be callable
			if (PyCallable_Check(m_pPythonFunction)) {
				// Remove our reference of the Python tuple
				if (m_pPythonTuple) {
					Py_DECREF(m_pPythonTuple);
					m_pPythonTuple = nullptr;
				}

				// Current argument is 0
				m_nCurrentArgument = 0;

				// Remove our reference of from Python function result
				if (m_pPythonFunctionResult) {
					Py_DECREF(m_pPythonFunctionResult);
					m_pPythonFunctionResult = nullptr;
				}

				// Done
				return true;
			} else {
				// "m_pPythonFunction" is a borrowed reference, don't use Py_DECREF on it
				m_pPythonFunction = nullptr;

				// Error!
				LogOutput(Log::Error, "The function '" + sFunctionName + "' is no callable");
			}
		} else {
			// Error!
			LogOutput(Log::Error, "The function '" + sFunctionName + "' was not found");
		}
	} else {
		// Error!
		LogOutput(Log::Error, "The function '" + sFunctionName + "' was not found because there's no Python module dictionary");
	}

	// Error!
	return false;
}

void Script::PushArgument(bool bValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(bValue));
	}
}

void Script::PushArgument(float fValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyFloat_FromDouble(fValue));
	}
}

void Script::PushArgument(double fValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyFloat_FromDouble(fValue));
	}
}

void Script::PushArgument(int8 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(nValue));
	}
}

void Script::PushArgument(int16 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(nValue));
	}
}

void Script::PushArgument(int32 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(nValue));
	}
}

void Script::PushArgument(int64 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		// [TODO] There's no int64 support in Python (?)
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(static_cast<long>(nValue)));
	}
}

void Script::PushArgument(uint8 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(nValue));
	}
}

void Script::PushArgument(uint16 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(nValue));
	}
}

void Script::PushArgument(uint32 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(nValue));
	}
}

void Script::PushArgument(uint64 nValue)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		// [TODO] There's no uint64 support in Python (?)
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyInt_FromLong(static_cast<long>(nValue)));
	}
}

void Script::PushArgument(const String &sString)
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Increases the number of arguments
		IncreaseNumOfArguments();

		// Set the current tuple item
		PyTuple_SetItem(m_pPythonTuple, m_nCurrentArgument - 1, PyString_FromString(sString));
	}
}

void Script::PushArgument(Object *pObject)
{
	// [TODO] Implement me
}

void Script::PushArgument(Object &cObject)
{
	// [TODO] Implement me
}

bool Script::EndCall()
{
	// Is there a current Python function?
	if (m_pPythonFunction) {
		// Call the Python function (results in new reference, use Py_DECREF on the result)
		m_pPythonFunctionResult = PyObject_CallObject(m_pPythonFunction, m_pPythonTuple);
		if (!m_pPythonFunctionResult) {
			// Give some log error output (results in new reference, use Py_DECREF on the result)
			PyObject *pPythonFunctionName = PyObject_GetAttrString(m_pPythonFunction, "__name__");
			if (pPythonFunctionName) {
				LogOutputWithErrorDescription(Log::Error, String("The call of the function '") + PyString_AsString(pPythonFunctionName) + "' failed", GetPythonErrorDescription());

				// Remove our reference of from Python function name
				Py_DECREF(pPythonFunctionName);
			} else {
				LogOutputWithErrorDescription(Log::Error, "Function call failed", GetPythonErrorDescription());
			}

			// Error!
			return false;
		}
	}

	// Done
	return true;
}

bool Script::GetReturn(bool nValue)
{
	return m_pPythonFunctionResult ? (PyInt_AsLong(m_pPythonFunctionResult) != 0): false;
}

float Script::GetReturn(float nValue)
{
	return m_pPythonFunctionResult ? static_cast<float>(PyFloat_AsDouble(m_pPythonFunctionResult)) : 0.0f;
}

double Script::GetReturn(double nValue)
{
	return m_pPythonFunctionResult ? PyFloat_AsDouble(m_pPythonFunctionResult) : 0.0;
}

int8 Script::GetReturn(int8 nValue)
{
	return m_pPythonFunctionResult ? static_cast<uint8>(PyInt_AsLong(m_pPythonFunctionResult)) : 0;
}

int16 Script::GetReturn(int16 nValue)
{
	return m_pPythonFunctionResult ? static_cast<uint16>(PyInt_AsLong(m_pPythonFunctionResult)) : 0;
}

int32 Script::GetReturn(int32 nValue)
{
	return m_pPythonFunctionResult ? PyInt_AsLong(m_pPythonFunctionResult) : 0;
}

int64 Script::GetReturn(int64 nValue)
{
	// [TODO] There's no int64 support in Python (?)
	return m_pPythonFunctionResult ? PyInt_AsLong(m_pPythonFunctionResult) : 0;
}

uint8 Script::GetReturn(uint8 nValue)
{
	return m_pPythonFunctionResult ? static_cast<uint8>(PyInt_AsLong(m_pPythonFunctionResult)) : 0;
}

uint16 Script::GetReturn(uint16 nValue)
{
	return m_pPythonFunctionResult ? static_cast<uint16>(PyInt_AsLong(m_pPythonFunctionResult)) : 0;
}

uint32 Script::GetReturn(uint32 nValue)
{
	return m_pPythonFunctionResult ? PyInt_AsLong(m_pPythonFunctionResult) : 0;
}

uint64 Script::GetReturn(uint64 nValue)
{
	// [TODO] There's no uint64 support in Python (?)
	return m_pPythonFunctionResult ? PyInt_AsLong(m_pPythonFunctionResult) : 0;
}

String Script::GetReturn(String nValue)
{
	return m_pPythonFunctionResult ? PyString_AsString(m_pPythonFunctionResult) : "";
}

Object *Script::GetReturn(Object *nValue)
{
	// [TODO] Implement me
	return nullptr;
}

Object &Script::GetReturn(Object &nValue)
{
	// ... please note that in here, we can't return a null pointer...
	Object *pObject = GetReturn(&nValue);
	return pObject ? *pObject : nValue;
}


//[-------------------------------------------------------]
//[ Private static Python callback functions              ]
//[-------------------------------------------------------]
/*
*  @brief
*    Python function callback
*/
PyObject *Script::PythonFunctionCallback(PyObject *pPythonSelf, PyObject *pPythonArguments)
{
	// Get the global function and ensure that the Python arguments are in fact a tuple
	GlobalFunction *psGlobalFunction = reinterpret_cast<GlobalFunction*>(PyCapsule_GetPointer(pPythonSelf, nullptr));
	if (psGlobalFunction && PyTuple_Check(pPythonArguments)) {
		// Get the number of arguments Python gave to us
		String sParams;
		const Py_ssize_t nNumOfArguments = PyTuple_Size(pPythonArguments);
		for (Py_ssize_t i=0; i<nNumOfArguments; i++) {
			// Get the Python argument
			PyObject *pPythonArgument = PyTuple_GetItem(pPythonArguments, i);

			// Get the value
			String sValue;
			if (PyString_Check(pPythonArgument))
				sValue = PyString_AsString(pPythonArgument);
			else if (PyInt_Check(pPythonArgument))
				sValue = static_cast<int32>(PyInt_AsLong(pPythonArgument));	// [TODO] Use int32 or int64? (currently int32 is used)
			else if (PyFloat_Check(pPythonArgument))
				sValue = PyFloat_AsDouble(pPythonArgument);

			// Add to functor parameters
			sParams += String("Param") + i + "=\"" + sValue + "\" ";
		}

		// Call the functor
		const String sReturn = psGlobalFunction->pDynFunc->CallWithReturn(sParams);

		// Process the functor return
		switch (psGlobalFunction->pDynFunc->GetReturnTypeID()) {
			case TypeBool:		return PyInt_FromLong	  (sReturn.GetBool());
			case TypeDouble:	return PyFloat_FromDouble (sReturn.GetDouble());
			case TypeFloat:		return PyFloat_FromDouble (sReturn.GetFloat());
			case TypeInt:		return PyInt_FromLong	  (sReturn.GetInt());
			case TypeInt16:		return PyInt_FromLong	  (sReturn.GetInt());
			case TypeInt32:		return PyInt_FromLong	  (sReturn.GetInt());
			case TypeInt64:		return PyInt_FromLong	  (sReturn.GetInt());		// [TODO] TypeInt64 is currently handled just as long
			case TypeInt8:		return PyInt_FromLong	  (sReturn.GetInt());
			case TypeString:	return PyString_FromString(sReturn);
			case TypeUInt16:	return PyInt_FromLong	  (sReturn.GetUInt16());
			case TypeUInt32:	return PyInt_FromLong	  (sReturn.GetUInt32());
			case TypeUInt64:	return PyInt_FromLong	  (static_cast<long>(sReturn.GetUInt64()));	// [TODO] TypeUInt64 is currently handled just as long
			case TypeUInt8:		return PyInt_FromLong	  (sReturn.GetUInt8());
			default:			return Py_None;// TypeVoid, TypeNull, TypeObjectPtr, -1
		}
	}

	// Error!
	return Py_None;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Script::Script(const Script &cSource) :
	m_pPythonModule(nullptr),
	m_pPythonDictionary(nullptr),
	m_pPythonFunction(nullptr),
	m_pPythonTuple(nullptr),
	m_nCurrentArgument(0),
	m_pPythonFunctionResult(nullptr),
	m_pPythonTableOfFunctions(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Script &Script::operator =(const Script &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Returns a string containing the Python error description
*/
String Script::GetPythonErrorDescription() const
{
	String sErrorDescription;

	// Was there an Python error at all?
	if (PyErr_Occurred()) {
		PyObject *pPythonErrorType, *pPythonErrorValue, *pPythonTraceback;
		PyErr_Fetch(&pPythonErrorType, &pPythonErrorValue, &pPythonTraceback);
		if(pPythonErrorValue) {
			// Get a string representation of the Python object (results in new reference, use Py_DECREF on the result)
			PyObject *pPythonString = PyObject_Str(pPythonErrorValue);

			// Get the line number Python object from the track back Python object
			if (pPythonTraceback) {
				PyObject *pPythonLineNumber = PyObject_GetAttrString(pPythonTraceback, "tb_lineno");
				if (pPythonLineNumber)
					sErrorDescription += String("Line ") + static_cast<int>(PyInt_AsLong(pPythonLineNumber)) + ' ';
			}

			// Get the error description string
			sErrorDescription += String('\"') + PyString_AS_STRING(pPythonString) + '\"';

			// Remove our reference from the Python string object
			Py_DECREF(pPythonString);
		}
		Py_XDECREF(pPythonErrorValue);
		Py_XDECREF(pPythonErrorType);
		Py_XDECREF(pPythonTraceback);
	}

	// Return the error description
	return sErrorDescription;
}

/**
*  @brief
*    Write a string into the log
*/
bool Script::LogOutputWithErrorDescription(uint8 nLogLevel, const String &sText, const String &sErrorDescription)
{
	// Call the base implementation
	return sErrorDescription.GetLength() ? Script::LogOutput(nLogLevel, sText + " (" + sErrorDescription + ')') : Script::LogOutput(nLogLevel, sText);
}

/**
*  @brief
*    Clears the script
*/
void Script::Clear()
{
	// Is there a Python module?
	if (m_pPythonModule) {

		// [TODO] Remove Python module, use "PyObject_Del()" for this purpose?

		// Reset the source code
		m_sSourceCode = "";

		// "m_pPythonModule", "m_pPythonDictionary" and "m_pPythonFunction" are borrowed references, don't use Py_DECREF on them
		m_pPythonModule     = nullptr;
		m_pPythonDictionary = nullptr;
		m_pPythonFunction   = nullptr;

		// Reset the current number of arguments
		m_nCurrentArgument = 0;

		// Remove our reference of from Python tuple
		if (m_pPythonTuple) {
			Py_DECREF(m_pPythonTuple);
			m_pPythonTuple = nullptr;
		}

		// Remove our reference of from Python function result
		if (m_pPythonFunctionResult) {
			Py_DECREF(m_pPythonFunctionResult);
			m_pPythonFunctionResult = nullptr;
		}

		// Destroy the Python table of functions
		if (m_pPythonTableOfFunctions) {
			delete [] m_pPythonTableOfFunctions;
			m_pPythonTableOfFunctions = nullptr;
		}
	}
}

/**
*  @brief
*    Increases the number of arguments
*/
void Script::IncreaseNumOfArguments()
{
	// Increase the number of arguments
	m_nCurrentArgument++;

	// Create/resize the Python tuple (results in new reference, use Py_DECREF on the result)
	if (m_pPythonTuple)
		_PyTuple_Resize(&m_pPythonTuple, m_nCurrentArgument);
	else
		m_pPythonTuple = PyTuple_New(m_nCurrentArgument);
}

/**
*  @brief
*    Adds a Python function
*/
bool Script::AddPythonFunction(PyObject *pPythonDictionary, const String &sFunction, PyObject *pPythonFunction, const String &sNamespace) const
{
	// Is the given namespace empty?
	if (sNamespace.GetLength()) {
		// Nope, the target namespace is not yet reached...

		// Find the next "." within the given nested Python namespace name
		int nPartEnd = sNamespace.IndexOf(".");
		if (nPartEnd < 0)
			nPartEnd = sNamespace.GetLength();

		// Get the current Python namespace name
		const String sSubsNamespaceName = sNamespace.GetSubstring(0, nPartEnd);

		// Is there already a Python dictionary object with the given name?
		// Request the Python dictionary (results in borrowed reference, don't use Py_DECREF on it)
		PyObject *pNestedPythonDictionary = PyDict_GetItemString(pPythonDictionary, sSubsNamespaceName);
		if (pNestedPythonDictionary) {
			// There's already an Python object with the given name - Must be a Python dictionary object...
			if (PyDict_Check(pNestedPythonDictionary)) {
				// Go down the rabbit hole...
				return AddPythonFunction(pNestedPythonDictionary, sFunction, pPythonFunction, sNamespace.GetSubstring(nPartEnd + 1));
			} else {
				// Error!
				return false;
			}
		} else {
			// Create the Python dictionary object (results in new reference, use Py_DECREF on the result)
			pNestedPythonDictionary = PyDict_New();

			// Add the new Python dictionary object to the current one
			if (PyDict_SetItemString(pPythonDictionary, sSubsNamespaceName, pNestedPythonDictionary) == -1) {
				// Remove our reference from the Python dictionary object
				Py_DECREF(pNestedPythonDictionary);

				// Error!
				return false;
			}

			// Go down the rabbit hole...
			const bool bResult = AddPythonFunction(pNestedPythonDictionary, sFunction, pPythonFunction, sNamespace.GetSubstring(nPartEnd + 1));

			// Remove our reference from the Python dictionary object
			Py_DECREF(pNestedPythonDictionary);

			// Done
			return bResult;
		}
	} else {
		// Jap, now add the given Python function object to the current Python dictionary object
		if (PyDict_SetItemString(pPythonDictionary, sFunction, pPythonFunction) == -1) {
			// Error!
			return false;
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptPython


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
