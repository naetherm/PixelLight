/*********************************************************\
 *  File: SNMScript.h                                    *
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


#ifndef __PLSCRIPTBINDINGS_SNMSCRIPT_H__
#define __PLSCRIPTBINDINGS_SNMSCRIPT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNodeModifier.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Script;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Script scene node modifier
*
*  @note
*    - Adds the global variable "this" to the script so that it's able to access "this" RTTI class instance
*/
class SNMScript : public PLScene::SceneNodeModifier {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SNMScript, "PLScriptBindings", PLScene::SceneNodeModifier, "Script scene node modifier")
		// Attributes
		pl_attribute(Script,			PLCore::String,	"",			ReadWrite,	GetSet,			"Script to use (set the script again in order to reload it)",																									"")
		pl_attribute(OnInitFunction,	PLCore::String,	"OnInit",	ReadWrite,	DirectValue,	"Name of the optional script function called by C++ when the scene node modifier should initialize itself",														"")
		pl_attribute(OnUpdateFunction,	PLCore::String,	"OnUpdate",	ReadWrite,	GetSet,			"Name of the optional script function called by C++ when the scene node modifier should update itself",															"")
		pl_attribute(OnDeInitFunction,	PLCore::String,	"OnDeInit",	ReadWrite,	DirectValue,	"Name of the optional script function called by C++ when the scene node modifier should de-initialize itself",													"")
		pl_attribute(ScriptExecute,		PLCore::String,	"",			ReadWrite,	GetSet,			"Script source code to execute, can be used to set global variables (Lua example: ScriptExecute=\"PublicVariables.Speed=0.07 PublicVariables.Radius=0.01\")",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
		// Methods
		pl_method_0(GetScriptInstance,	pl_ret_type(PLCore::Script*),	"Returns the instance of the used script (can be a null pointer)",	"")
		// Slots
		pl_slot_0(OnUpdate,	"Called when the scene node modifier needs to be updated",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::String GetScript() const;
		void SetScript(const PLCore::String &sValue);
		PLCore::String GetOnUpdateFunction() const;
		void SetOnUpdateFunction(const PLCore::String &sValue);
		PLCore::String GetScriptExecute() const;
		void SetScriptExecute(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		SNMScript(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMScript();

		/**
		*  @brief
		*    Returns the instance of the used script
		*
		*  @return
		*    The instance of the used script, can be a null pointer (don't destroy the returned instance!)
		*/
		PLCore::Script *GetScriptInstance() const;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Destroys the script
		*/
		void DestroyScript();

		/**
		*  @brief
		*    Called when the scene node modifier needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String  m_sScript;				/**< Script to use */
		PLCore::Script *m_pScript;				/**< Used script instance, can be a null pointer */
		PLCore::String  m_sOnUpdateFunction;	/**< Name of the optional script function called by C++ when the scene node modifier should update itself */
		PLCore::String  m_sScriptExecute;		/**< Script source code to execute, can be used to set global variables (Lua example: ScriptExecute="PublicVariables.Speed=0.07 PublicVariables.Radius=0.01") */
		bool			m_bActive;				/**< Backup of the real active state */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings


#endif // __PLSCRIPTBINDINGS_SNMSCRIPT_H__
