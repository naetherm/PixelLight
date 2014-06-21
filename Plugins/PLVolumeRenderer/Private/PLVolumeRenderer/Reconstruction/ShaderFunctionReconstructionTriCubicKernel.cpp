/*********************************************************\
 *  File: ShaderFunctionReconstructionTriCubicKernel.cpp *
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
#include <PLMath/Math.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/Reconstruction/ShaderFunctionReconstructionTriCubicKernel.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionReconstructionTriCubicKernel)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionReconstructionTriCubicKernel::ShaderFunctionReconstructionTriCubicKernel() :
	m_p1DCubicFilterKernelTextureBuffer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionReconstructionTriCubicKernel::~ShaderFunctionReconstructionTriCubicKernel()
{
	// Destroy the 1D cubic filter kernel texture buffer
	if (m_p1DCubicFilterKernelTextureBuffer)
		delete m_p1DCubicFilterKernelTextureBuffer;
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionReconstructionTriCubicKernel::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "TriCubicKernel_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderHeader)
			return sSourceCode_Fragment_Header;
	} else if (sShaderLanguage == Cg) {
		#include "TriCubicKernel_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderHeader)
			return sSourceCode_Fragment_Header;
	}

	// Error!
	return "";
}


// Source: http://http.developer.nvidia.com/GPUGems/gpugems_ch24.html "Example 24-7. OpenGL C++ Code for Generating a Floating-Point Kernel Texture"
// Mitchell Netravali Reconstruction Filter
// B = 1,   C = 0   - cubic B-spline
// B = 1/3, C = 1/3 - recommended
// B = 0,   C = 1/2 - Catmull-Rom spline
float MitchellNetravali(float x, float B, float C)
{
	const float ax = Math::Abs(x);
	if (ax < 1) {
		return ((12 - 9 * B - 6 * C) * ax * ax * ax + (-18 + 12 * B + 6 * C) * ax * ax + (6 - 2 * B)) / 6;
	} else if ((ax >= 1) && (ax < 2)) {
		return ((-B - 6 * C) * ax * ax * ax + (6 * B + 30 * C) * ax * ax + (-12 * B - 48 * C) * ax + (8 * B + 24 * C)) / 6;
	} else {
		return 0;
	}
}


void ShaderFunctionReconstructionTriCubicKernel::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
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
	if (pVolumeTextureBuffer) {
		// Get the size of the volume texture buffer
		const Vector3i vSize = pVolumeTextureBuffer->GetUniformSize();

		// Volume texture size
		cProgramWrapper.Set("VolumeTextureSize", static_cast<float>(vSize.x), static_cast<float>(vSize.y), static_cast<float>(vSize.z));
	}

	// Create the 1D cubic filter kernel texture buffer if it doesn't exist, yet
	if (!m_p1DCubicFilterKernelTextureBuffer) {
		// Usually, 64 or 128 texels is sufficient
		const uint32 nSize = 128;

		// Create the 1D cubic filter kernel texture image
		Image cImage = Image::CreateImage(DataFloat, ColorRGBA, Vector3i(nSize, 1, 1), CompressionNone);

		// Get the image buffer and fill the 1D cubic filter kernel texture image
		ImageBuffer *pImageBuffer = cImage.GetBuffer();
		if (pImageBuffer) {
			// Cubic B-spline
			static const float B = 1.0f;
			static const float C = 0.0f;

			// Fill data
			float *pfData = reinterpret_cast<float*>(pImageBuffer->GetData());
			for (uint32 i=0; i<nSize; i++) {
				// Calculate the four weights as described within http://http.developer.nvidia.com/GPUGems/gpugems_ch24.html "Example 24-7. OpenGL C++ Code for Generating a Floating-Point Kernel Texture"
				const float x  = i/static_cast<float>(nSize - 1);
				const float w0 = MitchellNetravali(x + 1, B, C);
				const float w1 = MitchellNetravali(x,     B, C);
				const float w2 = MitchellNetravali(1 - x, B, C);
				const float w3 = MitchellNetravali(2 - x, B, C);

				// Calculate the periodic g and h functions (interval [0..1]) as described within http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter20.html "Equation 5"
				// -> Encode the periodic g and h functions as seen within http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter20.html "Figure 20-2 Cubic Filtering of a One-Dimensional Texture"
				const float g0 = *pfData++ = w0 + w1;			// Red   = g0
				const float g1 = *pfData++ = w2 + w3;			// Green = g1
								 *pfData++ = (w1 / g0) - 0.5f;	// Blue  = h0, move from [-0.5, extent-0.5] to [0, extent]
								 *pfData++ = (w3 / g1) + 1.5f;	// Alpha = h1, move from [-0.5, extent-0.5] to [0, extent]
			}
		}

		// Create the 1D cubic filter kernel texture buffer
		m_p1DCubicFilterKernelTextureBuffer = reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer1D(cImage, TextureBuffer::Unknown, 0));
	}

	{ // Filter kernel texture
		const int nTextureUnit = cProgramWrapper.Set("tex_hg", m_p1DCubicFilterKernelTextureBuffer);
		if (nTextureUnit >= 0) {
			// Setup texture addressing by using repeat (= wrap)
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Wrap);

			// Setup texture filtering: No filter please, this is a simple lookup function!
			cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
