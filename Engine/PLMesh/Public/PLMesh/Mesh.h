/*********************************************************\
 *  File: Mesh.h                                         *
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


#ifndef __PLMESH_MESH_H__
#define __PLMESH_MESH_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Resource.h>
#include <PLMath/Vector3.h>
#include "PLMesh/Weight.h"
#include "PLMesh/AnchorPoint.h"
#include "PLMesh/VertexWeights.h"
#include "PLMesh/MorphTargetAni.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
	class Material;
	class MaterialHandler;
}
namespace PLMesh {
	class Skeleton;
	class MeshManager;
	class MeshLODLevel;
	class MeshMorphTarget;
	class SkeletonHandler;
	class SkeletonManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mesh triangle
*/
class MeshTriangle {
	public:
	PLCore::uint32 nGeometry;		/**< Index of the geometry this triangle is in */
	PLCore::uint32 nVertex[3];		/**< The three vertex indices */
	int			  nNeighbour[3];	/**< Neighbor triangles */

	PLMESH_API MeshTriangle &operator =(const MeshTriangle &cSource);
	PLMESH_API bool operator ==(const MeshTriangle &cMeshTriangle) const;
};

/**
*  @brief
*    Mesh edge
*/
class MeshEdge {
	public:
	PLCore::uint32 nVertex[2];		/**< The two edge vertex indices */
	PLCore::uint32 nTriangle[2];	/**< The two edge triangles */

	PLMESH_API MeshEdge &operator =(const MeshEdge &cSource);
	PLMESH_API bool operator ==(const MeshEdge &cMeshEdge) const;
};

/**
*  @brief
*    Mesh class
*
*  @remarks
*    A mesh defining a 3D mesh consists of several information:\n
*    Animations    (list of predefined animations)\n
*    Anchor points (anchor points are in fact a kind of index to a vertex)\n
*    Morph targets (that contains the vertex data for every frame)\n
*    LOD levels    (each of which can define the geometries and/or index data)\n
*    Skeleton      (that defines a hierarchical structure on the mesh)\n
*    Materials     (list of materials used by the mesh)
*/
class Mesh : public PLCore::Resource<Mesh> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MeshManager;
	friend class MeshMorphTarget;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Mesh draw flags
		*/
		enum EFlags {
			DrawVertices = 1<<0,	/**< Draw vertices */
			DrawNormals  = 1<<1,	/**< Draw normals */
			DrawOctree   = 1<<2		/**< Draw (optional) octree */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Main functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~Mesh();

		/**
		*  @brief
		*    Returns the owner mesh manager
		*
		*  @return
		*    Pointer to the owner mesh manager, a null pointer on error
		*/
		PLMESH_API MeshManager *GetMeshManager() const;

		/**
		*  @brief
		*    Returns the skeleton manager
		*
		*  @return
		*    The skeleton manager
		*/
		PLMESH_API SkeletonManager &GetSkeletonManager();

		/**
		*  @brief
		*    Returns the used renderer
		*
		*  @return
		*    Pointer to the renderer the mesh uses, can be a null pointer
		*/
		PLMESH_API PLRenderer::Renderer *GetRenderer() const;

		/**
		*  @brief
		*    Checks all LODs and returns the maximum number of vertices
		*
		*  @return
		*    Maximum number of vertices
		*/
		PLMESH_API PLCore::uint32 GetMaxNumOfVertices() const;

		/**
		*  @brief
		*    Checks all LODs and returns the maximum number of geometries
		*
		*  @return
		*    Maximum number of geometries
		*/
		PLMESH_API PLCore::uint32 GetMaxNumOfGeometries() const;

		/**
		*  @brief
		*    Checks all LODs and returns the maximum number of triangles
		*
		*  @return
		*    Maximum number of triangles
		*/
		PLMESH_API PLCore::uint32 GetMaxNumOfTriangles() const;

		/**
		*  @brief
		*    Draws the mesh
		*
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] bBlend
		*    Draw only mesh parts which use a blended material?
		*  @param[in] nFlags
		*    Draw flags (use EFlags)
		*  @param[in] nLODLevel
		*    Which LOD level to use
		*  @param[in] nMorphTarget
		*    Which morph target to use
		*  @param[in] bUseMaterials
		*    Use the mesh materials? If 'false' no material is bound.
		*
		*  @note
		*    - Use this function only if you know that there's only one special
		*      usage of the mesh. (for instance you dynamically change it)
		*      Normally you should use a mesh handler to handle the mesh
		*/
		PLMESH_API void Draw(const PLMath::Matrix4x4 &mWorldViewProjection, bool bBlend = false, PLCore::uint32 nFlags = 0, PLCore::uint32 nLODLevel = 0,
							 PLCore::uint32 nMorphTarget = 0, bool bUseMaterials = true) const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This mesh
		*/
		PLMESH_API Mesh &operator =(const Mesh &cSource);

		//[-------------------------------------------------------]
		//[ Mesh data                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the anchor point manager
		*
		*  @return
		*    The anchor point manager of the mesh
		*
		*  @remarks
		*    To add an anchor point to the manager you can e.g. do the following:\n
		*    AnchorPoint *pAnchorPoint = new AnchorPoint("LeftHand", 0,\n
		*      99, &pMyMesh->GetAnchorPointManager());
		*/
		PLMESH_API AnchorPointManager &GetAnchorPointManager();

		//[-------------------------------------------------------]
		//[ Morph target functions                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Clears all morph targets
		*/
		PLMESH_API void ClearMorphTargets();

		/**
		*  @brief
		*    Returns the number of morph targets
		*
		*  @return
		*    Number of morph targets the mesh contains
		*/
		PLMESH_API PLCore::uint32 GetNumOfMorphTargets() const;

		/**
		*  @brief
		*    Adds a new morph target
		*
		*  @return
		*    Pointer to the new morph target, a null pointer on error
		*/
		PLMESH_API MeshMorphTarget *AddMorphTarget();

		/**
		*  @brief
		*    Gets one of the mesh's morph targets by index
		*
		*  @param[in] nMorphTarget
		*    Number of the morph target to get
		*
		*  @return
		*    Pointer to the morph target, or a null pointer
		*/
		PLMESH_API MeshMorphTarget *GetMorphTarget(PLCore::uint32 nMorphTarget = 0) const;

		/**
		*  @brief
		*    Gets one of the mesh's morph targets by name
		*
		*  @param[in] sName
		*    Name of the morph target to get
		*
		*  @return
		*    Pointer to the morph target, or a null pointer
		*/
		PLMESH_API MeshMorphTarget *GetMorphTarget(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Returns the index of a given morph target
		*
		*  @param[in] sName
		*    Name of the morph target the index should be returned
		*
		*  @return
		*    The index of the given morph target, < 0 if the morph target wasn't found
		*/
		PLMESH_API int GetMorphTargetIndex(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Returns the morph target animation manager
		*
		*  @return
		*    The morph target animation manager
		*
		*  @remarks
		*    To add an morph target animation to the manager you can e.g. do the following:\n
		*    MorphTargetAni *pAnimation = new MorphTargetAni(&pMyMesh->GetMorphTargetAnimationManager());
		*/
		PLMESH_API MorphTargetAniManager &GetMorphTargetAnimationManager();

		/**
		*  @brief
		*    Adds a morph target animation
		*
		*  @param[in] sFilename
		*    Morph target animation filename
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMESH_API bool AddMorphTargetAnimation(const PLCore::String &sFilename);

		//[-------------------------------------------------------]
		//[ LOD functions                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Clears all LOD levels
		*/
		PLMESH_API void ClearLODLevels();

		/**
		*  @brief
		*    Returns the number of LOD levels
		*
		*  @return
		*    Number of LOD levels the mesh contains
		*/
		PLMESH_API PLCore::uint32 GetNumOfLODLevels() const;

		/**
		*  @brief
		*    Adds a new LOD level
		*
		*  @return
		*    Pointer to the new LOD level, a null pointer on error
		*/
		PLMESH_API MeshLODLevel *AddLODLevel();

		/**
		*  @brief
		*    Gets one of the mesh's LOD levels
		*
		*  @param[in] nLODLevel
		*    Number of the LOD level to get
		*
		*  @return
		*    Pointer to the LOD level, or a null pointer
		*/
		PLMESH_API MeshLODLevel *GetLODLevel(PLCore::uint32 nLODLevel = 0) const;

		//[-------------------------------------------------------]
		//[ Material functions                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Clears all materials
		*/
		PLMESH_API void ClearMaterials();

		/**
		*  @brief
		*    Returns the number of materials
		*
		*  @return
		*    Number of materials
		*/
		PLMESH_API PLCore::uint32 GetNumOfMaterials() const;

		/**
		*  @brief
		*    Adds a new material
		*
		*  @param[in] pMaterial
		*    Material to add, if a null pointer, nothing happens
		*
		*  @return
		*    Pointer to the added material, a null pointer on error
		*/
		PLMESH_API PLRenderer::Material *AddMaterial(PLRenderer::Material *pMaterial);

		/**
		*  @brief
		*    Deletes a material
		*
		*  @param[in] nMaterial
		*    Number of the material to delete
		*
		*  @return
		*    'true' if all went fine else 'false'
		*
		*  @note
		*    - If the deleted material was used by a geometry the geometry
		*      material index will be set to 0
		*    - The material index of the geometries will automatically corrected
		*      if the material index has changed
		*/
		PLMESH_API bool DeleteMaterial(PLCore::uint32 nMaterial);

		/**
		*  @brief
		*    Returns the number of geometries using this material
		*
		*  @param[in] nMaterial
		*    Number of the material to check
		*
		*  @return
		*    Number of geometries using this material
		*
		*  @note
		*    - Use this function for instance to find unused materials in order to
		*      delete them
		*/
		PLMESH_API PLCore::uint32 GetMaterialUsage(PLCore::uint32 nMaterial) const;

		/**
		*  @brief
		*    Gets one of the mesh's materials
		*
		*  @param[in] nMaterial
		*    Number of the material to get
		*
		*  @return
		*    Pointer to the material, or a null pointer
		*/
		PLMESH_API PLRenderer::Material *GetMaterial(PLCore::uint32 nMaterial = 0) const;

		/**
		*  @brief
		*    Sets one of the mesh's materials
		*
		*  @param[in] nMaterial
		*    Number of the material to set
		*  @param[in] pMaterial
		*    Pointer to the material to set, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMESH_API bool SetMaterial(PLCore::uint32 nMaterial, PLRenderer::Material *pMaterial);

		//[-------------------------------------------------------]
		//[ Skeleton functions                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Clears the list of skeleton handlers
		*
		*  @note
		*    - The skeletons are managed by the global skeleton manager, the meshes only
		*      can have a list of skeletons 'assigned' to them. This skeletons for instance
		*      are saved/loaded directly within the mesh file.
		*    - By default the mesh has no skeleton assigned to it
		*    - The first skeleton is the base skeleton, all other are for instance
		*      additional animations assigned with this mesh
		*    - See MeshLODLevel::GetVertexWeights()
		*/
		PLMESH_API void ClearSkeletonHandlers();

		/**
		*  @brief
		*    Returns the list of skeleton handlers
		*  
		*  @return
		*    The list of skeleton handlers
		*
		*  @see
		*    - ClearSkeletonHandlers()
		*/
		PLMESH_API PLCore::Array<SkeletonHandler*> &GetSkeletonHandlers();

		/**
		*  @brief
		*    Returns the weights array
		*
		*  @return
		*    Weights array
		*
		*  @note
		*    - The weights are optional, but required for mesh skinning
		*/
		PLMESH_API PLCore::Array<Weight> &GetWeights();

		/**
		*  @brief
		*    Returns the vertex weights array
		*
		*  @return
		*    Vertex weights array
		*
		*  @note
		*    - The number of vertices in this array should always be the same number
		*      as the number of vertices within the vertex buffer of this LOD level!
		*     (see MeshMorphTarget::GetVertexBuffer())
		*    - The vertex weights are optional, but required for mesh skinning
		*/
		PLMESH_API PLCore::Array<VertexWeights> &GetVertexWeights();

		//[-------------------------------------------------------]
		//[ Tool functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Builds the connectivity information of all LOD levels
		*
		*  @note
		*    - For more information have a look at MeshLODLevel::BuildConnectivity()
		*/
		PLMESH_API void BuildConnectivity();

		/**
		*  @brief
		*    Builds the current triangle planes of all morph targets
		*
		*  @note
		*    - For more information have a look at MeshMorphTarget::BuildTrianglePlaneList()
		*/
		PLMESH_API void BuildTrianglePlaneList();

		/**
		*  @brief
		*    Builds the LOD levels automatically
		*
		*  @param[in] nNumLODLevels
		*    Number of LOD levels to create (including the original)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - The first LOD level and morph target must exist already
		*/
		PLMESH_API bool BuildLOD(PLCore::uint32 nNumLODLevels);

		/**
		*  @brief
		*    Calculate the vertex normals of all morph targets of the mesh
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This function will add normals to the vertex buffer if there are no one
		*      allocated yet
		*/
		PLMESH_API bool CalculateNormals();

		/**
		*  @brief
		*    Calculates all tangent space vectors of all morph targets of the mesh
		*
		*  @param[in] bTangent
		*    Create tangent vectors?
		*  @param[in] bBinormal
		*    Create binormal vectors?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This function will add tangents and binormals to the
		*      vertex buffer if there are no one allocated yet
		*    - If there are no normals available they will be computed (see CalculateNormals())
		*    - To save memory one can only hold tangent vectors and compute the binormal for
		*      instance within a vertex shader using a cross product of the normal and
		*      tangent vector
		*/
		PLMESH_API bool CalculateTangentSpaceVectors(bool bTangent = true, bool bBinormal = true);

		/**
		*  @brief
		*    Returns the mesh bounding box
		*
		*  @param[out] vMin
		*    Will receive the minimum bounding box position
		*  @param[out] vMax
		*    Will receive the maximum bounding box position
		*
		*  @remarks
		*    This bounding box should enclose the whole mesh. If it is a 0-bounding box
		*    you can use CalculateBoundingBox() to calculate a valid bounding box.
		*/
		PLMESH_API void GetBoundingBox(PLMath::Vector3 &vMin, PLMath::Vector3 &vMax) const;

		/**
		*  @brief
		*    Set's the mesh bounding box
		*
		*  @param[in] vMin
		*    The minimum bounding box position
		*  @param[in] vMax
		*    The maximum bounding box position
		*
		*  @see
		*    - SetBoundingBox()
		*/
		PLMESH_API void SetBoundingBox(const PLMath::Vector3 &vMin, const PLMath::Vector3 &vMax);

		/**
		*  @brief
		*    Calculates the mesh bounding box
		*
		*  @param[out] vMin
		*    Will receive the minimum bounding box position
		*  @param[out] vMax
		*    Will receive the maximum bounding box position
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This function will take all morph targets into account
		*    - The internal mesh bounding box is NOT manipulated!
		*/
		PLMESH_API bool CalculateBoundingBox(PLMath::Vector3 &vMin, PLMath::Vector3 &vMax);

		/**
		*  @brief
		*    Calculates the mesh bounding sphere
		*
		*  @param[out] vPos
		*    Will receive the bounding sphere position
		*  @param[out] fRadius
		*    Will receive the bounding sphere radius
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This function will take all morph targets into account
		*/
		PLMESH_API bool CalculateBoundingSphere(PLMath::Vector3 &vPos, float &fRadius);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pRenderer
		*    Pointer to the renderer to use can be a null pointer
		*  @param[in] cManager
		*    Mesh manager using this resource
		*  @param[in] sName
		*    Resource name to set
		*  @param[in] bStatic
		*    Static mesh? (better performance!)
		*/
		Mesh(PLRenderer::Renderer *pRenderer, PLCore::ResourceManager<Mesh> &cManager, const PLCore::String &sName, bool bStatic = true);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Data
		PLRenderer::Renderer *m_pRenderer;	/**< Renderer the mesh is using, can be a null pointer */
		bool				  m_bStatic;	/**< Static mesh? (better performance!) */
		PLMath::Vector3		  m_vBBMin;		/**< Minimum bounding box position */
		PLMath::Vector3		  m_vBBMax;		/**< Maximum bounding box position */

		/** Anchor points */
		AnchorPointManager m_cAnchorPointManager;

		/** Morph targets */
		PLCore::Array<MeshMorphTarget*>					  m_lstMorphTargets;
		MorphTargetAniManager							  m_cMorphTargetAnimation;
		PLCore::HashMap<PLCore::String, MeshMorphTarget*> m_mapMorphTargets;

		/** LOD data */
		PLCore::Array<MeshLODLevel*> m_lstLODLevels;

		/** Materials */
		PLCore::Array<PLRenderer::MaterialHandler*> m_lstMaterials;

		/** Skeleton */
		SkeletonManager				    *m_pSkeletonManager;	/**< Skeleton manager, can be a null pointer */
		PLCore::Array<SkeletonHandler*>  m_lstSkeletonHandler;	/**< List of skeleton handlers */
		PLCore::Array<Weight>		     m_lstWeights;			/**< Optional weights */
		PLCore::Array<VertexWeights>	 m_lstVertexWeights;	/**< Optional vertex weights per vertex */


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API virtual bool Unload() override;
		PLMESH_API virtual PLCore::String GetLoadableTypeName() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESH_H__
