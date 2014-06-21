/*********************************************************\
 *  File: SensorRaycast.h                                *
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


#ifndef __PLPHYSICS_SENSORRAYCAST_H__
#define __PLPHYSICS_SENSORRAYCAST_H__
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
*    Abstract PL physics ray cast sensor
*/
class SensorRaycast : public Sensor {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~SensorRaycast();

		/**
		*  @brief
		*    Returns the start point
		*
		*  @return
		*    The start point
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetStart() const;

		/**
		*  @brief
		*    Sets the start point
		*
		*  @param[in] vPoint
		*    The new start point
		*/
		PLPHYSICS_API void SetStart(const PLMath::Vector3 &vPoint);

		/**
		*  @brief
		*    Returns the end point
		*
		*  @return
		*    The end point
		*/
		PLPHYSICS_API const PLMath::Vector3 &GetEnd() const;

		/**
		*  @brief
		*    Sets the end point
		*
		*  @param[in] vPoint
		*    The new end point
		*/
		PLPHYSICS_API void SetEnd(const PLMath::Vector3 &vPoint);


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
		*  @param[in] vStart
		*    Beginning of the ray in global space
		*  @param[in] vEnd
		*    End of the ray in global space
		*  @param[in] nFlags
		*    Flags (see EFlags)
		*/
		PLPHYSICS_API SensorRaycast(World &cWorld, const PLMath::Vector3 &vStart = PLMath::Vector3::Zero, const PLMath::Vector3 &vEnd = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLMath::Vector3 m_vStart;	/**< The start point */
		PLMath::Vector3 m_vEnd;		/**< The end point */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SENSORRAYCAST_H__
