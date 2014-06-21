/*********************************************************\
 *  File: WorldThread.h                                  *
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


#ifndef __PLNEWTON_WORLDTHREAD_H__
#define __PLNEWTON_WORLDTHREAD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/System/Thread.h>
#include "PLPhysicsNewton/WorldUpdate.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Mutex;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysicsNewton {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PL Newton physics world update thread
*/
class WorldThread : public WorldUpdate, public PLCore::Thread {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class World;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cWorld
		*    Owner Newton world
		*/
		WorldThread(World &cWorld);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WorldThread();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Mutex  *m_pMutex;				/**< Synchronization object (always valid!) */
		volatile bool	m_bShutDown;			/**< Shut down this thread? */
		volatile float	m_fTimeScaleFactor;		/**< Cached time scale factor */
		volatile float	m_fMaxTimeDifference;	/**< Cached maximum time difference */
		volatile float	m_fSimulationSpeed;		/**< Cached simulation speed */
		volatile float	m_fFrameRate;			/**< Cached simulation frame frame */


	//[-------------------------------------------------------]
	//[ Private virtual WorldUpdate functions                 ]
	//[-------------------------------------------------------]
	private:
		virtual bool IsThread() override;
		virtual bool Lock() override;
		virtual bool Update(float fTimeScaleFactor, float fMaxTimeDifference, float fSimulationSpeed, float fFrameRate) override;
		virtual bool Unlock() override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Thread functions               ]
	//[-------------------------------------------------------]
	public:
		virtual int Run() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysicsNewton


#endif // __PLNEWTON_WORLDTHREAD_H__
