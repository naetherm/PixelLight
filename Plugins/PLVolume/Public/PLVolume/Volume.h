/*********************************************************\
 *  File: Volume.h                                       *
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


#ifndef __PLVOLUME_VOLUME_H__
#define __PLVOLUME_VOLUME_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Resource.h>
#include <PLMath/Vector3.h>
#include <PLGraphics/Image/Image.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
	class TextureBuffer;
	class ResourceHandler;
}
namespace PLVolume {
	class Histogram;
	class TransferFunction;
	class TransferFunctionControl;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Volume resource
*/
class Volume : public PLCore::Resource<Volume> {


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLVOLUME_API const PLMath::Vector3 DefaultVoxelSize;	/**< 0.001, 0.001, 0.001 */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Resource name to set
		*  @param[in] pManager
		*    Resource manager using this resource, can be a null pointer
		*/
		PLVOLUME_API Volume(const PLCore::String &sName = "", PLCore::ResourceManager<Volume> *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~Volume();

		/**
		*  @brief
		*    Returns the image holding the volumetric data
		*
		*  @return
		*    The image holding the volumetric data
		*/
		PLVOLUME_API PLGraphics::Image &GetVolumeImage();

		/**
		*  @brief
		*    Returns the image holding the volumetric data
		*
		*  @return
		*    The image holding the volumetric data
		*/
		PLVOLUME_API const PLGraphics::Image &GetVolumeImage() const;

		/**
		*  @brief
		*    Returns the size of the image holding the volumetric data
		*
		*  @return
		*    The size of the image holding the volumetric data
		*/
		PLVOLUME_API PLMath::Vector3i GetVolumeImageSize() const;

		/**
		*  @brief
		*    Returns the size of one voxel
		*
		*  @return
		*    The size of one voxel (without metric, but usually one unit is equal to one meter)
		*/
		PLVOLUME_API const PLMath::Vector3 &GetVoxelSize() const;

		/**
		*  @brief
		*    Sets the size of one voxel
		*
		*  @param[in] vVoxelSize
		*    The size of one voxel (without metric, but usually one unit is equal to one meter)
		*/
		PLVOLUME_API void SetVoxelSize(const PLMath::Vector3 &vVoxelSize);

		/**
		*  @brief
		*    Returns the transfer function
		*
		*  @return
		*    The transfer function
		*/
		PLVOLUME_API TransferFunction &GetTransferFunction();

		/**
		*  @brief
		*    Returns the transfer function
		*
		*  @return
		*    The transfer function
		*/
		PLVOLUME_API const TransferFunction &GetTransferFunction() const;

		/**
		*  @brief
		*    Returns the transfer function control
		*
		*  @return
		*    The transfer function control
		*/
		PLVOLUME_API TransferFunctionControl &GetTransferFunctionControl();

		/**
		*  @brief
		*    Returns the transfer function control
		*
		*  @return
		*    The transfer function control
		*/
		PLVOLUME_API const TransferFunctionControl &GetTransferFunctionControl() const;

		/**
		*  @brief
		*    Returns the step size
		*
		*  @param[in]  fVolumeTextureLOD
		*    Volume texture level of detail (0...<number of mipmaps>-1), usually the value 3 shows a well notable effect (2.2 - Fetch Scalar), must >=0
		*  @param[in]  fSampleRateFactor
		*    Sample rate factor to use (1.0 = 100% for correct result, 0.0 = 50% = take only half of the samples)
		*  @param[out] pfOpacityCorrectionFactor
		*    If not a null pointer, this receives the opacity correction factor, 0 on error
		*  @param[in]  fMaxSampleRateFactor
		*    Security check: The global sample rate factor should never ever be too big, else we can stall the complete system via the GPU forcing a complete reboot
		*
		*  @return
		*    The step size, <0 on error (you really want to check for error)
		*
		*  @remarks
		*    Nyquist–Shannon sampling theorem
		*    -> Quote from the book "Real-Time Volume Graphics", section "9.1.1 Sampling Theorem" (page 217)
		*       "It states that, when converting analog signals to digital, the sampling frequency must be greater than twice the
		*        highest frequency of the input signal to be able to later reconstruct the original signal from the sampled version perfectly."
		*        ...
		*        "Thus, in order to accurately reconstruct the original signal from the discrete data, we need to take at least two samples per smallest inter-voxel distance."
		*
		*    Opacity correction factor as described
		*    - Within the book "Real-Time Volume Graphics", section "9.1.3 Opacity Correction" (page 219)
		*    - In "GPU Gems", section "39.4.3 Rendering" (http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html)
		*/
		PLVOLUME_API float GetStepSize(float fVolumeTextureLOD = 0.0f, float fSampleRateFactor = 1.0f, float *pfOpacityCorrectionFactor = nullptr, float fMaxSampleRateFactor = 10.0f) const;

		/**
		*  @brief
		*    Creates a histogram instance by using the base map of the volume data
		*
		*  @return
		*    Histogram instance created by using the base map of the volume data, null pointer on error (destroy the instance if you no longer need it)
		*
		*  @remarks
		*    For the histogram, we need the volume image data. If we're in luck it's still on the host memory, else we have
		*    to download the data temporarily from the GPU and possibly let it uncompress on the fly. May not be optimal for huge datasets.
		*
		*  @note
		*    - This method may internally need to download the data from the GPU (memory & performance may be an issue on huge datasets)
		*/
		PLVOLUME_API Histogram *CreateHistogram();

		//[-------------------------------------------------------]
		//[ GPU relevant part                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the renderer texture buffer holding the 3D voxel data
		*
		*  @return
		*    Renderer texture buffer holding the 3D voxel data, can be a null pointer
		*
		*  @remarks
		*    The texture type can be "PLRenderer::Resource::TypeTextureBuffer3D" or "PLRenderer::Resource::TypeTextureBuffer2DArray".
		*/
		// [TODO] texture atlas
		PLVOLUME_API PLRenderer::TextureBuffer *GetVolumeTextureBuffer() const;

		/**
		*  @brief
		*    Returns the renderer texture buffer holding the 3D voxel data
		*
		*  @param[in] cRenderer
		*    Used renderer instance
		*  @param[in] bTextureCompression
		*    Is the usage of texture compression allowed? (if yes, the renderer will automatically try to find the best fitting texture compression format for the given image data)
		*  @param[in] bMipmaps
		*    Is the usage of mipmaps allowed? (Lookout! May take a moment to generate mipmaps if no mipmap data is provided.)
		*
		*  @return
		*    Renderer texture buffer holding the 3D voxel data, can be a null pointer
		*
		*  @note
		*    - Texture buffer is created automatically on-the-fly if there's currently no such instance
		*
		*  @see
		*    - "GetVolumeTextureBuffer()"-method above
		*/
		// [TODO] texture atlas
		PLVOLUME_API PLRenderer::TextureBuffer *GetVolumeTextureBuffer(PLRenderer::Renderer &cRenderer, bool bTextureCompression = true, bool bMipmaps = true);


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Resource functions             ]
	//[-------------------------------------------------------]
	public:
		PLVOLUME_API virtual Volume &operator =(const Volume &cSource);


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLVOLUME_API virtual bool Unload() override;
		PLVOLUME_API virtual PLCore::String GetLoadableTypeName() const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Volume(const Volume &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		PLGraphics::Image			 m_cVolumeImage;				/**< Image holding the volumetric data */
		PLMath::Vector3				 m_vVoxelSize;					/**< Size of one voxel (without metric, but usually one unit is equal to one meter) */
		TransferFunctionControl		*m_pTransferFunctionControl;	/**< Transfer function control, always valid */
		// Cached texture buffer
		PLRenderer::ResourceHandler *m_pTextureBufferHandler;		/**< Renderer texture buffer holding the 3D voxel data, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_VOLUME_H__
