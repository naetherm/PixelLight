/*********************************************************\
 *  File: RTTIObjectMethodPointer.cpp                    *
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
#include <PLCore/Base/Object.h>
#include "PLScriptLua/Script.h"
#include "PLScriptLua/LuaContext.h"
#include "PLScriptLua/RTTIObjectMethodPointer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the current Lua function parameters on the Lua stack as string
*/
String RTTIObjectMethodPointer::GetLuaFunctionParametersAsString(Script &cScript, DynSignature &cDynSignature, bool bIsMethod, Array<String> &lstTempStrings)
{
	// Get the Lua state
	lua_State *pLuaState = cScript.GetLuaState();

	// Get the number of arguments Lua gave to us
	String sParams;
	uint32 nParameterIndex = 0;
	const int nOffset				= bIsMethod ? 2 : 0;
	const int nNumOfArguments		= lua_gettop(pLuaState) - nOffset;
	const int nIndexOfFirstArgument	= 1+nOffset;
	for (int i=nIndexOfFirstArgument; i<=nOffset+nNumOfArguments; i++, nParameterIndex++) {
		const String sPrefix = String("Param") + nParameterIndex;

		// Is it user data?
		if (lua_isuserdata(pLuaState, i)) {
			// Get user data from the Lua stack without removing it
			Object *pObject = nullptr;
			LuaUserData *pLuaUserData = GetUserDataFromLuaStack(pLuaState, i);
			if (pLuaUserData) {
				// [TODO] Do any type tests in here?
				pObject = reinterpret_cast<RTTIObjectPointerBase*>(pLuaUserData)->GetObject();
			}

			// Add the Lua argument to the parameter string
			if (pObject)
				sParams += sPrefix + '=' + Type<Object*>::ConvertToString(pObject) + ' ';
			else
				sParams += sPrefix + "=0 ";

		// Is it nil?
		} else if (lua_isnil(pLuaState, i)) {
			// Add the Lua argument to the parameter string
			sParams += sPrefix + "=0 ";

		// Is it boolean?
		} else if (lua_isboolean(pLuaState, i)) {
			// Add the Lua argument to the parameter string
			sParams += sPrefix + ((lua_toboolean(pLuaState, i) != 0) ? "=1 " : "=0 ");

		// Is it string?
		} else if (lua_isstring(pLuaState, i)) {
			// Strings are somewhat of a special case... see method documentation for details
			const int nTypeID = cDynSignature.GetParameterTypeID(nParameterIndex);
			switch (nTypeID) {
				case TypeRef:
					// Give the RTTI method a reference to the temporary string and add the Lua argument to the parameter string
					sParams += sPrefix + '=' + Type<String&>::ConvertToString(lstTempStrings.Add(lua_tolstring(pLuaState, i, nullptr))) + ' ';
					break;

				case TypePtr:
					// Give the RTTI method a pointer to the temporary string and add the Lua argument to the parameter string
					sParams += sPrefix + '=' + Type<String*>::ConvertToString(&lstTempStrings.Add(lua_tolstring(pLuaState, i, nullptr))) + ' ';
					break;

				default:
				{
					// It's a blank string, look out with the used quotation mark!
					// If the string content is "Name="Bob"" the resulting parameter string will be Param0="Name="Bob"" and the
					// parser will have troubles with it and will take "Name=" instead of "Name="Bob"" for the value of "Param0"

					// Get the string
					const String sValue = lua_tolstring(pLuaState, i, nullptr);

					// Check for " within the string
					if (sValue.IndexOf('\"') > -1)
						sParams += sPrefix + "='" + sValue + "' ";		// Use ' as quotation mark
					else
						sParams += sPrefix + "=\"" + sValue + "\" ";	// Use " as quotation mark
					break;
				}
			}

		// ...
		} else {
			// If not just let Lua decide how to convert the stuff into a string... but only if it's no object pointer!

			// Add the Lua argument to the parameter string
			if ((cDynSignature.GetParameterTypeID(nParameterIndex) == Type<Object*>::TypeID))
				sParams += sPrefix + "=0 ";
			else
				sParams += sPrefix + "=\"" + lua_tolstring(pLuaState, i, nullptr) + "\" ";
		}
	}

	// Does the RTTI signature demand more parameters as the script programmer provided?
	if (nParameterIndex < cDynSignature.GetNumOfParameters()) {
		// Add the missing parameters by using an empty string
		for (; nParameterIndex<cDynSignature.GetNumOfParameters(); nParameterIndex++) {
			String sValue = "";

			// Strings are somewhat of a special case... see method documentation for details
			// In here, TypePtr is not interesting because it automatically results in a null pointer
			if (cDynSignature.GetParameterTypeID(nParameterIndex) == TypeRef) {
				// Give the RTTI method a reference to the temporary string
				sValue = Type<String&>::ConvertToString(lstTempStrings.Add(sValue));
			}

			// Add the dummy to the parameter string
			sParams += String("Param") + (nParameterIndex) + "=\"" + sValue + "\" ";
		}
	}

	// Return the parameters string
	return sParams;
}

/**
*  @brief
*    Calls the current Lua stack dynamic function
*/
int RTTIObjectMethodPointer::CallDynFunc(Script &cScript, DynFunc &cDynFunc, bool bIsMethod)
{
	// Get the Lua state
	lua_State *pLuaState = cScript.GetLuaState();

	// Has the given dynamic function any parameters?
	if (cDynFunc.GetNumOfParameters()) {
		// This is a bit more complex due to parameter conversion

		// Get the current Lua function parameters on the Lua stack as string
		Array<String> lstTempStrings;
		const String sParams = GetLuaFunctionParametersAsString(cScript, cDynFunc, bIsMethod, lstTempStrings);

		// Get the global function
		const String sReturn = cDynFunc.CallWithReturn(sParams);
		if (sReturn.GetLength()) {
			// Process the functor return
			switch (cDynFunc.GetReturnTypeID()) {
				case TypeVoid:																								return 0;	// The function returns nothing
				case TypeBool:		lua_pushboolean(pLuaState, sReturn.GetBool());											break;
				case TypeDouble:	lua_pushnumber (pLuaState, sReturn.GetDouble());										break;
				case TypeFloat:		lua_pushnumber (pLuaState, sReturn.GetFloat());											break;
				case TypeInt:		lua_pushinteger(pLuaState, sReturn.GetInt());											break;
				case TypeInt16:		lua_pushinteger(pLuaState, sReturn.GetInt());											break;
				case TypeInt32:		lua_pushinteger(pLuaState, sReturn.GetInt());											break;
				case TypeInt64:		lua_pushinteger(pLuaState, sReturn.GetInt());											break;	// [TODO] TypeInt64 is currently handled just as long
				case TypeInt8:		lua_pushinteger(pLuaState, sReturn.GetInt());											break;
				case TypeString:	lua_pushstring (pLuaState, sReturn);													break;
				case TypeUInt16:	lua_pushinteger(pLuaState, sReturn.GetUInt16());										break;
				case TypeUInt32:	lua_pushinteger(pLuaState, sReturn.GetUInt32());										break;
				case TypeUInt64:	lua_pushinteger(pLuaState, static_cast<lua_Integer>(sReturn.GetUInt64()));				break;	// [TODO] TypeUInt64 is currently handled just as long
				case TypeUInt8:		lua_pushinteger(pLuaState, sReturn.GetUInt8());											break;

				// [HACK] Currently, classes derived from "PLCore::Object" are just recognized as type "void*"... but "PLCore::Object*" type would be perfect
				case TypeRef:
				case TypePtr:
				case TypeObjectPtr:
					RTTIObjectPointer::LuaStackPush(cScript, Type<Object*>::ConvertFromString(sReturn));
					break;

				default:			lua_pushstring (pLuaState, sReturn);													break;	// Unknown type
			}

			// The function returns one argument
			return 1;
		}
	} else {
		// There's no need to perform expensive parameter conversion

		// Process the functor dependent on it's return type
		switch (cDynFunc.GetReturnTypeID()) {
			case TypeVoid:
				// The function returns nothing
				cDynFunc.Call(Params<void>());
				return 0;

			case TypeBool:
			{
				Params<bool> cParams;
				cDynFunc.Call(cParams);
				lua_pushboolean(pLuaState, cParams.Return);
				break;
			}

			case TypeDouble:
			{
				Params<double> cParams;
				cDynFunc.Call(cParams);
				lua_pushnumber(pLuaState, cParams.Return);
				break;
			}

			case TypeFloat:
			{
				Params<float> cParams;
				cDynFunc.Call(cParams);
				lua_pushnumber(pLuaState, cParams.Return);
				break;
			}

			case TypeInt:
			{
				Params<int> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, cParams.Return);
				break;
			}

			case TypeInt16:
			{
				Params<int16> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, cParams.Return);
				break;
			}

			case TypeInt32:
			{
				Params<int32> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, cParams.Return);
				break;
			}

			case TypeInt64:
			{
				Params<int64> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, static_cast<lua_Integer>(cParams.Return));
				break;	// [TODO] TypeInt64 is currently handled just as long
			}

			case TypeInt8:
			{
				Params<int8> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, cParams.Return);
				break;
			}

			case TypeString:
			{
				Params<String> cParams;
				cDynFunc.Call(cParams);
				lua_pushstring(pLuaState, cParams.Return);
				break;
			}

			case TypeUInt16:
			{
				Params<uint16> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, cParams.Return);
				break;
			}

			case TypeUInt32:
			{
				Params<uint32> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, cParams.Return);
				break;
			}

			case TypeUInt64:
			{
				Params<uint64> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, static_cast<lua_Integer>(cParams.Return));
				break;	// [TODO] TypeUInt64 is currently handled just as long
			}

			case TypeUInt8:
			{
				Params<uint8> cParams;
				cDynFunc.Call(cParams);
				lua_pushinteger(pLuaState, cParams.Return);
				break;
			}

			// [HACK] Currently, classes derived from "PLCore::Object" are just recognized as type "void*"... but "PLCore::Object*" type would be perfect
			case TypeRef:
			{
				Params<Object&> cParams;
				cDynFunc.Call(cParams);
				RTTIObjectPointer::LuaStackPush(cScript, static_cast<Object*>(cParams.Return));
				break;
			}
			case TypePtr:
			case TypeObjectPtr:
			{
				Params<void*> cParams;	// Must be "void*", not "Object*" else the signature test will fail
				cDynFunc.Call(cParams);
				RTTIObjectPointer::LuaStackPush(cScript, static_cast<Object*>(cParams.Return));
				break;
			}

			// Unknown type, fall back to generic string
			default:
				lua_pushstring(pLuaState, cDynFunc.CallWithReturn(""));
				break;
		}

		// The function returns one argument
		return 1;
	}

	// The function returns nothing
	return 0;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
RTTIObjectMethodPointer::RTTIObjectMethodPointer() : RTTIObjectPointerBase(TypeObjectMethodPointer),
	m_pDynFunc(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
RTTIObjectMethodPointer::~RTTIObjectMethodPointer()
{
}

/**
*  @brief
*    Returns the pointer to the RTTI object method to wrap
*/
DynFuncPtr RTTIObjectMethodPointer::GetDynFuncPtr() const
{
	return m_pDynFunc;
}


//[-------------------------------------------------------]
//[ Protected virtual LuaUserData functions               ]
//[-------------------------------------------------------]
int RTTIObjectMethodPointer::IndexMetamethod(lua_State *pLuaState)
{
	// Error! A method can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Error!
	return 0;
}

int RTTIObjectMethodPointer::NewIndexMetamethod(lua_State *pLuaState)
{
	// Error! A method can't be called like an object...
	// [TODO] Write an error message into the log? (with current script line etc.)

	// Done
	return 0;
}

void RTTIObjectMethodPointer::CGMetamethod(lua_State *pLuaState)
{
	// De-initializes this instance
	RTTIObjectPointerBase::DeInitializeInstance();
	m_pDynFunc = nullptr;

	// Release this instance, but do not delete it because we can reuse it later on
	LuaContext::ReleaseRTTIObjectMethodPointer(*this);
}

void RTTIObjectMethodPointer::CallMetamethod(lua_State *pLuaState)
{
	// Is there a RTTI object and a RTTI object method?
	if (GetObject() && m_pDynFunc) {
		// Call the dynamic function
		CallDynFunc(*m_pScript, *m_pDynFunc, true);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes this instance
*/
void RTTIObjectMethodPointer::InitializeInstance(Script &cScript, Object *pRTTIObject, DynFuncPtr pDynFunc)
{
	// Call base implementation
	RTTIObjectPointerBase::InitializeInstance(cScript, pRTTIObject);

	// Set given data
	m_pDynFunc = pDynFunc;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua
