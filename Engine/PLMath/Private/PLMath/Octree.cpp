/*********************************************************\
 *  File: Octree.cpp                                     *
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
#include <PLCore/Container/Bitset.h>
#include "PLMath/Intersect.h"
#include "PLMath/Octree.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Octree::Octree()
{
	Init();
}

/**
*  @brief
*    Destructor
*/
Octree::~Octree()
{
	Destroy();
}

/**
*  @brief
*    Returns whether the octree is build or not
*/
bool Octree::IsBuild() const
{
	return m_bBuild;
}

/**
*  @brief
*    Initializes the octree
*/
void Octree::Init(Octree *pParent, int nSubdivide, uint32 nMinGeometries, uint32 nIDOffset)
{
	m_nIDOffset = nIDOffset;
	if (pParent) {
		m_nID      = pParent->m_nID+8+m_nIDOffset;
		m_pTopmost = pParent->m_pTopmost;
		m_nLevel   = pParent->m_nLevel+1;
	} else {
		m_nID      = nIDOffset;
		m_pTopmost = this;
		m_nLevel   = 0;
	}
	m_pParent		 = pParent;
	m_nNumOfChildren = 0;
	m_ppChild		 = nullptr;
	m_bVisible		 = false;
	m_bBuild		 = false;
	m_nSubdivide	 = nSubdivide;
	m_nMinGeometries = nMinGeometries;
}

/**
*  @brief
*    Destroys the octree
*/
void Octree::Destroy()
{
	for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
		delete m_ppChild[nChild];
	if (m_ppChild)
		delete [] m_ppChild;
	Init();
}

/**
*  @brief
*    Returns the subdivision level
*/
uint32 Octree::GetLevel() const
{
	return m_nLevel;
}

/**
*  @brief
*    Returns whether the octree is currently visible or not
*/
bool Octree::IsVisible() const
{
	return m_bVisible;
}

/**
*  @brief
*    Sets whether the octree is currently visible or not
*/
void Octree::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

/**
*  @brief
*    Returns the octrees bounding box
*/
const BoundingBox &Octree::GetBoundingBox() const
{
	return m_cBoundingBox;
}

/**
*  @brief
*    Returns the subdivide
*/
int Octree::GetSubdivide() const
{
	return m_nSubdivide;
}

/**
*  @brief
*    Returns the minimum number of geometries per octree
*/
uint32 Octree::GetMinGeometries() const
{
	return m_nMinGeometries;
}

/**
*  @brief
*    Returns the current position
*/
const Vector3 &Octree::GetPos() const
{
	return m_vPos;
}

/**
*  @brief
*    Set the current position
*/
void Octree::SetPos(const Vector3 &vPos)
{
	// Set position
	m_vPos = vPos;
	m_cBoundingBox.SetPos(vPos+m_vBBCenter*m_cBoundingBox.GetScale());

	// Set children
	for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
		m_ppChild[nChild]->SetPos(vPos);
}

/**
*  @brief
*    Returns the current scale
*/
const Vector3 &Octree::GetScale() const
{
	return m_cBoundingBox.GetScale();
}

/**
*  @brief
*    Set the current scale
*/
void Octree::SetScale(const Vector3 &vScale)
{
	// Set scale
	m_cBoundingBox.SetScale(vScale);

	// Update bounding box position
	m_cBoundingBox.SetPos(m_vPos+vScale*m_vBBCenter);

	// Set children
	for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
		m_ppChild[nChild]->SetScale(vScale);
}

/**
*  @brief
*    Returns the current rotation matrix
*/
const Matrix3x3 &Octree::GetRot() const
{
	return m_cBoundingBox.GetRot();
}

/**
*  @brief
*    Set the current rotation matrix
*/
void Octree::SetRot(const Matrix3x3 &mRot)
{
	// Set rotation
	m_cBoundingBox.SetRot(mRot);

	// Set children
	for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++) {
		Octree *pChild = m_ppChild[nChild];

		// Update position and rotation
		pChild->m_cBoundingBox.SetPos(m_vPos+(mRot*pChild->m_vBBCenter)*pChild->m_cBoundingBox.GetScale());
		pChild->SetRot(mRot);
	}
}

/**
*  @brief
*    Updates the visibility information of the octree
*/
void Octree::UpdateVisibility(const PlaneSet &cPlaneSet, Bitset *pBitset)
{
	// Is the octree build?
	if (m_bBuild) {
		// Check whether this octree is visible
		if (Intersect::BoxPlaneSet(m_cBoundingBox, cPlaneSet)) {
			// Visible
			m_bVisible = true;

			if (m_nNumOfChildren) {
				// Update children
				for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
					m_ppChild[nChild]->UpdateVisibility(cPlaneSet, pBitset);
			} else {
				// Call custom visible function
				CustomVisible(pBitset);
			}
		} else {
			// Not visible
			m_bVisible = false;

			// Call custom invisible function
			CustomInvisible(pBitset);
		}
	}
}

/**
*  @brief
*    Check whether a sphere intersects with this octree
*/
void Octree::CheckSphere(const Vector3 &vPos, float fRadius)
{
	// Is the octree build?
	if (m_bBuild) {
		// Check whether the sphere is in the octree
		if (Intersect::SphereBox(vPos, fRadius, m_cBoundingBox)) {
			// Visible
			m_bVisible = true;

			if (m_nNumOfChildren) {
				// Update children
				for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
					m_ppChild[nChild]->CheckSphere(vPos, fRadius);
			} else {
				// Call custom visible function
				CustomVisible();
			}
		} else {
			// Not visible
			m_bVisible = false;

			// Call custom invisible function
			CustomInvisible();
		}
	}
}

/**
*  @brief
*    Check whether a box intersects with this octree
*/
void Octree::CheckBox(const BoundingBox &cBox)
{
	// Is the octree build?
	if (m_bBuild) {
		// Check whether the box is in the octree
		if (Intersect::BoxBox(m_cBoundingBox, cBox)) {
			// Visible
			m_bVisible = true;

			if (m_nNumOfChildren) {
				// Update children
				for (uint32 nChild=0; nChild<m_nNumOfChildren; nChild++)
					m_ppChild[nChild]->CheckBox(cBox);
			} else {
				// Call custom visible function
				CustomVisible();
			}
		} else {
			// Not visible
			m_bVisible = false;

			// Call custom invisible function
			CustomInvisible();
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Octree::Octree(const Octree &cSource) :
	m_nIDOffset(0),
	m_nID(0),
	m_pTopmost(nullptr),
	m_pParent(nullptr),
	m_nLevel(0),
	m_nNumOfChildren(0),
	m_ppChild(nullptr),
	m_bVisible(0),
	m_bBuild(0),
	m_nSubdivide(0),
	m_nMinGeometries(0)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Octree &Octree::operator =(const Octree &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Private virtual Octree functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Custom visible function
*/
void Octree::CustomVisible(Bitset *pBitset)
{
	// Here a derived class will do some custom things...
}

/**
*  @brief
*    Custom invisible function
*/
void Octree::CustomInvisible(Bitset *pBitset)
{
	// Here a derived class will do some custom things...
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
