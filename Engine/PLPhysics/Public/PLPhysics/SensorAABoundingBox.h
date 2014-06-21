/*********************************************************\
 *  File: SensorAABoundingBox.h                          *
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


#ifndef __PLPHYSICS_SENSORAABOUNDINGBOX_H__
#define __PLPHYSICS_SENSORAABOUNDINGBOX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/Sensor.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics axis aligned bounding box sensor
*/
class SensorAABoundingBox : public Sensor {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SensorAABoundingBox();

		/**
		*  @brief
		*    Returns the minimum point
		*
		*  @return
		*    The minimum point
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetMin() const;

		/**
		*  @brief
		*    Sets the minimum point
		*
		*  @param[in] vMin
		*    The new minimum point
		*/
		PLPHYSICS_API void SetMin(const PLMath::Vector3 &vMin);

		/**
		*  @brief
		*    Returns the maximum point
		*
		*  @return
		*    The maximum point
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetMax() const;

		/**
		*  @brief
		*    Sets the maximum point
		*
		*  @param[in] vMax
		*    The new maximum point
		*/
		PLPHYSICS_API void SetMax(const PLMath::Vector3 &vMax);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    World this sensor is in
		*  @param[in] vMin
		*    Minimum of the axis aligned bounding box in global space
		*  @param[in] vMax
		*    Maximum of the axis aligned bounding box in global space
		*  @param[in] nFlags
		*    Flags (see EFlags)
		*/
		PLPHYSICS_API SensorAABoundingBox(World &cWorld, const PLMath::Vector3 &vMin = PLMath::Vector3::Zero, const PLMath::Vector3 &vMax = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLMath::Vector3 m_vMin;	/**< The minimum point */
		PLMath::Vector3 m_vMax;	/**< The maximum point */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SENSORAABOUNDINGBOX_H__
