/*********************************************************\
 *  File: SceneNodeModifier.cpp                          *
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
#include <PLCore/Base/Class.h>
#include "PLScene/Scene/SceneNodeModifier.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneNodeModifier)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
uint32 SceneNodeModifier::GetFlags() const
{
	return m_nFlags;
}

void SceneNodeModifier::SetFlags(uint32 nValue)
{
	// We now have to reevaluate the active state of this scene node modifier (similar procedure as in "SceneNode::SetFlags()")
	// -> But don't call "SetActive(!(nValue & Inactive));" in here or we will end up in an endless recursion
	if ((m_nFlags & Inactive) != (nValue & Inactive)) {
		// Set new flags
		m_nFlags = nValue;

		// Call the "OnActivate()"-method, please note that we also have to take the global active state of the owner scene node into account
		OnActivate(!(m_nFlags & Inactive) && m_pSceneNode->EvaluateGlobalActiveState());
	} else {
		// Set the new flags
		m_nFlags = nValue;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a clone of this scene node modifier within the owner scene node
*/
SceneNodeModifier *SceneNodeModifier::Clone()
{
	return m_pSceneNode->AddModifier(GetClass()->GetClassName(), GetValues());
}

/**
*  @brief
*    Creates a clone of this scene node modifier within the owner scene node at a certain index inside the scene node modifier list
*/
SceneNodeModifier *SceneNodeModifier::CloneAtIndex(int nPosition)
{
	return m_pSceneNode->AddModifierAtIndex(GetClass()->GetClassName(), GetValues(), nPosition);
}

/**
*  @brief
*    Returns the scene node class this modifier operates on
*/
String SceneNodeModifier::GetSceneNodeClass() const
{
	const Class *pClass = GetClass();
	return (pClass != nullptr) ? pClass->GetProperties().Get("SceneNodeClass") : "PLScene::SceneNode";
}

/**
*  @brief
*    Returns an unique absolute name for the scene node modifier
*/
String SceneNodeModifier::GetAbsoluteName() const
{
	// Get the scene node modifier class name
	const Class *pClass = GetClass();
	const String sClass = pClass ? pClass->GetClassName() : "";
	if (sClass.GetLength()) {
		// Find a decent scene node modifier index
		int nIndex = -1;
		SceneNodeModifier *pFoundSceneNodeModifier = nullptr;
		do {
			nIndex++;
			pFoundSceneNodeModifier = m_pSceneNode->GetModifier(sClass, static_cast<uint32>(nIndex));
		} while (pFoundSceneNodeModifier && pFoundSceneNodeModifier != this);

		// Return an constructed unique absolute name for the scene node modifier
		return m_pSceneNode->GetAbsoluteName() + ':' + sClass + '.' + nIndex;
	} else {
		// This should never ever happen... but anyway, deal with it...

		// Return a constructed unique absolute name for the scene node modifier
		return m_pSceneNode->GetAbsoluteName() + ":?.42";	// ... as good as any fallback name...
	}
}

/**
*  @brief
*    Sets whether the scene node modifier is active or not
*/
void SceneNodeModifier::SetActive(bool bActive)
{
	// State change?
	if (!(m_nFlags & Inactive) != bActive) {
		// Get new flags
		uint32 nNewFlags = m_nFlags;
		if (bActive)
			nNewFlags &= ~Inactive;
		else
			nNewFlags |=  Inactive;

		// Set new flags
		// -> "Flags" is an RTTI get/set attribute calling the virtual method "SetFlags()"
		// -> If required, "SetFlags()" calls "OnActivate()"
		Flags = nNewFlags;
	}
}


//[-------------------------------------------------------]
//[ Public virtual SceneNodeModifier functions            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get input controller
*/
PLInput::Controller *SceneNodeModifier::GetInputController() const
{
	// The default implementation is empty
	return nullptr;
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Informed on scene node modifier initialization
*/
void SceneNodeModifier::InformedOnInit()
{
	// The default implementation is empty
}

/**
*  @brief
*    Called when the scene node modifier has been activated or deactivated
*/
void SceneNodeModifier::OnActivate(bool bActivate)
{
	// The default implementation is empty
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneNodeModifier::SceneNodeModifier(SceneNode &cSceneNode) :
	Flags(this),
	m_nFlags(0),
	m_pSceneNode(&cSceneNode)
{
}

/**
*  @brief
*    Destructor
*/
SceneNodeModifier::~SceneNodeModifier()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
