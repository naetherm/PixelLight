/*********************************************************\
 *  File: SNMesh.h                                       *
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


#ifndef __PLSCENE_SCENENODE_MESH_H__
#define __PLSCENE_SCENENODE_MESH_H__
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
*    An mesh is a special scene node that has a 3D mesh attached to it by default
*
*  @note
*    - If 'Skin' is not empty and not a skin file, all materials are replaced through the new one
*    - By default, invisible scene nodes internally don't load in their mesh at once and delay this until
*      the scene node becomes visible and/or 'GetMeshHandler()' is called (use 'LoadAtOnce'-flag do force loading at once)
*    - For the debug visualization of normal vectors the config setting 'EngineDebugConfig->NormalScale' is
*      used to scale the length
*/
class SNMesh : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoAutomaticBoundingBox = 1<<10,	/**< Do not create the bounding box of the scene node automatically using the bounding box of the used mesh */
			LoadAtOnce			   = 1<<11,	/**< Load the mesh at once even if not used at the moment */
			DynamicMesh			   = 1<<12,	/**< Hint that this is a dynamic mesh, may have an impact on the performance */
			GenerateStrips		   = 1<<13,	/**< Generate triangle strips? (if possible, don't use this flag -> loading times...) */
			CalculateNormals	   = 1<<14,	/**< Calculate normal vectors? (if not already there) (if possible, don't use this flag -> loading times...) */
			CalculateTSVs		   = 1<<15	/**< Calculate tangent space vectors? (if not already there) (if possible, don't use this flag -> loading times...) */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(NoAutomaticBoundingBox,	"Do not create the bounding box of the scene node automatically using the bounding box of the used mesh")
			pl_enum_value(LoadAtOnce,				"Load the mesh at once even if not used at the moment")
			pl_enum_value(DynamicMesh,				"Hint that this is a dynamic mesh, may have an impact on the performance")
			pl_enum_value(GenerateStrips,			"Generate triangle strips? (if possible, don't use this flag -> loading times...)")
			pl_enum_value(CalculateNormals,			"Calculate normal vectors? (if not already there) (if possible, don't use this flag -> loading times...)")
			pl_enum_value(CalculateTSVs,			"Calculate tangent space vectors? (if not already there) (if possible, don't use this flag -> loading times...)")
		pl_enum_end

		/**
		*  @brief
		*    Scene node debug flags (SceneNode debug flags extension)
		*/
		enum EDebugFlags {
			// Mesh
			DebugShowWireframe      = 1<<8,		/**< Draw wireframe */
			DebugShowVertices       = 1<<9,		/**< Draw vertices */
			DebugShowTangents       = 1<<10,	/**< Draw tangents (red = x/u) */
			DebugShowBinormals      = 1<<11,	/**< Draw binormals (green = y/v) */
			DebugShowNormals        = 1<<12,	/**< Draw normals (blue = z/w) */
			DebugShowVertexNumbers  = 1<<13,	/**< Draw vertex numbers */
			// Skeleton (optional) information
			DebugShowJoints         = 1<<14,	/**< Draw joints */
			DebugShowJointNames     = 1<<15,	/**< Draw joint names */
			DebugShowSkeleton       = 1<<16,	/**< Draw skeleton */
			// Misc
			DebugShowAnchorPoints   = 1<<17		/**< Draw anchor points */
		};
		pl_enum(EDebugFlags)
			pl_enum_base(SceneNode::EDebugFlags)
			// Mesh
			pl_enum_value(DebugShowWireframe,		"Draw wireframe")
			pl_enum_value(DebugShowVertices,		"Draw vertices")
			pl_enum_value(DebugShowTangents,		"Draw tangents (red = x/u)")
			pl_enum_value(DebugShowBinormals,		"Draw binormals (green = y/v)")
			pl_enum_value(DebugShowNormals,			"Draw normals (blue = z/w)")
			pl_enum_value(DebugShowVertexNumbers,	"Draw vertex numbers")
			// Skeleton (optional) information
			pl_enum_value(DebugShowJoints,			"Draw joints")
			pl_enum_value(DebugShowJointNames,		"Draw joint names")
			pl_enum_value(DebugShowSkeleton,		"Draw skeleton")
			// Misc
			pl_enum_value(DebugShowAnchorPoints,	"Draw anchor points")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMesh, "PLScene", PLScene::SceneNode, "An mesh is a special scene node that has a 3D mesh attached to it by default")
		// Properties
		pl_properties
			pl_property("Icon",	"Data/Textures/IconMesh.dds")
		pl_properties_end
		// Attributes
		pl_attribute(Mesh,			PLCore::String,				"",	ReadWrite,	GetSet,	"Mesh to use",																		"Type='Mesh'")
		pl_attribute(Skin,			PLCore::String,				"",	ReadWrite,	GetSet,	"Skin file or material name overwriting the default materials of the used mesh",	"Type='Skin'")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),		0,	ReadWrite,	GetSet,	"Flags",																			"")
		pl_attribute(DebugFlags,	pl_flag_type(EDebugFlags),	0,	ReadWrite,	GetSet,	"Debug flags",																		"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetMesh() const;
		PLS_API void SetMesh(const PLCore::String &sValue);
		PLS_API PLCore::String GetSkin() const;
		PLS_API void SetSkin(const PLCore::String &sValue);
		PLS_API virtual void SetFlags(PLCore::uint32 nValue);	// From SceneNode


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNMesh();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMesh();

		/**
		*  @brief
		*    Loads a mesh
		*
		*  @param[in] sFilename
		*    Mesh filename to load
		*  @param[in] sParams
		*    Optional load method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the load method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If this entity is initialized the mesh defined in the entity variable
		*      m_sMesh is loaded automatically
		*/
		PLS_API bool LoadMesh(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "");

		/**
		*  @brief
		*    Loads a skin
		*
		*  @param[in] sFilename
		*    Skin filename to load, if empty, the default materials of the given mesh are used
		*  @param[in] sParams
		*    Optional load method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the load method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool LoadSkin(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "");

		/**
		*  @brief
		*    Saves the skin
		*
		*  @param[in] sFilename
		*    Skin filename
		*  @param[in] sParams
		*    Optional save method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the save method to use, can be an empty string
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool SaveSkin(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "");

		/**
		*  @brief
		*    Unloads the current mesh
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool UnloadMesh();

		/*
		*  @brief
		*    Returns the world position of a skeleton joint
		*
		*  @param[in]  sJointName
		*    Name of the skeleton joint to return the world position from
		*  @param[out] vPosition
		*    On success this will receive the world position of the requested skeleton joint
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (There's no mesh? There's no skeleton? There's no joint with the given name?)
		*/
		PLS_API bool GetSkeletonJointWorldPosition(const PLCore::String &sJointName, PLMath::Vector3 &vPosition);


	//[-------------------------------------------------------]
	//[ Protected virtual SNMesh functions                    ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    This function is called after the mesh handler loads a mesh
		*
		*  @note
		*    - Use e.g. to get animation information
		*/
		PLS_API virtual void MeshInitFunction();

		/**
		*  @brief
		*    This function is called after the mesh handler unloads a mesh
		*
		*  @note
		*    - The default implementation is empty
		*
		*  @see
		*    - MeshInitFunction()
		*/
		PLS_API virtual void MeshDeInitFunction();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String		 m_sMesh;			/**< Mesh to use */
		PLCore::String		 m_sSkin;			/**< Skin file overwriting the default materials of the used mesh */
		PLMesh::MeshHandler *m_pMeshHandler;	/**< The mesh handler, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual PLMesh::MeshHandler *GetMeshHandler() override;
		PLS_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void DeInitFunction() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_MESH_H__
