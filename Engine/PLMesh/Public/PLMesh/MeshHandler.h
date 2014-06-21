/*********************************************************\
 *  File: MeshHandler.h                                  *
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


#ifndef __PLMESH_MESHHANDLER_H__
#define __PLMESH_MESHHANDLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLCore/Container/Bitset.h>
#include <PLCore/Container/ResourceManager.h>
#include <PLCore/Container/ResourceHandler.h>
#include <PLMath/Plane.h>
#include <PLRenderer/Renderer/Types.h>
#include "PLMesh/Mesh.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector3;
	class PlaneSet;
	class AABoundingBox;
}
namespace PLGraphics {
	class Color4;
}
namespace PLRenderer {
	class Font;
	class Material;
	class Renderer;
	class Animation;
	class IndexBuffer;
	class VertexBuffer;
	class AnimationInfo;
	class MaterialHandler;
}
namespace PLMesh {
	class Mesh;
	class MeshEdge;
	class JointHandler;
	class MeshTriangle;
	class MeshLODLevel;
	class SkeletonHandler;
	class MeshAnimationManager;
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
*    Mesh handler
*
*  @remarks
*    A mesh handler will use a mesh and is able to use own materials and an own
*    vertex buffer for e.g. animations. Whenever working with meshes you should use
*    a mesh handler!\n
*    Animation example\n
*    pMH->CreateMeshAnimationManager(); // Create a mesh animation manager first!\n
*    Animation *pAnimation = pMH->GetMeshAnimationManager()->Create("attack");\n
*    pAnimation->Start(pMH->GetAnimationInfo(pAnimation->GetName()));
*
*  @note
*    - Whenever you manipulated mesh data "from outside" you need to call "MeshUpdateRequired()" to set a "dirty"-flag
*    - Animation frame changes automatically set the internal 'mesh update required'-flag
*/
class MeshHandler : public PLCore::ResourceHandler<Mesh> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Main functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API MeshHandler();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshHandler();

		/**
		*  @brief
		*    Returns the used renderer
		*
		*  @return
		*    Pointer to the renderer that is used, can be a null pointer
		*/
		PLMESH_API PLRenderer::Renderer *GetRenderer() const;

		/**
		*  @brief
		*    Whenever you manipulated mesh data "from outside" you need to call this function to set a "dirty"-flag
		*/
		PLMESH_API void MeshUpdateRequired();

		/**
		*  @brief
		*    Returns the used mesh
		*
		*  @return
		*    Pointer to the used mesh, can be a null pointer
		*
		*  @note
		*    - Call "MeshUpdateRequired()" to set a "dirty"-flag if you manipulated the mesh
		*/
		PLMESH_API Mesh *GetMesh() const;

		/**
		*  @brief
		*    Sets the used mesh
		*
		*  @param[in] pMesh
		*    Pointer to the mesh which should be used, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - The mesh handler materials are set to the mesh materials, therefore
		*      first load/setup a mesh and then use this function!
		*    - After a mesh itself was manipulated, for instance the material list was
		*      changed you should reset the mesh in your mesh handler!
		*/
		PLMESH_API bool SetMesh(Mesh *pMesh = nullptr);

		/**
		*  @brief
		*    Updates the mesh handler
		*
		*  @param[in] fTimeDifference
		*    Past time since last frame (use e.g. PLCore::Timing::GetInstance()->GetTimeDifference())
		*  @param[in] nLODLevel
		*    LOD level to use
		*
		*  @remarks
		*    This function for instance updates the animations but will also setup
		*    individual mesh data required for drawing the mesh the mesh handler is
		*    using.
		*/
		PLMESH_API void Update(float fTimeDifference, PLCore::uint32 nLODLevel = 0);

		//[-------------------------------------------------------]
		//[ Draw functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Draws the mesh
		*
		*  @param[in] bBlend
		*    Draw only mesh parts which use a blended material?
		*  @param[in] bUseMaterials
		*    Use the mesh materials? If 'false' no material is bound.
		*
		*  @note
		*    - Update() must have been called before the mesh can be drawn because
		*      this function setup the individual mesh data
		*/
		PLMESH_API void Draw(bool bBlend = false, bool bUseMaterials = true) const;

		/**
		*  @brief
		*    Draws the vertices of the mesh (for debugging)
		*  @param[in] cColor
		*    Color to use
		*
		*  @note
		*    - Ensure that correct render states are set...
		*/
		PLMESH_API void DrawVertices(const PLGraphics::Color4 &cColor) const;

		/**
		*  @brief
		*    Draws the normals of the mesh (for debugging)
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] fScale
		*    Normal scale
		*  @param[in] fLineWidth
		*    Line width
		*
		*  @see
		*    - DrawVertices()
		*/
		PLMESH_API void DrawNormals(const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fScale = 1.0f, float fLineWidth = 1.0f) const;

		/**
		*  @brief
		*    Draws the tangents of the mesh (for debugging)
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] fScale
		*    Tangent scale
		*  @param[in] fLineWidth
		*    Line width
		*
		*  @see
		*    - DrawVertices()
		*/
		PLMESH_API void DrawTangents(const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fScale = 1.0f, float fLineWidth = 1.0f) const;

		/**
		*  @brief
		*    Draws the binormals of the mesh (for debugging)
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] fScale
		*    Binormals scale
		*  @param[in] fLineWidth
		*    Line width
		*
		*  @see
		*    - DrawVertices()
		*/
		PLMESH_API void DrawBinormals(const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fScale = 1.0f, float fLineWidth = 1.0f) const;

		/**
		*  @brief
		*    Draws the vertex numbers of the mesh (for debugging)
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*
		*  @see
		*    - DrawVertices()
		*/
		PLMESH_API void DrawVertexNumbers(PLRenderer::Font &cFont, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection) const;

		/**
		*  @brief
		*    Draws the anchor points of the mesh (for debugging)
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*
		*  @see
		*    - DrawVertices()
		*/
		PLMESH_API void DrawAnchorPoints(PLRenderer::Font &cFont, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection) const;

		//[-------------------------------------------------------]
		//[ Material functions                                    ]
		//[-------------------------------------------------------]
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
		*    Gets one of the mesh handler's materials
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
		*    Sets one of the mesh handler's materials
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
		//[ Morph target functions                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns an array holding the the base weight of each morph target
		*
		*  @return
		*    Morph target base weights
		*
		*  @see
		*    - Have a look at the mesh class Mesh for more morph target stuff
		*
		* @note
		*    - Use whenever possible the 'const' version of this function (performance)
		*    - The none 'const' version automatically sets the internal 'mesh update required'-flag
		*/
		PLMESH_API PLCore::Array<float> &GetBaseMorphTargetWeights();
		PLMESH_API const PLCore::Array<float> &GetBaseMorphTargetWeights() const;

		/**
		*  @brief
		*    Returns an array holding the the current weight of each morph target
		*
		*  @return
		*    Current morph target weights
		*
		*  @note
		*    - This current weights are changed automatically through animations, use
		*      the base weights to set the 'general' base weight
		*
		*  @see
		*    - GetBaseMorphTargetWeights()
		*
		* @note
		*    - Use whenever possible the 'const' version of this function (performance)
		*    - The none 'const' version automatically sets the internal 'mesh update required'-flag
		*/
		PLMESH_API PLCore::Array<float> &GetMorphTargetWeights();
		PLMESH_API const PLCore::Array<float> &GetMorphTargetWeights() const;

		//[-------------------------------------------------------]
		//[ Animation functions                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the skeleton handler
		*
		*  @return
		*    The mesh handlers skeleton handler, a null pointer if there's no such handler
		*
		*  @note
		*    - Call "MeshUpdateRequired()" to set a "dirty"-flag if you manipulated the skeleton
		*/
		PLMESH_API SkeletonHandler *GetSkeletonHandler() const;

		/**
		*  @brief
		*    Returns the mesh animation manager
		*
		*  @return
		*    The mesh handlers animation manager, a null pointer if there's no such manager
		*
		*  @note
		*    - By default the mesh handler has no animation manager, you have
		*      to create a manager using CreateMeshAnimationManager() by yourself!
		*/
		PLMESH_API MeshAnimationManager *GetMeshAnimationManager() const;
		
		/**
		*  @brief
		*    Creates the mesh animation manager
		*
		*  @param[in] sName
		*    Name of the mesh animation manager class to use, empty to
		*    destroy the actual manager without creating a new one
		*
		*  @return
		*    The created mesh handlers animation manager, a null pointer on error
		*    (maybe unknown class or the class is not derived from 'PLMesh::MeshAnimationManager')
		*/
		PLMESH_API MeshAnimationManager *CreateMeshAnimationManager(const PLCore::String &sName = "PLMesh::MeshAnimationManagerSoftware");

		/**
		*  @brief
		*    Returns a list of all currently available animations
		*
		*  @param[out] lstAnimations
		*    Receives a list of all currently available animations (list is cleared automatically before it's filled)
		*
		*  @see
		*    - GetAnimationInfo()
		*/
		PLMESH_API void GetAnimationsList(PLCore::Array<PLCore::String> &lstAnimations) const;

		/**
		*  @brief
		*    Returns a pointer to the requested animation information
		*
		*  @param[in] sName
		*    Name of the animation information
		*  @param[in] nLogMessage
		*    Should a warning message be written into the log if the requested
		*    animation information wasn't found? If < 0, write no log message,
		*    else the integer indicates the debug mode where the message should be written.
		*
		*  @return
		*    Pointer to the requested animation information, a null pointer if there was an error
		*
		*  @remarks
		*    Using this function you are e.g. able to backup pointers to the animation information\n
		*    in a quit comfortable way for instance in an scene node:\n
		*    m_pStandAnimation = GetMeshHandler()->GetAnimationInfo("Stand");
		*
		*  @note
		*    - This function will search for the requested animation information within the skeleton
		*      manager and the morph targets of the mesh the mesh handler is using.
		*/
		PLMESH_API PLRenderer::AnimationInfo *GetAnimationInfo(const PLCore::String &sName, int nLogMessage = 1) const;

		/**
		*  @brief
		*    Returns a current joint handler
		*
		*  @param[in] sJointName
		*    Name of the joint
		*
		*  @return
		*    The requested current joint handler, a null pointer on error
		*
		*  @note
		*    - Call "MeshUpdateRequired()" to set a "dirty"-flag if you manipulated the skeleton
		*/
		PLMESH_API JointHandler *GetJointHandler(const PLCore::String &sJointName) const;

		/**
		*  @brief
		*    Returns a base joint handler
		*
		*  @param[in] sJointName
		*    Name of the joint
		*
		*  @return
		*    The requested base joint handler, a null pointer on error
		*
		*  @note
		*    - Using this base joint handler you can for instance change the initial joint
		*      settings
		*    - Call "MeshUpdateRequired()" to set a "dirty"-flag if you manipulated the skeleton
		*/
		PLMESH_API JointHandler *GetBaseJointHandler(const PLCore::String &sJointName) const;

		//[-------------------------------------------------------]
		//[ Visibility functions                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Updates the mesh visibility
		*
		*  @param[in] cPlaneSet
		*    Plane set we want to check against - the planes must be in the object space
		*  @param[in] bFirst
		*    First visibility determination or add visible things?
		*
		*  @note
		*    - Current LOD level is used (see Update())
		*    - Use GetGeometryVisibility() to get the visibility of each mesh geometry
		*/
		PLMESH_API void UpdateVisibility(const PLMath::PlaneSet &cPlaneSet, bool bFirst = true);

		/**
		*  @brief
		*    Returns the bit set holding the geometry visibility state
		*
		*  @return
		*    Bitset holding the geometry visibility state
		*
		*  @note
		*    - Call UpdateVisibility() to update the visibility stuff
		*/
		PLMESH_API PLCore::Bitset &GetGeometryVisibility();

		//[-------------------------------------------------------]
		//[ LOD functions                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of mesh LOD levels
		*
		*  @return
		*    The number of mesh LOD levels
		*
		*  @see
		*    - Update()
		*/
		PLMESH_API PLCore::uint32 GetNumOfLODLevels() const;

		/**
		*  @brief
		*    Returns the current used mesh LOD level index
		*
		*  @return
		*    The current used LOD level index
		*
		*  @see
		*    - Update()
		*/
		PLMESH_API PLCore::uint32 GetLODLevelIndex() const;

		/**
		*  @brief
		*    Returns the current used mesh LOD level
		*
		*  @return
		*    The current used LOD level, a null pointer on error
		*
		*  @note
		*    - Don't manipulate the LOD level!
		*
		*  @see
		*    - Update()
		*/
		PLMESH_API MeshLODLevel *GetLODLevel() const;

		//[-------------------------------------------------------]
		//[ Tool functions                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Calculates the current axis aligned joint bounding box
		*
		*  @param[out] cAAB
		*    Will receive the current axis aligned joint bounding box
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMESH_API bool CalculateJointBoundingBox(PLMath::AABoundingBox &cAAB) const;

		/**
		*  @brief
		*    Returns the current used vertex buffer of the mesh
		*
		*  @return
		*    Current used vertex buffer of the mesh, can be a null pointer
		*
		*  @note
		*    - You shouldn't manipulate this vertex buffer - use it for read only!
		*      Internally the mesh handler will use the meshes vertex buffer directly
		*      whenever possible, but if there are e.g. animations the mesh handler has
		*      it's own vertex buffer with the current individual data.
		*
		*  @see
		*    - Update()
		*/
		PLMESH_API PLRenderer::VertexBuffer *GetVertexBuffer() const;

		/**
		*  @brief
		*    Returns the total number of current triangles
		*
		*  @return
		*    Total number of current triangles
		*/
		PLMESH_API PLCore::uint32 GetNumOfTriangles() const;

		/**
		*  @brief
		*    Returns the first mesh triangle intersecting the line
		*
		*  @param[in]  vLineStartPos
		*    Line start position
		*  @param[in]  vLineEndPos
		*    Line end position
		*  @param[out] nTriangle
		*    Will receive the ID of the found triangle
		*  @param[out] pnGeometry
		*    Receives the owner geometry ID of the found triangle if not a null pointer
		*  @param[out] pvCollisionPoint
		*    Will receive the collision point if not a null pointer
		*  @param[in]  plstGeometries
		*    List of geometry indices to use, if a null pointer all geometries are used
		*  @param[in]  nCull
		*    Cull mode (see "PLRenderer::Cull")
		*
		*  @return
		*    'true' if a triangle was found, else 'false'
		*
		*  @note
		*    - The line is assumed to be in the object space of the mesh handlers mesh
		*/
		PLMESH_API bool FindTriangle(const PLMath::Vector3 &vLineStartPos,
									 const PLMath::Vector3 &vLineEndPos,
									 PLCore::uint32 &nTriangle, PLCore::uint32 *pnGeometry = nullptr,
									 PLMath::Vector3 *pvCollisionPoint = nullptr, PLCore::Array<PLCore::uint32> *plstGeometries = nullptr,
									 PLRenderer::Cull::Enum nCull = PLRenderer::Cull::CCW) const;

		/**
		*  @brief
		*    Returns a list of mesh geometries intersecting the plane set
		*
		*  @param[in]  cPlaneSet
		*    Plane set we want to check against
		*  @param[out] ppnGeometries
		*    If not a null pointer, receives the geometries intersecting the plane set.
		*    You have to delete this list by self after usage! (delete [] ...)
		*  @param[in]  pvCamDir
		*    Camera direction vector, if not null faces where the normal is
		*    facing away from the given camera direction are ignored
		*
		*  @return
		*    Number of found geometries if all went fine, else 0
		*
		*  @note
		*    - The the planes must be in the object space
		*/
		PLMESH_API PLCore::uint32 FindGeometries(const PLMath::PlaneSet &cPlaneSet, PLCore::uint32 **ppnGeometries = nullptr, PLMath::Vector3 *pvCamDir = nullptr) const;

		/**
		*  @brief
		*    Gets the triangle list
		*
		*  @return
		*    Triangle list, a null pointer on error
		*
		*  @see
		*    - MeshLODLevel::BuildTriangleList()
		*/
		PLMESH_API PLCore::Array<MeshTriangle> *GetTriangleList();

		/**
		*  @brief
		*    Gets the edge list
		*
		*  @return
		*    Edge list, a null pointer on error
		*
		*  @see
		*    - MeshLODLevel::BuildEdgeList()
		*/
		PLMESH_API PLCore::Array<MeshEdge> *GetEdgeList();

		/**
		*  @brief
		*    Builds the current triangle planes
		*
		*  @note
		*    - The triangle planes must be updated after manipulating
		*      geometries or vertices!
		*    - If the triangle list is available (MeshLODLevel::GetTriangleList())
		*      it will be used for faster triangle plane calculation, else the slower
		*      GetTriangle() is used instead.
		*/
		PLMESH_API void BuildTrianglePlaneList();

		/**
		*  @brief
		*    Gets the triangle plane list
		*
		*  @return
		*    Triangle plane list, a null pointer on error
		*
		*  @note
		*    - If the mesh handler currently is using the original morph target vertex
		*      data the triangle planes of this morph target will be returned. But
		*      if the mesh handler is using an animated mesh it will need it's individual
		*      triangle planes!
		*
		*  @see
		*    - See BuildTrianglePlaneList()
		*/
		PLMESH_API PLCore::Array<PLMath::Plane> *GetTrianglePlaneList();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Clears all materials
		*/
		void ClearMaterials();

		/**
		*  @brief
		*    Adds a new material
		*
		*  @param[in] pMaterial
		*    Material to add, can be a null pointer
		*
		*  @return
		*    Pointer to the added material, a null pointer on error
		*/
		PLRenderer::Material *AddMaterial(PLRenderer::Material *pMaterial);

		/**
		*  @brief
		*    Called when the frame of an animation manager animation has been changed
		*/
		void OnAnimationFrameChange();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerAnimationFrameChange;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer						*m_pRenderer;		/**< Used renderer, can be a null pointer */
		Mesh										*m_pMesh;			/**< Used mesh, can be a null pointer */
		PLCore::Array<PLRenderer::MaterialHandler*>  m_lstMaterials;	/**< Materials */
		PLRenderer::VertexBuffer					*m_pVertexBuffer;	/**< Mesh handler own vertex buffer (optional, can be a null pointer) */

		// Current stuff
		bool					  m_bMeshUpdateRequired;		/**< Mesh update required */
		PLCore::uint32			  m_nLOD;						/**< Current LOD level */
		PLRenderer::VertexBuffer *m_pCurrentVertexBuffer;		/**< Current vertex buffer to use, can be a null pointer */
		PLCore::Bitset			  m_cGeometryVisibility;		/**< Holds which geometry is visible and which not */
		PLCore::Array<float>	  m_lstBaseMorphTargetWeights;	/**< Morph target base weights */
		PLCore::Array<float>	  m_lstMorphTargetWeights;		/**< Current morph target weights */
		SkeletonHandler			 *m_pSkeletonHandler;			/**< Skeleton handler, can be a null pointer */
		MeshAnimationManager	 *m_pMeshAnimationManager;		/**< Mesh animation manager, can be a null pointer */

		// Precalculated data
		bool						 m_bRecalculateTrianglePlanes;	/**< Do we need to recalculate the triangle planes? */
		PLCore::Array<PLMath::Plane> m_lstTrianglePlanes;			/**< List of triangle planes */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHHANDLER_H__
