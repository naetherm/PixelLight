/*********************************************************\
 *  File: SNClip.cpp                                     *
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
#include "PLVolume/Scene/SNClip.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNClip, "PLVolume", PLScene::SceneNode, "Abstract clip scene node base class")
	// Attributes
	pl_attribute_metadata(DebugColorPositive,	PLGraphics::Color4,						PLGraphics::Color4(0.0f, 1.0f, 0.0f, 0.1f),	ReadWrite,	"Debug color for the view side allowing to see the non-clipped rest",	"")
	pl_attribute_metadata(DebugColorNegative,	PLGraphics::Color4,						PLGraphics::Color4(1.0f, 0.0f, 0.0f, 0.1f),	ReadWrite,	"Debug color for the view side allowing to see the clipped part",		"")
		// Overwritten PLScene::SceneNode attributes
	pl_attribute_metadata(Flags,				pl_flag_type_def3(SNClip, EFlags),		0,											ReadWrite,	"Flags",																"")
	pl_attribute_metadata(DebugFlags,			pl_flag_type_def3(SNClip, EDebugFlags),	0,											ReadWrite,	"Debug flags",															"")
pl_class_metadata_end(SNClip)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNClip::SNClip() :
	DebugColorPositive(this),
	DebugColorNegative(this),
	Flags(this),
	DebugFlags(this)
{
}

/**
*  @brief
*    Destructor
*/
SNClip::~SNClip()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
