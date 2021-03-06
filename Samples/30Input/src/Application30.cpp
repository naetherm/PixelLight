/*********************************************************\
 *  File: Application30.cpp                              *
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
#include <PLCore/System/System.h>
#include <PLCore/System/Console.h>
#include <PLInput/Input/InputManager.h>
#include <PLInput/Input/Controller.h>
#include <PLInput/Input/Devices/Device.h>
#include <PLInput/Input/Controls/Effect.h>
#include <PLInput/Input/Controls/LED.h>
#include <PLInput/Input/Virtual/VirtualStandardController.h>
#include "GameBoyController.h"
#include "Application30.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLInput;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Application30, "", PLCore::FrontendApplication, "Application class")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLCore::Frontend&,	"Parameter constructor. Frontend this application instance is running in as first parameter.",	"")
pl_class_metadata_end(Application30)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application30::Application30(Frontend &cFrontend) : FrontendApplication(cFrontend),
	EventHandlerOnControl(&Application30::OnControl, this),
	EventHandlerOnControlExit(&Application30::OnControlExit, this),
	m_bExit(false)
{
}

/**
*  @brief
*    Destructor
*/
Application30::~Application30()
{
}

/**
*  @brief
*    Test: List devices
*/
void Application30::TestDevices()
{
	// Start
	System::GetInstance()->GetConsole().Print("Input devices:\n");

	// Get device list
	const List<Device*> &lstDevices = InputManager::GetInstance()->GetDevices();
	for (uint32 i=0; i<lstDevices.GetNumOfElements(); i++) {
		// Get device
		Device *pDevice = lstDevices[i];
		String  sName = pDevice->GetName();
		String  sDesc = pDevice->GetDescription();

		// Print device info
		System::GetInstance()->GetConsole().Print("- " + sName + ": " + sDesc + '\n');
	}

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

/**
*  @brief
*    Test: Controller
*/
void Application30::TestController()
{
	// Start
	System::GetInstance()->GetConsole().Print("Controller 'GameBoy'\n");
	System::GetInstance()->GetConsole().Print('\n');

	// Create test controller
	GameBoyController cController;

	// List all controls
	System::GetInstance()->GetConsole().Print("Controls:\n");
	const List<Control*> &lstControls = cController.GetControls();
	for (uint32 i=0; i<lstControls.GetNumOfElements(); i++) {
		// Get control
		Control *pControl = lstControls[i];
		if (pControl->GetType() == ControlButton)
			System::GetInstance()->GetConsole().Print("- Button '" + pControl->GetName() + "' [" + pControl->GetDescription() + "]\n");
		else if (pControl->GetType() == ControlAxis)
			System::GetInstance()->GetConsole().Print("- Axis   '" + pControl->GetName() + "' [" + pControl->GetDescription() + "]\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// List buttons
	System::GetInstance()->GetConsole().Print("Buttons:\n");
	const List<Button*> &lstButtons = cController.GetButtons();
	for (uint32 i=0; i<lstButtons.GetNumOfElements(); i++) {
		Button *pButton = lstButtons[i];
		System::GetInstance()->GetConsole().Print("- Button   '" + pButton->GetName() + "' [" + pButton->GetDescription() + "]\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// List axes
	System::GetInstance()->GetConsole().Print("Axes:\n");
	const List<Axis*> &lstAxes = cController.GetAxes();
	for (uint32 i=0; i<lstAxes.GetNumOfElements(); i++) {
		Axis *pAxis = lstAxes[i];
		System::GetInstance()->GetConsole().Print("- Axis '" + pAxis->GetName() + "' [" + pAxis->GetDescription() + "]\n");
	}
	System::GetInstance()->GetConsole().Print('\n');

	// Done
	System::GetInstance()->GetConsole().Print('\n');
}

/**
*  @brief
*    Test: Input
*/
void Application30::TestInput(const String &sDevice)
{
	// Start
	System::GetInstance()->GetConsole().Print("Getting input from device '" + sDevice + "'\n");

	// Connect keyboard
	Device *pDevice = InputManager::GetInstance()->GetDevice("Keyboard");
	if (pDevice) {
		// Connect event handler
		pDevice->SignalOnControl.Connect(EventHandlerOnControlExit);
	}

	// Get device
	pDevice = InputManager::GetInstance()->GetDevice(sDevice);
	if (pDevice) {
		// Connect event handler
		pDevice->SignalOnControl.Connect(EventHandlerOnControl);

		// Main loop
		m_bExit = false;
		while (!m_bExit) {
			// Update devices
			InputManager::GetInstance()->Update();

			// Ping the frontend
			GetFrontend().Ping();

			// Wait 10 milliseconds
			System::GetInstance()->Sleep(10);
		}
	} else {
		// Error
		System::GetInstance()->GetConsole().Print("ERROR: Could not get device\n");
	}
}

/**
*  @brief
*    Test: GetChar() functionality
*/
void Application30::TestGetChar(const String &sDevice)
{
	// Start
	System::GetInstance()->GetConsole().Print("Testing GetChar() with device '" + sDevice + "'\n");

	// Get device
	Device *pDevice = InputManager::GetInstance()->GetDevice(sDevice);
	if (pDevice) {
		String sInput;

		// Main loop
		m_bExit = false;
		while (!m_bExit) {
			// Ping the frontend
			GetFrontend().Ping();

			// Update devices
			InputManager::GetInstance()->Update();

			// Get character
			char nChar = pDevice->GetChar();
			if (nChar != '\0') {
				// Escape
				if (nChar == 0x1B) {
					// Exit
					m_bExit = true;

				// Enter
				} else if (nChar == 0x0D) {
					// Next word
					System::GetInstance()->GetConsole().Print('\n');
					sInput = "";

				// Backspace
				} else if (nChar == 0x08) {
					// Delete last character
					sInput = sInput.GetSubstring(0, sInput.GetLength()-1);

					// Print on next line
					System::GetInstance()->GetConsole().Print('\n');
					System::GetInstance()->GetConsole().Print(sInput);

				// Character
				} else if ((nChar >= '0' && nChar <= '9') || (nChar >= 'a' && nChar <= 'z') ||
							nChar == '+' || nChar == '-' || nChar == '*' || nChar == '/' || nChar == ',' || nChar == '.' ||
							nChar == 0x20) {
					// Add char to string
					sInput += nChar;

					// Print char
					System::GetInstance()->GetConsole().Print(String() + nChar);
				}
			}

			// Wait 10 milliseconds
			System::GetInstance()->Sleep(10);
		}
	} else {
		// Error
		System::GetInstance()->GetConsole().Print("ERROR: Could not get device\n");
	}
}

/**
*  @brief
*    Test: Connections
*/
void Application30::TestConnections()
{
	// Start
	System::GetInstance()->GetConsole().Print("Connecting virtual controller\n");

	// Connect keyboard
	Device *pDevice = InputManager::GetInstance()->GetDevice("Keyboard");
	if (pDevice) {
		// Connect event handler
		pDevice->SignalOnControl.Connect(EventHandlerOnControlExit);
	}

	// Create virtual controller
	VirtualController *pVirtualController = new VirtualStandardController();
	pVirtualController->ConnectToDevices();

	// Get device
	pDevice = reinterpret_cast<Device*>(pVirtualController);
	if (pDevice) {
		// Connect event handler
		pDevice->SignalOnControl.Connect(EventHandlerOnControl);

		// Main loop
		m_bExit = false;
		System::GetInstance()->GetConsole().Print("Ready to receive input\n");
		while (!m_bExit) {
			// Update devices
			InputManager::GetInstance()->Update();

			// Ping the frontend
			GetFrontend().Ping();

			// Wait 10 milliseconds
			System::GetInstance()->Sleep(10);
		}
	}

	// Destroy virtual controller
	delete pVirtualController;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a control of the input controller has been activated
*/
void Application30::OnControl(Control &cControl)
{
	// Get name of control
	String sControl = cControl.GetName();

	// Display control value
	String sValue;
	if (cControl.GetType() == ControlButton)
		sValue = static_cast<Button&>(cControl).IsPressed() ? "<pressed>" : "<released>";
	else if (cControl.GetType() == ControlAxis)
		sValue = String::Format("%5.2f", static_cast<Axis&>(cControl).GetValue());
	System::GetInstance()->GetConsole().Print("- '" + sControl + "': " + sValue + '\n');

	// LED test
	if ((cControl.GetName() == "Plus" || cControl.GetName() == "Minus") && static_cast<Button&>(cControl).IsPressed()) {
		// Get LED control
		LED *pLED = static_cast<LED*>(cControl.GetController()->GetControl("LED"));
		if (pLED) {
			// Change LED value
			uint32 nLED = pLED->GetLEDs();
			if (cControl.GetName() == "Plus")
				nLED++;
			else
				nLED--;
			if (nLED > 15)
				nLED = 0;
			pLED->SetLEDs(nLED);
		}
	}

	// Rumble test
	if (cControl.GetName() == "Button1" || cControl.GetName() == "Button2") {
		// Get rumble control (try "Rumble3" first for joystick, then "Rumble1" for WiiMote)
		Effect *pRumble = static_cast<Effect*>(cControl.GetController()->GetControl("Rumble3"));
		if (!pRumble)
			pRumble = static_cast<Effect*>(cControl.GetController()->GetControl("Rumble1"));
		if (pRumble) {
			// Enable or disable rumble?
			if (cControl.GetName() == "Button1")
				pRumble->SetValue(1.0f);
			if (cControl.GetName() == "Button2")
				pRumble->SetValue(0.0f);
		}
	}
}

/**
*  @brief
*    Called when a control of the input controller has been activated
*/
void Application30::OnControlExit(Control &cControl)
{
	// Exit?
	if (cControl.GetName() == "Escape" || cControl.GetName() == "Q")
		m_bExit = true;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::AbstractLifecycle functions   ]
//[-------------------------------------------------------]
bool Application30::OnStart()
{
	// Call base implementation
	if (FrontendApplication::OnStart()) {
		// Detect input devices
		System::GetInstance()->GetConsole().Print("Detecting input devices: ");
		InputManager::GetInstance()->DetectDevices();
		System::GetInstance()->GetConsole().Print("done.\n");

		// Run test
		TestDevices();
	//	TestController();
	//	TestInput("Keyboard");
	//	TestInput("Mouse");
	//	TestInput("Joystick0");
	//	TestInput("WiiMote0");
	//	TestInput("SpaceMouse0");
	//	TestInput("SensorManager");
	//	TestGetChar("Keyboard");
		TestConnections();

		// Exit the application
		Exit(0);

		// Done
		return true;
	}

	// Error
	return false;
}
