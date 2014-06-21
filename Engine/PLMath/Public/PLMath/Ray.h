/*********************************************************\
 *  File: Ray.h                                          *
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


#ifndef __PLMATH_RAY_H__
#define __PLMATH_RAY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Vector3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ray class (infinite length, in both directions)
*/
class Ray {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all position and direction components to 0
		*/
		inline Ray();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline Ray(const Ray &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Ray();

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
		inline Ray &operator =(const Ray &cSource);

		/**
		*  @brief
		*    Set the current ray using a line
		*
		*  @param[in] vLineStartPos
		*    Line start position
		*  @param[in] vLineEndPos
		*    Line end position
		*/
		inline void Set(const Vector3 &vLineStartPos, const Vector3 &vLineEndPos);

		/**
		*  @brief
		*    Returns the current position
		*
		*  @return
		*    Current position
		*/
		inline const Vector3 &GetPos() const;

		/**
		*  @brief
		*    Set the current position
		*
		*  @param[in] fX
		*    X component of the new position
		*  @param[in] fY
		*    Y component of the new position
		*  @param[in] fZ
		*    Z component of the new position
		*/
		inline void SetPos(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);

		/**
		*  @brief
		*    Set the current position
		*
		*  @param[in] vPos
		*    New position
		*/
		inline void SetPos(const Vector3 &vPos);

		/**
		*  @brief
		*    Returns the ray direction
		*
		*  @return
		*    Ray direction
		*/
		inline const Vector3 &GetDir() const;

		/**
		*  @brief
		*    Set the ray direction
		*
		*  @param[in] fX
		*    X component of the new direction vector
		*  @param[in] fY
		*    Y component of the new direction vector
		*  @param[in] fZ
		*    Z component of the new direction vector
		*/
		inline void SetDir(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);

		/**
		*  @brief
		*    Set the ray direction
		*
		*  @param[in] vDir
		*    New ray direction
		*/
		inline void SetDir(const Vector3 &vDir);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Vector3 m_vPos;	/**< Ray origin */
		Vector3 m_vDir;	/**< Ray direction */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Ray.inl"


#endif // __PLMATH_RAY_H__
