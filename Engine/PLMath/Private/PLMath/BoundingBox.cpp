/*********************************************************\
 *  File: BoundingBox.cpp                                *
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
#include "PLMath/BoundingBox.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BoundingBox::BoundingBox() :
	m_fW(0.0f),
	m_fH(0.0f),
	m_fD(0.0f),
	m_fExtX(0.0f),
	m_fExtY(0.0f),
	m_fExtZ(0.0f),
	m_vScale(1.0f, 1.0f, 1.0f),
	m_vX(1.0f, 0.0f, 0.0f),
	m_vY(0.0f, 1.0f, 0.0f),
	m_vZ(0.0f, 0.0f, 1.0f)
{
}

/**
*  @brief
*    Copy constructor
*/
BoundingBox::BoundingBox(const BoundingBox &cSource) :
	m_vCorner1(cSource.m_vCorner1),
	m_vCorner2(cSource.m_vCorner2),
	m_fW(cSource.m_fW),
	m_fH(cSource.m_fH),
	m_fD(cSource.m_fD),
	m_fExtX(cSource.m_fExtX),
	m_fExtY(cSource.m_fExtY),
	m_fExtZ(cSource.m_fExtZ),
	m_vPos(cSource.m_vPos),
	m_vScale(cSource.m_vScale),
	m_mRot(cSource.m_mRot),
	m_mBaseRot(cSource.m_mBaseRot),
	m_mFinalRot(cSource.m_mFinalRot),
	m_vCenter(cSource.m_vCenter),
	m_fRadius(cSource.m_fRadius),
	m_vX(cSource.m_vX),
	m_vY(cSource.m_vY),
	m_vZ(cSource.m_vZ)
{
}

/**
*  @brief
*    Destructor
*/
BoundingBox::~BoundingBox()
{
}

/**
*  @brief
*    Copy operator
*/
BoundingBox &BoundingBox::operator =(const BoundingBox &cSource)
{
	// Copy data
	m_vCorner1	= cSource.m_vCorner1;
	m_vCorner2	= cSource.m_vCorner2;
	m_fW		= cSource.m_fW;
	m_fH		= cSource.m_fH;
	m_fD		= cSource.m_fD;
	m_fExtX		= cSource.m_fExtX;
	m_fExtY		= cSource.m_fExtY;
	m_fExtZ		= cSource.m_fExtZ;
	m_vPos		= cSource.m_vPos;
	m_vScale	= cSource.m_vScale;
	m_mRot		= cSource.m_mRot;
	m_mBaseRot	= cSource.m_mBaseRot;
	m_mFinalRot	= cSource.m_mFinalRot;
	m_vCenter	= cSource.m_vCenter;
	m_vX		= cSource.m_vX;
	m_vY		= cSource.m_vY;
	m_vZ		= cSource.m_vZ;

	// Return this
	return *this;
}

/**
*  @brief
*    Returns the size of the bounding box
*/
void BoundingBox::GetSize(float &fX1, float &fY1, float &fZ1, float &fX2, float &fY2, float &fZ2) const
{
	fX1 = m_vCorner1.GetX();
	fY1 = m_vCorner1.GetY();
	fZ1 = m_vCorner1.GetZ();
	fX2 = m_vCorner2.GetX();
	fY2 = m_vCorner2.GetY();
	fZ2 = m_vCorner2.GetZ();
}

/**
*  @brief
*    Set the size of the bounding box
*/
void BoundingBox::SetSize(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2)
{
	// Set new bounding box size
	m_vCorner1.SetXYZ(fX1, fY1, fZ1);
	m_vCorner2.SetXYZ(fX2, fY2, fZ2);

	// Update bounding box
	Update();
}

/**
*  @brief
*    Returns corner 1 (min -> world space)
*/
Vector3 BoundingBox::GetCorner1(bool bRotate) const
{
	return bRotate ? m_mFinalRot*m_vCorner1*m_vScale+m_vPos : m_vCorner1*m_vScale+m_vPos;
}

/**
*  @brief
*    Returns corner 2 (max -> world space)
*/
Vector3 BoundingBox::GetCorner2(bool bRotate) const
{
	return bRotate ? m_mFinalRot*m_vCorner2*m_vScale+m_vPos : m_vCorner2*m_vScale+m_vPos;
}

/**
*  @brief
*    Returns the radius of the bounding sphere
*/
float BoundingBox::GetBoundingSphere() const
{
	return m_fRadius;
}

/**
*  @brief
*    Returns the current position
*/
const Vector3 &BoundingBox::GetPos() const
{
	// Return position
	return m_vPos;
}

/**
*  @brief
*    Set the current position
*/
void BoundingBox::SetPos(float fX, float fY, float fZ)
{
	// Set new position
	m_vPos.SetXYZ(fX, fY, fZ);

	// Update bounding box
	Update();
}

/**
*  @brief
*    Set the current position
*/
void BoundingBox::SetPos(const Vector3 &vPos)
{
	// Set new position
	m_vPos = vPos;

	// Update bounding box
	Update();
}

/**
*  @brief
*    Returns the current scale
*/
const Vector3 &BoundingBox::GetScale() const
{
	// Return scale
	return m_vScale;
}

/**
*  @brief
*    Set the current scale
*/
void BoundingBox::SetScale(float fX, float fY, float fZ)
{
	// Set new scale
	m_vScale.SetXYZ(fX, fY, fZ);

	// Update bounding box
	Update();
}

/**
*  @brief
*    Set the current scale
*/
void BoundingBox::SetScale(const Vector3 &vScale)
{
	// Set new scale
	m_vScale = vScale;

	// Update bounding box
	Update();
}

/**
*  @brief
*    Returns the current final rotation matrix
*/
const Matrix3x3 &BoundingBox::GetFinalRot() const
{
	// Return final rotation matrix
	return m_mFinalRot;
}

/**
*  @brief
*    Returns the current rotation matrix
*/
const Matrix3x3 &BoundingBox::GetRot() const
{
	// Return rotation matrix
	return m_mRot;
}

/**
*  @brief
*    Set the current rotation matrix
*/
void BoundingBox::SetRot(const Matrix3x3 &mRot)
{
	// Set new rotation matrix
	m_mRot = mRot;

	// Update bounding box
	Update();
}

/**
*  @brief
*    Returns the current base rotation matrix
*/
const Matrix3x3 &BoundingBox::GetBaseRot() const
{
	// Return base rotation matrix
	return m_mBaseRot;
}

/**
*  @brief
*    Set the current base rotation matrix
*/
void BoundingBox::SetBaseRot(const Matrix3x3 &mBaseRot)
{
	// Set new base rotation matrix
	m_mBaseRot = mBaseRot;

	// Update bounding box
	Update();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the bounding box values
*/
void BoundingBox::Update()
{
	// Get width, height and depth
	m_fW	= (m_vCorner2.x-m_vCorner1.x)*m_vScale.x;
	m_fH	= (m_vCorner2.y-m_vCorner1.y)*m_vScale.y;
	m_fD	= (m_vCorner2.z-m_vCorner1.z)*m_vScale.z;
	m_fExtX = Math::Abs(m_fW/2.0f);
	m_fExtY = Math::Abs(m_fH/2.0f);
	m_fExtZ = Math::Abs(m_fD/2.0f);

	// Get final rotation matrix
	m_mFinalRot = m_mBaseRot*m_mRot;

	// Get center
	m_vCenter = m_vPos + (m_mFinalRot*m_vCorner1*m_vScale + m_mFinalRot*m_vCorner2*m_vScale) * 0.5f;

	// Get radius
	m_fRadius = Math::Max((m_vCorner1*m_vScale).GetLength(), (m_vCorner2*m_vScale).GetLength());

	// Get local axis
	m_vX = m_mFinalRot.GetXAxis().Normalize();
	m_vY = m_mFinalRot.GetYAxis().Normalize();
	m_vZ = m_mFinalRot.GetZAxis().Normalize();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
