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


#ifndef __PLSCRIPTLUA_SCRIPT_H__
#define __PLSCRIPTLUA_SCRIPT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Script/Script.h>
#include "PLScriptLua/PLScriptLua.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class DynEvent;
	class DynEventHandler;
}
typedef struct lua_State lua_State;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptLua {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Lua (http://www.lua.org/) script implementation
*/
class Script : public PLCore::Script {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RTTIObjectSignalMethodPointer;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLSCRIPTLUA_RTTI_EXPORT, Script, "PLScriptLua", PLCore::Script, "Lua (http://www.lua.org/) script implementation")
		// Properties
		pl_properties
			pl_property("Language",	"Lua")
			pl_property("Formats",	"lua,LUA")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLSCRIPTLUA_API Script();

		/**
		*  @brief
		*    Destructor
		*/
		PLSCRIPTLUA_API virtual ~Script();

		/**
		*  @brief
		*    Returns the Lua state
		*
		*  @return
		*    Lua state, can be a null pointer
		*/
		PLSCRIPTLUA_API lua_State *GetLuaState() const;

		/**
		*  @brief
		*    Writes the current Lua stack content into the log
		*
		*  @note
		*    - For debugging
		*/
		PLSCRIPTLUA_API void LuaStackDump();


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Script functions               ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		PLSCRIPTLUA_API virtual bool IsGlobalFunction(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTLUA_API virtual bool AddGlobalFunction(const PLCore::String &sFunction, const PLCore::DynFunc &cDynFunc, const PLCore::String &sNamespace = "") override;
		PLSCRIPTLUA_API virtual bool RemoveAllGlobalFunctions() override;

		//[-------------------------------------------------------]
		//[ Script source code                                    ]
		//[-------------------------------------------------------]
		PLSCRIPTLUA_API virtual PLCore::String GetSourceCode() const override;
		PLSCRIPTLUA_API virtual bool SetSourceCode(const PLCore::String &sSourceCode) override;
		PLSCRIPTLUA_API virtual void GetAssociatedFilenames(PLCore::Array<PLCore::String> &lstFilenames) override;
		PLSCRIPTLUA_API virtual bool Execute(const PLCore::String &sSourceCode) override;

		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		PLSCRIPTLUA_API virtual void GetGlobalVariables(PLCore::Array<PLCore::String> &lstGlobalVariables, const PLCore::String &sNamespace = "") override;
		PLSCRIPTLUA_API virtual bool IsGlobalVariable(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTLUA_API virtual PLCore::ETypeID GetGlobalVariableTypeID(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTLUA_API virtual PLCore::String GetGlobalVariable(const PLCore::String &sName, const PLCore::String &sNamespace = "") override;
		PLSCRIPTLUA_API virtual void SetGlobalVariable(const PLCore::String &sName, const PLCore::DynVar &cValue, const PLCore::String &sNamespace = "") override;

		//[-------------------------------------------------------]
		//[ Global function call, used by "FuncScriptPtr"         ]
		//[-------------------------------------------------------]
		PLSCRIPTLUA_API virtual bool BeginCall(const PLCore::String &sFunctionName, const PLCore::String &sFunctionSignature, const PLCore::String &sNamespace = "") override;
		PLSCRIPTLUA_API virtual void PushArgument(bool bValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(float fValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(double fValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::int8 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::int16 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::int32 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::int64 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::uint8 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::uint16 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::uint32 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::uint64 nValue) override;
		PLSCRIPTLUA_API virtual void PushArgument(const PLCore::String &sString) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::Object *pObject) override;
		PLSCRIPTLUA_API virtual void PushArgument(PLCore::Object &cObject) override;
		PLSCRIPTLUA_API virtual bool EndCall() override;
		PLSCRIPTLUA_API virtual bool GetReturn(bool nValue) override;
		PLSCRIPTLUA_API virtual float GetReturn(float nValue) override;
		PLSCRIPTLUA_API virtual double GetReturn(double nValue) override;
		PLSCRIPTLUA_API virtual PLCore::int8 GetReturn(PLCore::int8 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::int16 GetReturn(PLCore::int16 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::int32 GetReturn(PLCore::int32 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::int64 GetReturn(PLCore::int64 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::uint8 GetReturn(PLCore::uint8 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::uint16 GetReturn(PLCore::uint16 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::uint32 GetReturn(PLCore::uint32 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::uint64 GetReturn(PLCore::uint64 nValue) override;
		PLSCRIPTLUA_API virtual PLCore::String GetReturn(PLCore::String nValue) override;
		PLSCRIPTLUA_API virtual PLCore::Object *GetReturn(PLCore::Object *nValue) override;
		PLSCRIPTLUA_API virtual PLCore::Object &GetReturn(PLCore::Object &nValue) override;


	//[-------------------------------------------------------]
	//[ Private static Lua callback functions                 ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @brief
		*    Lua function callback
		*
		*  @param[in] pLuaState
		*    Lua state
		*
		*  @return
		*    Number of parameters to return to Lua
		*/
		static int LuaFunctionCallback(lua_State *pLuaState);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    A global function
		*/
		struct GlobalFunction {
			Script			*pScript;		/**< Pointer to the owner script instance, always valid! */
			PLCore::String   sFunction;		/**< Function name used inside the script to call the global function */
			PLCore::DynFunc *pDynFunc;		/**< Dynamic function to be called, always valid, destroy when done */
			PLCore::String   sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
		};

		/**
		*  @brief
		*    The structure is used to connect script functions with RTTI signals
		*/
		struct EventUserData {
			PLCore::DynEventHandler *pDynEventHandler;		/**< The generic event handler, always valid! (delete the instance when no longer required) */
			Script					*pScript;				/**< The owner script instance, always valid! */
			int						 nLuaFunctionReference;	/**< The Lua-function or C-function to be called, never LUA_NOREF (use luaL_unref(<LuaState>, LUA_REGISTRYINDEX, <Reference>) when no longer required) */
		};


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

		/**
		*  @brief
		*    Reports Lua errors
		*
		*  @note
		*    - Do only call this method if m_pLuaState is valid and there was in fact an error
		*/
		void ReportErrors();

		/**
		*  @brief
		*    Clears the script
		*/
		void Clear();

		/**
		*  @brief
		*    Creates a nested Lua table
		*
		*  @param[in] sTableName
		*    Lua table name (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on), if empty string, the global Lua table is pushed onto the Lua stack
		*
		*  @note
		*    - Creates all required subtables
		*    - Leaves the deepest table on the Lua stack
		*    - Already existing Lua tables are not overwritten
		*    - m_pLuaState must be valid
		*
		*  @return
		*    'true' if all went fine, else 'false' (error within the given Lua table name?)
		*/
		bool CreateNestedTable(const PLCore::String &sTableName);

		/**
		*  @brief
		*    Gets a nested Lua table
		*
		*  @param[in] sTableName
		*    Lua table name (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on), if empty string, the global Lua table is pushed onto the Lua stack
		*
		*  @note
		*    - Leaves the deepest table on the Lua stack
		*    - m_pLuaState must be valid
		*
		*  @return
		*    'true' if all went fine, else 'false' (error within the given Lua table name?)
		*/
		bool GetNestedTable(const PLCore::String &sTableName);

		//[-------------------------------------------------------]
		//[ Event and event handler stuff                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns event user data key
		*
		*  @param[in] pDynEvent
		*    Dynamic event, must be valid
		*  @param[in] pLuaPointer
		*    Lua pointer to the function, must be valid
		*
		*  @return
		*    Event user data key
		*/
		PLCore::String GetEventUserDataKey(PLCore::DynEvent *pDynEvent, const void *pLuaPointer) const;

		/**
		*  @brief
		*    Returns event user data
		*
		*  @param[in] pDynEvent
		*    Dynamic event, must be valid
		*  @param[in] pLuaPointer
		*    Lua pointer to the function, must be valid
		*
		*  @return
		*    Event user data, can be a null pointer
		*/
		EventUserData *GetEventUserData(PLCore::DynEvent *pDynEvent, const void *pLuaPointer) const;

		/**
		*  @brief
		*    Adds event user data
		*
		*  @param[in] pDynEvent
		*    Dynamic event, must be valid
		*  @param[in] pLuaPointer
		*    Lua pointer to the function, must be valid
		*  @param[in] pEventUserData
		*   Event user data to add, must be valid
		*
		*  @note
		*    - Do only call this method if the event user data is not yet added
		*/
		void AddEventUserData(PLCore::DynEvent *pDynEvent, const void *pLuaPointer, EventUserData *pEventUserData);

		/**
		*  @brief
		*    Destroys all registered event user data (a kind of "disconnect all slots at once")
		*/
		void DestroyEventUserData();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String									 m_sSourceCode;			/**< Script source code */
		lua_State										*m_pLuaState;			/**< Lua state, can be a null pointer */
		PLCore::String									 m_sCurrentFunction;	/**< Name of the current function */
		bool											 m_bFunctionResult;		/**< Has the current function a result? */
		PLCore::uint32									 m_nCurrentArgument;	/**< Current argument, used during function call */
		PLCore::Array<GlobalFunction*>				     m_lstGlobalFunctions;	/**< List of global functions */
		PLCore::HashMap<PLCore::String, EventUserData*>  m_mapEventUserData;	/**< Map holding all event user data instances */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptLua


#endif // __PLSCRIPTLUA_SCRIPT_H__
