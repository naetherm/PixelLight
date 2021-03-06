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
pl_class_metadata(SceneNodeModifier, "PLScene", PLCore::Object, "Abstract scene node modifier (other name: controller) class")
	// Properties
	pl_properties
		pl_property("SceneNodeClass",	"PLScene::SceneNode")
	pl_properties_end
	// Methods
	pl_method_0_metadata(GetSceneNode,			pl_ret_type(SceneNode&),					"Returns the owner scene node.",																																																																																												"")
	pl_method_0_metadata(GetSceneNodeIndex,		pl_ret_type(int),							"Returns the index of this scene node modifier within the scene node modifier list of the owner scene node, <0 on failure.",																																																																					"")
	pl_method_0_metadata(Clone,					pl_ret_type(SceneNodeModifier*),			"Creates a clone of this scene node modifier within the owner scene node. Returns the created clone of this scene node modifier within the owner scene node, null pointer on error.",																																																							"")
	pl_method_1_metadata(CloneAtIndex,			pl_ret_type(SceneNodeModifier*),	int,	"Creates a clone of this scene node modifier within the owner scene node at a certain index inside the scene node modifier list. Index position specifying the location within the scene node modifier list where the scene node modifier should be added as first parameter (<0 for at the end). Returns the created clone of this scene node modifier within the owner scene node, null pointer on error.",	"")
	pl_method_0_metadata(GetAbsoluteName,		pl_ret_type(PLCore::String),				"Constructs an unique absolute name for the scene node modifier by using \"<absolute owner scene node name>:<scene node modifier class name>.<zero based index>\" (for instance 'Root.MyScene.MyNode:SNMRotationLinearAnimation.0'). Do not use this method on a regular basis.",																																"")
	pl_method_0_metadata(IsActive,				pl_ret_type(bool),							"Returns whether the scene node modifier is active or not. Returns 'true' if the scene node modifier is active, else 'false'.",																																																																					"")
	pl_method_1_metadata(SetActive,				pl_ret_type(void),					bool,	"Sets whether the scene node modifier is active or not. 'true' as first parameter if the scene node modifier should be active, else 'false' (sets/unsets the 'Inactive'-flag).",																																																								"")
	pl_method_0_metadata(GetInputController,	pl_ret_type(PLInput::Controller*),			"Get the input controller. Returns the input controller (can be a null pointer).",																																																																																"")
	// Attributes
	pl_attribute_metadata(Flags,	pl_flag_type_def3(SceneNodeModifier, EFlags),	0,	ReadWrite,	"Flags",	"")
pl_class_metadata_end(SceneNodeModifier)


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
