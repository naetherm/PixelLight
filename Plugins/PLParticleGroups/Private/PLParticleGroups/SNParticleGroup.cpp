/*********************************************************\
 *  File: SNParticleGroup.cpp                            *
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
#include <PLCore/File/Url.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLParticleGroups/SNParticleGroup.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNParticleGroup)


//[-------------------------------------------------------]
//[ SNParticleGroup::Particle                             ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
SNParticleGroup::Particle::Particle() :
	bActive(true),
	fEnergy(0.0f),
	fSize(0.0f),
	fRot(0.0f),
	nAnimationStep(0),
	fAnimationTimer(0.0f),
	nCustom1(0),
	nCustom2(0),
	fCustom1(0.0f),
	fCustom2(0.0f),
	pRot(nullptr),
	bDistorted(false)
{
}

SNParticleGroup::Particle::~Particle()
{
	if (pRot)
		delete pRot;
}

bool SNParticleGroup::Particle::operator ==(const Particle &cOther) const
{
	// We do not need an implementation in here...
	return false;
}




//[-------------------------------------------------------]
//[ SNParticleGroup                                       ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNParticleGroup::GetMaterial() const
{
	return m_sMaterial;
}

void SNParticleGroup::SetMaterial(const String &sValue)
{
	if (m_sMaterial != sValue) {
		m_sMaterial = sValue;
		if (IsInitialized()) {
			DeInitFunction();
			InitFunction();
		}
	}
}

uint32 SNParticleGroup::GetParticles() const
{
	return m_nParticles;
}

void SNParticleGroup::SetParticles(const uint32 &nValue)
{
	if (m_nParticles != nValue) {
		m_nParticles = nValue;
		if (IsInitialized()) {
			DeInitFunction();
			InitFunction();
		}
	}
}

uint32 SNParticleGroup::GetTextureAnimationColumns() const
{
	return m_nTextureAnimationColumns;
}

void SNParticleGroup::SetTextureAnimationColumns(const uint32 &nValue)
{
	if (m_nTextureAnimationColumns != nValue) {
		m_nTextureAnimationColumns = nValue;
		if (IsInitialized()) {
			DeInitFunction();
			InitFunction();
		}
	}
}

uint32 SNParticleGroup::GetTextureAnimationRows() const
{
	return m_nTextureAnimationRows;
}

void SNParticleGroup::SetTextureAnimationRows(const uint32 &nValue)
{
	if (m_nTextureAnimationRows != nValue) {
		m_nTextureAnimationRows = nValue;
		if (IsInitialized()) {
			DeInitFunction();
			InitFunction();
		}
	}
}

void SNParticleGroup::SetFlags(uint32 nValue)
{
	if (GetFlags() != nValue) {
		// Call base implementation
		SceneNode::SetFlags(nValue);

		if (IsInitialized()) {
			DeInitFunction();
			InitFunction();
		}
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initializes the particle group
*/
bool SNParticleGroup::InitParticleGroup(uint32 nMaxNumOfParticles, const String &sMaterial, const void *pData)
{
	// Initialize the particles
	return InitParticles(nMaxNumOfParticles, sMaterial);
}

/**
*  @brief
*    Returns the maximum possible number of particles
*/
uint32 SNParticleGroup::GetMaxNumOfParticles() const
{
	return m_nMaxNumOfParticles;
}

/**
*  @brief
*    Returns the current number of particles
*/
uint32 SNParticleGroup::GetNumOfParticles() const
{
	return m_lstParticles.GetNumOfElements();
}

/**
*  @brief
*    Adds a new particle
*/
SNParticleGroup::Particle *SNParticleGroup::AddParticle()
{
	// Is the creation of new particles allowed?
	if (m_bCreateParticles) {
		// Particle limitation reached?
		if (m_lstParticles.GetNumOfElements() < m_nMaxNumOfParticles) {
			// Add/get particle
			Particle &cParticle = m_lstParticles.Add();
			return (&cParticle != &FastPool<Particle>::Null) ? &cParticle : nullptr;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Removes a particle
*/
void SNParticleGroup::RemoveParticle(Particle &cParticle)
{
	m_lstParticles.RemoveElement(cParticle);
}

/**
*  @brief
*    Returns a particle iterator
*/
Iterator<SNParticleGroup::Particle> SNParticleGroup::GetParticleIterator() const
{
	return m_lstParticles.GetIterator();
}

/**
*  @brief
*    Returns whether the particle group can create new particles or not
*/
bool SNParticleGroup::GetCreateParticles() const
{
	return m_bCreateParticles;
}

/**
*  @brief
*    Sets whether the particle group can create new particles or not
*/
void SNParticleGroup::SetCreateParticles(bool bCreate)
{
	m_bCreateParticles = bCreate;
}

/**
*  @brief
*    Configures the texture animation
*/
void SNParticleGroup::SetupTextureAnimation(uint32 nColumns, uint32 nRows)
{
	ParticleTexCoord sTexCoord;
	if (nColumns < 1)
		nColumns = 1;
	if (nRows < 1)
		nRows = 1;
	m_lstTexCoord.Clear();
	if (nColumns <= 1 && nRows <= 1) {
		sTexCoord.vTexCoord[0].SetXY(1.0f, 1.0f);
		sTexCoord.vTexCoord[1].SetXY(1.0f, 0.0f);
		sTexCoord.vTexCoord[2].SetXY(0.0f, 0.0f);
		sTexCoord.vTexCoord[3].SetXY(0.0f, 1.0f);
		m_lstTexCoord.Add(sTexCoord);
	} else {
		m_nTextureAnimationColumns = nColumns;
		m_nTextureAnimationRows    = nRows;

		// Create the list of texture coordinates
		const float fWidth  = 1.0f/static_cast<float>(nColumns);
		const float fHeight = 1.0f/static_cast<float>(nRows);
		float fY = 1.0f;
		for (uint32 nRow=0; nRow<nRows; nRow++) {
			float fX = 0.0f;
			for (uint32 nColumn=0; nColumn<nColumns; nColumn++) {
				// Setup texture coordinates
				sTexCoord.vTexCoord[0].SetXY(fX+fWidth, fY);
				sTexCoord.vTexCoord[1].SetXY(fX+fWidth, fY-fHeight);
				sTexCoord.vTexCoord[2].SetXY(fX,        fY-fHeight);
				sTexCoord.vTexCoord[3].SetXY(fX,        fY);
				m_lstTexCoord.Add(sTexCoord);
				fX += fWidth;
			}
			fY -= fHeight;
		}
	}
}

/**
*  @brief
*    Returns the number of texture animation steps
*/
uint32 SNParticleGroup::GetTextureAnimationSteps() const
{
	return m_lstTexCoord.GetNumOfElements();
}

/**
*  @brief
*    Returns whether the particle group should be removed automatically after all particles are inactive or not
*/
bool SNParticleGroup::GetRemoveAutomatically() const
{
	return m_bRemoveAutomatically;
}

/**
*  @brief
*    Should the particle group be removed automatically after all particles are inactive?
*/
void SNParticleGroup::RemoveAutomatically(bool bRemoveAutomatically)
{
	m_bRemoveAutomatically = bRemoveAutomatically;
}

/**
*  @brief
*    Returns the handler of the particle group material
*/
const MaterialHandler &SNParticleGroup::GetMaterialHandler()
{
	return m_cMaterial;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNParticleGroup::SNParticleGroup() :
	Material(this),
	GlobalColor(this),
	Particles(this),
	TextureAnimationColumns(this),
	TextureAnimationRows(this),
	Flags(this),
	m_sMaterial("Data/Effects/DefaultParticle.plfx"),
	m_nParticles(10),
	m_nTextureAnimationColumns(1),
	m_nTextureAnimationRows(1),
	m_pIndexBuffer(nullptr),
	m_pVertexBuffer(nullptr),
	m_nUsedIndices(0),
	m_nUsedVertices(0),
	m_bRemoveAutomatically(false),
	m_nMaxNumOfParticles(0),
	m_bCreateParticles(true)
{
	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawSolid | UseDrawTransparent));
}

/**
*  @brief
*    Destructor
*/
SNParticleGroup::~SNParticleGroup()
{
}

/**
*  @brief
*    Initializes the particles
*/
bool SNParticleGroup::InitParticles(uint32 nMaxNumOfParticles, const String &sMaterial)
{
	// Delete the old particles
	m_lstParticles.Clear();
	m_nMaxNumOfParticles = 0;

	// Check parameter
	if (!nMaxNumOfParticles)
		return false; // Error!
	m_nMaxNumOfParticles = nMaxNumOfParticles;

	// Get the renderer
	Renderer &cRenderer = GetSceneContext()->GetRendererContext().GetRenderer();

	if (GetFlags() & PointSprites) {
		// Point sprites will need no index buffer!
		if (m_pIndexBuffer) {
			delete m_pIndexBuffer;
			m_pIndexBuffer = nullptr;
		}

		// Setup the vertex buffer
		if (m_pVertexBuffer)
			delete m_pVertexBuffer;
		m_pVertexBuffer = cRenderer.CreateVertexBuffer();
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (!(GetFlags() & UseGlobalColor))
			m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Color, 0, VertexBuffer::RGBA);
		// Even if the vertex buffer is used 'write only', the managed version was more performant
		// within my tests...
		m_pVertexBuffer->Allocate(m_nMaxNumOfParticles, Usage::WriteOnly);
	} else {
		// Setup the index buffer
		if (m_pIndexBuffer)
			delete m_pIndexBuffer;
		m_pIndexBuffer = cRenderer.CreateIndexBuffer();
		m_pIndexBuffer->Allocate(m_nMaxNumOfParticles*6, Usage::Static, false);
		if (m_pIndexBuffer->Lock(Lock::WriteOnly)) {
			for (uint32 i=0, nVertices=4, nIndex=0; i<m_nMaxNumOfParticles; i++, nVertices+=4) {
				// Triangle 1
				m_pIndexBuffer->SetData(nIndex++, nVertices-4);
				m_pIndexBuffer->SetData(nIndex++, nVertices-3);
				m_pIndexBuffer->SetData(nIndex++, nVertices-2);

				// Triangle 2
				m_pIndexBuffer->SetData(nIndex++, nVertices-4);
				m_pIndexBuffer->SetData(nIndex++, nVertices-2);
				m_pIndexBuffer->SetData(nIndex++, nVertices-1);
			}

			// Unlock the index buffer
			m_pIndexBuffer->Unlock();
		}

		// Setup the vertex buffer
		if (m_pVertexBuffer)
			delete m_pVertexBuffer;
		m_pVertexBuffer = cRenderer.CreateVertexBuffer();
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
		m_pVertexBuffer->AddVertexAttribute(VertexBuffer::Color,    0, VertexBuffer::RGBA);
		// Even if the vertex buffer is used 'write only', the managed version was more performant
		// within my tests...
		m_pVertexBuffer->Allocate(m_nMaxNumOfParticles*4, Usage::WriteOnly);
	}

	{ // Setup the particle material
		// Load material
		MaterialManager &cMaterialManager = cRenderer.GetRendererContext().GetMaterialManager();
		PLRenderer::Material *pMaterial = sMaterial.GetLength() ? cMaterialManager.LoadResource(sMaterial) : cMaterialManager.LoadResource(m_sMaterial);
		m_cMaterial.SetResource(pMaterial);

		// Check whether this is a material or an effect, if not, we need to set a nice default particle effect
		if (pMaterial) {
			String sExtension = Url(pMaterial->GetName()).GetExtension();
			if (sExtension != "mat" && sExtension != "plfx") {
				// Set the particle default effect
				PLRenderer::Material *pDefaultMaterial = cMaterialManager.LoadResource(Material.GetDefault());
				if (pDefaultMaterial)
					pMaterial->SetEffect(pDefaultMaterial->GetEffect());
			}
		}
	}

	// Precompute texture animation
	SetupTextureAnimation(m_nTextureAnimationColumns, m_nTextureAnimationRows);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create the particle shapes
*/
void SNParticleGroup::CreateShapes()
{
	// Lock the vertex buffer
	if (m_pVertexBuffer->Lock(Lock::WriteOnly)) {
		// Vertex buffer data
		const uint32 nVertexSize = m_pVertexBuffer->GetVertexSize();
		float *pfPosition = static_cast<float*>(m_pVertexBuffer->GetData(0, VertexBuffer::Position));

		// Particle data
		if (GetFlags() & PointSprites) {
			m_nUsedVertices = 0;
			if (GetFlags() & UseGlobalColor) {
				Iterator<Particle> cIterator = m_lstParticles.GetIterator();
				while (cIterator.HasNext()) {
					const Particle &cParticle = cIterator.Next();
					if (cParticle.bActive) {
						pfPosition[0] = cParticle.vPos.x;
						pfPosition[1] = cParticle.vPos.y;
						pfPosition[2] = cParticle.vPos.z;
						pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);
						m_nUsedVertices++;
					}
				}
			} else {
				Iterator<Particle> cIterator = m_lstParticles.GetIterator();
				while (cIterator.HasNext()) {
					const Particle &cParticle = cIterator.Next();
					if (cParticle.bActive) {
						pfPosition[0] = cParticle.vPos.x;
						pfPosition[1] = cParticle.vPos.y;
						pfPosition[2] = cParticle.vPos.z;
						pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);
						m_pVertexBuffer->SetColor(m_nUsedVertices, cParticle.vColor);
						m_nUsedVertices++;
					}
				}
			}
		} else {
			// Get billboard information
			SNCamera *pCamera = SNCamera::GetCamera();
			Vector3 vCamPos;
			if (pCamera)
				vCamPos = pCamera->GetTransform().GetPosition();
			Vector3 vTopLeft, vTopRight, vBottomLeft, vBottomRight;

			FixedFunctions *pFixedFunctions = m_pVertexBuffer->GetRenderer().GetFixedFunctions();
			const Matrix4x4 &mView = pFixedFunctions ? pFixedFunctions->GetTransformState(FixedFunctions::Transform::View) : Matrix4x4::Identity;
			Vector3 vX(mView.fM[0], mView.fM[4], mView.fM[8]);
			Vector3 vY(mView.fM[1], mView.fM[5], mView.fM[9]);
			Vector3 vTopLeftT  = vX-vY;
			Vector3 vTopRightT = vX+vY;
			vX.Invert();
			Vector3 vBottomLeftT  = vX-vY;
			Vector3 vBottomRightT = vX+vY;
			vX.Invert();

			// Create the particle shapes
			float *pfTexCoord = static_cast<float*>(m_pVertexBuffer->GetData(0, VertexBuffer::TexCoord));
			Vector3 vRight, vUp, vXT, vYT;

			m_nUsedIndices  = 0;
			m_nUsedVertices = 0;
			Iterator<Particle> cIterator = m_lstParticles.GetIterator();
			while (cIterator.HasNext()) {
				const Particle &cParticle = cIterator.Next();
				if (cParticle.bActive) {
					const Vector3 &vPos = cParticle.vPos;

					if (cParticle.pRot) { // Own rotation
						vUp    = cParticle.pRot->GetYAxis();
						vRight = cParticle.pRot->GetXAxis();
						if (cParticle.fRot)
							vRight.RotateAxis(vUp, cParticle.fRot);
						if (cParticle.bDistorted) {
							vRight *= cParticle.vDistortion;
							vUp    *= cParticle.vDistortion;
						}
						if (cParticle.fSize != 1.0f) {
							vRight *= cParticle.fSize;
							vUp    *= cParticle.fSize;
						}

						vTopLeft     = vPos - vRight - vUp;
						vBottomLeft  = vPos - vRight + vUp;
						vTopRight    = vPos + vRight - vUp;
						vBottomRight = vPos + vRight + vUp;

					} else if (cParticle.bDistorted) { // Distorted particle
						Vector3 vEndPos = cParticle.vPos + cParticle.vDistortion*cParticle.fSize;
						Vector3 vV      = ((vCamPos - (vPos + vEndPos)*0.5f).GetNormalized()).CrossProduct(cParticle.vDistortion.GetNormalized());

						// Calculate shape
						vTopLeft     =    vPos - vV*cParticle.fSize;
						vBottomLeft  =    vPos + vV*cParticle.fSize;
						vTopRight	 = vEndPos - vV*cParticle.fSize;
						vBottomRight = vEndPos + vV*cParticle.fSize;

					} else if (cParticle.fRot) { // Rotated particle
						vXT = vX;
						vYT = vY;
						vXT.RotateAxis(vYT, -cParticle.fRot);
						vTopLeft  = vXT - vYT;
						vTopRight = vXT + vYT;
						vXT.Invert();
						vBottomLeft  = vXT - vYT;
						vBottomRight = vXT + vYT;

						// Compute particle position
						if (cParticle.fSize != 1.0f) {
							vTopLeft     *= cParticle.fSize;
							vBottomLeft  *= cParticle.fSize;
							vTopRight    *= cParticle.fSize;
							vBottomRight *= cParticle.fSize;
						}

						// Add particle position
						vTopLeft     += vPos;
						vBottomLeft  += vPos;
						vTopRight    += vPos;
						vBottomRight += vPos;

					} else { // Quite standard particle
						// Compute particle position
						if (cParticle.fSize != 1.0f) {
							vTopLeft     =     vTopLeftT*cParticle.fSize;
							vBottomLeft  =  vBottomLeftT*cParticle.fSize;
							vTopRight    =    vTopRightT*cParticle.fSize;
							vBottomRight = vBottomRightT*cParticle.fSize;
						} else {
							vTopLeft     =     vTopLeftT;
							vBottomLeft  =  vBottomLeftT;
							vTopRight    =    vTopRightT;
							vBottomRight = vBottomRightT;
						}

						// Add particle position
						vTopLeft     += vPos;
						vBottomLeft  += vPos;
						vTopRight    += vPos;
						vBottomRight += vPos;
					}

					// Get texture coordinates
					const Vector2 *pvParticleTexCoord = m_lstTexCoord[cParticle.nAnimationStep].vTexCoord;
					if (!pvParticleTexCoord)
						pvParticleTexCoord = m_lstTexCoord[0].vTexCoord;
					if (pvParticleTexCoord) {
						// Top left
						pfPosition[0] = vTopLeft.x;
						pfPosition[1] = vTopLeft.y;
						pfPosition[2] = vTopLeft.z;
						pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);
						pfTexCoord[0] = pvParticleTexCoord[0].x;
						pfTexCoord[1] = pvParticleTexCoord[0].y;
						pfTexCoord = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTexCoord)+nVertexSize);
						m_pVertexBuffer->SetColor(m_nUsedVertices, cParticle.vColor);
						m_nUsedVertices++;

						// Top right
						pfPosition[0] = vTopRight.x;
						pfPosition[1] = vTopRight.y;
						pfPosition[2] = vTopRight.z;
						pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);
						pfTexCoord[0] = pvParticleTexCoord[1].x;
						pfTexCoord[1] = pvParticleTexCoord[1].y;
						pfTexCoord = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTexCoord)+nVertexSize);
						m_pVertexBuffer->SetColor(m_nUsedVertices, cParticle.vColor);
						m_nUsedVertices++;

						// Bottom right
						pfPosition[0] = vBottomRight.x;
						pfPosition[1] = vBottomRight.y;
						pfPosition[2] = vBottomRight.z;
						pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);
						pfTexCoord[0] = pvParticleTexCoord[2].x;
						pfTexCoord[1] = pvParticleTexCoord[2].y;
						pfTexCoord = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTexCoord)+nVertexSize);
						m_pVertexBuffer->SetColor(m_nUsedVertices, cParticle.vColor);
						m_nUsedVertices++;

						// Bottom left
						pfPosition[0] = vBottomLeft.x;
						pfPosition[1] = vBottomLeft.y;
						pfPosition[2] = vBottomLeft.z;
						pfPosition = reinterpret_cast<float*>(reinterpret_cast<char*>(pfPosition)+nVertexSize);
						pfTexCoord[0] = pvParticleTexCoord[3].x;
						pfTexCoord[1] = pvParticleTexCoord[3].y;
						pfTexCoord = reinterpret_cast<float*>(reinterpret_cast<char*>(pfTexCoord)+nVertexSize);
						m_pVertexBuffer->SetColor(m_nUsedVertices, cParticle.vColor);
						m_nUsedVertices++;

						// Setup indices
						m_nUsedIndices += 6;
					}
				}
			}
		}

		// Unlock the vertex buffer
		m_pVertexBuffer->Unlock();
	}
}

/**
*  @brief
*    Draws the particle shapes
*/
void SNParticleGroup::DrawShapes(Renderer &cRenderer)
{
	// Get material
	PLRenderer::Material *pMaterial = m_cMaterial.GetResource();
	if (pMaterial) {
		// Create shapes - we have to recreate them each time because they are camera dependent
		CreateShapes();

		// Bind buffers
		if (!(GetFlags() & PointSprites))
			cRenderer.SetIndexBuffer(m_pIndexBuffer);
		// [TODO] Remove FixedFunctions usage by using the new shader interface
		FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
		if (pFixedFunctions)
			pFixedFunctions->SetVertexBuffer(m_pVertexBuffer);

		// Draw primitives
		if (GetFlags() & PointSprites) {
			if (GetFlags() & UseGlobalColor) {
				pFixedFunctions = cRenderer.GetFixedFunctions();
				if (pFixedFunctions)
					pFixedFunctions->SetColor(GlobalColor.Get());
			}

			// Draw point sprites
			for (uint32 i=0; i<pMaterial->GetNumOfPasses(); i++) {
				pMaterial->SetupPass(i);
				cRenderer.DrawPrimitives(Primitive::PointList, 0, m_nUsedVertices);
			}
		} else {
			for (uint32 i=0; i<pMaterial->GetNumOfPasses(); i++) {
				pMaterial->SetupPass(i);
				cRenderer.DrawIndexedPrimitives(Primitive::TriangleList, 0, m_nUsedVertices-1, 0, m_nUsedIndices);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNParticleGroup::DrawSolid(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawSolid(cRenderer, pVisNode);

	// Check index and vertex buffer
	if (((GetFlags() & PointSprites) || m_pIndexBuffer) && m_pVertexBuffer) {
		// Check if the particle group material is transparent or not
		PLRenderer::Material *pMaterial = m_cMaterial.GetResource();
		if (pMaterial && !pMaterial->GetBlend()) {
			// Are the particles within the local scene node space?
			if (!(GetFlags() & SceneNodeSpaceParticles)) {
				// Each particle is in 'world' space
				FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
				if (pFixedFunctions)
					pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);
			} else {
				// Set world matrix
				if (pVisNode) {
					FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
					if (pFixedFunctions)
						pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, pVisNode->GetWorldMatrix());
				}
			}

			// Draw the particle shapes
			DrawShapes(cRenderer);
		}
	}
}

void SNParticleGroup::DrawTransparent(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Check index and vertex buffer
	if (((GetFlags() & PointSprites) || m_pIndexBuffer) && m_pVertexBuffer) {
		// Check if the particle group material is transparent or not
		PLRenderer::Material *pMaterial = m_cMaterial.GetResource();
		if (pMaterial && pMaterial->GetBlend()) {
			// Are the particles within the local scene node space?
			if (!(GetFlags() & SceneNodeSpaceParticles)) {
				// Each particle is in 'world' space
				FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
				if (pFixedFunctions)
					pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);
			} else {
				// Set world matrix
				if (pVisNode) {
					FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
					if (pFixedFunctions)
						pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, pVisNode->GetWorldMatrix());
				}
			}

			// Draw the particle shapes
			DrawShapes(cRenderer);
		}
	}

	// Call base implementation
	SceneNode::DrawTransparent(cRenderer, pVisNode);
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNParticleGroup::InitFunction()
{
	// Call base implementation
	SceneNode::InitFunction();

	// Initialize particle group
	InitParticleGroup(m_nParticles, m_sMaterial);
}

void SNParticleGroup::DeInitFunction()
{
	// Destroy particles
	m_lstParticles.Clear();
	m_nMaxNumOfParticles = 0;

	// Delete the index buffer
	if (m_pIndexBuffer) {
		delete m_pIndexBuffer;
		m_pIndexBuffer = nullptr;
	}

	// Delete the vertex buffer
	if (m_pVertexBuffer) {
		delete m_pVertexBuffer;
		m_pVertexBuffer = nullptr;
	}

	// Clear texture coordinate list
	m_lstTexCoord.Clear();

	// Call base implementation
	SceneNode::DeInitFunction();
}

void SNParticleGroup::UpdateAABoundingBox()
{
	float fBoundingBox[2][3] = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
	bool bActiveParticles = false;	// Is there at least one active particle?

	{ // Get the bounding box
		// Initialize bounding box by using the position of the first found active particle
		Iterator<Particle> cIterator = m_lstParticles.GetIterator();
		while (cIterator.HasNext()) {
			const Particle &cParticle = cIterator.Next();
			if (cParticle.bActive) {
				bActiveParticles = true;
				for (int i=0; i<3; i++) {
					fBoundingBox[0][i] = cParticle.vPos[i];
					fBoundingBox[1][i] = cParticle.vPos[i];
				}
				break;
			}
		}

		// Get correct bounding box size
		while (cIterator.HasNext()) {
			const Particle &cParticle = cIterator.Next();
			if (cParticle.bActive) {
				if (cParticle.bDistorted) {
					Vector3 vT = cParticle.vPos-cParticle.vDistortion*cParticle.fSize;
					for (int i=0; i<3; i++) {
						if (vT[i] < fBoundingBox[0][i])
							fBoundingBox[0][i] = vT[i];
						if (vT[i] > fBoundingBox[1][i])
							fBoundingBox[1][i] = vT[i];
					}
					vT = cParticle.vPos+cParticle.vDistortion*cParticle.fSize;
					for (int i=0; i<3; i++) {
						if (vT[i] < fBoundingBox[0][i])
							fBoundingBox[0][i] = vT[i];
						if (vT[i] > fBoundingBox[1][i])
							fBoundingBox[1][i] = vT[i];
					}
				} else {
					for (int i=0; i<3; i++) {
						if (cParticle.vPos[i]-cParticle.fSize < fBoundingBox[0][i])
							fBoundingBox[0][i] = cParticle.vPos[i]-cParticle.fSize;
						if (cParticle.vPos[i]+cParticle.fSize > fBoundingBox[1][i])
							fBoundingBox[1][i] = cParticle.vPos[i]+cParticle.fSize;
					}
				}
			}
		}
	}

	// Set bounding box
	AABoundingBox cAABoundingBox(fBoundingBox[0][Vector3::X], fBoundingBox[0][Vector3::Y], fBoundingBox[0][Vector3::Z],
								 fBoundingBox[1][Vector3::X], fBoundingBox[1][Vector3::Y], fBoundingBox[1][Vector3::Z]);

	// Are the particles within the local scene node space?
	if (bActiveParticles && !(GetFlags() & SceneNodeSpaceParticles)) {
		// To scene node space
		cAABoundingBox.vMin -= GetTransform().GetPosition();
		cAABoundingBox.vMax -= GetTransform().GetPosition();
	}

	// Set the new axis aligned bounding box
	SetAABoundingBox(cAABoundingBox);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
