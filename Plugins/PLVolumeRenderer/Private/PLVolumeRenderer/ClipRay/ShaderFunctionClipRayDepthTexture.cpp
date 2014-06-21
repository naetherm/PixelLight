/*********************************************************\
 *  File: ShaderFunctionClipRayDepthTexture.cpp          *
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
#include <PLCompositing/SRPBegin.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/ClipRay/ShaderFunctionClipRayDepthTexture.h"


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
pl_implement_class(ShaderFunctionClipRayDepthTexture)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionClipRayDepthTexture::ShaderFunctionClipRayDepthTexture()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionClipRayDepthTexture::~ShaderFunctionClipRayDepthTexture()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionClipRayDepthTexture::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "DepthTexture_GLSL.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case FragmentShaderTemplate:
				return sSourceCode_Fragment_Template;

			case VertexShaderHeader:
			case VertexShaderBody:
			case VertexShaderTemplate:
			case FragmentShaderHeader:
				// Nothing to do in here
				break;
		}
	} else if (sShaderLanguage == Cg) {
		#include "DepthTexture_Cg.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case FragmentShaderTemplate:
				return sSourceCode_Fragment_Template;

			case VertexShaderHeader:
			case VertexShaderBody:
			case VertexShaderTemplate:
			case FragmentShaderHeader:
				// Nothing to do in here
				break;
		}
	}

	// Error!
	return "";
}

void ShaderFunctionClipRayDepthTexture::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get used renderer instance
	Renderer &cRenderer = cProgram.GetRenderer();

	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	{ // Set depth texture
		// Get the "PLCompositing::SRPBegin" instance
		PLCompositing::SRPBegin *pSRPBegin = static_cast<PLCompositing::SRPBegin*>(cSRPVolume.GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin"));
		if (pSRPBegin) {
			// Set depth texture
			const int nTextureUnit = cProgramWrapper.Set("DepthTexture_x_", reinterpret_cast<TextureBuffer*>(pSRPBegin->GetTextureBufferDepth()));
			if (nTextureUnit >= 0) {
				// Setup texture addressing by using clamp
				// -> Clamp: Last valid value is reused for out-of-bound access
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
				cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);

				// No need to perform any texture filtering setup in here
				// -> Within the shader we're performing an unfiltered texture lookup in our sampler by using the texel fetch instruction
			}
		}
	}

	{ // Set clip space to object space matrix
		// Get object space to clip space matrix, and invert it
		Matrix4x4 mClipSpaceToObjectSpace = cVisNode.GetWorldViewProjectionMatrix();
		mClipSpaceToObjectSpace.Invert();

		// Set clip space to object space matrix
		cProgramWrapper.Set("ClipSpaceToObjectSpace_x_", mClipSpaceToObjectSpace);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
