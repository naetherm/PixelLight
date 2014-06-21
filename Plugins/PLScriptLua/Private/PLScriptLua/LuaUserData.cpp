/*********************************************************\
 *  File: LuaUserData.cpp                                *
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
	#include <Lua/lauxlib.h>
}
#include "PLScriptLua/Script.h"
#include "PLScriptLua/LuaUserData.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
char LuaUserData::LuaMetatable = 42;


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates the metatable for LuaUserData events
*/
void LuaUserData::CreateMetatable(lua_State *pLuaState)
{
	lua_pushlightuserdata(pLuaState, &LuaMetatable);
	lua_newtable(pLuaState);

	// Lua __index metamethod callback (for table accesses)
	lua_pushstring(pLuaState, "__index");
	lua_pushcfunction(pLuaState, LuaIndexMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __newindex metamethod callback (for table updates)
	lua_pushstring(pLuaState, "__newindex");
	lua_pushcfunction(pLuaState, LuaNewIndexMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __gc metamethod callback (finalizer/destructor called by garbage collector)
	lua_pushstring(pLuaState, "__gc");
	lua_pushcfunction(pLuaState, LuaCGMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __call metamethod callback (called when Lua calls a value)
	lua_pushstring(pLuaState, "__call");
	lua_pushcfunction(pLuaState, LuaCallMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __tostring metamethod callback (called when Lua tries to convert the user data into a string)
	lua_pushstring(pLuaState, "__tostring");
	lua_pushcfunction(pLuaState, LuaToStringMetamethodCallback);
	lua_rawset(pLuaState, -3);

	// Lua __eq metamethod callback (called when Lua tries to check for equality)
	lua_pushstring(pLuaState, "__eq");
	lua_pushcfunction(pLuaState, LuaEqualityMetamethodCallback);
	lua_rawset(pLuaState, -3);

	lua_rawset(pLuaState, LUA_REGISTRYINDEX);
}

/**
*  @brief
*    Destroys the metatable for LuaUserData events
*/
void LuaUserData::DestroyMetatable(lua_State *pLuaState)
{
	lua_pushlightuserdata(pLuaState, &LuaMetatable);
	lua_pushnil(pLuaState);
	lua_rawset(pLuaState, LUA_REGISTRYINDEX);
}

/**
*  @brief
*    Returns user data from the Lua stack without removing it
*/
LuaUserData *LuaUserData::GetUserDataFromLuaStack(lua_State *pLuaState, int nIndex)
{
	// Must be a metatable
	if (lua_getmetatable(pLuaState, nIndex)) {
		lua_pushlightuserdata(pLuaState, &LuaMetatable);
		lua_rawget(pLuaState, LUA_REGISTRYINDEX);
		if (lua_rawequal(pLuaState, -2, -1)) {
			lua_pop(pLuaState, 2);
			LuaUserData **ppLuaUserData = static_cast<LuaUserData**>(lua_touserdata(pLuaState, nIndex));
			return *ppLuaUserData;
		}
		lua_pop(pLuaState, 1);
	}
	lua_pop(pLuaState, 1);

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns user data from the top of the Lua stack and removes it from the Lua stack
*/
LuaUserData *LuaUserData::PopUserDataFromLuaStack(lua_State *pLuaState)
{
	// Must be a metatable
	if (lua_getmetatable(pLuaState, -1)) {
		lua_pushlightuserdata(pLuaState, &LuaMetatable);
		lua_rawget(pLuaState, LUA_REGISTRYINDEX);
		if (lua_rawequal(pLuaState, -2, -1)) {
			lua_pop(pLuaState, 2);
			LuaUserData **ppLuaUserData = static_cast<LuaUserData**>(lua_touserdata(pLuaState, -1));
			lua_pop(pLuaState, 1);
			return *ppLuaUserData;
		}
		lua_pop(pLuaState, 1);
	}
	lua_pop(pLuaState, 1);
	lua_pop(pLuaState, 1);

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Return the Lua user data type
*/
LuaUserData::EType LuaUserData::GetType() const
{
	return m_nType;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
LuaUserData::LuaUserData(EType nType) :
	m_pScript(nullptr),	// Set known value, "InitializeInstance()" setting valid data will be called shortly
	m_nType(nType)
{
}

/**
*  @brief
*    Destructor
*/
LuaUserData::~LuaUserData()
{
}

/**
*  @brief
*    Initializes this instance
*/
void LuaUserData::InitializeInstance(Script &cScript)
{
	// Set given data
	m_pScript = &cScript;

	// Get the Lua state
	lua_State *pLuaState = m_pScript->GetLuaState();

	// Allocate memory for a pointer to to object
	LuaUserData **ppLuaUserData = reinterpret_cast<LuaUserData**>(lua_newuserdata(pLuaState, sizeof(LuaUserData*)));

	// Set as data a pointer to this instance
	*ppLuaUserData = this;

	// Attach metatable
	lua_pushlightuserdata(pLuaState, &LuaMetatable);
	lua_rawget(pLuaState, LUA_REGISTRYINDEX);
	lua_setmetatable(pLuaState, -2);
}

/**
*  @brief
*    Copy operator
*/
LuaUserData &LuaUserData::operator =(const LuaUserData &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Private static Lua callback functions                 ]
//[-------------------------------------------------------]
/*
*  @brief
*    Lua __index metamethod callback (for table accesses)
*/
int LuaUserData::LuaIndexMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		return pLuaUserData->IndexMetamethod(pLuaState);
	}

	// Error!
	return 0;
}

/*
*  @brief
*    Lua __newindex metamethod callback (for table updates)
*/
int LuaUserData::LuaNewIndexMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		return pLuaUserData->NewIndexMetamethod(pLuaState);
	}

	// Error!
	return 0;
}

/*
*  @brief
*    Lua __gc metamethod callback (finalizer/destructor called by garbage collector)
*/
int LuaUserData::LuaCGMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		pLuaUserData->CGMetamethod(pLuaState);
	}

	// Done
	return 0;
}

/*
*  @brief
*    Lua __call metamethod callback (called when Lua calls a value)
*/
int LuaUserData::LuaCallMetamethodCallback(lua_State *pLuaState)
{
	// Get the current top of the Lua stack
	const int nLuaStackTop = lua_gettop(pLuaState);

	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		pLuaUserData->CallMetamethod(pLuaState);
	}

	// Done, inform Lua what's on the stack
	return lua_gettop(pLuaState) - nLuaStackTop;
}

/*
*  @brief
*    Lua __tostring metamethod callback (called when Lua tries to convert the user data into a string)
*/
int LuaUserData::LuaToStringMetamethodCallback(lua_State *pLuaState)
{
	// Get the current top of the Lua stack
	const int nLuaStackTop = lua_gettop(pLuaState);

	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		pLuaUserData->ToStringMetamethod(pLuaState);
	}

	// Done, inform Lua what's on the stack
	return lua_gettop(pLuaState) - nLuaStackTop;
}

/*
*  @brief
*    Lua __eq metamethod callback (called when Lua tries to check for equality)
*/
int LuaUserData::LuaEqualityMetamethodCallback(lua_State *pLuaState)
{
	// Get user data from the Lua stack without removing it
	LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, 1);
	if (pLuaUserData) {
		// Call the virtual method
		return pLuaUserData->EqualityMetamethod(pLuaState);
	}

	// Error!
	return 0;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
LuaUserData::LuaUserData(const LuaUserData &cSource)
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua
