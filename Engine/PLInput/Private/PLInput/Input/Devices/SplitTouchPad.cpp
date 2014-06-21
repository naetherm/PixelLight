/*********************************************************\
 *  File: SplitTouchPad.cpp                              *
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
#include "PLInput/Input/Devices/SplitTouchPad.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(SplitTouchPad)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SplitTouchPad::SplitTouchPad(const String &sName, DeviceImpl *pImpl) : Device(sName, "Gamepad device emulation by using a touch screen making it possible to e.g. move & look at the same time", pImpl),
	LeftX	(this, "LeftX",		"Absolute x axis on the left touchscreen side"),
	LeftY	(this, "LeftY",		"Absolute y axis on the left touchscreen side"),
	RightX	(this, "RightX",	"Absolute x axis on the right touchscreen side"),
	RightY	(this, "RightY",	"Absolute y axis on the right touchscreen side")
{
}

/**
*  @brief
*    Destructor
*/
SplitTouchPad::~SplitTouchPad()
{
}


//[-------------------------------------------------------]
//[ Public virtual Controller functions                   ]
//[-------------------------------------------------------]
void SplitTouchPad::Update()
{
	// Update device backend
	if (m_pImpl && m_pImpl->GetBackendType() == BackendUpdateDevice)
		static_cast<UpdateDevice*>(m_pImpl)->Update();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
