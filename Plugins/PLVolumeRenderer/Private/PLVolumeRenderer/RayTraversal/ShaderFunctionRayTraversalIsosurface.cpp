/*********************************************************\
 *  File: ShaderFunctionRayTraversalIsosurface.cpp       *
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
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/RayTraversal/ShaderFunctionRayTraversalIsosurface.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionRayTraversalIsosurface)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionRayTraversalIsosurface::ShaderFunctionRayTraversalIsosurface()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionRayTraversalIsosurface::~ShaderFunctionRayTraversalIsosurface()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionRayTraversalIsosurface::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "Isosurface_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	} else if (sShaderLanguage == Cg) {
		#include "Isosurface_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	}

	// Error!
	return "";
}

void ShaderFunctionRayTraversalIsosurface::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return;	// Early escape, there's no sense in continuing

	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Set dither ray
	cProgramWrapper.Set("IsosurfaceValue", pSNVolume->IsosurfaceValue.Get());

	// Set opacity, usually within the interval [~0 .. 1] = [transparent .. solid]
	cProgramWrapper.Set("Opacity", pSNVolume->Opacity.Get());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
