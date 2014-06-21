/*********************************************************\
 *  File: RTTIObjectSlotPointer.cpp                      *
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
#include "PLScriptLua/LuaContext.h"
#include "PLScriptLua/RTTIObjectSlotPointer.h"


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
*    Constructor
*/
RTTIObjectSlotPointer::RTTIObjectSlotPointer() : RTTIObjectPointerBase(TypeObjectSlotPointer),
	m_pDynEventHandler(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
RTTIObjectSlotPointer::~RTTIObjectSlotPointer()
{
}

/**
*  @brief
*    Returns the pointer to the RTTI object slot to wrap
*/
DynEventHandler *RTTIObjectSlotPointer::GetDynEventHandler() const
{
	return m_pDynEventHandler;
}


//[-------------------------------------------------------]
//[ Protected virtual LuaUserData functions               ]
//[-------------------------------------------------------]
int RTTIObjectSlotPointer::IndexMetamethod(lua_State *pLuaState)
{
	// Error! A slot can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Error!
	return 0;
}

int RTTIObjectSlotPointer::NewIndexMetamethod(lua_State *pLuaState)
{
	// Error! A slot can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Done
	return 0;
}

void RTTIObjectSlotPointer::CGMetamethod(lua_State *pLuaState)
{
	// De-initializes this instance
	RTTIObjectPointerBase::DeInitializeInstance();
	m_pDynEventHandler = nullptr;

	// Release this instance, but do not delete it because we can reuse it later on
	LuaContext::ReleaseRTTIObjectSlotPointer(*this);
}

void RTTIObjectSlotPointer::CallMetamethod(lua_State *pLuaState)
{
	// Error! A slot can't be called like a function...
	// [TODO] Write an error message into the log? (with current script line etc.)
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes this instance
*/
void RTTIObjectSlotPointer::InitializeInstance(Script &cScript, Object *pRTTIObject, PLCore::DynEventHandler *pDynEventHandler)
{
	// Call base implementation
	RTTIObjectPointerBase::InitializeInstance(cScript, pRTTIObject);

	// Set given data
	m_pDynEventHandler = pDynEventHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua
