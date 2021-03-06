/*********************************************************\
 *  File: ShaderFunctionRaySetupBoundingBoxIntersection.cpp *
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
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLScene/Visibility/VisContainer.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/RaySetup/ShaderFunctionRaySetupBoundingBoxIntersection.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ShaderFunctionRaySetupBoundingBoxIntersection, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionRaySetup, "Bounding box intersection shader function implementation class for ray setup")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(ShaderFunctionRaySetupBoundingBoxIntersection)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionRaySetupBoundingBoxIntersection::ShaderFunctionRaySetupBoundingBoxIntersection()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionRaySetupBoundingBoxIntersection::~ShaderFunctionRaySetupBoundingBoxIntersection()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionRaySetupBoundingBoxIntersection::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "BoundingBoxIntersection_GLSL.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case VertexShaderBody:
				return sSourceCode_Vertex;

			case FragmentShaderHeader:
				return sSourceCode_Fragment_Header;

			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case VertexShaderHeader:
			case VertexShaderTemplate:
			case FragmentShaderTemplate:
				// Nothing to do in here
				break;
		}
	} else if (sShaderLanguage == Cg) {
		#include "BoundingBoxIntersection_Cg.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case VertexShaderBody:
				return sSourceCode_Vertex;

			case FragmentShaderHeader:
				return sSourceCode_Fragment_Header;

			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case VertexShaderHeader:
			case VertexShaderTemplate:
			case FragmentShaderTemplate:
				// Nothing to do in here
				break;
		}
	}

	// Error!
	return "";
}

void ShaderFunctionRaySetupBoundingBoxIntersection::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return;	// Early escape, there's no sense in continuing

	// Get the volume resource
	PLVolume::Volume *pVolume = pSNVolume->GetVolume();
	if (!pVolume)
		return;	// Early escape, there's no sense in continuing

	// Get used renderer instance
	Renderer &cRenderer = cProgram.GetRenderer();

	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Set object space to clip space matrix
	cProgramWrapper.Set("ObjectSpaceToClipSpaceMatrix", cVisNode.GetWorldViewProjectionMatrix());

	// Set window size
	cProgramWrapper.Set("WindowSize", static_cast<int>(cRenderer.GetViewport().GetWidth()), static_cast<int>(cRenderer.GetViewport().GetHeight()));

	{ // Set clip space to object space matrix
		// Get object space to clip space matrix, and invert it
		Matrix4x4 mClipSpaceToObjectSpace = cVisNode.GetWorldViewProjectionMatrix();
		mClipSpaceToObjectSpace.Invert();

		// Set clip space to object space matrix
		cProgramWrapper.Set("ClipSpaceToObjectSpace", mClipSpaceToObjectSpace);
	}

	{ // Set ray origin in volume object space
		// Bring the view space origin into scene node space
		Matrix4x4 mViewMatrix = cVisNode.GetWorldViewMatrix();
		mViewMatrix.Invert();
		const Vector3 vRayOrigin = mViewMatrix*Vector3::Zero;

		// Set ray origin in volume object space
		cProgramWrapper.Set("RayOrigin", vRayOrigin);
	}

	// Set step size
	cProgramWrapper.Set("StepSize", pVolume->GetStepSize(pSNVolume->VolumeTextureLOD.Get() + cSRPVolume.GetGlobalVolumeTextureLOD(), pSNVolume->SampleRateFactor.Get()*cSRPVolume.GetGlobalSampleRateFactor()));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
