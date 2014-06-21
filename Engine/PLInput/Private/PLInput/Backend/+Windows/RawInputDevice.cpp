/*********************************************************\
 *  File: RawInputDevice.cpp                             *
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
#include <PLCore/Core/MemoryManager.h>
#include "PLInput/Input/Controls/Button.h"
#include "PLInput/Input/Controls/Axis.h"
#include "PLInput/Input/Devices/Keyboard.h"
#include "PLInput/Input/Devices/Mouse.h"
#include "PLInput/Backend/+Windows/RawInputDevice.h"


//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Virtual keys
*/
enum EKey {
	KeyBackspace			= 0x08,		/**< Backspace */
	KeyTab					= 0x09,		/**< Tab */
	KeyClear				= 0x0C,		/**< Clear (not available everywhere) */
	KeyReturn				= 0x0D,		/**< Return (often the same as "Enter") */
	KeyShift				= 0x10,		/**< Shift */
	KeyControl				= 0x11,		/**< Control ("Ctrl") */
	KeyAlt					= 0x12,		/**< Alt */
	KeyPause				= 0x13,		/**< Pause */
	KeyCapsLock				= 0x14,		/**< Caps lock */
	KeyEscape				= 0x1B,		/**< Escape */
	KeySpace				= 0x20,		/**< Space */
	KeyPageUp				= 0x21,		/**< Page up */
	KeyPageDown				= 0x22,		/**< Page down */
	KeyEnd					= 0x23,		/**< End */
	KeyHome					= 0x24,		/**< Home */
	KeyLeft					= 0x25,		/**< Left arrow */
	KeyUp					= 0x26,		/**< Up arrow */
	KeyRight				= 0x27,		/**< Right arrow */
	KeyDown					= 0x28,		/**< Down arrow */
	KeySelect				= 0x29,		/**< Select (not available everywhere) */
	KeyExecute				= 0x2B,		/**< Execute (not available everywhere) */
	KeyPrint				= 0x2C,		/**< Print screen */
	KeyInsert				= 0x2D,		/**< Insert */
	KeyDelete				= 0x2E,		/**< Delete */
	KeyHelp					= 0x2F,		/**< Help (not available everywhere) */
	Key0					= 0x30,		/**< 0 */
	Key1					= 0x31,		/**< 1 */
	Key2					= 0x32,		/**< 2 */
	Key3					= 0x33,		/**< 3 */
	Key4					= 0x34,		/**< 4 */
	Key5					= 0x35,		/**< 5 */
	Key6					= 0x36,		/**< 6 */
	Key7					= 0x37,		/**< 7 */
	Key8					= 0x38,		/**< 8 */
	Key9					= 0x39,		/**< 9 */
	KeyA					= 0x41,		/**< A */
	KeyB					= 0x42,		/**< B */
	KeyC					= 0x43,		/**< C */
	KeyD					= 0x44,		/**< D */
	KeyE					= 0x45,		/**< E */
	KeyF					= 0x46,		/**< F */
	KeyG					= 0x47,		/**< G */
	KeyH					= 0x48,		/**< H */
	KeyI					= 0x49,		/**< I */
	KeyJ					= 0x4A,		/**< J */
	KeyK					= 0x4B,		/**< K */
	KeyL					= 0x4C,		/**< L */
	KeyM					= 0x4D,		/**< M */
	KeyN					= 0x4E,		/**< N */
	KeyO					= 0x4F,		/**< O */
	KeyP					= 0x50,		/**< P */
	KeyQ					= 0x51,		/**< Q */
	KeyR					= 0x52,		/**< R */
	KeyS					= 0x53,		/**< S */
	KeyT					= 0x54,		/**< T */
	KeyU					= 0x55,		/**< U */
	KeyV					= 0x56,		/**< V */
	KeyW					= 0x57,		/**< W */
	KeyX					= 0x58,		/**< X */
	KeyY					= 0x59,		/**< Y */
	KeyZ					= 0x5A,		/**< Z */
	KeyNumpad0				= 0x60,		/**< Numeric keypad 0 */
	KeyNumpad1				= 0x61,		/**< Numeric keypad 1 */
	KeyNumpad2				= 0x62,		/**< Numeric keypad 2 */
	KeyNumpad3				= 0x63,		/**< Numeric keypad 3 */
	KeyNumpad4				= 0x64,		/**< Numeric keypad 4 */
	KeyNumpad5				= 0x65,		/**< Numeric keypad 5 */
	KeyNumpad6				= 0x66,		/**< Numeric keypad 6 */
	KeyNumpad7				= 0x67,		/**< Numeric keypad 7 */
	KeyNumpad8				= 0x68,		/**< Numeric keypad 8 */
	KeyNumpad9				= 0x69,		/**< Numeric keypad 9 */
	KeyNumpadMultiply		= 0x6A,		/**< Numeric keypad "Multiply" */
	KeyNumpadAdd			= 0x6B,		/**< Numeric keypad "Add" */
	KeyNumpadSeparator		= 0x6C,		/**< Numeric keypad "Separator" */
	KeyNumpadSubtract		= 0x6D,		/**< Numeric keypad "Subtract" */
	KeyNumpadDecimal		= 0x6E,		/**< Numeric keypad "Decimal" */
	KeyNumpadDivide			= 0x6F,		/**< Numeric keypad "Divide" */
	KeyF1					= 0x70,		/**< F1 */
	KeyF2					= 0x71,		/**< F2 */
	KeyF3					= 0x72,		/**< F3 */
	KeyF4					= 0x73,		/**< F4 */
	KeyF5					= 0x74,		/**< F5 */
	KeyF6					= 0x75,		/**< F6 */
	KeyF7					= 0x76,		/**< F7 */
	KeyF8					= 0x77,		/**< F8 */
	KeyF9					= 0x78,		/**< F9 */
	KeyF10					= 0x79,		/**< F10 */
	KeyF11					= 0x7A,		/**< F11 */
	KeyF12					= 0x7B,		/**< F12 */
	KeyNumLock				= 0x90,		/**< Num lock */
	KeyScrollLock			= 0x91,		/**< Scroll lock */
	KeyCircumflex			= 0xDC,		/**< Circumflex (^) */
	KeyLeftWindows			= 0x5B,		/**< Left Windows key (natural keyboard) */
	KeyRightWindows			= 0x5C,		/**< Right Windows key (natural keyboard) */
	KeyApplications			= 0x5D,		/**< Applications key (natural keyboard) */
	KeyF13					= 0x7C,		/**< F13 key */
	KeyF14					= 0x7D,		/**< F14 key */
	KeyF15					= 0x7E,		/**< F15 key */
	KeyF16					= 0x7F,		/**< F16 key */
	KeyF17					= 0x80,		/**< F17 key */
	KeyF18					= 0x81,		/**< F18 key */
	KeyF19					= 0x82,		/**< F19 key */
	KeyF20					= 0x83,		/**< F20 key */
	KeyF21					= 0x84,		/**< F21 key */
	KeyF22					= 0x85,		/**< F22 key */
	KeyF23					= 0x86,		/**< F23 key */
	KeyF24					= 0x87,		/**< F24 key */
	KeyLeftShift			= 0xA0,		/**< Left shift key */
	KeyRightShift			= 0xA1,		/**< Right shift key */
	KeyLeftControl			= 0xA2,		/**< Left control key */
	KeyRightControl			= 0xA3,		/**< Right control key */
	KeyVolumeMute			= 0xAD,		/**< Volume mute key */
	KeyVolumeDown			= 0xAE,		/**< Volume down key */
	KeyVolumeUp				= 0xAF,		/**< Volume up key */
	KeyMediaNextTrack		= 0xB0,		/**< Next track key */
	KeyMediaPreviousTrack	= 0xB1,		/**< Previous track key */
	KeyMediaStop			= 0xB2,		/**< Stop media key */
	KeyMediaPlayPause		= 0xB3,		/**< Play/pause media key */
	KeyAdd					= 0xBB,		/**< For any country/region, the '+' key */
	KeySeparator			= 0xBC,		/**< For any country/region, the ',' key */
	KeySubtract				= 0xBD,		/**< For any country/region, the '-' key */
	KeyDecimal				= 0xBE,		/**< For any country/region, the '.' key */
	KeyOEM1					= 0xBA,		/**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key. */
	KeyOEM2					= 0xBF,		/**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key. */
	KeyOEM3					= 0xC0,		/**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key. */
	KeyOEM4					= 0xDB,		/**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key. */
	KeyOEM5					= 0xDC,		/**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key. */
	KeyOEM6					= 0xDD,		/**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key. */
	KeyOEM7					= 0xDE,		/**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key. */
	KeyOEM8					= 0xDF,		/**< Used for miscellaneous characters; it can vary by keyboard. */
	KeyOEM102				= 0xE2		/**< Either the angle bracket key or the backslash key on the RT 102-key keyboard */
};


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
RawInputDevice::RawInputDevice() :
	m_nType(0),
	m_hDevice(nullptr),
	m_bVirtual(false),
	m_nOldX(0),
	m_nOldY(0)
{
	// Do not destroy device implementation automatically, because this is managed by RawInput
	m_bDelete = false;

	// Clear deviceinfo struct
	MemoryManager::Set(&m_sDeviceInfo, 0, sizeof(m_sDeviceInfo));
}

/**
*  @brief
*    Copy constructor
*/
RawInputDevice::RawInputDevice(const RawInputDevice &cSource) :
	m_sName(cSource.m_sName),
	m_nType(cSource.m_nType),
	m_hDevice(cSource.m_hDevice),
	m_bVirtual(cSource.m_bVirtual)
{
	// Do not destroy device implementation automatically, because this is managed by RawInput
	m_bDelete = false;

	// Copy deviceinfo struct
	MemoryManager::Copy(&m_sDeviceInfo, &cSource.m_sDeviceInfo, sizeof(m_sDeviceInfo));
}

/**
*  @brief
*    Destructor
*/
RawInputDevice::~RawInputDevice()
{
}

/**
*  @brief
*    Copy operator
*/
RawInputDevice &RawInputDevice::operator =(const RawInputDevice &cSource)
{
	m_sName		= cSource.m_sName;
	m_nType		= cSource.m_nType;
	m_hDevice	= cSource.m_hDevice;
	m_bVirtual	= cSource.m_bVirtual;
	MemoryManager::Copy(&m_sDeviceInfo, &cSource.m_sDeviceInfo, sizeof(m_sDeviceInfo));
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
bool RawInputDevice::operator ==(const RawInputDevice &cSource)
{
	return (m_sName		== cSource.m_sName &&
			m_nType		== cSource.m_nType &&
			m_hDevice	== cSource.m_hDevice &&
			m_bVirtual	== cSource.m_bVirtual &&
			MemoryManager::Compare(&m_sDeviceInfo, &cSource.m_sDeviceInfo, sizeof(m_sDeviceInfo)) == 0);
}

/**
*  @brief
*    Get device name
*/
String RawInputDevice::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Get device type
*/
DWORD RawInputDevice::GetType() const
{
	return m_nType;
}

/**
*  @brief
*    Get device handle
*/
HANDLE RawInputDevice::GetHandle() const
{
	return m_hDevice;
}

/**
*  @brief
*    Get device info
*/
RID_DEVICE_INFO RawInputDevice::GetDeviceInfo() const
{
	return m_sDeviceInfo;
}

/**
*  @brief
*    Check if the device is a virtual device
*/
bool RawInputDevice::IsVirtual() const
{
	// Check virtual device
	return m_bVirtual;
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void RawInputDevice::Update()
{
	// Check if input device is valid
	if (m_pDevice && m_nType == RIM_TYPEMOUSE) {
		// Get mouse device
		Mouse *pMouse = static_cast<Mouse*>(m_pDevice);

		// Reset mouse movement
		if (pMouse->X.GetValue() != 0.0f)
			pMouse->X.SetValue(0.0f, true);
		if (pMouse->Y.GetValue() != 0.0f)
			pMouse->Y.SetValue(0, true);
		if (pMouse->Wheel.GetValue() != 0.0f)
			pMouse->Wheel.SetValue(0, true);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Process raw keyboard data
*/
void RawInputDevice::ProcessKeyboardData(unsigned short nMakeCode, unsigned short nFlags, unsigned short nReserved, unsigned short nKey, unsigned int nMessage, unsigned long nExtra)
{
	// Check if input device is valid
	if (m_pDevice && m_nType == RIM_TYPEKEYBOARD) {
		// Get keyboard device
		Keyboard *pKeyboard = static_cast<Keyboard*>(m_pDevice);

		// Get button
		Button *pButton = GetKeyboardKey(pKeyboard, nKey);
		if (pButton) {
			// Make or break code?
			bool bPressed = ((nFlags & RI_KEY_BREAK) == 0);
			if (pButton->IsPressed() != bPressed)
				pButton->SetPressed(bPressed);
		}
	}
}

/**
*  @brief
*    Process raw mouse data
*/
void RawInputDevice::ProcessMouseData(unsigned short nFlags, unsigned long nButtons, unsigned short nButtonFlags, unsigned short nButtonData, long nRawButtons, long nLastX, long nLastY, unsigned long nExtra)
{
	// Check if input device is valid
	if (m_pDevice && m_nType == RIM_TYPEMOUSE) {
		// Get mouse device
		Mouse *pMouse = static_cast<Mouse*>(m_pDevice);

		// Update axes
		float fX, fY;
		if (nFlags & MOUSE_MOVE_ABSOLUTE) {
			// Absolute position
			fX = pMouse->X.GetValue() + static_cast<float>(nLastX - m_nOldX);
			fY = pMouse->Y.GetValue() + static_cast<float>(nLastY - m_nOldY);
			m_nOldX = nLastX;
			m_nOldY = nLastY;
		} else {
			// Accumulate relative position
			fX = pMouse->X.GetValue() + static_cast<float>(nLastX);
			fY = pMouse->Y.GetValue() + static_cast<float>(nLastY);
		}

		// Set new axis values
		if (pMouse->X.GetValue() != fX)
			pMouse->X.SetValue(fX, true);
		if (pMouse->Y.GetValue() != fY)
			pMouse->Y.SetValue(fY, true);

		// Update buttons
		if (nButtonFlags) {
			if (nButtonFlags & RI_MOUSE_BUTTON_1_DOWN) {
				if (!pMouse->Left.IsPressed())
					pMouse->Left.SetPressed(true);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_1_UP) {
				if (pMouse->Left.IsPressed())
					pMouse->Left.SetPressed(false);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_2_DOWN) {
				if (!pMouse->Right.IsPressed())
					pMouse->Right.SetPressed(true);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_2_UP) {
				if (pMouse->Right.IsPressed())
					pMouse->Right.SetPressed(false);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_3_DOWN) {
				if (!pMouse->Middle.IsPressed())
					pMouse->Middle.SetPressed(true);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_3_UP) {
				if (pMouse->Middle.IsPressed())
					pMouse->Middle.SetPressed(false);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
				if (!pMouse->Button4.IsPressed())
					pMouse->Button4.SetPressed(true);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_4_UP) {
				if (pMouse->Button4.IsPressed())
					pMouse->Button4.SetPressed(false);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
				if (!pMouse->Button5.IsPressed())
					pMouse->Button5.SetPressed(true);
			}
			if (nButtonFlags & RI_MOUSE_BUTTON_5_UP) {
				if (pMouse->Button5.IsPressed())
					pMouse->Button5.SetPressed(false);
			}
			if (nButtonFlags & RI_MOUSE_WHEEL) {
				// Well ... nButtonData is unsigned, but the wheel delta can be negative. So let's cast USHORT to SHORT.
				float fValue = static_cast<float>(*reinterpret_cast<short*>(&nButtonData));
				if (pMouse->Wheel.GetValue() != fValue)
					pMouse->Wheel.SetValue(fValue, true);
			}
		}
	}
}

/**
*  @brief
*    Get key for virtual key code
*/
Button *RawInputDevice::GetKeyboardKey(Keyboard *pKeyboard, unsigned short nKey)
{
	// Return key that corresponds to the given key code
	switch (nKey) {
		case KeyBackspace:		return &pKeyboard->Backspace;
		case KeyTab:			return &pKeyboard->Tab;
		case KeyClear:			return &pKeyboard->Clear;
		case KeyReturn:			return &pKeyboard->Return;
		case KeyShift:			return &pKeyboard->Shift;
		case KeyControl:		return &pKeyboard->Control;
		case KeyAlt:			return &pKeyboard->Alt;
		case KeyPause:			return &pKeyboard->Pause;
		case KeyCapsLock:		return &pKeyboard->CapsLock;
		case KeyEscape:			return &pKeyboard->Escape;
		case KeySpace:			return &pKeyboard->Space;
		case KeyPageUp:			return &pKeyboard->PageUp;
		case KeyPageDown:		return &pKeyboard->PageDown;
		case KeyEnd:			return &pKeyboard->End;
		case KeyHome:			return &pKeyboard->Home;
		case KeyLeft:			return &pKeyboard->Left;
		case KeyUp:				return &pKeyboard->Up;
		case KeyRight:			return &pKeyboard->Right;
		case KeyDown:			return &pKeyboard->Down;
		case KeySelect:			return &pKeyboard->Select;
		case KeyExecute:		return &pKeyboard->Execute;
		case KeyPrint:			return &pKeyboard->Print;
		case KeyInsert:			return &pKeyboard->Insert;
		case KeyDelete:			return &pKeyboard->Delete;
		case KeyHelp:			return &pKeyboard->Help;
		case Key0:				return &pKeyboard->Key0;
		case Key1:				return &pKeyboard->Key1;
		case Key2:				return &pKeyboard->Key2;
		case Key3:				return &pKeyboard->Key3;
		case Key4:				return &pKeyboard->Key4;
		case Key5:				return &pKeyboard->Key5;
		case Key6:				return &pKeyboard->Key6;
		case Key7:				return &pKeyboard->Key7;
		case Key8:				return &pKeyboard->Key8;
		case Key9:				return &pKeyboard->Key9;
		case KeyA:				return &pKeyboard->A;
		case KeyB:				return &pKeyboard->B;
		case KeyC:				return &pKeyboard->C;
		case KeyD:				return &pKeyboard->D;
		case KeyE:				return &pKeyboard->E;
		case KeyF:				return &pKeyboard->F;
		case KeyG:				return &pKeyboard->G;
		case KeyH:				return &pKeyboard->H;
		case KeyI:				return &pKeyboard->I;
		case KeyJ:				return &pKeyboard->J;
		case KeyK:				return &pKeyboard->K;
		case KeyL:				return &pKeyboard->L;
		case KeyM:				return &pKeyboard->M;
		case KeyN:				return &pKeyboard->N;
		case KeyO:				return &pKeyboard->O;
		case KeyP:				return &pKeyboard->P;
		case KeyQ:				return &pKeyboard->Q;
		case KeyR:				return &pKeyboard->R;
		case KeyS:				return &pKeyboard->S;
		case KeyT:				return &pKeyboard->T;
		case KeyU:				return &pKeyboard->U;
		case KeyV:				return &pKeyboard->V;
		case KeyW:				return &pKeyboard->W;
		case KeyX:				return &pKeyboard->X;
		case KeyY:				return &pKeyboard->Y;
		case KeyZ:				return &pKeyboard->Z;
		case KeyNumpad0:		return &pKeyboard->Numpad0;
		case KeyNumpad1:		return &pKeyboard->Numpad1;
		case KeyNumpad2:		return &pKeyboard->Numpad2;
		case KeyNumpad3:		return &pKeyboard->Numpad3;
		case KeyNumpad4:		return &pKeyboard->Numpad4;
		case KeyNumpad5:		return &pKeyboard->Numpad5;
		case KeyNumpad6:		return &pKeyboard->Numpad6;
		case KeyNumpad7:		return &pKeyboard->Numpad7;
		case KeyNumpad8:		return &pKeyboard->Numpad8;
		case KeyNumpad9:		return &pKeyboard->Numpad9;
		case KeyNumpadMultiply:	return &pKeyboard->NumpadMultiply;
		case KeyNumpadAdd:		return &pKeyboard->NumpadAdd;
		case KeyNumpadSeparator:return &pKeyboard->NumpadSeparator;
		case KeyNumpadSubtract:	return &pKeyboard->NumpadSubtract;
		case KeyNumpadDecimal:	return &pKeyboard->NumpadDecimal;
		case KeyNumpadDivide:	return &pKeyboard->NumpadDivide;
		case KeyF1:				return &pKeyboard->F1;
		case KeyF2:				return &pKeyboard->F2;
		case KeyF3:				return &pKeyboard->F3;
		case KeyF4:				return &pKeyboard->F4;
		case KeyF5:				return &pKeyboard->F5;
		case KeyF6:				return &pKeyboard->F6;
		case KeyF7:				return &pKeyboard->F7;
		case KeyF8:				return &pKeyboard->F8;
		case KeyF9:				return &pKeyboard->F9;
		case KeyF10:			return &pKeyboard->F10;
		case KeyF11:			return &pKeyboard->F11;
		case KeyF12:			return &pKeyboard->F12;
		case KeyNumLock:		return &pKeyboard->NumLock;
		case KeyScrollLock:		return &pKeyboard->ScrollLock;
		case KeyCircumflex:		return &pKeyboard->Circumflex;
		case KeyLeftWindows:	return &pKeyboard->LeftWindows;
		case KeyRightWindows:	return &pKeyboard->RightWindows;
		case KeyApplications:	return &pKeyboard->Applications;
		case KeyF13:			return &pKeyboard->F13;
		case KeyF14:			return &pKeyboard->F14;
		case KeyF15:			return &pKeyboard->F15;
		case KeyF16:			return &pKeyboard->F16;
		case KeyF17:			return &pKeyboard->F17;
		case KeyF18:			return &pKeyboard->F18;
		case KeyF19:			return &pKeyboard->F19;
		case KeyF20:			return &pKeyboard->F20;
		case KeyF21:			return &pKeyboard->F21;
		case KeyF22:			return &pKeyboard->F22;
		case KeyF23:			return &pKeyboard->F23;
		case KeyF24:			return &pKeyboard->F24;
		case KeyLeftShift:		return &pKeyboard->LeftShift; // figure out how we can get these four instead of only Shift and Control
		case KeyRightShift:		return &pKeyboard->RightShift; // * ^
		case KeyLeftControl:	return &pKeyboard->LeftControl; // * ^
		case KeyRightControl:	return &pKeyboard->RightControl; // * ^
		case KeyVolumeMute:		return &pKeyboard->VolumeMute;
		case KeyVolumeDown:		return &pKeyboard->VolumeDown;
		case KeyVolumeUp:		return &pKeyboard->VolumeUp;
		case KeyMediaNextTrack:		return &pKeyboard->MediaNextTrack;
		case KeyMediaPreviousTrack:	return &pKeyboard->MediaPreviousTrack;
		case KeyMediaStop:			return &pKeyboard->MediaStop;
		case KeyMediaPlayPause:		return &pKeyboard->MediaPlayPause;
		case KeyAdd:			return &pKeyboard->Add;
		case KeySeparator:		return &pKeyboard->Separator;
		case KeySubtract:		return &pKeyboard->Subtract;
		case KeyDecimal:		return &pKeyboard->Decimal;
		case KeyOEM1:			return &pKeyboard->OEM1;
		case KeyOEM2:			return &pKeyboard->OEM2;
		case KeyOEM3:			return &pKeyboard->OEM3;
		case KeyOEM4:			return &pKeyboard->OEM4;
		// case KeyOEM5:			return &pKeyboard->OEM5; // Circumflex was already defined
		case KeyOEM6:			return &pKeyboard->OEM6;
		case KeyOEM7:			return &pKeyboard->OEM7;
		case KeyOEM8:			return &pKeyboard->OEM8;
		case KeyOEM102:			return &pKeyboard->OEM102;
		default:				return  nullptr;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
