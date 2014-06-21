/*********************************************************\
 *  File: AndroidKeyboardDevice.cpp                      *
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
#include "PLInput/Input/Devices/Keyboard.h"
#include "PLInput/Backend/Android/AndroidKeyboardDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AndroidKeyboardDevice::AndroidKeyboardDevice()
{
	// Destroy device implementation automatically
	m_bDelete = true;
}

/**
*  @brief
*    Destructor
*/
AndroidKeyboardDevice::~AndroidKeyboardDevice()
{
}

/**
*  @brief
*    Call this to process the next key input event
*/
void AndroidKeyboardDevice::OnKeyInputEvent(const struct AInputEvent &cAKeyInputEvent)
{
	// Check if input device is valid
	if (m_pDevice) {
		// Get Android key code
		const int32_t nKeyCode = AKeyEvent_getKeyCode(&cAKeyInputEvent);

		// Lookout! The virtual keyboard of Android sends "down" and "up" directly one after another
		// -> This is really a problem and we have to delay further keys...
		if (m_lstProcessedKeys.IsElement(nKeyCode)) {
			// Add key for later processing
			KeyInfo sKeyInfo;
			sKeyInfo.nKeyCode = nKeyCode;
			sKeyInfo.bPressed = (AKeyEvent_getAction(&cAKeyInputEvent) == AKEY_EVENT_ACTION_DOWN);
			m_lstDelayedKeys.Add(sKeyInfo);
		} else {
			// Get keyboard device
			Keyboard *pKeyboard = static_cast<Keyboard*>(m_pDevice);

			// Get button
			Button *pButton = GetKeyboardKey(*pKeyboard, nKeyCode);
			if (pButton) {
				// Get button state
				const bool bPressed = (AKeyEvent_getAction(&cAKeyInputEvent) == AKEY_EVENT_ACTION_DOWN);

				// Propagate changes
				if (pButton->IsPressed() != bPressed)
					pButton->SetPressed(bPressed);
			}

			// Add this key to the processed keys
			m_lstProcessedKeys.Add(nKeyCode);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void AndroidKeyboardDevice::Update()
{
	// Check if input device is valid
	if (m_pDevice) {
		// Get keyboard device
		Keyboard *pKeyboard = static_cast<Keyboard*>(m_pDevice);

		// Process delayed keys
		for (uint32 i=0; i<m_lstDelayedKeys.GetNumOfElements(); i++) {
			const KeyInfo &sKeyInfo = m_lstDelayedKeys[i];

			// Get button
			Button *pButton = GetKeyboardKey(*pKeyboard, sKeyInfo.nKeyCode);
			if (pButton) {
				// Get button state
				const bool bPressed = sKeyInfo.bPressed;

				// Propagate changes
				if (pButton->IsPressed() != bPressed)
					pButton->SetPressed(bPressed);
			}
		}
		m_lstDelayedKeys.Clear();
		m_lstProcessedKeys.Clear();
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get key for Android key code
*/
Button *AndroidKeyboardDevice::GetKeyboardKey(Keyboard &cKeyboard, int32_t nKeyCode)
{
	// Return key that corresponds to the given Android key code
	switch (nKeyCode) {
		// AKEYCODE_SOFT_LEFT			-> ignored
		// AKEYCODE_SOFT_RIGHT			-> ignored
		case AKEYCODE_HOME:				return &cKeyboard.Home;
		case AKEYCODE_BACK:				return &cKeyboard.Backspace;
		// AKEYCODE_CALL				-> ignored
		// AKEYCODE_ENDCAL				-> ignored
		case AKEYCODE_0:				return &cKeyboard.Key0;
		case AKEYCODE_1:				return &cKeyboard.Key1;
		case AKEYCODE_2:				return &cKeyboard.Key2;
		case AKEYCODE_3:				return &cKeyboard.Key3;
		case AKEYCODE_4:				return &cKeyboard.Key4;
		case AKEYCODE_5:				return &cKeyboard.Key5;
		case AKEYCODE_6:				return &cKeyboard.Key6;
		case AKEYCODE_7:				return &cKeyboard.Key7;
		case AKEYCODE_8:				return &cKeyboard.Key8;
		case AKEYCODE_9:				return &cKeyboard.Key9;
		// AKEYCODE_STAR				-> ignored
		// AKEYCODE_POUND				-> ignored
		case AKEYCODE_DPAD_UP:			return &cKeyboard.Up;
		case AKEYCODE_DPAD_DOWN:		return &cKeyboard.Down;
		case AKEYCODE_DPAD_LEFT:		return &cKeyboard.Left;
		case AKEYCODE_DPAD_RIGHT:		return &cKeyboard.Right;
		// AKEYCODE_DPAD_CENTER			-> ignored
		// AKEYCODE_VOLUME_UP			-> ignored
		// AKEYCODE_VOLUME_DOWN			-> ignored
		// AKEYCODE_POWER				-> ignored
		// AKEYCODE_CAMERA				-> ignored
		case AKEYCODE_CLEAR:			return &cKeyboard.Clear;
		case AKEYCODE_A:				return &cKeyboard.A;
		case AKEYCODE_B:				return &cKeyboard.B;
		case AKEYCODE_C:				return &cKeyboard.C;
		case AKEYCODE_D:				return &cKeyboard.D;
		case AKEYCODE_E:				return &cKeyboard.E;
		case AKEYCODE_F:				return &cKeyboard.F;
		case AKEYCODE_G:				return &cKeyboard.G;
		case AKEYCODE_H:				return &cKeyboard.H;
		case AKEYCODE_I:				return &cKeyboard.I;
		case AKEYCODE_J:				return &cKeyboard.J;
		case AKEYCODE_K:				return &cKeyboard.K;
		case AKEYCODE_L:				return &cKeyboard.L;
		case AKEYCODE_M:				return &cKeyboard.M;
		case AKEYCODE_N:				return &cKeyboard.N;
		case AKEYCODE_O:				return &cKeyboard.O;
		case AKEYCODE_P:				return &cKeyboard.P;
		case AKEYCODE_Q:				return &cKeyboard.Q;
		case AKEYCODE_R:				return &cKeyboard.R;
		case AKEYCODE_S:				return &cKeyboard.S;
		case AKEYCODE_T:				return &cKeyboard.T;
		case AKEYCODE_U:				return &cKeyboard.U;
		case AKEYCODE_V:				return &cKeyboard.V;
		case AKEYCODE_W:				return &cKeyboard.W;
		case AKEYCODE_X:				return &cKeyboard.X;
		case AKEYCODE_Y:				return &cKeyboard.Y;
		case AKEYCODE_Z:				return &cKeyboard.Z;
		// AKEYCODE_COMMA				-> ignored
		// AKEYCODE_PERIOD				-> ignored
		case AKEYCODE_ALT_LEFT:			return &cKeyboard.Alt;
		case AKEYCODE_ALT_RIGHT:		return &cKeyboard.Control;
		case AKEYCODE_SHIFT_LEFT:		return &cKeyboard.Shift;
		// AKEYCODE_SHIFT_RIGHT			-> ignored
		case AKEYCODE_TAB:				return &cKeyboard.Tab;
		case AKEYCODE_SPACE:			return &cKeyboard.Space;
		// AKEYCODE_SYM					-> ignored
		// AKEYCODE_EXPLORER			-> ignored
		// AKEYCODE_ENVELOPE			-> ignored
		case AKEYCODE_ENTER:			return &cKeyboard.Return;
		case AKEYCODE_DEL:				return &cKeyboard.Delete;
		// AKEYCODE_GRAVE				-> ignored
		case AKEYCODE_MINUS:			return &cKeyboard.NumpadSubtract;
		// AKEYCODE_EQUALS				-> ignored
		// AKEYCODE_LEFT_BRACKET		-> ignored
		// AKEYCODE_RIGHT_BRACKET		-> ignored
		// AKEYCODE_BACKSLASH			-> ignored
		// AKEYCODE_SEMICOLON			-> ignored
		// AKEYCODE_APOSTROPHE			-> ignored
		// AKEYCODE_SLASH				-> ignored
		// AKEYCODE_AT					-> ignored
		// AKEYCODE_NUM					-> ignored
		// AKEYCODE_HEADSETHOOK			-> ignored
		// AKEYCODE_FOCUS				-> ignored
		case AKEYCODE_PLUS:				return &cKeyboard.NumpadAdd;
		// AKEYCODE_MENU				-> ignored
		// AKEYCODE_NOTIFICATION		-> ignored
		// AKEYCODE_SEARCH				-> ignored
		case AKEYCODE_MEDIA_PLAY_PAUSE:	return &cKeyboard.Pause;
		// AKEYCODE_MEDIA_STOP			-> ignored
		// AKEYCODE_MEDIA_NEXT			-> ignored
		// AKEYCODE_MEDIA_PREVIOUS		-> ignored
		// AKEYCODE_MEDIA_REWIND		-> ignored
		// AKEYCODE_MEDIA_FAST_FORWARD	-> ignored
		// AKEYCODE_MUTE				-> ignored
		case AKEYCODE_PAGE_UP:			return &cKeyboard.PageUp;
		case AKEYCODE_PAGE_DOWN:		return &cKeyboard.PageDown;
		// AKEYCODE_PICTSYMBOLS			-> ignored
		// AKEYCODE_SWITCH_CHARSET		-> ignored
		case AKEYCODE_BUTTON_A:			return &cKeyboard.A;
		case AKEYCODE_BUTTON_B:			return &cKeyboard.B;
		case AKEYCODE_BUTTON_C:			return &cKeyboard.C;
		case AKEYCODE_BUTTON_X:			return &cKeyboard.X;
		case AKEYCODE_BUTTON_Y:			return &cKeyboard.Y;
		case AKEYCODE_BUTTON_Z:			return &cKeyboard.Z;
		// AKEYCODE_BUTTON_L1			-> ignored
		// AKEYCODE_BUTTON_R1			-> ignored
		// AKEYCODE_BUTTON_L2			-> ignored
		// AKEYCODE_BUTTON_R2			-> ignored
		// AKEYCODE_BUTTON_THUMBL		-> ignored
		// AKEYCODE_BUTTON_THUMBR		-> ignored
		// AKEYCODE_BUTTON_START		-> ignored
		case AKEYCODE_BUTTON_SELECT:	return &cKeyboard.Select;
		// AKEYCODE_BUTTON_MODE			-> ignored
		default:						return nullptr;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
