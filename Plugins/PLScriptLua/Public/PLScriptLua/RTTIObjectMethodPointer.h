/*********************************************************\
 *  File: RTTIObjectMethodPointer.h                      *
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


#ifndef __PLSCRIPTLUA_RTTIOBJECTMETHODPOINTER_H__
#define __PLSCRIPTLUA_RTTIOBJECTMETHODPOINTER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLCore/Base/Func/DynFunc.h>
#include <PLCore/Container/FastPool.h>
#include "PLScriptLua/RTTIObjectPointerBase.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    RTTI object method pointer
*/
class RTTIObjectMethodPointer : public RTTIObjectPointerBase, public PLCore::FastPoolElement<RTTIObjectMethodPointer> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LuaContext;


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the current Lua function parameters on the Lua stack as string
		*
		*  @param[in]  cScript
		*    The owner script instance
		*  @param[in]  cDynSignature
		*    Dynamic signature of the function to be called
		*  @param[in]  bIsMethod
		*    'true' if the dynamic function is a method, 'false' if it's a global function
		*  @param[out] lstTempStrings
		*    List were temporary strings can be put onto, see remarks below for details (the given list is not cleared, new entries are just added)
		*
		*  @return
		*    The current Lua function parameters on the Lua stack as string
		*
		*  @remarks
		*    Strings are somewhat of a special case... It's possible that there's a RTTI method with
		*    a "Object*(const PLCore::String&)"-signature meaning that the parameter is a reference.
		*    Within scripts, strings are fundamental and therefore it should be possible to use such a
		*    RTTI method by writing for instance
		*      this:GetSceneNode():GetByName("Soldier")
		*    so that the script programmer doesn't need to care whether or not the required RTTI method
		*    parameter is in fact a reference instead of a string. Of course, a real reference a parameter
		*    should still also be possible as well.
		*    To solve this issue, whenever the script programmer provides a string, but a RTTI method is
		*    expecting a reference or a pointer, this given string is stored within a string-list on the
		*    heap during the function call to that the reference/pointer has a valid address. That's what
		*    the "lstTempStrings"-parameter of this method is for.
		*/
		static PLCore::String GetLuaFunctionParametersAsString(Script &cScript, PLCore::DynSignature &cDynSignature, bool bIsMethod, PLCore::Array<PLCore::String> &lstTempStrings);

		/**
		*  @brief
		*    Calls the current Lua stack dynamic function
		*
		*  @param[in] cScript
		*    The owner script instance
		*  @param[in] cDynFunc
		*    Dynamic function to be called
		*  @param[in] bIsMethod
		*    'true' if the dynamic function is a method, 'false' if it's a global function
		*
		*  @return
		*    Number of results on the Lua stack
		*/
		static int CallDynFunc(Script &cScript, PLCore::DynFunc &cDynFunc, bool bIsMethod);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RTTIObjectMethodPointer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RTTIObjectMethodPointer();

		/**
		*  @brief
		*    Returns the pointer to the RTTI object method to wrap
		*
		*  @return
		*    Pointer to the RTTI object method to wrap, can be a null pointer
		*/
		PLCore::DynFuncPtr GetDynFuncPtr() const;


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
		PLCore::DynFuncPtr m_pDynFunc;	/**< Smart pointer to the RTTI object method to wrap, can be a null pointer */


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
		*  @param[in] pDynFunc
		*    Smart pointer to the RTTI object method to wrap, can be a null pointer
		*/
		void InitializeInstance(Script &cScript, PLCore::Object *pRTTIObject, PLCore::DynFuncPtr pDynFunc);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua


#endif // __PLSCRIPTLUA_RTTIOBJECTMETHODPOINTER_H__
