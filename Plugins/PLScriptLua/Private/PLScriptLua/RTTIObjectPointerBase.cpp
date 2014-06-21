/*********************************************************\
 *  File: RTTIObjectPointerBase.cpp                      *
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
}
#include <PLCore/Base/Class.h>
#include <PLCore/Base/Object.h>
#include "PLScriptLua/Script.h"
#include "PLScriptLua/LuaContext.h"
#include "PLScriptLua/RTTIObjectMethodPointer.h"
#include "PLScriptLua/RTTIObjectSignalPointer.h"
#include "PLScriptLua/RTTIObjectSlotPointer.h"
#include "PLScriptLua/RTTIObjectPointerBase.h"


// [TODO] Enum


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
RTTIObjectPointerBase::~RTTIObjectPointerBase()
{
}

/**
*  @brief
*    Returns the pointer to the RTTI object to wrap
*/
Object *RTTIObjectPointerBase::GetObject() const
{
	return m_pRTTIObject;
}

/**
*  @brief
*    Comparison operator
*/
bool RTTIObjectPointerBase::operator ==(const RTTIObjectPointerBase &cOther) const
{
	return (this == &cOther);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RTTIObjectPointerBase::RTTIObjectPointerBase(EType nType) : LuaUserData(nType),
	m_cEventHandlerOnDestroy(&RTTIObjectPointerBase::OnDestroy, this),
	m_pRTTIObject(nullptr)
{
}

/**
*  @brief
*    Initializes this instance
*/
void RTTIObjectPointerBase::InitializeInstance(Script &cScript, Object *pRTTIObject)
{
	// Call base implementation
	LuaUserData::InitializeInstance(cScript);

	// Set given data
	m_pRTTIObject = pRTTIObject;

	// Add RTTI object reference
	if (m_pRTTIObject) {
		m_pRTTIObject->AddReference();
		m_pRTTIObject->SignalDestroyed.Connect(m_cEventHandlerOnDestroy);
	}
}

/**
*  @brief
*    De-initializes this instance
*/
void RTTIObjectPointerBase::DeInitializeInstance()
{
	// Release RTTI object reference
	if (m_pRTTIObject) {
		m_pRTTIObject->SignalDestroyed.Disconnect(m_cEventHandlerOnDestroy);
		m_pRTTIObject->Release();
		m_pRTTIObject = nullptr;
	}

	// Reset
	m_pScript = nullptr;
}


//[-------------------------------------------------------]
//[ Protected virtual LuaUserData functions               ]
//[-------------------------------------------------------]
int RTTIObjectPointerBase::IndexMetamethod(lua_State *pLuaState)
{
	// Valid RTTI object?
	if (m_pRTTIObject) {
		// Get the indexed name
		const String sName = lua_tostring(pLuaState, 2);

		// [TODO] Currently we can use within Lua the dot operator to access RTTI attributes and properties, but there may occur name conflicts...

		// Is it an attribute or a property?
		DynVar *pDynVar = m_pRTTIObject->GetAttribute(sName);
		if (pDynVar) {
			// It's an attribute
			switch (pDynVar->GetType().GetTypeID()) {
				case TypeBool:		lua_pushboolean(pLuaState, pDynVar->GetBool());								break;
				case TypeDouble:	lua_pushnumber (pLuaState, pDynVar->GetDouble());							break;
				case TypeFloat:		lua_pushnumber (pLuaState, pDynVar->GetFloat());							break;
				case TypeInt:		lua_pushinteger(pLuaState, pDynVar->GetInt());								break;
				case TypeInt16:		lua_pushinteger(pLuaState, pDynVar->GetInt());								break;
				case TypeInt32:		lua_pushinteger(pLuaState, pDynVar->GetInt());								break;
				case TypeInt64:		lua_pushinteger(pLuaState, pDynVar->GetInt());								break;	// [TODO] TypeInt64 is currently handled just as long
				case TypeInt8:		lua_pushinteger(pLuaState, pDynVar->GetInt());								break;
				case TypeString:	lua_pushstring (pLuaState, pDynVar->GetString());							break;
				case TypeUInt16:	lua_pushinteger(pLuaState, pDynVar->GetUInt16());							break;
				case TypeUInt32:	lua_pushinteger(pLuaState, pDynVar->GetUInt32());							break;
				case TypeUInt64:	lua_pushinteger(pLuaState, static_cast<lua_Integer>(pDynVar->GetUInt64()));	break;	// [TODO] TypeUInt64 is currently handled just as long
				case TypeUInt8:		lua_pushinteger(pLuaState, pDynVar->GetUInt8());							break;
				default:			lua_pushstring (pLuaState, pDynVar->GetString());							break;	// TypeVoid, TypeNull, TypeObjectPtr, -1
			}

			// Done
			return 1;
		} else {
			// Is it a property?
			const String sValue = m_pRTTIObject->GetClass()->GetProperties().Get(sName);
			if (sValue.GetLength()) {
				lua_pushstring(pLuaState, sValue);

				// Done
				return 1;
			} else {

				// [TODO] Currently methods and signals may have name conflicts because their usage is identical (but that's a general 'problem'...)

				// Is it a method?
				DynFuncPtr pDynFunc = m_pRTTIObject->GetMethod(sName);
				if (pDynFunc) {
					// It's a method... just put another user data instance on the Lua stack...
					// The destruction of the new RTTIObjectMethodPointer instance is done by the Lua garbage collector
					LuaContext::GetRTTIObjectMethodPointer(*m_pScript, m_pRTTIObject, pDynFunc);

					// Done
					return 1;
				} else {
					// Is it a signal?
					DynEvent *pDynEvent = m_pRTTIObject->GetSignal(sName);
					if (pDynEvent) {
						// It's a signal... just put another user data instance on the Lua stack...
						// The destruction of the new RTTIObjectSignalPointer instance is done by the Lua garbage collector
						LuaContext::GetRTTIObjectSignalPointer(*m_pScript, m_pRTTIObject, pDynEvent);

						// Done
						return 1;
					} else {
						// Is it a slot?
						DynEventHandler *pDynEventHandler = m_pRTTIObject->GetSlot(sName);
						if (pDynEventHandler) {
							// It's a slot... just put another user data instance on the Lua stack...
							// The destruction of the new RTTIObjectSlotPointer instance is done by the Lua garbage collector
							LuaContext::GetRTTIObjectSlotPointer(*m_pScript, m_pRTTIObject, pDynEventHandler);

							// Done
							return 1;
						}
					}
				}
			}
		}
	}

	// Error!
	return 0;
}

int RTTIObjectPointerBase::NewIndexMetamethod(lua_State *pLuaState)
{
	static const int LuaNameIndex  = 2;	// Attribute name is at index 2
	static const int LuaValueIndex = 3;	// Attribute value is at index 3

	// Lookout! We can't just use "lua_tostring(pLuaState, 3)" because Lua will give us e.g.
	// for a boolean type just an empty string, meaning that within a Lua script something
	// intuitive like "MyRTTIObject.MyBooleanAttribute = true" would not work.
	const int nLuaType = lua_type(pLuaState, LuaValueIndex);
	switch (nLuaType) {
		case LUA_TNIL:
		{
			// Get the RTTI attribute were we want to assign our value to and set value
			DynVar *pDynVar = m_pRTTIObject->GetAttribute(lua_tostring(pLuaState, LuaNameIndex));
			if (pDynVar)
				pDynVar->SetUIntPtr(0); // Setting a null pointer is a little bit problematic
			break;
		}

		case LUA_TNUMBER:
		{
			// Get the RTTI attribute were we want to assign our value to and set value
			DynVar *pDynVar = m_pRTTIObject->GetAttribute(lua_tostring(pLuaState, LuaNameIndex));
			if (pDynVar)
				pDynVar->SetDouble(lua_tonumber(pLuaState, LuaValueIndex));
			break;
		}

		case LUA_TBOOLEAN:
		{
			// Get the RTTI attribute were we want to assign our value to and set value
			DynVar *pDynVar = m_pRTTIObject->GetAttribute(lua_tostring(pLuaState, LuaNameIndex));
			if (pDynVar)
				pDynVar->SetBool(lua_toboolean(pLuaState, LuaValueIndex) != 0);
			break;
		}

		case LUA_TSTRING:
		{
			// Get the RTTI attribute were we want to assign our value to and set value
			DynVar *pDynVar = m_pRTTIObject->GetAttribute(lua_tostring(pLuaState, LuaNameIndex));
			if (pDynVar)
				pDynVar->SetString(lua_tostring(pLuaState, LuaValueIndex));
			break;
		}

		default:
			// Setting e.g. "LUA_TTABLE", "LUA_TFUNCTION", "LUA_TUSERDATA", "LUA_TTHREAD", "LUA_TLIGHTUSERDATA" won't work
			break;
	}

	// Done
	return 0;
}

void RTTIObjectPointerBase::CallMetamethod(lua_State *pLuaState)
{
	// Nothing to do in here
}

void RTTIObjectPointerBase::ToStringMetamethod(lua_State *pLuaState)
{
	// Convert RTTI class instance pointer into a string
	lua_pushstring(pLuaState, Type<Object*>::ConvertToString(m_pRTTIObject));
}

int RTTIObjectPointerBase::EqualityMetamethod(lua_State *pLuaState)
{
	// Is it user data?
	bool bEqual = false;
	if (lua_isuserdata(pLuaState, 2)) {
		// Get user data from the Lua stack without removing it
		LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, 2);
		if (pLuaUserData) {
			// [TODO] Do any type tests in here?
			Object *pObject = reinterpret_cast<RTTIObjectPointerBase*>(pLuaUserData)->GetObject();

			// Compare object pointers
			bEqual = (m_pRTTIObject == pObject);
		}
	}

	// Push the result onto the Lua stack
	lua_pushboolean(pLuaState, bEqual);
	return 1;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the RTTI object assigned with this wrapper was destroyed
*/
void RTTIObjectPointerBase::OnDestroy()
{
	// Argh! Mayday! We lost our RTTI object!
	m_pRTTIObject = nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua
