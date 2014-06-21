/*********************************************************\
 *  File: WiiMote.cpp                                    *
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
#include "PLInput/Backend/ConnectionDevice.h"
#include "PLInput/Input/Devices/WiiMote.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(WiiMote)


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    WiiMote ports
*/
enum EPort {
	ControlPort				= 17,			/**< Control port (output) */
	InterruptPort			= 19			/**< Interrupt port (input) */
};

/**
*  @brief
*    WiiMote commands
*/
enum ECommand {
	CmdNone					= 0x00,			/**< No command */
	CmdLEDs					= 0x11,			/**< Read LEDs */
	CmdType					= 0x12,			/**< Set report mode */
	CmdIR					= 0x13,			/**< Enable IR */
	CmdSpeakerEnable		= 0x14,			/**< Enable speaker */
	CmdStatus				= 0x15,			/**< Get status */
	CmdWriteMemory			= 0x16,			/**< Write to memory */
	CmdReadMemory			= 0x17,			/**< Read from memory */
	CmdSpeakerData			= 0x18,			/**< Send speaker data */
	CmdSpeakerMute			= 0x19,			/**< Mute speaker */
	CmdIR2					= 0x1a			/**< Enable IR */
};

/**
*  @brief
*    WiiMote registers
*/
enum ERegister {
	RegCalibration			= 0x00000016,	/**< Calibration */
	RegIR					= 0x04b00030,	/**< IR */
	RegIRSensitivity1		= 0x04b00000,	/**< IR sensitivity (1) */
	RegIRSensitivity2		= 0x04b0001a,	/**< IR sensitivity (2) */
	RegIRMode				= 0x04b00033,	/**< IR mode */
	RegExtensionInit		= 0x04a40040,	/**< Extension initialization */
	RegExtensionType		= 0x04a400fe,	/**< Extension type */
	RegExtensionCalibration	= 0x04a40020	/**< Extension calibration */
};

/**
*  @brief
*    WiiMote buttons
*/
enum EButton {
	BtnLeft					= 0x0001,		/**< Button "Left" */
	BtnRight				= 0x0002,		/**< Button "Right" */
	BtnDown					= 0x0004,		/**< Button "Down" */
	BtnUp					= 0x0008,		/**< Button "Up" */
	BtnPlus					= 0x0010,		/**< Button "+" */
	Btn2					= 0x0100,		/**< Button "2" */
	Btn1					= 0x0200,		/**< Button "1" */
	BtnB					= 0x0400,		/**< Button "B" */
	BtnA					= 0x0800,		/**< Button "A" */
	BtnMinus				= 0x1000,		/**< Button "-" */
	BtnHome					= 0x8000		/**< Button "Home" */
};

/**
*  @brief
*    Nunchuk buttons
*/
enum ENunchukButton {
	BtnNunchukZ				= 0x0001,		/**< Button "Z" */
	BtnNunchukC				= 0x0002		/**< Button "C" */
};

// IR camera
static const int IR_MaxX	= 1016;			/**< Effective X Resolution of IR sensor */
static const int IR_MaxY	=  760;			/**< Effective Y Resolution of IR sensor */


//[-------------------------------------------------------]
//[ Global helper functions                               ]
//[-------------------------------------------------------]
bool ValueChanged(float fValueOld, float fValueNew)
{
	// Apply epsilon boundary
	return (Math::Abs(fValueNew - fValueOld) >= 0.01);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
WiiMote::WiiMote(const String &sName, DeviceImpl *pImpl) : Device(sName, "WiiMote controller", pImpl),
	LEDs			(this,	"LEDs",				"LEDs"),
	Rumble1			(this,	"Rumble1",			"Rumble motor #1"),
	Button1			(this,	"Button1",			"Button '1'"),
	Button2			(this,	"Button2",			"Button '2'"),
	ButtonA			(this,	"ButtonA",			"Button 'A'"),
	ButtonB			(this,	"ButtonB",			"Button 'B'"),
	ButtonMinus		(this,	"ButtonMinus",		"Button 'Minus'"),
	ButtonPlus		(this,	"ButtonPlus",		"Button 'Plus'"),
	ButtonHome		(this,	"ButtonHome",		"Button 'Home'"),
	ButtonLeft		(this,	"ButtonLeft",		"Button 'Left'"),
	ButtonRight		(this,	"ButtonRight",		"Button 'Right'"),
	ButtonUp		(this,	"ButtonUp",			"Button 'Up'"),
	ButtonDown		(this,	"ButtonDown",		"Button 'Down'"),
	AccX			(this,	"AccX",				"Acceleration axis (X)"),
	AccY			(this,	"AccY",				"Acceleration axis (Y)"),
	AccZ			(this,	"AccZ",				"Acceleration axis (Z)"),
	OrientX			(this,	"OrientX",			"Orientation axis (X)"),
	OrientY			(this,	"OrientY",			"Orientation axis (Y)"),
	OrientZ			(this,	"OrientZ",			"Orientation axis (Z)"),
	Roll			(this,	"Roll",				"Rotation (roll)"),
	Pitch			(this,	"Pitch",			"Rotation (pitch)"),
	PointerX		(this,	"PointerX",			"Pointer(X)"),
	PointerY		(this,	"PointerY",			"Pointer(Y)"),
	NunchukButtonC	(this,	"NunchukButtonC",	"Nunchuk button 'C'"),
	NunchukButtonZ	(this,	"NunchukButtonZ",	"Nunchuk button 'Z'"),
	NunchukAccX		(this,	"NunchukAccX",		"Nunchuk acceleration axis (X)"),
	NunchukAccY		(this,	"NunchukAccY",		"Nunchuk acceleration axis (Y)"),
	NunchukAccZ		(this,	"NunchukAccZ",		"Nunchuk acceleration axis (Z)"),
	NunchukOrientX	(this,	"NunchukOrientX",	"Nunchuk orientation axis (X)"),
	NunchukOrientY	(this,	"NunchukOrientY",	"Nunchuk orientation axis (Y)"),
	NunchukOrientZ	(this,	"NunchukOrientZ",	"Nunchuk orientation axis (Z)"),
	NunchukRoll		(this,	"NunchukRoll",		"Nunchuk rotation (roll)"),
	NunchukPitch	(this,	"NunchukPitch",		"Nunchuk rotation (pitch)"),
	NunchukX		(this,	"NunchukX",			"Nunchuk joystick (X)"),
	NunchukY		(this,	"NunchukY",			"Nunchuk joystick (Y)"),
	EventHandlerOnDeviceConnect	  (&WiiMote::OnDeviceConnect,	 this),
	EventHandlerOnDeviceDisconnect(&WiiMote::OnDeviceDisconnect, this),
	EventHandlerOnDeviceRead	  (&WiiMote::OnDeviceRead,		 this),
	m_pConnectionDevice(static_cast<ConnectionDevice*>(pImpl)),
	m_pInputBuffer(nullptr),
	m_pOutputBuffer(nullptr),
	m_nReportMode(ReportButtons),
	m_nIRMode(IROff),
	m_nExtension(ExtNone),
	m_nBattery(0),
	m_nLEDs(0),
	m_nRumble(0),
	m_nButtons(0),
	m_nNunchukButtons(0)
{
	// Set input and output report size
	if (m_pConnectionDevice->GetDeviceType() == ConnectionDevice::DeviceTypeBluetooth) {
		m_pConnectionDevice->SetInputReportSize(22);
		m_pConnectionDevice->SetOutputReportSize(22);
	}

	// Connect to HIDDevice events
	m_pConnectionDevice->EventOnConnect.Connect		(EventHandlerOnDeviceConnect);
	m_pConnectionDevice->EventOnDisconnect.Connect	(EventHandlerOnDeviceDisconnect);
	m_pConnectionDevice->EventOnRead.Connect		(EventHandlerOnDeviceRead);

	// Connect to device
	m_pConnectionDevice->Open(ControlPort, InterruptPort);

	// Init data
	m_sAcc.nUpdateNearG			 = 0;
	m_sNunchukAcc.nUpdateNearG	 = 0;
}

/**
*  @brief
*    Destructor
*/
WiiMote::~WiiMote()
{
	// We use m_pImpl here to check, because if the device backend has been deleted before, m_pImpl has
	// been reset to a null pointer, but not m_pConnectionDevice as this is unknown in the base class
	if (m_pImpl) {
		// Disconnect
		m_pConnectionDevice->Close();
	}
}

/**
*  @brief
*    Get report mode
*/
WiiMote::EReport WiiMote::GetReportMode() const
{
	// Return report mode
	return m_nReportMode;
}

/**
*  @brief
*    Set report mode
*/
void WiiMote::SetReportMode(EReport nReportMode, bool bContinuous)
{
	// Set report mode
	m_nReportMode = nReportMode;

	// Enable/Disable infrared sensor
	switch (m_nReportMode) {
		case ReportButtonsAccelIR:
			SetIRMode(IRExtended);
			break;

		case ReportButtonsAccelIRExt:
			SetIRMode(IRBasic);
			break;

		case ReportStatus:
		case ReportReadMemory:
		case ReportButtons:
		case ReportButtonsAccel:
		case ReportButtonsAccelExt:
		default:
			SetIRMode(IROff);
			break;
	}

	// Set new mode
	ClearReport();
	m_pOutputBuffer[0] = CmdType;
	m_pOutputBuffer[1] = (bContinuous ? 0x04 : 0x00) | m_nRumble;
	m_pOutputBuffer[2] = static_cast<uint8>(nReportMode);
	Send(m_pOutputBuffer, 3);
}

/**
*  @brief
*    Get infrared mode
*/
WiiMote::EIRMode WiiMote::GetIRMode() const
{
	// Return IR mode
	return m_nIRMode;
}

/**
*  @brief
*    Set infrared mode
*/
void WiiMote::SetIRMode(EIRMode nIRMode)
{
	// Set IR mode
	m_nIRMode = nIRMode;

	// Set new mode
	if (m_nIRMode != IROff) {
		ClearReport();
		m_pOutputBuffer[0] = CmdIR;
		m_pOutputBuffer[1] = 0x04 | m_nRumble;
		Send(m_pOutputBuffer, 2);
		m_pOutputBuffer[0] = CmdIR2;
		m_pOutputBuffer[1] = 0x04 | m_nRumble;
		Send(m_pOutputBuffer, 2);

		static const uint8 ir_sens1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0xc0};
		static const uint8 ir_sens2[] = {0x40, 0x00};

		WriteMemory(RegIR, 0x08);
		WriteMemory(RegIRSensitivity1, ir_sens1, sizeof(ir_sens1));
		WriteMemory(RegIRSensitivity2, ir_sens2, sizeof(ir_sens2));
		WriteMemory(RegIRMode, static_cast<uint8>(nIRMode));
	} else {
		ClearReport();
		m_pOutputBuffer[0] = CmdIR;
		m_pOutputBuffer[1] = m_nRumble;
		Send(m_pOutputBuffer, 2);
		m_pOutputBuffer[0] = CmdIR2;
		m_pOutputBuffer[1] = m_nRumble;
		Send(m_pOutputBuffer, 2);
	}
}

/**
*  @brief
*    Get extension
*/
WiiMote::EExtension WiiMote::GetExtension() const
{
	return m_nExtension;
}

/**
*  @brief
*    Get battery state
*/
uint8 WiiMote::GetBattery() const
{
	return m_nBattery;
}

/**
*  @brief
*    Calibrate device
*/
void WiiMote::Calibrate()
{
	SendCalibrationRequest();
}


//[-------------------------------------------------------]
//[ Public virtual Controller functions                   ]
//[-------------------------------------------------------]
void WiiMote::UpdateOutputControl(Control *pControl)
{
	// Update LEDs
	if (pControl == &LEDs) {
		// Set LEDs
		m_nLEDs = static_cast<uint8>(LEDs.GetLEDs()) & 0x0f;

		// Send command
		ClearReport();
		m_pOutputBuffer[0] = CmdLEDs;
		m_pOutputBuffer[1] = (m_nLEDs << 4) | m_nRumble;
		Send(m_pOutputBuffer, 2);

	// Update rumble
	} else if (pControl == &Rumble1) {
		// Switch rumble on or off?
		if (Rumble1.GetValue() > 0) {
			// Enable rumble
			m_nRumble = 1;
			SendStatusRequest();
		} else {
			// Disable rumble
			m_nRumble = 0;
			SendStatusRequest();
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the HID device has been connected
*/
void WiiMote::OnDeviceConnect()
{
	m_pInputBuffer  = m_pConnectionDevice->GetInputBuffer();
	m_pOutputBuffer = m_pConnectionDevice->GetOutputBuffer();

	SendStatusRequest();
	SendCalibrationRequest();
	SetReportMode(ReportButtonsAccelIR, false);
}

/**
*  @brief
*    Called when the HID device has been disconnected
*/
void WiiMote::OnDeviceDisconnect()
{
}

/**
*  @brief
*    Called when the HID device has read some data
*/
void WiiMote::OnDeviceRead()
{
	// Read data
	OnReadData();

	// Propagate changes
	m_bChanged = true;
}

/**
*  @brief
*    Data from the WiiMote has been received
*/
void WiiMote::OnReadData()
{
	// What kind of data has been received?
	switch (m_pInputBuffer[0]) {
		// Get buttons
		case ReportButtons:
			OnReadButtons();
			break;

		// Get buttons and acceleration
		case ReportButtonsAccel:
			OnReadButtons();
			OnReadAccel();
			break;

		// Get buttons, acceleration and extension
		case ReportButtonsAccelExt:
			OnReadButtons();
			OnReadAccel();
			DecryptBuffer(0, m_pConnectionDevice->GetInputReportSize());
			OnReadExtension(6);
			break;

		// Get buttons, acceleration and IR (extended mode)
		case ReportButtonsAccelIR:
			OnReadButtons();
			OnReadAccel();
			OnReadIR();
			break;

		// Get buttons, acceleration, IR (basic mode) and extension
		case ReportButtonsAccelIRExt:
			OnReadButtons();
			OnReadAccel();
			OnReadIR();
			DecryptBuffer(0, m_pConnectionDevice->GetInputReportSize());
			OnReadExtension(16);
			break;

		// Get data from memory
		case ReportReadMemory:
			OnReadButtons();
			OnReadMemory();
			break;

		// Get status report
		case ReportStatus:
			OnReadStatus();
			break;
	}
}

/**
*  @brief
*    Data from memory received
*/
void WiiMote::OnReadMemory()
{
	// Check memory address
	if ((m_pInputBuffer[3] & 0x08) != 0) {
		// Error: Invalid read-address
		return;
	} else if((m_pInputBuffer[3] & 0x07) != 0) {
		// Error: Attempt to read from write-only registers
		return;
	}

	// Get size
	int nSize = m_pInputBuffer[3] >> 4;

	// Get address
	int nAddress = m_pInputBuffer[4]<<8 | m_pInputBuffer[5];

	// *NOTE*: this is a major (but convenient) hack!  The returned data only
	//          contains the lower two bytes of the address that was queried.
	//			as these don't collide between any of the addresses/registers
	//			we currently read, it's OK to match just those two bytes
	switch (nAddress) {
		case (RegCalibration & 0xffff):
			if (nSize != 6)
				break;	// Error! Wrong size ...
			OnReadCalibration();
			break;

		case (RegExtensionType & 0xffff):
			if (nSize != 1)
				break;	// Error! Wrong size ...
			OnReadExtensionType();
			break;

		case (RegExtensionCalibration & 0xffff):
			if (nSize != 15)
				break;	// Error! Wrong size ...
			DecryptBuffer(6, 16);
			if (m_nExtension == ExtNunchuk)
				OnReadNunchukCalibration();
			else if (m_nExtension == ExtClassic)
				OnReadClassicCalibration();
			break;
	}
}

/**
*  @brief
*    Calibration data received
*/
void WiiMote::OnReadCalibration()
{
	// Get calibration info
	m_sAcc.nX0 = m_pInputBuffer[6+0];
	m_sAcc.nY0 = m_pInputBuffer[6+1];
	m_sAcc.nZ0 = m_pInputBuffer[6+2];
	m_sAcc.nXG = m_pInputBuffer[6+4];
	m_sAcc.nYG = m_pInputBuffer[6+5];
	m_sAcc.nZG = m_pInputBuffer[6+6];
}

/**
*  @brief
*    Extension information received
*/
void WiiMote::OnReadExtensionType()
{
	// Get extension type
	uint16 nExtension = *reinterpret_cast<uint16*>(&m_pInputBuffer[6]);

	// If new extension, get calibration info
	if (nExtension == ExtNunchuk && m_nExtension != ExtNunchuk) {
		// Nunchuk registered
		ReadMemory(RegExtensionCalibration, 16);
		m_nExtension = ExtNunchuk;
	} else if (nExtension == ExtClassic && m_nExtension != ExtClassic) {
		// Classic-controller registered
		ReadMemory(RegExtensionCalibration, 16);
		m_nExtension = ExtClassic;
	} else if (nExtension == ExtPartiallyInserted && m_nExtension != ExtPartiallyInserted) {
		// Extension not registered correctly
		m_nExtension = ExtPartiallyInserted;
		SendStatusRequest();
	} else {
		// Unknown extension
	}
}

/**
*  @brief
*    Nunchuk calibration data received
*/
void WiiMote::OnReadNunchukCalibration()
{
	// Get nunchuk calibration
	m_sNunchukAcc.nX0	 = m_pInputBuffer[6+0];
	m_sNunchukAcc.nY0	 = m_pInputBuffer[6+1];
	m_sNunchukAcc.nZ0	 = m_pInputBuffer[6+2];
	m_sNunchukAcc.nXG	 = m_pInputBuffer[6+4];
	m_sNunchukAcc.nYG	 = m_pInputBuffer[6+5];
	m_sNunchukAcc.nZG	 = m_pInputBuffer[6+6];
	m_sNunchukJoy.nMaxX  = m_pInputBuffer[6+8];
	m_sNunchukJoy.nMinX  = m_pInputBuffer[6+9];
	m_sNunchukJoy.nMidX  = m_pInputBuffer[6+10];
	m_sNunchukJoy.nMaxY  = m_pInputBuffer[6+11];
	m_sNunchukJoy.nMinY  = m_pInputBuffer[6+12];
	m_sNunchukJoy.nMidY  = m_pInputBuffer[6+13];

	// Reset report mode
	SetReportMode(m_nReportMode, false);
}

/**
*  @brief
*    Classic calibration data received
*/
void WiiMote::OnReadClassicCalibration()
{
	/*
	Internal.ClassicController.CalibrationInfo.MaxXL = m_pInputBuffer[6+ 0] >> 2;
	Internal.ClassicController.CalibrationInfo.MinXL = m_pInputBuffer[6+ 1] >> 2;
	Internal.ClassicController.CalibrationInfo.MidXL = m_pInputBuffer[6+ 2] >> 2;
	Internal.ClassicController.CalibrationInfo.MaxYL = m_pInputBuffer[6+ 3] >> 2;
	Internal.ClassicController.CalibrationInfo.MinYL = m_pInputBuffer[6+ 4] >> 2;
	Internal.ClassicController.CalibrationInfo.MidYL = m_pInputBuffer[6+ 5] >> 2;
	Internal.ClassicController.CalibrationInfo.MaxXR = m_pInputBuffer[6+ 6] >> 3;
	Internal.ClassicController.CalibrationInfo.MinXR = m_pInputBuffer[6+ 7] >> 3;
	Internal.ClassicController.CalibrationInfo.MidXR = m_pInputBuffer[6+ 8] >> 3;
	Internal.ClassicController.CalibrationInfo.MaxYR = m_pInputBuffer[6+ 9] >> 3;
	Internal.ClassicController.CalibrationInfo.MinYR = m_pInputBuffer[6+10] >> 3;
	Internal.ClassicController.CalibrationInfo.MidYR = m_pInputBuffer[6+11] >> 3;
	// this doesn't seem right...
	//	Internal.ClassicController.CalibrationInfo.MinTriggerL = m_pInputBuffer[6+12] >> 3;
	//	Internal.ClassicController.CalibrationInfo.MaxTriggerL = m_pInputBuffer[6+14] >> 3;
	//	Internal.ClassicController.CalibrationInfo.MinTriggerR = m_pInputBuffer[6+13] >> 3;
	//	Internal.ClassicController.CalibrationInfo.MaxTriggerR = m_pInputBuffer[6+15] >> 3;
	Internal.ClassicController.CalibrationInfo.MinTriggerL = 0;
	Internal.ClassicController.CalibrationInfo.MaxTriggerL = 31;
	Internal.ClassicController.CalibrationInfo.MinTriggerR = 0;
	Internal.ClassicController.CalibrationInfo.MaxTriggerR = 31;
	SetReportType(ReportType);
	*/
}

/**
*  @brief
*    Status information received
*/
void WiiMote::OnReadStatus()
{
	// Read button state
	OnReadButtons();

	// Get battery
	m_nBattery = m_pInputBuffer[6] / 2;

	// Get LEDs
	m_nLEDs = m_pInputBuffer[3] >> 4;

	// Get extension
	bool bExtension = ((m_pInputBuffer[3] & 0x02) != 0);
	if ((m_nExtension == ExtNone || m_nExtension == ExtPartiallyInserted) && bExtension) {
		// Initialize extension
		WriteMemory(RegExtensionInit, 0x00);
		ReadMemory (RegExtensionType, 2);
	}
}

/**
*  @brief
*    Button status received
*/
void WiiMote::OnReadButtons()
{
	// Get state of buttons
	m_nButtons = *reinterpret_cast<uint16*>(&m_pInputBuffer[1]);

	// Button '1'
	bool bPressed;
	bPressed = ((m_nButtons & Btn1) != 0);
	if (Button1.IsPressed() != bPressed)
		Button1.SetPressed(bPressed);

	// Button '2'
	bPressed = ((m_nButtons & Btn2) != 0);
	if (Button2.IsPressed() != bPressed)
		Button2.SetPressed(bPressed);

	// Button 'A'
	bPressed = ((m_nButtons & BtnA) != 0);
	if (ButtonA.IsPressed() != bPressed)
		ButtonA.SetPressed(bPressed);

	// Button 'B'
	bPressed = ((m_nButtons & BtnB) != 0);
	if (ButtonB.IsPressed() != bPressed)
		ButtonB.SetPressed(bPressed);

	// Button 'Minus'
	bPressed = ((m_nButtons & BtnMinus) != 0);
	if (ButtonMinus.IsPressed() != bPressed)
		ButtonMinus.SetPressed(bPressed);

	// Button 'Plus'
	bPressed = ((m_nButtons & BtnPlus) != 0);
	if (ButtonPlus.IsPressed() != bPressed)
		ButtonPlus.SetPressed(bPressed);

	// Button 'Home'
	bPressed = ((m_nButtons & BtnHome) != 0);
	if (ButtonHome.IsPressed() != bPressed)
		ButtonHome.SetPressed(bPressed);

	// Button 'Left'
	bPressed = ((m_nButtons & BtnLeft) != 0);
	if (ButtonLeft.IsPressed() != bPressed)
		ButtonLeft.SetPressed(bPressed);

	// Button 'Right'
	bPressed = ((m_nButtons & BtnRight) != 0);
	if (ButtonRight.IsPressed() != bPressed)
		ButtonRight.SetPressed(bPressed);

	// Button 'Up'
	bPressed = ((m_nButtons & BtnUp) != 0);
	if (ButtonUp.IsPressed() != bPressed)
		ButtonUp.SetPressed(bPressed);

	// Button 'Down'
	bPressed = ((m_nButtons & BtnDown) != 0);
	if (ButtonDown.IsPressed() != bPressed)
		ButtonDown.SetPressed(bPressed);
}

/**
*  @brief
*    Acceleration status received
*/
void WiiMote::OnReadAccel()
{
	// Get raw acceleration data
	uint8 nRawX = m_pInputBuffer[3];
	uint8 nRawY = m_pInputBuffer[4];
	uint8 nRawZ = m_pInputBuffer[5];

	// Compute acceleration
	m_sAcc.fAccX = static_cast<float>(nRawX - m_sAcc.nX0) / static_cast<float>(m_sAcc.nXG - m_sAcc.nX0);
	m_sAcc.fAccY = static_cast<float>(nRawY - m_sAcc.nY0) / static_cast<float>(m_sAcc.nYG - m_sAcc.nY0);
	m_sAcc.fAccZ = static_cast<float>(nRawZ - m_sAcc.nZ0) / static_cast<float>(m_sAcc.nZG - m_sAcc.nZ0);

		// AccX
		if (ValueChanged(AccX.GetValue(), m_sAcc.fAccX))
			AccX.SetValue(m_sAcc.fAccX, false);

		// AccY
		if (ValueChanged(AccY.GetValue(), m_sAcc.fAccY))
			AccY.SetValue(m_sAcc.fAccY, false);

		// AccZ
		if (ValueChanged(AccZ.GetValue(), m_sAcc.fAccZ))
			AccZ.SetValue(m_sAcc.fAccZ, false);

	// Compute orientation
	m_sAcc.CalculateOrientation();

		// OrientX
		if (ValueChanged(OrientX.GetValue(), m_sAcc.fOrientX))
			OrientX.SetValue(m_sAcc.fOrientX, false);

		// OrientY
		if (ValueChanged(OrientY.GetValue(), m_sAcc.fOrientY))
			OrientY.SetValue(m_sAcc.fOrientY, false);

		// OrientZ
		if (ValueChanged(OrientZ.GetValue(), m_sAcc.fOrientZ))
			OrientZ.SetValue(m_sAcc.fOrientZ, false);

		// Roll
		if (ValueChanged(Roll.GetValue(), m_sAcc.fRoll))
			Roll.SetValue(m_sAcc.fRoll, false);

		// Pitch
		if (ValueChanged(Pitch.GetValue(), m_sAcc.fPitch))
			Pitch.SetValue(m_sAcc.fPitch, false);
}

/**
*  @brief
*    IR status received
*/
void WiiMote::OnReadIR()
{
	// Get IR mode
	switch (m_nIRMode) {
		// Basic mode
		case IRBasic:
			m_sDots[0].bFound = !(m_pInputBuffer[6] == 0xff && m_pInputBuffer[ 7] == 0xff);
			m_sDots[1].bFound = !(m_pInputBuffer[9] == 0xff && m_pInputBuffer[10] == 0xff);
			if (m_sDots[1].bFound) {
				m_sDots[1].nRawX = m_pInputBuffer[ 9] | ((m_pInputBuffer[8] >> 0) & 0x03) << 8;
				m_sDots[1].nRawY = m_pInputBuffer[10] | ((m_pInputBuffer[8] >> 2) & 0x03) << 8;
			}
			m_sDots[0].nSize = 0;
			m_sDots[1].nSize = 0;
			break;
		
		// Extended mode
		case IRExtended:
			m_sDots[0].bFound = !(m_pInputBuffer[6]==0xff && m_pInputBuffer[ 7]==0xff && m_pInputBuffer[ 8]==0xff);
			m_sDots[1].bFound = !(m_pInputBuffer[9]==0xff && m_pInputBuffer[10]==0xff && m_pInputBuffer[11]==0xff);
			if (m_sDots[0].bFound)
				m_sDots[0].nSize = m_pInputBuffer[8] & 0x0f;
			if (m_sDots[1].bFound) {
				m_sDots[1].nRawX = m_pInputBuffer[ 9] | ((m_pInputBuffer[11] >> 4) & 0x03) << 8;
				m_sDots[1].nRawY = m_pInputBuffer[10] | ((m_pInputBuffer[11] >> 6) & 0x03) << 8;
				m_sDots[1].nSize = m_pInputBuffer[11] & 0x0f;
			}
			break;

		// Full mode
		case IRFull:
			// Unimplemented
			return;

		// Off/unknown
		case IROff:
		default:
			return;
	}

	// Check first dot
	if (m_sDots[0].bFound) {
		m_sDots[0].nRawX = m_pInputBuffer[6] | ((m_pInputBuffer[8] >> 4) & 0x03) << 8;
		m_sDots[0].nRawY = m_pInputBuffer[7] | ((m_pInputBuffer[8] >> 6) & 0x03) << 8;
		m_sDots[0].fX    = 1.f - (m_sDots[0].nRawX / static_cast<float>(IR_MaxX));
		m_sDots[0].fY    =	     (m_sDots[0].nRawY / static_cast<float>(IR_MaxY));
	}

	// Check second dot
	if (m_sDots[1].bFound) {
		m_sDots[1].fX = 1.f - (m_sDots[1].nRawX / static_cast<float>(IR_MaxX));
		m_sDots[1].fY =	      (m_sDots[1].nRawY / static_cast<float>(IR_MaxY));
	}

	// Compute IR center
	m_vIRPos.x = (m_sDots[0].fX + m_sDots[1].fX) / 2;
	m_vIRPos.y = (m_sDots[0].fY + m_sDots[1].fY) / 2;

		// PointerX
		if (ValueChanged(PointerX.GetValue(), m_vIRPos.x))
			PointerX.SetValue(m_vIRPos.x, false);

		// PointerY
		if (ValueChanged(PointerY.GetValue(), m_vIRPos.y))
			PointerY.SetValue(m_vIRPos.y, false);
}

/**
*  @brief
*    Extension status received
*/
void WiiMote::OnReadExtension(uint32 nOffset)
{
	// Check extension type
	switch (m_nExtension) {
		// Nunchuk
		case ExtNunchuk:
			OnReadNunchuk(nOffset);
			break;

		// Classic controller
		case ExtClassic:
			OnReadClassic(nOffset);
			break;

		case ExtNone:
		case ExtPartiallyInserted:
		default:
			// [TODO] Review this: Do we need to handle this case?
			break;
	}
}

/**
*  @brief
*    Nunchuk status received
*/
void WiiMote::OnReadNunchuk(uint32 nOffset)
{
	// Get buttons
	m_nNunchukButtons = 0;
	if ((m_pInputBuffer[nOffset+5] & 0x02) == 0)
		m_nNunchukButtons |= BtnNunchukC;
	if ((m_pInputBuffer[nOffset+5] & 0x01) == 0)
		m_nNunchukButtons |= BtnNunchukZ;

		// Button 'C'
		bool bPressed;
		bPressed = ((m_nButtons & BtnNunchukC) != 0);
		if (NunchukButtonC.IsPressed() != bPressed)
			NunchukButtonC.SetPressed(bPressed);

		// Button 'Z'
		bPressed = ((m_nButtons & BtnNunchukZ) != 0);
		if (NunchukButtonZ.IsPressed() != bPressed)
			NunchukButtonZ.SetPressed(bPressed);

	// Get raw acceleration data
	uint8 nRawX = m_pInputBuffer[nOffset+2];
	uint8 nRawY = m_pInputBuffer[nOffset+3];
	uint8 nRawZ = m_pInputBuffer[nOffset+4];

	// Compute acceleration
	m_sNunchukAcc.fAccX = (static_cast<float>(nRawX) - m_sNunchukAcc.nX0) / (static_cast<float>(m_sNunchukAcc.nXG) - m_sNunchukAcc.nX0);
	m_sNunchukAcc.fAccY = (static_cast<float>(nRawY) - m_sNunchukAcc.nY0) / (static_cast<float>(m_sNunchukAcc.nYG) - m_sNunchukAcc.nY0);
	m_sNunchukAcc.fAccZ = (static_cast<float>(nRawZ) - m_sNunchukAcc.nZ0) / (static_cast<float>(m_sNunchukAcc.nZG) - m_sNunchukAcc.nZ0);

		// AccX
		if (ValueChanged(NunchukAccX.GetValue(), m_sNunchukAcc.fAccX))
			NunchukAccX.SetValue(m_sNunchukAcc.fAccX, false);

		// AccY
		if (ValueChanged(NunchukAccY.GetValue(), m_sNunchukAcc.fAccY))
			NunchukAccY.SetValue(m_sNunchukAcc.fAccY, false);

		// AccZ
		if (ValueChanged(NunchukAccZ.GetValue(), m_sNunchukAcc.fAccZ))
			NunchukAccZ.SetValue(m_sNunchukAcc.fAccZ, false);

	// Compute orientation
	m_sNunchukAcc.CalculateOrientation();

		// OrientX
		if (ValueChanged(NunchukOrientX.GetValue(), m_sNunchukAcc.fOrientX))
			NunchukOrientX.SetValue(m_sNunchukAcc.fOrientX, false);

		// OrientY
		if (ValueChanged(NunchukOrientY.GetValue(), m_sNunchukAcc.fOrientY))
			NunchukOrientY.SetValue(m_sNunchukAcc.fOrientY, false);

		// OrientZ
		if (ValueChanged(NunchukOrientZ.GetValue(), m_sNunchukAcc.fOrientZ))
			NunchukOrientZ.SetValue(m_sNunchukAcc.fOrientZ, false);

		// Roll
		if (ValueChanged(NunchukRoll.GetValue(), m_sNunchukAcc.fRoll))
			NunchukRoll.SetValue(m_sNunchukAcc.fRoll, false);

		// Pitch
		if (ValueChanged(NunchukPitch.GetValue(), m_sNunchukAcc.fPitch))
			NunchukPitch.SetValue(m_sNunchukAcc.fPitch, false);

	// Get raw joystick position
	uint8 nJoyRawX = m_pInputBuffer[nOffset+0];
	uint8 nJoyRawY = m_pInputBuffer[nOffset+1];

	// Compute joystick position
	if (m_sNunchukJoy.nMaxX != 0x00) {
		m_sNunchukJoy.fX  = (static_cast<float>(nJoyRawX) - m_sNunchukJoy.nMidX) / (static_cast<float>(m_sNunchukJoy.nMaxX) - m_sNunchukJoy.nMinX);
		m_sNunchukJoy.fX *= 2.0f;
	}

	if (m_sNunchukJoy.nMaxY != 0x00) {
		m_sNunchukJoy.fY = (static_cast<float>(nJoyRawY) - m_sNunchukJoy.nMidY) / (static_cast<float>(m_sNunchukJoy.nMaxY) - m_sNunchukJoy.nMinY);
		m_sNunchukJoy.fY *= 2.0f;
	}

		// NunchukX
		if (ValueChanged(NunchukX.GetValue(), m_sNunchukJoy.fX))
			NunchukX.SetValue(m_sNunchukJoy.fX, false);

		// NunchukY
		if (ValueChanged(NunchukY.GetValue(), m_sNunchukJoy.fY))
			NunchukY.SetValue(m_sNunchukJoy.fY, false);
}

/**
*  @brief
*    Classic-controller status received
*/
void WiiMote::OnReadClassic(uint32 nOffset)
{
	/*
	// buttons:
	uint16 bits = *(uint16*)(m_pInputBuffer+4);

	// joysticks:
	wiimote_state::joystick &joyL = Internal.ClassicController.JoystickL;
	wiimote_state::joystick &joyR = Internal.ClassicController.JoystickR;

	// copy the current joystick state to detect changes
	wiimote_state::joystick curr_joy_l = joyL;
	wiimote_state::joystick curr_joy_r = joyR;

	joyL.RawX = (float) (m_pInputBuffer[nOffset+0] & 0x3f);
	joyL.RawY = (float) (m_pInputBuffer[nOffset+1] & 0x3f);
	joyR.RawX = (float)((m_pInputBuffer[nOffset+2]			>> 7) |
					   ((m_pInputBuffer[nOffset+1] & 0xc0) >> 5) |
					   ((m_pInputBuffer[nOffset+0] & 0xc0) >> 3));
	joyR.RawY = (float) (m_pInputBuffer[nOffset+2] & 0x1f);

	float xr, yr, xl, yl;
	if (Internal.ClassicController.CalibrationInfo.MaxXL != 0x00)
		xl = ((float)joyL.RawX - Internal.ClassicController.CalibrationInfo.MidXL) / 
			 ((float)Internal.ClassicController.CalibrationInfo.MaxXL -
					 Internal.ClassicController.CalibrationInfo.MinXL);

	if (Internal.ClassicController.CalibrationInfo.MaxYL != 0x00)
		yl = ((float)joyL.RawY - Internal.ClassicController.CalibrationInfo.MidYL) / 
			 ((float)Internal.ClassicController.CalibrationInfo.MaxYL -
					 Internal.ClassicController.CalibrationInfo.MinYL);

	if (Internal.ClassicController.CalibrationInfo.MaxXR != 0x00)
		xr = ((float)joyR.RawX - Internal.ClassicController.CalibrationInfo.MidXR) / 
			 ((float)Internal.ClassicController.CalibrationInfo.MaxXR -
				     Internal.ClassicController.CalibrationInfo.MinXR);

	if (Internal.ClassicController.CalibrationInfo.MaxYR != 0x00)
		yr = ((float)joyR.RawY - Internal.ClassicController.CalibrationInfo.MidYR) / 
			 ((float)Internal.ClassicController.CalibrationInfo.MaxYR -
					 Internal.ClassicController.CalibrationInfo.MinYR);

	// i prefer the joystick outputs to range -1 - +1 (note this also affects
	//  the deadzone calculations)
	xl *= 2; yl *= 2; xr *= 2; yr *= 2;

	// apply the dead zones (if any) and write the final values to the state
	ApplyJoystickDeadZonesAndWrite(joyL, xl, yl);
	ApplyJoystickDeadZonesAndWrite(joyR, xr, yr);

	// have the joystick states changed?
	if (memcmp(&curr_joy_l, &joyL, sizeof(curr_joy_l)) != 0)
		changed |= CLASSIC_JOYSTICK_L_CHANGED;
	if (memcmp(&curr_joy_r, &joyR, sizeof(curr_joy_r)) != 0)
		changed |= CLASSIC_JOYSTICK_R_CHANGED;

	// triggers
	uint8 raw_trigger_l = ((m_pInputBuffer[nOffset+2] & 0x60) >> 2) |
						   (m_pInputBuffer[nOffset+3]		  >> 5);
	uint8 raw_trigger_r =   m_pInputBuffer[nOffset+3] & 0x1f;
			
	if ((raw_trigger_l != Internal.ClassicController.RawTriggerL) ||
	    (raw_trigger_r != Internal.ClassicController.RawTriggerR))
		   changed |= CLASSIC_TRIGGERS_CHANGED;
			
	Internal.ClassicController.RawTriggerL  = raw_trigger_l;
	Internal.ClassicController.RawTriggerR  = raw_trigger_r;

	if (Internal.ClassicController.CalibrationInfo.MaxTriggerL != 0x00)
		Internal.ClassicController.TriggerL =
				 (float)Internal.ClassicController.RawTriggerL / 
				((float)Internal.ClassicController.CalibrationInfo.MaxTriggerL -
						Internal.ClassicController.CalibrationInfo.MinTriggerL);

	if (Internal.ClassicController.CalibrationInfo.MaxTriggerR != 0x00)
		Internal.ClassicController.TriggerR =
				 (float)Internal.ClassicController.RawTriggerR / 
				((float)Internal.ClassicController.CalibrationInfo.MaxTriggerR -
						Internal.ClassicController.CalibrationInfo.MinTriggerR);
	*/
}

/**
*  @brief
*    Read from WiiMote memory
*/
void WiiMote::ReadMemory(int nAddress, uint8 nSize)
{
	// Send command
	ClearReport();
	m_pOutputBuffer[0] = CmdReadMemory;
	m_pOutputBuffer[1] = static_cast<uint8>((nAddress	& 0xff000000) >> 24) | m_nRumble;
	m_pOutputBuffer[2] = static_cast<uint8>((nAddress	& 0x00ff0000) >> 16);
	m_pOutputBuffer[3] = static_cast<uint8>((nAddress	& 0x0000ff00) >>  8);
	m_pOutputBuffer[4] = static_cast<uint8>((nAddress	& 0x000000ff));
	m_pOutputBuffer[5] = static_cast<uint8>((nSize		& 0xff00	) >>  8);
	m_pOutputBuffer[6] = static_cast<uint8>((nSize		& 0xff));
	Send(m_pOutputBuffer, 7);
}

/**
*  @brief
*    Write to WiiMote memory
*/
void WiiMote::WriteMemory(int nAddress, const uint8* pBuffer, uint8 nSize)
{
	// Check buffer
	if (pBuffer && nSize <= 16) {
		// Clear write buffer
		MemoryManager::Set(m_pOutputBuffer, 0, m_pConnectionDevice->GetOutputReportSize());

		// Set command
		ClearReport();
		m_pOutputBuffer[0] = CmdWriteMemory;
		m_pOutputBuffer[1] = static_cast<uint8>(((nAddress & 0xff000000) >> 24) | m_nRumble);
		m_pOutputBuffer[2] = static_cast<uint8>( (nAddress & 0x00ff0000) >> 16);
		m_pOutputBuffer[3] = static_cast<uint8>( (nAddress & 0x0000ff00) >>  8);
		m_pOutputBuffer[4] = static_cast<uint8>( (nAddress & 0x000000ff));
		m_pOutputBuffer[5] = nSize;
		MemoryManager::Copy(m_pOutputBuffer+6, pBuffer, nSize);

		// Send command
		m_pConnectionDevice->Write(m_pOutputBuffer, m_pConnectionDevice->GetOutputReportSize());
	}
}

/**
*  @brief
*    Write a single byte to WiiMote memory
*/
void WiiMote::WriteMemory(int nAddress, uint8 nData)
{
	// Write one byte
	WriteMemory(nAddress, &nData, 1);
}

/**
*  @brief
*    Decrypt data
*/
void WiiMote::DecryptBuffer(uint32 nOffset, uint32 nSize)
{
	// Decrypt buffer
	for (unsigned i=0; i<nSize; i++)
		m_pInputBuffer[nOffset+i] = ((m_pInputBuffer[nOffset+i] ^ 0x17) + 0x17) & 0xff;
}

/**
*  @brief
*    Clear output report
*/
void WiiMote::ClearReport()
{
	// Clear report
	MemoryManager::Set(m_pOutputBuffer, 0, 22);
}

/**
*  @brief
*    Send data to WiiMote device
*/
void WiiMote::Send(uint8 *pBuffer, uint32 nSize)
{
	// This is kinda strange. On Windows, using the HID API, a whole output report of size 22 has to be sent to the device,
	// otherwise strange things happen. On Linux, using the BlueZ API, only as many data as needed has to be sent, otherwise
	// we get a ResultErrInvalidParameter error.

	// Send data to device
	if (m_pConnectionDevice->GetDeviceType() == ConnectionDevice::DeviceTypeBluetooth)
		m_pConnectionDevice->Write(pBuffer, nSize);
	else
		m_pConnectionDevice->Write(pBuffer, 22);
}

/**
*  @brief
*    Get status from WiiMote command
*/
void WiiMote::SendStatusRequest()
{
	ClearReport();
	m_pOutputBuffer[0] = CmdStatus;
	m_pOutputBuffer[1] = m_nRumble;
	Send(m_pOutputBuffer, 2);
}

/**
*  @brief
*    Get calibration information
*/
void WiiMote::SendCalibrationRequest()
{
	ReadMemory(RegCalibration, 7);
}


// Calculate orientation from acceleration data
void WiiMote::SAcceleration::CalculateOrientation()
{
	// Calculate orientation from acceleration data
	static const float fEpsilon = 0.2f;
	float fSquareLen =  fAccX*fAccX + fAccY*fAccY + fAccZ*fAccZ;
	if ((fSquareLen >= (1.0f - fEpsilon)) && (fSquareLen <= (1.0f + fEpsilon))) {
		// Is the acceleration near 1G for at least 2 update cycles?
		if (++nUpdateNearG >= 2) {
			// Normalize vector
			float inv_len = 1.f / Math::Sqrt(fSquareLen);
			fOrientX = fAccX * inv_len;
			fOrientY = fAccY * inv_len;
			fOrientZ = fAccZ * inv_len;

			// Get pitch and roll angles
			fPitch = -Math::ASin(fOrientY) * 57.2957795f;
			fRoll  =  Math::ASin(fOrientX) * 57.2957795f;
			if (fOrientZ < 0) {
				fPitch = (fOrientY < 0 ?  180 - fPitch : -180 - fPitch);
				fRoll  = (fOrientX < 0 ? -180 - fRoll  :  180 - fRoll);
			}
		}
	} else {
		// Reset update counter
		nUpdateNearG = 0;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
