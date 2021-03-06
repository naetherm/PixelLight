/*********************************************************\
 *  File: ShaderFunctionFetchScalarSampler2DArray.cpp    *
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
#include <PLRenderer/Renderer/TextureBuffer2DArray.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/FetchScalar/ShaderFunctionFetchScalarSampler2DArray.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(ShaderFunctionFetchScalarSampler2DArray, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionFetchScalar, "2D array sampler shader function implementation class for fetch scalar")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(ShaderFunctionFetchScalarSampler2DArray)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionFetchScalarSampler2DArray::ShaderFunctionFetchScalarSampler2DArray()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionFetchScalarSampler2DArray::~ShaderFunctionFetchScalarSampler2DArray()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionFetchScalarSampler2DArray::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "Sampler2DArray_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderHeader)
			return sSourceCode_Fragment_Header;
	} else if (sShaderLanguage == Cg) {
		#include "Sampler2DArray_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderHeader)
			return sSourceCode_Fragment_Header;
	}

	// Error!
	return "";
}

void ShaderFunctionFetchScalarSampler2DArray::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get used renderer instance
	Renderer &cRenderer = cProgram.GetRenderer();

	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return;	// Early escape, there's no sense in continuing

	// Get the volume resource
	PLVolume::Volume *pVolume = pSNVolume->GetVolume();
	if (!pVolume)
		return;	// Early escape, there's no sense in continuing

	// Get the renderer texture buffer holding the 3D voxel data
	PLRenderer::TextureBuffer *pVolumeTextureBuffer = pVolume->GetVolumeTextureBuffer(cRenderer);
	if (!pVolumeTextureBuffer || pVolumeTextureBuffer->GetType() != PLRenderer::Resource::TypeTextureBuffer2DArray)
		return;	// Early escape, there's no sense in continuing

	// Get the number of available mipmaps within the volume texture
	const uint32 nNumOfMipmaps = pVolumeTextureBuffer->GetNumOfMipmaps();

	{ // Volume texture
		const int nTextureUnit = cProgramWrapper.Set("VolumeTexture", pVolumeTextureBuffer);
		if (nTextureUnit >= 0) {
			// Setup texture filtering by using clamp to border color
			// -> By default, the border color is black -> out-of-bound access will return black
			// -> Do not use "TextureAddressing::Clamp" in here which would reuse the last valid color
			//    which might result in funny looking infinity stretched borders
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Border);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Border);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressW, TextureAddressing::Border);

			// Setup texture filtering
			switch (pSNVolume->Reconstruction.Get()) {
				// Nearest neighbour filtering
				case PLVolume::SNVolume::NearestNeighbourFiltering:
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::Linear);	// We still want to have mipmapping
					break;

				// Trilinear filtering
				case PLVolume::SNVolume::TrilinearFiltering:
				// Tri-cubic filtering (third-order texture filtering, has same basic setup as trilinear filtering)
				case PLVolume::SNVolume::TriCubicFiltering:
				case PLVolume::SNVolume::TriCubicKernelFiltering:
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::Linear);
					break;
			}

			// In case there are no mipmaps available within the volume texture: Disable mipmapping
			if (!nNumOfMipmaps)
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);

			// Set the depth of the volume texture (z-component)
			cProgramWrapper.Set("VolumeTextureDepth", static_cast<float>(static_cast<TextureBuffer2DArray*>(pVolumeTextureBuffer)->GetSize().z));
		}
	}

	{ // Set volume texture level of detail (0...<number of mipmaps>-1)
	  // -> Ensure that we don't exceed the number of mipmaps available within the volume texture
		float fVolumeTextureLOD = pSNVolume->VolumeTextureLOD.Get() + cSRPVolume.GetGlobalVolumeTextureLOD();
		if (fVolumeTextureLOD < 0.0f)
			fVolumeTextureLOD = 0.0f;
		else if (fVolumeTextureLOD > nNumOfMipmaps)
			fVolumeTextureLOD = static_cast<float>(nNumOfMipmaps);
		cProgramWrapper.Set("VolumeTextureLOD", fVolumeTextureLOD);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
