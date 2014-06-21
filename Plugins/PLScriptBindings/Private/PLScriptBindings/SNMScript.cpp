/*********************************************************\
 *  File: SNMScript.cpp                                  *
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
#include <PLCore/Script/Script.h>
#include <PLCore/Script/FuncScriptPtr.h>
#include <PLCore/Script/ScriptManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLScriptBindings/SNMScript.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMScript)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMScript::GetScript() const
{
	return m_sScript;
}

void SNMScript::SetScript(const String &sValue)
{
	// Backup the given string
	m_sScript = sValue;

	// Destroy the used script
	DestroyScript();

	// Create the script instance
	m_pScript = ScriptManager::GetInstance()->CreateFromFile(m_sScript);
	if (m_pScript) {
		// Add the global variable "this" to the script so that it's able to access "this" RTTI class instance
		m_pScript->SetGlobalVariable("this", Var<Object*>(this));

		// Update active state
		OnActivate(m_bActive);

		// Call the initialize script function, but only when it's really there because it's optional
		if (m_pScript->IsGlobalFunction(OnInitFunction.Get()))
			FuncScriptPtr<void>(m_pScript, OnInitFunction.Get()).Call(Params<void>());

		// Is there any script source code to execute?
		if (m_sScriptExecute.GetLength())
			m_pScript->Execute(m_sScriptExecute);
	}
}

String SNMScript::GetOnUpdateFunction() const
{
	return m_sOnUpdateFunction;
}

void SNMScript::SetOnUpdateFunction(const String &sValue)
{
	// Backup the given value
	m_sOnUpdateFunction = sValue;

	// Update active state
	OnActivate(m_bActive);
}

String SNMScript::GetScriptExecute() const
{
	return m_sScriptExecute;
}

void SNMScript::SetScriptExecute(const String &sValue)
{
	// Backup the given string
	m_sScriptExecute = sValue;

	// Is there any script source code to execute?
	if (m_sScriptExecute.GetLength() && m_pScript)
		m_pScript->Execute(m_sScriptExecute);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMScript::SNMScript(SceneNode &cSceneNode) : SceneNodeModifier(cSceneNode),
	Script(this),
	OnInitFunction(this),
	OnUpdateFunction(this),
	OnDeInitFunction(this),
	ScriptExecute(this),
	SlotOnUpdate(this),
	m_pScript(nullptr),
	m_sOnUpdateFunction("OnUpdate"),
	m_bActive(false)
{
}

/**
*  @brief
*    Destructor
*/
SNMScript::~SNMScript()
{
	// Destroy the used script
	DestroyScript();
}

/**
*  @brief
*    Returns the instance of the used script
*/
Script *SNMScript::GetScriptInstance() const
{
	return m_pScript;
}


//[-------------------------------------------------------]
//[ Protected virtual PLScene::SceneNodeModifier functions ]
//[-------------------------------------------------------]
void SNMScript::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		// Backup the real active state
		m_bActive = bActivate;

		// In case there's no script instance or no valid update function, we also don't need a constant update
		if (bActivate && m_pScript && m_pScript->IsGlobalFunction(m_sOnUpdateFunction))
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destroys the script
*/
void SNMScript::DestroyScript()
{
	// Is there a script?
	if (m_pScript) {
		// Call the de-initialize script function, but only when it's really there because it's optional
		if (m_pScript->IsGlobalFunction(OnDeInitFunction.Get()))
			FuncScriptPtr<void>(m_pScript, OnDeInitFunction.Get()).Call(Params<void>());

		// Destroy the used script instance
		delete m_pScript;
		m_pScript = nullptr;
	}
}

/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMScript::OnUpdate()
{
	// When we're in here, we know that the script instance and the optional global script update function are both valid (see "SNMScript::OnActivate()")

	// Call the update script function
	FuncScriptPtr<void>(m_pScript, m_sOnUpdateFunction).Call(Params<void>());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings
