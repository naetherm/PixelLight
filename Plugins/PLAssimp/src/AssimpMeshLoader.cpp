/*********************************************************\
 *  File: AssimpMeshLoader.cpp                           *
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
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/Geometry.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLMesh/MeshMorphTarget.h>
#include "PLAssimp/IOSystem.h"
#include "PLAssimp/AssimpMeshLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
AssimpMeshLoader::AssimpMeshLoader() :
	m_pMesh(nullptr),
	m_pAssimpScene(nullptr),
	m_bHasNormals(false),
	m_bHasTangentsAndBitangents(false),
	m_bHasTexCoords(false)
{
	for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++)
		m_nNumUVComponents[nChannel] = 0;
}

/**
*  @brief
*    Constructor
*/
AssimpMeshLoader::AssimpMeshLoader(const String &sDefaultTextureFileExtension) : AssimpLoader(sDefaultTextureFileExtension),
	m_pMesh(nullptr),
	m_pAssimpScene(nullptr),
	m_bHasNormals(false),
	m_bHasTangentsAndBitangents(false),
	m_bHasTexCoords(false)
{
	for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++)
		m_nNumUVComponents[nChannel] = 0;
}

/**
*  @brief
*    Destructor
*/
AssimpMeshLoader::~AssimpMeshLoader()
{
}

/**
*  @brief
*    Loads the mesh
*/
bool AssimpMeshLoader::Load(Mesh &cMesh, File &cFile, bool bStatic, uint8 nQuality, const String &sHint)
{
	// Reset states
	m_pMesh							= &cMesh;
	m_pAssimpScene					= nullptr;
	m_bHasNormals					= false;
	m_bHasTangentsAndBitangents	= false;
	m_bHasTexCoords					= false;
	for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++)
		m_nNumUVComponents[nChannel] = 0;

	// Create an instance of the Assimp importer class
	Assimp::Importer cAssimpImporter;

	// Get the magic filename (concept based on "Assimp::MemoryIOSystem")
	const String sMagicFilename = (IOSystem::MagicDefaultFilename + String('.') + (sHint.GetLength() ? sHint : cFile.GetUrl().GetExtension()));

	// Set IO handler (the importer takes ownership of the object and will destroy it afterwards)
	cAssimpImporter.SetIOHandler(new IOSystem(cFile, sMagicFilename.GetASCII(), sMagicFilename.GetLength()));

	// Let Assimp load in the scene (scene remains in possession of the importer instance)
	m_pAssimpScene = cAssimpImporter.ReadFile(sMagicFilename.GetUTF8(), QualityToAssimpFlags(nQuality)|aiProcess_TransformUVCoords|aiProcess_FlipUVs);
	if (m_pAssimpScene) {
		// Get the total number of vertices and indices required for everything as one PixelLight mesh recursively
		uint32 nNumOfVertices = 0;
		uint32 nNumOfIndices  = 0;
		GetNumOfVerticesAndIndicesRec(*m_pAssimpScene->mRootNode, nNumOfVertices, nNumOfIndices);

		// Create the morph target
		MeshMorphTarget *pMorphTarget = m_pMesh->AddMorphTarget();

		// Create the LOD level
		MeshLODLevel *pLODLevel = m_pMesh->AddLODLevel();
		pLODLevel->CreateGeometries();
		pLODLevel->CreateIndexBuffer();

		// Add the materials
		AddMaterials();

		// Create the vertex buffer
		VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (m_bHasNormals)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal, 0, VertexBuffer::Float3);
		if (m_bHasTangentsAndBitangents) {
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Tangent,  0, VertexBuffer::Float3);
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Binormal, 0, VertexBuffer::Float3);
		}
		if (m_bHasTexCoords) {
			// Loop through the supported number of texture coord sets (UV(W) channels)
			for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++) {
				switch (m_nNumUVComponents[nChannel]) {
					case 1:
						pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, nChannel, VertexBuffer::Float1);
						break;

					case 2:
						pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, nChannel, VertexBuffer::Float2);
						break;

					case 3:
						pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, nChannel, VertexBuffer::Float3);
						break;
				}
			}
		}
		if (pVertexBuffer->Allocate(nNumOfVertices, bStatic ? Usage::Static : Usage::Dynamic)) {
			// Allocate index
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			pIndexBuffer->SetElementTypeByMaximumIndex(nNumOfVertices - 1);
			if (pIndexBuffer->Allocate(nNumOfIndices, bStatic ? Usage::Static : Usage::Dynamic)) {
				// Lock the vertex buffer
				if (pVertexBuffer->Lock(Lock::WriteOnly)) {
					// Lock the index buffer
					if (pIndexBuffer->Lock(Lock::WriteOnly)) {
						// Fill the mesh data recursively
						nNumOfVertices = 0;
						nNumOfIndices  = 0;
						FillMeshRec(*m_pAssimpScene->mRootNode, *pVertexBuffer, *pIndexBuffer, *pLODLevel->GetGeometries(), aiMatrix4x4(), nNumOfVertices, nNumOfIndices);

						// Unlock the index buffer
						pIndexBuffer->Unlock();
					}

					// Unlock the vertex buffer
					pVertexBuffer->Unlock();
				}

				// Done
				return true;
			} else {
				// Error!
				PL_LOG(Error, '\"' + cFile.GetUrl().GetUrl() + "\": Failed to allocate the index buffer")
			}
		} else {
			// Error!
			PL_LOG(Error, '\"' + cFile.GetUrl().GetUrl() + "\": Failed to allocate the vertex buffer")
		}

		// Error!
		return false;
	} else {
		// Write an error message into the log
		PL_LOG(Error, cAssimpImporter.GetErrorString())

		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds the materials
*/
void AssimpMeshLoader::AddMaterials()
{
	// Get the PixelLight material manager
	MaterialManager &cMaterialManager = m_pMesh->GetRenderer()->GetRendererContext().GetMaterialManager();

	// Loop through all Assimp materials
	for (unsigned int nAssimpMaterial=0; nAssimpMaterial<m_pAssimpScene->mNumMaterials; nAssimpMaterial++) {
		// Get the Assimp material
		const aiMaterial &cAssimpMaterial = *m_pAssimpScene->mMaterials[nAssimpMaterial];

		// AI_MATKEY_NAME - Get Assimp material name
		aiString sAssimpMaterialName;
		cAssimpMaterial.Get(AI_MATKEY_NAME, sAssimpMaterialName);

		// Construct an unique material name to avoid material conflicts
		const String sMaterialNameOriginal = m_pMesh->GetName() + '_' + AssimpStringToPL(sAssimpMaterialName);
		String sMaterialName = sMaterialNameOriginal;
		uint32 nIndex = 0;
		while (cMaterialManager.GetByName(sMaterialName)) {
			sMaterialName = sMaterialNameOriginal + '_' + nIndex;
			nIndex++;
		}

		// Add the PixelLight material
		Material *pMaterial = cMaterialManager.Create(sMaterialName);
		m_pMesh->AddMaterial(pMaterial);

		// Convert the Assimp material into the PixelLight material
		if (pMaterial)
			AssimpMaterialToPL(cAssimpMaterial, *pMaterial);
	}
}

/**
*  @brief
*    Gets the total number of vertices and indices required for everything as one PixelLight mesh recursively
*/
void AssimpMeshLoader::GetNumOfVerticesAndIndicesRec(const aiNode &cAssimpNode, uint32 &nNumOfVertices, uint32 &nNumOfIndices)
{
	// Loop through all meshes this node is using
	for (unsigned int nAssimpMesh=0; nAssimpMesh<cAssimpNode.mNumMeshes; nAssimpMesh++) {
		// Get the used mesh
		const aiMesh &cAssimpMesh = *m_pAssimpScene->mMeshes[cAssimpNode.mMeshes[nAssimpMesh]];

		// Update the number of vertices
		nNumOfVertices += cAssimpMesh.mNumVertices;

		// Loop through all mesh faces and update the number of indices
		for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++)
			nNumOfIndices += cAssimpMesh.mFaces[nAssimpFace].mNumIndices;

		// Are there normals?
		if (cAssimpMesh.HasNormals())
			m_bHasNormals = true;

		// Are there tangents and bitangents?
		if (cAssimpMesh.HasTangentsAndBitangents())
			m_bHasTangentsAndBitangents = true;

		// Are there texture coordinates?
		if (cAssimpMesh.GetNumUVChannels()) {
			m_bHasTexCoords = true;

			// Loop through the supported number of texture coord sets (UV(W) channels)
			for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++) {
				if (m_nNumUVComponents[nChannel] < cAssimpMesh.mNumUVComponents[nChannel])
					m_nNumUVComponents[nChannel] = cAssimpMesh.mNumUVComponents[nChannel];
			}
		}
	}

	// Loop through all child nodes recursively
	for (unsigned int nAssimpChild=0; nAssimpChild<cAssimpNode.mNumChildren; nAssimpChild++)
		GetNumOfVerticesAndIndicesRec(*cAssimpNode.mChildren[nAssimpChild], nNumOfVertices, nNumOfIndices);
}

/**
*  @brief
*    Fills the mesh data recursively
*/
void AssimpMeshLoader::FillMeshRec(const aiNode &cAssimpNode, VertexBuffer &cVertexBuffer, IndexBuffer &cIndexBuffer, Array<Geometry> &lstGeometries, const aiMatrix4x4 &mAssimpTransformation, uint32 &nNumOfVertices, uint32 &nNumOfIndices)
{
	// We're using aiProcessPreset_TargetRealtime_Quality which also sets the flag aiProcess_SortByPType
	// resulting in 'clean' meshes which consist of a single type of primitives. So, one Assimp mesh will
	// result within one PixelLight mesh geometry.

	// Get the absolute transformation matrix of this Assimp node
	const aiMatrix4x4 mCurrentAssimpTransformation = mAssimpTransformation*cAssimpNode.mTransformation;

	// Loop through all meshes this node is using
	for (unsigned int nAssimpMesh=0; nAssimpMesh<cAssimpNode.mNumMeshes; nAssimpMesh++) {
		// Get the used mesh
		const aiMesh &cAssimpMesh = *m_pAssimpScene->mMeshes[cAssimpNode.mMeshes[nAssimpMesh]];

		// Get the start vertex inside the PixelLight mesh vertex buffer
		const uint32 nStarVertex = nNumOfVertices;

		// Loop through the Assimp mesh vertices
		for (unsigned int nAssimpVertex=0; nAssimpVertex<cAssimpMesh.mNumVertices; nAssimpVertex++) {
			// Get the Assimp mesh vertex position
			aiVector3D cAssimpVertex = cAssimpMesh.mVertices[nAssimpVertex];

			// Transform the Assimp mesh vertex position into global space
			cAssimpVertex *= mCurrentAssimpTransformation;

			// Set vertex buffer position
			float *pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::Position));
			pfVertex[0] = cAssimpVertex.x;
			pfVertex[1] = cAssimpVertex.y;
			pfVertex[2] = cAssimpVertex.z;

			// Are there any normals at all?
			if (m_bHasNormals) {
				pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::Normal));

				// Are there normals within the current mesh?
				if (cAssimpMesh.HasNormals()) {
					const aiVector3D &cAssimpVertexNormal = cAssimpMesh.mNormals[nAssimpVertex];
					pfVertex[0] = cAssimpVertexNormal.x;
					pfVertex[1] = cAssimpVertexNormal.y;
					pfVertex[2] = cAssimpVertexNormal.z;
				} else {
					// Fallback
					pfVertex[0] = 1.0f;
					pfVertex[1] = 0.0f;
					pfVertex[2] = 0.0f;
				}
			}

			// Are there any tangents and bitangents at all?
			if (m_bHasTangentsAndBitangents) {
				// Are there tangents and bitangents within the current mesh?
				if (cAssimpMesh.HasTangentsAndBitangents()) {
					{ // Tangent
						const aiVector3D &cAssimpVertexTangent = cAssimpMesh.mTangents[nAssimpVertex];
						pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::Tangent));
						pfVertex[0] = cAssimpVertexTangent.x;
						pfVertex[1] = cAssimpVertexTangent.y;
						pfVertex[2] = cAssimpVertexTangent.z;
					}
					{ // Binormal
						const aiVector3D &cAssimpVertexBinormal = cAssimpMesh.mBitangents[nAssimpVertex];
						pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::Binormal));
						pfVertex[0] = cAssimpVertexBinormal.x;
						pfVertex[1] = cAssimpVertexBinormal.y;
						pfVertex[2] = cAssimpVertexBinormal.z;
					}
				} else {
					// Fallback
					pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::Tangent));
					pfVertex[0] = 0.0f;
					pfVertex[1] = 1.0f;
					pfVertex[2] = 0.0f;
					pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::Binormal));
					pfVertex[0] = 0.0f;
					pfVertex[1] = 0.0f;
					pfVertex[2] = 1.0f;
				}
			}

			// Are there any texture coordinates at all?
			if (m_bHasTexCoords) {
				// Loop through the supported number of texture coord sets (UV(W) channels)
				for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++) {
					// Are there any texture coordinates within the current channel at all?
					if (m_nNumUVComponents[nChannel]) {
						// Are there texture coordinates within the current mesh?
						if (cAssimpMesh.HasTextureCoords(nChannel)) {
							const aiVector3D &cAssimpVertexTexCoord = cAssimpMesh.mTextureCoords[nChannel][nAssimpVertex];
							pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::TexCoord, nChannel));
							pfVertex[0] = cAssimpVertexTexCoord.x;
							if (m_nNumUVComponents[nChannel] > 1) {
								if (cAssimpMesh.mNumUVComponents[nChannel] > 1) {
									pfVertex[1] = cAssimpVertexTexCoord.y;
									if (m_nNumUVComponents[nChannel] > 2) {
										if (cAssimpMesh.mNumUVComponents[nChannel] > 2)
											pfVertex[2] = cAssimpVertexTexCoord.z;
										else
											pfVertex[2] = 0.0f; // Fallback
									}
								} else {
									// Fallback
									pfVertex[1] = 0.0f;
									if (m_nNumUVComponents[nChannel] > 2)
										pfVertex[2] = 0.0f;
								}
							}
						} else {
							// Fallback
							pfVertex = static_cast<float*>(cVertexBuffer.GetData(nNumOfVertices, VertexBuffer::TexCoord, nChannel));
							pfVertex[0] = 0.0f;
							if (m_nNumUVComponents[nChannel] > 1) {
								pfVertex[1] = 0.0f;
								if (m_nNumUVComponents[nChannel] > 2)
									pfVertex[2] = 0.0f;
							}
						}
					}
				}
			}

			// Update the number of processed vertices
			nNumOfVertices++;
		}

		// Get the start index of this geometry
		const uint32 nGeometryStartIndex = nNumOfIndices;

		// Check the index buffer element type
		switch (cIndexBuffer.GetElementType()) {
			case IndexBuffer::UInt:
			{
				// Loop through all Assimp mesh faces
				uint32 *pnIndex = static_cast<uint32*>(cIndexBuffer.GetData()) + nNumOfIndices;
				for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++) {
					// Get the Assimp face
					const aiFace &cAssimpFace = cAssimpMesh.mFaces[nAssimpFace];

					// Loop through all indices of the Assimp face and set the PixelLight mesh vertex indices
					for (unsigned int nAssimpIndex=0; nAssimpIndex<cAssimpFace.mNumIndices; nAssimpIndex++, pnIndex++) {
						//			Assimp mesh vertex index			Where the Assimp mesh starts within the PixelLight mesh vertex buffer
						*pnIndex = cAssimpFace.mIndices[nAssimpIndex] + nStarVertex;
					}

					// Update the number if processed indices
					nNumOfIndices += cAssimpFace.mNumIndices;
				}
				break;
			}

			case IndexBuffer::UShort:
			{
				// Loop through all Assimp mesh faces
				uint16 *pnIndex = static_cast<uint16*>(cIndexBuffer.GetData()) + nNumOfIndices;
				for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++) {
					// Get the Assimp face
					const aiFace &cAssimpFace = cAssimpMesh.mFaces[nAssimpFace];

					// Loop through all indices of the Assimp face and set the PixelLight mesh vertex indices
					for (unsigned int nAssimpIndex=0; nAssimpIndex<cAssimpFace.mNumIndices; nAssimpIndex++, pnIndex++) {
						//			Assimp mesh vertex index								Where the Assimp mesh starts within the PixelLight mesh vertex buffer
						*pnIndex = static_cast<uint16>(cAssimpFace.mIndices[nAssimpIndex] + nStarVertex);
					}

					// Update the number if processed indices
					nNumOfIndices += cAssimpFace.mNumIndices;
				}
				break;
			}

			case IndexBuffer::UByte:
			{
				// Loop through all Assimp mesh faces
				uint8 *pnIndex = static_cast<uint8*>(cIndexBuffer.GetData()) + nNumOfIndices;
				for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++) {
					// Get the Assimp face
					const aiFace &cAssimpFace = cAssimpMesh.mFaces[nAssimpFace];

					// Loop through all indices of the Assimp face and set the PixelLight mesh vertex indices
					for (unsigned int nAssimpIndex=0; nAssimpIndex<cAssimpFace.mNumIndices; nAssimpIndex++, pnIndex++) {
						//			sAsimp mesh vertex index								Where the Assimp mesh starts within the PixelLight mesh vertex buffer
						*pnIndex = static_cast<uint8>(cAssimpFace.mIndices[nAssimpIndex] + nStarVertex);
					}

					// Update the number if processed indices
					nNumOfIndices += cAssimpFace.mNumIndices;
				}
				break;
			}
		}

		// Get the primitive type (see also comment at the beginning of this method)
		Primitive::Enum nPrimitiveType = Primitive::Unknown;
		if (cAssimpMesh.mPrimitiveTypes & aiPrimitiveType_TRIANGLE)
			nPrimitiveType = Primitive::TriangleList;
		else if (cAssimpMesh.mPrimitiveTypes & aiPrimitiveType_LINE)
			nPrimitiveType = Primitive::LineList;
		else if (cAssimpMesh.mPrimitiveTypes & aiPrimitiveType_POINT)
			nPrimitiveType = Primitive::PointList;
		if (nPrimitiveType != Primitive::Unknown) {
			// Add the geometry
			Geometry &cGeometry = lstGeometries.Add();
			cGeometry.SetName(AssimpStringToPL(cAssimpMesh.mName));
			cGeometry.SetFlags(0);
			cGeometry.SetActive(true);
			cGeometry.SetPrimitiveType(nPrimitiveType);
			cGeometry.SetMaterial(cAssimpMesh.mMaterialIndex);
			cGeometry.SetStartIndex(nGeometryStartIndex);
			cGeometry.SetIndexSize(nNumOfIndices - nGeometryStartIndex);
		}
	}

	// Loop through all child nodes recursively
	for (unsigned int nAssimpChild=0; nAssimpChild<cAssimpNode.mNumChildren; nAssimpChild++)
		FillMeshRec(*cAssimpNode.mChildren[nAssimpChild], cVertexBuffer, cIndexBuffer, lstGeometries, mCurrentAssimpTransformation, nNumOfVertices, nNumOfIndices);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
