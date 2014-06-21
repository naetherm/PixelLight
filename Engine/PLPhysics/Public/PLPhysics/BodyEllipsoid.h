/*********************************************************\
 *  File: BodyEllipsoid.h                                *
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


#ifndef __PLPHYSICS_BODYELLIPSOID_H__
#define __PLPHYSICS_BODYELLIPSOID_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLPhysics/Body.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics ellipsoid body
*
*  @remarks
*    Sphere collision are generalized ellipsoids, the application can create many different kind
*    of objects by just playing with dimensions of the radius. For example to make a sphere set all
*    tree radius to the same value, to make a ellipse of revolution just set two of the tree radius
*    to the same value.\n
*    General ellipsoids are very good hull geometries to represent the outer shell of avatars in a game.
*/
class BodyEllipsoid : public Body {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~BodyEllipsoid();

		/**
		*  @brief
		*    Returns the ellipsoid radius along each axis
		*
		*  @return
		*    The ellipsoid radius along each axis
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetRadius() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    World this body is in
		*  @param[in] cBodyImpl
		*    Reference to the physics API specific body implementation
		*  @param[in] vRadius
		*    Ellipsoid radius along each axis
		*/
		PLPHYSICS_API BodyEllipsoid(World &cWorld, BodyImpl &cBodyImpl, const PLMath::Vector3 &vRadius);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLMath::Vector3 m_vRadius;	/**< Ellipsoid radius along each axis */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_BODYELLIPSOID_H__
