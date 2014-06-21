/*********************************************************\
 *  File: Rectangle.h                                    *
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


#ifndef __PLMATH_RECTANGLE_H__
#define __PLMATH_RECTANGLE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Vector2.h"
#include "PLMath/Vector4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Rectangle (2D axis aligned bounding box) class
*/
class Rectangle {


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		Vector2 vMin;	/**< Minimum position */
		Vector2 vMax;	/**< Maximum position */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor setting all minimum and maximum components to 0
		*/
		inline Rectangle();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline Rectangle(const Rectangle &cSource);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] vMin
		*    Minimum position
		*  @param[in] vMax
		*    Maximum position
		*/
		inline Rectangle(const Vector2 &vMin, const Vector2 &vMax);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] fMinX
		*    X component of the minimum position
		*  @param[in] fMinY
		*    Y component of the minimum position
		*  @param[in] fMaxX
		*    X component of the maximum position
		*  @param[in] fMaxY
		*    Y component of the maximum position
		*/
		inline Rectangle(float fMinX, float fMinY, float fMaxX, float fMaxY);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Rectangle();

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
		inline Rectangle &operator =(const Rectangle &cSource);

		/**
		*  @brief
		*    Returns the center of the rectangle
		*
		*  @return
		*    Center of the rectangle ((vMax+vMin)/2)
		*/
		inline Vector2 GetCenter() const;

		/**
		*  @brief
		*    Returns the x position
		*
		*  @return
		*    X position
		*/
		inline float GetX() const;

		/**
		*  @brief
		*    Returns the y position
		*
		*  @return
		*    Y position
		*/
		inline float GetY() const;

		/**
		*  @brief
		*    Returns the width
		*
		*  @return
		*    Width
		*/
		inline float GetWidth() const;

		/**
		*  @brief
		*    Returns the height
		*
		*  @return
		*    Height
		*/
		inline float GetHeight() const;

		/**
		*  @brief
		*    Calculates the surface of the rectangle
		*
		*  @return
		*    Surface of the rectangle
		*/
		inline float CalculateSurface() const;

		/**
		*  @brief
		*    Clips this rectangle with another one
		*
		*  @param[in] cEnclosed
		*    Rectangle to clip this rectangle with
		*/
		inline void ClipByRectangle(const Rectangle &cEnclosed);

		/**
		*  @brief
		*    Appends a vertex to the rectangle
		*
		*  @param[in] vV
		*    Vertex to append
		*/
		inline void AppendToRectangle(const Vector2 &vV);

		/**
		*  @brief
		*    Combines two rectangles
		*
		*  @param[in] cRectangle
		*    Rectangle to combine with this rectangle
		*/
		inline void CombineRectangles(const Rectangle &cRectangle);

		/**
		*  @brief
		*    Calculates the screen-space bounding rectangle of the given array of points
		*
		*  @param[in] lstPoints
		*    Array of points
		*  @param[in] bZCull
		*    Determines whether or not the function should also return 'false' if the rectangle
		*    lies outside the depth range
		*  @param[in] mMVP
		*    Concatenated modelview/projection matrix
		*  @param[in] nX
		*    X screen position
		*  @param[in] nY
		*    Y screen position
		*  @param[in] nWidth
		*    Screen width
		*  @param[in] nHeight
		*    Screen height
		*
		*  @remarks
		*    If all went fine, this rectangle will receive the screen-space bounding rectangle of the given array of points.
		*
		*  @return
		*    'false' if the rectangle is entirely off-screen, else 'true'
		*/
		PLMATH_API bool ScreenRectangle(const PLCore::Array<Vector3> &lstPoints, bool bZCull, const Matrix4x4 &mMVP, PLCore::uint32 nX, PLCore::uint32 nY, PLCore::uint32 nWidth, PLCore::uint32 nHeight);

		/**
		*  @brief
		*    Calculates the screen-space bounding rectangle of the given array of points
		*
		*  @param[in] lstPoints
		*    Array of points
		*  @param[in] bZCull
		*    Determines whether or not the function should also return 'false' if the rectangle
		*    lies outside the depth range
		*  @param[in] mMVP
		*    Concatenated modelview/projection matrix
		*  @param[in] nX
		*    X screen position
		*  @param[in] nY
		*    Y screen position
		*  @param[in] nWidth
		*    Screen width
		*  @param[in] nHeight
		*    Screen height
		*
		*  @remarks
		*    If all went fine, this rectangle will receive the screen-space bounding rectangle of the given array of points.
		*
		*  @return
		*    'false' if the rectangle is entirely off-screen, else 'true'
		*/
		PLMATH_API bool ScreenRectangle(const PLCore::Array<Vector4> &lstPoints, bool bZCull, const Matrix4x4 &mMVP, PLCore::uint32 nX, PLCore::uint32 nY, PLCore::uint32 nWidth, PLCore::uint32 nHeight);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Rectangle.inl"


#endif // __PLMATH_RECTANGLE_H__
