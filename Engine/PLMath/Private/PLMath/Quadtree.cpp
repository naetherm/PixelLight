/*********************************************************\
 *  File: Quadtree.cpp                                   *
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
#include "PLMath/Intersect.h"
#include "PLMath/Quadtree.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ QuadtreePatch                                         ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
QuadtreePatch::QuadtreePatch() :
	m_bVisible(true)
{
	// Init data
	m_vBoundingBox[0] = Vector3::Zero;
	m_vBoundingBox[1] = Vector3::Zero;
}

/**
*  @brief
*    Destructor
*/
QuadtreePatch::~QuadtreePatch()
{
}

/**
*  @brief
*    Returns whether the patch is currently visible or not
*/
bool QuadtreePatch::IsVisible() const
{
	return m_bVisible;
}

/**
*  @brief
*    Sets whether the patch is currently visible or not
*/
void QuadtreePatch::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

/**
*  @brief
*    Returns the bounding box minimum coordinate
*/
Vector3 &QuadtreePatch::GetBBMin()
{
	return m_vBoundingBox[0];
}

/**
*  @brief
*    Returns the bounding box maximum coordinate
*/
Vector3 &QuadtreePatch::GetBBMax()
{
	return m_vBoundingBox[1];
}


//[-------------------------------------------------------]
//[ Quadtree                                              ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Quadtree::Quadtree()
{
	Init();
}

/**
*  @brief
*    Destructor
*/
Quadtree::~Quadtree()
{
	Destroy();
}

/**
*  @brief
*    Initializes the quadtree
*/
void Quadtree::Init(uint32 nXOffset, uint32 nYOffset, uint32 nXSize, uint32 nYSize, Quadtree *pParent)
{
	m_nXOffset		  = nXOffset;
	m_nYOffset		  = nYOffset;
	m_nXSize		  = nXSize;
	m_nYSize		  = nYSize;
	m_pTopmost		  = pParent ? pParent->m_pTopmost : this;
	m_pParent		  = pParent;
	m_nNumOfChildren  = 0;
	m_pChild		  = nullptr;
	m_vBoundingBox[0] = Vector3::Zero;
	m_vBoundingBox[1] = Vector3::Zero;
}

/**
*  @brief
*    Builds the quadtree
*/
bool Quadtree::Build()
{
	// Get number of children
	if (m_nXSize > 1)
		m_nNumOfChildren += 2;
	if (m_nYSize > 1)
		m_nNumOfChildren += 2;

	// Setup children
	if (m_nNumOfChildren) {
		// Allocate memory
		m_pChild = new Quadtree[m_nNumOfChildren];
		
		// Get the half quadtree size
		uint32 nXHalfSize = (m_nXSize > 1) ? m_nXSize/2 : m_nXSize;
		uint32 nYHalfSize = (m_nYSize > 1) ? m_nYSize/2 : m_nYSize;

		Quadtree *pQuadtree = m_pChild;
		pQuadtree->Init(m_nXOffset, m_nYOffset, nXHalfSize, nYHalfSize, this);
		pQuadtree->Build();
		pQuadtree++;
		if (m_nXSize > 1) {
			pQuadtree->Init(m_nXOffset+nXHalfSize, m_nYOffset, nXHalfSize, nYHalfSize, this);
			pQuadtree->Build();
			pQuadtree++;
		}
		if (m_nYSize > 1) {
			pQuadtree->Init(m_nXOffset+nXHalfSize, m_nYOffset+nYHalfSize, nXHalfSize, nYHalfSize, this);
			pQuadtree->Build();
			pQuadtree++;
			if (m_nXSize > 1) {
				pQuadtree->Init(m_nXOffset, m_nYOffset+nYHalfSize, nXHalfSize, nYHalfSize, this);
				pQuadtree->Build();
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Destroy the quadtree
*/
void Quadtree::Destroy()
{
	for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
		m_pChild[nChild].Destroy();
	if (m_pChild)
		delete [] m_pChild;
	Init();
}

/**
*  @brief
*    Updates the bounding boxes of the quadtree
*/
void Quadtree::UpdateBoundingBoxes(QuadtreePatch **ppPatch)
{
	// Check pointer
	if (ppPatch) {
		// Init bounding box
		m_vBoundingBox[0].SetXYZ( 100000.0f,  100000.0f,  100000.0f);
		m_vBoundingBox[1].SetXYZ(-100000.0f, -100000.0f, -100000.0f);

		// Get bounding box
		for (uint32 nY=0; nY<m_nYSize; nY++)
			for (uint32 nX=0; nX<m_nXSize; nX++) {
				// Get current patch
				QuadtreePatch *pPatchT = ppPatch[(m_nYOffset+nY)*m_pTopmost->m_nXSize+m_nXOffset+nX];

				// Update min
				if (m_vBoundingBox[0].x > pPatchT->GetBBMin().x)
					m_vBoundingBox[0].x = pPatchT->GetBBMin().x;
				if (m_vBoundingBox[0].y > pPatchT->GetBBMin().y)
					m_vBoundingBox[0].y = pPatchT->GetBBMin().y;
				if (m_vBoundingBox[0].z > pPatchT->GetBBMin().z)
					m_vBoundingBox[0].z = pPatchT->GetBBMin().z;

				// Update max
				if (m_vBoundingBox[1].x < pPatchT->GetBBMax().x)
					m_vBoundingBox[1].x = pPatchT->GetBBMax().x;
				if (m_vBoundingBox[1].y < pPatchT->GetBBMax().y)
					m_vBoundingBox[1].y = pPatchT->GetBBMax().y;
				if (m_vBoundingBox[1].z < pPatchT->GetBBMax().z)
					m_vBoundingBox[1].z = pPatchT->GetBBMax().z;
				
			}

		// Update children
		for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
			m_pChild[nChild].UpdateBoundingBoxes(ppPatch);
	}
}

/**
*  @brief
*    Updates the visibility information of the quadtree patches
*/
void Quadtree::UpdateVisibility(const PlaneSet &cPlaneSet, QuadtreePatch **ppPatch) const
{
	// Check pointer
	if (ppPatch) {
		// Initialize patch visibility
		if (!m_pParent) {
			// Topmost quadtree
			uint32 nNumOfElements = m_nXSize*m_nYSize;
			for (uint32 i=0; i<nNumOfElements; i++)
				ppPatch[i]->SetVisible(true);
		}

		// Check whether this quadtree is visible
		if (Intersect::PlaneSetAABox(cPlaneSet, m_vBoundingBox[0], m_vBoundingBox[1])) {
			// Update children
			for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
				m_pChild[nChild].UpdateVisibility(cPlaneSet, ppPatch);
		} else {
			for (uint32 nY=0; nY<m_nYSize; nY++)
				for (uint32 nX=0; nX<m_nXSize; nX++)
					ppPatch[(m_nYOffset+nY)*m_pTopmost->m_nXSize+m_nXOffset+nX]->SetVisible(false);
		}
	}
}

/**
*  @brief
*    Returns the bounding box minimum/maximum values
*/
void Quadtree::GetBoundingBoxMinMax(Vector3 &vMin, Vector3 &vMax) const
{
	vMin = m_vBoundingBox[0];
	vMax = m_vBoundingBox[1];
}

/**
*  @brief
*    Returns the number of children
*/
uint32 Quadtree::GetNumOfChildren() const
{
	return m_nNumOfChildren;
}

/**
*  @brief
*    Returns a child
*/
Quadtree *Quadtree::GetChild(uint32 nChild) const
{
	// Return the requested child
	return nChild < m_nNumOfChildren ? &m_pChild[nChild] : nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Quadtree::Quadtree(const Quadtree &cSource) :
	m_nXOffset(0),
	m_nYOffset(0),
	m_nXSize(0),
	m_nYSize(0),
	m_pTopmost(nullptr),
	m_pParent(nullptr),
	m_nNumOfChildren(0),
	m_pChild(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Quadtree &Quadtree::operator =(const Quadtree &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
