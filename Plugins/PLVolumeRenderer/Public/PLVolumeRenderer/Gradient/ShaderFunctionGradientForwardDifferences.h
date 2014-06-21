/*********************************************************\
 *  File: ShaderFunctionGradientForwardDifferences.h     *
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


#ifndef __PLVOLUMERENDERER_SHADERFUNCTION_GRADIENT_FORWARDDIFFERENCES_H__
#define __PLVOLUMERENDERER_SHADERFUNCTION_GRADIENT_FORWARDDIFFERENCES_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/Gradient/ShaderFunctionGradient.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Forward differences (finite differences) shader function implementation class for on-the-fly gradient (4 volume texture fetches for the gradient)
*
*  @remarks
*    On-the-fly gradient computation approximating gradient vectors using the forward differences scheme
*    as described within the book "Real-Time Volume Graphics", section "5.3.1 Gradient Estimation" (page 109).
*    -> Also described within "ACM SIGGRAPH Asia 2008 - Course : Advanced Illumination Techniques for GPU Volume Raycasting"
*       (http://www.voreen.org/files/sa08-coursenotes_1.pdf), section "3.2 Gradient Computation"
*/
class ShaderFunctionGradientForwardDifferences : public ShaderFunctionGradient {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderFunctionGradientForwardDifferences, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionGradient, "Forward differences (finite differences) shader function implementation class for on-the-fly gradient (4 volume texture fetches for the gradient)")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUMERENDERER_API ShaderFunctionGradientForwardDifferences();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderFunctionGradientForwardDifferences();


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunction functions               ]
	//[-------------------------------------------------------]
	public:
		PLVOLUMERENDERER_API virtual PLCore::String GetSourceCode(const PLCore::String &sShaderLanguage, ESourceCodeType nSourceCodeType) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERFUNCTION_GRADIENT_FORWARDDIFFERENCES_H__
