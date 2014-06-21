/*********************************************************\
 *  File: AndroidMouseDevice.h                           *
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


#ifndef __PLINPUT_ANDROIDMOUSEDEVICE_H__
#define __PLINPUT_ANDROIDMOUSEDEVICE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Backend/UpdateDevice.h"


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
struct AInputEvent;
typedef struct AInputEvent AInputEvent;
namespace PLInput {
	class AndroidSplitTouchPadDevice;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mouse implementation for Android
*
*  @remarks
*    Mouse emulation by using the touchscreen. When moving around the finger on the touchscreen,
*    the change in movement is used for the mouse axis. A short touch without any movement is handled
*    as "left mouse button clicked". As soon the mouse is moved, no "left mouse button"-events can be
*    generated anymore during the current touch. When touching without movement for half a second, the
*    emulation changes into "left mouse button hold down"-mode, following mouse movement will not change
*    this mode.
*/
class AndroidMouseDevice : public UpdateDevice {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pAndroidSplitTouchPadDevice
		*    Optional Android split touch pad device this device interacts with, can be a null pointer
		*/
		AndroidMouseDevice(AndroidSplitTouchPadDevice *pAndroidSplitTouchPadDevice);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AndroidMouseDevice();

		/**
		*  @brief
		*    Call this to process the next motion input event
		*
		*  @param[in] cAMotionInputEvent
		*    Motion input event to process
		*/
		void OnMotionInputEvent(const struct AInputEvent &cAMotionInputEvent);


	//[-------------------------------------------------------]
	//[ Public virtual UpdateDevice functions                 ]
	//[-------------------------------------------------------]
	public:
		virtual void Update() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AndroidSplitTouchPadDevice *m_pAndroidSplitTouchPadDevice;	/**< Optional Android split touch pad device this device interacts with, can be a null pointer */
		bool						m_bMouseMoved;					/**< Mouse moved during the current touch? */
		float						m_fPreviousMousePositionX;		/**< Previous mouse x position */
		float						m_fPreviousMousePositionY;		/**< Previous mouse y position */
		float						m_fPreviousMousePressure;		/**< Previous mouse pressure */
		float						m_fMousePositionX;				/**< Current mouse x position */
		float						m_fMousePositionY;				/**< Current mouse y position */
		float						m_fMousePressure;				/**< Current mouse pressure */
		bool						m_bLeftMouseButton;				/**< Is the left mouse button currently down? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_ANDROIDMOUSEDEVICE_H__
