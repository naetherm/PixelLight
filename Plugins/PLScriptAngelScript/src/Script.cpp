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
#include <AngelScript/angelscript.h>
#include <PLCore/PLCore.h>
PL_WARNING_PUSH
PL_WARNING_DISABLE(4530) // "C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc"
	#include <../../../External/Optional/AngelScript/add_on/scriptstring/scriptstring.h>
PL_WARNING_POP
#include <PLCore/Log/Log.h>
#include "PLScriptAngelScript/AngelScriptContext.h"
#include "PLScriptAngelScript/Script.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptAngelScript {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Script, "PLScriptAngelScript", PLCore::Script, "AngelScript (http://www.angelcode.com/angelscript/) script implementation")
	// Properties
	pl_properties
		pl_property("Language",	"AngelScript")
		pl_property("Formats",	"as,AS")
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
	m_pAngelScriptEngine(nullptr),
	m_pAngelScriptModule(nullptr),
	m_pAngelScriptContext(nullptr),
	m_nCurrentArgument(0)
{
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

	// Release context reference
	if (m_pAngelScriptEngine)
		AngelScriptContext::ReleaseContextReference();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Script functions               ]
//[-------------------------------------------------------]
bool Script::IsGlobalFunction(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support
	if (sNamespace.GetLength())
		return false;	// Not implemented, yet. Get us out of here right now!

	// There must be a valid AngelScript engine and module instance
	return (m_pAngelScriptEngine && m_pAngelScriptModule && m_pAngelScriptModule->GetFunctionIdByName(sName) >= 0);
}

bool Script::AddGlobalFunction(const String &sFunction, const DynFunc &cDynFunc, const String &sNamespace)
{
	// Is there already a AngelScript engine instance?
	if (m_pAngelScriptEngine) {
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
	// Is there already a AngelScript engine instance?
	if (m_pAngelScriptEngine) {
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
		// Is there already a AngelScript engine instance?
		if (!m_pAngelScriptEngine)
			m_pAngelScriptEngine = AngelScriptContext::AddContextReference();

		// Get a AngelScript module instance
		if (m_pAngelScriptEngine) {
			// Add the global functions
			for (uint32 i=0; i<m_lstGlobalFunctions.GetNumOfElements(); i++) {
				// Get the global function
				GlobalFunction *psGlobalFunction = m_lstGlobalFunctions[i];

				// [TODO] It looks like that AngelScript (2.20.2) has currently no support for namespaces... so right now I'am doing
				// an ugly hack: e.g. "PL.Timing.GetTimeDifference()" is written within scripts as "PL_Timing_GetTimeDifference()"
				String sFunction;
				if (psGlobalFunction->sNamespace.GetLength()) {
					sFunction = psGlobalFunction->sNamespace;
					sFunction.Replace('.', '_');
					sFunction += '_';
					sFunction += psGlobalFunction->sFunction;
				} else {
					sFunction = psGlobalFunction->sFunction;
				}

				// Get the AngelScript function declaration
				String sFunctionDeclaration = GetAngelScriptFunctionDeclaration(sFunction, psGlobalFunction->pDynFunc->GetSignature(), true);

				// Register global AngelScript function
				const int nFunctionID = m_pAngelScriptEngine->RegisterGlobalFunction(sFunctionDeclaration, asFUNCTION(AngelScriptFunctionCallback), asCALL_GENERIC);
				if (nFunctionID < 0) {
					// Error!
					String sErrorDescription;
					switch (nFunctionID) {
						case asNOT_SUPPORTED:
							sErrorDescription = " (The calling convention is not supported)";
							break;

						case asWRONG_CALLING_CONV:
							sErrorDescription = " (The function's calling convention doesn't match callConv)";
							break;

						case asINVALID_DECLARATION:
							sErrorDescription = " (The function declaration is invalid)";
							break;

						case asNAME_TAKEN:
							sErrorDescription = " (The function name is already used elsewhere)";
							break;
					}
					LogOutput(Log::Error, "Failed to register the global AngelScript function '" + sFunctionDeclaration + '\'');
				} else {
					// Put a pointer to the global function into the user data of the registered AngelScript function
					asIScriptFunction *pAngelScriptFunction = m_pAngelScriptEngine->GetFunctionDescriptorById(nFunctionID);
					if (pAngelScriptFunction)
						pAngelScriptFunction->SetUserData(psGlobalFunction);
				}
			}

			// Get the AngelScript module
			m_pAngelScriptModule = m_pAngelScriptEngine->GetModule(AngelScriptContext::GetUniqueName(), asGM_ALWAYS_CREATE);
			if (m_pAngelScriptModule) {
				// Add script section
				if (m_pAngelScriptModule->AddScriptSection(Name.Get(), sSourceCode, sSourceCode.GetLength()) >= 0) {
					// Compile the script
					const int nResult = m_pAngelScriptModule->Build();
					if (nResult >= 0) {
						// Done
						return true;
					} else {
						// Error!
						String sErrorDescription;
						switch (nResult) {
							case asINVALID_CONFIGURATION:
								sErrorDescription = " (The AngelScript engine configuration is invalid)";
								break;

							case asERROR:
								sErrorDescription = " (The script failed to build)";
								break;

							case asBUILD_IN_PROGRESS:
								sErrorDescription = " (Another thread is currently building)";
								break;

							case asINIT_GLOBAL_VARS_FAILED:
								sErrorDescription = " (It was not possible to initialize at least one of the global variables)";
								break;
						}
						LogOutput(Log::Error, "Failed to compile the script" + sErrorDescription);
					}
				} else {
					// Error!
					LogOutput(Log::Error, "Failed to add script section");
				}
			}
		}
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

	// There must be a valid AngelScript engine and module instance
	if (m_pAngelScriptEngine && m_pAngelScriptModule) {
		// Get the number of global variables
		const int nNumOfGlobalVariables = m_pAngelScriptModule->GetGlobalVarCount();

		// Enumerate all global variables
		for (int i=0; i<nNumOfGlobalVariables; i++) {
			// Get the information about the global variable
			const char *pszGlobalVariableName = nullptr;
			if (m_pAngelScriptModule->GetGlobalVar(i, &pszGlobalVariableName, nullptr, nullptr) >= 0 && pszGlobalVariableName)
				lstGlobalVariables.Add(pszGlobalVariableName);
		}
	}
}

bool Script::IsGlobalVariable(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support

	return m_pAngelScriptModule ? (m_pAngelScriptModule->GetGlobalVarIndexByName(sName) >= 0) : false;
}

ETypeID Script::GetGlobalVariableTypeID(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support

	// There must be a valid AngelScript engine and module instance
	if (m_pAngelScriptEngine && m_pAngelScriptModule) {
		// Get the index of the global variable
		const int nGlobalVarIndex = m_pAngelScriptModule->GetGlobalVarIndexByName(sName);

		// Get the type of the global variable
		int nGlobalVarTypeID = asTYPEID_VOID;
		if (m_pAngelScriptModule->GetGlobalVar(nGlobalVarIndex, nullptr, &nGlobalVarTypeID, nullptr) >= 0) {
			// Process the global variable depending on it's type
			switch (nGlobalVarTypeID) {
				case asTYPEID_BOOL:		return TypeBool;
				case asTYPEID_INT8:		return TypeInt8;
				case asTYPEID_INT16:	return TypeInt16;
				case asTYPEID_INT32:	return TypeInt32;
				case asTYPEID_INT64:	return TypeInt64;
				case asTYPEID_UINT8:	return TypeUInt8;
				case asTYPEID_UINT16:	return TypeUInt16;
				case asTYPEID_UINT32:	return TypeUInt32;
				case asTYPEID_UINT64:	return TypeUInt64;
				case asTYPEID_FLOAT:	return TypeFloat;
				case asTYPEID_DOUBLE:	return TypeDouble;
				// [TODO] Add Object* support

				default:
					if (nGlobalVarTypeID == m_pAngelScriptEngine->GetTypeIdByDecl("string"))
						return TypeString;
					break;
			}
		}
	}

	// Error!
	return TypeInvalid;
}

String Script::GetGlobalVariable(const String &sName, const String &sNamespace)
{
	// [TODO] Add namespace support

	// There must be a valid AngelScript engine and module instance
	if (m_pAngelScriptEngine && m_pAngelScriptModule) {
		// Get the index of the global variable
		const int nGlobalVarIndex = m_pAngelScriptModule->GetGlobalVarIndexByName(sName);

		// Get the type of the global variable
		int nGlobalVarTypeID = asTYPEID_VOID;
		if (m_pAngelScriptModule->GetGlobalVar(nGlobalVarIndex, nullptr, &nGlobalVarTypeID, nullptr) >= 0) {
			// Get the address of the global variable
			void *pGlobalVarAddress = m_pAngelScriptModule->GetAddressOfGlobalVar(nGlobalVarIndex);
			if (pGlobalVarAddress) {
				// Process the global variable depending on it's type
				switch (nGlobalVarTypeID) {
					case asTYPEID_BOOL:		return *static_cast<bool*>  (pGlobalVarAddress);
					case asTYPEID_INT8:		return *static_cast<int8*>  (pGlobalVarAddress);
					case asTYPEID_INT16:	return *static_cast<int16*> (pGlobalVarAddress);
					case asTYPEID_INT32:	return *static_cast<int32*> (pGlobalVarAddress);
					case asTYPEID_INT64:	return *static_cast<int64*> (pGlobalVarAddress);
					case asTYPEID_UINT8:	return *static_cast<uint8*> (pGlobalVarAddress);
					case asTYPEID_UINT16:	return *static_cast<uint16*>(pGlobalVarAddress);
					case asTYPEID_UINT32:	return *static_cast<uint32*>(pGlobalVarAddress);
					case asTYPEID_UINT64:	return *static_cast<uint64*>(pGlobalVarAddress);
					case asTYPEID_FLOAT:	return *static_cast<float*> (pGlobalVarAddress);
					case asTYPEID_DOUBLE:	return *static_cast<double*>(pGlobalVarAddress);

					default:
						if (nGlobalVarTypeID == m_pAngelScriptEngine->GetTypeIdByDecl("string"))
							return static_cast<std::string*>(pGlobalVarAddress)->c_str();
						break;
				}
			}
		}
	}

	// Error!
	return "";
}

void Script::SetGlobalVariable(const String &sName, const DynVar &cValue, const String &sNamespace)
{
	// [TODO] Add namespace support

	// There must be a valid AngelScript engine and module instance
	if (m_pAngelScriptEngine && m_pAngelScriptModule) {
		// Get the index of the global variable
		const int nGlobalVarIndex = m_pAngelScriptModule->GetGlobalVarIndexByName(sName);

		// Get the type of the global variable because we don't want to change it's type
		int nGlobalVarTypeID = asTYPEID_VOID;
		if (m_pAngelScriptModule->GetGlobalVar(nGlobalVarIndex, nullptr, &nGlobalVarTypeID, nullptr) >= 0) {
			// Get the address of the global variable
			void *pGlobalVarAddress = m_pAngelScriptModule->GetAddressOfGlobalVar(nGlobalVarIndex);
			if (pGlobalVarAddress) {
				// Process the global variable depending on it's type
				switch (nGlobalVarTypeID) {
					case asTYPEID_BOOL:		*static_cast<bool*>  (pGlobalVarAddress) = cValue.GetBool();					break;
					case asTYPEID_INT8:		*static_cast<int8*>  (pGlobalVarAddress) = static_cast<int8>(cValue.GetInt());	break;
					case asTYPEID_INT16:	*static_cast<int16*> (pGlobalVarAddress) = static_cast<int16>(cValue.GetInt());	break;
					case asTYPEID_INT32:	*static_cast<int32*> (pGlobalVarAddress) = cValue.GetInt();						break;
					case asTYPEID_INT64:	*static_cast<int64*> (pGlobalVarAddress) = cValue.GetInt64();					break;
					case asTYPEID_UINT8:	*static_cast<uint8*> (pGlobalVarAddress) = cValue.GetUInt8();					break;
					case asTYPEID_UINT16:	*static_cast<uint16*>(pGlobalVarAddress) = cValue.GetUInt16();					break;
					case asTYPEID_UINT32:	*static_cast<uint32*>(pGlobalVarAddress) = cValue.GetUInt32();					break;
					case asTYPEID_UINT64:	*static_cast<uint64*>(pGlobalVarAddress) = cValue.GetUInt64();					break;
					case asTYPEID_FLOAT:	*static_cast<float*> (pGlobalVarAddress) = cValue.GetFloat();					break;
					case asTYPEID_DOUBLE:	*static_cast<double*>(pGlobalVarAddress) = cValue.GetDouble();					break;
					// [TODO] Add Object* support

					default:
						if (nGlobalVarTypeID == m_pAngelScriptEngine->GetTypeIdByDecl("string"))
							*static_cast<std::string*>(pGlobalVarAddress) = cValue.GetString();
						break;
				}
			}
		} else {
			// [TODO] Add new global variable
		}
	}
}

bool Script::BeginCall(const String &sFunctionName, const String &sFunctionSignature, const String &sNamespace)
{
	// [TODO] Add namespace support
	if (sNamespace.GetLength())
		return false;	// Error! Not implemented, yet. Get us out of here right now!

	// There must be a valid AngelScript engine and module instance
	if (m_pAngelScriptEngine && m_pAngelScriptModule) {
		// Is there already a context instance that will execute the script?
		if (!m_pAngelScriptContext) {
			m_pAngelScriptContext = m_pAngelScriptEngine->CreateContext();
			if (!m_pAngelScriptContext)
				LogOutput(Log::Error, "Failed to create the context");

			// [TODO] Add SetLineCallback
			//		cout << "Failed to set the line callback function." << endl;
		}
		if (m_pAngelScriptContext) {
			// Get the AngelScript function declaration
			const String sFunctionDeclaration = GetAngelScriptFunctionDeclaration(sFunctionName, sFunctionSignature, false);

			// Find the function ID for the function we want to execute
			const int nFunctionID = m_pAngelScriptModule->GetFunctionIdByDecl(sFunctionDeclaration);
			if (nFunctionID >= 0) {
				// Prepare the script context with the function we wish to execute
				int nResult = m_pAngelScriptContext->Prepare(nFunctionID);
				if (nResult >= 0) {
					// Current argument is 0
					m_nCurrentArgument = 0;

					// Done
					return true;
				} else {
					// Error!
					String sErrorDescription;
					switch (nResult) {
						case asCONTEXT_ACTIVE:
							// If your're here, you may have tried to call a script function by using PixelLights functors,
							// AngelScript can only run one function per script context call...
							sErrorDescription = " (The context is still active or suspended)";
							break;

						case asNO_FUNCTION:
							sErrorDescription = " (The function id doesn't exist)";
							break;
					}
					LogOutput(Log::Error, "Failed to prepare the context for the function '" + sFunctionDeclaration + "'" + sErrorDescription);
				}
			} else {
				// Error!
				LogOutput(Log::Error, "The function '" + sFunctionDeclaration + "' was not found");
			}
		}
	}

	// Error!
	return false;
}

void Script::PushArgument(bool bValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgByte(m_nCurrentArgument++, bValue);
}

void Script::PushArgument(float fValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgFloat(m_nCurrentArgument++, fValue);
}

void Script::PushArgument(double fValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgDouble(m_nCurrentArgument++, fValue);
}

void Script::PushArgument(int8 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgByte(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(int16 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgWord(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(int32 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgDWord(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(int64 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgQWord(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(uint8 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgByte(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(uint16 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgWord(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(uint32 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgDWord(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(uint64 nValue)
{
	if (m_pAngelScriptContext)
		m_pAngelScriptContext->SetArgQWord(m_nCurrentArgument++, nValue);
}

void Script::PushArgument(const String &sString)
{
	if (m_pAngelScriptContext) {
		CScriptString *pCScriptString = new CScriptString(sString.GetASCII(), sString.GetLength());
		m_pAngelScriptContext->SetArgObject(m_nCurrentArgument++, pCScriptString);
		pCScriptString->Release();	// Destroy our CScriptString instance
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
	if (m_pAngelScriptContext) {
		// Execute the function
		const int nResult = m_pAngelScriptContext->Execute();
		if (nResult == asEXECUTION_FINISHED) {
			// Done
			return true;
		} else {
			// The execution didn't finish as we had planned. Determine why.
			if (nResult == asEXECUTION_ABORTED) {
				LogOutput(Log::Error, "The script was aborted before it could finish. Probably it timed out.");
			} else if (nResult == asEXECUTION_EXCEPTION) {
				LogOutput(Log::Error, "The script ended with an exception");

				// Write some information about the script exception
				const int nExceptionFunctionID = m_pAngelScriptContext->GetExceptionFunction();
				asIScriptFunction *pAngelScriptFunction = m_pAngelScriptEngine->GetFunctionDescriptorById(nExceptionFunctionID);
				String sErrorDescription;
				sErrorDescription += String("Function: ")		+ pAngelScriptFunction->GetDeclaration()			+ '\n';
				sErrorDescription += String("Module: ")			+ pAngelScriptFunction->GetModuleName()				+ '\n';
				sErrorDescription += String("Section: ")		+ pAngelScriptFunction->GetScriptSectionName()		+ '\n';
				sErrorDescription += String("Line: ")			+ m_pAngelScriptContext->GetExceptionLineNumber()	+ '\n';
				sErrorDescription += String("Description: ")	+ m_pAngelScriptContext->GetExceptionString()		+ '\n';
				LogOutput(Log::Error, sErrorDescription);
			} else {
				LogOutput(Log::Error, String("The script ended for some unforeseen reason (")  + nResult + ")");
			}
		}
	}

	// Error!
	return false;
}

bool Script::GetReturn(bool nValue)
{
	return m_pAngelScriptContext ? (m_pAngelScriptContext->GetReturnByte() != 0) : false;
}

float Script::GetReturn(float nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnFloat() : 0;
}

double Script::GetReturn(double nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnDouble() : 0;
}

int8 Script::GetReturn(int8 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnByte() : 0;
}

int16 Script::GetReturn(int16 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnWord() : 0;
}

int32 Script::GetReturn(int32 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnDWord() : 0;
}

int64 Script::GetReturn(int64 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnQWord() : 0;
}

uint8 Script::GetReturn(uint8 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnByte() : 0;
}

uint16 Script::GetReturn(uint16 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnWord() : 0;
}

uint32 Script::GetReturn(uint32 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnDWord() : 0;
}

uint64 Script::GetReturn(uint64 nValue)
{
	return m_pAngelScriptContext ? m_pAngelScriptContext->GetReturnQWord() : 0;
}

String Script::GetReturn(String sValue)
{
	// There must be a valid AngelScript engine and context instance
	if (m_pAngelScriptEngine && m_pAngelScriptContext) {
		// Get the current AngelScript function and check the function return type
		asIScriptFunction *pAngelScriptFunction = m_pAngelScriptContext->GetFunction();
		if (pAngelScriptFunction && pAngelScriptFunction->GetReturnTypeId() == m_pAngelScriptEngine->GetTypeIdByDecl("string"))
			return m_pAngelScriptContext ? static_cast<std::string*>(m_pAngelScriptContext->GetReturnObject())->c_str() : "";
	}

	// Error!
	return "";
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
//[ Private static AngelScript callback functions         ]
//[-------------------------------------------------------]
/*
*  @brief
*    AngelScript function callback
*/
void Script::AngelScriptFunctionCallback(asIScriptGeneric *pAngelScriptGeneric)
{
	// Get the global function
	GlobalFunction *psGlobalFunction = reinterpret_cast<GlobalFunction*>(pAngelScriptGeneric->GetFunctionUserData());

	// Get the number of arguments AngelScript gave to us and transform the arguments into a functor parameters string
	String sParams;
	const int nNumOfArguments = pAngelScriptGeneric->GetArgCount();
	for (int i=0; i<nNumOfArguments; i++) {
		String sValue;
		const int nAngelScriptArgTypeId = pAngelScriptGeneric->GetArgTypeId(i);
		switch (nAngelScriptArgTypeId) {
			case asTYPEID_BOOL:		sValue = pAngelScriptGeneric->GetArgByte(i);						break;
			case asTYPEID_INT8:		sValue = pAngelScriptGeneric->GetArgByte(i);						break;
			case asTYPEID_INT16:	sValue = static_cast<int16>(pAngelScriptGeneric->GetArgWord(i));	break;
			case asTYPEID_INT32:	sValue = static_cast<int32>(pAngelScriptGeneric->GetArgDWord(i));	break;
			case asTYPEID_INT64:	sValue = pAngelScriptGeneric->GetArgQWord(i);						break;
			case asTYPEID_UINT8:	sValue = pAngelScriptGeneric->GetArgByte(i);						break;
			case asTYPEID_UINT16:	sValue = static_cast<uint32>(pAngelScriptGeneric->GetArgWord(i));	break;
			case asTYPEID_UINT32:	sValue = static_cast<uint32>(pAngelScriptGeneric->GetArgDWord(i));	break;
			case asTYPEID_UINT64:	sValue = pAngelScriptGeneric->GetArgQWord(i);						break;
			case asTYPEID_FLOAT:	sValue = pAngelScriptGeneric->GetArgFloat(i);						break;
			case asTYPEID_DOUBLE:	sValue = pAngelScriptGeneric->GetArgDouble(i);						break;
			default:
				// Is it a string?
				if (nAngelScriptArgTypeId == pAngelScriptGeneric->GetEngine()->GetTypeIdByDecl("string"))
					sValue = static_cast<std::string*>(pAngelScriptGeneric->GetArgObject(i))->c_str();
				else
					; // Do nothing
				break;
		}

		// Add to functor parameters
		sParams += String("Param") + i + "=\"" + sValue + "\" ";
	}

	// Call the functor
	const String sReturn = psGlobalFunction->pDynFunc->CallWithReturn(sParams);

	// Process the functor return
	if (sReturn.GetLength()) {
		switch (psGlobalFunction->pDynFunc->GetReturnTypeID()) {
			case TypeBool:		pAngelScriptGeneric->SetReturnByte(sReturn.GetBool());												break;
			case TypeDouble:	pAngelScriptGeneric->SetReturnDouble(sReturn.GetDouble());											break;
			case TypeFloat:		pAngelScriptGeneric->SetReturnFloat(sReturn.GetFloat());											break;
			case TypeInt:		pAngelScriptGeneric->SetReturnDWord(sReturn.GetInt());												break;
			case TypeInt16:		pAngelScriptGeneric->SetReturnWord(static_cast<asWORD>(sReturn.GetInt()));							break;
			case TypeInt32:		pAngelScriptGeneric->SetReturnDWord(sReturn.GetInt());												break;
			case TypeInt64:		pAngelScriptGeneric->SetReturnQWord(sReturn.GetInt64());											break;
			case TypeInt8:		pAngelScriptGeneric->SetReturnByte(static_cast<asBYTE>(sReturn.GetInt()));							break;
			case TypeString:	pAngelScriptGeneric->SetReturnAddress(new CScriptString(sReturn.GetASCII(), sReturn.GetLength()));	break;	// AngelScript takes over the control of the allocated object
			case TypeUInt16:	pAngelScriptGeneric->SetReturnWord(sReturn.GetUInt16());											break;
			case TypeUInt32:	pAngelScriptGeneric->SetReturnDWord(sReturn.GetUInt32());											break;
			case TypeUInt64:	pAngelScriptGeneric->SetReturnQWord(sReturn.GetUInt64());											break;
			case TypeUInt8:		pAngelScriptGeneric->SetReturnByte(sReturn.GetUInt8());												break;
			default:																												break;
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Script::Script(const Script &cSource) :
	m_pAngelScriptEngine(nullptr),
	m_pAngelScriptModule(nullptr),
	m_pAngelScriptContext(nullptr),
	m_nCurrentArgument(0)
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
*    Clears the script
*/
void Script::Clear()
{
	// Reset the source code
	m_sSourceCode = "";

	// Release the AngelScript context instance
	if (m_pAngelScriptContext) {
		m_pAngelScriptContext->Release();
		m_pAngelScriptContext = nullptr;
	}

	// Destroy the AngelScript module instance
	if (m_pAngelScriptModule) {
		m_pAngelScriptModule->GetEngine()->DiscardModule(m_pAngelScriptModule->GetName());
		m_pAngelScriptModule = nullptr;
	}

	// Reset the current argument
	m_nCurrentArgument = 0;
}

/**
*  @brief
*    Gets a AngelScript function declaration
*/
String Script::GetAngelScriptFunctionDeclaration(const String &sFunctionName, const String &sFunctionSignature, bool bCppToAngelScript) const
{
	// Start with the PixelLight function signature (e.g. "void(int,float)")
	String sFunctionDeclaration = sFunctionSignature;

	// Find the index of the "("
	int nIndex = sFunctionDeclaration.IndexOf("(");
	if (nIndex > -1) {

		// [HACK] AngelScript really don't like something like "string MyFunction(string)", it want's "string @MyFunction(const string &)"!
		// I assume that "@" means "AngelScript, take over the control of the given memory". I wasn't able to find the documentation about
		// the AngelScript function declaration syntax, just "scriptstring.cpp" as example.
		if (bCppToAngelScript && sFunctionDeclaration.IndexOf("string") > -1) {
			String sParameters = sFunctionDeclaration.GetSubstring(nIndex);	// Find the parameters part in the string
			sParameters.Replace("string", "const string &");				// Change parameters
			sFunctionDeclaration.Delete(nIndex);							// Remove parameters from original function declaration
			sFunctionDeclaration.Replace("string", "string @");				// Change return
			sFunctionDeclaration += sParameters;							// Construct new function declaration
			nIndex = sFunctionDeclaration.IndexOf("(");						// Update the "(" index
		}

		// Create the AngelScript function declaration (e.g. "void MyFunction(int,float)")
		sFunctionDeclaration.Insert(' ' + sFunctionName, nIndex);
	}

	// Return the AngelScript function declaration (e.g. "void MyFunction(int,float)")
	return sFunctionDeclaration;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptAngelScript
