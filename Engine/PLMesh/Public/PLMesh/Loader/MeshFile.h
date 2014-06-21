/*********************************************************\
 *  File: MeshFile.h                                     *
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


#ifndef __PLMESH_MESHFILE_H__
#define __PLMESH_MESHFILE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/PLCore.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PL mesh file format (binary, Little-Endian)
*
*  @verbatim
  PixelLight mesh format description:

	MESHFILE
	|
	+--PLCore::uint32 nMagic
	|
	+--PLCore::uint32 nVersion
	|
	+--MATERIALS
	|  |
	|  +--PLCore::uint32 nMaterials
	|  |
	|  +--char szName[256][nMaterials]
	|
	+--MESH
	|  |
	|  +--PLCore::uint32 nLODLevels
	|  |
	|  +--PLCore::uint32 nMorphTargets
	|  |
	|  +--PLCore::uint32 nWeights
	|  |
	|  +--PLCore::uint32 nVertexWeights
	|  |
	|  +--LODLEVEL (*)
	|  |  |
	|  |  +--float fDistance
	|  |  |
	|  |  +--PLCore::uint32 nGeometries
	|  |  |
	|  |  +--PLCore::uint32 nOctreeSubdivide
	|  |  |
	|  |  +--PLCore::uint32 nOctreeMinGeometries
	|  |  |
	|  |  +--INDEXBUFFER
	|  |  |  |
	|  |  |  +--PLCore::uint32 nElementType
	|  |  |  |
	|  |  |  +--PLCore::uint32 nElements
	|  |  |  |
	|  |  |  +--PLCore::uint32 nSize;
	|  |  |  |
	|  |  |  +--... 'nElements' indices of type 'nElementType'
	|  |  |
	|  |  +--GEOMETRY (*)
	|  |  |  |
	|  |  |  +--char szName[64]
	|  |  |  |
	|  |  |  +--PLCore::uint32 nFlags
	|  |  |  |
	|  |  |  +--bool bActive
	|  |  |  |
	|  |  |  +--int nPrimitiveType
	|  |  |  |
	|  |  |  +--int nMaterial
	|  |  |  |
	|  |  |  +--int nStartIndex
	|  |  |  |
	|  |  |  +--int nIndexSize
	|  |  |
	|  |  +--... 'nGeometries' geometries
	|  |
	|  +--... 'nLODLevels' LOD levels
	|  |
	|  +--MORPHTARGET (*)
	|  |  |
	|  |  +--char szName
	|  |  |
	|  |  +--bool bRelative
	|  |  |
	|  |  +--PLCore::uint32 nVertexIDs
	|  |  |
	|  |  +--PLCore::uint32 nVertexBuffers
	|  |  |
	|  |  +--'nVertexIDs' vertex ID's
	|  |  |
	|  |  +--VERTEXBUFFER (*)
	|  |  |  |
	|  |  |  +--PLCore::uint32 nVertexAttributes
	|  |  |  |
	|  |  |  +--PLCore::uint32 nVertices
	|  |  |  |
	|  |  |  +--PLCore::uint32 nSize;
	|  |  |  |
	|  |  |  +--VERTEXATTRIBUTE (*)
	|  |  |  |  |
	|  |  |  |  +--int nSemantic;
	|  |  |  |  |
	|  |  |  |  +--int nChannel;
	|  |  |  |  |
	|  |  |  |  +--int nType;
	|  |  |  |
	|  |  |  +--... 'nVertexAttributes' vertex attributes
	|  |  |  |
	|  |  |  +--... 'nVertices' vertices, a vertex consists of 'nVertexAttributes' attributes
	|  |  |
	|  |  +--... 'nVertexBuffers' vertex buffers
	|  |
	|  +--... 'nMorphTargets' morph targets
	|  |
	|  +--WEIGHT (*)
	|  |  |
	|  |  +--int nJoint
	|  |  |
	|  |  +--float fBias
	|  |
	|  +--... 'nWeights' weights
	|  |
	|  +--VERTEXWEIGHTS (*)
	|  |  |
	|  |  +--PLCore::uint32 nWeights
	|  |  |
	|  |  +-- 'nWeights' weight indices for the vertex
	|  |
	|   +--... one vertex weight for each LOD level vertex
	|
	+--MORPHTARGETANIMATION (*)
	|  |
	|  +--char szName
	|  |
	|  +--PLCore::uint32 nMorphTargets
	|  |
	|  +--PLCore::uint32 nFrames
	|  |
	|  +--MORPHTARGETANIMATIONTARGET (*)
	|  |  |
	|  |  +-- char szName[64]
	|  |
	|  +--... 'nMorphTargets' animation morph targets
	|  |
	|  +--FRAMEKEYS (*)
	|  |  |
	|  |  +-- FRAMEKEY (*)
	|  |  |   |
	|  |  |   +-- float fKey
	|  |  |
	|  |  +--... for each animation morph target
	|  |
	|  +-... 'nFrames' frame keys
	|
	+--SKELETON
	|  |
	|  +--char szName
	|  |
	|  +--PLCore::uint32 nJoints
	|  |
	|  +--PLCore::uint32 nFrames
	|  |
	|  +--JOINT (*)
	|  |  |
	|  |  +-- char szName[64]
	|  |  |
	|  |  +-- int nParent
	|  |  |
	|  |  +-- PLCore::uint8 nAnimatedComponents
	|  |
	|  +--... 'nJoints' joints
	|  |
	|  +--BASEFRAME
	|  |  |
	|  |  +--JOINTSTATE (*)
	|  |  |  |
	|  |  |  +-- float fTranslation[3]
	|  |  |  |
	|  |  |  +-- float fRotation[4]
	|  |  |  |
	|  |  |  +-- float fTranslationJointSpace[3]
	|  |  |  |
	|  |  |  +-- float fRotationJointSpace[4]
	|  |  |
	|  |  +--... 'nJoints' joint states
	|  |
	|  +--FRAMEKEYS (*)
	|  |  |
	|  |  +-- FRAMEKEY (*)
	|  |  |   |
	|  |  |   +-- float fKey
	|  |  |
	|  |  +--... for each animated component
	|  |
	|  +-... 'nFrames' frame keys
	|
	+--ANCHORPOINTS
	|  |
	|  +--PLCore::uint32 nAnchorPoints
	|  |
	|  +--ANCHORPOINT (*)
	|  |  |
	|  |  +-- char szName[64]
	|  |  |
	|  |  +-- bool bType
	|  |  |
	|  |  +-- int nID
	|  |
	|  +--... 'nAnchorPoints' anchor points
	|
	+--ANIMATIONS
	|  |
	|  +--PLCore::uint32 nAnimations
	|  |
	|  +--ANIMATION (*)
	|  |  |
	|  |  +--char szName[64]
	|  |  |
	|  |  +--int nType
	|  |  |
	|  |  +--int nStart
	|  |  |
	|  |  +--int nEnd
	|  |  |
	|  |  +--float fSpeed
	|  |  |
	|  |  +--PLCore::uint32 nFlags
	|  |  |
	|  |  +--int nEvents
	|  |  |
	|  |  +--ANIMATION_FRAMES
	|  |  |  |
	|  |  |  +--ANIMATION_FRAME (*)
	|  |  |  |  |
	|  |  |  |  +--float fSpeed
	|  |  |  |
	|  |  |  +--... |nEnd-nStart| animation frames
	|  |  |
	|  |  +--ANIMATION_EVENTS
	|  |     |
	|  |     +--ANIMATION_EVENT (*)
	|  |     |  |
	|  |     |  +--int nID
	|  |     |  |
	|  |     |  +--int nFrame
	|  |     |
	|  |     +--... 'nEvents' animation events
	|  |
	|  +--MESHBOUNDINGBOX (*)
	|  |  |
	|  |  +--float fMin[3]
	|  |  |
	|  |  +--float fMax[3]
	|  |
	|  +--...
	|
   @endverbatim
*/
class MeshFile {


	//[-------------------------------------------------------]
	//[ Constants                                             ]
	//[-------------------------------------------------------]
	public:
		// Format definition
		static const PLCore::uint32 MAGIC						= 0x57754631;
		static const PLCore::uint32 VERSION						= 2;

		// Chunk types (0x0-------)
		static const PLCore::uint32 CHUNK_MESHFILE				= 0x00000001;
		static const PLCore::uint32 CHUNK_MATERIALS				= 0x00000010;
		static const PLCore::uint32 CHUNK_MESH					= 0x00000011;
		static const PLCore::uint32 CHUNK_LODLEVEL				= 0x00000012;
		static const PLCore::uint32 CHUNK_INDEXBUFFER			= 0x00000013;
		static const PLCore::uint32 CHUNK_GEOMETRY				= 0x00000014;
		static const PLCore::uint32 CHUNK_MORPHTARGET			= 0x00000015;
		static const PLCore::uint32 CHUNK_VERTEXBUFFER			= 0x00000016;
		static const PLCore::uint32 CHUNK_VERTEXATTRIBUTE		= 0x00000017;
		static const PLCore::uint32 CHUNK_WEIGHT				= 0x00000018;
		static const PLCore::uint32 CHUNK_VERTEXWEIGHTS			= 0x00000019;
		static const PLCore::uint32 CHUNK_SKELETON				= 0x00000020;
		static const PLCore::uint32 CHUNK_SKELETONANIMATION		= 0x00000021;
		static const PLCore::uint32 CHUNK_ANCHORPOINTS			= 0x00000022;
		static const PLCore::uint32 CHUNK_ANIMATIONS			= 0x00000023;
		static const PLCore::uint32 CHUNK_MORPHTARGETANIMATION	= 0x00000024;
		static const PLCore::uint32 CHUNK_MESHBOUNDINGBOX		= 0x00000025;

		// Experimental chunks (0xA-------)

		// Unimplemented chunks (0xE-------)

		// Obsolete chunks (0xF-------)


	//[-------------------------------------------------------]
	//[ File format structures                                ]
	//[-------------------------------------------------------]
	public:
		/** 
		*  @brief
		*    Chunk header
		*/
		struct Chunk {
			PLCore::uint32 nType;	/**< Chunk type (e.g. CHUNK_MESHFILE) */
			PLCore::uint32 nSize;	/**< Chunk size in bytes (including the 8 bytes for the chunk header) */
		};

		/** 
		*  @brief
		*    Main file header
		*/
		struct MeshFileHeader {
			PLCore::uint32 nMagic;		/**< MeshFile ID "" (see MAGIC above) */
			PLCore::uint32 nVersion;	/**< MeshFile Version (see VERSION above) */
		};

		/** 
		*  @brief
		*    Material list
		*/
		struct Materials {
			PLCore::uint32 nMaterials;	/**< Number of materials */
		};

		/** 
		*  @brief
		*    Mesh
		*/
		struct Mesh {
			PLCore::uint32 nLODLevels;		/**< Number of LOD levels */
			PLCore::uint32 nMorphTargets;	/**< Number of morph targets */
			PLCore::uint32 nWeights;		/**< Total number of weights */
			PLCore::uint32 nVertexWeights;	/**< Number of vertices with weights */
		};

		/** 
		*  @brief
		*    Bounding box
		*/
		struct BoundingBox {
			float fMin[3];	/**< Minimum bounding box position */
			float fMax[3];	/**< Maximum bounding box position */
		};

		/** 
		*  @brief
		*    Weight
		*/
		struct Weight {
			int	  nJoint;	/**< Joint this weight is for */
			float fBias;	/**< Bias factor */
		};

		/** 
		*  @brief
		*    Vertex weights
		*/
		struct VertexWeights {
			PLCore::uint32 nWeights;	/**< Number of weights for the vertex */
		};

		/** 
		*  @brief
		*    LOD level
		*/
		struct LODLevel {
			float		   fDistance;				/**< Distance this LOD level is used */
			PLCore::uint32 nGeometries;				/**< Number of geometries */
			PLCore::uint32 nOctreeSubdivide;		/**< Octree subdivide */
			PLCore::uint32 nOctreeMinGeometries;	/**< Minimum number of geometries per octree */
		};

		/** 
		*  @brief
		*    Index buffer
		*/
		struct IndexBuffer {
			PLCore::uint32 nElementType;	/**< Index element type (see IndexBuffer::EType) */
			PLCore::uint32 nElements;		/**< Number of indices */
			PLCore::uint32 nSize;			/**< Total index buffer size in bytes */
		};

		/** 
		*  @brief
		*    Geometry object
		*/
		struct Geometry {
			char		   szName[64];		/**< Optional geometry name */
			PLCore::uint32 nFlags;			/**< Optional geometry flags */
			bool		   bActive;			/**< Is the geometry active/inactive? */
			int			   nPrimitiveType;	/**< Geometry primitive type (see PLRenderer::Primitive) */
			int			   nMaterial;		/**< ID of the material the geometry is using */
			int			   nStartIndex;		/**< First geometry index within the index buffer */
			int			   nIndexSize;		/**< Number of indices */
		};

		/** 
		*  @brief
		*    Morph target
		*/
		struct MorphTarget {
			char		   szName[64];		/**< Name of the morph target */
			bool		   bRelative;		/**< Is this morph target relative to the basis morph target? */
			PLCore::uint32 nVertexIDs;		/**< Number of vertex ID's, if 0 ALL vertices are influenced */
			PLCore::uint32 nVertexBuffers;	/**< Number of vertex buffers (one VB per LOD level) */
		};

		/** 
		*  @brief
		*    Vertex buffer
		*/
		struct VertexBuffer {
			PLCore::uint32 nVertexAttributes;	/**< Number of vertex attributes */
			PLCore::uint32 nVertices;			/**< Number of vertices */
			PLCore::uint32 nSize;				/**< Total vertex buffer size in bytes */
		};

		/** 
		*  @brief
		*    Vertex attribute of a vertex buffer
		*/
		struct VertexAttribute {
			int nSemantic;	/**< Semantic of the vertex attribute (see VertexBuffer::ESemantic) */
			int nChannel;	/**< Vertex attribute pipeline channel (primary color, secondary color,
								 first texture unit, second texture unit etc.) */
			int nType;		/**< Vertex attribute type (see VertexBuffer::EType) */
		};

		/** 
		*  @brief
		*    Morph target animation
		*/
		struct MorphTargetAnimation {
			char		   szName[64];		/**< Name of the morph target animation */
			PLCore::uint32 nMorphTargets;	/**< Number of morph targets */
			PLCore::uint32 nFrames;			/**< Number of morph target animation frames */
		};

		/** 
		*  @brief
		*    Skeleton
		*/
		struct Skeleton {
			char		   szName[64];	/**< Name of the skeleton (animation) */
			PLCore::uint32 nJoints;		/**< Number of joints the skeleton consists of */
			PLCore::uint32 nFrames;		/**< Number of skeleton animation frames */
		};

		/** 
		*  @brief
		*    Joint
		*/
		struct Joint {
			char		  szName[64];			/**< Unique joint name */
			int			  nParent;				/**< Number of the parent joint, < 0 if no parent */
			PLCore::uint8 nAnimatedComponents;	/**< X, Y, Z, Yaw, Pitch, Roll, W */
		};

		/** 
		*  @brief
		*    Joint state
		*/
		struct JointState {
			float fTranslation[3];				/**< Joint x/y/z translation, relative if the joint has a parent */
			float fRotation[4];					/**< Joint w/x/y/z rotation quaternion, relative if the joint has a parent */
			float fTranslationJointSpace[3];	/**< Joint x/y/z translation in joint space */
			float fRotationJointSpace[4];		/**< Joint w/x/y/z rotation quaternion in joint space */
		};

		/** 
		*  @brief
		*    Anchor points list
		*/
		struct AnchorPoints {
			PLCore::uint32 nAnchorPoints;	/**< Number of anchor points */
		};

		/** 
		*  @brief
		*    Anchor point
		*/
		struct AnchorPoint {
			char szName[64];	/**< Unique anchor point name */
			bool bType;			/**< Anchor point type (0=vertex  1=joint) */
			int  nID;			/**< Vertex/bone ID the anchor is attached to */
		};

		/** 
		*  @brief
		*    Animations list
		*/
		struct Animations {
			PLCore::uint32 nAnimations;	/**< Number of animations */
		};

		/** 
		*  @brief
		*    Animation
		*/
		struct Animation {
			char			  szName[64];	/**< Unique animation name */
			int				  nType;		/**< Animation type (0=vertex  1=skeleton) */
			int				  nStart;		/**< Start frame of the animation (inclusive) */
			int				  nEnd;			/**< End frame of the animation (inclusive) */
			float			  fSpeed;		/**< Playback speed (1.0 = normal) */
			PLCore::uint32 nFlags;		/**< Animation information flags */
			int				  nEvents;		/**< Number of animation events */
		};

		/** 
		*  @brief
		*    Animation frame
		*/
		struct AnimationFrame {
			float m_fSpeed;	/**< Frame speed (1.0 = normal) */
		};

		/** 
		*  @brief
		*    Animation event
		*/
		struct AnimationEvent {
			int m_nID;		/**< Event ID */
			int m_nFrame;	/**< Frame causing this event */
		};


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHFILE_H__
