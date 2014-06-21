/*********************************************************\
 *  File: LinuxMouseDevice.cpp                           *
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
#include "PLInput/Input/Devices/Mouse.h"
#include "PLInput/Backend/Linux/LinuxMouseDevice.h"


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
LinuxMouseDevice::LinuxMouseDevice() :
	m_pDisplay(XOpenDisplay(nullptr)),
	m_nMouseX(0),
	m_nMouseY(0)
{
	// Destroy device implementation automatically
	m_bDelete = true;
}

/**
*  @brief
*    Destructor
*/
LinuxMouseDevice::~LinuxMouseDevice()
{
	// Close display
	if (m_pDisplay)
		XCloseDisplay(m_pDisplay);
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void LinuxMouseDevice::Update()
{
	// Check if display and input device is valid
	if (m_pDisplay && m_pDevice) {
		// Get mouse device
		Mouse *pMouse = static_cast<Mouse*>(m_pDevice);

		// Get mouse state
		::Window 	 nRootWindow, nChildWindow;
		int 		 nRootX, nRootY, nChildX, nChildY;
		unsigned int nMask;
		XLockDisplay(m_pDisplay);
		XQueryPointer(m_pDisplay, DefaultRootWindow(m_pDisplay), &nRootWindow, &nChildWindow, &nRootX, &nRootY, &nChildX, &nChildY, &nMask);
		XUnlockDisplay(m_pDisplay);

		// Update axes
		int nDeltaX = nRootX - m_nMouseX;
		int nDeltaY = nRootY - m_nMouseY;
		m_nMouseX = nRootX;
		m_nMouseY = nRootY;
		if (pMouse->X.GetValue() != nDeltaX)
			pMouse->X.SetValue(nDeltaX, true);
		if (pMouse->Y.GetValue() != nDeltaY)
			pMouse->Y.SetValue(nDeltaY, true);

		// Update buttons
		bool bPressed = ((nMask & Button1Mask) != 0);
		if (pMouse->Left.IsPressed() != bPressed)
			pMouse->Left.SetPressed(bPressed);
		bPressed = ((nMask & Button2Mask) != 0);
		if (pMouse->Right.IsPressed() != bPressed)
			pMouse->Right.SetPressed(bPressed);
		bPressed = ((nMask & Button3Mask) != 0);
		if (pMouse->Middle.IsPressed() != bPressed)
			pMouse->Middle.SetPressed(bPressed);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
