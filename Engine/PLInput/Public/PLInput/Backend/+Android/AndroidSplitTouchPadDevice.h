/*********************************************************\
 *  File: AndroidSplitTouchPadDevice.h                   *
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


#ifndef __PLINPUT_ANDROIDSPLITTOUCHPADDEVICE_H__
#define __PLINPUT_ANDROIDSPLITTOUCHPADDEVICE_H__
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Android gamepad device emulation by using a touch screen making it possible to e.g. move & look at the same time
*/
class AndroidSplitTouchPadDevice : public UpdateDevice {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		AndroidSplitTouchPadDevice();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AndroidSplitTouchPadDevice();

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
		// Left
		int   m_nLeftTouchPointerID;		/**< Left screen touch pointer ID, -1 if none */
		float m_fOriginLeftTouchPositionX;	/**< Origin left touch x position */
		float m_fOriginLeftTouchPositionY;	/**< Origin left touch y position */
		float m_fLeftTouchPositionX;		/**< Current left touch x position */
		float m_fLeftTouchPositionY;		/**< Current left touch y position */
		// Right
		int   m_nRightTouchPointerID;		/**< Right screen touch pointer ID, -1 if none */
		float m_fOriginRightTouchPositionX;	/**< Origin right touch x position */
		float m_fOriginRightTouchPositionY;	/**< Origin right touch y position */
		float m_fRightTouchPositionX;		/**< Current right touch x position */
		float m_fRightTouchPositionY;		/**< Current right touch y position */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_ANDROIDSPLITTOUCHPADDEVICE_H__
