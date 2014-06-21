/*********************************************************\
 *  File: SRPDebugWireframes.h                           *
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


#ifndef __PLCOMPOSITING_DEBUG_WIREFRAMES_H__
#define __PLCOMPOSITING_DEBUG_WIREFRAMES_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include <PLRenderer/Renderer/Types.h>
#include "PLCompositing/SRPDebug.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene renderer pass drawing just simple wireframes
*/
class SRPDebugWireframes : public SRPDebug {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			UseDepth = 1<<1	/**< Take the depth buffer into account (lines can be hidden by objects) */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDebug::EFlags)
			pl_enum_value(UseDepth, "Take the depth buffer into account (lines can be hidden by objects)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDebugWireframes, "PLCompositing", PLCompositing::SRPDebug, "Abstract scene renderer pass drawing just simple wireframes")
		// Attributes
		pl_attribute(LineWidth,				float,									1.0f,										ReadWrite,	DirectValue,	"Line width (if supported by the the used renderer API)",										"Min='1.0'")
		pl_attribute(LineColor,				PLGraphics::Color4,						PLGraphics::Color4(1.0f, 1.0f, 1.0f, 0.4f),	ReadWrite,	DirectValue,	"Line color",																					"")
		pl_attribute(MaxDrawDistance,		float,									-1.0f,										ReadWrite,	DirectValue,	"Maximum draw distance of wireframes to the camera, if less or equal 0, there's no limitation",	"")
		pl_attribute(CullMode,				pl_enum_type(PLRenderer::Cull::Enum),	PLRenderer::Cull::CCW,						ReadWrite,	DirectValue,	"Backface cull mode to use",																	"")
		pl_attribute(SlopeScaleDepthBias,	float,									0.0f,										ReadWrite,	DirectValue,	"Slope scale depth bias (polygon offset to avoid nasty line artifacts)",						"")
		pl_attribute(DepthBias,				float,									0.0f,										ReadWrite,	DirectValue,	"Depth bias (polygon offset to avoid nasty line artifacts)",									"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),						0,											ReadWrite,	GetSet,			"Flags",																						"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDebugWireframes();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPDebugWireframes();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEBUG_WIREFRAMES_H__
