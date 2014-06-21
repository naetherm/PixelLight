/*********************************************************\
 *  File: Sphere.h                                       *
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


#ifndef __PLMATH_SPHERE_H__
#define __PLMATH_SPHERE_H__
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
*    Sphere class
*/
class Sphere {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all position components and the radius to 0
		*/
		PLMATH_API Sphere();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLMATH_API Sphere(const Sphere &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLMATH_API ~Sphere();

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
		PLMATH_API Sphere &operator =(const Sphere &cSource);

		/**
		*  @brief
		*    Returns the radius of the sphere
		*
		*  @return
		*    Sphere radius
		*/
		PLMATH_API float GetRadius() const;

		/**
		*  @brief
		*    Sets the radius of the sphere
		*
		*  @param[in] fRadius
		*    Sphere radius
		*/
		PLMATH_API void SetRadius(float fRadius = 0.0f);

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
		*    X component of the new position
		*  @param[in] fY
		*    Y component of the new position
		*  @param[in] fZ
		*    Z component of the new position
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


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float   m_fRadius;	/**< Sphere radius */
		Vector3 m_vPos;		/**< Sphere position */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_SPHERE_H__
