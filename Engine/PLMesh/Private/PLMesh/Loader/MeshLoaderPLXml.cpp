/*********************************************************\
 *  File: MeshLoaderPLXml.cpp                            *
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
#include <PLCore/Xml/Xml.h>
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>
#include <PLCore/Container/Stack.h>
#include <PLCore/String/Tokenizer.h>
#include <PLCore/String/ParseTools.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Animation/AnimationEvent.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLMesh/Mesh.h"
#include "PLMesh/MeshOctree.h"
#include "PLMesh/AnchorPoint.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MorphTargetAni.h"
#include "PLMesh/SkeletonManager.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Loader/MeshLoaderPLXml.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(MeshLoaderPLXml)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool MeshLoaderPLXml::Load(Mesh &cMesh, File &cFile)
{
	return LoadParams(cMesh, cFile, true);
}

bool MeshLoaderPLXml::LoadParams(Mesh &cMesh, File &cFile, bool bStatic)
{
	bool bResult = false; // Error by default

	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get mesh element
		const XmlElement *pMeshElement = cDocument.GetFirstChildElement("Mesh");
		if (pMeshElement) {
			// Get the format version
			int nVersion = pMeshElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				bResult = LoadV1(cMesh, *pMeshElement, bStatic);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else PL_LOG(Error, "Can't find 'Mesh' element")
	} else PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())

	// If all went fine, create final octree
	if (bResult) {
		for (uint32 i=0; i<cMesh.GetNumOfLODLevels(); i++) {
			MeshLODLevel &cLODLevel = *cMesh.GetLODLevel(i);
			if (cLODLevel.GetOctree()) {
				cLODLevel.CreateOctree(cLODLevel.GetOctree()->GetSubdivide(),
									   cLODLevel.GetOctree()->GetMinGeometries());
			}
		}
	}

	// Done
	return bResult;
}

bool MeshLoaderPLXml::Save(Mesh &cMesh, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add mesh
	XmlElement *pMeshElement = new XmlElement("Mesh");
	pMeshElement->SetAttribute("Version", "1");

	// Write file
	WriteMeshFile(cMesh, *pMeshElement);

	// Link mesh element to parent
	cDocument.LinkEndChild(*pMeshElement);

	// Save config
	cDocument.Save(cFile);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshLoaderPLXml::MeshLoaderPLXml()
{
}

/**
*  @brief
*    Destructor
*/
MeshLoaderPLXml::~MeshLoaderPLXml()
{
}


//[-------------------------------------------------------]
//[ File loading                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool MeshLoaderPLXml::LoadV1(Mesh &cMesh, const XmlElement &cChunkElement, bool bStatic) const
{
	bool bResult = true; // No error by default

	// Iterate through all elements
	const XmlElement *pElement = cChunkElement.GetFirstChildElement();
	while (pElement) {
		// Check value
		const String sValue = pElement->GetValue();
		if (sValue.GetLength()) {
			// Materials
			if (sValue == "Materials") {
				bResult = ReadMaterials(cMesh, *pElement);

			// LODLevels
			} else if (sValue == "LODLevels") {
				bResult = ReadLODLevels(cMesh, *pElement, bStatic);

			// MorphTargets
			} else if (sValue == "MorphTargets") {
				bResult = ReadMorphTargets(cMesh, *pElement, bStatic);

			// Weights
			} else if (sValue == "Weights") {
				bResult = ReadWeights(cMesh, *pElement);

			// VertexWeightsContainer
			} else if (sValue == "VertexWeightsContainer") {
				bResult = ReadVertexWeightsContainer(cMesh, *pElement);

			// Skeletons
			} else if (sValue == "Skeletons") {
				bResult = ReadSkeletons(cMesh, *pElement);

			// AnchorPoints
			} else if (sValue == "AnchorPoints") {
				bResult = ReadAnchorPoints(cMesh, *pElement);

			// [TODO] Animations
//			} else if (sValue == "Animations") {
//				bResult = ReadAnimations(cFile);

			// MorphTargetAnimations
			} else if (sValue == "MorphTargetAnimations") {
				bResult = ReadMorphTargetAnimations(cMesh, *pElement);

			// BoundingBox
			} else if (sValue == "BoundingBox") {
				bResult = ReadMeshBoundingBox(cMesh, *pElement);
			}
		}

		// Next element, please
		pElement = pElement->GetNextSiblingElement();
	}

	// Done
	return bResult;
}

bool MeshLoaderPLXml::ReadMaterials(Mesh &cMesh, const XmlElement &cMaterialsElement) const
{
	// Get the material manager
	MaterialManager &cMaterialManager = cMesh.GetRenderer()->GetRendererContext().GetMaterialManager();

	// Read materials
	const XmlElement *pMaterialElement = cMaterialsElement.GetFirstChildElement("Material");
	while (pMaterialElement) {
		// Get value
		const XmlNode *pNode = pMaterialElement->GetFirstChild();
		if (pNode && pNode->GetType() == XmlNode::Text) {
			const String sValue = pNode->GetValue();
			if (sValue.GetLength()) {
				Material *pMaterial = cMaterialManager.LoadResource(sValue);
				cMesh.AddMaterial(pMaterial ? pMaterial : cMaterialManager.Create(sValue));
			}
		}

		// Next element, please
		pMaterialElement = pMaterialElement->GetNextSiblingElement("Material");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadLODLevels(Mesh &cMesh, const XmlElement &cLODLevelsElement, bool bStatic) const
{
	// Read LOD levels
	const XmlElement *pLODLevelElement = cLODLevelsElement.GetFirstChildElement("LODLevel");
	while (pLODLevelElement) {
		// Add new LOD level and read it in
		MeshLODLevel *pLODLevel = cMesh.AddLODLevel();
		if (!pLODLevel || !ReadLODLevel(*pLODLevel, *pLODLevelElement, bStatic))
			return false; // Error!

		// Next element, please
		pLODLevelElement = pLODLevelElement->GetNextSiblingElement("LODLevel");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadLODLevel(MeshLODLevel &cLODLevel, const XmlElement &cLODLevelElement, bool bStatic) const
{
	// Read attributes
	const String sDistance = cLODLevelElement.GetAttribute("Distance");
	if (sDistance.GetLength())
		cLODLevel.SetLODDistance(sDistance.GetFloat());
	// [TODO] Remove octree from mesh class :)
	// Create temp octree for storing the two settings
//	if (sLODLevel.nOctreeSubdivide != 0 && sLODLevel.nOctreeMinGeometries != 0)
//		cLODLevel.CreateOctree(sLODLevel.nOctreeSubdivide, sLODLevel.nOctreeMinGeometries);

	// Get the index buffer element
	const XmlElement *pIndexBufferElement = cLODLevelElement.GetFirstChildElement("IndexBuffer");
	if (pIndexBufferElement) {
		// Read in the index buffer
		cLODLevel.CreateIndexBuffer();
		IndexBuffer *pIndexBuffer = cLODLevel.GetIndexBuffer();
		if (!pIndexBuffer || !ReadIndexBuffer(*pIndexBuffer, *pIndexBufferElement, bStatic))
			return false; // Error!

		// Get the geometries element
		const XmlElement *pGeometriesElement = cLODLevelElement.GetFirstChildElement("Geometries");
		if (pGeometriesElement) {
			cLODLevel.CreateGeometries();
			Array<Geometry> *plstGeometries = cLODLevel.GetGeometries();

			// Read geometries
			const XmlElement *pGeometryElement = pGeometriesElement->GetFirstChildElement("Geometry");
			while (pGeometryElement) {
				// Add new geometry and load in in
				Geometry &cGeometry = plstGeometries->Add();
				if ((&cGeometry == &Array<Geometry>::Null) || !ReadGeometry(cGeometry, *pGeometryElement))
					return false; // Error!

				// Next element, please
				pGeometryElement = pGeometryElement->GetNextSiblingElement("Geometry");
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool MeshLoaderPLXml::ReadIndexBuffer(IndexBuffer &cIndexBuffer, const XmlElement &cIndexBufferElement, bool bStatic) const
{
	// Read attributes
	const String sElementType = cIndexBufferElement.GetAttribute("ElementType");
	IndexBuffer::EType nElementType = IndexBuffer::UInt;
	if (sElementType == "UInt")
		nElementType = IndexBuffer::UInt;
	else if (sElementType == "UShort")
		nElementType = IndexBuffer::UShort;
	else if (sElementType == "UByte")
		nElementType = IndexBuffer::UByte;
	const uint32 nElements = cIndexBufferElement.GetAttribute("Elements").GetUInt32();

	// Allocate the index buffer
	cIndexBuffer.SetElementType(nElementType);
	if (cIndexBuffer.Allocate(nElements, bStatic ? Usage::Static : Usage::Dynamic)) {
		// Get value
		const XmlNode *pNode = cIndexBufferElement.GetFirstChild();
		if (pNode && pNode->GetType() == XmlNode::Text) {
			const String sValue = pNode->GetValue();
			if (sValue.GetLength()) {
				void *pData = cIndexBuffer.Lock(Lock::WriteOnly);
				if (pData) {
					Tokenizer cTokenizer;
					cTokenizer.Start(sValue);
					for (uint32 i=0; i<nElements; i++) {
						// Get the next token
						const String sToken = cTokenizer.GetNextToken();
						if (sToken.GetLength()) {
							// Parse the token and set the index
							cIndexBuffer.SetData(i, sToken.GetUInt32());
						} else {
							// Nothing left to read in, get us out of the loop (shouldn't happen on totally valid input files...)
							i = nElements;
						}
					}

					// Unlock the index buffer
					cIndexBuffer.Unlock();
				}
			}
		}

		// Done
		return true;
	}

	// Error!
	return false;
}

bool MeshLoaderPLXml::ReadGeometry(Geometry &cGeometry, const XmlElement &cGeometryElement) const
{
	// Read attributes
	cGeometry.SetName(cGeometryElement.GetAttribute("Name"));
	cGeometry.SetFlags(cGeometryElement.GetAttribute("Flags").GetUInt32());
	cGeometry.SetActive(cGeometryElement.GetAttribute("Active").GetBool());
	const String sPrimitiveType = cGeometryElement.GetAttribute("PrimitiveType");
	if (sPrimitiveType == "PointList")
		cGeometry.SetPrimitiveType(Primitive::PointList);
	else if (sPrimitiveType == "LineList")
		cGeometry.SetPrimitiveType(Primitive::LineList);
	else if (sPrimitiveType == "LineStrip")
		cGeometry.SetPrimitiveType(Primitive::LineStrip);
	else if (sPrimitiveType == "TriangleList")
		cGeometry.SetPrimitiveType(Primitive::TriangleList);
	else if (sPrimitiveType == "TriangleStrip")
		cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
	else if (sPrimitiveType == "TriangleFan")
		cGeometry.SetPrimitiveType(Primitive::TriangleFan);
	cGeometry.SetMaterial(cGeometryElement.GetAttribute("Material").GetInt());
	cGeometry.SetStartIndex(cGeometryElement.GetAttribute("StartIndex").GetInt());
	cGeometry.SetIndexSize(cGeometryElement.GetAttribute("IndexSize").GetInt());

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadMorphTargets(Mesh &cMesh, const XmlElement &cMorphTargetsElement, bool bStatic) const
{
	// Read morph targets
	const XmlElement *pMorphTargetElement = cMorphTargetsElement.GetFirstChildElement("MorphTarget");
	while (pMorphTargetElement) {
		// Add new morph target and read it in
		MeshMorphTarget *pMeshMorphTarget = cMesh.AddMorphTarget();
		if (!pMeshMorphTarget || !ReadMorphTarget(*pMeshMorphTarget, *pMorphTargetElement, bStatic))
			return false; // Error!

		// Next element, please
		pMorphTargetElement = pMorphTargetElement->GetNextSiblingElement("MorphTarget");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadMorphTarget(MeshMorphTarget &cMorphTarget, const XmlElement &cMorphTargesElement, bool bStatic) const
{
	// Read attributes
	cMorphTarget.SetName(cMorphTargesElement.GetAttribute("Name"));
	cMorphTarget.SetRelative(cMorphTargesElement.GetAttribute("Relative").GetBool());

	// Read vertex ID's
	Array<uint32> &lstVertexIDs = cMorphTarget.GetVertexIDs();
	lstVertexIDs.Resize(cMorphTargesElement.GetAttribute("VertexIDs").GetUInt32());
	if (lstVertexIDs.GetNumOfElements()) {
		// Get the VertexIDs element
		const XmlElement *pVertexIDsElement = cMorphTargesElement.GetFirstChildElement("VertexIDs");
		if (pVertexIDsElement) {
			// Get value
			const XmlNode *pNode = pVertexIDsElement->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				const String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					Tokenizer cTokenizer;
					cTokenizer.Start(sValue);
					for (uint32 i=0; i<lstVertexIDs.GetNumOfElements(); i++) {
						// Get the next token
						const String sToken = cTokenizer.GetNextToken();
						if (sToken.GetLength()) {
							// Parse the token and set the index
							lstVertexIDs[i] = sToken.GetUInt32();
						} else {
							// Nothing left to read in, get us out of the loop (shouldn't happen on totally valid input files...)
							i = lstVertexIDs.GetNumOfElements();
						}
					}
				}
			}
		}
	}

	// Read vertex buffers
	const uint32 nVertexBuffers = cMorphTargesElement.GetAttribute("VertexBuffers").GetUInt32();
	if (nVertexBuffers) {
		// Get the vertex buffers element
		const XmlElement *pVertexBuffersElement = cMorphTargesElement.GetFirstChildElement("VertexBuffers");
		if (pVertexBuffersElement) {
			// Loop through all vertex buffer elements
			const XmlElement *pVertexBufferElement = pVertexBuffersElement->GetFirstChildElement("VertexBuffer");
			for (uint32 i=0; pVertexBufferElement&&i<nVertexBuffers; i++) {
				// Read in the vertex buffer
				if (!ReadVertexBuffer(*cMorphTarget.GetVertexBuffer(), i, *pVertexBufferElement, bStatic))
					return false; // Error!

				// Next element, please
				pVertexBufferElement = pVertexBufferElement->GetNextSiblingElement("VertexBuffer");
			}
		}
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadVertexBuffer(VertexBuffer &cVertexBuffer, uint32 nIndex, const XmlElement &cVertexBufferElement, bool bStatic) const
{
	// Read attributes
	const uint32 nVertexAttributes = cVertexBufferElement.GetAttribute("VertexAttributes").GetUInt32();
	const uint32 nVertices		   = cVertexBufferElement.GetAttribute("Vertices").GetUInt32();

	// Get the vertex attributes element
	const XmlElement *pVertexAttributesElement = cVertexBufferElement.GetFirstChildElement("VertexAttributes");
	if (pVertexAttributesElement) {
		// Loop through all vertex attribute elements
		const XmlElement *pVertexAttributeElement = pVertexAttributesElement->GetFirstChildElement("VertexAttribute");
		for (uint32 i=0; pVertexAttributeElement&&i<nVertexAttributes; i++) {
			// Read in the vertex attribute
			if (!ReadVertexAttribute(cVertexBuffer, *pVertexAttributeElement))
				return false; // Error!

			// Next element, please
			pVertexAttributeElement = pVertexAttributeElement->GetNextSiblingElement("VertexAttribute");
		}
	}

	// Allocate the vertex buffer
	if (nIndex)
		cVertexBuffer.Allocate(nVertices, Usage::Software);
	else
		cVertexBuffer.Allocate(nVertices, bStatic ? Usage::Static : Usage::Dynamic);

	// Read data
	bool bResult = true; // No error by default
	if (cVertexBuffer.Lock(Lock::WriteOnly)) {
		// Get the vertices element
		const XmlElement *pVerticesElement = cVertexBufferElement.GetFirstChildElement("Vertices");
		if (pVerticesElement) {
			// Loop through all vertex elements
			const XmlElement *pVertexElement = pVerticesElement->GetFirstChildElement("Vertex");
			for (uint32 nVertex=0; pVertexElement&&nVertex<nVertices; nVertex++) {
				// Loop through all vertex attributes
				const XmlElement *pVertexAttributeElement = pVertexElement->GetFirstChildElement();
				for (uint32 nAttribute=0; pVertexAttributeElement&&nAttribute<cVertexBuffer.GetNumOfVertexAttributes(); nAttribute++) {
					// Get the vertex attribute
					const VertexBuffer::Attribute *pVertexAttribute = cVertexBuffer.GetVertexAttribute(nAttribute);

					// Get the vertex data
					void *pData = cVertexBuffer.GetData(nVertex, pVertexAttribute->nSemantic, pVertexAttribute->nChannel);

					// Get the vertex attribute type
					VertexBuffer::EType nType = VertexBuffer::RGBA;
					const String sType = pVertexAttributeElement->GetValue();
					if (sType == "RGBA")
						nType = VertexBuffer::RGBA;
					else if (sType == "Float1")
						nType = VertexBuffer::Float1;
					else if (sType == "Float2")
						nType = VertexBuffer::Float2;
					else if (sType == "Float3")
						nType = VertexBuffer::Float3;
					else if (sType == "Float4")
						nType = VertexBuffer::Float4;
					else if (sType == "Short2")
						nType = VertexBuffer::Short2;
					else if (sType == "Short4")
						nType = VertexBuffer::Short4;

					// The type must match!
					if (pVertexAttribute->nType == nType) {
						// Get value
						const XmlNode *pNode = pVertexAttributeElement->GetFirstChild();
						if (pNode && pNode->GetType() == XmlNode::Text) {
							const String sValue = pNode->GetValue();
							if (sValue.GetLength()) {
								switch (nType) {
									case VertexBuffer::RGBA:
										ParseTools::ParseFloatArray(sValue, static_cast<float*>(pData), 4);
										break;

									case VertexBuffer::Float1:
										ParseTools::ParseFloatArray(sValue, static_cast<float*>(pData), 1);
										break;

									case VertexBuffer::Float2:
										ParseTools::ParseFloatArray(sValue, static_cast<float*>(pData), 2);
										break;

									case VertexBuffer::Float3:
										ParseTools::ParseFloatArray(sValue, static_cast<float*>(pData), 3);
										break;

									case VertexBuffer::Float4:
										ParseTools::ParseFloatArray(sValue, static_cast<float*>(pData), 4);
										break;

									case VertexBuffer::Short2:
									{
										int nData[2];
										ParseTools::ParseIntegerArray(sValue, static_cast<int*>(nData), 2);
										static_cast<short*>(pData)[0] = static_cast<short>(nData[0]);
										static_cast<short*>(pData)[1] = static_cast<short>(nData[1]);
										break;
									}

									case VertexBuffer::Short4:
									{
										int nData[4];
										ParseTools::ParseIntegerArray(sValue, static_cast<int*>(nData), 4);
										static_cast<short*>(pData)[0] = static_cast<short>(nData[0]);
										static_cast<short*>(pData)[1] = static_cast<short>(nData[1]);
										static_cast<short*>(pData)[2] = static_cast<short>(nData[2]);
										static_cast<short*>(pData)[3] = static_cast<short>(nData[3]);
										break;
									}

									case VertexBuffer::Half1:
									case VertexBuffer::Half2:
									case VertexBuffer::Half3:
									case VertexBuffer::Half4:
										// [TODO] Implement me
										break;
								}
							}
						}
					}

					// Next element, please
					pVertexAttributeElement = pVertexAttributeElement->GetNextSiblingElement();
				}

				// Next element, please
				pVertexElement = pVertexElement->GetNextSiblingElement("Vertex");
			}
		}

		// Unlock the vertex buffer
		cVertexBuffer.Unlock();
	} else {
		// Error!
		bResult = false;
	}

	// Done
	return bResult;
}

bool MeshLoaderPLXml::ReadVertexAttribute(VertexBuffer &cVertexBuffer, const XmlElement &cVertexAttributeElement) const
{
	// Read semantic attribute
	VertexBuffer::ESemantic nSemantic = VertexBuffer::Position;
	const String sSemantic = cVertexAttributeElement.GetAttribute("Semantic");
	if (sSemantic == "Position")
		nSemantic = VertexBuffer::Position;
	else if (sSemantic == "BlendWeight")
		nSemantic = VertexBuffer::BlendWeight;
	else if (sSemantic == "Normal")
		nSemantic = VertexBuffer::Normal;
	else if (sSemantic == "Color")
		nSemantic = VertexBuffer::Color;
	else if (sSemantic == "FogCoord")
		nSemantic = VertexBuffer::FogCoord;
	else if (sSemantic == "PointSize")
		nSemantic = VertexBuffer::PointSize;
	else if (sSemantic == "BlendIndices")
		nSemantic = VertexBuffer::BlendIndices;
	else if (sSemantic == "TexCoord")
		nSemantic = VertexBuffer::TexCoord;
	else if (sSemantic == "Tangent")
		nSemantic = VertexBuffer::Tangent;
	else if (sSemantic == "Binormal")
		nSemantic = VertexBuffer::Binormal;

	// Read channel attribute
	const uint32 nChannel = cVertexAttributeElement.GetAttribute("Channel").GetUInt32();

	// Read type attribute
	VertexBuffer::EType nType = VertexBuffer::RGBA;
	const String sType = cVertexAttributeElement.GetAttribute("Type");
	if (sType == "RGBA")
		nType = VertexBuffer::RGBA;
	else if (sType == "Float1")
		nType = VertexBuffer::Float1;
	else if (sType == "Float2")
		nType = VertexBuffer::Float2;
	else if (sType == "Float3")
		nType = VertexBuffer::Float3;
	else if (sType == "Float4")
		nType = VertexBuffer::Float4;
	else if (sType == "Short2")
		nType = VertexBuffer::Short2;
	else if (sType == "Short4")
		nType = VertexBuffer::Short4;

	// Finally, add the read in vertex attribute!
	cVertexBuffer.AddVertexAttribute(nSemantic, nChannel, nType);

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadWeights(Mesh &cMesh, const XmlElement &cWeightsElement) const
{
	// Get the number of weights
	const uint32 nNumOfWeights = cWeightsElement.GetAttribute("NumOfWeights").GetUInt32();
	cMesh.GetWeights().Resize(nNumOfWeights);

	// Read weights
	const XmlElement *pWeightElement = cWeightsElement.GetFirstChildElement("Weight");
	for (uint32 i=0; pWeightElement&&i<nNumOfWeights; i++) {
		// Get the weight
		Weight &cWeight = cMesh.GetWeights()[i];

		// Read joint attribute
		cWeight.SetJoint(pWeightElement->GetAttribute("Joint").GetInt());

		// Read bias attribute
		cWeight.SetBias(pWeightElement->GetAttribute("Bias").GetFloat());

		// Next element, please
		pWeightElement = pWeightElement->GetNextSiblingElement("Weight");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadVertexWeightsContainer(Mesh &cMesh, const XmlElement &cVertexWeightsContainerElement) const
{
	// Get the number of vertex weights
	const uint32 nNumOfVertexWeights = cVertexWeightsContainerElement.GetAttribute("NumOfVertexWeights").GetUInt32();
	Array<VertexWeights> &lstVW = cMesh.GetVertexWeights();
	lstVW.Resize(nNumOfVertexWeights);

	// Read vertex weights
	const XmlElement *pVertexWeightsElement = cVertexWeightsContainerElement.GetFirstChildElement("VertexWeights");
	for (uint32 i=0; pVertexWeightsElement&&i<nNumOfVertexWeights; i++) {
		// Get the vertex weights
		VertexWeights &cVertexWeights = lstVW[i];

		// Get the number of weights for the vertex
		Array<uint32> &cWeights = cVertexWeights.GetWeights();
		cWeights.Resize(pVertexWeightsElement->GetAttribute("NumOfWeights").GetUInt32());

		// Get value
		const XmlNode *pNode = pVertexWeightsElement->GetFirstChild();
		if (pNode && pNode->GetType() == XmlNode::Text) {
			const String sValue = pNode->GetValue();
			if (sValue.GetLength()) {
				Tokenizer cTokenizer;
				cTokenizer.Start(sValue);
				for (uint32 nWeight=0; nWeight<cWeights.GetNumOfElements(); nWeight++) {
					// Get the next token
					const String sToken = cTokenizer.GetNextToken();
					if (sToken.GetLength()) {
						// Parse the token and set the index
						cWeights[nWeight] = sToken.GetUInt32();
					} else {
						// Nothing left to read in, get us out of the loop (shouldn't happen on totally valid input files...)
						nWeight = cWeights.GetNumOfElements();
					}
				}
			}
		}

		// Next element, please
		pVertexWeightsElement = pVertexWeightsElement->GetNextSiblingElement("VertexWeights");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadSkeletons(Mesh &cMesh, const XmlElement &cSkeletonsElement) const
{
	// Read skeletons
	const XmlElement *pSkeletonElement = cSkeletonsElement.GetFirstChildElement("Skeleton");
	while (pSkeletonElement) {
		// Read the skeleton
		if (!ReadSkeleton(cMesh, *pSkeletonElement))
			return false; // Error!

		// Next element, please
		pSkeletonElement = pSkeletonElement->GetNextSiblingElement("Skeleton");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadSkeleton(Mesh &cMesh, const XmlElement &cSkeletonElement) const
{
	// Read attributes
	const String sName   = cSkeletonElement.GetAttribute("Name");
	const uint32 nJoints = cSkeletonElement.GetAttribute("Joints").GetUInt32();
	const uint32 nFrames = cSkeletonElement.GetAttribute("Frames").GetUInt32();

	// Get the skeleton manager
	SkeletonManager &cSkeletonManager = cMesh.GetSkeletonManager();

	// Create skeleton
	Skeleton *pSkeleton = cSkeletonManager.Create(sName);
	if (pSkeleton) {
		// Add this skeleton to the skeleton list of the mesh
		SkeletonHandler *pSkeletonHandler = new SkeletonHandler();
		pSkeletonHandler->SetResource(pSkeleton);
		cMesh.GetSkeletonHandlers().Add(pSkeletonHandler);

		// Animated joint states
		uint32 nAnimatedComponents = 0; // Total number of animated components per frame
		Array<Skeleton::AniJoint> &lstJoints = pSkeleton->GetJoints();
		lstJoints.Resize(nJoints);

		// Get the joints element
		const XmlElement *pJointsElement = cSkeletonElement.GetFirstChildElement("Joints");
		if (pJointsElement) {
			// Loop through all joint elements
			const XmlElement *pJointElement = pJointsElement->GetFirstChildElement("Joint");
			for (uint32 nJoint=0; pJointElement&&nJoint<nJoints; nJoint++) {
				Joint *pJoint = pSkeleton->Create(pJointElement->GetAttribute("Name"));
				pJoint->SetID(nJoint);
				pJoint->SetParent(pJointElement->GetAttribute("Parent").GetInt());
				Skeleton::AniJoint &cAniJoint = lstJoints[nJoint];
				cAniJoint.nAnimatedComponents = pJointElement->GetAttribute("AnimatedComponents").GetChar();
				if (cAniJoint.nAnimatedComponents & Skeleton::AX)
					nAnimatedComponents++;
				if (cAniJoint.nAnimatedComponents & Skeleton::AY)
					nAnimatedComponents++;
				if (cAniJoint.nAnimatedComponents & Skeleton::AZ)
					nAnimatedComponents++;
				if (cAniJoint.nAnimatedComponents & Skeleton::AYaw)
					nAnimatedComponents++;
				if (cAniJoint.nAnimatedComponents & Skeleton::APitch)
					nAnimatedComponents++;
				if (cAniJoint.nAnimatedComponents & Skeleton::ARoll)
					nAnimatedComponents++;
				if (cAniJoint.nAnimatedComponents & Skeleton::AW)
					nAnimatedComponents++;

				// Next element, please
				pJointElement = pJointElement->GetNextSiblingElement("Joint");
			}
		}

		// Get the base frame element
		const XmlElement *pBaseFrameElement = cSkeletonElement.GetFirstChildElement("BaseFrame");
		if (pBaseFrameElement) {
			// Loop through all joint state elements
			const XmlElement *pJointStateElement = pBaseFrameElement->GetFirstChildElement("JointState");
			for (uint32 nJoint=0; pJointStateElement&&nJoint<nJoints; nJoint++) {
				// Get the joint
				Joint *pJoint = pSkeleton->GetByIndex(nJoint);

				{ // Read translation attribute
					Vector3 vTranslation;
					vTranslation.FromString(pJointStateElement->GetAttribute("Translation"));
					pJoint->SetTranslation(vTranslation);
				}
				{ // Read rotation attribute
					Quaternion qRotation;
					qRotation.FromString(pJointStateElement->GetAttribute("Rotation"));
					pJoint->SetRotation(qRotation);
				}
				{ // Read translation joint space attribute
					Vector3 vTranslation;
					vTranslation.FromString(pJointStateElement->GetAttribute("TranslationJointSpace"));
					pJoint->SetTranslationJointSpace(vTranslation);
				}
				{ // Read rotation joint space attribute
					Quaternion qRotation;
					qRotation.FromString(pJointStateElement->GetAttribute("RotationJointSpace"));
					pJoint->SetRotationJointSpace(qRotation);
				}

				// Next element, please
				pJointStateElement = pJointStateElement->GetNextSiblingElement("JointState");
			}
		}

		// Get the frame keys element
		Array<Skeleton::FrameKeys> &lstFrameKeys = pSkeleton->GetFrameKeys();
		lstFrameKeys.Resize(nFrames);
		const XmlElement *pFrameKeysElement = cSkeletonElement.GetFirstChildElement("FrameKeys");
		if (pFrameKeysElement) {
			// Loop through all frame key elements
			const XmlElement *pFrameKeyElement = pFrameKeysElement->GetFirstChildElement("FrameKey");
			for (uint32 nFrame=0; pFrameKeyElement&&nFrame<nFrames; nFrame++) {
				// Read the frame key data
				Array<float> &lstFrameKeysT = lstFrameKeys[nFrame].lstFrameKeys;
				lstFrameKeysT.Resize(nAnimatedComponents);
				if (lstFrameKeysT.GetNumOfElements()) {
					// Get value
					const XmlNode *pNode = pFrameKeyElement->GetFirstChild();
					if (pNode && pNode->GetType() == XmlNode::Text) {
						const String sValue = pNode->GetValue();
						if (sValue.GetLength()) {
							Tokenizer cTokenizer;
							cTokenizer.Start(sValue);
							for (uint32 nValue=0; nValue<lstFrameKeysT.GetNumOfElements(); nValue++) {
								// Get the next token
								const String sToken = cTokenizer.GetNextToken();
								if (sToken.GetLength()) {
									// Parse the token and set the index
									lstFrameKeysT.GetData()[nValue] = sToken.GetFloat();
								} else {
									// Nothing left to read in, get us out of the loop (shouldn't happen on totally valid input files...)
									nValue = lstFrameKeysT.GetNumOfElements();
								}
							}
						}
					}
				}

				// Next element, please
				pFrameKeyElement = pFrameKeyElement->GetNextSiblingElement("FrameKey");
			}
		}
		pSkeleton->SetEndFrame(nFrames ? nFrames-1 : 0);
		pSkeleton->SetFlags(AnimationInfo::Loop);

		// Update skeleton
		pSkeleton->UpdateJointInformation();

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

bool MeshLoaderPLXml::ReadAnchorPoints(Mesh &cMesh, const XmlElement &cAnchorPointsElement) const
{
	// Get number of anchor points attribute
	const uint32 nAnchorPoints = cAnchorPointsElement.GetAttribute("AnchorPoints").GetUInt32();

	// Loop through all anchor point elements
	const XmlElement *pAnchorPointElement = cAnchorPointsElement.GetFirstChildElement("AnchorPoint");
	for (uint32 nAnchorPoint=0; pAnchorPointElement&&nAnchorPoint<nAnchorPoints; nAnchorPoint++) {
		// Added into a manager automatically...
		new AnchorPoint(pAnchorPointElement->GetAttribute("Name"), pAnchorPointElement->GetAttribute("Type").GetBool(), pAnchorPointElement->GetAttribute("ID").GetInt(), &cMesh.GetAnchorPointManager());

		// Next element, please
		pAnchorPointElement = pAnchorPointElement->GetNextSiblingElement("AnchorPoint");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadAnimations(File &cFile) const
{
// [TODO] Update this
/*
	// Read animations header
	MeshFile::Animations sAnimations;
	if (!cFile.Read(&sAnimations, 1, sizeof(sAnimations)))
		return false; // Error!

	// Read animations
	for (uint32 i=0; i<sAnimations.nAnimations; i++) {
		// Read animation
		MeshFile::Animation sAnimation;
		if (!cFile.Read(&sAnimation, 1, sizeof(sAnimation)))
			return false; // Error!
		AnimationInfo *pAni = new AnimationInfo();
		// [TODO] Update this
//		pAni->SetName(sAnimation.szName);
		pAni->SetType(sAnimation.nType);
		pAni->SetStartFrame(sAnimation.nStart);
		pAni->SetEndFrame(sAnimation.nEnd);
		pAni->SetSpeed(sAnimation.fSpeed);
		pAni->SetFlags(sAnimation.nFlags);
		// Read animation frames
		for (uint32 j=0; j<pAni->GetNumOfFrames(); j++) {
			MeshFile::AnimationFrame sAnimationFrame;
			if (!cFile.Read(&sAnimationFrame, 1, sizeof(sAnimationFrame)))
				return false; // Error!
			pAni->GetFrameInfo(j)->SetSpeed(sAnimationFrame.m_fSpeed);
		}
		// Read animation events
		for (int j=0; j<sAnimation.nEvents; j++) {
			MeshFile::AnimationEvent sAnimationEvent;
			if (!cFile.Read(&sAnimationEvent, 1, sizeof(sAnimationEvent)))
				return false; // Error!
			// Added into a manager automatically...
			new AnimationEvent(sAnimationEvent.m_nID, sAnimationEvent.m_nFrame, &pAni->GetEventManager());
		}
		delete pAni;
	}
*/
	// Done
	return true;
}

bool MeshLoaderPLXml::ReadMorphTargetAnimations(Mesh &cMesh, const XmlElement &cMorphTargetAnimationsElement) const
{
	// Loop through all morph target animation elements
	const XmlElement *pMorphTargetAnimationElement = cMorphTargetAnimationsElement.GetFirstChildElement("MorphTargetAnimation");
	while (pMorphTargetAnimationElement) {
		// Load the morph target animation
		if (!ReadMorphTargetAnimation(cMesh, *pMorphTargetAnimationElement))
			return false; // Error!

		// Next element, please
		pMorphTargetAnimationElement = pMorphTargetAnimationElement->GetNextSiblingElement("MorphTargetAnimation");
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadMorphTargetAnimation(Mesh &cMesh, const XmlElement &cMorphTargetAnimationElement) const
{
	// Read attributes
	const String sName		   = cMorphTargetAnimationElement.GetAttribute("Name");
	const uint32 nMorphTargets = cMorphTargetAnimationElement.GetAttribute("MorphTargets").GetUInt32();
	const uint32 nFrames	   = cMorphTargetAnimationElement.GetAttribute("Frames").GetUInt32();

	// Add morph target animation
	MorphTargetAni *pAni = new MorphTargetAni(sName, &cMesh.GetMorphTargetAnimationManager());

	// Read morph targets
	pAni->GetMorphTargets().Resize(nMorphTargets);
	const XmlElement *pMorphTargetElement = cMorphTargetAnimationElement.GetFirstChildElement("MorphTarget");
	for (uint32 nMorphTarget=0; pMorphTargetElement&&nMorphTarget<nMorphTargets; nMorphTarget++) {
		// Get value
		const XmlNode *pNode = pMorphTargetElement->GetFirstChild();
		if (pNode && pNode->GetType() == XmlNode::Text)
			pAni->GetMorphTargets()[nMorphTarget].sName = pNode->GetValue();

		// Next element, please
		pMorphTargetElement = pMorphTargetElement->GetNextSiblingElement("MorphTarget");
	}

	// Read frame keys
	Array<MorphTargetAni::FrameKeys> &lstFrameKeys = pAni->GetFrameKeys();
	lstFrameKeys.Resize(nFrames);
	const XmlElement *pFrameKeysElement = cMorphTargetAnimationElement.GetFirstChildElement("FrameKeys");
	if (pFrameKeysElement) {
		// Loop through all frame key elements
		const XmlElement *pFrameKeyElement = pFrameKeysElement->GetFirstChildElement("FrameKey");
		for (uint32 nFrame=0; pFrameKeyElement&&nFrame<nFrames; nFrame++) {
			// Read the frame key data
			Array<float> &lstFrameKeysT = lstFrameKeys[nFrame].lstFrameKeys;
			lstFrameKeysT.Resize(nMorphTargets);
			if (lstFrameKeysT.GetNumOfElements()) {
				// Get value
				const XmlNode *pNode = pFrameKeyElement->GetFirstChild();
				if (pNode && pNode->GetType() == XmlNode::Text) {
					const String sValue = pNode->GetValue();
					if (sValue.GetLength()) {
						Tokenizer cTokenizer;
						cTokenizer.Start(sValue);
						for (uint32 nValue=0; nValue<lstFrameKeysT.GetNumOfElements(); nValue++) {
							// Get the next token
							const String sToken = cTokenizer.GetNextToken();
							if (sToken.GetLength()) {
								// Parse the token and set the index
								lstFrameKeysT.GetData()[nValue] = sToken.GetFloat();
							} else {
								// Nothing left to read in, get us out of the loop (shouldn't happen on totally valid input files...)
								nValue = lstFrameKeysT.GetNumOfElements();
							}
						}
					}
				}
			}

			// Next element, please
			pFrameKeyElement = pFrameKeyElement->GetNextSiblingElement("FrameKey");
		}
	}
	pAni->SetEndFrame(nFrames ? nFrames-1 : 0);
	pAni->SetFlags(AnimationInfo::Loop);
	pAni->SetType(1);

	// Done
	return true;
}

bool MeshLoaderPLXml::ReadMeshBoundingBox(Mesh &cMesh, const XmlElement &cBoundingBoxElement) const
{
	// Get min
	Vector3 vMin;
	vMin.FromString(cBoundingBoxElement.GetAttribute("Min"));

	// Get max
	Vector3 vMax;
	vMax.FromString(cBoundingBoxElement.GetAttribute("Max"));

	// Set the mesh bounding box
	cMesh.SetBoundingBox(vMin, vMax);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ File saving                                           ]
//[-------------------------------------------------------]
bool MeshLoaderPLXml::WriteMeshFile(Mesh &cMesh, XmlElement &cMeshElement) const
{
	// Write mesh data
	if (WriteMaterials(cMesh, cMeshElement)) {
		if (WriteMesh(cMesh, cMeshElement)) {
			// Write skeleton data
			Array<SkeletonHandler*> &lstSkeletonHandlers = cMesh.GetSkeletonHandlers();
			if (lstSkeletonHandlers.GetNumOfElements()) {
				// Add skeletons
				XmlElement *pSkeletonsElement = new XmlElement("Skeletons");

				// Loop through all skeletons
				for (uint32 i=0; i<lstSkeletonHandlers.GetNumOfElements(); i++) {
					Skeleton *pSkeleton = lstSkeletonHandlers[i]->GetResource();
					if (!pSkeleton || !WriteSkeleton(*pSkeleton, i, *pSkeletonsElement))
						return false; // Error!
				}

				// Link skeletons element to parent
				cMeshElement.LinkEndChild(*pSkeletonsElement);
			}

			// Misc
			if (cMesh.GetAnchorPointManager().GetNumOfElements())
				WriteAnchorPoints(cMesh, cMeshElement);

		// [TODO] Update this
		//	if (!WriteAnimations(cMesh, cFile, cChunkStack))	 return false; // Error!

			// Write morph target animations
			if (cMesh.GetMorphTargetAnimationManager().GetNumOfElements()) {
				// Add morph target animations
				XmlElement *pMorphTargetAnimationsElement = new XmlElement("MorphTargetAnimations");

				// Loop through all morph targets
				for (uint32 i=0; i<cMesh.GetMorphTargetAnimationManager().GetNumOfElements(); i++)
					WriteMorphTargetAnimation(cMesh, i, *pMorphTargetAnimationsElement);

				// Link morph target animations element to parent
				cMeshElement.LinkEndChild(*pMorphTargetAnimationsElement);
			}
		}
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteMaterials(const Mesh &cMesh, XmlElement &cMeshElement) const
{
	// Are there any materials to save?
	if (cMesh.GetNumOfMaterials()) {
		// Add materials
		XmlElement *pMaterialsElement = new XmlElement("Materials");

		// Loop through all materials
		for (uint32 i=0; i<cMesh.GetNumOfMaterials(); i++) {
			// Add material
			XmlElement *pMaterialElement = new XmlElement("Material");

			// Write attribute
			pMaterialElement->SetAttribute("ID", i);

			// Add value
			XmlText *pValue = new XmlText(cMesh.GetMaterial(i)->GetName());
			pMaterialElement->LinkEndChild(*pValue);

			// Link materials element to parent
			pMaterialsElement->LinkEndChild(*pMaterialElement);
		}

		// Link materials element to parent
		cMeshElement.LinkEndChild(*pMaterialsElement);
	}

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteMesh(Mesh &cMesh, XmlElement &cMeshElement) const
{
	{ // Write LOD levels
		// Add LOD levels
		XmlElement *pLODLevelsElement = new XmlElement("LODLevels");

		// Loop through all LOD levels
		for (uint32 i=0; i<cMesh.GetNumOfLODLevels(); i++) {
			if (!WriteLODLevel(*cMesh.GetLODLevel(i), i, *pLODLevelsElement))
				return false; // Error!
		}

		// Link LOD levels element to parent
		cMeshElement.LinkEndChild(*pLODLevelsElement);
	}

	{ // Write morph targets
		// Add morph targets
		XmlElement *pMorphTargetsElement = new XmlElement("MorphTargets");

		// Loop through all morph targets
		for (uint32 i=0; i<cMesh.GetNumOfMorphTargets(); i++) {
			if (!WriteMorphTarget(*cMesh.GetMorphTarget(i), i, *pMorphTargetsElement))
				return false; // Error!
		}

		// Link morph targets element to parent
		cMeshElement.LinkEndChild(*pMorphTargetsElement);
	}

	{ // Write weights
		// Add weights
		XmlElement *pWeightsElement = new XmlElement("Weights");

		// Write number of weights attribute
		pWeightsElement->SetAttribute("NumOfWeights", cMesh.GetWeights().GetNumOfElements());

		// Loop through all weights
		for (uint32 i=0; i<cMesh.GetWeights().GetNumOfElements(); i++) {
			if (!WriteWeight(cMesh.GetWeights()[i], i, *pWeightsElement))
				return false; // Error!
		}

		// Link weights element to parent
		cMeshElement.LinkEndChild(*pWeightsElement);
	}

	{ // Write vertex weights
		// Add vertex weights container
		XmlElement *pVertexWeightsContainerElement = new XmlElement("VertexWeightsContainer");

		// Write number of vertex weights attribute
		pVertexWeightsContainerElement->SetAttribute("NumOfVertexWeights", String::Format("%d", cMesh.GetVertexWeights().GetNumOfElements()));

		// Loop through all vertex weights
		for (uint32 i=0; i<cMesh.GetVertexWeights().GetNumOfElements(); i++) {
			if (!WriteVertexWeights(cMesh.GetVertexWeights()[i], i, *pVertexWeightsContainerElement))
				return false; // Error!
		}

		// Link vertex weights container element to parent
		cMeshElement.LinkEndChild(*pVertexWeightsContainerElement);
	}

	// Write mesh bounding box
	return WriteMeshBoundingBox(cMesh, cMeshElement);
}

bool MeshLoaderPLXml::WriteLODLevel(MeshLODLevel &cLODLevel, uint32 nID, XmlElement &cLODLevelsElement) const
{
	// Add LOD level
	XmlElement *pLODLevelElement = new XmlElement("LODLevel");

	// Write attributes
	pLODLevelElement->SetAttribute("ID", nID);
	pLODLevelElement->SetAttribute("Distance", String(cLODLevel.GetLODDistance()));
	if (cLODLevel.GetOctree()) {
		pLODLevelElement->SetAttribute("OctreeSubdivide",	  cLODLevel.GetOctree()->GetSubdivide());
		pLODLevelElement->SetAttribute("OctreeMinGeometries", cLODLevel.GetOctree()->GetMinGeometries());
	} else {
		pLODLevelElement->SetAttribute("OctreeSubdivide",	  "0");
		pLODLevelElement->SetAttribute("OctreeMinGeometries", "0");
	}

	// Write index buffer
	IndexBuffer *pIndexBuffer = cLODLevel.GetIndexBuffer();
	if (!pIndexBuffer || !WriteIndexBuffer(*pIndexBuffer, *pLODLevelElement))
		return false; // Error!

	{ // Write geometries
		// Add geometries
		XmlElement *pGeometriesElement = new XmlElement("Geometries");

		// Loop through all geometries
		for (uint32 i=0; i<cLODLevel.GetGeometries()->GetNumOfElements(); i++) {
			if (!WriteGeometry(cLODLevel.GetGeometries()->Get(i), i, *pGeometriesElement))
				return false; // Error!
		}

		// Link geometries element to parent
		pLODLevelElement->LinkEndChild(*pGeometriesElement);
	}

	// Link materials element to parent
	cLODLevelsElement.LinkEndChild(*pLODLevelElement);

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteIndexBuffer(IndexBuffer &cIndexBuffer, XmlElement &cLODLevelElement) const
{
	// Add index buffer
	XmlElement *pIndexBufferElement = new XmlElement("IndexBuffer");

	// Write attributes
	switch (cIndexBuffer.GetElementType()) {
		case IndexBuffer::UInt:		pIndexBufferElement->SetAttribute("ElementType", "UInt");	break;
		case IndexBuffer::UShort:	pIndexBufferElement->SetAttribute("ElementType", "UShort");	break;
		case IndexBuffer::UByte:	pIndexBufferElement->SetAttribute("ElementType", "UByte");	break;
	}
	pIndexBufferElement->SetAttribute("Elements", cIndexBuffer.GetNumOfElements());

	// Write index buffer
	bool bResult = true; // No error by default
	void *pData = cIndexBuffer.Lock(Lock::ReadOnly);
	if (pData) {
		// Loop through all indices
		String sValue;
		switch (cIndexBuffer.GetElementType()) {
			case IndexBuffer::UInt:
				for (uint32 i=0; i<cIndexBuffer.GetNumOfElements(); i++)
					sValue += static_cast<uint32*>(pData)[i];
				break;

			case IndexBuffer::UShort:
				for (uint32 i=0; i<cIndexBuffer.GetNumOfElements(); i++)
					sValue += static_cast<uint32>(static_cast<uint16*>(pData)[i]);
				break;

			case IndexBuffer::UByte:
				for (uint32 i=0; i<cIndexBuffer.GetNumOfElements(); i++)
					sValue += static_cast<uint8*>(pData)[i];
				break;
		}

		// Add value
		XmlText *pValue = new XmlText(sValue);
		pIndexBufferElement->LinkEndChild(*pValue);

		// Unlock the index buffer
		cIndexBuffer.Unlock();
	} else {
		// Error!
		bResult = false;
	}

	// Link index buffer element to parent
	cLODLevelElement.LinkEndChild(*pIndexBufferElement);

	// Done
	return bResult;
}

bool MeshLoaderPLXml::WriteGeometry(const Geometry &cGeometry, uint32 nID, XmlElement &cGeometriesElement) const
{
	// Add geometry
	XmlElement *pGeometryElement = new XmlElement("Geometry");

	// Write attributes
	pGeometryElement->SetAttribute("ID", nID);
	if (cGeometry.GetName().GetLength())
		pGeometryElement->SetAttribute("Name", cGeometry.GetName());
	if (cGeometry.GetFlags())
		pGeometryElement->SetAttribute("Flags", cGeometry.GetFlags());
	pGeometryElement->SetAttribute("Active", cGeometry.IsActive());
	switch (cGeometry.GetPrimitiveType()) {
		case Primitive::PointList:		pGeometryElement->SetAttribute("PrimitiveType", "PointList");		break;
		case Primitive::LineList:		pGeometryElement->SetAttribute("PrimitiveType", "LineList");		break;
		case Primitive::LineStrip:		pGeometryElement->SetAttribute("PrimitiveType", "LineStrip");		break;
		case Primitive::TriangleList:	pGeometryElement->SetAttribute("PrimitiveType", "TriangleList");	break;
		case Primitive::TriangleStrip:	pGeometryElement->SetAttribute("PrimitiveType", "TriangleStrip");	break;
		case Primitive::TriangleFan:	pGeometryElement->SetAttribute("PrimitiveType", "TriangleFan");		break;

		case Primitive::Number:
		case Primitive::Unknown:
		default:
			// Error!
			break;
	}
	pGeometryElement->SetAttribute("Material", cGeometry.GetMaterial());
	pGeometryElement->SetAttribute("StartIndex", cGeometry.GetStartIndex());
	pGeometryElement->SetAttribute("IndexSize", cGeometry.GetIndexSize());

	// Link geometry element to parent
	cGeometriesElement.LinkEndChild(*pGeometryElement);

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteWeight(const Weight &cWeight, uint32 nID, XmlElement &cWeightsElement) const
{
	// Add weight
	XmlElement *pWeightElement = new XmlElement("Weight");

	// Write attributes
	pWeightElement->SetAttribute("ID", nID);
	pWeightElement->SetAttribute("Joint", cWeight.GetJoint());
	pWeightElement->SetAttribute("Bias", String(cWeight.GetBias()));

	// Link weight element to parent
	cWeightsElement.LinkEndChild(*pWeightElement);

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteVertexWeights(VertexWeights &cVertexWeights, uint32 nID, XmlElement &cVertexWeightsElement) const
{
	// Add vertex weights
	XmlElement *pVertexWeightsElement = new XmlElement("VertexWeights");

	// Write attributes
	Array<uint32> &cWeights = cVertexWeights.GetWeights();
	pVertexWeightsElement->SetAttribute("ID", nID);
	pVertexWeightsElement->SetAttribute("NumOfWeights", cVertexWeights.GetWeights().GetNumOfElements());

	// Write vertex weights
	if (cVertexWeights.GetWeights().GetNumOfElements()) {
		// Loop through all vertex weights
		String sValue;
		for (uint32 i=0; i<cVertexWeights.GetWeights().GetNumOfElements(); i++)
			sValue += cWeights.GetData()[i];

		// Add value
		XmlText *pValue = new XmlText(sValue);
		pVertexWeightsElement->LinkEndChild(*pValue);
	}

	// Link weight element to parent
	cVertexWeightsElement.LinkEndChild(*pVertexWeightsElement);

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteMorphTarget(MeshMorphTarget &cMorphTarget, uint32 nID, XmlElement &cMorphTargetsElement) const
{
	// Add morph target
	XmlElement *pMorphTargetElement = new XmlElement("MorphTarget");

	// Write attributes
	pMorphTargetElement->SetAttribute("ID", nID);
	pMorphTargetElement->SetAttribute("Name", cMorphTarget.GetName());
	pMorphTargetElement->SetAttribute("Relative", cMorphTarget.IsRelative());
	pMorphTargetElement->SetAttribute("VertexIDs", cMorphTarget.GetVertexIDs().GetNumOfElements());
	pMorphTargetElement->SetAttribute("VertexBuffers", "1");

	// Write vertex ID's
	Array<uint32> &lstVertexIDs = cMorphTarget.GetVertexIDs();
	if (lstVertexIDs.GetNumOfElements()) {
		// Add vertex ID's
		XmlElement *pVertexIDsElement = new XmlElement("VertexIDs");

		// Loop through all vertex ID's
		String sValue;
		for (uint32 i=0; i<lstVertexIDs.GetNumOfElements(); i++)
			sValue += String::Format("%d ", lstVertexIDs.GetData()[i]);

		// Add value
		XmlText *pValue = new XmlText(sValue);
		pVertexIDsElement->LinkEndChild(*pValue);

		// Link vertex ID's element to parent
		pMorphTargetElement->LinkEndChild(*pVertexIDsElement);
	}

	{ // Write vertex buffers
		// Add vertex buffers
		XmlElement *pVertexBuffersElement = new XmlElement("VertexBuffers");

		// Loop through all vertex buffers
		for (uint32 i=0; i<1; i++) {
			if (!WriteVertexBuffer(*cMorphTarget.GetVertexBuffer(), i, *pVertexBuffersElement))
				return false; // Error!
		}

		// Link vertex buffers element to parent
		pMorphTargetElement->LinkEndChild(*pVertexBuffersElement);
	}

	// Link morph target element to parent
	cMorphTargetsElement.LinkEndChild(*pMorphTargetElement);

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteVertexBuffer(VertexBuffer &cVertexBuffer, uint32 nID, XmlElement &pVertexBuffersElement) const
{
	// Add vertex buffer
	XmlElement *pVertexBufferElement = new XmlElement("VertexBuffer");

	// Write attributes
	pVertexBufferElement->SetAttribute("ID", nID);
	pVertexBufferElement->SetAttribute("VertexAttributes", cVertexBuffer.GetNumOfVertexAttributes());
	pVertexBufferElement->SetAttribute("Vertices", cVertexBuffer.GetNumOfElements());

	{ // Write vertex attributes
		// Add vertex attributes
		XmlElement *pVertexAttributesElement = new XmlElement("VertexAttributes");

		// Loop through all vertex attributes
		for (uint32 i=0; i<cVertexBuffer.GetNumOfVertexAttributes(); i++) {
			if (!WriteVertexAttribute(*cVertexBuffer.GetVertexAttribute(i), i, *pVertexAttributesElement))
				return false; // Error!
		}

		// Link vertex attributes element to parent
		pVertexBufferElement->LinkEndChild(*pVertexAttributesElement);
	}

	// Write vertex buffer
	bool bResult = true; // No error by default
	if (cVertexBuffer.Lock(Lock::ReadOnly)) {
		// Add vertices
		XmlElement *pVerticesElement = new XmlElement("Vertices");

		// Vertex element type
		Array<String> lstElementType;
		lstElementType.Add("RGBA");
		lstElementType.Add("Float1");
		lstElementType.Add("Float2");
		lstElementType.Add("Float3");
		lstElementType.Add("Float4");
		lstElementType.Add("Short2");
		lstElementType.Add("Short4");

		// Loop through all elements
		for (uint32 nVertex=0; nVertex<cVertexBuffer.GetNumOfElements(); nVertex++) {
			// Add vertex
			XmlElement *pVertexElement = new XmlElement("Vertex");

			// Write attributes
			pVertexElement->SetAttribute("ID", nVertex);

			// Loop through all vertex attributes
			for (uint32 nAttribute=0; nAttribute<cVertexBuffer.GetNumOfVertexAttributes(); nAttribute++) {
				// Get the vertex attribute
				const VertexBuffer::Attribute *pVertexAttribute = cVertexBuffer.GetVertexAttribute(nAttribute);

				// Add element
				XmlElement *pElement = new XmlElement(lstElementType[pVertexAttribute->nType]);

				// Write attributes
				pElement->SetAttribute("ID", nAttribute);

				// Get the vertex data
				const void *pData = cVertexBuffer.GetData(nVertex, pVertexAttribute->nSemantic, pVertexAttribute->nChannel);

				// Add value
				String sValue;
				switch (pVertexAttribute->nType) {
					case VertexBuffer::RGBA:	sValue = cVertexBuffer.GetColor(nVertex, pVertexAttribute->nChannel).ToString();																												break;
					case VertexBuffer::Float1:	sValue = *static_cast<const float*>(pData);																																						break;
					case VertexBuffer::Float2:	sValue = String::Format("%f %f", static_cast<const float*>(pData)[0], static_cast<const float*>(pData)[1]);																						break;
					case VertexBuffer::Float3:	sValue = String::Format("%f %f %f", static_cast<const float*>(pData)[0], static_cast<const float*>(pData)[1], static_cast<const float*>(pData)[2]);												break;
					case VertexBuffer::Float4:	sValue = String::Format("%f %f %f %f", static_cast<const float*>(pData)[0], static_cast<const float*>(pData)[1], static_cast<const float*>(pData)[2], static_cast<const float*>(pData)[3]);		break;
					case VertexBuffer::Short2:	sValue = String::Format("%d %d", static_cast<const uint16*>(pData)[0], static_cast<const uint16*>(pData)[1]);																					break;
					case VertexBuffer::Short4:	sValue = String::Format("%d %d %d %d", static_cast<const uint16*>(pData)[0], static_cast<const uint16*>(pData)[1], static_cast<const uint16*>(pData)[2], static_cast<const uint16*>(pData)[3]);	break;

					case VertexBuffer::Half1:
					case VertexBuffer::Half2:
					case VertexBuffer::Half3:
					case VertexBuffer::Half4:
						// [TODO] Implement me
						break;
				}
				XmlText *pValue = new XmlText(sValue);
				pElement->LinkEndChild(*pValue);

				// Link element to parent
				pVertexElement->LinkEndChild(*pElement);
			}

			// Link vertex element to parent
			pVerticesElement->LinkEndChild(*pVertexElement);
		}

		// Link vertices element to parent
		pVertexBufferElement->LinkEndChild(*pVerticesElement);

		// Unlock the vertex buffer
		cVertexBuffer.Unlock();
	} else {
		// Error!
		bResult = false;
	}

	// Link vertex buffer element to parent
	pVertexBuffersElement.LinkEndChild(*pVertexBufferElement);

	// Done
	return bResult;
}

bool MeshLoaderPLXml::WriteVertexAttribute(const VertexBuffer::Attribute &cVertexAttribute, uint32 nID, XmlElement &cVertexAttributesElement) const
{
	// Add vertex attribute
	XmlElement *pVertexAttributeElement = new XmlElement("VertexAttribute");

	// Write attributes
	pVertexAttributeElement->SetAttribute("ID", nID);
	switch (cVertexAttribute.nSemantic) {
		case VertexBuffer::Position:		pVertexAttributeElement->SetAttribute("Semantic", "Position");		break;
		case VertexBuffer::BlendWeight:		pVertexAttributeElement->SetAttribute("Semantic", "BlendWeight");	break;
		case VertexBuffer::Normal:			pVertexAttributeElement->SetAttribute("Semantic", "Normal");		break;
		case VertexBuffer::Color:			pVertexAttributeElement->SetAttribute("Semantic", "Color");			break;
		case VertexBuffer::FogCoord:		pVertexAttributeElement->SetAttribute("Semantic", "FogCoord");		break;
		case VertexBuffer::PointSize:		pVertexAttributeElement->SetAttribute("Semantic", "PointSize");		break;
		case VertexBuffer::BlendIndices:	pVertexAttributeElement->SetAttribute("Semantic", "BlendIndices");	break;
		case VertexBuffer::TexCoord:		pVertexAttributeElement->SetAttribute("Semantic", "TexCoord");		break;
		case VertexBuffer::Tangent:			pVertexAttributeElement->SetAttribute("Semantic", "Tangent");		break;
		case VertexBuffer::Binormal:		pVertexAttributeElement->SetAttribute("Semantic", "Binormal");		break;
	}
	pVertexAttributeElement->SetAttribute("Channel", cVertexAttribute.nChannel);
	switch (cVertexAttribute.nType) {
		case VertexBuffer::RGBA:	pVertexAttributeElement->SetAttribute("Type", "RGBA");		break;
		case VertexBuffer::Float1:	pVertexAttributeElement->SetAttribute("Type", "Float1");	break;
		case VertexBuffer::Float2:	pVertexAttributeElement->SetAttribute("Type", "Float2");	break;
		case VertexBuffer::Float3:	pVertexAttributeElement->SetAttribute("Type", "Float3");	break;
		case VertexBuffer::Float4:	pVertexAttributeElement->SetAttribute("Type", "Float4");	break;
		case VertexBuffer::Short2:	pVertexAttributeElement->SetAttribute("Type", "Short2");	break;
		case VertexBuffer::Short4:	pVertexAttributeElement->SetAttribute("Type", "Short4");	break;

		case VertexBuffer::Half1:
		case VertexBuffer::Half2:
		case VertexBuffer::Half3:
		case VertexBuffer::Half4:
			// [TODO] Implement me
			break;
	}

	// Link vertex attribute element to parent
	cVertexAttributesElement.LinkEndChild(*pVertexAttributeElement);

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteSkeleton(Skeleton &cSkeleton, uint32 nID, XmlElement &cSkeletonsElement) const
{
	// Add skeleton
	XmlElement *pSkeletonElement = new XmlElement("Skeleton");

	// Write attributes
	pSkeletonElement->SetAttribute("ID", nID);
	pSkeletonElement->SetAttribute("Name", cSkeleton.GetName());
	pSkeletonElement->SetAttribute("Joints", cSkeleton.GetNumOfElements());
	pSkeletonElement->SetAttribute("Frames", cSkeleton.GetFrameKeys().GetNumOfElements());

	{ // Write joints
		// Add joints
		XmlElement *pJointsElement = new XmlElement("Joints");

		// Loop through all joints
		for (uint32 i=0; i<cSkeleton.GetNumOfElements(); i++) {
			// Get the current joint
			const Joint *pJoint = cSkeleton.GetByIndex(i);

			// Add joint
			XmlElement *pJointElement = new XmlElement("Joint");

			// Write attributes
			pJointElement->SetAttribute("ID", i);
			pJointElement->SetAttribute("Name", pJoint->GetName());
			pJointElement->SetAttribute("Parent", pJoint->GetParent());
			pJointElement->SetAttribute("AnimatedComponents", cSkeleton.GetJoints()[i].nAnimatedComponents);

			// Link joint element to parent
			pJointsElement->LinkEndChild(*pJointElement);
		}

		// Link joints element to parent
		pSkeletonElement->LinkEndChild(*pJointsElement);
	}

	{ // Write base frame
		// Add base frame
		XmlElement *pBaseFrameElement = new XmlElement("BaseFrame");

		// Loop through all joints
		for (uint32 i=0; i<cSkeleton.GetNumOfElements(); i++) {
			// Get the current joint
			const Joint *pJoint = cSkeleton.GetByIndex(i);

			// Add joint state
			XmlElement *pJointStateElement = new XmlElement("JointState");

			// Write attributes
			pJointStateElement->SetAttribute("ID", i);
			pJointStateElement->SetAttribute("Translation", pJoint->GetTranslation().ToString());
			pJointStateElement->SetAttribute("Rotation", pJoint->GetRotation().ToString());
			pJointStateElement->SetAttribute("TranslationJointSpace", pJoint->GetTranslationJointSpace().ToString());
			pJointStateElement->SetAttribute("RotationJointSpace", pJoint->GetRotationJointSpace().ToString());

			// Link joint state element to parent
			pBaseFrameElement->LinkEndChild(*pJointStateElement);
		}

		// Link base frame element to parent
		pSkeletonElement->LinkEndChild(*pBaseFrameElement);
	}

	{ // Write frame keys
		// Add frame keys
		XmlElement *pFrameKeysElement = new XmlElement("FrameKeys");

		// Loop through all frame keys
		const Array<AnimationBase::FrameKeys> &lstFrameKeys = cSkeleton.GetFrameKeys();
		for (uint32 nFrameKey=0; nFrameKey<lstFrameKeys.GetNumOfElements(); nFrameKey++) {
			// Add frame key
			XmlElement *pFrameKeyElement = new XmlElement("FrameKey");

			// Write attributes
			pFrameKeyElement->SetAttribute("ID", nFrameKey);

			// Write keys
			Array<float> &lstFrameKeysT = lstFrameKeys[nFrameKey].lstFrameKeys;
			if (lstFrameKeysT.GetNumOfElements()) {
				// Loop through all keys
				String sValue;
				for (uint32 i=0; i<lstFrameKeysT.GetNumOfElements(); i++) {
					sValue += lstFrameKeysT.GetData()[i];
					sValue += ' ';
				}

				// Add value
				XmlText *pValue = new XmlText(sValue);
				pFrameKeyElement->LinkEndChild(*pValue);
			}

			// Link frame key element to parent
			pFrameKeysElement->LinkEndChild(*pFrameKeyElement);
		}

		// Link frame keys element to parent
		pSkeletonElement->LinkEndChild(*pFrameKeysElement);
	}

	// Link skeleton element to parent
	cSkeletonsElement.LinkEndChild(*pSkeletonElement);

	// Done
	return true;
}

bool MeshLoaderPLXml::WriteAnchorPoints(Mesh &cMesh, XmlElement &cMeshElement) const
{
	// Add anchor points
	XmlElement *pAnchorPointsElement = new XmlElement("AnchorPoints");

	// Write attributes
	pAnchorPointsElement->SetAttribute("AnchorPoints", "%d");

	// Loop through all anchor points
	for (uint32 i=0; i<cMesh.GetAnchorPointManager().GetNumOfElements(); i++) {
		// Get the current anchor point
		const AnchorPoint *pAnchorPoint = cMesh.GetAnchorPointManager().GetByIndex(i);

		// Add anchor point
		XmlElement *pAnchorPointElement = new XmlElement("AnchorPoint");

		// Write attributes
		pAnchorPointElement->SetAttribute("Name", pAnchorPoint->GetName());
		pAnchorPointElement->SetAttribute("Type", pAnchorPoint->GetType());
		pAnchorPointElement->SetAttribute("ID",   pAnchorPoint->GetID());

		// Link anchor point element to parent
		pAnchorPointsElement->LinkEndChild(*pAnchorPointElement);
	}

	// Link anchor points element to parent
	cMeshElement.LinkEndChild(*pAnchorPointsElement);

	// Done
	return true;
}

// [TODO] Update this
/*
bool MeshLoaderPLXml::WriteAnimations(const Mesh &cMesh, File &cFile, Stack<MeshFile::Chunk> &cChunkStack) const
{
	// Start chunk
	if (BeginChunk(cFile, cChunkStack, MeshFile::CHUNK_ANIMATIONS)) {
		// Write animations header
		MeshFile::Animations cAnimations;
		cAnimations.nAnimations = cMesh.GetAnimationPool().GetNumOfElements();
		if (cFile.Write(&cAnimations, 1, sizeof(cAnimations))) {
			// Write animations
			for (uint32 i=0; i<cAnimations.nAnimations; i++) {
				const AnimationInfo *pAni = (AnimationInfo*)cMesh.GetAnimationPool().Get(i);
				// Write animation
				MeshFile::Animation sAnimation;
				strcpy(sAnimation.szName, pAni->GetName());
				sAnimation.nType   = pAni->GetType();
				sAnimation.nStart  = pAni->GetStartFrame();
				sAnimation.nEnd    = pAni->GetEndFrame();
				sAnimation.fSpeed  = pAni->GetSpeed();
				sAnimation.nFlags  = pAni->GetFlags();
				sAnimation.nEvents = pAni->GetEventPool().GetNumOfElements();
				if (!cFile.Write(&sAnimation, 1, sizeof(sAnimation)))
					return false; // Error!

				// Write animation frames
				for (uint32 j=0; j<pAni->GetNumOfFrames(); j++) {
					MeshFile::AnimationFrame sAnimationFrame;
					sAnimationFrame.m_fSpeed = pAni->GetFrameInfo(j)->GetSpeed();
					if (!cFile.Write(&sAnimationFrame, 1, sizeof(sAnimationFrame)))
						return false; // Error!
				}

				// Write animation events
				for (uint32 j=0; j<sAnimation.nEvents; j++) {
					MeshFile::AnimationEvent sAnimationEvent;
					sAnimationEvent.m_nID    = ((AnimationEvent*)pAni->GetEventPool().Get(j))->GetID();
					sAnimationEvent.m_nFrame = ((AnimationEvent*)pAni->GetEventPool().Get(j))->GetFrame();
					if (!cFile.Write(&sAnimationEvent, 1, sizeof(sAnimationEvent)))
						return false; // Error!
				}
			}

			// End chunk
			return EndChunk(cFile, cChunkStack);
		}
	}

	// Error!
	return false;
}
*/

bool MeshLoaderPLXml::WriteMorphTargetAnimation(Mesh &cMesh, uint32 nAnimation, XmlElement &cMorphTargetAnimationsElement) const
{
	// Get the animation
	MorphTargetAni *pAni = cMesh.GetMorphTargetAnimationManager().GetByIndex(nAnimation);
	if (pAni) {
		// Add morph target animation
		XmlElement *pMorphTargetAnimationElement = new XmlElement("MorphTargetAnimation");

		// Write attributes
		pMorphTargetAnimationElement->SetAttribute("ID",		   nAnimation);
		pMorphTargetAnimationElement->SetAttribute("Name",		   pAni->GetName());
		pMorphTargetAnimationElement->SetAttribute("MorphTargets", pAni->GetMorphTargets().GetNumOfElements());
		pMorphTargetAnimationElement->SetAttribute("Frames",	   pAni->GetFrameKeys().GetNumOfElements());

		{ // Write morph targets
			// Add morph targets
			XmlElement *pMorphTargetsElement = new XmlElement("MorphTargets");

			// Loop through all morph targets
			for (uint32 i=0; i<pAni->GetMorphTargets().GetNumOfElements(); i++) {
				// Add morph target
				XmlElement *pMorphTargetElement = new XmlElement("MorphTarget");

				// Write attributes
				pMorphTargetElement->SetAttribute("ID", i);

				// Add value
				XmlText *pValue = new XmlText(pAni->GetMorphTargets()[i].sName);
				pMorphTargetElement->LinkEndChild(*pValue);

				// Link morph target element to parent
				pMorphTargetsElement->LinkEndChild(*pMorphTargetElement);
			}

			// Link morph targets element to parent
			pMorphTargetAnimationElement->LinkEndChild(*pMorphTargetsElement);
		}

		{ // Write frame keys
			// Add frame keys
			XmlElement *pFrameKeysElement = new XmlElement("FrameKeys");

			// Loop through all frame keys
			const Array<MorphTargetAni::FrameKeys> &lstFrameKeys = pAni->GetFrameKeys();
			for (uint32 nFrameKey=0; nFrameKey<lstFrameKeys.GetNumOfElements(); nFrameKey++) {
				// Add frame key
				XmlElement *pFrameKeyElement = new XmlElement("FrameKey");

				// Write attributes
				pFrameKeyElement->SetAttribute("ID", nFrameKey);

				// Write keys
				Array<float> &lstFrameKeysT = lstFrameKeys[nFrameKey].lstFrameKeys;
				if (lstFrameKeysT.GetNumOfElements()) {
					// Loop through all keys
					String sValue;
					for (uint32 i=0; i<lstFrameKeysT.GetNumOfElements(); i++) {
						sValue += lstFrameKeysT.GetData()[i];
						sValue += ' ';
					}

					// Add value
					XmlText *pValue = new XmlText(sValue);
					pFrameKeyElement->LinkEndChild(*pValue);
				}

				// Link frame key element to parent
				pFrameKeysElement->LinkEndChild(*pFrameKeyElement);
			}

			// Link frame keys element to parent
			pMorphTargetAnimationElement->LinkEndChild(*pFrameKeysElement);
		}

		// Link morph target animation element to parent
		cMorphTargetAnimationsElement.LinkEndChild(*pMorphTargetAnimationElement);

		// Done
		return true;
	}

	// Error!
	return false;
}

bool MeshLoaderPLXml::WriteMeshBoundingBox(const Mesh &cMesh, XmlElement &cMeshElement) const
{
	// Add bounding box
	XmlElement *pBoundingBoxElement = new XmlElement("BoundingBox");

	// Get the mesh bounding box
	Vector3 vMin, vMax;
	cMesh.GetBoundingBox(vMin, vMax);

	// Write attributes
	pBoundingBoxElement->SetAttribute("Min", vMin.ToString());
	pBoundingBoxElement->SetAttribute("Max", vMax.ToString());

	// Link bounding box element to parent
	cMeshElement.LinkEndChild(*pBoundingBoxElement);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
