/*********************************************************\
 *  File: SceneNodeModifier.h                            *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_H__
#define __PLSCENE_SCENENODEMODIFIER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene node modifier (other name: controller) class
*
*  @remarks
*    A scene node modifier modifies the behavior of a scene node. For instance it adds
*    a physical behavior, make a light source flickering etc. The 'SceneNodeClass' property
*    is the information on which class this modifier operates on. For instance, it's not
*    possible to assign a light flickering modifier to a a scene node which is a noise.
*
*  @note
*    - Derived classes should use a 'SNM'-prefix (example: SNMTransform)
*/
class SceneNodeModifier : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SceneNode;


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node modifier flags
		*/
		enum EFlags {
			Inactive  = 1<<0,	/**< This scene node modifier is currently NOT active */
			Automatic = 1<<1	/**< This scene node modifier was created automatically during runtime and should
									 not be saved with the scene. Such scene nodes modifiers may also be hidden for
									 instance within a scene editor. */
		};
		pl_enum(EFlags)
			pl_enum_value(Inactive,		"This scene renderer pass is currently NOT active")
			pl_enum_value(Automatic,	"This scene node modifier was created automatically during runtime and should not be saved with the scene. Such scene nodes modifiers may also be hidden for instance within a scene editor.")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneNodeModifier, "PLScene", PLCore::Object, "Abstract scene node modifier (other name: controller) class")
		// Properties
		pl_properties
			pl_property("SceneNodeClass",	"PLScene::SceneNode")
		pl_properties_end
		// Attributes
		pl_attribute(Flags,	pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",	"")
		#ifdef PLSCENE_EXPORTS	// The following is only required when compiling PLScene
			// Methods
			pl_method_0(GetSceneNode,		pl_ret_type(SceneNode&),					"Returns the owner scene node.",																																																																																												"")
			pl_method_0(GetSceneNodeIndex,	pl_ret_type(int),							"Returns the index of this scene node modifier within the scene node modifier list of the owner scene node, <0 on failure.",																																																																					"")
			pl_method_0(Clone,				pl_ret_type(SceneNodeModifier*),			"Creates a clone of this scene node modifier within the owner scene node. Returns the created clone of this scene node modifier within the owner scene node, null pointer on error.",																																																							"")
			pl_method_1(CloneAtIndex,		pl_ret_type(SceneNodeModifier*),	int,	"Creates a clone of this scene node modifier within the owner scene node at a certain index inside the scene node modifier list. Index position specifying the location within the scene node modifier list where the scene node modifier should be added as first parameter (<0 for at the end). Returns the created clone of this scene node modifier within the owner scene node, null pointer on error.",	"")
			pl_method_0(GetAbsoluteName,	pl_ret_type(PLCore::String),				"Constructs an unique absolute name for the scene node modifier by using \"<absolute owner scene node name>:<scene node modifier class name>.<zero based index>\" (for instance 'Root.MyScene.MyNode:SNMRotationLinearAnimation.0'). Do not use this method on a regular basis.",																																"")
			pl_method_0(IsActive,			pl_ret_type(bool),							"Returns whether the scene node modifier is active or not. Returns 'true' if the scene node modifier is active, else 'false'.",																																																																					"")
			pl_method_1(SetActive,			pl_ret_type(void),					bool,	"Sets whether the scene node modifier is active or not. 'true' as first parameter if the scene node modifier should be active, else 'false' (sets/unsets the 'Inactive'-flag).",																																																								"")
			pl_method_0(GetInputController,	pl_ret_type(PLInput::Controller*),			"Get the input controller. Returns the input controller (can be a null pointer).",																																																																																"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual PLCore::uint32 GetFlags() const;
		PLS_API virtual void SetFlags(PLCore::uint32 nValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the owner scene node
		*
		*  @return
		*    The owner scene node
		*/
		inline SceneNode &GetSceneNode() const;

		/**
		*  @brief
		*    Returns the index of this scene node modifier within the scene node modifier list of the owner scene node
		*
		*  @return
		*    The index of this scene node modifier within the scene node modifier list of the owner scene node, <0 on failure
		*/
		inline int GetSceneNodeIndex();

		/**
		*  @brief
		*    Creates a clone of this scene node modifier within the owner scene node
		*
		*  @return
		*    The created clone of this scene node modifier within the owner scene node, null pointer on error
		*/
		PLS_API SceneNodeModifier *Clone();

		/**
		*  @brief
		*    Creates a clone of this scene node modifier within the owner scene node at a certain index inside the scene node modifier list
		*
		*  @param[in] nPosition
		*    Index position specifying the location within the scene node modifier list where the scene node modifier should be added, <0 for at the end
		*
		*  @return
		*    The created clone of this scene node modifier within the owner scene node, null pointer on error
		*/
		PLS_API SceneNodeModifier *CloneAtIndex(int nPosition);

		/**
		*  @brief
		*    Returns the scene context the owner scene node is in
		*
		*  @return
		*    The scene context the owner scene node is in, can but shouldn't be a null pointer
		*/
		inline  SceneContext *GetSceneContext() const;

		/**
		*  @brief
		*    Returns the scene node class this modifier operates on
		*
		*  @return
		*    The scene node class this modifier operates on
		*/
		PLS_API PLCore::String GetSceneNodeClass() const;

		/**
		*  @brief
		*    Returns an unique absolute name for the scene node modifier
		*
		*  @return
		*    An unique absolute name for the scene node modifier
		*
		*  @remarks
		*    Constructs an unique absolute name for the scene node modifier by using
		*    "<absolute owner scene node name>:<scene node modifier class name>.<zero based index>"
		*    (for instance 'Root.MyScene.MyNode:SNMRotationLinearAnimation.0'). Do not use this
		*    method on a regular basis.
		*/
		PLS_API PLCore::String GetAbsoluteName() const;

		/**
		*  @brief
		*    Is the scene node modifier active?
		*
		*  @return
		*    'true' if the scene node modifier is active, else 'false'
		*
		*  @note
		*    - Please note that this active state doesn't necessarily mean that the owner scene node is active as well!
		*/
		inline bool IsActive() const;

		/**
		*  @brief
		*    Sets whether the scene node modifier is active or not
		*
		*  @param[in] bActive
		*    Should the scene node modifier be active?
		*
		*  @note
		*    - Sets/unsets the 'Inactive'-flag
		*
		*  @see
		*    - IsActive()
		*/
		PLS_API void SetActive(bool bActive = true);


	//[-------------------------------------------------------]
	//[ Public virtual SceneNodeModifier functions            ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get input controller
		*
		*  @return
		*    Input controller (can be a null pointer)
		*
		*  @note
		*    - The default implementation is empty
		*    - Derived scene node modifiers may add a string attribute called "InputSemantic" to tell the world about
		*      the purpose of the input controller (for example controlling a free camera)
		*/
		PLS_API virtual PLInput::Controller *GetInputController() const;


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNodeModifier functions         ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Informed on scene node modifier initialization
		*
		*  @note
		*    - The default implementation is empty
		*/
		PLS_API virtual void InformedOnInit();

		/**
		*  @brief
		*    Called when the scene node modifier has been activated or deactivated
		*
		*  @param[in] bActivate
		*    'true' if the scene node modifier is now active, else 'false'
		*
		*  @note
		*    - The scene node "Inactive"-flag as well as the "Frozen"-flag are taken into account
		*    - 'bActivate' will be 'true' if the scene node modifier AND the owner scene node AND it's parent scene container (recursive!) are active
		*    - 'bActivate' will be 'false' if the scene node modifier OR the owner scene node OR it's parent scene container (recursive!) is inactive
		*    - The default implementation is empty
		*/
		PLS_API virtual void OnActivate(bool bActivate);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLS_API SceneNodeModifier(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneNodeModifier();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32  m_nFlags;		/**< Flags */
		SceneNode	   *m_pSceneNode;	/**< Owner scene node (ALWAYS valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneNodeModifier.inl"


#endif // __PLSCENE_SCENENODEMODIFIER_H__
