/*********************************************************\
 *  File: SceneNode.h                                    *
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


#ifndef __PLSCENE_SCENENODE_H__
#define __PLSCENE_SCENENODE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLCore/Container/Element.h>
#include <PLCore/Container/ElementHandler.h>
#include <PLCore/Container/ElementManager.h>
#include <PLMath/Sphere.h>
#include <PLMath/Transform3.h>
#include <PLMath/AABoundingBox.h>
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLInput {
	class Controller;
}
namespace PLRenderer {
	class Renderer;
}
namespace PLMesh {
	class MeshHandler;
}
namespace PLScene {
	class VisNode;
	class SceneContext;
	class SceneContainer;
	class SceneHierarchy;
	class SceneNodeModifier;
	class SceneHierarchyNodeItem;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene node (leaf node) class
*
*  @remarks
*    This is a leaf node of the scene graph. The node transformation is relative to the
*    container the node is in.
*
*  @note
*    - '.' characters within 'concrete' scene node names are NOT allowed (SceneNode::SetName())
*    - The name 'Root' is NOT allowed for scene nodes, this name is reserved for the 'scene root container'
*    - The name 'Parent' is NOT allowed for scene nodes, this name is reserved for the 'parent scene container'
*    - The name 'This' is NOT allowed for scene nodes, this name is reserved for the 'this scene node'
*    - The name of the 'root scene container' can NOT be changed
*    - Derived classes should use a 'SN'-prefix (example: SNLight)
*    - The "Rotation"-attribute of the scene node is a derived (human friendly) Euler angles (in degree) representation of the internal rotation quaternion,
*      so, whenever possible, work internally with "GetTransform()" instead of "GetRotation()"!
*/
class SceneNode : public PLCore::Object, public PLCore::Element<SceneNode> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SNFog;
	friend class SQCull;
	friend class SCCell;
	friend class SNLight;
	friend class SNPortal;
	friend class SNCamera;
	friend class SQRender;
	friend class SceneContext;
	friend class SceneContainer;
	friend class SceneHierarchy;
	friend class SceneNodeModifier;
	friend class SceneHierarchyNode;
	friend class SceneHierarchyNodeItem;
	friend class PLCore::ElementManager<SceneNode>;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags
		*/
		enum EFlags {
			Inactive         = 1<<0,	/**< This scene node is currently NOT active */
			Invisible        = 1<<1,	/**< This scene node is currently NOT visible */
			Frozen           = 1<<2,	/**< This scene node is currently frozen and therefore not updated */
			Reserved         = 1<<3,	/**< Reserved for future use */
			Automatic        = 1<<4,	/**< This scene node was created automatically during runtime and should
											 not be saved with the scene. Such scene nodes are also hidden for instance
											 within the scene editor. */
			NoCulling        = 1<<5,	/**< No visibility culling for this node, please (the container the node is in may still be culled...) */
			NoLighting       = 1<<6,	/**< No lighting for this node, please */
			CanOcclude       = 1<<7,	/**< This scene node can occlude other scene nodes */
			CastShadow       = 1<<8,	/**< Shadow caster */
			ReceiveShadow    = 1<<9		/**< Shadow receiver */
		};
		pl_enum(EFlags)
			pl_enum_value(Inactive,			"This scene node is currently NOT active")
			pl_enum_value(Invisible,		"This scene node is currently NOT visible")
			pl_enum_value(Frozen,			"This scene node is currently frozen and therefore not updated")
			// pl_enum_value(Reserved,		"Reserved for future use")	// Don't expose this to the RTTI
			pl_enum_value(Automatic,		"This scene node was created automatically during runtime and should not be saved with the scene. Such scene nodes are also hidden for instance within the scene editor.")
			pl_enum_value(NoCulling,		"No visibility culling for this node, please (the container the node is in may still be culled...)")
			pl_enum_value(NoLighting,		"No lighting for this node, please")
			pl_enum_value(CanOcclude,		"This scene node can occlude other scene nodes")
			pl_enum_value(CastShadow,		"Shadow caster")
			pl_enum_value(ReceiveShadow,	"Shadow receiver")
		pl_enum_end

		/**
		*  @brief
		*    Scene node debug flags
		*/
		enum EDebugFlags {
			DebugEnabled               = 1<<0,	/**< Debug mode is enabled (if this flag isn't set, no debug information is drawn at all) */
			DebugNoDrawSignal          = 1<<1,	/**< Do not create a draw debug signal */
			DebugContainerAABBox       = 1<<2,	/**< Draw (the white) container space axis aligned bounding box */
			DebugContainerSphere       = 1<<3,	/**< Draw container space bounding sphere */
			DebugNoLocalCoordinateAxis = 1<<4,	/**< Do not draw the local coordinate axis */
			DebugNoName                = 1<<5,	/**< Do not draw the name of the scene node */
			DebugNoAABBox              = 1<<6,	/**< Do not draw (the yellow) axis aligned bounding box */
			DebugText                  = 1<<7	/**< Draw some basic debug text information */
		};
		pl_enum(EDebugFlags)
			pl_enum_value(DebugEnabled,					"Debug mode is enabled (if this flag isn't set, no debug information is drawn at all)")
			pl_enum_value(DebugNoDrawSignal,			"Do not create a draw debug signal")
			pl_enum_value(DebugContainerAABBox,			"Draw (the white) container space axis aligned bounding box")
			pl_enum_value(DebugContainerSphere,			"Draw container space bounding sphere")
			pl_enum_value(DebugNoLocalCoordinateAxis,	"Do not draw the local coordinate axis")
			pl_enum_value(DebugNoName,					"Do not draw the name of the scene node")
			pl_enum_value(DebugNoAABBox,				"Do not draw (the yellow) axis aligned bounding box")
			pl_enum_value(DebugText,					"Draw some basic debug text information")
		pl_enum_end

		/**
		*  @brief
		*    Draw function flags
		*/
		enum EDrawFunctionFlags {
			UseDrawPre         = 1<<0,	/**< Use DrawPre()-function */
			UseDrawSolid       = 1<<1,	/**< Use DrawSolid()-function */
			UseDrawTransparent = 1<<2,	/**< Use DrawTransparent()-function */
			UseDrawDebug       = 1<<3,	/**< Use DrawDebug()-function */
			UseDrawPost        = 1<<4	/**< Use DrawPost()-function */
		};


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneNode, "PLScene", PLCore::Object, "Abstract scene node (leaf node) class")
		// Properties
		pl_properties
			pl_property("Icon",	"Data/Textures/IconSceneNode.dds")
		pl_properties_end
		// Attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),		0,						ReadWrite,	GetSet,			"Flags",																															"")
		pl_attribute(DebugFlags,		pl_flag_type(EDebugFlags),	0,						ReadWrite,	GetSet,			"Debug flags",																														"")
		pl_attribute(Position,			PLMath::Vector3,			PLMath::Vector3::Zero,	ReadWrite,	GetSet,			"Position",																															"")
		pl_attribute(Rotation,			PLMath::Vector3,			PLMath::Vector3::Zero,	ReadWrite,	GetSet,			"Rotation as Euler angles in degree, [0, 360]",																						"Inc=1")
		pl_attribute(Scale,				PLMath::Vector3,			PLMath::Vector3::One,	ReadWrite,	GetSet,			"Scale",																															"")
		pl_attribute(MaxDrawDistance,	float,						0.0f,					ReadWrite,	DirectValue,	"Maximum draw distance of the scene node to the camera, if 0 do always draw, if negative, do always draw this node before other",	"")
		pl_attribute(AABBMin,			PLMath::Vector3,			PLMath::Vector3::Zero,	ReadWrite,	GetSet,			"Minimum position of the 'scene node space' axis aligned bounding box",																"")
		pl_attribute(AABBMax,			PLMath::Vector3,			PLMath::Vector3::Zero,	ReadWrite,	GetSet,			"Maximum position of the 'scene node space' axis aligned bounding box",																"")
		pl_attribute(Name,				PLCore::String,				"",						ReadWrite,	GetSet,			"Optional scene node name. If not defined, a name is chosen automatically",															"")
		#ifdef PLSCENE_EXPORTS	// The following is only required when compiling PLScene
			// Methods
			pl_method_0(GetContainer,				pl_ret_type(SceneContainer*),																"Returns the scene container the scene node is in or a null pointer if this is the root node.",																																																																																																																"")
			pl_method_1(SetContainer,				pl_ret_type(bool),					SceneContainer&,										"Sets the scene container the scene node is in. Scene container this node is in as first parameter. Returns 'true' if all went fine, else 'false' (Position, rotation, scale etc. are not manipulated, only the container is changed!).",																																																																													"")
			pl_method_0(GetRootContainer,			pl_ret_type(SceneContainer*),																"Returns the scene root container (this scene container can be the root scene container), a null pointer on error.",																																																																																																										"")
			pl_method_1(GetCommonContainer,			pl_ret_type(SceneContainer*),		SceneNode&,												"Gets the common container of this and another scene node. The other scene node as first parameter. Returns the common container, or a null pointer.",																																																																																																		"")
			pl_method_0(GetContainerIndex,			pl_ret_type(int),																			"Returns the index of this scene node within the scene node list of the scene container this scene node is in, <0 on failure (e.g. the scene node is not within a scene container).",																																																																																										"")
			pl_method_0(Clone,						pl_ret_type(SceneNode*),																	"Creates a clone of this scene node within the scene container this scene node is in. Returns the created clone of this scene node within the same scene container the original scene node is in, null pointer on error. Scene nodes and scene node modifiers with a set \"Automatic\"-flag will not be cloned. The debug flags of the created clone are set to 0.",																																														"")
			pl_method_1(CloneAtIndex,				pl_ret_type(SceneNode*),			int,													"Creates a clone of this scene node within the scene container this scene node is in at a certain index inside the scene node list. Index position specifying the location within the scene node list where the scene node should be added as first parameter (<0 for at the end). The created clone of this scene node within the same scene container the original scene node is in, null pointer on error. Scene nodes and scene node modifiers with a set \"Automatic\"-flag will not be cloned. The debug flags of the created clone are set to 0.",	"")
			pl_method_0(GetHierarchy,				pl_ret_type(SceneHierarchy*),																"Returns the scene hierarchy this scene node is linked into. Returns the scene hierarchy this scene node is linked into, a null pointer on error.",																																																																																																			"")
			pl_method_0(GetAbsoluteName,			pl_ret_type(PLCore::String),																"Returns the unique absolute name of the scene node (for instance 'Root.MyScene.MyNode').",																																																																																																																	"")
			pl_method_0(IsActive,					pl_ret_type(bool),																			"Returns whether the scene node is active or not. Returns 'true' if the scene node is active, else 'false'.",																																																																																																												"")
			pl_method_1(SetActive,					pl_ret_type(void),					bool,													"Sets whether the scene node is active or not. 'true' as first parameter if the scene node should be active, else 'false' (sets/unsets the 'Inactive'-flag).",																																																																																																"")
			pl_method_0(IsVisible,					pl_ret_type(bool),																			"Returns whether the scene node is visible or not. Returns 'true' if the scene node is visible, else 'false' (invisible/inactive). If the scene node is not active it's automatically invisible but the 'Invisible'-flag is not touched. 'Visible' doesn't mean 'currently' on screen, it just means 'can be seen in general'.",																																																							"")
			pl_method_1(SetVisible,					pl_ret_type(void),					bool,													"Sets whether the scene node is visible or not. 'true' as first parameter if the scene node should be visible, else 'false' (sets/unsets the 'Invisible'-flag). See 'IsVisible()'-method for more information.",																																																																																			"")
			pl_method_0(IsFrozen,					pl_ret_type(bool),																			"Returns whether the scene node is frozen or not. Returns 'true' if the scene node is frozen, else 'false'."										,																																																																																																		"")
			pl_method_1(SetFrozen,					pl_ret_type(void),					bool,													"Sets whether the scene node is frozen or not. 'true' as first parameter if the scene node should be frozen, else 'false' (sets/unsets the 'Frozen'-flag).",																																																																																																"")
			pl_method_0(IsContainer,				pl_ret_type(bool),																			"Returns whether this scene node is a scene container (SceneContainer) or not. Returns 'true' if this scene node is a scene container, else 'false'.",																																																																																																		"")
			pl_method_0(IsCell,						pl_ret_type(bool),																			"Returns whether this scene node is a cell (SCCell) or not. Returns 'true' if this scene node is a cell, else 'false'.",																																																																																																									"")
			pl_method_0(IsPortal,					pl_ret_type(bool),																			"Returns whether this scene node is a portal (SNPortal) or not. Returns 'true' if this scene node is a portal, else 'false'.",																																																																																																								"")
			pl_method_0(IsCamera,					pl_ret_type(bool),																			"Returns whether this scene node is a camera (SNCamera) or not. Returns 'true' if this scene node is a camera, else 'false'.",																																																																																																								"")
			pl_method_0(IsLight,					pl_ret_type(bool),																			"Returns whether this scene node is a light (SNLight) or not. Returns 'true' if this scene node is a light, else 'false'.",																																																																																																									"")
			pl_method_0(IsFog,						pl_ret_type(bool),																			"Returns whether this scene node is a fog (SNFog) or not. Returns 'true' if this scene node is a fog, else 'false'.",																																																																																																										"")
			pl_method_1(GetNumOfModifiers,			pl_ret_type(PLCore::uint32),		const PLCore::String&,									"Returns the number of modifiers. Optional modifier class name to return the number of instances from as first parameter (if empty return the total number of modifiers).",																																																																																													"")
			pl_method_2(AddModifier,				pl_ret_type(SceneNodeModifier*),	const PLCore::String&,	const PLCore::String&,			"Adds a modifier. Modifier class name of the modifier to add as first parameter and optional parameter string as second parameter. Returns a pointer to the modifier instance if all went fine, else a null pointer (maybe unknown/incompatible modifier).",																																																																								"")
			pl_method_3(AddModifierAtIndex,			pl_ret_type(SceneNodeModifier*),	const PLCore::String&,	const PLCore::String&,	int,	"Adds a modifier at a certain index inside the scene node modifier list. Modifier class name of the modifier to add as first parameter and optional parameter string as second parameter, optional index position specifying the location within the scene node modifier list where the scene node modifier should be added as third parameter (<0 for at the end). Returns a pointer to the modifier instance if all went fine, else a null pointer (maybe unknown/incompatible modifier).",																"")
			pl_method_2(GetModifier,				pl_ret_type(SceneNodeModifier*),	const PLCore::String&,	PLCore::uint32,					"Returns a modifier. Modifier class name of the modifier to return as first parameter, optional modifier index as second parameter (used if class name is empty or if there are multiple instances of this modifier class). Returns the requested modifier, a null pointer on error.",																																																																		"")
			pl_method_1(RemoveModifierByReference,	pl_ret_type(bool),					SceneNodeModifier&,										"Removes a modifier by using a given reference to the modifier to remove. Modifier to remove as first parameter. Returns 'true' if all went fine, else 'false' (maybe invalid modifier). After this method succeeded, the given reference is no longer valid.",																																																																								"")
			pl_method_2(RemoveModifier,				pl_ret_type(bool),					const PLCore::String&,	PLCore::uint32,					"Removes a modifier. Modifier class name of the modifier to remove as first parameter, modifier index as second parameter (used if class name is empty or if there are multiple instances of this modifier class). Returns 'true' if all went fine, else 'false' (maybe invalid modifier).",																																																																"")
			pl_method_0(ClearModifiers,				pl_ret_type(void),																			"Clears all modifiers.",																																																																																																																																	"")
			pl_method_1(Delete,						pl_ret_type(bool),					bool,													"Deletes this scene node. If the first parameter is 'true' the scene node will also be deleted when it's protected. Returns 'true' when all went fine, else 'false'.",																																																																																														"")
			pl_method_0(GetInputController,			pl_ret_type(PLInput::Controller*),															"Get the input controller. Returns the input controller (can be a null pointer).",																																																																																																																			"")
		#endif
		// Signals
		pl_signal_0(SignalDestroy,															"Scene node destruction signal. Unlike \"PLCore::Object::SignalDestroyed\", the scene node is still intact at the point the signal is emitted.","")
		pl_signal_0(SignalActive,															"Scene node active state change signal",																										"")
		pl_signal_0(SignalVisible,															"Scene node visible state change signal",																										"")
		pl_signal_0(SignalContainer,														"Scene node parent container change signal",																									"")
		pl_signal_0(SignalAABoundingBox,													"Scene node axis aligned bounding box change signal",																							"")
		pl_signal_0(SignalInit,																"Scene node initialization signal",																												"")
		pl_signal_0(SignalDeInit,															"Scene node de-initialization change signal",																									"")
		pl_signal_1(SignalAddedToVisibilityTree,	VisNode&,								"Scene node was added to a visibility tree signal. Visibility node representing this scene node within the visibility tree as parameter.",		"")
		pl_signal_2(SignalDrawPre,					PLRenderer::Renderer&, const VisNode*,	"Scene node pre-draw signal. Used renderer and current visibility node of the scene node (can be a null pointer) as parameter.",				"")
		pl_signal_2(SignalDrawSolid,				PLRenderer::Renderer&, const VisNode*,	"Scene node solid-draw signal. Used renderer and current visibility node of the scene node (can be a null pointer) as parameter.",				"")
		pl_signal_2(SignalDrawTransparent,			PLRenderer::Renderer&, const VisNode*,	"Scene node transparent-draw signal. Used renderer and current visibility node of the scene node (can be a null pointer) as parameter.",		"")
		pl_signal_2(SignalDrawDebug,				PLRenderer::Renderer&, const VisNode*,	"Scene node debug-draw signal. Used renderer and current visibility node of the scene node (can be a null pointer) as parameter.",				"")
		pl_signal_2(SignalDrawPost,					PLRenderer::Renderer&, const VisNode*,	"Scene node post-draw signal. Used renderer and current visibility node of the scene node (can be a null pointer) as parameter.",				"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual PLCore::uint32 GetFlags() const;
		PLS_API virtual void SetFlags(PLCore::uint32 nValue);
		PLS_API virtual PLCore::uint32 GetDebugFlags() const;
		PLS_API virtual void SetDebugFlags(PLCore::uint32 nValue);
		PLS_API const PLMath::Vector3 &GetPosition() const;
		PLS_API void SetPosition(const PLMath::Vector3 &vValue);
		PLS_API PLMath::Vector3 GetRotation() const;
		PLS_API void SetRotation(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetScale() const;
		PLS_API void SetScale(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetAABBMin() const;
		PLS_API void SetAABBMin(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetAABBMax() const;
		PLS_API void SetAABBMax(const PLMath::Vector3 &vValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the scene context the scene node is in
		*
		*  @return
		*    The scene context the scene node is in, can but shouldn't be a null pointer
		*
		*  @note
		*    - Do NOT use this function inside a SceneNode-constructor
		*/
		PLS_API SceneContext *GetSceneContext() const;

		/**
		*  @brief
		*    Returns the scene container the scene node is in
		*
		*  @return
		*    Scene container this node is in, or a null pointer if this is the root node
		*
		*  @note
		*    - You can also use GetManager() from PLCore::Element directly
		*/
		PLS_API SceneContainer *GetContainer() const;

		/**
		*  @brief
		*    Sets the scene container the scene node is in
		*
		*  @param[in] cSceneContainer
		*    Scene container this node is in
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Position, rotation, scale etc. are NOT manipulated, ONLY the container is changed!
		*/
		PLS_API bool SetContainer(SceneContainer &cSceneContainer);

		/**
		*  @brief
		*    Returns the scene root container
		*
		*  @return
		*    Scene root container, (this scene container can be the root scene container) a null pointer on error
		*
		*  @remarks
		*    This function searches for the scene root container (scene container without a parent) and returns it.
		*/
		PLS_API SceneContainer *GetRootContainer() const;

		/**
		*  @brief
		*    Gets the common container of this and another scene node
		*
		*  @param[in] cSceneNode
		*    The other scene node
		*
		*  @return
		*    Common container, or a null pointer
		*/
		PLS_API SceneContainer *GetCommonContainer(SceneNode &cSceneNode) const;

		/**
		*  @brief
		*    Returns the index of this scene node within the scene node list of the scene container this scene node is in
		*
		*  @return
		*    The index of this scene node within the scene node list of the scene container this scene node is in, <0 on failure
		*    (e.g. the scene node is not within a scene container)
		*/
		PLS_API int GetContainerIndex();

		/**
		*  @brief
		*    Returns the scene hierarchy this scene node is linked into
		*
		*  @return
		*    Scene hierarchy this scene node is linked into, a null pointer on error
		*/
		PLS_API SceneHierarchy *GetHierarchy() const;

		/**
		*  @brief
		*   Returns the unique absolute name of the scene node
		*
		*  @return
		*    Unique absolute name of the scene node (for instance 'Root.MyScene.MyNode')
		*
		*  @note
		*    - This function 'constructs' the absolute scene node name. For performance
		*      reasons DON'T call this function a few million times per frame!
		*/
		PLS_API PLCore::String GetAbsoluteName() const;

		/**
		*  @brief
		*    Is the scene node initialized?
		*
		*  @return
		*    'true' if the scene node is initialized, else 'false'
		*/
		PLS_API bool IsInitialized() const;

		/**
		*  @brief
		*    Returns whether the scene node is active or not
		*
		*  @return
		*    'true' if the scene node is active, else 'false'
		*/
		PLS_API bool IsActive() const;

		/**
		*  @brief
		*    Sets whether the scene node is active or not
		*
		*  @param[in] bActive
		*    Should the scene node be active?
		*
		*  @note
		*    - Sets/unsets the 'Inactive'-flag
		*/
		PLS_API void SetActive(bool bActive = true);

		/**
		*  @brief
		*    Evaluates whether or not the scene node is active in respect of the complete scene graph
		*
		*  @return
		*    'true' if the scene node is active in respect of the complete scene graph, else 'false'
		*
		*  @note
		*    - For evaluation, the scene node "Inactive"-flag as well as the "Frozen"-flag are taken into account
		*    - Will be 'true' if the scene node AND the parent scene container (recursive!) are active
		*    - Will be 'false' if the scene node OR the parent scene container (recursive!) is inactive
		*    - Try to avoid using this method when possible, due to it's recursive nature it may affect performance when called to often
		*/
		PLS_API bool EvaluateGlobalActiveState() const;

		/**
		*  @brief
		*    Returns whether the scene node is visible or not
		*
		*  @return
		*    'true' if the scene node is visible, else 'false' (invisible/inactive)
		*
		*  @remarks
		*    If the scene node is not active it's automatically invisible but
		*    the 'Invisible'-flag is not touched. 'Visible' doesn't mean 'currently'
		*    on screen, it just means 'can be seen in general'.
		*/
		PLS_API bool IsVisible() const;

		/**
		*  @brief
		*    Sets whether the scene node is visible or not
		*
		*  @param[in] bVisible
		*    Should the scene node be visible?
		*
		*  @note
		*    - Sets/unsets the 'Invisible'-flag
		*
		*  @see
		*    - IsVisible()
		*/
		PLS_API void SetVisible(bool bVisible = true);

		/**
		*  @brief
		*    Returns whether the scene node is frozen or not
		*
		*  @return
		*    'true' if the scene node is frozen, else 'false'
		*/
		PLS_API bool IsFrozen() const;

		/**
		*  @brief
		*    Sets whether the scene node is frozen or not
		*
		*  @param[in] bFrozen
		*    Should the scene node be frozen?
		*
		*  @note
		*    - Sets/unsets the 'Frozen'-flag
		*/
		PLS_API void SetFrozen(bool bFrozen = true);

		/**
		*  @brief
		*    Gets the scene node draw function flags
		*
		*  @return
		*    Scene node draw function flags (see EDrawFunctionFlags)
		*/
		PLS_API PLCore::uint8 GetDrawFunctionFlags() const;

		/**
		*  @brief
		*    Sets the scene node draw function flags
		*
		*  @param[in] nFlags
		*    New scene node draw function flags which should be set (see EDrawFunctionFlags)
		*/
		PLS_API void SetDrawFunctionFlags(PLCore::uint8 nFlags);

		//[-------------------------------------------------------]
		//[ Transformation                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Gets the node transform
		*
		*  @return
		*    Node transform
		*
		*  @remarks
		*    This function will get/set ONLY the scene node transform. No cell-portal check
		*    etc. is performed! When just 'moving' the node, use MoveTo() instead.
		*/
		inline const PLMath::Transform3 &GetTransform() const;
		inline PLMath::Transform3 &GetTransform();

		/**
		*  @brief
		*    Moves the node to a new position
		*
		*  @param[in] vPosition
		*    New node position
		*
		*  @remarks
		*    Unlike 'SetPosition()' this function checks whether or not the node
		*    passes a cell-portal while 'moving' from the current position to the new one.
		*/
		PLS_API void MoveTo(const PLMath::Vector3 &vPosition);

		//[-------------------------------------------------------]
		//[ Bounding volume                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the axis align bounding box in 'scene node space'
		*
		*  @return
		*    The axis align bounding box in 'scene node space'
		*
		*  @note
		*    - The 'scene node space' can be seen as 'object space' :)
		*    - Use GetContainerAABoundingBox() to get this bounding box in
		*      'scene container space' (see it as 'world space' :)
		*    - This axis align bounding box is recalculated internally if required
		*    - Within the default implementation, the axis align bounding box
		*      is not recalculated
		*/
		PLS_API const PLMath::AABoundingBox &GetAABoundingBox();

		/**
		*  @brief
		*    Sets the axis align bounding box in 'scene node space'
		*
		*  @param[in] cAABoundingBox
		*    The axis align bounding box in 'scene node space'
		*
		*  @see
		*    - GetAABoundingBox()
		*/
		PLS_API void SetAABoundingBox(const PLMath::AABoundingBox &cAABoundingBox);

		/**
		*  @brief
		*    Returns the current axis align bounding box in 'scene container space'
		*
		*  @return
		*    The current axis align bounding box in 'scene container space'
		*
		*  @note
		*    - If position, rotation, scale or the bounding box itself was changed, the
		*      current axis align bounding box in 'scene container space' is recalculated
		*      internally before it is returned
		*
		*  @see
		*    - GetAABoundingBox()
		*/
		PLS_API const PLMath::AABoundingBox &GetContainerAABoundingBox();

		/**
		*  @brief
		*    Returns the bounding sphere in 'scene node space'
		*
		*  @return
		*    The bounding sphere in 'scene node space'
		*
		*  @note
		*    - This sphere recalculated internally if the bounding box was changed
		*    - Within the default implementation, the bounding sphere is calculated
		*      by using the the bounding box
		*    - Note that it's not guaranteed that this sphere totally includes the axis align bounding box
		*      in any space. A derived class may decide that another radius fit's the needs
		*      better - for instance a light is using it's radius.
		*
		*  @see
		*    - GetAABoundingBox()
		*/
		PLS_API const PLMath::Sphere &GetBoundingSphere();

		/**
		*  @brief
		*    Returns the current bounding sphere in 'scene container space'
		*
		*  @return
		*    The current bounding in 'scene container space'
		*
		*  @note
		*    - If position, rotation, scale or the bounding box itself was changed, the current bounding
		*      sphere in 'scene container space' is recalculated internally before it is returned
		*    - Note that it's not guaranteed that this sphere totally includes the axis align bounding box
		*      in 'scene container space' (that's no bug or so :)
		*
		*  @see
		*    - GetBoundingSphere()
		*/
		PLS_API const PLMath::Sphere &GetContainerBoundingSphere();

		//[-------------------------------------------------------]
		//[ Instance of                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether this scene node is a scene container (SceneContainer) or not
		*
		*  @return
		*    'true' if this scene node is a scene container, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SceneContainer")
		*/
		inline bool IsContainer() const;

		/**
		*  @brief
		*    Returns whether this scene node is a cell (SCCell) or not
		*
		*  @return
		*    'true' if this scene node is a cell, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SCCell")
		*/
		inline bool IsCell() const;

		/**
		*  @brief
		*    Returns whether this scene node is a portal (SNPortal) or not
		*
		*  @return
		*    'true' if this scene node is a portal, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNPortal")
		*/
		inline bool IsPortal() const;

		/**
		*  @brief
		*    Returns whether this scene node is a camera (SNCamera) or not
		*
		*  @return
		*    'true' if this scene node is a camera, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNCamera")
		*/
		inline bool IsCamera() const;

		/**
		*  @brief
		*    Returns whether this scene node is a light (SNLight) or not
		*
		*  @return
		*    'true' if this scene node is a light, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNLight")
		*/
		inline bool IsLight() const;

		/**
		*  @brief
		*    Returns whether this scene node is a fog (SNFog) or not
		*
		*  @return
		*    'true' if this scene node is a fog, else 'false'
		*
		*  @note
		*    - More performant than IsInstanceOf("PLScene::SNFog")
		*/
		inline bool IsFog() const;

		//[-------------------------------------------------------]
		//[ Modifier                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of modifiers
		*
		*  @param[in] sClass
		*    Modifier class name to return the number of instances from, if empty
		*    return the total number of modifiers
		*
		*  @return
		*    Number of modifiers
		*/
		PLS_API PLCore::uint32 GetNumOfModifiers(const PLCore::String &sClass = "") const;

		/**
		*  @brief
		*    Adds a modifier
		*
		*  @param[in] sClass
		*    Modifier class name of the modifier to add
		*  @param[in] sParameters
		*    Optional parameter string
		*
		*  @return
		*    Pointer to the modifier instance if all went fine, else a null pointer
		*    (maybe unknown/incompatible modifier)
		*/
		inline SceneNodeModifier *AddModifier(const PLCore::String &sClass, const PLCore::String &sParameters = "");

		/**
		*  @brief
		*    Adds a modifier at a certain index inside the scene node modifier list
		*
		*  @param[in] sClass
		*    Modifier class name of the modifier to add
		*  @param[in] sParameters
		*    Optional parameter string
		*  @param[in] nPosition
		*    Optional index position specifying the location within the scene node modifier list where the scene node modifier should be added, <0 for at the end
		*
		*  @return
		*    Pointer to the modifier instance if all went fine, else a null pointer
		*    (maybe unknown/incompatible modifier)
		*/
		PLS_API SceneNodeModifier *AddModifierAtIndex(const PLCore::String &sClass, const PLCore::String &sParameters = "", int nPosition = -1);

		/**
		*  @brief
		*    Returns a modifier
		*
		*  @param[in] sClass
		*    Modifier class name of the modifier to return
		*  @param[in] nIndex
		*    Modifier index, used if sClass is empty or if there are multiple instances
		*    of this modifier class
		*
		*  @return
		*    The requested modifier, a null pointer on error
		*/
		PLS_API SceneNodeModifier *GetModifier(const PLCore::String &sClass, PLCore::uint32 nIndex = 0) const;

		/**
		*  @brief
		*    Removes a modifier by using a given reference to the modifier to remove
		*
		*  @param[in] cModifier
		*    Modifier to remove, after this method succeeded, the given reference is no longer valid
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid modifier)
		*/
		PLS_API bool RemoveModifierByReference(SceneNodeModifier &cModifier);

		/**
		*  @brief
		*    Removes a modifier
		*
		*  @param[in] sClass
		*    Modifier class name of the modifier to remove
		*  @param[in] nIndex
		*    Modifier index, used if sClass is empty or if there are multiple instances
		*    of this modifier class
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid modifier)
		*/
		PLS_API bool RemoveModifier(const PLCore::String &sClass, PLCore::uint32 nIndex = 0);

		/**
		*  @brief
		*    Clears all modifiers
		*/
		PLS_API void ClearModifiers();


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Creates a clone of this scene node within the scene container this scene node is in
		*
		*  @return
		*    The created clone of this scene node within the same scene container the original scene node is in, null pointer on error
		*
		*  @note
		*    - Scene nodes and scene node modifiers with a set "Automatic"-flag will not be cloned
		*    - The debug flags of the created clone are set to 0
		*/
		PLS_API virtual SceneNode *Clone();

		/**
		*  @brief
		*    Creates a clone of this scene node within the scene container this scene node is in at a certain index inside the scene node list
		*
		*  @param[in] nPosition
		*    Index position specifying the location within the scene node list where the scene node should be added, <0 for at the end
		*
		*  @return
		*    The created clone of this scene node within the same scene container the original scene node is in, null pointer on error
		*
		*  @see
		*    - "Clone()"
		*/
		PLS_API virtual SceneNode *CloneAtIndex(int nPosition);

		/**
		*  @brief
		*    Returns a pointer to the mesh handler
		*
		*  @return
		*    Pointer to the mesh handler, a null pointer if there's no mesh handler
		*
		*  @note
		*    - Returns a null pointer by default, function can be implemented in derived classes
		*
		*  @see
		*    - DrawPre()
		*/
		PLS_API virtual PLMesh::MeshHandler *GetMeshHandler();

		/**
		*  @brief
		*    Get input controller
		*
		*  @return
		*    Input controller (can be a null pointer)
		*
		*  @note
		*    - The default implementation is empty
		*    - Derived scene nodes may add a string attribute called "InputSemantic" to tell the world about
		*      the purpose of the input controller (for example controlling a free camera)
		*/
		PLS_API virtual PLInput::Controller *GetInputController() const;

		/**
		*  @brief
		*    This function is called before any solid parts of the scene are drawn
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] pVisNode
		*    The current visibility node of this scene node, can be a null pointer
		*
		*  @remarks
		*    The DrawPre(), DrawSolid(), DrawTransparent(), DrawDebug() and DrawPost() allows custom draw
		*    calls inside a scene rendering process. The function names indicate when each function is called
		*    during scene rendering. It's not predictable which render states are currently set if these functions
		*    are called and these draw functions will NOT interact automatically with 'uniform lighting and shadowing'
		*    performed on nodes providing a mesh. (GetMeshHandler()) So, try to avoid using these functions whenever
		*    possible and provide a scene node mesh instead. The default implementation does only
		*    informing the listeners, functions can be implemented in derived classes - only DrawDebug() provides a default
		*    implementation to draw for instance the bounding box of the scene node. From outside, this draw functions
		*    should ONLY be called if a node is active & visible & 'on the screen' & the draw function of the function is
		*    set. (see GetDrawFunctionFlags()) It's recommended to call these functions ONLY from inside a scene renderer!
		*    If you don't call the base implementation of a scene node draw function inside your derived draw function
		*    or if you are rendering the mesh of the node, you should call SetDraw() to mark this node as drawn. After
		*    the scene node was updated, this drawn-flag is reset automatically. Use this this information to avoid
		*    useless scene node updates. For instance do not update particle effects or other dynamic meshes if they are
		*    currently 'invisible'.
		*
		*  @note
		*    - The default implementation only emits the SignalDrawPre signal
		*    - Should only be called if the draw function flag 'UseDrawPre' is set
		*/
		PLS_API virtual void DrawPre(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr);

		/**
		*  @brief
		*    This function is called when the solid parts of the scene are drawn
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] pVisNode
		*    The current visibility node of this scene node, can be a null pointer
		*
		*  @note
		*    - The default implementation only emits the SignalDrawSolid signal
		*
		*  @see
		*    - DrawPre()
		*/
		PLS_API virtual void DrawSolid(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr);

		/**
		*  @brief
		*    This function is called when the transparent parts of the scene are drawn
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] pVisNode
		*    The current visibility node of this scene node, can be a null pointer
		*
		*  @note
		*    - The default implementation only emits the SignalDrawTransparent signal
		*
		*  @see
		*    - DrawPre()
		*/
		PLS_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr);

		/**
		*  @brief
		*    This function is called when the debug parts of the scene are drawn
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] pVisNode
		*    The current visibility node of this scene node, can be a null pointer
		*
		*  @note
		*    - Should only be called if the 'UseDrawDebug' draw flag and the 'DebugEnabled' debug flag is set
		*
		*  @note
		*    - Beside drawing scene node stuff, the default implementation emits the SignalDrawDebug signal
		*
		*  @see
		*    - DrawPre()
		*/
		PLS_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr);

		/**
		*  @brief
		*    This function is called after transparent parts of the scene are drawn
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] pVisNode
		*    The current visibility node of this scene node, can be a null pointer
		*
		*  @note
		*    - The default implementation only emits the SignalDrawPost signal
		*
		*  @see
		*    - DrawPre()
		*/
		PLS_API virtual void DrawPost(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SceneNode();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneNode();

		/**
		*  @brief
		*    The current axis align bounding box is dirty and must be updated if used next time
		*/
		PLS_API void DirtyAABoundingBox();


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    This function is called when the scene node gets initialized
		*/
		PLS_API virtual void InitFunction();

		/**
		*  @brief
		*    This function is called when the scene node gets de-initialized
		*/
		PLS_API virtual void DeInitFunction();

		/**
		*  @brief
		*    Called when the scene node has been activated or deactivated
		*
		*  @param[in] bActivate
		*    'true' if the scene node is now active, else 'false'
		*
		*  @note
		*    - The scene node "Inactive"-flag as well as the "Frozen"-flag are taken into account
		*    - 'bActivate' will be 'true' if the scene node AND the parent scene container (recursive!) are active
		*    - 'bActivate' will be 'false' if the scene node OR the parent scene container (recursive!) is inactive
		*    - The default implementation calls the "OnActivate()" of all attached scene node modifiers
		*/
		PLS_API virtual void OnActivate(bool bActivate);

		/**
		*  @brief
		*    Updates the axis align bounding box in 'scene node space'
		*
		*  @remarks
		*    This function is called when the axis align bounding box needs to be calculated. One can overwrite
		*    the default implementation to calculate the axis align bounding box in another way. The default
		*    implementation is empty. (current set axis align bounding box is still used)
		*
		*  @note
		*    - We recommend to use 'SetAABoundingBox()' inside your own implementation to set the new axis align
		*      bounding box, this function will take care of all other required updates
		*/
		PLS_API virtual void UpdateAABoundingBox();

		/**
		*  @brief
		*    Returns the bounding sphere in 'scene node space'
		*
		*  @param[out] cSphere
		*    Receives the bounding sphere in 'scene node space'
		*
		*  @remarks
		*    This function is called when the sphere needs to be calculated. One can overwrite
		*    the default implementation to calculate the sphere in another way.
		*/
		PLS_API virtual void GetBoundingSphere(PLMath::Sphere &cSphere);

		/**
		*  @brief
		*    Returns the current bounding sphere in 'scene container space'
		*
		*  @param[out] cSphere
		*    Receives the current bounding in 'scene container space'
		*
		*  @see
		*    - GetBoundingSphere()
		*/
		PLS_API virtual void GetContainerBoundingSphere(PLMath::Sphere &cSphere);

		/**
		*  @brief
		*    Called when the scene node was added to a visibility tree
		*
		*  @param[in] cVisNode
		*    Visibility node which is representing this scene node within the visibility tree
		*
		*  @note
		*    - The default implementation only emits the SignalAddedToVisibilityTree signal
		*    - You can use this method to get informed whether or not the scene node was, for example,
		*      rendered to the screen in order to update only seen scene nodes
		*    - You can use this method to manipulate the world matrix of the visibility node (for example useful for billboards)
		*/
		PLS_API virtual void OnAddedToVisibilityTree(VisNode &cVisNode);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Flags which hold ínternal information
		*/
		enum EInternalFlags {
			// Recalculate
			RecalculateAABoundingBox		   = 1<<0,	/**< Recalculation of the current axis align bounding box required */
			RecalculateContainerAABoundingBox  = 1<<1,	/**< Recalculation of the current axis align bounding box in 'scene container space' required (data is derived from the axis align bounding box and is using the current scene node transformation) */
			RecalculateBoundingSphere		   = 1<<2,	/**< Recalculation of the bounding sphere in 'scene node space' required (data is derived from the axis align bounding box) */
			RecalculateContainerBoundingSphere = 1<<3,	/**< Recalculation of the current bounding sphere in 'scene container space' required (data is derived from the axis align bounding box and is using the current scene node transformation) */
			RecalculateHierarchy			   = 1<<4,	/**< Recalculation of the scene hierarchy for this scene node is required */
			// Scene node types (to avoid RTTI checks in performance critical situations)
			ClassContainer					   = 1<<5,	/**< Derived from 'SceneContainer' */
			ClassCell						   = 1<<6,	/**< Derived from 'SCCell' */
			ClassPortal						   = 1<<7,	/**< Derived from 'SNPortal' */
			ClassCamera						   = 1<<8,	/**< Derived from 'SNCamera' */
			ClassLight						   = 1<<9,	/**< Derived from 'SNLight' */
			ClassFog						   = 1<<10,	/**< Derived from 'SNFog' */
			// Misc
			Initialized						   = 1<<11,	/**< The scene node is initialized */
			DestroyThis						   = 1<<12	/**< The scene node should be destroyed */
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initializes the scene node
		*/
		void InitSceneNode();

		/**
		*  @brief
		*    De-Initializes the scene node
		*/
		void DeInitSceneNode();

		/**
		*  @brief
		*    Clones the given scene node
		*
		*  @param[in] cTargetSceneContainer
		*    Scene container were to create the new scene node in
		*  @param[in] cSceneNode
		*    Scene node to clone
		*  @param[in] sNameExtension
		*    Clone name extension
		*  @param[in] nPosition
		*    Optional index position specifying the location within the child list where the scene node should be added, <0 for at the end
		*
		*  @return
		*    The created clone, null pointer on error
		*
		*  @note
		*    - Scene nodes and scene node modifiers with a set "Automatic"-flag will not be cloned
		*    - The debug flags of the created clone are set to 0
		*/
		SceneNode *CloneSceneNode(SceneContainer &cTargetSceneContainer, const SceneNode &cSceneNode, const PLCore::String &sNameExtension, int nPosition = -1);

		/**
		*  @brief
		*    Call this function if the scene node bounding box was changed and the
		*    hierarchy the scene node is in needs an update
		*
		*  @note
		*    - The hierarchy refresh will be done by the scene container if the
		*      scene hierarchy is used the next time
		*/
		void HierarchyRefreshRequired();

		/**
		*  @brief
		*    Called on position transform change
		*/
		void OnPosition();

		/**
		*  @brief
		*    Called on rotation transform change
		*/
		void OnRotation();

		/**
		*  @brief
		*    Called on scale transform change
		*/
		void OnScale();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerPosition;
		PLCore::EventHandler<> EventHandlerRotation;
		PLCore::EventHandler<> EventHandlerScale;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32					  m_nFlags;							/**< Flags */
		PLCore::uint32					  m_nDebugFlags;					/**< Debug flags */
		PLMath::AABoundingBox			  m_cAABoundingBox;					/**< Axis align bounding box in 'scene node space' */
		PLCore::uint8					  m_nDrawFunctionFlags;				/**< Scene node draw function flags */
		PLCore::uint32					  m_nCounter;						/**< Internal scene node counter */
		PLCore::uint16					  m_nInternalFlags;					/**< Internal flags */
		PLMath::Transform3				  m_cTransform;						/**< 3D transform */
		PLMath::AABoundingBox			  m_cContainerAABoundingBox;		/**< Current axis align bounding box in 'scene container space' */
		PLMath::Sphere					  m_cBoundingSphere;				/**< Bounding sphere in 'scene node space' */
		PLMath::Sphere					  m_cContainerBoundingSphere;		/**< Current bounding sphere in 'scene container space'*/
		PLCore::List<SceneNodeModifier*>  m_lstModifiers;					/**< List of scene node modifiers */
		SceneHierarchyNodeItem			 *m_pFirstSceneHierarchyNodeItem;	/**< The first scene hierarchy node item, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Element functions              ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool Delete(bool bProtectedToo = false) override;
		PLS_API virtual bool SetName(const PLCore::String &sName) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::Element functions             ]
	//[-------------------------------------------------------]
	private:
		PLS_API virtual void DeleteElement() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneNode.inl"


#endif // __PLSCENE_SCENENODE_H__
