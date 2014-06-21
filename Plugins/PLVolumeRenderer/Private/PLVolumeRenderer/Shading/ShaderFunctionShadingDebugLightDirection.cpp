/*********************************************************\
 *  File: ShaderFunctionShadingDebugLightDirection.cpp   *
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
#include <PLCore/Base/ClassManager.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLScene/Scene/SNLight.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/Shading/ShaderFunctionShadingDebugLightDirection.h"


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
pl_implement_class(ShaderFunctionShadingDebugLightDirection)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionShadingDebugLightDirection::ShaderFunctionShadingDebugLightDirection()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionShadingDebugLightDirection::~ShaderFunctionShadingDebugLightDirection()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionShadingDebugLightDirection::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "DebugLightDirection_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	} else if (sShaderLanguage == Cg) {
		#include "DebugLightDirection_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	}

	// Error!
	return "";
}

void ShaderFunctionShadingDebugLightDirection::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return;	// Early escape, there's no sense in continuing

	// Light
	Vector3	vLightDirection = Vector3::UnitZ;	// Normalized view space light direction vector (any default value)
	{ // Get a list of all visible scene node instances intersecting with the given scene node
		const Class *pClass = ClassManager::GetInstance()->GetClass("PLScene::SNDirectionalLight");
		if (pClass) {
			Array<const VisNode*> lstIntersecting;
			cSRPVolume.GetIntersectingInstancesOf(cCullQuery, *pSNVolume, *pClass, lstIntersecting);
			if (lstIntersecting.GetNumOfElements()) {
				// Get the view space light direction vector and the light color
				const VisNode &cLightVisNode = *lstIntersecting[0];
				vLightDirection = cLightVisNode.GetWorldViewMatrix().RotateVector(Vector3::UnitZ);	// Vector from surface point to light source
			}
		}
	}

	// Get the view space to object space matrix
	Matrix4x4 mViewSpaceToObjectSpace = cVisNode.GetWorldViewMatrix();
	mViewSpaceToObjectSpace.Invert();

	{ // Set light direction
		// Bring the normalized view space light direction vector into scene node space
		Vector3 vObjectSpaceLightDirection = mViewSpaceToObjectSpace.RotateVector(vLightDirection);
		vObjectSpaceLightDirection.Normalize();

		// Set the normalized light direction in scene node space
		cProgramWrapper.Set("LightDirection", vObjectSpaceLightDirection);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
