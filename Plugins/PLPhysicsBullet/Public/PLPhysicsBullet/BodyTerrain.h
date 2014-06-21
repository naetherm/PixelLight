/*********************************************************\
 *  File: BodyTerrain.h                                  *
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


#ifndef __PLPHYSICSBULLET_BODYTERRAIN_H__
#define __PLPHYSICSBULLET_BODYTERRAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLPhysics/BodyTerrain.h>
#include "PLPhysicsBullet/PLPhysicsBullet.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsBullet {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Bullet physics terrain body implementation
*/
class BodyTerrain : public PLPhysics::BodyTerrain {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class World;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICSBULLET_API virtual ~BodyTerrain();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    World this body is in
		*  @param[in] nWidth
		*    Terrain data width
		*  @param[in] nHeight
		*    Terrain data height
		*  @param[in] fTerrain
		*    Terrain data
		*  @param[in] vBoxMin
		*    Bounding box minimum
		*  @param[in] vBoxMax
		*    Bounding box maximum
		*  @param[in] vScale
		*    Scale
		*/
		BodyTerrain(PLPhysics::World &cWorld, PLCore::uint32 nWidth, PLCore::uint32 nHeight,
					const float fTerrain[], const PLMath::Vector3 &vBoxMin, const PLMath::Vector3 &vBoxMax,
					const PLMath::Vector3 &vScale);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Private data
		PLCore::uint32     m_nWidth;	/**< Terrain data width */
		PLCore::uint32     m_nHeight;	/**< Terrain data height */
		const float		  *m_pfTerrain;	/**< Terrain data (always valid, do NOT destroy the data itself, it's only shared) */
		PLMath::Vector3	   m_vBoxMin;	/**< Bounding box minimum */
		PLMath::Vector3	   m_vBoxMax;	/**< Bounding box maximum */
		PLMath::Vector3	   m_vScale;	/**< Scale */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsBullet


#endif // __PLPHYSICSBULLET_BODYTERRAIN_H__
