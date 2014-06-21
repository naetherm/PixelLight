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
extern "C" {
	#include <Lua/lua.h>
	#include <Lua/lualib.h>
	#include <Lua/lauxlib.h>
}
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>
#include "PLScriptLua/LuaContext.h"
#include "PLScriptLua/RTTIObjectPointer.h"
#include "PLScriptLua/RTTIObjectMethodPointer.h"
#include "PLScriptLua/Script.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Script)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Script::Script() :
	m_pLuaState(nullptr),
	m_bFunctionResult(false),
	m_nCurrentArgument(0)
{
	// Add a context reference
	LuaContext::AddContextReference();
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
	LuaContext::ReleaseContextReference();
}

/**
*  @brief
*    Returns the Lua state
*/
lua_State *Script::GetLuaState() const
{
	return m_pLuaState;
}

/**
*  @brief
*    Writes the current Lua stack content into the log
*/
void Script::LuaStackDump()
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Get the number of elements on the Lua stack
		const int nNumOfStackElements = lua_gettop(m_pLuaState);

		// Write this number into the log
		LogOutput(Log::Info, String("Number of elements on the Lua stack: ") + nNumOfStackElements);

		// Iterate through the Lua stack
		for (int i=1; i<=nNumOfStackElements; i++) {
			const int nLuaType = lua_type(m_pLuaState, i);
			String sValue;
			switch (nLuaType) {
				case LUA_TNIL:
					sValue = "nil";
					break;

				case LUA_TNUMBER:
					sValue = lua_tonumber(m_pLuaState, i);
					break;

				case LUA_TBOOLEAN:
					sValue = lua_toboolean(m_pLuaState, i) ? "true" : "false";
					break;

				case LUA_TSTRING:
					sValue = lua_tostring(m_pLuaState, i);
					break;

				case LUA_TTABLE:
					sValue = "Table";
					break;

				case LUA_TFUNCTION:
					sValue = lua_iscfunction(m_pLuaState, i) ? "C-function" : "Lua-function";
					break;

				case LUA_TUSERDATA:
					sValue = "User data";
					break;

				case LUA_TTHREAD:
					sValue = "Thread";
					break;

				case LUA_TLIGHTUSERDATA:
					sValue = "Light user data";
					break;

				default:
					sValue = "?";
					break;
			}
			LogOutput(Log::Info, String("Lua stack element ") + (i-1) + ": \"" + sValue + "\" (Lua type name: \"" + lua_typename(m_pLuaState, nLuaType) + "\")");
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Script functions               ]
//[-------------------------------------------------------]
bool Script::IsGlobalFunction(const String &sName, const String &sNamespace)
{
	bool bIsGlobalFunction = false;	// By default, the global script function does not exist

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && GetNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the function name onto the Lua stack

		// This function pops the key from the stack - this gets the function
		lua_gettable(m_pLuaState, -2);

		// Is it a function?
		bIsGlobalFunction = lua_isfunction(m_pLuaState, -1);

		// Pop the function from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return bIsGlobalFunction;
}

bool Script::AddGlobalFunction(const String &sFunction, const DynFunc &cDynFunc, const String &sNamespace)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Error!
		return false;
	} else {
		// Add the global function
		GlobalFunction *psGlobalFunction = new GlobalFunction;
		psGlobalFunction->pScript	 = this;
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
	// Is there a Lua state?
	if (m_pLuaState) {
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
		// Create the Lua state
		m_pLuaState = lua_open();
		if (m_pLuaState) {
			// Open all standard Lua libraries into the given state
			luaL_openlibs(m_pLuaState);

			// Create the metatable for LuaUserData events
			LuaUserData::CreateMetatable(m_pLuaState);

			// Add the global functions
			for (uint32 i=0; i<m_lstGlobalFunctions.GetNumOfElements(); i++) {
				// Get the global function
				GlobalFunction *psGlobalFunction = m_lstGlobalFunctions[i];

				// Create a nested Lua table
				if (CreateNestedTable(psGlobalFunction->sNamespace)) {
					// Table key
					lua_pushstring(m_pLuaState, psGlobalFunction->sFunction);		// Push the function name onto the Lua stack

					// Table value: Store a pointer to the global function in the c-closure
					lua_pushlightuserdata(m_pLuaState, psGlobalFunction);			// Push a pointer to the global function onto the Lua stack
					lua_pushcclosure(m_pLuaState, &Script::LuaFunctionCallback, 1);	// Push the function pointer onto the Lua stack

					// This function pops both the key and the value from the stack
					lua_settable(m_pLuaState, -3);

					// Pop the table from the Lua stack
					lua_pop(m_pLuaState, 1);
				}
			}

			// Load the script
			const int nResult = luaL_loadbuffer(m_pLuaState, sSourceCode, sSourceCode.GetLength(), Name.Get());
			if (!nResult && !lua_pcall(m_pLuaState, 0, 0, 0)) {
				// Done
				return true;
			} else {
				// Error!

				// Write a log message
				String sErrorDescription;
				switch (nResult) {
					case LUA_ERRSYNTAX:
						sErrorDescription = " (Syntax error during pre-compilation)";
						break;

					case LUA_ERRMEM:
						sErrorDescription = " (Memory allocation error)";
						break;
				}
				LogOutput(Log::Error, "Failed to compile the script" + sErrorDescription);

				// Report Lua errors
				ReportErrors();
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
	// We want to have a list of script filenames which were included within this script
	// -> It appears that there's no "easy" way in Lua to get this kind of information :/

	// Contains "Application", "Interaction" and so on (no final filenames)
	Array<String> lstRequire;

	// Get a list of loaded "require"-files
	{ // -> The files loaded within a Lua script by using "require" can be accessed by using the
		//    global control table variable "_LOADED". See http://www.lua.org/pil/8.1.html for details.
		lua_getfield(m_pLuaState, LUA_REGISTRYINDEX, "_LOADED");
		if (lua_istable(m_pLuaState, -1)) {
			lua_pushnil(m_pLuaState);
			while (lua_next(m_pLuaState, -2)) {
				if (lua_isstring(m_pLuaState, -2))
					lstRequire.Add(lua_tostring(m_pLuaState, -2));
				lua_pop(m_pLuaState, 1);
			}
		}

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Get the content of "package.path" used by "require" to search for a Lua loader
	// -> The content looks like "?.lua;C:\SomePath\?.lua;"
	const String sPackagePath = GetGlobalVariable("path", "package");

	// Iterate over the "require"-list
	const String sToReplace = "?.";
	for (uint32 i=0; i<lstRequire.GetNumOfElements(); i++) {
		// Get the current "require"
		const String sRequire = lstRequire[i] + '.';

		// Get the index of the first ";" within the package path
		int nPreviousIndex = 0;
		int nIndex = sPackagePath.IndexOf(';');
		while (nIndex>-1) {
			// Get current package search path, we now have e.g. "C:\SomePath\?.lua"
			String sFilename = sPackagePath.GetSubstring(nPreviousIndex, nIndex-nPreviousIndex);

			// Replace "?." with the "require"-name
			sFilename.Replace(sToReplace, sRequire);

			// Does this file exist?
			if (File(sFilename).Exists()) {
				// We found a match!
				lstFilenames.Add(sFilename);

				// Get us out of the while-loop
				nIndex = -1;
			} else {
				// Get the index of the next ";" within the package path
				nPreviousIndex = nIndex + 1;
				nIndex = sPackagePath.IndexOf(';', nPreviousIndex);
			}
		}
	}
}

bool Script::Execute(const String &sSourceCode)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Execute
		const bool bResult = !luaL_dostring(m_pLuaState, sSourceCode.GetASCII());

		// -> For now we don't want to bother the script API user with something
		//    like garbage collection, so do this automatically
		// -> If this is not done, reuse of "RTTIObjectPointer"-instances (as well as derived classes of course)
		//    will not work that well because unused stuff will not be removed automatically by Lua when not asking
		//    Lua to do so
		lua_gc(m_pLuaState, LUA_GCSTEP, 1);

		// Done
		return bResult;
	}

	// Error!
	return false;
}

void Script::GetGlobalVariables(Array<String> &lstGlobalVariables, const String &sNamespace)
{
	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && GetNestedTable(sNamespace)) {
		// Push the first key onto the Lua stack
		lua_pushnil(m_pLuaState);

		// Iterate through the Lua table
		while (lua_next(m_pLuaState, 1) != 0) {
			// Lua stack content: The 'key' is at index -2 and the 'value' at index -1

			// Check the 'key' type (at index -2) - must be a string
			if (lua_isstring(m_pLuaState, -2)) {
				// Check whether or not the 'value' (at index -1) is a global variable
				// (something like "_VERSION" is passing this test as well, but that's probably ok because it's just a Lua build in global variable)
				if (lua_isnumber(m_pLuaState, -1) || lua_isstring(m_pLuaState, -1)) {
					// Add the global variable to our list
					lstGlobalVariables.Add(lua_tostring(m_pLuaState, -2));
				}
			}

			// Next, please (removes 'value'; keeps 'key' for next iteration)
			lua_pop(m_pLuaState, 1);
		}

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}
}

bool Script::IsGlobalVariable(const String &sName, const String &sNamespace)
{
	bool bGlobalVariable = false;

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && GetNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

		// This function pops the key from the stack - this gets the variable
		lua_gettable(m_pLuaState, -2);

		// Check the type of the variable
		bGlobalVariable = (lua_isnumber(m_pLuaState, -1) || lua_isstring(m_pLuaState, -1));

		// Pop the variable from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return bGlobalVariable;
}

ETypeID Script::GetGlobalVariableTypeID(const String &sName, const String &sNamespace)
{
	ETypeID nType = TypeInvalid;

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && GetNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

		// This function pops the key from the stack - this gets the variable
		lua_gettable(m_pLuaState, -2);

		// Check the type of the variable
		if (lua_isboolean(m_pLuaState, -1))
			nType = TypeBool;
		else if (lua_isnumber(m_pLuaState, -1))
			nType = TypeDouble;
		else if (lua_isstring(m_pLuaState, -1))
			nType = TypeString;
		else if (lua_isuserdata(m_pLuaState, -1))
			nType = TypeObjectPtr;	// [TODO] Do any type tests in here?

		// Pop the variable from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return nType;
}

String Script::GetGlobalVariable(const String &sName, const String &sNamespace)
{
	String sValue;

	// Is there a Lua state? If so, get a nested Lua table
	if (m_pLuaState && GetNestedTable(sNamespace)) {
		// Table key
		lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

		// This function pops the key from the stack - this gets the variable
		lua_gettable(m_pLuaState, -2);

		// Get the value of the variable as string
		sValue = lua_tostring(m_pLuaState, -1);

		// Pop the variable from the Lua state stack
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, 1);
	}

	// Done
	return sValue;
}

void Script::SetGlobalVariable(const String &sName, const DynVar &cValue, const String &sNamespace)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Get the type of the variable because we don't want to change it's type
		int nTypeID = GetGlobalVariableTypeID(sName);
		if (nTypeID == TypeInvalid) {
			// Ok, this must be a new variable
			nTypeID = cValue.GetTypeID();
		}

		// Create a nested Lua table
		if (CreateNestedTable(sNamespace)) {
			// Table key
			lua_pushstring(m_pLuaState, sName);	// Push the variable name onto the Lua stack

			// Table value: Push the value of the variable onto the Lua stack
			switch (nTypeID) {
				case TypeVoid:																								return;	// ? Yeah, that's really funny!
				case TypeBool:		lua_pushboolean(m_pLuaState, cValue.GetBool());											break;
				case TypeDouble:	lua_pushnumber (m_pLuaState, cValue.GetDouble());										break;
				case TypeFloat:		lua_pushnumber (m_pLuaState, cValue.GetFloat());										break;
				case TypeInt:		lua_pushinteger(m_pLuaState, cValue.GetInt());											break;
				case TypeInt16:		lua_pushinteger(m_pLuaState, cValue.GetInt());											break;
				case TypeInt32:		lua_pushinteger(m_pLuaState, cValue.GetInt());											break;
				case TypeInt64:		lua_pushinteger(m_pLuaState, cValue.GetInt());											break;	// [TODO] TypeInt64 is currently handled just as long
				case TypeInt8:		lua_pushinteger(m_pLuaState, cValue.GetInt());											break;
				case TypeString:	lua_pushstring (m_pLuaState, cValue.GetString());										break;
				case TypeUInt16:	lua_pushinteger(m_pLuaState, cValue.GetUInt16());										break;
				case TypeUInt32:	lua_pushinteger(m_pLuaState, cValue.GetUInt32());										break;
				case TypeUInt64:	lua_pushinteger(m_pLuaState, static_cast<lua_Integer>(cValue.GetUInt64()));				break;	// [TODO] TypeUInt64 is currently handled just as long
				case TypeUInt8:		lua_pushinteger(m_pLuaState, cValue.GetUInt8());										break;

				// [HACK] Currently, classes derived from "PLCore::Object" are just recognized as type "void*"... but "PLCore::Object*" type would be perfect
				case TypePtr:
				case TypeObjectPtr:	RTTIObjectPointer::LuaStackPush(*this, reinterpret_cast<Object*>(cValue.GetUIntPtr()));	break;

				default:			lua_pushstring (m_pLuaState, cValue.GetString());										break;	// Unknown type
			}

			// This function pops both the key and the value from the stack - this sets the variable
			lua_settable(m_pLuaState, -3);

			// Pop the table from the Lua stack
			lua_pop(m_pLuaState, 1);
		}
	}
}

bool Script::BeginCall(const String &sFunctionName, const String &sFunctionSignature, const String &sNamespace)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Get a nested Lua table
		if (GetNestedTable(sNamespace)) {
			// Table key
			lua_pushstring(m_pLuaState, sFunctionName);	// Push the function name onto the Lua stack

			// Push the function to be called onto the Lua state stack
			lua_gettable(m_pLuaState, -2);
			if (lua_isfunction(m_pLuaState, -1)) {
				// Backup the name of the current function (we may need it for error log output)
				m_sCurrentFunction = sFunctionName;

				// Has the current function a result?
				m_bFunctionResult = !sFunctionSignature.Compare("void(", 0, 5);

				// Current argument is 0
				m_nCurrentArgument = 0;

				// Done
				return true;
			} else {
				// Error!
				if (sNamespace.GetLength())
					LogOutput(Log::Error, "The function '" + sFunctionName + "' was not found within the namespace '" + sNamespace + '\'');
				else
					LogOutput(Log::Error, "The function '" + sFunctionName + "' was not found");
			}
		} else {
			// Error!
			LogOutput(Log::Error, "The namespace '" + sNamespace + "' of the function '" + sFunctionName + "' was not found");
		}
	}

	// Error!
	return false;
}

void Script::PushArgument(bool bValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushboolean(m_pLuaState, bValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(float fValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushnumber(m_pLuaState, fValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(double fValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushnumber(m_pLuaState, fValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(int8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(int16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(int32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(int64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// [TODO] There's no int64 support in Lua (?)
		lua_pushinteger(m_pLuaState, static_cast<lua_Integer>(nValue));
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(uint8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(uint16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(uint32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushinteger(m_pLuaState, nValue);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(uint64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// [TODO] There's no uint64 support in Lua (?)
		lua_pushinteger(m_pLuaState, static_cast<lua_Integer>(nValue));
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(const String &sString)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		lua_pushstring(m_pLuaState, sString);
		m_nCurrentArgument++;
	}
}

void Script::PushArgument(Object *pObject)
{
	RTTIObjectPointer::LuaStackPush(*this, pObject);
	m_nCurrentArgument++;
}

void Script::PushArgument(Object &cObject)
{
	RTTIObjectPointer::LuaStackPush(*this, &cObject);
	m_nCurrentArgument++;
}

bool Script::EndCall()
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Do the call ('m_nCurrentArgument' arguments, 1 result)
		const int nResult = lua_pcall(m_pLuaState, m_nCurrentArgument, m_bFunctionResult, 0);

		// Perform an incremental step of garbage collection
		// -> For now we don't want to bother the script API user with something
		//    like garbage collection, so do this automatically
		// -> If this is not done, reuse of "RTTIObjectPointer"-instances (as well as derived classes of course)
		//    will not work that well because unused stuff will not be removed automatically by Lua when not asking
		//    Lua to do so
		lua_gc(m_pLuaState, LUA_GCSTEP, 1);

		// Evaluate the result
		if (nResult) {
			// Write a log message
			String sErrorDescription;
			switch (nResult) {
				case LUA_ERRRUN:
					sErrorDescription = " (A runtime error)";
					break;

				case LUA_ERRMEM:
					sErrorDescription = " (Memory allocation error)";
					break;

				case LUA_ERRERR:
					sErrorDescription = " (Error while running the error handler function)";
					break;
			}
			LogOutput(Log::Error, "Error running function '" + m_sCurrentFunction + '\'' + sErrorDescription);

			// Report Lua errors
			ReportErrors();
		} else {
			// If there's no function result we need to pop the table from the Lua stack right now, else this is done within the "GetReturn()"-method
			if (!m_bFunctionResult)
				lua_pop(m_pLuaState, -1);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool Script::GetReturn(bool nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a boolean
		if (lua_isboolean(m_pLuaState, -1)) {
			// Get the result
			nValue = (lua_toboolean(m_pLuaState, -1) != 0);
		} else {
			// Error!
			nValue = false;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a boolean");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = false;
	}

	// Done
	return nValue;
}

float Script::GetReturn(float nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<float>(lua_tonumber(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0.0f;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0.0f;
	}

	// Done
	return nValue;
}

double Script::GetReturn(double nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = lua_tonumber(m_pLuaState, -1);
		} else {
			// Error!
			nValue = 0.0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0.0;
	}

	// Done
	return nValue;
}

int8 Script::GetReturn(int8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint8>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

int16 Script::GetReturn(int16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint16>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

int32 Script::GetReturn(int32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the value
			nValue = static_cast<int32>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

int64 Script::GetReturn(int64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// [TODO] There's no int64 support in Lua (?)
			// Get the result
			nValue = lua_tointeger(m_pLuaState, -1);
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint8 Script::GetReturn(uint8 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint8>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint16 Script::GetReturn(uint16 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint16>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint32 Script::GetReturn(uint32 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// Get the result
			nValue = static_cast<uint32>(lua_tointeger(m_pLuaState, -1));
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

uint64 Script::GetReturn(uint64 nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a number
		if (lua_isnumber(m_pLuaState, -1)) {
			// [TODO] There's no uint64 support in Lua (?)
			// Get the result
			nValue = lua_tointeger(m_pLuaState, -1);
		} else {
			// Error!
			nValue = 0;
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a number");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = 0;
	}

	// Done
	return nValue;
}

String Script::GetReturn(String nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be a string
		if (lua_isstring(m_pLuaState, -1)) {
			// Get the result
			nValue = lua_tostring(m_pLuaState, -1);
		} else {
			// Error!
			nValue = "";
			LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return a string");
		}
		lua_pop(m_pLuaState, 1);

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = "";
	}

	// Done
	return nValue;
}

Object *Script::GetReturn(Object *nValue)
{
	// Is there a Lua state?
	if (m_pLuaState) {
		// Must be user data or nil
		if (lua_isuserdata(m_pLuaState, -1)) {
			// Get user data from the top of the Lua stack and removes it from the Lua stack
			// [TODO] Do any type tests in here?
			nValue = reinterpret_cast<RTTIObjectPointer*>(LuaUserData::PopUserDataFromLuaStack(m_pLuaState))->GetObject();
		} else {
			// Nil is fine, too
			nValue = nullptr;
			if (!lua_isnil(m_pLuaState, -1)) {
				// Error!
				LogOutput(Log::Error, "Function '" + m_sCurrentFunction + "' must return user data");
			}
			lua_pop(m_pLuaState, 1);
		}

		// Pop the table from the Lua stack
		lua_pop(m_pLuaState, -1);
	} else {
		// Error!
		nValue = nullptr;
	}

	// Done
	return nValue;
}

Object &Script::GetReturn(Object &nValue)
{
	// ... please note that in here, we can't return a null pointer...
	Object *pObject = GetReturn(&nValue);
	return pObject ? *pObject : nValue;
}


//[-------------------------------------------------------]
//[ Private static Lua callback functions                 ]
//[-------------------------------------------------------]
/*
*  @brief
*    Lua function callback
*/
int Script::LuaFunctionCallback(lua_State *pLuaState)
{
	// Get the global function
	GlobalFunction *psGlobalFunction = reinterpret_cast<GlobalFunction*>(lua_touserdata(pLuaState, lua_upvalueindex(1)));

	// Call the dynamic function
	return psGlobalFunction ? RTTIObjectMethodPointer::CallDynFunc(*psGlobalFunction->pScript, *psGlobalFunction->pDynFunc, false) : 0;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Script::Script(const Script &cSource) :
	m_pLuaState(nullptr),
	m_bFunctionResult(false),
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
*    Reports Lua errors
*/
void Script::ReportErrors()
{
	// Output the Lua errors
	LogOutput(Log::Error, lua_tostring(m_pLuaState, -1));

	// Pop the error value from the stack
	lua_pop(m_pLuaState, 1);
}

/**
*  @brief
*    Clears the script
*/
void Script::Clear()
{
	// Is there a Lua state?
	if (m_pLuaState) {
		//  Destroy all registered event user data (a kind of "disconnect all slots at once")
		DestroyEventUserData();

		// Reset the source code
		m_sSourceCode = "";

		// Verify the stack and write a warning into the log if the script stack is not empty
		if (lua_gettop(m_pLuaState)) {
			LogOutput(Log::Warning, "Script termination, but the stack is not empty");
			LuaStackDump();
		}

		// Clear all global variables
		while (true) {
			lua_pushnil(m_pLuaState);
			if (!lua_next(m_pLuaState, LUA_GLOBALSINDEX))
				break;
			lua_pop(m_pLuaState, 1);
			lua_pushnil(m_pLuaState);
			lua_rawset(m_pLuaState, LUA_GLOBALSINDEX);
		}

		// Force the garbage collector to cleanup right now... because required stuff like the LuaUserData metatable are still there at the moment...
		lua_gc(m_pLuaState, LUA_GCCOLLECT, 0);

		// Destroy the metatable for LuaUserData events
		LuaUserData::DestroyMetatable(m_pLuaState);

		// Close the Lua state
		lua_close(m_pLuaState);
		m_pLuaState = nullptr;

		// Reset states
		m_sCurrentFunction = "";
		m_bFunctionResult  = false;
		m_nCurrentArgument = 0;
	}
}

/**
*  @brief
*    Creates a nested Lua table
*/
bool Script::CreateNestedTable(const String &sTableName)
{
	// Is there a table name?
	if (sTableName.GetLength()) {
		// Loop through all components within the given nested Lua table name
		uint32 nPartBegin = 0;
		while (nPartBegin<=sTableName.GetLength()) {
			// Find the next "." within the given nested Lua table name
			int nPartEnd = sTableName.IndexOf(".", nPartBegin);
			if (nPartEnd < 0)
				nPartEnd = sTableName.GetLength();

			// Get the current Lua table name
			const String sSubTableName = sTableName.GetSubstring(nPartBegin, nPartEnd - nPartBegin);

			// Each table must have a name!
			if (!sSubTableName.GetLength())
				return false;	// Error!

			// Does the Lua table already exist within the current Lua table?
			if (nPartBegin) {
				// We're already within a Lua table
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_gettable(m_pLuaState, -2);
				if (!lua_isnil(m_pLuaState, -1))
					lua_remove(m_pLuaState, -2);
			} else {
				// Currently we're in global space
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_gettable(m_pLuaState, LUA_GLOBALSINDEX);
			}

			// Lua table found?
			if (lua_isnil(m_pLuaState, -1)) {
				// Nope, create a new one

				// Pop nil from the Lua stack
				lua_pop(m_pLuaState, 1);

				// Create new Lua table on the Lua stack
				lua_newtable(m_pLuaState);
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_pushvalue(m_pLuaState, -2);	// Pushes a copy of the element at the given valid index onto the stack
				if (nPartBegin) {
					// We're already within a Lua table
					lua_settable(m_pLuaState, -4);
					lua_remove(m_pLuaState, -2);
				} else {
					// Currently we're in global space
					lua_settable(m_pLuaState, LUA_GLOBALSINDEX);
				}
			}

			// Skip "."
			nPartBegin = nPartEnd + 1;
		}
	} else {
		// No table name given, so just push the global Lua table onto the stack
		lua_getglobal(m_pLuaState, "_G");
		if (!lua_istable(m_pLuaState, -1))
			return false;	// Error!
	}

	// Done
	return true;
}

/**
*  @brief
*    Gets a nested Lua table
*/
bool Script::GetNestedTable(const String &sTableName)
{
	// Is there a table name?
	if (sTableName.GetLength()) {
		// Loop through all components within the given nested Lua table name
		uint32 nPartBegin = 0;
		while (nPartBegin<=sTableName.GetLength()) {
			// Find the next "." within the given nested Lua table name
			int nPartEnd = sTableName.IndexOf(".", nPartBegin);
			if (nPartEnd < 0)
				nPartEnd = sTableName.GetLength();

			// Get the current Lua table name
			const String sSubTableName = sTableName.GetSubstring(nPartBegin, nPartEnd - nPartBegin);

			// Each table must have a name!
			if (!sSubTableName.GetLength())
				return false;	// Error!

			// Does the Lua table already exist within the current Lua table?
			if (nPartBegin) {
				// We're already within a Lua table
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_gettable(m_pLuaState, -2);
				if (!lua_isnil(m_pLuaState, -1))
					lua_remove(m_pLuaState, -2);
			} else {
				// Currently we're in global space
				lua_pushstring(m_pLuaState, sSubTableName);
				lua_gettable(m_pLuaState, LUA_GLOBALSINDEX);
			}

			// Lua table found?
			if (lua_isnil(m_pLuaState, -1)) {
				// Nope, don't create a new one in here

				// Pop nil from the Lua stack
				lua_pop(m_pLuaState, 1);

				// Cleanup the Lua stack
				if (nPartBegin) {
					// We're already within a Lua table

					// Pop the table from the Lua stack
					lua_pop(m_pLuaState, 1);
				} else {
					// Currently we're in global space
				}

				// Error!
				return false;
			}

			// Skip "."
			nPartBegin = nPartEnd + 1;
		}
	} else {
		// No table name given, so just push the global Lua table onto the stack
		lua_getglobal(m_pLuaState, "_G");
		if (!lua_istable(m_pLuaState, -1))
			return false;	// Error!
	}

	// Done
	return true;
}

/**
*  @brief
*    Returns event user data key
*/
String Script::GetEventUserDataKey(DynEvent *pDynEvent, const void *pLuaPointer) const
{
	return String(pDynEvent) + String(const_cast<void*>(pLuaPointer));
}

/**
*  @brief
*    Returns event user data
*/
Script::EventUserData *Script::GetEventUserData(DynEvent *pDynEvent, const void *pLuaPointer) const
{
	return m_mapEventUserData.Get(GetEventUserDataKey(pDynEvent, pLuaPointer));
}

/**
*  @brief
*    Adds event user data
*/
void Script::AddEventUserData(DynEvent *pDynEvent, const void *pLuaPointer, EventUserData *pEventUserData)
{
	m_mapEventUserData.Add(GetEventUserDataKey(pDynEvent, pLuaPointer), pEventUserData);
}

/**
*  @brief
*    Destroys all registered event user data (a kind of "disconnect all slots at once")
*/
void Script::DestroyEventUserData()
{
	// Iterate through the map content
	Iterator<String> cKeyIterator = m_mapEventUserData.GetKeyIterator();
	while (cKeyIterator.HasNext()) {
		// Get the event user data instance
		EventUserData *pEventUserData = m_mapEventUserData.Get(cKeyIterator.Next());
		
		// Destroy the event user data
		delete pEventUserData->pDynEventHandler;
		luaL_unref(m_pLuaState, LUA_REGISTRYINDEX, pEventUserData->nLuaFunctionReference);
		delete pEventUserData;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua
