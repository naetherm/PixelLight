/*********************************************************\
 *  File: WorldUpdate.h                                  *
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


#ifndef __PLPHYSICSNEWTON_WORLDUPDATE_H__
#define __PLPHYSICSNEWTON_WORLDUPDATE_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class World;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PL Newton physics world update class
*/
class WorldUpdate {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class World;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    Owner Newton world
		*/
		WorldUpdate(World &cWorld);


	//[-------------------------------------------------------]
	//[ Protected virtual WorldUpdate functions               ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WorldUpdate();

		/**
		*  @brief
		*    Returns whether or not the implementation is using a thread
		*
		*  @return
		*    'true' if the implementation is using a thread, else 'false'
		*/
		virtual bool IsThread();

		/**
		*  @brief
		*    Locks the updated
		*
		*  @return
		*    'true' if successful, 'false' on error
		*/
		virtual bool Lock();

		/**
		*  @brief
		*    Updates the physics
		*
		*  @param[in] fTimeScaleFactor
		*    The time scale factor
		*  @param[in] fMaxTimeDifference
		*    The maximum time difference in seconds
		*  @param[in] fSimulationSpeed
		*    The simulation speed
		*  @param[in] fFrameRate
		*    The frame rate the simulation runs on
		*
		*  @return
		*    'true' if successful, 'false' on error
		*
		*  @note
		*    - It's recommended to call Lock() before, and Unlock() after this function
		*/
		virtual bool Update(float fTimeScaleFactor, float fMaxTimeDifference, float fSimulationSpeed, float fFrameRate);

		/**
		*  @brief
		*    Unlocks the updated
		*
		*  @return
		*    'true' if successful, 'false' on error
		*/
		virtual bool Unlock();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		World		   *m_pWorld;			/**< Owner Newton world (always valid!) */
		PLCore::uint32  m_nTotalNumOfSteps;	/**< Total number of performed physics steps */
		PLCore::uint64  m_nLastTime;		/**< Time the last update was performed in milliseconds */
		float			m_fTimeElapsed;		/**< Elapsed time in seconds */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton


#endif // __PLPHYSICSNEWTON_WORLDUPDATE_H__
