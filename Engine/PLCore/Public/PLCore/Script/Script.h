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


#ifndef __PLCORE_SCRIPT_H__
#define __PLCORE_SCRIPT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Object.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract script base class
*
*  @remarks
*    Each script should have the following properties:
*    - "Language": Script language (for example: "JavaScript" or "Lua")
*    - "Formats":  File format extensions this script can load in (for example: "js" or "lua")
*
*    Supported script features:
*    - Global variables (with namespace support)
*    - Global functions (with namespace support)
*      - C++ calls script
*      - Script calls C++
*    - RTTI objects
*      - Properties
*      - Attributes
*      - Methods
*      - Signals
*      - Slots
*
*    Supported primitive data types: bool, float, double, int8, int16, int32, int64, uint8, uint16, uint32, uint64, PLCore::Object*, PLCore::Object&
*    Please note that not each script language/API may make such a detailed data type distinction.
*    Because strings are fundamental within scripts, String is supported as well.
*/
class Script : public Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, Script, "PLCore", PLCore::Object, "Abstract script base class")
		// Properties
		pl_properties
			pl_property("Language",	"Unknown")
			pl_property("Formats",	"Unknown")
		pl_properties_end
		// Attributes
		pl_attribute(Name,	String,	"",	ReadWrite,	DirectValue,	"Name of this script, optional but recommended for better debugging",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Script();

		/**
		*  @brief
		*    Returns the name of the script language the script is using
		*
		*  @return
		*    The name of the script language the script is using (for example "Lua" or "JavaScript")
		*/
		PLCORE_API String GetScriptLanguage() const;

		/**
		*  @brief
		*    Returns a list of file formats this script supports
		*
		*  @param[out] lstFormats
		*    List of file formats this script supports (the given list is not cleared before new entries are added)
		*/
		PLCORE_API void GetFormats(Array<String> &lstFormats) const;

		/**
		*  @brief
		*    Adds a script binding to connect the given RTTI class instance with this script
		*
		*  @param[in] cObject
		*    RTTI class instance, must stay valid as long as this script lives
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @note
		*    - The added RTTI class instance methods will be available to the script as simple global functions
		*/
		PLCORE_API void AddBinding(Object &cObject, const String &sNamespace = "");

		/**
		*  @brief
		*    Add all script bindings to this script
		*
		*  @remarks
		*    Iterates over all available script binding instances and adds them to this script.
		*
		*  @note
		*    - The added RTTI class instance methods will be available to the script as simple global functions
		*/
		PLCORE_API void AddBindings();


	//[-------------------------------------------------------]
	//[ Public virtual Script functions                       ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether or not the given name belongs to a global function
		*
		*  @param[in] sName
		*    Name of the global function
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    'true' if the given name belongs to a global function, else 'false'
		*
		*  @remarks
		*    When calling a global script function, the script backend usually writes an error into the
		*    log when the given global script function wasn't found. So, when using optional global script
		*    functions, it's a good idea to check whether there's such a global script function by using "IsGlobalFunction()".
		*/
		virtual bool IsGlobalFunction(const String &sName, const String &sNamespace = "") = 0;

		/**
		*  @brief
		*    Adds a global function to the script
		*
		*  @param[in] sFunction
		*    Function name used inside the script to call the global function
		*  @param[in] cDynFunc
		*    Dynamic function
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe a script is already set?)
		*
		*  @note
		*    - If there's already a set script ("SetSourceCode()") this method will return an error
		*/
		virtual bool AddGlobalFunction(const String &sFunction, const DynFunc &cDynFunc, const String &sNamespace = "") = 0;

		/**
		*  @brief
		*    Removes all global functions from the script
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe a script is already set?)
		*
		*  @note
		*    - If there's already a set script ("SetSourceCode()") this method will return an error
		*/
		virtual bool RemoveAllGlobalFunctions() = 0;

		//[-------------------------------------------------------]
		//[ Script source code                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the script source code
		*
		*  @return
		*    The script source code
		*/
		virtual String GetSourceCode() const = 0;

		/**
		*  @brief
		*    Sets the script source code
		*
		*  @param[in] sSourceCode
		*    Script source code, usually blank ASCII code, empty string to set to script at all
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool SetSourceCode(const String &sSourceCode) = 0;

		/**
		*  @brief
		*    Returns a list of filenames associated with this script
		*
		*  @param[out] lstFilenames
		*    Receives a list of filenames associated with this script (list is not cleared before adding new entries)
		*
		*  @remarks
		*    For example Lua allows to use the keyword "require" to add the content of another script. This method
		*    returns a list of the filenames of the files which are included within this script. One can use this
		*    information to e.g. reload the script when the one of the associated files was changed, beside reloading
		*    when just the original script file was changed.
		*/
		virtual void GetAssociatedFilenames(Array<String> &lstFilenames) = 0;

		/**
		*  @brief
		*    Executes a given script source code string
		*
		*  @param[in] sSourceCode
		*    Script source code to execute, usually blank ASCII code
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Only works if there's already a valid script currently used
		*    - Lua example statement: "Speed=42"
		*/
		virtual bool Execute(const String &sSourceCode) = 0;

		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Adds the names of found global variables to a given list
		*
		*  @param[out] lstGlobalVariables
		*    List to be filled with the names (without namespace) of the found global variables, the given list is not cleared before new entries are added
		*  @param[in]  sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*/
		virtual void GetGlobalVariables(Array<String> &lstGlobalVariables, const String &sNamespace = "") = 0;

		/**
		*  @brief
		*    Returns whether or not the given name belongs to a global variable
		*
		*  @param[in] sName
		*    Name of the global variable
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    'true' if the given name belongs to a global variable, else 'false'
		*/
		virtual bool IsGlobalVariable(const String &sName, const String &sNamespace = "") = 0;

		/**
		*  @brief
		*    Returns the type ID a global variable
		*
		*  @param[in] sName
		*    Name of the global variable
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    The type ID of the global variable (e.g. "PLCore::TypeFloat" for "float") or "PLCore::TypeInvalid" on error
		*/
		virtual ETypeID GetGlobalVariableTypeID(const String &sName, const String &sNamespace = "") = 0;

		/**
		*  @brief
		*    Returns the current value of a global variable
		*
		*  @param[in] sName
		*    Name of the global variable
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    The current value of the global variable
		*/
		virtual String GetGlobalVariable(const String &sName, const String &sNamespace = "") = 0;

		/**
		*  @brief
		*    Sets the current value of a global variable
		*
		*  @param[in] sName
		*    Name of the global variable
		*  @param[in] cValue
		*    New value of the global variable
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @note
		*    - If there's no global variable with the given name, a new global variable is added to the script
		*    - Please note that it depends on the used script language/API which data types are really available,
		*      this means that "GetGlobalVariableTypeID()" may return another data type as the one you specified
		*/
		virtual void SetGlobalVariable(const String &sName, const DynVar &cValue, const String &sNamespace = "") = 0;

		//[-------------------------------------------------------]
		//[ Global function call, used by "FuncScriptPtr"         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Starts a function call
		*
		*  @param[in] sFunctionName
		*    Name of the function to call
		*  @param[in] sFunctionSignature
		*    Signature of the function to call (e.g. "void(int,float)")
		*  @param[in] sNamespace
		*    Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - It's not recommended to use this method directly, use "FuncScriptPtr" instead
		*
		*  @see
		*    - Have a look at "IsGlobalFunction()" for additional information
		*/
		virtual bool BeginCall(const String &sFunctionName, const String &sFunctionSignature, const String &sNamespace = "") = 0;

		/**
		*  @brief
		*    Pushes an argument required for the current function call
		*
		*  @param[in] nValue
		*    Argument value
		*
		*  @note
		*    - It's not recommended to use this method directly, use "FuncScriptPtr" instead
		*/
		virtual void PushArgument(bool bValue) = 0;
		virtual void PushArgument(float fValue) = 0;
		virtual void PushArgument(double fValue) = 0;
		virtual void PushArgument(int8 nValue) = 0;
		virtual void PushArgument(int16 nValue) = 0;
		virtual void PushArgument(int32 nValue) = 0;
		virtual void PushArgument(int64 nValue) = 0;
		virtual void PushArgument(uint8 nValue) = 0;
		virtual void PushArgument(uint16 nValue) = 0;
		virtual void PushArgument(uint32 nValue) = 0;
		virtual void PushArgument(uint64 nValue) = 0;
		virtual void PushArgument(const String &sString) = 0;
		virtual void PushArgument(Object *pObject) = 0;
		virtual void PushArgument(Object &cObject) = 0;

		/**
		*  @brief
		*    Ends a function call
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - It's not recommended to use this method directly, use "FuncScriptPtr" instead
		*    - This actually performs the prepared function call
		*/
		virtual bool EndCall() = 0;

		/**
		*  @brief
		*    Returns the result of a function call
		*
		*  @param[in] nValue
		*    Unused value... just there so the compiler can figure out the proper method
		*
		*  @return
		*    The result of a function call
		*
		*  @note
		*    - It's not recommended to use this method directly, use "FuncScriptPtr" instead
		*/
		virtual bool GetReturn(bool nValue) = 0;
		virtual float GetReturn(float nValue) = 0;
		virtual double GetReturn(double nValue) = 0;
		virtual int8 GetReturn(int8 nValue) = 0;
		virtual int16 GetReturn(int16 nValue) = 0;
		virtual int32 GetReturn(int32 nValue) = 0;
		virtual int64 GetReturn(int64 nValue) = 0;
		virtual uint8 GetReturn(uint8 nValue) = 0;
		virtual uint16 GetReturn(uint16 nValue) = 0;
		virtual uint32 GetReturn(uint32 nValue) = 0;
		virtual uint64 GetReturn(uint64 nValue) = 0;
		virtual String GetReturn(String nValue) = 0;
		virtual Object *GetReturn(Object *nValue) = 0;
		virtual Object &GetReturn(Object &nValue) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API Script();

		/**
		*  @brief
		*    Write a string into the log
		*
		*  @param[in] nLogLevel
		*    Log level
		*  @param[in] sText
		*    Text which should be written into the log
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    The text is written to the log only if the current
		*    log level is greater or equal to the specified value.
		*    This method is an extension of "Log::Output()"
		*    which also adds the name of the script to the given
		*    text.
		*/
		PLCORE_API bool LogOutput(uint8 nLogLevel, const String &sText);


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


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_SCRIPT_H__
