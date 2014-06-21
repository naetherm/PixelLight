/*********************************************************\
 *  File: BoundingBox.h                                  *
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


#ifndef __PLMATH_BOUNDINGBOX_H__
#define __PLMATH_BOUNDINGBOX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Matrix3x3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Bounding box class
*/
class BoundingBox {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Intersect;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMATH_API BoundingBox();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLMATH_API BoundingBox(const BoundingBox &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLMATH_API ~BoundingBox();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		PLMATH_API BoundingBox &operator =(const BoundingBox &cSource);

		/**
		*  @brief
		*    Returns the size of the bounding box
		*
		*  @param[out] fX1
		*    Receives the x position of corner 1
		*  @param[out] fY1
		*    Receives the y position of corner 1
		*  @param[out] fZ1
		*    Receives the z position of corner 1
		*  @param[out] fX2
		*    Receives the x position of corner 2
		*  @param[out] fY2
		*    Receives the y position of corner 2
		*  @param[out] fZ2
		*    Receives the z position of corner 2
		*/
		PLMATH_API void GetSize(float &fX1, float &fY1, float &fZ1, float &fX2, float &fY2, float &fZ2) const;

		/**
		*  @brief
		*    Set the size of the bounding box
		*
		*  @param[in] fX1
		*    X position of corner 1
		*  @param[in] fY1
		*    Y position of corner 1
		*  @param[in] fZ1
		*    Z position of corner 1
		*  @param[in] fX2
		*    X position of corner 2
		*  @param[in] fY2
		*    Y position of corner 2
		*  @param[in] fZ2
		*    Z position of corner 2
		*/
		PLMATH_API void SetSize(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2);

		/**
		*  @brief
		*    Returns corner 1 (min -> world space)
		*
		*  @param[in] bRotate
		*    Rotate corner corresponding to the bounding box?
		*
		*  @return
		*    Corner 1
		*/
		PLMATH_API Vector3 GetCorner1(bool bRotate = true) const;

		/**
		*  @brief
		*    Returns corner 2 (max -> world space)
		*
		*  @param[in] bRotate
		*    Rotate corner corresponding to the bounding box?
		*
		*  @return
		*    Corner 2
		*/
		PLMATH_API Vector3 GetCorner2(bool bRotate = true) const;

		/**
		*  @brief
		*    Returns the radius of the bounding sphere
		*
		*  @return
		*    Bounding sphere radius
		*/
		PLMATH_API float GetBoundingSphere() const;

		/**
		*  @brief
		*    Returns the current position
		*
		*  @return
		*    Current position
		*/
		PLMATH_API const Vector3 &GetPos() const;

		/**
		*  @brief
		*    Set the current position
		*
		*  @param[in] fX
		*    New x position
		*  @param[in] fY
		*    New y position
		*  @param[in] fZ
		*    New z position
		*/
		PLMATH_API void SetPos(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);

		/**
		*  @brief
		*    Set the current position
		*
		*  @param[in] vPos
		*    New position
		*/
		PLMATH_API void SetPos(const Vector3 &vPos);

		/**
		*  @brief
		*    Returns the current scale
		*
		*  @return
		*    Current scale
		*/
		PLMATH_API const Vector3 &GetScale() const;

		/**
		*  @brief
		*    Set the current scale
		*
		*  @param[in] fX
		*    New x scale
		*  @param[in] fY
		*    New y scale
		*  @param[in] fZ
		*    New z scale
		*/
		PLMATH_API void SetScale(float fX = 1.0f, float fY = 1.0f, float fZ = 1.0f);

		/**
		*  @brief
		*    Set the current scale
		*
		*  @param[in] vScale
		*    New scale
		*/
		PLMATH_API void SetScale(const Vector3 &vScale);

		/**
		*  @brief
		*    Returns the current final rotation matrix
		*
		*  @return
		*    Current final rotation matrix
		*/
		PLMATH_API const Matrix3x3 &GetFinalRot() const;

		/**
		*  @brief
		*    Returns the current rotation matrix
		*
		*  @return
		*    Current rotation matrix
		*/
		PLMATH_API const Matrix3x3 &GetRot() const;

		/**
		*  @brief
		*    Set the current rotation matrix
		*
		*  @param[in] mRot
		*    New rotation matrix
		*/
		PLMATH_API void SetRot(const Matrix3x3 &mRot);

		/**
		*  @brief
		*    Returns the current base rotation matrix
		*
		*  @return
		*    Current base rotation matrix
		*/
		PLMATH_API const Matrix3x3 &GetBaseRot() const;

		/**
		*  @brief
		*    Set the current base rotation matrix
		*
		*  @param[in] mBaseRot
		*    New base rotation matrix
		*/
		PLMATH_API void SetBaseRot(const Matrix3x3 &mBaseRot);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Updates the bounding box values
		*/
		void Update();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Object space definition of the box
		Vector3		m_vCorner1, m_vCorner2;		/**< Corners of the box */
		float		m_fW, m_fH, m_fD;			/**< Size of the box (from one corner to the other) */
		float		m_fExtX, m_fExtY, m_fExtZ;	/**< Size of the box (extends from the center of the box) */

		// World space coordinates
		Vector3		m_vPos;						/**< Box position */
		Vector3		m_vScale;					/**< Box scale */
		Matrix3x3	m_mRot;						/**< Box rotation */
		Matrix3x3	m_mBaseRot;					/**< Box base rotation */
		Matrix3x3	m_mFinalRot;				/**< Box final rotation */
		Vector3		m_vCenter;					/**< Center of the box */
		float		m_fRadius;					/**< Bounding sphere radius */
		Vector3		m_vX, m_vY, m_vZ;			/**< Local axis */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_BOUNDINGBOX_H__
