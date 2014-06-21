/*********************************************************\
 *  File: MeshLoaderPLXml.h                              *
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


#ifndef __PLMESH_MESHLOADER_PLXML_H__
#define __PLMESH_MESHLOADER_PLXML_H__
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
*    Mesh loader implementation for the PixelLight mesh XML file format
*/
class MeshLoaderPLXml : public MeshLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshLoaderPLXml, "PLMesh", PLMesh::MeshLoader, "Mesh loader implementation for the PixelLight mesh XML file format")
		// Properties
		pl_properties
			pl_property("Formats",	"xmesh,XMESH")
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
		PLMESH_API MeshLoaderPLXml();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshLoaderPLXml();


	//[-------------------------------------------------------]
	//[ File loading                                          ]
	//[-------------------------------------------------------]
	private:
		// File loading
		bool LoadV1(Mesh &cMesh, const PLCore::XmlElement &cMeshElement, bool bStatic) const;
		bool ReadMaterials(Mesh &cMesh, const PLCore::XmlElement &cMaterialsElement) const;
		bool ReadLODLevels(Mesh &cMesh, const PLCore::XmlElement &cLODLevelsElement, bool bStatic) const;
		bool ReadLODLevel(MeshLODLevel &cLODLevel, const PLCore::XmlElement &cLODLevelElement, bool bStatic) const;
		bool ReadIndexBuffer(PLRenderer::IndexBuffer &cIndexBuffer, const PLCore::XmlElement &cIndexBufferElement, bool bStatic) const;
		bool ReadGeometry(Geometry &cGeometry, const PLCore::XmlElement &cGeometryElement) const;
		bool ReadMorphTargets(Mesh &cMesh, const PLCore::XmlElement &cMorphTargetsElement, bool bStatic) const;
		bool ReadMorphTarget(MeshMorphTarget &cMorphTarget, const PLCore::XmlElement &cMorphTargetElement, bool bStatic) const;
		bool ReadVertexBuffer(PLRenderer::VertexBuffer &cVertexBuffer, PLCore::uint32 nIndex, const PLCore::XmlElement &cVertexBufferElement, bool bStatic) const;
		bool ReadVertexAttribute(PLRenderer::VertexBuffer &cVertexBuffer, const PLCore::XmlElement &cVertexAttributeElement) const;
		bool ReadWeights(Mesh &cMesh, const PLCore::XmlElement &cWeightsElement) const;
		bool ReadVertexWeightsContainer(Mesh &cMesh, const PLCore::XmlElement &cVertexWeightsContainerElement) const;
		bool ReadSkeletons(Mesh &cMesh, const PLCore::XmlElement &cSkeletonsElement) const;
		bool ReadSkeleton(Mesh &cMesh, const PLCore::XmlElement &cSkeletonElement) const;
		bool ReadAnchorPoints(Mesh &cMesh, const PLCore::XmlElement &cAnchorPointsElement) const;
		bool ReadAnimations(PLCore::File &cFile) const;
		bool ReadMorphTargetAnimations(Mesh &cMesh, const PLCore::XmlElement &cMorphTargetAnimationsElement) const;
		bool ReadMorphTargetAnimation(Mesh &cMesh, const PLCore::XmlElement &cMorphTargetElement) const;
		bool ReadMeshBoundingBox(Mesh &cMesh, const PLCore::XmlElement &cBoundingBoxElement) const;


	//[-------------------------------------------------------]
	//[ File saving                                           ]
	//[-------------------------------------------------------]
	private:
		// File saving
		bool WriteMeshFile(Mesh &cMesh, PLCore::XmlElement &cMeshElement) const;
		bool WriteMaterials(const Mesh &cMesh, PLCore::XmlElement &cMeshElement) const;
		bool WriteMesh(Mesh &cMesh, PLCore::XmlElement &cMeshElement) const;
		bool WriteLODLevel(MeshLODLevel &cLODLevel, PLCore::uint32 nID, PLCore::XmlElement &cLODLevelsElement) const;
		bool WriteIndexBuffer(PLRenderer::IndexBuffer &cIndexBuffer, PLCore::XmlElement &cLODLevelElement) const;
		bool WriteGeometry(const Geometry &cGeometry, PLCore::uint32 nID, PLCore::XmlElement &cGeometriesElement) const;
		bool WriteWeight(const Weight &cWeight, PLCore::uint32 nID, PLCore::XmlElement &cWeightsElement) const;
		bool WriteVertexWeights(VertexWeights &cVertexWeights, PLCore::uint32 nID, PLCore::XmlElement &cVertexWeightsElement) const;
		bool WriteMorphTarget(MeshMorphTarget &cMorphTarget, PLCore::uint32 nID, PLCore::XmlElement &cMorphTargetsElement) const;
		bool WriteVertexBuffer(PLRenderer::VertexBuffer &cVertexBuffer, PLCore::uint32 nID, PLCore::XmlElement &pVertexBuffersElement) const;
		bool WriteVertexAttribute(const PLRenderer::VertexBuffer::Attribute &cVertexAttribute, PLCore::uint32 nID, PLCore::XmlElement &cVertexAttributesElement) const;
		bool WriteSkeleton(Skeleton &cSkeleton, PLCore::uint32 nID, PLCore::XmlElement &cSkeletonsElement) const;
		bool WriteAnchorPoints(Mesh &cMesh, PLCore::XmlElement &cMeshElement) const;
// [TODO] Update this
//		bool WriteAnimations(const Mesh &cMesh, PLCore::File &cFile, PLCore::Stack<MeshFile::Chunk> &cChunkStack) const;
		bool WriteMorphTargetAnimation(Mesh &cMesh, PLCore::uint32 nAnimation, PLCore::XmlElement &cMorphTargetAnimationsElement) const;
		bool WriteMeshBoundingBox(const Mesh &cMesh, PLCore::XmlElement &cMeshElement) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHLOADER_PLXML_H__
