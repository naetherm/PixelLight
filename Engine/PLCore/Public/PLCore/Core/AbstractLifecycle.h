/*********************************************************\
 *  File: AbstractLifecycle.h                            *
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


#ifndef __PLCORE_ABSTRACTLIFECYCLE_H__
#define __PLCORE_ABSTRACTLIFECYCLE_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract life cycle class
*
*  @remarks
*  @verbatim
*    The "Life cycle" is:
*      "OnCreate()"         - Called directly after the object has been created
*        ("OnRestart()")    - Called directly before a stopped object is going to start again (always followed by "OnStart()")
*        "OnStart()"        - Called when the object becoming visible to the user
*          "OnResume()"     - Called when the object has the focus (keep the implementation lightweight)
*          "OnPause()"      - Called when the object has no longer the focus (keep the implementation lightweight)
*        "OnStop()"         - Called when the object is no longer visible to the user
*      "OnDestroy()"        - Called before the object is going to be finally destroyed
*    This life cycle has the same interface as the "Android Activity Life cycle"
*    (http://developer.android.com/reference/android/app/Activity.html#ActivityLifecycle)
*    because this interface looks just the right way to go for this purpose... also there
*    are nice diagrams and documentations explaining the interface so it shouldn't be to
*    hard to understand it.
*  @endverbatim
*/
class AbstractLifecycle {


	//[-------------------------------------------------------]
	//[ Public virtual AbstractLifecycle functions            ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called directly after the object has been created
		*/
		virtual void OnCreate() = 0;

		/**
		*  @brief
		*    Called directly before a stopped object is going to start again (always followed by "OnStart()")
		*/
		virtual void OnRestart() = 0;

		/**
		*  @brief
		*    Called when the object becoming visible to the user
		*
		*  @return
		*    'true' if all went fine, else 'false' (on failure, no "OnResume()", "OnPause()" or "OnStop()" will be called)
		*/
		virtual bool OnStart() = 0;

		/**
		*  @brief
		*    Called when the object has the focus (keep the implementation lightweight)
		*/
		virtual void OnResume() = 0;

		/**
		*  @brief
		*    Called when the object has no longer the focus (keep the implementation lightweight)
		*/
		virtual void OnPause() = 0;

		/**
		*  @brief
		*    Called when the object is no longer visible to the user
		*/
		virtual void OnStop() = 0;

		/**
		*  @brief
		*    Called before the object is going to be finally destroyed
		*/
		virtual void OnDestroy() = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		AbstractLifecycle();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AbstractLifecycle();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		AbstractLifecycle(const AbstractLifecycle &cSource);

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
		AbstractLifecycle &operator =(const AbstractLifecycle &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_ABSTRACTLIFECYCLE_H__
