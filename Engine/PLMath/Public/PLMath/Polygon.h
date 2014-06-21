/*********************************************************\
 *  File: Polygon.h                                      *
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


#ifndef __PLMATH_POLYGON_H__
#define __PLMATH_POLYGON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include "PLMath/Plane.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A polygon is a set of vertices on a same plane. This is only a base class with
*    vertices... to add texture coords etc. to have to derive it!
*/
class Polygon {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Plane/point relation
		*/
		enum ESide {
			Behind     = -1,	/**< Behind the plane (distance < 0) */
			Coinciding =  0,	/**< Lies on the plane (distance = 0) */
			InFront    =  1,	/**< Is in front of the plane (distance > 0) */
			Spanning   =  2		/**< Spans the plane */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMATH_API Polygon();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Polygon to copy
		*/
		PLMATH_API Polygon(const Polygon &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLMATH_API virtual ~Polygon();

		/**
		*  @brief
		*    Returns the plane the polygon is on
		*
		*  @return
		*    Plane the polygon is on
		*/
		PLMATH_API const Plane &GetPlane() const;

		/**
		*  @brief
		*    Computes and returns the plane the polygon is on
		*
		*  @return
		*    Plane the polygon is on
		*
		*  @note
		*    - For the plane calculation the first three vertices are used
		*/
		PLMATH_API const Plane &ComputePlane();

		/**
		*  @brief
		*    Checks whether all vertices are on the polygon plane or not
		*
		*  @return
		*    'true' if all vertices are on the polygon plane, else 'false'
		*/
		PLMATH_API bool CheckVerticesOnPlane() const;

		/**
		*  @brief
		*    Returns a list of all vertices
		*
		*  @return
		*    List of all vertices
		*
		*  @note
		*    - If vertices are changed to should call the ComputePlane()
		*      function to keep the polygon plane up to date!
		*/
		PLMATH_API PLCore::Array<Vector3> &GetVertexList();

		/**
		*  @brief
		*    Adds a vertex to the polygon
		*
		*  @param[in] vV
		*    Vertex to add
		*
		*  @return
		*    'true' if all went fine and the vertex was added, else 'false'
		*    (maybe the current last vertex is the same as the given one?)
		*/
		PLMATH_API bool AddVertex(const Vector3 &vV);

		/**
		*  @brief
		*    Splits the polygon
		*
		*  @param[in]  cSplitter
		*    Splitter plane
		*  @param[out] cFrontPolygon
		*    Polygon in front of the plane
		*  @param[out] cBackPolygon
		*    Polygon behind the plane
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMATH_API bool Split(const Plane &cSplitter, Polygon &cFrontPolygon, Polygon &cBackPolygon);

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
		PLMATH_API Polygon &operator =(const Polygon &cSource);

		/**
		*  @brief
		*    Calculates the side the polygon is on
		*
		*  @return
		*    Plane side
		*/
		PLMATH_API ESide GetSide(const Plane &cPlane) const;

		/**
		*  @brief
		*    Checks whether the polygon is completely in front of the given plane
		*
		*  @return
		*    'true' if the polygon is completely in front of the given plane, else 'false'
		*/
		PLMATH_API bool IsInFront(const Plane &cPlane) const;

		/**
		*  @brief
		*    Checks whether the polygon is completely behind the given plane
		*
		*  @return
		*    'true' if the polygon is completely behind the given plane, else 'false'
		*/
		PLMATH_API bool IsBehind(const Plane &cPlane) const;

		/**
		*  @brief
		*    Clips the polygon against a plane
		*
		*  @param[in] cPlane
		*    Plane to clip this polygon against
		*
		*  @return
		*    The resulting clipped plane
		*/
		PLMATH_API Polygon Clip(const Plane &cPlane) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<Vector3> m_lstVertices;	/**< The vertices (xyz) */
		Plane				   m_cPlane;		/**< Plane the polygon is on */


	//[-------------------------------------------------------]
	//[ Public virtual Polygon functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Initializes the data of the splitted polygon
		*
		*  @param[out] cPolygon
		*    Polygon which should be initialized
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMATH_API virtual bool CustomInit(Polygon &cPolygon);

		/**
		*  @brief
		*    Adds data to a polygon
		*
		*  @param[out] cPolygon
		*    Polygon the data should be added
		*  @param[in]  nID
		*    Current data ID
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Is called by Split() to get an custom split behavior... a
		*      derived polygon could e.g. also have texture coordinates to be split or added!
		*/
		PLMATH_API virtual bool CustomAdd(Polygon &cPolygon, PLCore::uint32 nID);

		/**
		*  @brief
		*    Splits the polygon using the custom function
		*
		*  @param[out] cFrontPolygon
		*    Polygon in front of the plane
		*  @param[out] cBackPolygon
		*    Polygon behind the plane
		*  @param[in]  nID
		*    Current data ID
		*  @param[in]  fDistance
		*    Distance from nID-1 to the splitter plane intersection point
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - CustomAdd()
		*/
		PLMATH_API virtual bool CustomSplit(Polygon &cFrontPolygon, Polygon &cBackPolygon, PLCore::uint32 nID, float fDistance);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_POLYGON_H__
