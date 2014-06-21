/*********************************************************\
 *  File: SceneContainer.h                               *
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


#ifndef __PLSCENE_SCENECONTAINER_H__
#define __PLSCENE_SCENECONTAINER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Pool.h>
#include <PLCore/Tools/Loadable.h>
#include "PLScene/Scene/SceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SceneQuery;
class SceneHierarchy;
class SceneQueryManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene container node (group node) class which is using scene nodes
*
*  @remarks
*    This is a root or inner node of the scene graph.
*
*  @note
*    - The 'scene node space' axis aligned bounding box should always cover all scene nodes of this container
*    - Within GetByName(<Name>) you can also use 'absolute names' instead of 'relative' names. 'Root.MyScene.MyNode'
*      for instance will return the scene node 'MyNode' within the scene container 'MyScene' which is a
*      subnode of 'Root'. 'MyScene.MyNode' will return the node 'MyNode' of the container 'MyScene' which is
*      an element of the current container. 'Parent.MyNode' will return the scene node 'MyNode' within the parent
*      scene container. 'Parent' returns the parent scene container, 'This' returns THIS container, 'Root' will return
*      the root scene container itself.
*    - Use "GetSceneContext()->GetRoot()" to get the 'root' node in which you can insert your scenes
*    - Derived classes should use a 'SC'-prefix (example: SCCell)
*    - By default, all draw function flags are set
*/
class SceneContainer : public SceneNode, public PLCore::ElementManager<SceneNode>, public PLCore::Loadable {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SceneNode;
	friend class SceneLoader;
	friend class SceneContext;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoRecursion = 1<<10	/**< Do NOT take the scene nodes of this container into account when for instance
									 rendering the scene. 'SCRenderToTexture' for example sets this flag. */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(NoRecursion, "Do NOT take the scene nodes of this container into account when for instance rendering the scene. 'SCRenderToTexture' for example sets this flag.")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneContainer, "PLScene", PLScene::SceneNode, "Scene container node (group node) class which is using scene nodes")
		// Attributes
		pl_attribute(Hierarchy,	PLCore::String,			"PLScene::SHList",									ReadWrite,	GetSet,	"Class name of the scene container hierarchy",							"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,		pl_flag_type(EFlags),	0,													ReadWrite,	GetSet,	"Flags",																"")
		pl_attribute(AABBMin,	PLMath::Vector3,		PLMath::Vector3(-10000.0f, -10000.0f, -10000.0f),	ReadWrite,	GetSet,	"Minimum position of the 'scene node space' axis aligned bounding box",	"")
		pl_attribute(AABBMax,	PLMath::Vector3,		PLMath::Vector3( 10000.0f,  10000.0f,  10000.0f),	ReadWrite,	GetSet,	"Maximum position of the 'scene node space' axis aligned bounding box",	"")
			// Overwritten Loadable attributes
		pl_attribute(Filename,	PLCore::String,			"",													ReadWrite,	GetSet,	"Filename of the file to load the container from",						"Type='Scene'")
		#ifdef PLSCENE_EXPORTS	// The following is only required when compiling PLScene
			// Constructors
			pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
			// Methods
			pl_method_1(Clear,					pl_ret_type(bool),			bool,																			"Destroys all scene nodes within this scene container. If the first parameter is 'true' protected scene nodes are destroyed as well. Returns 'true' if all went fine, else 'false'.",																																																																																													"")
			pl_method_1(GetByIndex,				pl_ret_type(SceneNode*),	PLCore::uint32,																	"Returns a scene node by using the given index, result can be a null pointer.",																																																																																																																							"")
			pl_method_1(GetByName,				pl_ret_type(SceneNode*),	const PLCore::String&,															"Returns a scene node by using the given name, result can be a null pointer.",																																																																																																																							"")
			pl_method_3(Create,					pl_ret_type(SceneNode*),	const PLCore::String&,	const PLCore::String&,	const PLCore::String&,			"Creates a new scene node. Name of the scene node class to create an instance from as first parameter, scene node name as second parameter and optional parameter string as third parameter. Returns a pointer to the new scene node or a null pointer if something went wrong (maybe unknown class or the class is not derived from \"PLScene::SceneNode\").",																																																			"")
			pl_method_4(CreateAtIndex,			pl_ret_type(SceneNode*),	const PLCore::String&,	const PLCore::String&,	const PLCore::String&,	int,	"Creates a new scene node at a certain index inside the scene node list. Name of the scene node class to create an instance from as first parameter, scene node name as second parameter and optional parameter string as third parameter, optional index position specifying the location within the scene node list where the scene node should be added as fourth parameter (<0 for at the end). Returns a pointer to the new scene node or a null pointer if something went wrong (maybe unknown class or the class is not derived from \"PLScene::SceneNode\").",	"")
			pl_method_0(CalculateAABoundingBox,	pl_ret_type(void),																							"Calculates and sets the axis align bounding box in 'scene node space'. Because the 'scene node space' axis aligned bounding box should always cover all scene nodes of this container, you can use this function to calculate and set this a bounding box automatically.",																																																																								"")
			pl_method_3(LoadByFilename,			pl_ret_type(bool),			const PLCore::String&,	const PLCore::String&,	const PLCore::String&,			"Load a scene from a file given by filename. Scene filename as first parameter, optional load method parameters as second parameter, optional name of the load method to use as third parameter. Returns 'true' if all went fine, else 'false'.",																																																																														"")
		#endif
		// Signals
		pl_signal_1(SignalLoadProgress,	float,	"Scene load progress signal. Current load progress as parameter - if not within 0-1 loading is done.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetHierarchy() const;
		PLS_API void SetHierarchy(const PLCore::String &sValue);
		PLS_API void SetFilename(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SceneContainer();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneContainer();

		/**
		*  @brief
		*    Creates a new scene node
		*
		*  @param[in] sClass
		*    Name of the scene node class to create an instance from
		*  @param[in] sName
		*    Scene node name
		*  @param[in] sParameters
		*    Optional parameter string
		*
		*  @return
		*    Pointer to the new scene node or a null pointer if something went wrong
		*    (maybe unknown class or the class is not derived from SceneNode)
		*
		*  @note
		*    - If the desired name is already in use, the name is chosen automatically
		*/
		inline SceneNode *Create(const PLCore::String &sClass, const PLCore::String &sName = "", const PLCore::String &sParameters = "");

		/**
		*  @brief
		*    Creates a new scene node at a certain index inside the scene node list
		*
		*  @param[in] sClass
		*    Name of the scene node class to create an instance from
		*  @param[in] sName
		*    Scene node name
		*  @param[in] sParameters
		*    Optional parameter string
		*  @param[in] nPosition
		*    Optional index position specifying the location within the scene node list where the scene node should be added, <0 for at the end
		*
		*  @return
		*    Pointer to the new scene node or a null pointer if something went wrong
		*    (maybe unknown class or the class is not derived from SceneNode)
		*
		*  @note
		*    - If the desired name is already in use, the name is chosen automatically
		*/
		PLS_API SceneNode *CreateAtIndex(const PLCore::String &sClass, const PLCore::String &sName = "", const PLCore::String &sParameters = "", int nPosition = -1);

		/**
		*  @brief
		*    Calculates and sets the axis align bounding box in 'scene node space'
		*
		*  @remarks
		*    Because the 'scene node space' axis aligned bounding box should always cover all scene nodes of
		*    this container, you can use this function to calculate and set this a bounding box automatically.
		*/
		PLS_API void CalculateAABoundingBox();

		/**
		*  @brief
		*    Calculates a transform matrix to bring a transformation from 'this scene container' into 'another scene container'
		*
		*  @param[in]  cContainer
		*    'Target' scene container
		*  @param[out] mTransform
		*    Receives the transform matrix, may contain an invalid matrix on error
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool GetTransformMatrixTo(SceneContainer &cContainer, PLMath::Matrix3x4 &mTransform);

		//[-------------------------------------------------------]
		//[ Hierarchy functions                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates the scene hierarchy of this scene container
		*
		*  @param[in] sClass
		*    Class name of the scene hierarchy to create, if empty this function will do nothing
		*
		*  @return
		*    Pointer to the created scene hierarchy of this scene container. This is NEVER
		*    a null pointer - if no special hierarchy is used the default hierarchy 'PLScene::SHList'
		*    is used instead which is in fact only a simple list which is using the container directly.
		*
		*  @note
		*    - This hierarchy is destroyed by this scene container automatically
		*    - All current container scene nodes are added to the hierarchy automatically
		*      and the hierarchy is created
		*/
		PLS_API SceneHierarchy *CreateHierarchy(const PLCore::String &sClass = "PLScene::SHList");

		/**
		*  @brief
		*    Returns the scene hierarchy of this scene container
		*
		*  @return
		*    Pointer to the scene hierarchy of this scene container,
		*    can NEVER be a null pointer!
		*
		*  @remarks
		*    Before the hierarchy is returned, scene nodes are refreshed if required. So, do NOT
		*    backup the returned pointer, call this function if you need a pointer to the hierarchy!
		*/
		PLS_API SceneHierarchy *GetHierarchyInstance();

		//[-------------------------------------------------------]
		//[ Query functions                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates a scene query
		*
		*  @param[in] sClass
		*    Class name of the scene query to create
		*
		*  @remarks
		*    Have a look at SceneQuery for scene query remarks. If your created
		*    query is no longer required you can destroy it using the DestroyQuery()
		*    functions. But note that if you need a query frequently it's not a good
		*    idea to create & destroy it every time you perform this query. For instance
		*    queries for rendering the scene will store different current rendering information,
		*    in this case it would be terrible to create & destroy the query every frame!
		*    Maybe the rendering algorithm doesn't work correctly if this is done because
		*    sometimes they will need information from the previous frame!
		*    Such queries should be created once and then be used all the time. Use
		*    SceneQueryHandler to hold your pointer to the query. If the container is destroyed
		*    your pointer is set to a null pointer automatically.
		*    If for instance the scene hierarchy was changed some queries like render queries have
		*    to update some internal data. The internal scene query manager of the scene container
		*    will inform it's queries in such a case.
		*    The created scene query can ONLY operate on this scene container and it's child scene nodes/
		*    containers. Therefore you can limit your queries to for instance to a single room instead
		*    using the hold scene. Use the scene root (GetRootContainer()) to access the complete scene.
		*
		*  @return
		*    Pointer to the created scene query operating on this scene container,
		*    a null pointer on error (maybe unknown class or the class is not derived from
		*    'SceneQuery')
		*/
		PLS_API SceneQuery *CreateQuery(const PLCore::String &sClass);

		/**
		*  @brief
		*    Destroys a scene query
		*
		*  @param[in] cQuery
		*    Scene query to destroy
		*
		*  @return
		*    'true' if all went fine, else 'false' (invalid query :)
		*/
		PLS_API bool DestroyQuery(SceneQuery &cQuery);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Adds a node
		*
		*  @param[in] cNode
		*    Reference to the node that should be added
		*  @param[in] sName
		*    Node name
		*  @param[in] bInitNode
		*    Should the node be initialized?
		*  @param[in] nPosition
		*    Optional index position specifying the location within the child list where the scene node should be added, <0 for at the end
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If the node name is already used another unused node name will be used instead!
		*    - Use this function only in special cases!
		*/
		bool Add(SceneNode &cNode, const PLCore::String &sName = "", bool bInitNode = true, int nPosition = -1);

		/**
		*  @brief
		*    Removes a node
		*
		*  @param[in] cNode
		*    Reference to the node that should be removed
		*  @param[in] bDeInitNode
		*    Should the node be de-initialized?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - The node object itself is still there after it is de-initialized
		*      and removed from the manager!
		*    - Use this function only in special cases!
		*/
		bool Remove(SceneNode &cNode, bool bDeInitNode = true);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Private data
		PLCore::String	   m_sHierarchy;	/**< Class name of the scene container hierarchy */
		SceneContext	  *m_pSceneContext;	/**< The scene context this scene container is in (should be always valid!) */
		SceneHierarchy	  *m_pHierarchy;	/**< Scene hierarchy, can be a null pointer */
		SceneQueryManager *m_pQueryManager;	/**< Scene query manager, can be a null pointer */

		/** List of scene nodes which need a scene hierarchy refresh */
		PLCore::Pool<SceneNode*> m_lstHierarchyRefresh;


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void InitFunction() override;
		PLS_API virtual void DeInitFunction() override;
		PLS_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::ElementManager functions       ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool DeInit() override;
		PLS_API virtual SceneNode *GetByIndex(PLCore::uint32 nIndex = 0) const override;
		PLS_API virtual SceneNode *GetByName(const PLCore::String &sName) const override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool LoadByFilename(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "") override;
		PLS_API virtual bool LoadByFile(PLCore::File &cFile, const PLCore::String &sParams = "", const PLCore::String &sMethod = "") override;
		PLS_API virtual bool Unload() override;
		PLS_API virtual PLCore::String GetLoadableTypeName() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLCore::Loadable functions          ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual bool CallLoadable(PLCore::File &cFile, PLCore::Loader &cLoader, const PLCore::String &sMethod, const PLCore::String &sParams) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		PLS_API virtual SceneNode *CreateElement(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneContainer.inl"


#endif // __PLSCENE_SCENECONTAINER_H__
