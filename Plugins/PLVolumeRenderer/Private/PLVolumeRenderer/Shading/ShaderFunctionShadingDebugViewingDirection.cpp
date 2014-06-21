/*********************************************************\
 *  File: ShaderFunctionShadingDebugViewingDirection.cpp *
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
#include "PLVolumeRenderer/Shading/ShaderFunctionShadingDebugViewingDirection.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionShadingDebugViewingDirection)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionShadingDebugViewingDirection::ShaderFunctionShadingDebugViewingDirection()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionShadingDebugViewingDirection::~ShaderFunctionShadingDebugViewingDirection()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionShadingDebugViewingDirection::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "DebugViewingDirection_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	} else if (sShaderLanguage == Cg) {
		#include "DebugViewingDirection_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	}

	// Error!
	return "";
}

void ShaderFunctionShadingDebugViewingDirection::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Get the view space to object space matrix
	Matrix4x4 mViewSpaceToObjectSpace = cVisNode.GetWorldViewMatrix();
	mViewSpaceToObjectSpace.Invert();

	{ // Set viewing direction
		// Bring the normalized view space viewing direction vector into scene node space
		Vector3 vObjectSpaceViewingDirection = mViewSpaceToObjectSpace.RotateVector(Vector3::NegativeUnitZ);	// Vector from surface point to camera (in view space, the camera looks along the negative z-axis)
		vObjectSpaceViewingDirection.Normalize();

		// Set the normalized viewing direction in scene node space
		cProgramWrapper.Set("ViewingDirection", vObjectSpaceViewingDirection);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
