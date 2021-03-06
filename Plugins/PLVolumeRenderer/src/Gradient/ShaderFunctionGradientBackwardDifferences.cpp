/*********************************************************\
 *  File: ShaderFunctionGradientBackwardDifferences.cpp  *
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
#include "PLVolumeRenderer/Gradient/ShaderFunctionGradientBackwardDifferences.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ShaderFunctionGradientBackwardDifferences, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionGradient, "Backward differences (finite differences) shader function implementation class for on-the-fly gradient (4 volume texture fetches for the gradient)")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(ShaderFunctionGradientBackwardDifferences)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionGradientBackwardDifferences::ShaderFunctionGradientBackwardDifferences()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionGradientBackwardDifferences::~ShaderFunctionGradientBackwardDifferences()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionGradientBackwardDifferences::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "BackwardDifferences_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	} else if (sShaderLanguage == Cg) {
		#include "BackwardDifferences_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
