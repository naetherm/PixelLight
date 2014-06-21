/*********************************************************\
 *  File: Volume.cpp                                     *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLGraphics/Image/ImageEffects.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/ResourceHandler.h>
#include "PLVolume/TransferFunctionControl/Histogram.h"
#include "PLVolume/TransferFunctionControl/TransferFunctionControl.h"
#include "PLVolume/VolumeManager.h"
#include "PLVolume/Volume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLVolume {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Vector3 Volume::DefaultVoxelSize(0.001f, 0.001f, 0.001f);


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Volume::Volume(const String &sName, ResourceManager<Volume> *pManager) : PLCore::Resource<Volume>(sName, pManager),
	m_vVoxelSize(DefaultVoxelSize),
	m_pTransferFunctionControl(new TransferFunctionControl()),
	m_pTextureBufferHandler(new PLRenderer::ResourceHandler())
{
	/*
	// [TODO] Create automatic volume data
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		Texture *pTexture = pSceneContext->GetRendererContext().GetTextureManager().CreateTexture("PLVolume::TextureCreatorPyroclasticCloud3D", "");
		if (pTexture)
			m_pTextureBufferHandler->SetResource(pTexture->GetTextureBuffer());
	}
*/
}

/**
*  @brief
*    Destructor
*/
Volume::~Volume()
{
	// Destroy the renderer texture buffer
	if (m_pTextureBufferHandler->GetResource())
		delete m_pTextureBufferHandler->GetResource();

	// Destroy the texture buffer handler
	delete m_pTextureBufferHandler;

	// Destroy the transfer function control instance
	delete m_pTransferFunctionControl;
}

/**
*  @brief
*    Returns the image holding the volumetric data
*/
Image &Volume::GetVolumeImage()
{
	return m_cVolumeImage;
}

/**
*  @brief
*    Returns the image holding the volumetric data
*/
const Image &Volume::GetVolumeImage() const
{
	return m_cVolumeImage;
}

/**
*  @brief
*    Returns the size of the image holding the volumetric data
*/
Vector3i Volume::GetVolumeImageSize() const
{
	// Get the image buffer, if there's one
	ImageBuffer *pImageBuffer = m_cVolumeImage.GetBuffer();
	if (pImageBuffer)
		return pImageBuffer->GetSize();

	// Is there a volume texture buffer?
	TextureBuffer *pTextureBuffer = static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
	if (pTextureBuffer)
		return pTextureBuffer->GetUniformSize();

	// Error!
	return Vector3i::Zero;
}

/**
*  @brief
*    Returns the size of one voxel
*/
const Vector3 &Volume::GetVoxelSize() const
{
	return m_vVoxelSize;
}

/**
*  @brief
*    Sets the size of one voxel
*/
void Volume::SetVoxelSize(const Vector3 &vVoxelSize)
{
	m_vVoxelSize = vVoxelSize;
}

/**
*  @brief
*    Returns the transfer function
*/
TransferFunction &Volume::GetTransferFunction()
{
	return *m_pTransferFunctionControl;
}

/**
*  @brief
*    Returns the transfer function
*/
const TransferFunction &Volume::GetTransferFunction() const
{
	return *m_pTransferFunctionControl;
}

/**
*  @brief
*    Returns the transfer function control
*/
TransferFunctionControl &Volume::GetTransferFunctionControl()
{
	return *m_pTransferFunctionControl;
}

/**
*  @brief
*    Returns the transfer function control
*/
const TransferFunctionControl &Volume::GetTransferFunctionControl() const
{
	return *m_pTransferFunctionControl;
}

/**
*  @brief
*    Returns the step size
*/
float Volume::GetStepSize(float fVolumeTextureLOD, float fSampleRateFactor, float *pfOpacityCorrectionFactor, float fMaxSampleRateFactor) const
{
	// [TODO] This currently only works in case there's a texture buffer instance, we may also want to use this in case there's just image data on the host side

	// Is there a volume texture buffer?
	TextureBuffer *pTextureBuffer = static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
	if (pTextureBuffer) {
		float fStepSize = 1.0f;

		// Security check: Volume texture level of detail (0...<number of mipmaps>-1), usually the value 3 shows a well notable effect (2.2 - Fetch Scalar), must >=0
		if (fVolumeTextureLOD < 0.0f)
			fVolumeTextureLOD = 0.0f;
		if (static_cast<uint32>(Math::Ceil(fVolumeTextureLOD)) > pTextureBuffer->GetNumOfMipmaps())
			fVolumeTextureLOD = static_cast<float>(pTextureBuffer->GetNumOfMipmaps());

		// Get the current and next mipmap level
		const uint32 nCurrentMipmapSize = static_cast<uint32>(Math::Floor(fVolumeTextureLOD));
		const uint32 nNextMipmapSize    = static_cast<uint32>(Math::Ceil(fVolumeTextureLOD));
		const float  fMipmapFactor      = 1.0f - (nNextMipmapSize - fVolumeTextureLOD);

		// Get the size of the volume (= number of voxels along each axis)
		Vector3 vSize;
		const Vector3i &vCurrentMipmapSize = pTextureBuffer->GetUniformSize(nCurrentMipmapSize);
		if (fMipmapFactor < 1.0f) {
			// Get the size of the next mipmap level
			const Vector3i &vNextMipmapSize = pTextureBuffer->GetUniformSize(nNextMipmapSize);

			// Interpolate mipmap size
			vSize.x = vCurrentMipmapSize.x + (vNextMipmapSize.x - vCurrentMipmapSize.x)*fMipmapFactor;
			vSize.y = vCurrentMipmapSize.y + (vNextMipmapSize.y - vCurrentMipmapSize.y)*fMipmapFactor;
			vSize.z = vCurrentMipmapSize.z + (vNextMipmapSize.z - vCurrentMipmapSize.z)*fMipmapFactor;
		} else {
			vSize.x = static_cast<float>(vCurrentMipmapSize.x);
			vSize.y = static_cast<float>(vCurrentMipmapSize.y);
			vSize.z = static_cast<float>(vCurrentMipmapSize.z);
		}

		// Security check: The global sample rate factor should never ever be too big, else we can stall the complete system via the GPU forcing a complete reboot
		if (fSampleRateFactor > fMaxSampleRateFactor)
			fSampleRateFactor = fMaxSampleRateFactor;

		// Nyquist–Shannon sampling theorem
		// -> Quote from the book "Real-Time Volume Graphics", section "9.1.1 Sampling Theorem" (page 217)
		//    "It states that, when converting analog signals to digital, the sampling frequency must be greater than twice the
		//     highest frequency of the input signal to be able to later reconstruct the original signal from the sampled version perfectly."
		//     ...
		//     "Thus, in order to accurately reconstruct the original signal from the discrete data, we need to take at least two samples per smallest inter-voxel distance."
		float fSampleRate = 0.5f;
		fSampleRate *= 1/fSampleRateFactor;	// Apply our factor to make it easy to influence the sampling rate

		// Get the longest volume axis -> "smallest inter-voxel distance"
		const float fLongestAxis = Math::Max(vSize.x, Math::Max(vSize.y, vSize.z));

		// Calculate the base/reference step size (1/x = highest frequency)
		const float fBaseStepSize = fLongestAxis ? 1.0f/fLongestAxis : 0.0f;

		// Calculate the actual step size
		fStepSize = fBaseStepSize*fSampleRate;

		// Calculate opacity correction factor
		// -> Described in the book "Real-Time Volume Graphics", section "9.1.3 Opacity Correction" (page 219)
		// -> Also described in "GPU Gems", section "39.4.3 Rendering" ( http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html )
		if (pfOpacityCorrectionFactor)
			*pfOpacityCorrectionFactor = fStepSize/fBaseStepSize;

		// Done
		return fStepSize;
	} else {
		// Error!
		if (pfOpacityCorrectionFactor)
			*pfOpacityCorrectionFactor = 0.0f;
		return -1.0f;
	}
}

/**
*  @brief
*    Creates a histogram instance by using the base map of the volume data
*/
Histogram *Volume::CreateHistogram()
{
	// Get the image holding the volumetric data (internal image data is not copied, just shared, so this is pretty lightweight)
	Image cImage = m_cVolumeImage;

	// Do we need to download the volume data from the GPU?
	if (!cImage.GetNumOfParts()) {
		// Get the renderer texture buffer holding the 3D voxel data
		TextureBuffer *pTextureBuffer = static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
		if (pTextureBuffer) {
			// -> We may get an memory issue in here using huge datasets within a 32 bit build
			// -> In case we're receiving compressed data, the final histogram will not be completely identical to
			//    a histogram of the original uncompressed data (which we don't have when we're in here, so we can't do anything about it)
			// -> We only need the base-map, so, no mipmaps required (would be a waste of resources in here)
			pTextureBuffer->CopyDataToImage(cImage, false);
		}
	}

	// Get the image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer) {
		// Create the histogram and return the instance
		return new Histogram(pImageBuffer->GetBytesPerPixel(), pImageBuffer->GetNumOfVoxels(), pImageBuffer->GetData());
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns the renderer texture buffer holding the 3D voxel data
*/
TextureBuffer *Volume::GetVolumeTextureBuffer() const
{
	return static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
}

/**
*  @brief
*    Returns the renderer texture buffer holding the 3D voxel data
*/
TextureBuffer *Volume::GetVolumeTextureBuffer(Renderer &cRenderer, bool bTextureCompression, bool bMipmaps)
{
	// Create the texture buffer right now?
	if (!m_pTextureBufferHandler->GetResource() && m_cVolumeImage.GetNumOfParts()) {
		// Get the image buffer
		ImageBuffer *pImageBuffer = m_cVolumeImage.GetBuffer();
		if (pImageBuffer) {
			// Get the renderer capabilities instance
			const Capabilities &cCapabilities = cRenderer.GetCapabilities();

			// Is the image to big for the GPU? (ignore texture compression which may e.g. only half the required texture memory)
			if (pImageBuffer->GetDataSize()/1024 > cCapabilities.nTotalAvailableGPUMemory) {
				// Apply scale
				// [TODO] Figure out correct scale value
		//		cImage.ApplyEffect(ImageEffects::Scale(pImageBuffer->GetSize()/2, false));
				int ii = 0;
			}

			// Get texture buffer flags
			uint32 nFlags = (bTextureCompression ? TextureBuffer::Compression : 0);
			// [HACK] Don't use mipmaps when using 2D array textures, had a crash on my system
			// -> Used system: "ATI Mobility Radeon HD 4850" (512 MiB) using Catalyst 12.1 and Windows 7 64 bit
			if (bMipmaps && cCapabilities.bTextureBuffer3D)
				nFlags |= TextureBuffer::Mipmaps;

			// Automatic 3D or array texture detection
			// -> Prefer 3D textures because we're getting automatic trilinear filtering
			// -> When using 2D array textures, we just have automatic bilinear filtering and have to perform filtering between the layers within the fragment shader
			// -> In case there's no other option, use a 2D texture atlas [TODO] Add support for texture atlas in case neither 3D, nor 2D array textures are available
			// [TODO] Add a trigger so we can set/force this from the outside? (would be good for testing)
			if (cCapabilities.bTextureBuffer3D)
				m_pTextureBufferHandler->SetResource(reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer3D(m_cVolumeImage, TextureBuffer::Unknown, nFlags)));
			else if (cCapabilities.bTextureBuffer2DArray)
				m_pTextureBufferHandler->SetResource(reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer2DArray(m_cVolumeImage, TextureBuffer::Unknown, nFlags)));
		}

		// We no longer need the image data
		// [TODO] Make this optional?
		m_cVolumeImage.Unload();
	}

	// Return the texture buffer
	return static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Resource functions             ]
//[-------------------------------------------------------]
Volume &Volume::operator =(const Volume &cSource)
{
	// Unload the old volume
	Unload();

	// Call base function
	*static_cast<PLCore::Resource<Volume>*>(this) = cSource;

	// Copy volume
	m_cVolumeImage		= cSource.m_cVolumeImage;
	m_vVoxelSize		= cSource.m_vVoxelSize;
//	m_cTransferFunction	= cSource.m_cTransferFunction;	// [TODO] Implement me

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Volume::Unload()
{
	// Unload the image holding the volumetric data
	m_cVolumeImage.Unload();

	// Unload the transfer function
	m_pTransferFunctionControl->Unload();

	// Call base implementation
	return PLCore::Resource<Volume>::Unload();
}

String Volume::GetLoadableTypeName() const
{
	static const String sString = "Volume";
	return sString;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Volume::Volume(const Volume &cSource)
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
