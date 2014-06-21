/*********************************************************\
 *  File: MeshLoaderPL.h                                 *
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


#ifndef __PLMESH_MESHLOADER_PL_H__
#define __PLMESH_MESHLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Loader/MeshFile.h"
#include "PLMesh/Loader/MeshLoader.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	template <class AType> class Stack;
}
namespace PLRenderer {
	class IndexBuffer;
}
namespace PLMesh {
	class Mesh;
	class Weight;
	class Skeleton;
	class Geometry;
	class MeshLODLevel;
	class VertexWeights;
	class MeshMorphTarget;
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
*    Mesh loader implementation for the PixelLight mesh binary (Little-Endian) file format
*/
class MeshLoaderPL : public MeshLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshLoaderPL, "PLMesh", PLMesh::MeshLoader, "Mesh loader implementation for the PixelLight mesh binary (Little-Endian) file format")
		// Properties
		pl_properties
			pl_property("Formats",	"mesh,MESH")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	Mesh&,	PLCore::File&,			"Load method. The loaded mesh is static.",															"")
		pl_method_3(LoadParams,	pl_ret_type(bool),	Mesh&,	PLCore::File&,	bool,	"Load method. Parameters: First 'bool' parameter determines whether or not the mesh is static.",	"")
		pl_method_2(Save,		pl_ret_type(bool),	Mesh&,	PLCore::File&,			"Save method",																						"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API bool Load(Mesh &cMesh, PLCore::File &cFile);
		PLMESH_API bool LoadParams(Mesh &cMesh, PLCore::File &cFile, bool bStatic);
		PLMESH_API bool Save(Mesh &cMesh, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLMESH_API MeshLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshLoaderPL();


	//[-------------------------------------------------------]
	//[ File loading                                          ]
	//[-------------------------------------------------------]
	private:
		// File loading
		bool ReadMeshFile(Mesh &cMesh, PLCore::File &cFile, MeshFile::Chunk &cChunk, bool bStatic) const;
		bool ReadMaterials(Mesh &cMesh, PLCore::File &cFile) const;
		bool ReadMesh(Mesh &cMesh, PLCore::File &cFile, bool bStatic) const;
		bool ReadLODLevel(PLCore::File &cFile, MeshLODLevel &cLODLevel, bool bStatic) const;
		bool ReadIndexBuffer(PLCore::File &cFile, PLRenderer::IndexBuffer &cIndexBuffer, bool bStatic) const;
		bool ReadGeometry(PLCore::File &cFile, Geometry &cGeometry) const;
		bool ReadWeight(PLCore::File &cFile, Weight &cWeight) const;
		bool ReadVertexWeights(PLCore::File &cFile, VertexWeights &cVertexWeights) const;
		bool ReadMorphTarget(PLCore::File &cFile, MeshMorphTarget &cMorphTarget, bool bStatic) const;
		bool ReadVertexBuffer(PLCore::File &cFile, PLRenderer::VertexBuffer &cVertexBuffer, PLCore::uint32 nIndex, bool bStatic) const;
		bool ReadVertexAttribute(PLCore::File &cFile, PLRenderer::VertexBuffer &cVertexBuffer) const;
		bool ReadSkeleton(Mesh &cMesh, PLCore::File &cFile) const;
		bool ReadAnchorPoints(Mesh &cMesh, PLCore::File &cFile) const;
		bool ReadAnimations(PLCore::File &cFile) const;
		bool ReadMorphTargetAnimation(Mesh &cMesh, PLCore::File &cFile) const;
		bool ReadMeshBoundingBox(Mesh &cMesh, PLCore::File &cFile) const;

		// Tools
		MeshFile::Chunk ReadChunk(PLCore::File &cFile) const;


	//[-------------------------------------------------------]
	//[ File saving                                           ]
	//[-------------------------------------------------------]
	private:
		// File saving
		bool WriteMeshFile(Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;
		bool WriteMaterials(const Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;
		bool WriteMesh(Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;
		bool WriteLODLevel(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, MeshLODLevel &cLODLevel) const;
		bool WriteIndexBuffer(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, PLRenderer::IndexBuffer &cIndexBuffer) const;
		bool WriteGeometry(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, const Geometry &cGeometry) const;
		bool WriteWeight(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, const Weight &cWeight) const;
		bool WriteVertexWeights(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, VertexWeights &cVertexWeights) const;
		bool WriteMorphTarget(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, MeshMorphTarget &cMorphTarget) const;
		bool WriteVertexBuffer(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, PLRenderer::VertexBuffer &cVertexBuffer) const;
		bool WriteVertexAttribute(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, const PLRenderer::VertexBuffer::Attribute &cVertexAttribute) const;
		bool WriteSkeleton(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, Skeleton &cSkeleton) const;
		bool WriteAnchorPoints(Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;
// [TODO] Update this
//		bool WriteAnimations(const Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;
		bool WriteMorphTargetAnimation(Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, PLCore::uint32 nAnimation) const;
		bool WriteMeshBoundingBox(const Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;

		// Tools
		bool BeginChunk(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack, PLCore::uint32 nType) const;
		bool EndChunk(PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHLOADER_PL_H__
