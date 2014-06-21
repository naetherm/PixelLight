/*********************************************************\
 *  File: ShaderFunctionClipPositionVolumeTexture.cpp    *
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
#include <PLRenderer/Renderer/TextureBuffer3D.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNClipVolumeTexture.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/ClipPosition/ShaderFunctionClipPositionVolumeTexture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLScene;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionClipPositionVolumeTexture)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionClipPositionVolumeTexture::ShaderFunctionClipPositionVolumeTexture()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionClipPositionVolumeTexture::~ShaderFunctionClipPositionVolumeTexture()
{
}

/**
*  @brief
*    Sets the clip volume texture parameters
*/
void ShaderFunctionClipPositionVolumeTexture::SetVolumeTexture(Program &cProgram, const VisNode &cVolumeVisNode, const VisNode &cVolumeTextureVisNode, int nIndex)
{
	// Get used renderer instance
	Renderer &cRenderer = cProgram.GetRenderer();

	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Get the clip volume texture scene node
	PLVolume::SNClipVolumeTexture *pSNClipVolumeTexture = static_cast<PLVolume::SNClipVolumeTexture*>(cVolumeTextureVisNode.GetSceneNode());
	if (!pSNClipVolumeTexture)
		return;	// Early escape, there's no sense in continuing

	// Get the volume resource
	PLVolume::Volume *pVolume = pSNClipVolumeTexture->GetVolume();
	if (!pVolume)
		return;	// Early escape, there's no sense in continuing

	// Get the renderer texture buffer holding the 3D voxel data
	PLRenderer::TextureBuffer *pVolumeTextureBuffer = pVolume->GetVolumeTextureBuffer(cRenderer);
	if (!pVolumeTextureBuffer || pVolumeTextureBuffer->GetType() != PLRenderer::Resource::TypeTextureBuffer3D)
		return;	// Early escape, there's no sense in continuing we only support 3D textures

	// Calculate the volume space to clip volume texture space matrix
	// Volume space to view space
	Matrix4x4 mVolumeSpaceToClipVolumeTextureSpace = cVolumeVisNode.GetWorldViewMatrix();
	// View space to clip volume texture space
	mVolumeSpaceToClipVolumeTextureSpace = cVolumeTextureVisNode.GetWorldViewMatrix().GetInverted()*mVolumeSpaceToClipVolumeTextureSpace;

	// Set the volume space to clip volume texture space matrix
	cProgramWrapper.Set((nIndex < 0) ? "VolumeSpaceToClipVolumeTextureSpace_x_" : (String("VolumeSpaceToCliVvolumeTextureSpace_") + nIndex + '_'), mVolumeSpaceToClipVolumeTextureSpace);

	// Set invert clipping
	cProgramWrapper.Set((nIndex < 0) ? "InvertClipping_x_" : (String("InvertClipping_") + nIndex + '_'), (cVolumeTextureVisNode.GetSceneNode()->GetFlags() & PLVolume::SNClip::InvertClipping) != 0);

	{ // Set clip volume texture map
		const int nTextureUnit = cProgramWrapper.Set((nIndex < 0) ? "ClipVolumeTexture_x_" : (String("ClipVolumeTexture_") + nIndex + '_'), pVolumeTextureBuffer);
		if (nTextureUnit >= 0) {
			// Setup texture addressing by using clamp
			// -> Clamp: Last valid value is reused for out-of-bound access
			// -> "stretched color" instead of color being set to border color which is black by default
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressW, TextureAddressing::Clamp);

			// No need to perform any texture filtering in here
			cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
		}
	}

	// Set clip threshold, everything above will be clipped
	cProgramWrapper.Set((nIndex < 0) ? "ClipThreshold_x_" : (String("ClipThreshold_") + nIndex + '_'), pSNClipVolumeTexture->ClipThreshold.Get());
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunctionClipPosition functions   ]
//[-------------------------------------------------------]
void ShaderFunctionClipPositionVolumeTexture::SetVolumeTextures(Program &cProgram, const VisNode &cVolumeVisNode, const Array<const VisNode*> &lstClipVolumeTextures)
{
	// None-template version

	// We only know a single clip volume texture, ignore the rest
	if (lstClipVolumeTextures.GetNumOfElements())
		SetVolumeTexture(cProgram, cVolumeVisNode, *lstClipVolumeTextures[0], -1);
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionClipPositionVolumeTexture::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "VolumeTexture_GLSL.h"

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
		#include "VolumeTexture_Cg.h"

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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
