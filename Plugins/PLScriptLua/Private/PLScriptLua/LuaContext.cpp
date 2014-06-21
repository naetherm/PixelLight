/*********************************************************\
 *  File: LuaContext.cpp                                 *
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
#include <PLCore/Log/Log.h>
#include "PLScriptLua/LuaContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
uint32									LuaContext::m_nContexCounter = 0;
FastPool<RTTIObjectPointer>				LuaContext::m_lstRTTIObjectPointer;
FastPool<RTTIObjectSlotPointer>			LuaContext::m_lstRTTIObjectSlotPointer;
FastPool<RTTIObjectMethodPointer>		LuaContext::m_lstRTTIObjectMethodPointer;
FastPool<RTTIObjectSignalPointer>		LuaContext::m_lstRTTIObjectSignalPointer;
FastPool<RTTIObjectSignalMethodPointer>	LuaContext::m_lstRTTIObjectSignalMethodPointer;


//[-------------------------------------------------------]
//[ Public static methods                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a context reference
*/
void LuaContext::AddContextReference()
{
	// Check context
	if (!m_nContexCounter)
		PL_LOG(Info, String("Initialize ") + LUA_RELEASE)
	m_nContexCounter++;
}

/**
*  @brief
*    Releases a context reference
*/
void LuaContext::ReleaseContextReference()
{
	// Check context
	m_nContexCounter--;
	if (!m_nContexCounter)
		PL_LOG(Info, String("De-initialize" ) + LUA_RELEASE)
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua
