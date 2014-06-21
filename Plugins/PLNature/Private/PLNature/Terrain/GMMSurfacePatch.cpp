/*********************************************************\
 *  File: GMMSurfacePatch.cpp                            *
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
#include "PLNature/Terrain/GMMSurface.h"
#include "PLNature/Terrain/GMMSurfacePatch.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLNature {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether the patch is active or not
*/
bool GMMSurfacePatch::IsActive() const
{
	return m_bActive;
}

/**
*  @brief
*    Sets whether the patch is active or not
*/
void GMMSurfacePatch::SetActive(bool bActive)
{
	m_bActive = bActive;
}

/**
*  @brief
*    Computes the variance of each mipmap map level
*/
void GMMSurfacePatch::ComputeVariance(uint32 &nOffset)
{
	uint32 nStep, nXOffset, nYOffset, px, py;
	float fHeight, fHeight1, fHeight2;
	float fVariance = 0.0f, f;

	// Reset variance
	for (uint32 i=0; i<m_pGMMSurface->GetGeoMipMaps(); i++)
		m_pVariance[i] = 0.0f;

	m_nOffset = nOffset;
	GMMSurface::Vertex *pVertex = m_pGMMSurface->GetVertex(nOffset);

	// Get variance
	for (uint32 nX=0; nX<=m_pGMMSurface->GetPatchSize(); nX++)
		for (uint32 nY=0; nY<=m_pGMMSurface->GetPatchSize(); nY++) {
			// Calculate offset
			nXOffset = m_nXOffset+nX;
			nYOffset = m_nYOffset+nY;

			// Get height
			fHeight = m_pGMMSurface->GetHeightMap()[nYOffset*m_pGMMSurface->GetHeightMapSize()+nXOffset];

			// First texture coordinates
			pVertex->u = static_cast<float>(nXOffset)/m_pGMMSurface->GetHeightMapSize();
			pVertex->v = static_cast<float>(nYOffset)/m_pGMMSurface->GetHeightMapSize();

			// Set vertex position
			pVertex->x = static_cast<float>(nXOffset);
			pVertex->y = fHeight;
			pVertex->z = static_cast<float>(nYOffset);

			// Update variance levels
			for (int i=0; i<4; i++) {
				nStep = m_pGMMSurface->GetPatchSize()>>i;
				if ((nXOffset%nStep) || (nYOffset%nStep)) {
					px = (nXOffset/nStep)*nStep;
					py = (nYOffset/nStep)*nStep;

					if (!(nXOffset%nStep)) {
						// Get height
						fHeight1 = m_pGMMSurface->GetHeightMap()[py*m_pGMMSurface->GetHeightMapSize()+px];
						fHeight2 = m_pGMMSurface->GetHeightMap()[(py+nStep)*m_pGMMSurface->GetHeightMapSize()+px];

						// Compute variance
						f = static_cast<float>(fHeight2-fHeight1)/nStep;
						fVariance = pVertex->y-(fHeight1+(nYOffset%nStep)*f);
					} else if (!(nYOffset%nStep)) {
						// Get height
						fHeight1 = m_pGMMSurface->GetHeightMap()[py*m_pGMMSurface->GetHeightMapSize()+px];
						fHeight2 = m_pGMMSurface->GetHeightMap()[py*m_pGMMSurface->GetHeightMapSize()+px+nStep];

						// Compute variance
						f = static_cast<float>(fHeight2-fHeight1)/nStep;
						fVariance = pVertex->y-(fHeight1+(nXOffset%nStep)*f);
					}

					if (fVariance > m_pVariance[i])
						m_pVariance[i] =  fVariance;
					else if (fVariance < -m_pVariance[i])
						m_pVariance[i] = -fVariance;
				}
			}

			// Next vertex
			pVertex++;
			nOffset++;
		}
}

/**
*  @brief
*    Updates the patch bounding box
*/
void GMMSurfacePatch::UpdateBoundingBox()
{
	// Init bounding box
	GetBBMin().y = 255.0f;
	GetBBMax().y =   0.0f;
	for (uint32 nY=0; nY<=m_pGMMSurface->GetPatchSize(); nY++)
		for (uint32 nX=0; nX<=m_pGMMSurface->GetPatchSize(); nX++) {
			float fHeight = m_pGMMSurface->GetHeightMap()[(m_nYOffset+nY)*m_pGMMSurface->GetHeightMapSize()+m_nXOffset+nX];
			if (GetBBMin().y > fHeight)
				GetBBMin().y = fHeight;
			if (GetBBMax().y < fHeight)
				GetBBMax().y = fHeight;
		}

	// Get bounding box
	GetBBMin().x = static_cast<float>(m_nXOffset);
	GetBBMin().z = static_cast<float>(m_nYOffset);
	GetBBMax().x = static_cast<float>(m_nXOffset)+m_pGMMSurface->GetPatchSize();
	GetBBMax().z = static_cast<float>(m_nYOffset)+m_pGMMSurface->GetPatchSize();

	// Get center
	m_vCenter = (GetBBMin()+GetBBMax())/2;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GMMSurfacePatch::GMMSurfacePatch(GMMSurface &cGMMSurface) :
	m_pGMMSurface(&cGMMSurface)
{
	Init();
}

/**
*  @brief
*    Destructor
*/
GMMSurfacePatch::~GMMSurfacePatch()
{
	Destroy();
}

/**
*  @brief
*    Initializes the GMM surface patch
*/
void GMMSurfacePatch::Init()
{
	m_bActive		= true;
	m_nLevel		= 0;
	m_pVariance		= nullptr;
	m_nOffset		= 0;
	m_nXOffset		= 0;
	m_nYOffset		= 0;
	m_pNeighbour[0] = nullptr;
	m_pNeighbour[1] = nullptr;
	m_pNeighbour[2] = nullptr;
	m_pNeighbour[3] = nullptr;
}

/**
*  @brief
*    Creates the GMM surface patch
*/
bool GMMSurfacePatch::Create(uint32 nXOffset, uint32 nYOffset, uint32 &nOffset)
{
	// Destroy the old GMM surface patch
	Destroy();

	// Setup
	m_nXOffset = nXOffset;
	m_nYOffset = nYOffset;

	// Height variance
	m_pVariance = new float[m_pGMMSurface->GetGeoMipMaps()];
	ComputeVariance(nOffset);

	// Get bounding box
	UpdateBoundingBox();

	// Done
	return true;
}

/**
*  @brief
*    Kills the GMM surface patch
*/
void GMMSurfacePatch::Destroy()
{
	if (m_pVariance)
		delete [] m_pVariance;

	// Init patch
	Init();
}

/**
*  @brief
*    Updates the GMM surface patch
*/
void GMMSurfacePatch::Update()
{
	// Is the patch active and visible?
	if (m_bActive && IsVisible()) {
		// Update level
		const Vector3 &vViewerPos = m_pGMMSurface->GetViewerPos();
		Vector3 vDelta;

		double detconst = 0.00075*12*1/1.0f;
		uint32 md = m_pGMMSurface->GetGeoMipMaps()-1, me = m_pGMMSurface->GetGeoMipMaps()-1;
		float f = 3000*3000;

		vDelta = m_vCenter-vViewerPos;
		float l = vDelta.DotProduct(vDelta);

		// [TODO] Better current LOD calculation!
//		m_pGMMSurface->GetGeoMipMaps()
		if (    0   <= l  &&  l < f/2)	   md=5;
		if (  f/2   <= l  &&  l < 3*f/4)   md=4;
		if (3*f/4   <= l  &&  l < 7*f/8)   md=3;
		if (7*f/8   <= l  &&  l < 15*f/16) md=2;
		if (15*f/16 <= l  &&  l < 30*f/32) md=1;
		if (20*f/20 <= l)				   md=0;

		for (uint32 i=0; i<m_pGMMSurface->GetGeoMipMaps(); i++) {
			if (m_pVariance[i]*m_pVariance[i]/l <= detconst*detconst) {
				me = i+1;
				break;
			}
		}

		(md < me) ? (m_nLevel = md) : (m_nLevel = me);
		if (l > 500000)
			m_nLevel = 0;
		if (m_nLevel >= m_pGMMSurface->GetGeoMipMaps())
			m_nLevel = m_pGMMSurface->GetGeoMipMaps()-1;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature
