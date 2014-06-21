/*********************************************************\
 *  File: RTTIObjectSignalMethodPointer.h                *
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


#ifndef __PLSCRIPTLUA_RTTIOBJECTSIGNALMETHODPOINTER_H__
#define __PLSCRIPTLUA_RTTIOBJECTSIGNALMETHODPOINTER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/FastPool.h>
#include "PLScriptLua/RTTIObjectSignalPointerBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class DynParams;
	class DynEventHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RTTI object build in signal method pointer
*/
class RTTIObjectSignalMethodPointer : public RTTIObjectSignalPointerBase, public PLCore::FastPoolElement<RTTIObjectSignalMethodPointer> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LuaContext;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Build in method
		*/
		enum EMethod {
			MethodUnknown    = -1,	/**< Unknown method */
			MethodConnect    = 0,	/**< Connect method */
			MethodDisconnect = 1	/**< Disconnect method */
		};


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the build in method defined by a given string
		*
		*  @param[in] sMethod
		*    String to return the build in method from
		*
		*  @return
		*    The build in method defined by the given string
		*/
		static EMethod StringToMethod(const PLCore::String &sMethod);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RTTIObjectSignalMethodPointer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RTTIObjectSignalMethodPointer();


	//[-------------------------------------------------------]
	//[ Protected virtual LuaUserData functions               ]
	//[-------------------------------------------------------]
	protected:
		virtual int IndexMetamethod(lua_State *pLuaState) override;
		virtual int NewIndexMetamethod(lua_State *pLuaState) override;
		virtual void CGMetamethod(lua_State *pLuaState) override;
		virtual void CallMetamethod(lua_State *pLuaState) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		EMethod m_nMethod;	/**< Build in method */


	//[-------------------------------------------------------]
	//[ Private static event callback functions               ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @brief
		*    Event callback function
		*
		*  @param[in] cDynParams
		*    Dynamic parameters
		*  @param[in] pUserData
		*    Pointer to optional user data, can be a null pointer
		*/
		static void EventCallback(PLCore::DynParams &cDynParams, void *pUserData);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initializes this instance
		*
		*  @param[in] cScript
		*    The owner script instance
		*  @param[in] pRTTIObject
		*    Pointer to the RTTI object to wrap, can be a null pointer
		*  @param[in] pDynEvent
		*    Pointer to the RTTI object signal to wrap, can be a null pointer
		*  @param[in] nMethod
		*    Build in method
		*/
		void InitializeInstance(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynEvent *pDynEvent, EMethod nMethod);

		/**
		*  @brief
		*    Returns a RTTI slot from the Lua stack without removing it
		*
		*  @param[in] pLuaState
		*    Lua state
		*
		*  @return
		*    The valid RTTI slot, null pointer on error
		*
		*  @note
		*    - Performs also a signal/slot signature match
		*    - m_pDynEvent must be valid
		*/
		PLCore::DynEventHandler *GetSlotFromLuaStack(lua_State *pLuaState);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua


#endif // __PLSCRIPTLUA_RTTIOBJECTSIGNALMETHODPOINTER_H__
