/*********************************************************\
 *  File: PLInputDefinitions.h                           *
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


#ifndef __PLINPUT_DEFINITIONS_H__
#define __PLINPUT_DEFINITIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Rtti.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Controllers                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Controller type
*/
enum EControllerType {
	ControllerDevice = 0,	/**< Controller is a real input device, no input connections are allowed */
	ControllerVirtual		/**< Controller is a virtual controller, input and output connections are allowed */
};

	// RTTI definition
	pl_enum(EControllerType)
		pl_enum_value(ControllerDevice,  "Real input device, no input connections are allowed")
		pl_enum_value(ControllerVirtual, "Virtual controller, input and output connections are allowed")
	pl_enum_end

/**
*  @brief
*    Control type
*/
enum EControlType {
	ControlUnknown = 0,	/**< Unknown control */
	ControlButton = 1,	/**< Button or key */
	ControlAxis,		/**< Axis (can be absolute or relative) */
	ControlLED,			/**< LED output */
	ControlEffect		/**< Effect output */
};

	// RTTI definition
	pl_enum(EControlType)
		pl_enum_value(ControlUnknown,	"Unknown control")
		pl_enum_value(ControlButton,	"Button or key")
		pl_enum_value(ControlAxis,		"Axis")
		pl_enum_value(ControlLED,		"LED output")
		pl_enum_value(ControlEffect,	"Effect output")
	pl_enum_end

/**
*  @brief
*    Device backend type
*/
enum EDeviceBackend {
	BackendUnknown = 0,			/**< Unknown backend */
	BackendUpdateDevice,		/**< UpdateDevice backend */
	BackendConnectionDevice,	/**< ConnectionDevice backend */
	BackendHID					/**< HIDDevice backend (which is also a ConnectionDevice!) */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_DEFINITIONS_H__
