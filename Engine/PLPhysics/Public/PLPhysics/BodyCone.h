/*********************************************************\
 *  File: BodyCone.h                                     *
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


#ifndef __PLPHYSICS_BODYCONE_H__
#define __PLPHYSICS_BODYCONE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    Abstract PL physics cone body
*
*  @note
*    - The cone height must equal of larger than the sum of the cap radius. If this is not
*      the case the height will be clamped the 2 * radius.
*/
class BodyCone : public Body {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~BodyCone();

		/**
		*  @brief
		*    Returns the cone radius at the base
		*
		*  @return
		*    The cone radius at the base
		*/
		PLPHYSICS_API float GetRadius() const;

		/**
		*  @brief
		*    Returns the cone height along the x local axis from base to top
		*
		*  @return
		*    The cone height along the x local axis from base to top
		*/
		PLPHYSICS_API float GetHeight() const;


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
		*  @param[in] fRadius
		*    Cone radius at the base
		*  @param[in] fHeight
		*    Cone height along the x local axis from base to top
		*/
		PLPHYSICS_API BodyCone(World &cWorld, BodyImpl &cBodyImpl, float fRadius, float fHeight);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		float m_fRadius;	/**< Cone radius at the base */
		float m_fHeight;	/**< Cone height along the x local axis from base to top */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_BODYCONE_H__
