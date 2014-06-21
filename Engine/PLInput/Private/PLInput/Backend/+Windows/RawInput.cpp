/*********************************************************\
 *  File: RawInput.cpp                                   *
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
#include "PLInput/Backend/+Windows/RawInputDevice.h"
#include "PLInput/Backend/+Windows/RawInput.h"


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
namespace PLCore {
	template class Singleton<PLInput::RawInput>;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
RawInput *RawInput::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<RawInput>::GetInstance();
}

bool RawInput::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<RawInput>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Remove all devices
*/
void RawInput::Clear()
{
	// Destroy devices
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++)
		delete m_lstDevices[i];
	m_lstDevices.Clear();
	m_mapDevices.Clear();
	m_pDeviceKeyboard = nullptr;
	m_pDeviceMouse	  = nullptr;
}

/**
*  @brief
*    Detect available Raw-Input devices
*/
void RawInput::DetectDevices()
{
	// Clear device list
	Clear();

	// Get length of device list
	UINT nDeviceCount = 0;
	if (GetRawInputDeviceList(nullptr, &nDeviceCount, sizeof(RAWINPUTDEVICELIST)) == 0 && nDeviceCount > 0) {
		// Create device array
		RAWINPUTDEVICELIST *pDevices = new RAWINPUTDEVICELIST[nDeviceCount];

		// Get device list
		// Note: We pass nDeviceCount, according to the new documentation of GetRawInputDeviceList.
		// Previously, it was the byte size. See http://support.microsoft.com/kb/946924/en/
		if (GetRawInputDeviceList(pDevices, &nDeviceCount, sizeof(RAWINPUTDEVICELIST)) > 0) {
			// Loop through devices
			for (uint32 i=0; i<nDeviceCount; i++) {
				// Get device name
				String sName = "Unknown";
				UINT   nSize = 0;
				GetRawInputDeviceInfo(pDevices[i].hDevice, RIDI_DEVICENAME, nullptr, &nSize);
				if (nSize > 0) {
					wchar_t *pszName = new wchar_t[nSize+1];
					GetRawInputDeviceInfo(pDevices[i].hDevice, RIDI_DEVICENAME, pszName, &nSize);
					sName = String(pszName, false);
				}

				// Get device info
				RID_DEVICE_INFO sDeviceInfo;
				nSize = sDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
				if (GetRawInputDeviceInfo(pDevices[i].hDevice, RIDI_DEVICEINFO, &sDeviceInfo, &nSize) > 0) {
					// Add device
					RawInputDevice *pDevice = new RawInputDevice();
					pDevice->m_sName		= sName;
					pDevice->m_nType		= pDevices[i].dwType;
					pDevice->m_hDevice		= pDevices[i].hDevice;
					pDevice->m_sDeviceInfo	= sDeviceInfo;
					pDevice->m_bVirtual		= false;
					m_lstDevices.Add(pDevice);
					m_mapDevices.Add(reinterpret_cast<uint32>(pDevice->m_hDevice), pDevice);
				}
			}

			// Add virtual Keyboard device (will catches all events of all keyboard devices)
			m_pDeviceKeyboard = new RawInputDevice();
			m_pDeviceKeyboard->m_sName    = "Keyboard";
			m_pDeviceKeyboard->m_nType    = RIM_TYPEKEYBOARD;
			m_pDeviceKeyboard->m_hDevice  = nullptr;
			m_pDeviceKeyboard->m_bVirtual = true;
			m_lstDevices.Add(m_pDeviceKeyboard);

			// Add virtual Mouse device (will catches all events of all mouse devices)
			m_pDeviceMouse = new RawInputDevice();
			m_pDeviceMouse->m_sName    = "Mouse";
			m_pDeviceMouse->m_nType    = RIM_TYPEMOUSE;
			m_pDeviceMouse->m_hDevice  = nullptr;
			m_pDeviceMouse->m_bVirtual = true;
			m_lstDevices.Add(m_pDeviceMouse);
		}

		// Destroy buffer
		delete [] pDevices;
	}
}

/**
*  @brief
*    Get list of available input devices
*/
const List<RawInputDevice*> &RawInput::GetDevices() const
{
	// Return device list
	return m_lstDevices;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RawInput::RawInput() :
	m_hWnd(nullptr),
	m_cThread(&RawInput::RawInputThread, this),
	m_bThreadFinished(false)
{
	// Create window class
	m_cWndClass.lpszClassName	= L"PLInputWindows";
	m_cWndClass.lpfnWndProc		= WndProc;
	m_cWndClass.style			= CS_VREDRAW | CS_HREDRAW;
	m_cWndClass.hInstance		= GetModuleHandle(nullptr);
	m_cWndClass.hIcon			= nullptr;
	m_cWndClass.hCursor			= LoadCursor(nullptr, IDC_ARROW);
	m_cWndClass.hbrBackground	= reinterpret_cast<HBRUSH>(COLOR_WINDOW);
	m_cWndClass.lpszMenuName	= nullptr;
	m_cWndClass.cbClsExtra		= 0;
	m_cWndClass.cbWndExtra		= 0;
	RegisterClass(&m_cWndClass);

	// Create window
	m_hWnd = CreateWindow(L"PLInputWindows", L"PLInputWindows", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	// Register Raw-Input devices
	RAWINPUTDEVICE cRawInputDevice[2];
	cRawInputDevice[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
	cRawInputDevice[0].usUsage		= HID_USAGE_GENERIC_MOUSE;
	cRawInputDevice[0].dwFlags		= RIDEV_INPUTSINK; // [TODO] Might be: RIDEV_INPUTSINK, to get input even in background.
	cRawInputDevice[0].hwndTarget	= m_hWnd;
	cRawInputDevice[1].usUsagePage	= HID_USAGE_PAGE_GENERIC;
	cRawInputDevice[1].usUsage		= HID_USAGE_GENERIC_KEYBOARD;
	cRawInputDevice[1].dwFlags		= RIDEV_INPUTSINK; // [TODO] Might be: RIDEV_INPUTSINK, to get input even in background.
	cRawInputDevice[1].hwndTarget	= m_hWnd;
	RegisterRawInputDevices(cRawInputDevice, 2, sizeof(cRawInputDevice[0]));

	// Detect devices
	DetectDevices();

	// Start thread
	m_cThread.Start();
}

/**
*  @brief
*    Destructor
*/
RawInput::~RawInput()
{
	// Stop thread - and set a timeout to ensure it terminates in time
	m_bThreadFinished = true;
	m_cThread.Join(100);

	// Clear
	Clear();

	// Destroy window
	if (m_hWnd) {
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}

	// Delete window class
	HMODULE hModuleHandle = GetModuleHandle(nullptr);
	if (hModuleHandle)
		UnregisterClass(L"PLInputWindows", hModuleHandle);
}

/**
*  @brief
*    Raw-Input message processing
*/
LRESULT RawInput::ProcessRawInput(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// Create buffer
	UINT nSize;
	GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &nSize, sizeof(RAWINPUTHEADER));
	BYTE *pBuffer = new BYTE[nSize];
	if (pBuffer) {
		// Read Raw-Input data
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, pBuffer, &nSize, sizeof(RAWINPUTHEADER)) == nSize) {
			RAWINPUT *pRawInput = reinterpret_cast<RAWINPUT*>(pBuffer);

			// Get device
			RawInputDevice *pDevice = m_mapDevices.Get(reinterpret_cast<uint32>(pRawInput->header.hDevice));

			// Keyboard input
			if (pRawInput->header.dwType == RIM_TYPEKEYBOARD) {
				// Send to device
				if (pDevice) {
					pDevice->ProcessKeyboardData(
						pRawInput->data.keyboard.MakeCode,
						pRawInput->data.keyboard.Flags,
						pRawInput->data.keyboard.Reserved,
						pRawInput->data.keyboard.VKey,
						pRawInput->data.keyboard.Message,
						pRawInput->data.keyboard.ExtraInformation
					);
				}

				// Send to virtual device "Keyboard"
				if (m_pDeviceKeyboard) {
					m_pDeviceKeyboard->ProcessKeyboardData(
						pRawInput->data.keyboard.MakeCode,
						pRawInput->data.keyboard.Flags,
						pRawInput->data.keyboard.Reserved,
						pRawInput->data.keyboard.VKey,
						pRawInput->data.keyboard.Message,
						pRawInput->data.keyboard.ExtraInformation
					);
				}

			// Mouse input
			} else if (pRawInput->header.dwType == RIM_TYPEMOUSE) {
				// Send to device
				if (pDevice) {
					pDevice->ProcessMouseData(
						pRawInput->data.mouse.usFlags,
						pRawInput->data.mouse.ulButtons,
						pRawInput->data.mouse.usButtonFlags,
						pRawInput->data.mouse.usButtonData,
						pRawInput->data.mouse.ulRawButtons,
						pRawInput->data.mouse.lLastX,
						pRawInput->data.mouse.lLastY,
						pRawInput->data.mouse.ulExtraInformation
					);
				}

				// Send to virtual device "Mouse"
				if (m_pDeviceMouse) {
					m_pDeviceMouse->ProcessMouseData(
						pRawInput->data.mouse.usFlags,
						pRawInput->data.mouse.ulButtons,
						pRawInput->data.mouse.usButtonFlags,
						pRawInput->data.mouse.usButtonData,
						pRawInput->data.mouse.ulRawButtons,
						pRawInput->data.mouse.lLastX,
						pRawInput->data.mouse.lLastY,
						pRawInput->data.mouse.ulExtraInformation
					);
				}
			}
		} else {
			// Error reading buffer
		}

		// Destroy buffer
		delete [] pBuffer;
	} else {
		// Error creating buffer
	}

	// Done
	return 0;
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Window callback function
*/
LRESULT WINAPI RawInput::WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// Catch Raw-Input messages
	if (nMsg == WM_INPUT) {
		// Get device provider instance
		RawInput *pRawInput = reinterpret_cast<RawInput*>(GetWindowLongPtr(hWnd, -21));
		if (pRawInput) {
			// Call Raw-Input message handler
			return pRawInput->ProcessRawInput(hWnd, nMsg, wParam, lParam);
		}
	}

	// Unhandled message
	return DefWindowProc(hWnd, nMsg, wParam, lParam);
}

/**
*  @brief
*    Thread for reading raw-input messages constantly
*/
int RawInput::RawInputThread(void *pData)
{
	// Get RawInput object
	RawInput *pRawInput = static_cast<RawInput*>(pData);

	// Read window message for RawInput window
	MSG sMsg;
	while (!pRawInput->m_bThreadFinished && GetMessage(&sMsg, pRawInput->m_hWnd, 0, 0)) {
		// Nothing to do
	}

	// Done
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
