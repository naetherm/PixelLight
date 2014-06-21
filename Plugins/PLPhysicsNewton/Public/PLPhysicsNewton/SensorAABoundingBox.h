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


#ifndef __PLPHYSICSNEWTON_SENSORAABOUNDINGBOX_H__
#define __PLPHYSICSNEWTON_SENSORAABOUNDINGBOX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLPhysics/SensorAABoundingBox.h>
#include "PLPhysicsNewton/PLPhysicsNewton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Newton physics axis aligned bounding box sensor
*/
class SensorAABoundingBox : public PLPhysics::SensorAABoundingBox {


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
		PLPHYSICSNEWTON_API virtual ~SensorAABoundingBox();


	//[-------------------------------------------------------]
	//[ Public Newton callback functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Newton function to be called for each body hit during the axis aligned bounding box check
		*
		*  @param[in] pPhysicsBody
		*    Newton body (always valid!)
		*  @param[in] pUserData
		*    User data, can be a null pointer
		*/
		static PLPHYSICSNEWTON_API void AABoundingBoxIteratorCallback(const Newton::NewtonBody *pPhysicsBody, void *pUserData);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
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
		SensorAABoundingBox(PLPhysics::World &cWorld, const PLMath::Vector3 &vMin = PLMath::Vector3::Zero, const PLMath::Vector3 &vMax = PLMath::Vector3::Zero, PLCore::uint32 nFlags = 0);


	//[-------------------------------------------------------]
	//[ Private static Newton callback variable               ]
	//[-------------------------------------------------------]
	private:
		static SensorAABoundingBox *m_pCurrentSensor;	/**< Totally ugly, but we have no change to pass the sensor instance by parameter to the Newton iterator callback function */


	//[-------------------------------------------------------]
	//[ Public virtual PLPhysics::Sensor functions            ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICSNEWTON_API virtual PLCore::uint32 Check() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton


#endif // __PLPHYSICSNEWTON_SENSORAABOUNDINGBOX_H__
