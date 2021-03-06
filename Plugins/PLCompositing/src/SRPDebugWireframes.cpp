/*********************************************************\
 *  File: SRPDebugWireframes.cpp                         *
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
#include "PLCompositing/SRPDebugWireframes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SRPDebugWireframes, "PLCompositing", PLCompositing::SRPDebug, "Abstract scene renderer pass drawing just simple wireframes")
	// Attributes
	pl_attribute_metadata(LineWidth,			float,											1.0f,										ReadWrite,	"Line width (if supported by the the used renderer API)",									"Min='1.0'")
	pl_attribute_metadata(LineColor,			PLGraphics::Color4,								PLGraphics::Color4(1.0f, 1.0f, 1.0f, 0.4f),	ReadWrite,	"Line color",																							"")
	pl_attribute_metadata(MaxDrawDistance,		float,											-1.0f,										ReadWrite,	"Maximum draw distance of wireframes to the camera, if less or equal 0, there's no limitation",	"")
	pl_attribute_metadata(CullMode,				pl_enum_type_def3(PLRenderer::Cull, Enum),		PLRenderer::Cull::CCW,						ReadWrite,	"Backface cull mode to use",																		"")
	pl_attribute_metadata(SlopeScaleDepthBias,	float,											0.0f,										ReadWrite,	"Slope scale depth bias (polygon offset to avoid nasty line artifacts)",							"")
	pl_attribute_metadata(DepthBias,			float,											0.0f,										ReadWrite,	"Depth bias (polygon offset to avoid nasty line artifacts)",										"")
		// Overwritten PLScene::SceneRendererPass attributes
	pl_attribute_metadata(Flags,				pl_flag_type_def3(SRPDebugWireframes, EFlags),	0,											ReadWrite,	"Flags",																								"")
pl_class_metadata_end(SRPDebugWireframes)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDebugWireframes::SRPDebugWireframes() :
	LineWidth(this),
	LineColor(this),
	MaxDrawDistance(this),
	CullMode(this),
	SlopeScaleDepthBias(this),
	DepthBias(this),
	Flags(this)
{
}

/**
*  @brief
*    Destructor
*/
SRPDebugWireframes::~SRPDebugWireframes()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
