/*********************************************************\
 *  File: HIDDefinitions.h                               *
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


#ifndef __PLINPUT_HIDDEFINITIONS_H__
#define __PLINPUT_HIDDEFINITIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// Usage Page: Generic (e.g. Values, Axes)
static const int UsagePageGeneric	= 0x01;
	static const int UsageJoystick				= 0x04;
	static const int UsageGamepad				= 0x05;
	static const int UsageMultiAxisController	= 0x08;
	static const int UsageX						= 0x30;
	static const int UsageY						= 0x31;
	static const int UsageZ						= 0x32;
	static const int UsageRX					= 0x33;
	static const int UsageRY					= 0x34;
	static const int UsageRZ					= 0x35;
	static const int UsageSlider				= 0x36;
	static const int UsageDial					= 0x37;
	static const int UsageWheel					= 0x38;
	static const int UsageHat					= 0x39;


// Usage Page: Buttons
static const int UsagePageButtons	= 0x09;
	static const int UsageNoButton				= 0x00;
	static const int UsageButton1				= 0x01;
	static const int UsageButton2				= 0x02;
	static const int UsageButton3				= 0x03;
	static const int UsageButton4				= 0x04;
	static const int UsageButton5				= 0x05;
	static const int UsageButton6				= 0x06;
	static const int UsageButton7				= 0x07;
	static const int UsageButton8				= 0x08;


// Usage Page: LED (used for rumble on my gamepad)
static const int UsagePageLED		= 0x08;
	static const int UsageSlowBlinkOnTime		= 0x43;
	static const int UsageSlowBlinkOffTime		= 0x44;
	static const int UsageFastBlinkOnTime		= 0x45;
	static const int UsageFastBlinkOffTime		= 0x46;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_HIDDEFINITIONS_H__
