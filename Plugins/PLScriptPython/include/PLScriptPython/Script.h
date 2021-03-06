/*********************************************************\
 *  File: Script.h                                       *
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


#ifndef __PLSCRIPTPYTHON_SCRIPT_H__
#define __PLSCRIPTPYTHON_SCRIPT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScriptPython/Python.h"
#include <PLCore/Script/Script.h>
#include "PLScriptPython/PLScriptPython.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptPython {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Python (http://www.python.org/) script implementation
*/
class Script : public PLCore::Script {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLSCRIPTPYTHON_API)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLSCRIPTPYTHON_API Script();

		/**
		*  @brief
		*    Destructor
		*/
		PLSCRIPTPYTHON_API virtual ~Script();


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Script functions               ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		PLSCRIPTPYTHON_API virtual bool IsGlobalFunction(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTPYTHON_API virtual bool AddGlobalFunction(const PLCore::String &sFunction, const PLCore::DynFunc &cDynFunc, const PLCore::String &sNamespace = "") override;
		PLSCRIPTPYTHON_API virtual bool RemoveAllGlobalFunctions() override;

		//[-------------------------------------------------------]
		//[ Script source code                                    ]
		//[-------------------------------------------------------]
		PLSCRIPTPYTHON_API virtual PLCore::String GetSourceCode() const override;
		PLSCRIPTPYTHON_API virtual bool SetSourceCode(const PLCore::String &sSourceCode) override;
		PLSCRIPTPYTHON_API virtual void GetAssociatedFilenames(PLCore::Array<PLCore::String> &lstFilenames) override;
		PLSCRIPTPYTHON_API virtual bool Execute(const PLCore::String &sSourceCode) override;

		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		PLSCRIPTPYTHON_API virtual void GetGlobalVariables(PLCore::Array<PLCore::String> &lstGlobalVariables, const PLCore::String &sNamespace = "") override;
		PLSCRIPTPYTHON_API virtual bool IsGlobalVariable(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTPYTHON_API virtual PLCore::ETypeID GetGlobalVariableTypeID(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTPYTHON_API virtual PLCore::String GetGlobalVariable(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTPYTHON_API virtual void SetGlobalVariable(const PLCore::String &sName, const PLCore::DynVar &cValue, const PLCore::String &sNamespace = "") override;

		//[-------------------------------------------------------]
		//[ Global function call, used by "FuncScriptPtr"         ]
		//[-------------------------------------------------------]
		PLSCRIPTPYTHON_API virtual bool BeginCall(const PLCore::String &sFunctionName, const PLCore::String &sFunctionSignature, const PLCore::String &sNamespace = "") override;
		PLSCRIPTPYTHON_API virtual void PushArgument(bool bValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(float fValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(double fValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::int8 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::int16 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::int32 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::int64 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::uint8 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::uint16 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::uint32 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::uint64 nValue) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(const PLCore::String &sString) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::Object *pObject) override;
		PLSCRIPTPYTHON_API virtual void PushArgument(PLCore::Object &cObject) override;
		PLSCRIPTPYTHON_API virtual bool EndCall() override;
		PLSCRIPTPYTHON_API virtual bool GetReturn(bool nValue) override;
		PLSCRIPTPYTHON_API virtual float GetReturn(float nValue) override;
		PLSCRIPTPYTHON_API virtual double GetReturn(double nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::int8 GetReturn(PLCore::int8 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::int16 GetReturn(PLCore::int16 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::int32 GetReturn(PLCore::int32 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::int64 GetReturn(PLCore::int64 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::uint8 GetReturn(PLCore::uint8 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::uint16 GetReturn(PLCore::uint16 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::uint32 GetReturn(PLCore::uint32 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::uint64 GetReturn(PLCore::uint64 nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::String GetReturn(PLCore::String nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::Object *GetReturn(PLCore::Object *nValue) override;
		PLSCRIPTPYTHON_API virtual PLCore::Object &GetReturn(PLCore::Object &nValue) override;


	//[-------------------------------------------------------]
	//[ Private static Python callback functions              ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @brief
		*    Python function callback
		*
		*  @param[in] pPythonSelf
		*    Python self object
		*  @param[in] pPythonArguments
		*    Python tuple object representing all arguments
		*
		*  @return
		*    Python result object, null pointer if an exception was set
		*/
		static PyObject *PythonFunctionCallback(PyObject *pPythonSelf, PyObject *pPythonArguments);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Script(const Script &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Script &operator =(const Script &cSource);

		/**
		*  @brief
		*    Returns a string containing the Python error description
		*
		*  @return
		*    A string containing the Python error description, empty string if there
		*    wasn't an error or there was an error in receiving the error description
		*/
		PLCore::String GetPythonErrorDescription() const;

		/**
		*  @brief
		*    Write a string into the log
		*
		*  @param[in] nLogLevel
		*    Log level
		*  @param[in] sText
		*    Text which should be written into the log
		*  @param[in] sErrorDescription
		*    Error description
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    The text is written to the log only if the current
		*    log level is greater or equal to the specified value.
		*    This method is an extension of "PLCore::Log::Output()"
		*    which also adds the name of the script to the given
		*    text and the "sErrorDescription" content.
		*/
		bool LogOutputWithErrorDescription(PLCore::uint8 nLogLevel, const PLCore::String &sText, const PLCore::String &sErrorDescription);

		/**
		*  @brief
		*    Clears the script
		*/
		void Clear();

		/**
		*  @brief
		*    Increases the number of arguments
		*/
		void IncreaseNumOfArguments();

		/**
		*  @brief
		*    Adds a Python function
		*
		*  @param[in] pPythonDictionary
		*    Python dictionary were to add the function to
		*  @param[in] sFunction
		*    Name of the function
		*  @param[in] pPythonFunction
		*    Python function object
		*  @param[in] sNamespace
		*    Namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool AddPythonFunction(PyObject *pPythonDictionary, const PLCore::String &sFunction, PyObject *pPythonFunction, const PLCore::String &sNamespace) const;


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    A global function
		*/
		struct GlobalFunction {
			PLCore::String   sFunction;		/**< Function name used inside the script to call the global function */
			PLCore::DynFunc *pDynFunc;		/**< Dynamic function to be called, always valid, destroy when done */
			PLCore::String   sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String					m_sSourceCode;				/**< Script source code */
		PyObject					   *m_pPythonModule;			/**< Python module, can be a null pointer (borrowed reference, don't use Py_DECREF on it) */
		PyObject					   *m_pPythonDictionary;		/**< Python dictionary of the module, can be a null pointer (borrowed reference, don't use Py_DECREF on it) */
		PyObject					   *m_pPythonFunction;			/**< Current Python function, can be a null pointer (borrowed reference, don't use Py_DECREF on it) */
		PyObject					   *m_pPythonTuple;				/**< Python tuple, used during function call, can be a null pointer (own reference, use Py_DECREF on it) */
		PLCore::uint32					m_nCurrentArgument;			/**< Current argument, used during function call */
		PyObject					   *m_pPythonFunctionResult;	/**< Python function result, used during function call, can be a null pointer (own reference, use Py_DECREF on it) */
		PLCore::Array<GlobalFunction*>  m_lstGlobalFunctions;		/**< List of global functions */
		PyMethodDef					   *m_pPythonTableOfFunctions;	/**< Python table of functions, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptPython


#endif // __PLSCRIPTPYTHON_SCRIPT_H__
