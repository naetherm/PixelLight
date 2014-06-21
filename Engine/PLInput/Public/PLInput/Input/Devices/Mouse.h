/*********************************************************\
 *  File: Mouse.h                                        *
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


#ifndef __PLINPUT_MOUSE_H__
#define __PLINPUT_MOUSE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Input/Devices/Device.h"
#include "PLInput/Input/Controls/Axis.h"
#include "PLInput/Input/Controls/Button.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mouse input device
*
*  @remarks
*    This class supports the following device backend types:
*    - UpdateDevice
*/
class Mouse : public Device {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, Mouse, "PLInput", PLInput::Device, "Mouse input controller")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Controller definition                                 ]
	//[-------------------------------------------------------]
	public:
		PLInput::Axis	X;			/**< X axis (movement data, no absolute data) */
		PLInput::Axis	Y;			/**< Y axis (movement data, no absolute data) */
		PLInput::Axis	Wheel;		/**< Mouse wheel (movement data, no absolute data) */
		PLInput::Button	Left;		/**< Left mouse button (mouse button #0) */
		PLInput::Button	Right;		/**< Right mouse button (mouse button #1) */
		PLInput::Button	Middle;		/**< Middle mouse button (mouse button #2) */
		PLInput::Button	Button4;	/**< Mouse button #4 */
		PLInput::Button	Button5;	/**< Mouse button #5 */
		PLInput::Button	Button6;	/**< Mouse button #6 */
		PLInput::Button	Button7;	/**< Mouse button #7 */
		PLInput::Button	Button8;	/**< Mouse button #8 */
		PLInput::Button	Button9;	/**< Mouse button #9 */
		PLInput::Button	Button10;	/**< Mouse button #10 */
		PLInput::Button	Button11;	/**< Mouse button #11 */
		PLInput::Button	Button12;	/**< Mouse button #12 */


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
		*    System specific device implementation, can, but shouldn't be a null pointer
		*/
		PLINPUT_API Mouse(const PLCore::String &sName, DeviceImpl *pImpl);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Mouse();


	//[-------------------------------------------------------]
	//[ Public virtual Controller functions                   ]
	//[-------------------------------------------------------]
	public:
		PLINPUT_API virtual void Update() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_MOUSE_H__
