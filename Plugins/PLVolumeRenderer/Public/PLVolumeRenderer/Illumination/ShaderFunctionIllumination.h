/*********************************************************\
 *  File: ShaderFunctionIllumination.h                   *
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


#ifndef __PLVOLUMERENDERER_SHADERFUNCTION_ILLUMINATION_H__
#define __PLVOLUMERENDERER_SHADERFUNCTION_ILLUMINATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/ShaderFunction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract shader function base class for illumination
*/
class ShaderFunctionIllumination : public ShaderFunction {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderFunctionIllumination, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunction, "Abstract shader function base class for illumination")
		// Properties
		pl_properties
			pl_property("FunctionTitle",			"2.6 - Illumination")
			pl_property("FunctionName",				"Illumination")
			pl_property("FunctionSignature_GLSL",	"vec3 Illumination(vec3 SurfaceColor, vec3 SurfaceNormal, vec3 ViewingDirection, vec3 LightDirection)")
			pl_property("FunctionSignature_Cg",		"float3 Illumination(float3 SurfaceColor, float3 SurfaceNormal, float3 ViewingDirection, float3 LightDirection)")
		pl_properties_end
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUMERENDERER_API ShaderFunctionIllumination();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderFunctionIllumination();


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunction functions               ]
	//[-------------------------------------------------------]
	public:
		PLVOLUMERENDERER_API virtual void SetProgram(PLRenderer::Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERFUNCTION_ILLUMINATION_H__
