/*********************************************************\
 *  File: LuaContext.h                                   *
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


#ifndef __PLSCRIPTLUA_LUACONTEXT_H__
#define __PLSCRIPTLUA_LUACONTEXT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/AbstractContext.h>
#include <PLCore/Container/FastPool.h>
#include "PLScriptLua/RTTIObjectPointer.h"
#include "PLScriptLua/RTTIObjectSlotPointer.h"
#include "PLScriptLua/RTTIObjectMethodPointer.h"
#include "PLScriptLua/RTTIObjectSignalPointer.h"
#include "PLScriptLua/RTTIObjectSignalMethodPointer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static Lua context
*/
class LuaContext : public PLCore::AbstractContext {


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Adds a context reference
		*
		*  @note
		*    - If this is the first reference, the internal Lua context is created automatically
		*/
		static void AddContextReference();

		/**
		*  @brief
		*    Releases a context reference
		*
		*  @note
		*    - If this is the last reference, the internal Lua context is destroyed automatically
		*/
		static void ReleaseContextReference();

		//[-------------------------------------------------------]
		//[ Lua user data types                                   ]
		//[-------------------------------------------------------]
		// RTTIObjectPointer
		static inline RTTIObjectPointer &GetRTTIObjectPointer(Script &cScript, PLCore::Object *pRTTIObject);
		static inline void ReleaseRTTIObjectPointer(RTTIObjectPointer &cInstance);
		// RTTIObjectSlotPointer
		static inline RTTIObjectSlotPointer &GetRTTIObjectSlotPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynEventHandler *pDynEventHandler);
		static inline void ReleaseRTTIObjectSlotPointer(RTTIObjectSlotPointer &cInstance);
		// RTTIObjectMethodPointer
		static inline RTTIObjectMethodPointer &GetRTTIObjectMethodPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynFuncPtr pDynFunc);
		static inline void ReleaseRTTIObjectMethodPointer(RTTIObjectMethodPointer &cInstance);
		// RTTIObjectSignalPointer
		static inline RTTIObjectSignalPointer &GetRTTIObjectSignalPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynEvent *pDynEvent);
		static inline void ReleaseRTTIObjectSignalPointer(RTTIObjectSignalPointer &cInstance);
		// RTTIObjectSignalMethodPointer
		static inline RTTIObjectSignalMethodPointer &GetRTTIObjectSignalMethodPointer(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynEvent *pDynEvent, RTTIObjectSignalMethodPointer::EMethod nMethod);
		static inline void ReleaseRTTIObjectSignalMethodPointer(RTTIObjectSignalMethodPointer &cInstance);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static PLCore::uint32 m_nContexCounter;	/**< Lua context counter */

		// Lua user data types
		static PLCore::FastPool<RTTIObjectPointer>				m_lstRTTIObjectPointer;				/**< Fast pool for "RTTIObjectPointer"-instances */
		static PLCore::FastPool<RTTIObjectSlotPointer>			m_lstRTTIObjectSlotPointer;			/**< Fast pool for "RTTIObjectSlotPointer"-instances */
		static PLCore::FastPool<RTTIObjectMethodPointer>		m_lstRTTIObjectMethodPointer;		/**< Fast pool for "RTTIObjectMethodPointer"-instances */
		static PLCore::FastPool<RTTIObjectSignalPointer>		m_lstRTTIObjectSignalPointer;		/**< Fast pool for "RTTIObjectSignalPointer"-instances */
		static PLCore::FastPool<RTTIObjectSignalMethodPointer>	m_lstRTTIObjectSignalMethodPointer;	/**< Fast pool for "RTTIObjectSignalMethodPointer"-instances */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLScriptLua/LuaContext.inl"


#endif // __PLSCRIPTLUA_LUACONTEXT_H__
