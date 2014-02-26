/*********************************************************\
 *  File: AssimpSceneLoader.cpp                          *
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
#include <Assimp/scene.h>
#include <Assimp/Importer.hpp>
#include <Assimp/postProcess.h>
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>
#include <PLMath/Matrix3x3.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/Mesh.h>
#include <PLMesh/Geometry.h>
#include <PLMesh/MeshManager.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLMesh/MeshMorphTarget.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SNSpotLight.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include "PLAssimp/IOSystem.h"
#include "PLAssimp/AssimpSceneLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
AssimpSceneLoader::AssimpSceneLoader() :
	m_pContainer(nullptr),
	m_pAssimpScene(nullptr)
{
}

/**
*  @brief
*    Constructor
*/
AssimpSceneLoader::AssimpSceneLoader(const String &sDefaultTextureFileExtension) : AssimpLoader(sDefaultTextureFileExtension),
	m_pContainer(nullptr),
	m_pAssimpScene(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
AssimpSceneLoader::~AssimpSceneLoader()
{
}

/**
*  @brief
*    Loads the scene
*/
bool AssimpSceneLoader::Load(SceneContainer &cContainer, File &cFile, uint8 nQuality, const String &sHint)
{
	// Reset states
	m_pContainer	= &cContainer;
	m_pAssimpScene	= nullptr;

	// Create an instance of the Assimp importer class
	Assimp::Importer cAssimpImporter;

	// Get the magic filename (concept based on "Assimp::MemoryIOSystem")
	const String sMagicFilename = (IOSystem::MagicDefaultFilename + String('.') + (sHint.GetLength() ? sHint : cFile.GetUrl().GetExtension()));

	// Set IO handler (the importer takes ownership of the object and will destroy it afterwards)
	cAssimpImporter.SetIOHandler(new IOSystem(cFile, sMagicFilename.GetASCII(), sMagicFilename.GetLength()));

	// Let Assimp load in the scene (scene remains in possession of the importer instance)
	m_pAssimpScene = cAssimpImporter.ReadFile(sMagicFilename.GetUTF8(), QualityToAssimpFlags(nQuality)|aiProcess_TransformUVCoords|aiProcess_FlipUVs);
	if (m_pAssimpScene) {
		// Load the scene recursively
		LoadRec(cContainer, *m_pAssimpScene->mRootNode);

		// Load the scene lights
		LoadLights(cContainer);

		// Load the scene cameras
		LoadCameras(cContainer);

		// Done
		return true;
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
*    Loads the scene lights
*/
void AssimpSceneLoader::LoadLights(SceneContainer &cParentContainer)
{
	// Loop through all lights this Assimp scene is providing
	for (unsigned int nAssimpLight=0; nAssimpLight<m_pAssimpScene->mNumLights; nAssimpLight++) {
		// Get the Assimp light instance
		const aiLight &cAssimpLight = *m_pAssimpScene->mLights[nAssimpLight];

		// Create the PixelLight light scene node
		SNLight *pPLLight = nullptr;
		switch (cAssimpLight.mType) {
			case aiLightSource_DIRECTIONAL:
				pPLLight = static_cast<SNLight*>(cParentContainer.Create("PLScene::SNDirectionalLight", AssimpStringToPL(cAssimpLight.mName)));
				break;

			case aiLightSource_POINT:
				pPLLight = static_cast<SNLight*>(cParentContainer.Create("PLScene::SNPointLight", AssimpStringToPL(cAssimpLight.mName)));
				break;

			case aiLightSource_SPOT:
				pPLLight = static_cast<SNLight*>(cParentContainer.Create("PLScene::SNSpotLight", AssimpStringToPL(cAssimpLight.mName)));
				if (pPLLight) {
					// Cast to the spot light class
					SNSpotLight *pSNSpotLight = static_cast<SNSpotLight*>(pPLLight);

					// Set inner angle of a spot light's light cone
					// -> Assimp: The angle is given in radians
					// -> PixelLight: Inner cone angle in degree for better artists usability (smaller than the outer angle)
					pSNSpotLight->SetInnerAngle(static_cast<float>(cAssimpLight.mAngleInnerCone*Math::RadToDeg));

					// Set outer angle of a spot light's light cone
					// -> Assimp: The angle is given in radians
					// -> PixelLight: Outer cone angle in degree for better artists usability
					pSNSpotLight->SetOuterAngle(static_cast<float>(cAssimpLight.mAngleOuterCone*Math::RadToDeg));
				}
				break;
		}
		if (pPLLight) {
			// Set position
			pPLLight->SetPosition(Vector3(cAssimpLight.mPosition.x, cAssimpLight.mPosition.y, cAssimpLight.mPosition.z));

			{ // Set rotation (= direction)
				Matrix3x3 mRotation;
				mRotation.LookAt(Vector3::Zero,
									Vector3(cAssimpLight.mDirection.x, cAssimpLight.mDirection.y, cAssimpLight.mDirection.z),
									Vector3::UnitY);
				Quaternion qRotation;
				qRotation.FromRotationMatrix(mRotation);
				pPLLight->GetTransform().SetRotation(qRotation);
			}

			// Set light color
			static_cast<SNLight*>(pPLLight)->Color = Color3(cAssimpLight.mColorDiffuse.r, cAssimpLight.mColorDiffuse.g, cAssimpLight.mColorDiffuse.b);

			// There's no specular color within PixelLight lights ("aiLight::mColorDiffuse")

			// There's no ambient color within PixelLight lights ("aiLight::mColorAmbient")

			// Is it a point light or a light derived from this type?
			if (pPLLight->IsPointLight()) {
				// Cast to the point light class
				SNPointLight *pSNPointLight = static_cast<SNPointLight*>(pPLLight);

				// There's no constant attenuation within PixelLight lights ("aiLight::mAttenuationConstant")

				// Set range
				pSNPointLight->SetRange(cAssimpLight.mAttenuationLinear);

				// There's no quadratic attenuation within PixelLight lights ("aiLight::mAttenuationQuadratic")
			}
		}
	}
}

/**
*  @brief
*    Loads the scene cameras
*/
void AssimpSceneLoader::LoadCameras(SceneContainer &cParentContainer)
{
	// Loop through all cameras this Assimp scene is providing
	for (unsigned int nAssimpCamera=0; nAssimpCamera<m_pAssimpScene->mNumCameras; nAssimpCamera++) {
		// Get the Assimp camera instance
		const aiCamera &cAssimpCamera = *m_pAssimpScene->mCameras[nAssimpCamera];

		// Create the PixelLight camera scene node
		SNCamera *pPLCamera = static_cast<SNCamera*>(cParentContainer.Create("PLScene::SNCamera", AssimpStringToPL(cAssimpCamera.mName)));
		if (pPLCamera) {
			// Set position
			pPLCamera->SetPosition(Vector3(cAssimpCamera.mPosition.x, cAssimpCamera.mPosition.y, cAssimpCamera.mPosition.z));

			{ // Set rotation
				Matrix3x3 mRotation;
				mRotation.LookAt(Vector3::Zero,
									Vector3(cAssimpCamera.mLookAt.x, cAssimpCamera.mLookAt.y, cAssimpCamera.mLookAt.z),
									Vector3(cAssimpCamera.mUp.x, cAssimpCamera.mUp.y, cAssimpCamera.mUp.z));
				Quaternion qRotation;
				qRotation.FromRotationMatrix(mRotation);
				pPLCamera->GetTransform().SetRotation(qRotation);
			}

			// Set field of view in degree
			// -> Assimp is using half horizontal field of view angle, in radians
			// -> For better artists usability PixelLight is using degree and full horizontal field of view angle
			pPLCamera->SetFOV(static_cast<float>(cAssimpCamera.mHorizontalFOV*Math::RadToDeg*2));

			// Set aspect factor
			pPLCamera->SetAspect(cAssimpCamera.mAspect);

			// Set near clipping plane
			pPLCamera->SetZNear(cAssimpCamera.mClipPlaneNear);

			// Set far clipping plane
			pPLCamera->SetZFar(cAssimpCamera.mClipPlaneFar);
		}
	}
}

/**
*  @brief
*    Loads the scene recursively
*/
void AssimpSceneLoader::LoadRec(SceneContainer &cParentContainer, const aiNode &cAssimpNode)
{
	// Are there any child nodes?
	if (cAssimpNode.mNumChildren) {
		// There are child nodes, this means we need a scene container for this Assimp node

		// Create the scene container for this Assimp node
		SceneContainer *pContainer = static_cast<SceneContainer*>(cParentContainer.Create("PLScene::SceneContainer", AssimpStringToPL(cAssimpNode.mName)));
		if (pContainer) {
			// Set the transformation (relative to the parent) of a PixelLight scene node by using a given Assimp node
			SetNodeTransformation(*pContainer, cAssimpNode);

			// Load the current Assimp node, don't set a transform (already set within the created scene container above)
			LoadNode(*pContainer, cAssimpNode, false);

			// Loop through all child nodes recursively
			for (unsigned int nAssimpChild=0; nAssimpChild<cAssimpNode.mNumChildren; nAssimpChild++)
				LoadRec(*pContainer, *cAssimpNode.mChildren[nAssimpChild]);
		}
	} else {
		// There are no child nodes, this is a leaf (= "PLScene::SceneNode" instead of "PLScene::SceneContainer")

		// Load the current Assimp node
		LoadNode(cParentContainer, cAssimpNode, true);
	}
}

/**
*  @brief
*    Loads the given Assimp node (not recursively)
*/
void AssimpSceneLoader::LoadNode(SceneContainer &cParentContainer, const aiNode &cAssimpNode, bool bSetTransformation)
{
	// PixelLight scene node created to represent the given Assimp node
	SceneNode *pSceneNode = nullptr;

	// Are there any meshes within this Assimp node?
	if (cAssimpNode.mNumMeshes) {
		// Has the Assimp node multiple meshes?
		if (cAssimpNode.mNumMeshes > 1) {
			// Assimp and PixelLight have different concepts when it comes to meshes with multiple materials...

			// In Assimp, each mesh can only have a single material assigned to it. As a result, when required,
			// Assimp splits one multi materials mesh into multiple meshes. So, in Assimp node can have multiple
			// meshes and each mesh can only have one material.

			// In PixelLight, one mesh is made up of one or more geometries, all using the same index and vertex buffer.
			// Each mesh geometry has one material assigned to it. This means, one mesh can have multiple materials and
			// one PixelLight scene node can only have one mesh assigned to it.

			// -> If an Assimp node has multiple meshes, we need to create a PixelLight scene container with one scene
			//    node per Assimp mesh in it.

			// Create the scene container for this Assimp node
			const String sAssimpNodeName = AssimpStringToPL(cAssimpNode.mName);
			pSceneNode = cParentContainer.Create("PLScene::SceneContainer", sAssimpNodeName);
			if (pSceneNode) {
				// Loop through all meshes this Assimp node is using and create one PixelLight mesh scene node per Assimp mesh
				SceneContainer *pContainer = static_cast<SceneContainer*>(pSceneNode);
				for (unsigned int nAssimpMesh=0; nAssimpMesh<cAssimpNode.mNumMeshes; nAssimpMesh++) {
					// Get the Assimp mesh
					const aiMesh &cAssimpMesh = *m_pAssimpScene->mMeshes[cAssimpNode.mMeshes[nAssimpMesh]];

					// Construct the PixelLight scene node name
					const String sSceneNodeName = cAssimpMesh.mName.length ? (sAssimpNodeName + '_' + AssimpStringToPL(cAssimpMesh.mName)) : (sAssimpNodeName + '_' + nAssimpMesh);

					// Load the mesh of this Assimp node
					LoadMeshNode(*pContainer, cAssimpMesh, sSceneNodeName);
				}
			}
		} else {
			// Nope, meaning one Assimp node is equal to one PixelLight scene node
			pSceneNode = LoadMeshNode(cParentContainer, *m_pAssimpScene->mMeshes[cAssimpNode.mMeshes[0]], AssimpStringToPL(cAssimpNode.mName));
		}
	} else {
		// This Assimp node has not a single mesh, so create an "PLScene::SNUnknown"-instance
		pSceneNode = cParentContainer.Create("PLScene::SNUnknown", AssimpStringToPL(cAssimpNode.mName));
	}

	// Set the transformation (relative to the parent) of a PixelLight scene node by using a given Assimp node?
	if (bSetTransformation && pSceneNode)
		SetNodeTransformation(*pSceneNode, cAssimpNode);
}

/**
*  @brief
*    Set's the transformation (relative to the parent) of a PixelLight scene node by using a given Assimp node
*/
void AssimpSceneLoader::SetNodeTransformation(SceneNode &cSceneNode, const aiNode &cAssimpNode) const
{
	// Lookout!
	// -> PixelLight matrices are stored in column major order like OpenGL does (right-handed matrix)
	// -> Assimp matrices are stored in row major order
	Matrix3x4 mPL;
	const aiMatrix4x4 &mAssimp = cAssimpNode.mTransformation;
	mPL.xx = mAssimp.a1; mPL.xy = mAssimp.a2; mPL.xz = mAssimp.a3; mPL.xw = mAssimp.a4;
	mPL.yx = mAssimp.b1; mPL.yy = mAssimp.b2; mPL.yz = mAssimp.b3; mPL.yw = mAssimp.b4;
	mPL.zx = mAssimp.c1; mPL.zy = mAssimp.c2; mPL.zz = mAssimp.c3; mPL.zw = mAssimp.c4;
//	mPL.wx = mAssimp.d1; mPL.wy = mAssimp.d2; mPL.wz = mAssimp.d3; mPL.ww = mAssimp.d4; // Not required

	// Set scene node transformation
	cSceneNode.GetTransform().SetMatrix(mPL);
}

/**
*  @brief
*    Loads the PixelLight mesh scene node of the given Assimp mesh
*/
SceneNode *AssimpSceneLoader::LoadMeshNode(SceneContainer &cParentContainer, const aiMesh &cAssimpMesh, const String &sSceneNodeName)
{
	Mesh *pPLMesh = nullptr;

	{ // Load the PixelLight mesh of the given Assimp mesh, this method takes also care of instanced meshes (each PixelLight mesh has an unique resource name)
		// Get the scene context
		SceneContext *pSceneContext = m_pContainer->GetSceneContext();
		if (pSceneContext) {
			// Get the mesh manager instance
			MeshManager &cMeshManager = pSceneContext->GetMeshManager();

			// Mesh already loaded?
			const String sAssimpMesh = Type<const aiMesh&>::ConvertToString(cAssimpMesh);
			pPLMesh = m_mapAssimpMeshToPL.Get(sAssimpMesh);
			if (!pPLMesh) {
				// Create a new PixelLight mesh
				pPLMesh = cMeshManager.Create();
				if (pPLMesh) {
					// Register this mesh so that we know it's already there
					m_mapAssimpMeshToPL.Add(sAssimpMesh, pPLMesh);

					// Load the PixelLight mesh of the given Assimp mesh
					LoadMesh(cAssimpMesh, *pPLMesh);
				}
			}
		}
	}

	// Create an "PLScene::SNMesh"-instance
	return cParentContainer.Create("PLScene::SNMesh", sSceneNodeName, pPLMesh ? ("Mesh=\"" + pPLMesh->GetName() + '\"') : "");
}

/**
*  @brief
*    Loads the PixelLight mesh of the given Assimp mesh
*/
void AssimpSceneLoader::LoadMesh(const aiMesh &cAssimpMesh, Mesh &cPLMesh)
{
	// General mesh information
	const bool   bHasNormals			   = cAssimpMesh.HasNormals();					// Are there normals?
	const bool   bHasTangentsAndBitangents = cAssimpMesh.HasTangentsAndBitangents();	// Are there tangents and bitangents?
	const bool   bHasTexCoords			   = (cAssimpMesh.GetNumUVChannels() != 0);		// Are there texture coordinates?
	const uint32 nNumOfVertices			   = cAssimpMesh.mNumVertices;					// Number of vertices
		  uint32 nNumOfIndices			   = 0;											// Number of indices (must be calculated)

	// Loop through all mesh faces and update the number of indices
	for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++)
		nNumOfIndices += cAssimpMesh.mFaces[nAssimpFace].mNumIndices;

	// Create the morph target
	MeshMorphTarget *pMorphTarget = cPLMesh.AddMorphTarget();

	// Create the LOD level
	MeshLODLevel *pLODLevel = cPLMesh.AddLODLevel();
	pLODLevel->CreateGeometries();
	pLODLevel->CreateIndexBuffer();

	// Add the mesh material (there's only one in here)
	AddMaterial(cAssimpMesh, cPLMesh);

	// Create the vertex buffer
	VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
	pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
	if (bHasNormals)
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal, 0, VertexBuffer::Float3);
	if (bHasTangentsAndBitangents) {
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Tangent,  0, VertexBuffer::Float3);
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Binormal, 0, VertexBuffer::Float3);
	}
	if (bHasTexCoords) {
		// Loop through the supported number of texture coord sets (UV(W) channels)
		for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++) {
			switch (cAssimpMesh.mNumUVComponents[nChannel]) {
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
	if (pVertexBuffer->Allocate(nNumOfVertices, Usage::Static)) {
		// Allocate index
		IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
		pIndexBuffer->SetElementTypeByMaximumIndex(nNumOfVertices - 1);
		if (pIndexBuffer->Allocate(nNumOfIndices, Usage::Static)) {
			// Lock the vertex buffer
			if (pVertexBuffer->Lock(Lock::WriteOnly)) {
				// Lock the index buffer
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					// Fill the mesh data
					FillMesh(cAssimpMesh, *pVertexBuffer, *pIndexBuffer, *pLODLevel->GetGeometries());

					// Unlock the index buffer
					pIndexBuffer->Unlock();
				}

				// Unlock the vertex buffer
				pVertexBuffer->Unlock();
			}
		}
	}
}

/**
*  @brief
*    Adds the mesh material (there's only one in here)
*/
void AssimpSceneLoader::AddMaterial(const aiMesh &cAssimpMesh, Mesh &cPLMesh)
{
	// Get the material manager instance
	if (cPLMesh.GetRenderer()) {
		MaterialManager &cMaterialManager = cPLMesh.GetRenderer()->GetRendererContext().GetMaterialManager();

		// Valid material?
		Material *pPLMaterial = nullptr;
		if (cAssimpMesh.mMaterialIndex < m_pAssimpScene->mNumMaterials) {
			// Get the Assimp material
			const aiMaterial &cAssimpMaterial = *m_pAssimpScene->mMaterials[cAssimpMesh.mMaterialIndex];

			// Is this material already registered?
			const String sAssimpMaterial = Type<const aiMaterial&>::ConvertToString(cAssimpMaterial);
			pPLMaterial = m_mapAssimpMaterialToPL.Get(sAssimpMaterial);
			if (!pPLMaterial) {
				// Nope, create and register this material right now

				// AI_MATKEY_NAME - Get Assimp material name
				aiString sAssimpMaterialName;
				cAssimpMaterial.Get(AI_MATKEY_NAME, sAssimpMaterialName);

				// Construct an unique material name to avoid material conflicts
				const String sMaterialNameOriginal = cPLMesh.GetName() + '_' + AssimpStringToPL(sAssimpMaterialName);
				String sMaterialName = sMaterialNameOriginal;
				uint32 nIndex = 0;
				while (cMaterialManager.GetByName(sMaterialName)) {
					sMaterialName = sMaterialNameOriginal + '_' + nIndex;
					nIndex++;
				}

				// Add the PixelLight material
				pPLMaterial = cMaterialManager.Create(sMaterialName);
				if (pPLMaterial) {
					// Register this material so that we know it's already there
					m_mapAssimpMaterialToPL.Add(sAssimpMaterial, pPLMaterial);

					// Convert the Assimp material into the PixelLight material
					AssimpMaterialToPL(cAssimpMaterial, *pPLMaterial);
				}
			}
		}

		// Add the PixelLight material to the PixelLight mesh - Use default material as fallback so we can see at least something
		cPLMesh.AddMaterial(pPLMaterial ? pPLMaterial : cMaterialManager.GetStandard());
	}
}

/**
*  @brief
*    Fills the mesh data recursively
*/
void AssimpSceneLoader::FillMesh(const aiMesh &cAssimpMesh, VertexBuffer &cVertexBuffer, IndexBuffer &cIndexBuffer, Array<Geometry> &lstGeometries)
{
	// General mesh information
	const bool			bHasNormals				  = cAssimpMesh.HasNormals();				// Are there normals?
	const bool			bHasTangentsAndBitangents = cAssimpMesh.HasTangentsAndBitangents();	// Are there tangents and bitangents?
	const bool			bHasTexCoords			  = (cAssimpMesh.GetNumUVChannels() != 0);	// Are there texture coordinates?
	const unsigned int *pnNumUVComponents		  = cAssimpMesh.mNumUVComponents;			// Specifies the number of components for a given UV channel (see aiMesh for details)

	// Loop through the Assimp mesh vertices
	for (unsigned int nAssimpVertex=0; nAssimpVertex<cAssimpMesh.mNumVertices; nAssimpVertex++) {
		// Get the Assimp mesh vertex position
		const aiVector3D &cAssimpVertex = cAssimpMesh.mVertices[nAssimpVertex];

		// Set vertex buffer position
		float *pfVertex = static_cast<float*>(cVertexBuffer.GetData(nAssimpVertex, VertexBuffer::Position));
		pfVertex[0] = cAssimpVertex.x;
		pfVertex[1] = cAssimpVertex.y;
		pfVertex[2] = cAssimpVertex.z;

		// Are there any normals?
		if (bHasNormals) {
			pfVertex = static_cast<float*>(cVertexBuffer.GetData(nAssimpVertex, VertexBuffer::Normal));
			const aiVector3D &cAssimpVertexNormal = cAssimpMesh.mNormals[nAssimpVertex];
			pfVertex[0] = cAssimpVertexNormal.x;
			pfVertex[1] = cAssimpVertexNormal.y;
			pfVertex[2] = cAssimpVertexNormal.z;
		}

		// Are there any tangents and bitangents?
		if (bHasTangentsAndBitangents) {
			{ // Tangent
				const aiVector3D &cAssimpVertexTangent = cAssimpMesh.mTangents[nAssimpVertex];
				pfVertex = static_cast<float*>(cVertexBuffer.GetData(nAssimpVertex, VertexBuffer::Tangent));
				pfVertex[0] = cAssimpVertexTangent.x;
				pfVertex[1] = cAssimpVertexTangent.y;
				pfVertex[2] = cAssimpVertexTangent.z;
			}
			{ // Binormal
				const aiVector3D &cAssimpVertexBinormal = cAssimpMesh.mBitangents[nAssimpVertex];
				pfVertex = static_cast<float*>(cVertexBuffer.GetData(nAssimpVertex, VertexBuffer::Binormal));
				pfVertex[0] = cAssimpVertexBinormal.x;
				pfVertex[1] = cAssimpVertexBinormal.y;
				pfVertex[2] = cAssimpVertexBinormal.z;
			}
		}

		// Are there any texture coordinates at all?
		if (bHasTexCoords) {
			// Loop through the supported number of texture coord sets (UV(W) channels)
			for (uint32 nChannel=0; nChannel<MaxNumOfTextureCoords; nChannel++) {
				// Are there any texture coordinates within the current channel?
				if (pnNumUVComponents[nChannel]) {
					const aiVector3D &cAssimpVertexTexCoord = cAssimpMesh.mTextureCoords[nChannel][nAssimpVertex];
					pfVertex = static_cast<float*>(cVertexBuffer.GetData(nAssimpVertex, VertexBuffer::TexCoord, nChannel));
					pfVertex[0] = cAssimpVertexTexCoord.x;
					if (pnNumUVComponents[nChannel] > 1) {
						pfVertex[1] = cAssimpVertexTexCoord.y;
						if (pnNumUVComponents[nChannel] > 2)
							pfVertex[2] = cAssimpVertexTexCoord.z;
					}
				}
			}
		}
	}

	// Check the index buffer element type
	switch (cIndexBuffer.GetElementType()) {
		case IndexBuffer::UInt:
		{
			// Loop through all Assimp mesh faces
			uint32 *pnIndex = static_cast<uint32*>(cIndexBuffer.GetData());
			for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++) {
				// Get the Assimp face
				const aiFace &cAssimpFace = cAssimpMesh.mFaces[nAssimpFace];

				// Loop through all indices of the Assimp face and set the PixelLight mesh vertex indices
				for (unsigned int nAssimpIndex=0; nAssimpIndex<cAssimpFace.mNumIndices; nAssimpIndex++, pnIndex++) {
					//			Assimp mesh vertex index = Where the Assimp mesh starts within the PixelLight mesh vertex buffer
					*pnIndex = cAssimpFace.mIndices[nAssimpIndex];
				}
			}
			break;
		}

		case IndexBuffer::UShort:
		{
			// Loop through all Assimp mesh faces
			uint16 *pnIndex = static_cast<uint16*>(cIndexBuffer.GetData());
			for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++) {
				// Get the Assimp face
				const aiFace &cAssimpFace = cAssimpMesh.mFaces[nAssimpFace];

				// Loop through all indices of the Assimp face and set the PixelLight mesh vertex indices
				for (unsigned int nAssimpIndex=0; nAssimpIndex<cAssimpFace.mNumIndices; nAssimpIndex++, pnIndex++) {
					//			Assimp mesh vertex index = Where the Assimp mesh starts within the PixelLight mesh vertex buffer
					*pnIndex = static_cast<uint16>(cAssimpFace.mIndices[nAssimpIndex]);
				}
			}
			break;
		}

		case IndexBuffer::UByte:
		{
			// Loop through all Assimp mesh faces
			uint8 *pnIndex = static_cast<uint8*>(cIndexBuffer.GetData());
			for (unsigned int nAssimpFace=0; nAssimpFace<cAssimpMesh.mNumFaces; nAssimpFace++) {
				// Get the Assimp face
				const aiFace &cAssimpFace = cAssimpMesh.mFaces[nAssimpFace];

				// Loop through all indices of the Assimp face and set the PixelLight mesh vertex indices
				for (unsigned int nAssimpIndex=0; nAssimpIndex<cAssimpFace.mNumIndices; nAssimpIndex++, pnIndex++) {
					//			sAsimp mesh vertex index = Where the Assimp mesh starts within the PixelLight mesh vertex buffer
					*pnIndex = static_cast<uint8>(cAssimpFace.mIndices[nAssimpIndex]);
				}
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
		cGeometry.SetMaterial(0);	// "When there's only one candidate, there's only one choice." (quote from a Monkey Island poster)
		cGeometry.SetStartIndex(0);
		cGeometry.SetIndexSize(cIndexBuffer.GetNumOfElements());
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
