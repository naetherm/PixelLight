/*********************************************************\
 *  File: AndroidSplitTouchPadDevice.cpp                 *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <android/input.h>
#include <android_native_app_glue.h>
#include <PLMath/Math.h>
#include <PLCore/System/System.h>
#include <PLCore/System/SystemAndroid.h>
#include "PLInput/Input/Devices/SplitTouchPad.h"
#include "PLInput/Backend/Android/AndroidSplitTouchPadDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AndroidSplitTouchPadDevice::AndroidSplitTouchPadDevice() :
	// Left
	m_nLeftTouchPointerID(-1),
	m_fOriginLeftTouchPositionX(0.0f),
	m_fOriginLeftTouchPositionY(0.0f),
	m_fLeftTouchPositionX(0.0f),
	m_fLeftTouchPositionY(0.0f),
	// Right
	m_nRightTouchPointerID(-1),
	m_fOriginRightTouchPositionX(0.0f),
	m_fOriginRightTouchPositionY(0.0f),
	m_fRightTouchPositionX(0.0f),
	m_fRightTouchPositionY(0.0f)
{
	// Destroy device implementation automatically
	m_bDelete = true;
}

/**
*  @brief
*    Destructor
*/
AndroidSplitTouchPadDevice::~AndroidSplitTouchPadDevice()
{
}

/**
*  @brief
*    Call this to process the next motion input event
*/
void AndroidSplitTouchPadDevice::OnMotionInputEvent(const struct AInputEvent &cAMotionInputEvent)
{
	// We need the display size and orientation for splitting the screen
	struct android_app *pAndroidApp = reinterpret_cast<SystemAndroid*>(System::GetInstance())->GetAndroidApp();
	if (pAndroidApp) {
		ANativeWindow *pANativeWindow = pAndroidApp->window;
		if (pANativeWindow) {
			// Get the screen width: This is our base line for splitting
			const int32_t nScreenWidth = ANativeWindow_getWidth(pANativeWindow);

			// Get the number of pointers of data contained in this event
			const size_t nAndroidPointerCount = AMotionEvent_getPointerCount(&cAMotionInputEvent);

			// Evaluate every point
			for (size_t i=0; i<nAndroidPointerCount; i++) {
				size_t nAndroidPointerID    = AMotionEvent_getPointerId(&cAMotionInputEvent, i);
				size_t nAndroidAction       = (AMotionEvent_getAction(&cAMotionInputEvent) & AMOTION_EVENT_ACTION_MASK);
				size_t nAndroidPointerIndex = i;
				if (nAndroidAction == AMOTION_EVENT_ACTION_POINTER_DOWN || nAndroidAction == AMOTION_EVENT_ACTION_POINTER_UP) {
					nAndroidPointerIndex = (AMotionEvent_getAction(&cAMotionInputEvent) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
					nAndroidPointerID    = AMotionEvent_getPointerId(&cAMotionInputEvent, nAndroidPointerIndex);
				}

				// Get the current X and Y coordinate of this event for the given pointer index
				const float fPointerPositionX = AMotionEvent_getX(&cAMotionInputEvent, nAndroidPointerIndex);
				const float fPointerPositionY = AMotionEvent_getY(&cAMotionInputEvent, nAndroidPointerIndex);

				// Does the touch start?
				if (nAndroidAction == AMOTION_EVENT_ACTION_DOWN || nAndroidAction == AMOTION_EVENT_ACTION_POINTER_DOWN) {
					// Decide if the point is in the left or right screen half
					if (fPointerPositionX < (nScreenWidth/2)) {
						// This is the on the left half of the screen
						if (m_nLeftTouchPointerID == -1) {
							// This is our initial start point (origin position = current position)
							m_fOriginLeftTouchPositionX = m_fLeftTouchPositionX = fPointerPositionX;
							m_fOriginLeftTouchPositionY = m_fLeftTouchPositionY = fPointerPositionY;

							// We're now active, save the ID of this pointer
							m_nLeftTouchPointerID = nAndroidPointerID;
						}
					} else {
						// This is on the right half of the screen
						if (m_nRightTouchPointerID == -1) {
							// This is our initial start point (origin position = current position)
							m_fOriginRightTouchPositionX = m_fRightTouchPositionX = fPointerPositionX;
							m_fOriginRightTouchPositionY = m_fRightTouchPositionY = fPointerPositionY;

							// We're now active, save the ID of this pointer
							m_nRightTouchPointerID = nAndroidPointerID;
						}
					}

				// Does the touch stop?
				} else if (nAndroidAction == AMOTION_EVENT_ACTION_UP || nAndroidAction == AMOTION_EVENT_ACTION_POINTER_UP) {
					// Use the pointer ID to figure out whether this is our left or right pointer
					if (m_nLeftTouchPointerID == nAndroidPointerID) {
						// We're now longer active
						m_nLeftTouchPointerID = -1;

						// Let the left simulated pad stick snap back to it's origin
						SplitTouchPad *pSplitTouchPad = static_cast<SplitTouchPad*>(m_pDevice);
						if (pSplitTouchPad) {
							pSplitTouchPad->LeftX.SetValue(0.0f, false);
							pSplitTouchPad->LeftY.SetValue(0.0f, false);
						}
						m_fLeftTouchPositionX = m_fOriginLeftTouchPositionX;
						m_fLeftTouchPositionY = m_fOriginLeftTouchPositionY;

					} else if (m_nRightTouchPointerID == nAndroidPointerID) {
						// We're now longer active
						m_nRightTouchPointerID = -1;

						// Let the right simulated pad stick snap back to it's origin
						SplitTouchPad *pSplitTouchPad = static_cast<SplitTouchPad*>(m_pDevice);
						if (pSplitTouchPad) {
							pSplitTouchPad->RightX.SetValue(0.0f, false);
							pSplitTouchPad->RightY.SetValue(0.0f, false);
						}
						m_fRightTouchPositionX = m_fOriginRightTouchPositionX;
						m_fRightTouchPositionY = m_fOriginRightTouchPositionY;
					}

				// Did we move?
				} else if (nAndroidAction == AMOTION_EVENT_ACTION_MOVE) {
					// Use the pointer ID to figure out whether this is our left or right pointer
					if (m_nLeftTouchPointerID == nAndroidPointerID) {
						m_fLeftTouchPositionX = fPointerPositionX;
						m_fLeftTouchPositionY = fPointerPositionY;

					} else if (m_nRightTouchPointerID == nAndroidPointerID) {
						m_fRightTouchPositionX = fPointerPositionX;
						m_fRightTouchPositionY = fPointerPositionY;
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void AndroidSplitTouchPadDevice::Update()
{
	// Check if input device is valid
	if (m_pDevice) {
		// Get split touch pad device
		SplitTouchPad *pSplitTouchPad = static_cast<SplitTouchPad*>(m_pDevice);

		// Maximum allowed delta
		static const float MaxDelta = 160.0f;
		static const float MinDelta = 5.0f;

		// Update left axes?
		if (m_nLeftTouchPointerID != -1) {
			// Get the left touch movement and clamp it to the maximum allowed delta
			float fLeftDeltaX = m_fLeftTouchPositionX - m_fOriginLeftTouchPositionX;
			float fLeftDeltaY = m_fLeftTouchPositionY - m_fOriginLeftTouchPositionY;
			if (fLeftDeltaX > MaxDelta)
				fLeftDeltaX = MaxDelta;
			if (fLeftDeltaX < -MaxDelta)
				fLeftDeltaX = -MaxDelta;
			if (fLeftDeltaY > MaxDelta)
				fLeftDeltaY = MaxDelta;
			if (fLeftDeltaY < -MaxDelta)
				fLeftDeltaY = -MaxDelta;

			// Give our fat finger some space to sit down :D
			if (Math::Abs(fLeftDeltaX) < MinDelta)
				fLeftDeltaX = 0.0f;
			else
				fLeftDeltaX -= MinDelta*Math::Sign(fLeftDeltaX);
			if (Math::Abs(fLeftDeltaY) < MinDelta)
				fLeftDeltaY = 0.0f;
			else
				fLeftDeltaY -= MinDelta*Math::Sign(fLeftDeltaY);

			// Now update left axes
			if (pSplitTouchPad->LeftX.GetValue() != fLeftDeltaX)
				pSplitTouchPad->LeftX.SetValue(fLeftDeltaX, false);
			if (pSplitTouchPad->LeftY.GetValue() != fLeftDeltaY)
				pSplitTouchPad->LeftY.SetValue(fLeftDeltaY, false);
		}

		// Update right axes?
		if (m_nRightTouchPointerID != -1) {
			// Get the right touch movement and clamp it to the maximum allowed delta
			float fRightDeltaX = m_fRightTouchPositionX - m_fOriginRightTouchPositionX;
			float fRightDeltaY = m_fRightTouchPositionY - m_fOriginRightTouchPositionY;
			if (fRightDeltaX > MaxDelta)
				fRightDeltaX = MaxDelta;
			if (fRightDeltaX < -MaxDelta)
				fRightDeltaX = -MaxDelta;
			if (fRightDeltaY > MaxDelta)
				fRightDeltaY = MaxDelta;
			if (fRightDeltaY < -MaxDelta)
				fRightDeltaY = -MaxDelta;

			// Give our fat finger some space to sit down :D
			if (Math::Abs(fRightDeltaX) < MinDelta)
				fRightDeltaX = 0.0f;
			else
				fRightDeltaX -= MinDelta*Math::Sign(fRightDeltaX);
			if (Math::Abs(fRightDeltaY) < MinDelta)
				fRightDeltaY = 0.0f;
			else
				fRightDeltaY -= MinDelta*Math::Sign(fRightDeltaY);

			// Now update right axes
			if (pSplitTouchPad->RightX.GetValue() != fRightDeltaX)
				pSplitTouchPad->RightX.SetValue(fRightDeltaX, false);
			if (pSplitTouchPad->RightY.GetValue() != fRightDeltaY)
				pSplitTouchPad->RightY.SetValue(fRightDeltaY, false);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
