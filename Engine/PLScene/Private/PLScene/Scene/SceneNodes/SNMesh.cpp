/*********************************************************\
 *  File: SNMesh.cpp                                     *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/File/File.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/Loader.h>
#include <PLCore/Tools/LoadableType.h>
#include <PLCore/Tools/LoadableManager.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/Skeleton.h>
#include <PLMesh/MeshManager.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLMesh/MeshMorphTarget.h>
#include <PLMesh/SkeletonHandler.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodes/Loader/SkinLoader.h"
#include "PLScene/Scene/SceneNodes/SNMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMesh)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMesh::GetMesh() const
{
	return m_sMesh;
}

void SNMesh::SetMesh(const String &sValue)
{
	if (m_sMesh != sValue) {
		m_sMesh = sValue;

		// Is this node visible?
		if (((GetFlags() & LoadAtOnce) || IsVisible()))
			LoadMesh(m_sMesh);
	}
}

String SNMesh::GetSkin() const
{
	return m_sSkin;
}

void SNMesh::SetSkin(const String &sValue)
{
	if (m_sSkin != sValue) {
		m_sSkin = sValue;
		LoadSkin(m_sSkin);
	}
}

void SNMesh::SetFlags(uint32 nValue)
{
	// State change?
	if (GetFlags() != nValue) {
		// Update the mesh?
		const bool bUpdate = (GetFlags() & DynamicMesh)      != (nValue & DynamicMesh)      ||
							 (GetFlags() & GenerateStrips)   != (nValue & GenerateStrips)   ||
							 (GetFlags() & CalculateNormals) != (nValue & CalculateNormals) ||
							 (GetFlags() & CalculateTSVs)    != (nValue & CalculateTSVs);

		// Call base implementation
		SceneNode::SetFlags(nValue);

		// Is this node visible?
		if (bUpdate && (((GetFlags() & LoadAtOnce) || IsVisible())))
			LoadMesh(m_sMesh);
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNMesh::SNMesh() :
	Mesh(this),
	Skin(this),
	Flags(this),
	DebugFlags(this),
	m_pMeshHandler(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SNMesh::~SNMesh()
{
	// Unload the used mesh
	UnloadMesh();
}

/**
*  @brief
*    Loads a mesh
*/
bool SNMesh::LoadMesh(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Check the parameter
	if (sFilename.GetLength()) {
		// Create the mesh handler, if there's currently none
		if (m_pMeshHandler) {
			// Unload the old mesh
			UnloadMesh();
		}
		if (!m_pMeshHandler)
			m_pMeshHandler = new MeshHandler();

		// Mesh already loaded?
		PLMesh::Mesh *pMesh = GetSceneContext()->GetMeshManager().GetByName(sFilename);
		if (!pMesh) {
			// Create and load mesh
			pMesh = GetSceneContext()->GetMeshManager().LoadMesh(sFilename, sParams, sMethod, false, !(GetFlags() & DynamicMesh));
			if (pMesh) {
				// Perform some mesh operations
				if ((GetFlags() & CalculateNormals) || (GetFlags() & CalculateTSVs) || (GetFlags() & GenerateStrips)) {
					MeshMorphTarget *pMorphTarget = pMesh->GetMorphTarget(0);
					VertexBuffer *pVertexBuffer = pMorphTarget ? pMorphTarget->GetVertexBuffer() : nullptr;
					if (pVertexBuffer) {
						// Calculate the normal vectors
						if ((GetFlags() & CalculateNormals) && !pVertexBuffer->GetData(0, VertexBuffer::Normal))
							pMesh->CalculateNormals();

						// Calculate the tangent space vectors
						if ((GetFlags() & CalculateTSVs) && (!pVertexBuffer->GetData(0, VertexBuffer::Tangent) ||
							!pVertexBuffer->GetData(0, VertexBuffer::Binormal)))
							pMesh->CalculateTangentSpaceVectors();

						// Optimize geometries
						if ((GetFlags() & GenerateStrips) && pMesh->GetLODLevel(0))
							pMesh->GetLODLevel(0)->GenerateStrips();
					}
				}
			}
		}
		if (pMesh) {
			// Assign this mesh to the mesh handler
			m_pMeshHandler->SetMesh(pMesh);

			// Load a special skin?
			if (m_sSkin.GetLength())
				LoadSkin(m_sSkin);

			// Call mesh initialization function
			MeshInitFunction();

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Loads a skin
*/
bool SNMesh::LoadSkin(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Set skin filename
	m_sSkin = sFilename;

	// Is there a mesh handler?
	if (m_pMeshHandler) {
		// Get the used mesh
		PLMesh::Mesh *pMesh = m_pMeshHandler->GetMesh();
		if (pMesh) {
			// Any special skin given for this particular mesh instance within the scene?
			if (m_sSkin.GetLength()) {
				// Get filename extension (in case it's a filename)
				// -> There are file formats without an extension, so no extension must also be valid
				const String sExtension = Url(sFilename).GetExtension();

				// Supported skin file given?
				if (LoadableManager::GetInstance()->IsFormatLoadSupported(sExtension, "Skin")) {
					PL_LOG(Debug, "Load skin: " + sFilename)

					// Get loadable type
					const LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName("Skin");
					if (pLoadableType) {
						// Open the file
						File cFile;
						if (LoadableManager::GetInstance()->OpenFile(cFile, sFilename, false)) {
							// Get loader, assigned to the current loadable type, capable of processing the file
							Loader *pLoader = pLoadableType->GetLoaderForLoadingByFile(cFile);
							if (pLoader) {
								// Get method name
								static const String sLoad = "Load";
								static const String sLoadParams = "LoadParams";
								String sMethodName = sMethod;
								if (!sMethodName.GetLength())
									sMethodName = sParams.GetLength() ? sLoadParams : sLoad;

								// Get the loader implementation
								LoaderImpl *pLoaderImpl = pLoader->GetImpl();
								if (pLoaderImpl) {
									// Finally, load the skin!
									if (sParams.GetLength()) {
										pLoaderImpl->CallMethod(sMethodName, "Param0=\"" + Type<SNMesh&>::ConvertToString(*this) + "\" Param1=\"" + Type<File&>::ConvertToString(cFile) + "\" " + sParams);

										// Done
										return true;
									} else {
										Params<bool, SNMesh&, File&> cParams(*this, cFile);
										pLoaderImpl->CallMethod(sMethodName, cParams);

										// Done, return the result
										return cParams.Return;
									}
								}
							} else {
								PL_LOG(Error, "Can't load the loadable 'Skin' from '" + sFilename + "' because the file format is not supported!")
							}
						} else {
							PL_LOG(Error, "Can't open the file '" + sFilename + "' to load in the loadable 'Skin'!")
						}
					} else {
						PL_LOG(Error, "Can't load '" + sFilename + "' because the loadable type 'Skin' is unknown!")
					}

				// No skin file given, just replace all materials... 
				} else {
					// "MaterialManager().LoadResource()" takes care of the rest
					Material *pMaterial = GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(sFilename);
					if (pMaterial) {
						// Brute force: Set all materials within the mesh to the just loaded material
						// -> If we want to deal with multi-materials meshes, we really need to work with skin files within this method
						// -> In case you're setting the materials via code, you may not want to use this skin method in the first place
						for (uint32 i=0; i<pMesh->GetNumOfMaterials(); i++)
							m_pMeshHandler->SetMaterial(i, pMaterial);

						// Done
						return true;
					}
				}

			// No special skin given for this particular mesh instance within the scene
			} else {
				// Use the default materials of the used mesh
				for (uint32 i=0; i<pMesh->GetNumOfMaterials(); i++)
					m_pMeshHandler->SetMaterial(i, pMesh->GetMaterial(i));

				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Saves the skin
*/
bool SNMesh::SaveSkin(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Get the filename extension
	// -> There are file formats without an extension, so no extension must also be valid
	const String sExtension = Url(sFilename).GetExtension();

	// Supported skin file given?
	if (LoadableManager::GetInstance()->IsFormatSaveSupported(sExtension, "Skin")) {
		PL_LOG(Debug, "Save skin: " + sFilename)

		// Get loadable type
		LoadableType *pLoadableType = LoadableManager::GetInstance()->GetTypeByName("Skin");
		if (!pLoadableType) {
			PL_LOG(Error, "Can't save '" + sFilename + "' because the loadable type 'Skin' is unknown!")

			// Error!
			return false;
		}

		// Get loader, assigned to the current loadable type, capable of processing the file extension
		Loader *pLoader = pLoadableType->GetLoaderByExtension(sExtension);
		if (!pLoader) {
			PL_LOG(Error, "Can't save the loadable 'Skin' from '" + sFilename + "' because the file format is not supported!")

			// Error!
			return false;
		}
		if (!pLoader->GetClass().IsDerivedFrom("PLScene::SkinLoader")) {
			PL_LOG(Error, "Can't save the loadable 'Skin' from '" + sFilename + "' because the file format is no valid 'Skin' format!")

			// Error!
			return false;
		}

		// Try to open the file directly
		File cFile(sFilename);

		// Open the file itself
		if (cFile.Open(File::FileWrite | File::FileCreate)) {
			// Get method name
			static const String sSave = "Save";
			static const String sSaveParams = "SaveParams";
			String sMethodName = sMethod;
			if (!sMethodName.GetLength())
				sMethodName = sParams.GetLength() ? sSaveParams : sSave;

			// Get the loader implementation
			LoaderImpl *pLoaderImpl = pLoader->GetImpl();
			if (pLoaderImpl) {
				// Finally, load the skin!
				if (sParams.GetLength()) {
					pLoaderImpl->CallMethod(sMethodName, "Param0=\"" + Type<SNMesh&>::ConvertToString(*this) + "\" Param1=\"" + Type<File&>::ConvertToString(cFile) + "\" " + sParams);
					return true;
				} else {
					Params<bool, SNMesh&, File&> cParams(*this, cFile);
					pLoaderImpl->CallMethod(sMethodName, cParams);
					return cParams.Return;
				}
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Unloads the current mesh
*/
bool SNMesh::UnloadMesh()
{
	if (m_pMeshHandler) {
		if (m_pMeshHandler->GetMesh()) {
			// Call mesh de-initialization function
			MeshDeInitFunction();

			// Clear handler
			m_pMeshHandler->SetMesh();
		}

		// Destroy the mesh handler
		delete m_pMeshHandler;
		m_pMeshHandler = nullptr;
	}

	// Done
	return true;
}

/*
*  @brief
*    Returns the world position of a skeleton joint
*/
bool SNMesh::GetSkeletonJointWorldPosition(const String &sJointName, Vector3 &vPosition)
{
	// Get the mesh handler
	MeshHandler *pMeshHandler = GetMeshHandler();
	if (pMeshHandler) {
		// Is there a skeleton handler?
		SkeletonHandler *pSkeletonHandler = pMeshHandler->GetSkeletonHandler();
		if (pSkeletonHandler) {
			// Has this skeleton handler a skeleton resource?
			Skeleton *pSkeleton = pSkeletonHandler->GetResource();
			if (pSkeleton) {
				// Is there a skeleton joint with the given name?
				Joint *pJoint = pSkeleton->GetByName(sJointName);
				if (pJoint) {
					// Get skeleton handler
					Array<JointHandler> &lstJointHandlers = pSkeletonHandler->GetJointHandlers();
					uint32 nID = pJoint->GetID();
					JointHandler &cJointHandler = lstJointHandlers[nID];
					if (&cJointHandler != &Array<JointHandler>::Null) {
						// ... AND finally we can calculate the world position of the given joint
						vPosition = GetTransform().GetMatrix()*cJointHandler.GetTranslationAbsolute();

						// Done
						return true;
					}
				}
			}
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Protected virtual SNMesh functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    This function is called after the mesh handler loads a mesh
*/
void SNMesh::MeshInitFunction()
{
	// Setup scene node bounding box
	MeshHandler *pMeshHandler = GetMeshHandler();
	if (pMeshHandler && pMeshHandler->GetResource()) {
		// Get and set the mesh bounding box automatically?
		if (!(GetFlags() & NoAutomaticBoundingBox)) {
			AABoundingBox cAABB;
			pMeshHandler->GetResource()->GetBoundingBox(cAABB.vMin, cAABB.vMax);
			if (cAABB.vMin.IsNull() && cAABB.vMax.IsNull()) {
				// We need to calculate the bounding box of the mesh :(
				pMeshHandler->GetResource()->CalculateBoundingBox(cAABB.vMin, cAABB.vMax);

				// Set a valid bounding box of the mesh so we do not need to calculate it again...
				pMeshHandler->GetResource()->SetBoundingBox(cAABB.vMin, cAABB.vMax);
			}
			SetAABoundingBox(cAABB);
		}
	}
}

/**
*  @brief
*    This function is called after the mesh handler unloads a mesh
*/
void SNMesh::MeshDeInitFunction()
{
	// The default implementation is empty
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
MeshHandler *SNMesh::GetMeshHandler()
{
	// Do we need to try to load in the mesh right now?
	if (!m_pMeshHandler)
		LoadMesh(m_sMesh);

	// Return the mesh handler... if there's one
	return m_pMeshHandler;
}

void SNMesh::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawDebug(cRenderer, pVisNode);

	// Is there a mesh handler?
	if (pVisNode && m_pMeshHandler) {
		// Draw anything?
		const uint32 nDebugFlags = GetDebugFlags();
		if ((nDebugFlags & DebugShowWireframe) ||
			(nDebugFlags & DebugShowVertices) ||
			(nDebugFlags & DebugShowTangents) ||
			(nDebugFlags & DebugShowBinormals) ||
			(nDebugFlags & DebugShowNormals) ||
			(nDebugFlags & DebugShowVertexNumbers) ||
			(nDebugFlags & DebugShowJoints) ||
			(nDebugFlags & DebugShowJointNames) ||
			(nDebugFlags & DebugShowSkeleton) ||
			(nDebugFlags & DebugShowAnchorPoints)) {
			// Set render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Draw wireframe?
			if (nDebugFlags & DebugShowWireframe) {
				uint32 nFixedFillModeT = cRenderer.GetRenderState(RenderState::FixedFillMode);
				cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Line);
				m_pMeshHandler->Draw(false, false);
				cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeT);
			}

			// Draw vertices?
			if (nDebugFlags & DebugShowVertices)
				m_pMeshHandler->DrawVertices(Color4::White);

			// Get normal scale value
			static const float fNormalScale = 1.0f;

			// Draw tangents? (red = x/u)
			if (nDebugFlags & DebugShowTangents)
				m_pMeshHandler->DrawTangents(Color4::Red, pVisNode->GetWorldViewProjectionMatrix(), fNormalScale, 1.0f);

			// Draw binormals? (green = y/v)
			if (nDebugFlags & DebugShowBinormals)
				m_pMeshHandler->DrawBinormals(Color4::Green, pVisNode->GetWorldViewProjectionMatrix(), fNormalScale, 1.0f);

			// Draw normals? (blue = z/w)
			if (nDebugFlags & DebugShowNormals)
				m_pMeshHandler->DrawNormals(Color4::Blue, pVisNode->GetWorldViewProjectionMatrix(), fNormalScale, 1.0f);

			// Draw vertex numbers?
			if (nDebugFlags & DebugShowVertexNumbers) {
				Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
				if (pFont)
					m_pMeshHandler->DrawVertexNumbers(*pFont, Color4::White, pVisNode->GetWorldViewProjectionMatrix());
			}

			// Draw skeleton?
			if ((nDebugFlags & DebugShowJoints) || (nDebugFlags & DebugShowJointNames) || (nDebugFlags & DebugShowSkeleton)) {
				SkeletonHandler *pSkeletonHandler = m_pMeshHandler->GetSkeletonHandler();
				if (pSkeletonHandler) {
					Skeleton *pSkeleton = pSkeletonHandler->GetResource();
					if (pSkeleton) {
						// Draw joints?
						if (nDebugFlags & DebugShowJoints)
							pSkeletonHandler->DrawJoints(cRenderer, Color4::Green, pVisNode->GetWorldViewProjectionMatrix(), 4.0f);

						// Draw joint names?
						if (nDebugFlags & DebugShowJointNames) {
							Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
							if (pFont)
								pSkeletonHandler->DrawJointNames(*pFont, Color4::Green, pVisNode->GetWorldViewProjectionMatrix());
						}

						// Draw skeleton?
						if (nDebugFlags & DebugShowSkeleton)
							pSkeletonHandler->DrawSkeleton(cRenderer, Color4::Blue, pVisNode->GetWorldViewProjectionMatrix(), 1.0f);
					}
				}
			}

			// Draw anchor points?
			if (nDebugFlags & DebugShowAnchorPoints) {
				Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
				if (pFont)
					m_pMeshHandler->DrawAnchorPoints(*pFont, Color4::Green, pVisNode->GetWorldViewProjectionMatrix());
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNMesh::DeInitFunction()
{
	// Unload the used mesh... we do this in here while virtual functions still work correctly...
	UnloadMesh();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
