/*********************************************************\
 *  File: SCMirror.cpp                                   *
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
#include <PLCore/String/Tokenizer.h>
#include <PLMath/Rectangle.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Renderer/SurfaceTextureBuffer.h>
#include <PLRenderer/Effect/Effect.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLRenderer/Material/ParameterManager.h>
#include <PLMesh/Geometry.h>
#include <PLMesh/MeshManager.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLMesh/MeshMorphTarget.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodes/SCMirror.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SCMirror)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SCMirror::SCMirror() :
	Material(this),
	DynamicMap(this),
	Vertices(this),
	NearScale(this),
	Far(this),
	FPSLimit(this)
{
	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawSolid | UseDrawTransparent));

	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}

/**
*  @brief
*    Destructor
*/
SCMirror::~SCMirror()
{
	// Delete mesh handler
	if (m_pMeshHandler) {
		if (m_pMeshHandler->GetMesh()) {
			delete m_pMeshHandler->GetMesh();
			m_pMeshHandler->SetMesh();
		}
		delete m_pMeshHandler;
		m_pMeshHandler = nullptr;
	}
}

/**
*  @brief
*    Returns the mirror polygon
*/
Polygon &SCMirror::GetPolygon()
{
	return m_cPolygon;
}

/**
*  @brief
*    Updates the surrounding bounding box of the mirror
*/
void SCMirror::UpdateBoundingBox()
{
	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the virtual camera
*/
void SCMirror::UpdateVirtualCamera()
{
	// Get the current camera
	SNCamera *pCamera = SNCamera::GetCamera();
	if (pCamera) {
		// Get the 'virtual camera'
		SNCamera *pVirtualCamera = static_cast<SNCamera*>(GetByName(CameraName.Get()));
		if (pVirtualCamera) {
			pVirtualCamera->SetAutoUpdate(false);

			// Get the world transform matrix
			const Matrix3x4 mWorldTransform = pCamera->GetViewMatrix()*GetTransform().GetMatrix();

			// Get the center of the mirror in camera space
			const Vector3 vCenter = mWorldTransform.GetTranslation();

			// Get the normal of the mirror in camera space
			const Vector3 vNormal = mWorldTransform.GetZAxis().Normalize();

			// Calculate the distance of the camera to the mirror
			const float fDistance = -vNormal.DotProduct(vCenter);

			// Check whether the camera is in front of the mirror, if not, we don't
			// need to update the mirror texture
			if (fDistance > 0.0f) {
				// Get the up vector of the mirror in camera space
				Vector3 vUp = mWorldTransform.GetYAxis().Normalize();

				// Get the virtual camera position in camera space
				Vector3 vVirtualCameraPosition = vNormal;
				vVirtualCameraPosition *= -2.0f*fDistance;

				// Get the view matrix of the virtual camera
				Matrix3x4 &mVirtualView = pVirtualCamera->GetViewMatrix();
				mVirtualView.LookAt(vVirtualCameraPosition, Vector3::Zero, vUp);
				mVirtualView *= pCamera->GetViewMatrix();
				pVirtualCamera->GetTransform().SetPosition(GetTransform().GetMatrix()*vVirtualCameraPosition);

				// Get the projection matrix of the virtual camera
				Vector3    vOc      = vCenter - vVirtualCameraPosition;
				Vector3    vLeft    = vUp.CrossProduct(vNormal);
				float      fXOffset = -vOc.DotProduct(vLeft);
				float	   fYOffset = vOc.DotProduct(vUp);
				float	   fXMin	= fXOffset - 0.5f*m_fWidth;
				float	   fXMax	= fXMin + m_fWidth;
				float	   fYMin	= fYOffset - 0.5f*m_fHeight;
				float	   fYMax	= fYMin + m_fHeight;
				Matrix4x4 &mVirtualProjection = pVirtualCamera->GetProjectionMatrix(Rectangle());
				mVirtualProjection.PerspectiveOffCenter(fXMin, fXMax, fYMin, fYMax, fDistance*NearScale, Far);

				// Concatenate (multiply) the view matrix and the projection matrix
				Matrix4x4 mViewProjection = mVirtualProjection;
				mViewProjection *= mVirtualView;

				// Update the frustum of the virtual camera
				pVirtualCamera->GetFrustum(GetSceneContext()->GetRendererContext().GetRenderer().GetViewport()).CreateViewPlanes(mViewProjection, true);
			}
		}
	}
}

/**
*  @brief
*    Sets the dynamic mirror texture
*/
void SCMirror::SetDynamicTexture()
{
	// Get the material
	if (m_pMeshHandler) {
		PLRenderer::Material *pMaterial = m_pMeshHandler->GetMaterial(0);
		if (pMaterial) {
			// Set current dynamic texture parameter
			if (pMaterial && DynamicMap.Get().GetLength()) {
				// Is there a material texture parameter with the given name?
				const Parameter *pParameter = pMaterial->GetParameterManager().GetParameter(DynamicMap.Get());
				if (pParameter) {
					if (pParameter->GetType() == PLRenderer::Parameters::TextureBuffer) {
						// Jipi, we can set the texture directly!
						pMaterial->GetParameterManager().SetParameterTextureBuffer(DynamicMap.Get(), static_cast<TextureBuffer*>(m_pSurfaceTextureBuffer->GetTextureBuffer()));
					} else {
						PL_LOG(Error, "Parameter '" + DynamicMap.Get() + "' found within material '" + Material.Get() + "', but it's no texture parameter!")
					}
				} else {
					// Has the effect a texture parameter with this name?
					const Effect *pFX = pMaterial->GetEffect();
					if (pFX) {
						// Is there an effect texture parameter with the given name?
						pParameter = pFX->GetParameterManager().GetParameter(DynamicMap.Get());
						if (pParameter) {
							if (pParameter->GetType() == PLRenderer::Parameters::TextureBuffer) {
								// Add a new parameter to the material
								if (pMaterial->GetParameterManager().CreateParameter(PLRenderer::Parameters::TextureBuffer, DynamicMap.Get())) {
									// Set the texture parameter
									pMaterial->GetParameterManager().SetParameterTextureBuffer(DynamicMap.Get(), static_cast<TextureBuffer*>(m_pSurfaceTextureBuffer->GetTextureBuffer()));
								} else {
									PL_LOG(Error, "Can't add new parameter '" + DynamicMap.Get() + "' to the material '" + Material.Get() + "'!")
								}
							} else {
								PL_LOG(Error, "Parameter '" + DynamicMap.Get() + "' found within effect '" + pFX->GetName() + "', but it's no texture parameter!")
							}
						} else {
							PL_LOG(Error, "Unknown material/effect parameter '" + DynamicMap.Get() + "'!")
						}
					} else {
						PL_LOG(Error, "Unknown material parameter '" + DynamicMap.Get() + "' and the material has no effect assigned to it!")
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
MeshHandler *SCMirror::GetMeshHandler()
{
	// [TODO] Check me
//	return m_pMeshHandler;
	return nullptr;
}

void SCMirror::DrawPre(Renderer &cRenderer, const VisNode *pVisNode)
{
	if (IsActive()) {
		// FIRST: Update the virtual camera
		UpdateVirtualCamera();

		// Call base implementation
		SCRenderToTexture::DrawPre(cRenderer, pVisNode);
	}
}

void SCMirror::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SCRenderToTexture::DrawSolid(cRenderer, pVisNode);

	// Draw the mirror
	if (m_pMeshHandler) {
		// Set the dynamic mirror texture
		SetDynamicTexture();

		// Set world matrix
		if (pVisNode) {
			FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
			if (pFixedFunctions)
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, pVisNode->GetWorldMatrix());
		}

		// Draw
		m_pMeshHandler->Draw(false);
	}
}

void SCMirror::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Draw the mirror
	if (m_pMeshHandler) {
		// Set the dynamic mirror texture
		SetDynamicTexture();

		// Set world matrix
		if (pVisNode) {
			FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
			if (pFixedFunctions)
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, pVisNode->GetWorldMatrix());
		}

		// Draw
		m_pMeshHandler->Draw(true);
	}

	// Call base implementation
	SCRenderToTexture::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Private virtual SceneNode functions                   ]
//[-------------------------------------------------------]
void SCMirror::InitFunction()
{
	// Call base implementation
	SCRenderToTexture::InitFunction();

	float fMin[2] = {0.0f, 0.0f}, fMax[2] = {0.0f, 0.0f};
	{ // Create the mirror polygon
		Tokenizer cTokenizer;
		cTokenizer.Start(Vertices.Get());
		Vector3 vV;
		bool bFirst = true;
		bool bRead = true;
		while (bRead) {
			for (int j=0; j<2; j++) {
				// Set value
				if (cTokenizer.ParseNumber(vV[j])) {
					// Update min/max values
					if (bFirst) {
						fMin[j] = fMax[j] = vV[j];
					} else {
						if (fMin[j] > vV[j])
							fMin[j] = vV[j];
						if (fMax[j] < vV[j])
							fMax[j] = vV[j];
					}
				} else {
					// Last value
					bRead = false;
					break;
				}
			}
			if (bRead)
				m_cPolygon.GetVertexList().Add(vV);
			if (bFirst)
				bFirst = false;
		}

		cTokenizer.Stop();
		m_cPolygon.ComputePlane();
	}

	// Set the width and height of the mirror
	m_fWidth  = fMax[Vector2::X]-fMin[Vector2::X];
	m_fHeight = fMax[Vector2::Y]-fMin[Vector2::Y];

	// Initialize mesh handler and setup mesh
	m_pMeshHandler = new MeshHandler();
	Mesh *pMesh = GetSceneContext()->GetMeshManager().Create(GetAbsoluteName() + "_Mesh");

	// Add morph target
	MeshMorphTarget *pMorphTarget = pMesh->AddMorphTarget();
	if (pMorphTarget) {
		// Add LOD level
		MeshLODLevel *pLODLevel = pMesh->AddLODLevel();
		if (pLODLevel) {
			pLODLevel->CreateGeometries();
			pLODLevel->CreateIndexBuffer();

			// Allocate vertex buffer
			VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
			if (pVertexBuffer && pLODLevel->GetIndexBuffer()) {
				pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
				pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
				pVertexBuffer->Allocate(m_cPolygon.GetVertexList().GetNumOfElements());

				// Allocate index buffer
				IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
				pIndexBuffer->Allocate(pVertexBuffer->GetNumOfElements());

				// Setup vertices & geometry
				if (pVertexBuffer->Lock(Lock::WriteOnly)) {
					if (pIndexBuffer->Lock(Lock::WriteOnly)) {
						Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();

						// Setup indices and vertices
						for (uint32 i=0; i<pIndexBuffer->GetNumOfElements(); i++) {
							const Vector3 &vPos = m_cPolygon.GetVertexList().Get(i);
							float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::Position));
							pfVertex[Vector3::X] = vPos.x;
							pfVertex[Vector3::Y] = vPos.y;
							pfVertex[Vector3::Z] = vPos.z;
							pIndexBuffer->SetData(i, i);

							// [TODO] Find another, more flexible way to setup the texture coordinates!!
							// (the 'mirror polygon' can have more that 4 vertices and the order of them
							// isn't fixed!)
							pfVertex = static_cast<float*>(pVertexBuffer->GetData(i, VertexBuffer::TexCoord));
							switch (i) {
								case 0:
									pfVertex[Vector2::X] = 0.0f;
									pfVertex[Vector2::Y] = 0.0f;
									break;

								case 1:
									pfVertex[Vector2::X] = 1.0f;
									pfVertex[Vector2::Y] = 0.0f;
									break;

								case 2:
									pfVertex[Vector2::X] = 1.0f;
									pfVertex[Vector2::Y] = 1.0f;
									break;

								case 3:
									pfVertex[Vector2::X] = 0.0f;
									pfVertex[Vector2::Y] = 1.0f;
									break;
							}
						}
						if (pIndexBuffer->GetNumOfElements() >= 4) {
							pIndexBuffer->SetData(0,								  pIndexBuffer->GetNumOfElements()-3);
							pIndexBuffer->SetData(pIndexBuffer->GetNumOfElements()-3, 0);
						}

						// Create and setup new geometry
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(0);
						cGeometry.SetMaterial(0);
						cGeometry.SetIndexSize(pIndexBuffer->GetNumOfElements());

						// Unlock the index buffer
						pIndexBuffer->Unlock();
					}

					// Unlock the vertex buffer
					pVertexBuffer->Unlock();
				}

				// Calculate the normals
				pMesh->CalculateNormals();
			}
		}
	}

	// Set material
	pMesh->AddMaterial(GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(Material.Get()));
	m_pMeshHandler->SetMesh(pMesh);

	// Create a virtual camera if required
	if (!GetByName(CameraName.Get()) || !GetByName(CameraName.Get())->IsCamera()) {
		CameraName.Set("Virtual camera");
		SceneNode *pCameraNode = Create("PLScene::SNCamera", CameraName.Get());

		// Get the resulting scene node name of the camera - just for sure :)
		if (pCameraNode)
			CameraName.Set(pCameraNode->GetName());
	}
}

void SCMirror::UpdateAABoundingBox()
{
	// Init min/max
	const Array<Vector3> &cVertexList = m_cPolygon.GetVertexList();
	if (cVertexList.GetNumOfElements()) {
		// Get min/max
		Vector3 vMin = cVertexList[0], vMax = vMin;
		for (uint32 i=1; i<cVertexList.GetNumOfElements(); i++) {
			const Vector3 &vPos = cVertexList[i];
			for (int j=0; j<3; j++) {
				if (vMin[j] > vPos[j])
					vMin[j] = vPos[j];
				if (vMax[j] < vPos[j])
					vMax[j] = vPos[j];
			}
		}

		// Set the bounding box of the scene node
		SetAABoundingBox(AABoundingBox(vMin, vMax));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
