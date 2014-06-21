/*********************************************************\
 *  File: Line.h                                         *
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


#ifndef __PLMATH_LINE_H__
#define __PLMATH_LINE_H__
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
*    Line class (finite length)
*/
class Line {


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		Vector3 vStart;	/**< Line start point */
		Vector3 vEnd;	/**< Line end point */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all start- and end-position components to 0
		*/
		inline Line();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline Line(const Line &cSource);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] vStartPoint
		*    Start point
		*  @param[in] vEndPoint
		*    End point
		*/
		inline Line(const Vector3 &vStartPoint, const Vector3 &vEndPoint);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Line();

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
		inline Line &operator =(const Line &cSource);

		/**
		*  @brief
		*    Sets the start and end position of the line
		*
		*  @param[in] vStart
		*    Line start position
		*  @param[in] vEnd
		*    Line end position
		*/
		inline void Set(const Vector3 &vStart, const Vector3 &vEnd);

		//[-------------------------------------------------------]
		//[ Transformation                                        ]
		//[-------------------------------------------------------]
		PLMATH_API Line  operator *(const Matrix3x3 &mRot) const;
		PLMATH_API Line  operator *(const Matrix3x4 &mTrans) const;
		PLMATH_API Line  operator *(const Matrix4x4 &mTrans) const;
		PLMATH_API Line &operator *=(const Matrix3x3 &mRot);
		PLMATH_API Line &operator *=(const Matrix3x4 &mTrans);
		PLMATH_API Line &operator *=(const Matrix4x4 &mTrans);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Line.inl"


#endif // __PLMATH_LINE_H__
