/*********************************************************\
 *  File: WiiMote.h                                      *
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


#ifndef __PLINPUT_WIIMOTE_H__
#define __PLINPUT_WIIMOTE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLMath/Vector3.h>
#include <PLCore/Base/Event/EventHandler.h>
#include "PLInput/Input/Controls/Button.h"
#include "PLInput/Input/Controls/Axis.h"
#include "PLInput/Input/Controls/LED.h"
#include "PLInput/Input/Controls/Effect.h"
#include "PLInput/Input/Devices/Device.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ConnectionDevice;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    WiiMote input device
*
*  @remarks
*    This class supports the following device backend types:
*    - ConnectionDevice
*/
class WiiMote : public Device {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, WiiMote, "PLInput", PLInput::Device, "WiiMote input controller")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Controller definition                                 ]
	//[-------------------------------------------------------]
	public:
		// LEDs and effects
		LED		LEDs;				/**< LEDs */
		Effect	Rumble1;			/**< Rumble effect (motor #1) */

		// Main buttons
		Button  Button1;			/**< Button '1' */
		Button  Button2;			/**< Button '2' */
		Button  ButtonA;			/**< Button 'A' */
		Button  ButtonB;			/**< Button 'B' */
		Button  ButtonMinus;		/**< Button 'Minus' */
		Button  ButtonPlus;			/**< Button 'Plus' */
		Button  ButtonHome;			/**< Button 'Home' */
		Button  ButtonLeft;			/**< Button 'Left' */
		Button  ButtonRight;		/**< Button 'Right' */
		Button  ButtonUp;			/**< Button 'Up' */
		Button  ButtonDown;			/**< Button 'Down' */

		// Main values
		Axis	AccX;				/**< Acceleration (X) */
		Axis	AccY;				/**< Acceleration (Y) */
		Axis	AccZ;				/**< Acceleration (Z) */
		Axis	OrientX;			/**< Orientation (X) */
		Axis	OrientY;			/**< Orientation (Y) */
		Axis	OrientZ;			/**< Orientation (Z) */
		Axis	Roll;				/**< Rotation (roll) */
		Axis	Pitch;				/**< Rotation (pitch) */
		Axis	PointerX;			/**< Pointer (X) */
		Axis	PointerY;			/**< Pointer (Y) */

		// Nunchuk
		Button  NunchukButtonC;		/**< Nunchuk Button 'C' */
		Button  NunchukButtonZ;		/**< Nunchuk Button 'Z' */
		Axis	NunchukAccX;		/**< Nunchuk Acceleration (X) */
		Axis	NunchukAccY;		/**< Nunchuk Acceleration (Y) */
		Axis	NunchukAccZ;		/**< Nunchuk Acceleration (Z) */
		Axis	NunchukOrientX;		/**< Nunchuk Orientation (X) */
		Axis	NunchukOrientY;		/**< Nunchuk Orientation (Y) */
		Axis	NunchukOrientZ;		/**< Nunchuk Orientation (Z) */
		Axis	NunchukRoll;		/**< Nunchuk Rotation (roll) */
		Axis	NunchukPitch;		/**< Nunchuk Rotation (pitch) */
		Axis	NunchukX;			/**< Nunchuk joystick (X) */
		Axis	NunchukY;			/**< Nunchuk joystick (Y) */


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		// Definitions
		enum EProductID {
			// HID definitions
			VendorID  = 0x057e,		/**< 'Nintendo' */
			ProductID = 0x0306,		/**< 'Wiimote' */

			// Bluetooth definitions
			DeviceClass0 = 0x04,
			DeviceClass1 = 0x25,
			DeviceClass2 = 0x00
		};

		/**
		*  @brief
		*    Report type
		*/
		enum EReport {
			ReportStatus			= 0x20,		/**< Status report */
			ReportReadMemory		= 0x21,		/**< Data from memory */
			ReportButtons			= 0x30,		/**< Buttons */
			ReportButtonsAccel		= 0x31,		/**< Buttons and acceleration */
			ReportButtonsAccelIR	= 0x33,		/**< Buttons, acceleration and IR (extended) */
			ReportButtonsAccelExt	= 0x35,		/**< Buttons, acceleration and extension */
			ReportButtonsAccelIRExt	= 0x37		/**< Buttons, acceleration, IR (basic) and extension */
		};

		/**
		*  @brief
		*    Infrared sensor mode
		*/
		enum EIRMode {
			IROff					= 0x00,		/**< IR off */
			IRBasic					= 0x01,		/**< Basic IR mode */
			IRExtended				= 0x03,		/**< Extended IR mode */
			IRFull					= 0x05		/**< Full IR mode */
		};

		/**
		*  @brief
		*    WiiMote extension
		*/
		enum EExtension {
			ExtNone					= 0x0000,	/**< No extension */
			ExtNunchuk				= 0xfefe,	/**< Nunchuk */
			ExtClassic				= 0xfdfd,	/**< Classic controller */
			ExtPartiallyInserted	= 0xffff	/**< Extension not inserted correctly */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Device name
		*  @param[in] pImpl
		*    System specific device implementation, must be valid!
		*/
		PLINPUT_API WiiMote(const PLCore::String &sName, DeviceImpl *pImpl);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~WiiMote();

		/**
		*  @brief
		*    Get report mode
		*
		*  @return
		*    Report mode
		*/
		PLINPUT_API EReport GetReportMode() const;

		/**
		*  @brief
		*    Set report mode
		*
		*  @param[in] nReportMode
		*    Report mode
		*  @param[in] bContinuous
		*    Continuous updates?
		*/
		PLINPUT_API void SetReportMode(EReport nReportMode, bool bContinuous = false);

		/**
		*  @brief
		*    Get infrared mode
		*
		*  @return
		*    Infrared mode
		*/
		PLINPUT_API EIRMode GetIRMode() const;

		/**
		*  @brief
		*    Set infrared mode
		*
		*  @param[in] nIRMode
		*    Infrared mode
		*/
		PLINPUT_API void SetIRMode(EIRMode nIRMode);

		/**
		*  @brief
		*    Get extension
		*
		*  @return
		*    Extension type
		*/
		PLINPUT_API EExtension GetExtension() const;

		/**
		*  @brief
		*    Get battery state
		*
		*  @return
		*    Battery state
		*/
		PLINPUT_API PLCore::uint8 GetBattery() const;

		/**
		*  @brief
		*    Calibrate device
		*/
		PLINPUT_API void Calibrate();


	//[-------------------------------------------------------]
	//[ Public virtual Controller functions                   ]
	//[-------------------------------------------------------]
	public:
		PLINPUT_API virtual void UpdateOutputControl(Control *pControl) override;


	//[-------------------------------------------------------]
	//[ Private data types                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Acceleration data
		*/
		struct SAcceleration {
			// Status
			int nUpdateNearG;	/**< Update counter when acceleration is near 1G */

			// Status
			float fAccX;		/**< Acceleration: X */
			float fAccY;		/**< Acceleration: Y */
			float fAccZ;		/**< Acceleration: Z */
			float fOrientX;		/**< Orientation:  X */
			float fOrientY;		/**< Orientation:  Y */
			float fOrientZ;		/**< Orientation:  Z */
			float fRoll;		/**< Angle:        Roll */
			float fPitch;		/**< Angle:        Pitch */

			// Calibration
			PLCore::uint8 nX0;	/**< Calibration: X0 */
			PLCore::uint8 nY0;	/**< Calibration: Y0 */
			PLCore::uint8 nZ0;	/**< Calibration: Z0 */
			PLCore::uint8 nXG;	/**< Calibration: XG */
			PLCore::uint8 nYG;	/**< Calibration: YG */
			PLCore::uint8 nZG;	/**< Calibration: ZG */

			// Calculate orientation from acceleration data
			void CalculateOrientation();
		};

		/**
		*  @brief
		*    Nunchuk joystick data
		*/
		struct SJoystick {
			// Status
			float fX;				/**< X position */
			float fY;				/**< Y position */

			// Calibration
			PLCore::uint8 nMinX;	/**< Calibration: Minimum X */
			PLCore::uint8 nMidX;	/**< Calibration: Middle  X */
			PLCore::uint8 nMaxX;	/**< Calibration: Maximum X */
			PLCore::uint8 nMinY;	/**< Calibration: Minimum Y */
			PLCore::uint8 nMidY;	/**< Calibration: Middle  Y */
			PLCore::uint8 nMaxY;	/**< Calibration: Maximum Y */
		};

		/**
		*  @brief
		*    IR sensor dot
		*/
		struct SDot {
			bool  bFound;	/**< The dot has been found */
			int   nRawX;	/**< Raw X position */
			int   nRawY;	/**< Raw Y position */
			float fX;		/**< X position (0..1) */
			float fY;		/**< Y position (0..1) */
			int   nSize;	/**< Dot size */
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the HID device has been connected
		*/
		void OnDeviceConnect();

		/**
		*  @brief
		*    Called when the HID device has been disconnected
		*/
		void OnDeviceDisconnect();

		/**
		*  @brief
		*    Called when the HID device has read some data
		*/
		void OnDeviceRead();

		/**
		*  @brief
		*    Data from the WiiMote has been received
		*/
		void OnReadData();

		/**
		*  @brief
		*    Data from memory received
		*/
		void OnReadMemory();

		/**
		*  @brief
		*    Calibration data received
		*/
		void OnReadCalibration();

		/**
		*  @brief
		*    Extension information received
		*/
		void OnReadExtensionType();

		/**
		*  @brief
		*    Nunchuk calibration data received
		*/
		void OnReadNunchukCalibration();

		/**
		*  @brief
		*    Classic calibration data received
		*/
		void OnReadClassicCalibration();

		/**
		*  @brief
		*    Status information received
		*/
		void OnReadStatus();

		/**
		*  @brief
		*    Button status received
		*/
		void OnReadButtons();

		/**
		*  @brief
		*    Acceleration status received
		*/
		void OnReadAccel();

		/**
		*  @brief
		*    IR status received
		*/
		void OnReadIR();

		/**
		*  @brief
		*    Extension status received
		*/
		void OnReadExtension(PLCore::uint32 nOffset);

		/**
		*  @brief
		*    Nunchuk status received
		*/
		void OnReadNunchuk(PLCore::uint32 nOffset);

		/**
		*  @brief
		*    Classic-controller status received
		*/
		void OnReadClassic(PLCore::uint32 nOffset);

		/**
		*  @brief
		*    Read from WiiMote memory
		*
		*  @param[in] nAddress
		*    Address to read from
		*  @param[in] nSize
		*    Size to read
		*/
		void ReadMemory(int nAddress, PLCore::uint8 nSize);

		/**
		*  @brief
		*    Write to WiiMote memory
		*
		*  @param[in] nAddress
		*    Address to write to
		*  @param[in] pBuffer
		*    Buffer containing the data
		*  @param[in] nSize
		*    Size to write
		*/
		void WriteMemory(int nAddress, const PLCore::uint8* pBuffer, PLCore::uint8 nSize);

		/**
		*  @brief
		*    Write a single byte to WiiMote memory
		*
		*  @param[in] nAddress
		*    Address to write to
		*  @param[in] nData
		*    Byte to write
		*/
		void WriteMemory(int nAddress, PLCore::uint8 nData);

		/**
		*  @brief
		*    Clear output report
		*/
		void ClearReport();

		/**
		*  @brief
		*    Send data to WiiMote device
		*
		*  @param[in] pBuffer
		*    Pointer to buffer (must be valid!)
		*  @param[in] nSize
		*    Size of buffer
		*/
		void Send(PLCore::uint8 *pBuffer, PLCore::uint32 nSize);

		/**
		*  @brief
		*    Decrypt data
		*
		*  @param[in] nOffset
		*    Start address inside m_nWriteBuffer
		*  @param[in] nSize
		*    Size inside m_nWriteBuffer
		*/
		void DecryptBuffer(PLCore::uint32 nOffset, PLCore::uint32 nSize);

		/**
		*  @brief
		*    Get WiiMote status
		*/
		void SendStatusRequest();

		/**
		*  @brief
		*    Get calibration information
		*/
		void SendCalibrationRequest();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Event handlers
		PLCore::EventHandler<> EventHandlerOnDeviceConnect;
		PLCore::EventHandler<> EventHandlerOnDeviceDisconnect;
		PLCore::EventHandler<> EventHandlerOnDeviceRead;

		// HID connection
		ConnectionDevice	*m_pConnectionDevice;	/**< Connection device, always valid! */
		PLCore::uint8		*m_pInputBuffer;		/**< Input buffer */
		PLCore::uint8		*m_pOutputBuffer;		/**< Output buffer */

		// WiiMote options
		EReport				 m_nReportMode;			/**< Report mode */
		EIRMode				 m_nIRMode;				/**< IR mode */
		EExtension			 m_nExtension;			/**< Extension type */

		// WiiMote status
		PLCore::uint8		 m_nBattery;			/**< Battery (percent) */
		PLCore::uint8		 m_nLEDs;				/**< LEDs status */
		PLCore::uint8		 m_nRumble;				/**< Rumble state (1=on, 0=off) */
		PLCore::uint16		 m_nButtons;			/**< WiiMote Buttons */
		SAcceleration		 m_sAcc;				/**< Acceleration sensor */
		SDot				 m_sDots[2];			/**< IR dots */
		PLMath::Vector2		 m_vIRPos;				/**< IR position (X, Y between 0..1) */

		// Nunchuk status
		PLCore::uint16		 m_nNunchukButtons;		/**< Nunchuk buttons */
		SAcceleration		 m_sNunchukAcc;			/**< Nunchuk acceleration sensor */
		SJoystick			 m_sNunchukJoy;			/**< Nunchuk joystick */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_WIIMOTE_H__
