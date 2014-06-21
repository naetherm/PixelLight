/*********************************************************\
 *  File: Sensor.h                                       *
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


#ifndef __PLPHYSICS_SENSOR_H__
#define __PLPHYSICS_SENSOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLPhysics/Element.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Body;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract PL physics sensor base class
*
*  @remarks
*    Can for instance be used for simple collision detection.
*/
class Sensor : public Element {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*   Flags
		*/
		enum EFlags {
			ClosestBody = 1<<0	/**< A performance hint that we are looking for the closest body */
		};


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Holds information about a detected body
		*/
		struct BodyInfo {
			Body			*pBody;			/**< Detected physics body (always valid!) */
			float			 fDistance;		/**< Distance (if supported by the sensor) */
			PLMath::Vector3  vNormal;		/**< Hit normal (if supported by the sensor) */
			int				 nCollisionID;	/**< Collision ID (if supported by the sensor) */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~Sensor();

		/**
		*  @brief
		*    Returns the flags
		*
		*  @return
		*    Flags (see EFlags)
		*/
		PLPHYSICS_API PLCore::uint32 GetFlags() const;

		/**
		*  @brief
		*    Sets the flags
		*
		*  @param[in] nFlags
		*    Flags (see EFlags)
		*/
		PLPHYSICS_API void SetFlags(PLCore::uint32 nFlags = 0);

		/**
		*  @brief
		*    Clears the list of hit bodies
		*/
		PLPHYSICS_API void ClearHitList();

		/**
		*  @brief
		*    Returns the number of hit physics bodies
		*
		*  @return
		*    The number of hit physics bodies
		*/
		PLPHYSICS_API PLCore::uint32 GetNumOfHitBodies() const;

		/**
		*  @brief
		*    Returns a hit physics body by index
		*
		*  @param[in] nIndex
		*    Index of the hit physics body
		*
		*  @return
		*    The hit physics body information, a null pointer on error (do NOT store a pointer on it!)
		*/
		PLPHYSICS_API const BodyInfo *GetHitBody(PLCore::uint32 nIndex) const;

		/**
		*  @brief
		*    Returns the closest hit physics body
		*
		*  @return
		*    The closest hit physics body information, a null pointer on error (do NOT store a pointer on it!)
		*/
		PLPHYSICS_API const BodyInfo *GetClosestBody() const;


	//[-------------------------------------------------------]
	//[ Public virtual Sensor functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Performs an immediate sensor check
		*
		*  @return
		*    The number of hit physics bodies
		*/
		PLPHYSICS_API virtual PLCore::uint32 Check();


	//[-------------------------------------------------------]
	//[ Public virtual Element functions                      ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API virtual bool IsBody() const override;
		PLPHYSICS_API virtual bool IsJoint() const override;
		PLPHYSICS_API virtual bool IsSensor() const override;


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
		*  @param[in] nFlags
		*    Flags (see EFlags)
		*/
		PLPHYSICS_API Sensor(World &cWorld, PLCore::uint32 nFlags = 0);

		/**
		*  @brief
		*    Returns a free body information
		*
		*  @return
		*    Free body information
		*
		*  @note
		*    - Use FreeBodyInfo() if you no longer need this body information
		*/
		PLPHYSICS_API BodyInfo &GetFreeBodyInfo();

		/**
		*  @brief
		*    Frees a body information
		*
		*  @param[in] cBodyInfo
		*    Body information to free
		*/
		PLPHYSICS_API void FreeBodyInfo(BodyInfo &cBodyInfo);


	//[-------------------------------------------------------]
	//[ Protected virtual Sensor functions                    ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Sensor callback function
		*
		*  @param[in] cBody
		*    Detected physics body
		*  @param[in] fDistance
		*    Distance
		*  @param[in] vNormal
		*    Hit normal
		*  @param[in] nCollisionID
		*    Collision ID
		*
		*  @return
		*    'true' to continue the sensor check, 'false' if stop it right now
		*
		*  @remarks
		*    The default implementation just adds the found body into a result list.
		*/
		PLPHYSICS_API virtual bool Callback(Body &cBody, float fDistance, const PLMath::Vector3 &vNormal, int nCollisionID);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<BodyInfo*>  m_lstBodyInfo;		/**< List of detected body information */
		BodyInfo				 *m_pClosedBodyInfo;	/**< Closest body information, can be a null pointer */
		PLCore::uint32			  m_nFlags;				/**< Flags (see EFlags) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SENSOR_H__
