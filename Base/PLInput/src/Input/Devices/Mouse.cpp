/*********************************************************\
 *  File: Mouse.cpp                                      *
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
#include "PLInput/Backend/UpdateDevice.h"
#include "PLInput/Input/Devices/Mouse.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_class_metadata(Mouse, "PLInput", PLInput::Device, "Mouse input controller")
pl_class_metadata_end(Mouse)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Mouse::Mouse(const String &sName, DeviceImpl *pImpl) : Device(sName, "Mouse input controller", pImpl),
	X		(this, "X",			"X axis (movement data, no absolute data)"),
	Y		(this, "Y",			"Y axis (movement data, no absolute data)"),
	Wheel	(this, "Wheel",		"Mouse wheel (movement data, no absolute data)"),
	Left	(this, "Left",		"Left mouse button (mouse button #0)"),
	Right	(this, "Right",		"Right mouse button (mouse button #1)"),
	Middle	(this, "Middle",	"Middle mouse button (mouse button #2)"),
	Button4	(this, "Button4",	"Mouse button #4"),
	Button5	(this, "Button5",	"Mouse button #5"),
	Button6	(this, "Button6",	"Mouse button #6"),
	Button7	(this, "Button7",	"Mouse button #7"),
	Button8	(this, "Button8",	"Mouse button #8"),
	Button9	(this, "Button9",	"Mouse button #9"),
	Button10(this, "Button10",	"Mouse button #10"),
	Button11(this, "Button11",	"Mouse button #11"),
	Button12(this, "Button12",	"Mouse button #12")
{
}

/**
*  @brief
*    Destructor
*/
Mouse::~Mouse()
{
}


//[-------------------------------------------------------]
//[ Public virtual Controller functions                   ]
//[-------------------------------------------------------]
void Mouse::Update()
{
	// Update device backend
	if (m_pImpl && m_pImpl->GetBackendType() == BackendUpdateDevice)
		static_cast<UpdateDevice*>(m_pImpl)->Update();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
