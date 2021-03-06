/*********************************************************\
 *  File: SNCoordinateAxis.cpp                           *
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
#include "PLScene/Scene/SceneNodes/SNCoordinateAxis.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNCoordinateAxis, "PLScene", PLScene::SceneNode, "Abstract base scene node for coordinate axis visualization")
	// Attributes
	pl_attribute_metadata(Width,	float,											1.0f,										ReadWrite,	"Line width (if supported by the the used renderer API)",	"Min='1.0'")
	pl_attribute_metadata(XColor,	PLGraphics::Color4,								PLGraphics::Color4(1.0f, 0.0f, 0.0f, 1.0f),	ReadWrite,	"Color of the x-axis",										"")
	pl_attribute_metadata(YColor,	PLGraphics::Color4,								PLGraphics::Color4(0.0f, 1.0f, 0.0f, 1.0f),	ReadWrite,	"Color of the y-axis",										"")
	pl_attribute_metadata(ZColor,	PLGraphics::Color4,								PLGraphics::Color4(0.0f, 0.0f, 1.0f, 1.0f),	ReadWrite,	"Color of the z-axis",										"")
		// Overwritten SceneNode attributes
	pl_attribute_metadata(Flags,	pl_flag_type_def3(SNCoordinateAxis, EFlags),	0,											ReadWrite,	"Flags",													"")
pl_class_metadata_end(SNCoordinateAxis)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNCoordinateAxis::SNCoordinateAxis() :
	Width(this),
	XColor(this),
	YColor(this),
	ZColor(this),
	Flags(this)
{
}

/**
*  @brief
*    Destructor
*/
SNCoordinateAxis::~SNCoordinateAxis()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
