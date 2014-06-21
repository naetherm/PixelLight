/*********************************************************\
 *  File: ImageBuffer.h                                  *
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


#ifndef __PLGRAPHICS_IMAGEBUFFER_H__
#define __PLGRAPHICS_IMAGEBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3i.h>
#include "PLGraphics/PLGraphics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ImageData;
class ImagePalette;
class ImageEffect;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image buffer class
*
*  @remarks
*    The image buffer class uses a 'copy on change' technique - therefore copying one image
*    buffer into another is quite performant because the internal image buffer data is shared
*    as long as a image buffer doesn't change.
*
*  @note
*    - The origin (0, 0) is on the upper-left ("top-down")
*    - The image components are stored in a linear byte array. RGB example:\n
*      R1G1B1R2G2B2R3G3B3...
*    - Implementation of the proxy design pattern, this class is the proxy
*/
class ImageBuffer {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the number of components per pixel
		*
		*  @param[in] nColorFormat
		*    Color format
		*
		*  @return
		*    The number of components per pixel
		*/
		static PLGRAPHICS_API PLCore::uint32 GetComponentsPerPixel(EColorFormat nColorFormat);

		/**
		*  @brief
		*    Returns the number of bytes per pixel component
		*
		*  @param[in] nDataFormat
		*    Data format
		*
		*  @return
		*    The number of bytes per pixel component
		*/
		static PLGRAPHICS_API PLCore::uint32 GetBytesPerPixelComponent(EDataFormat nDataFormat);

		/**
		*  @brief
		*    Returns the number of bytes per pixel
		*
		*  @param[in] nDataFormat
		*    Data format
		*  @param[in] nColorFormat
		*    Color format
		*
		*  @return
		*    The number of bytes per pixel
		*/
		static inline PLCore::uint32 GetBytesPerPixel(EDataFormat nDataFormat, EColorFormat nColorFormat);

		/**
		*  @brief
		*    Returns the size of a certain mipmap
		*
		*  @param[in] nBaseSize
		*    Size of the base level
		*  @param[in] nMipmap
		*    Mipmap level
		*
		*  @return
		*    The size of a certain mipmap
		*/
		static PLGRAPHICS_API PLCore::uint32 GetMipmapSize(PLCore::uint32 nBaseSize, PLCore::uint32 nMipmap);

		/**
		*  @brief
		*    Returns whether or not the given color format has an alpha channel
		*
		*  @param[in] nColorFormat
		*    Color format to check
		*
		*  @return
		*    'true' if the given color format has an alpha channel, else 'false'
		*/
		static inline bool IsAlphaChannelColorFormat(EColorFormat nColorFormat);

		/**
		*  @brief
		*    Returns a version with alpha channel of the given color format
		*
		*  @param[in] nColorFormat
		*    Color format to return a color format with alpha channel from
		*
		*  @return
		*    Color format with alpha channel on success, same color format channel if the given color format
		*    has already a alpha channel or when there's no version of the given color format with alpha channel
		*/
		static PLGRAPHICS_API EColorFormat GetAlphaChannelColorFormat(EColorFormat nColorFormat);

		/**
		*  @brief
		*    Returns a version without alpha channel of the given color format
		*
		*  @param[in] nColorFormat
		*    Color format to return a color format without alpha channel from
		*
		*  @return
		*    Color format without alpha channel on success, same color format channel if the given color format
		*    has already no alpha channel or when there's no version of the given color format without alpha channel
		*/
		static PLGRAPHICS_API EColorFormat GetNoneAlphaChannelColorFormat(EColorFormat nColorFormat);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline ImageBuffer();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline ImageBuffer(const ImageBuffer &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ImageBuffer();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this object
		*/
		inline ImageBuffer &operator =(const ImageBuffer &cSource);

		/**
		*  @brief
		*    Create image
		*
		*  @param[in] nDataFormat
		*    Desired data format (for example "DataByte")
		*  @param[in] nColorFormat
		*    Desired color format (for example "ColorRGB")
		*  @param[in] vSize
		*    Image size (for example "PLMath::Vector3i(64, 32, 1)")
		*  @param[in] nCompression
		*    Compression type (for example "CompressionNone")
		*
		*  @remarks
		*    Please note that a call to CreateImage() will not immediately create the image buffer itself.
		*    The image buffer is created on the first call to GetData() or GetCompressedData() respectively.
		*/
		inline void CreateImage(EDataFormat nDataFormat, EColorFormat nColorFormat, const PLMath::Vector3i &vSize, ECompression nCompression = CompressionNone);

		/**
		*  @brief
		*    Clear data
		*/
		inline void Clear();

		/**
		*  @brief
		*    Apply image effect
		*
		*  @param[in] cEffect
		*    Image effect
		*/
		PLGRAPHICS_API void ApplyEffect(const ImageEffect &cEffect);

		/**
		*  @brief
		*    Check image for possible problems
		*
		*  @return
		*    Error code describing any problem found, 'CheckOk' if everything is fine
		*/
		PLGRAPHICS_API ECheckConsistency CheckConsistency() const;

		/**
		*  @brief
		*    Create a test image
		*
		*  @param[in] nTestImage
		*    ID of test image
		*
		*  @remarks
		*    Only 2D and 3D test images are allowed here. For images containing more than
		*    one image buffer, you have to use Image::CreateTestImage().
		*/
		inline void CreateTestImage(ETestImage nTestImage = TestImage2DSimple);

		/**
		*  @brief
		*    Get data format
		*
		*  @return
		*    Data format
		*/
		inline EDataFormat GetDataFormat() const;

		/**
		*  @brief
		*    Get color format
		*
		*  @return
		*    Color format
		*/
		inline EColorFormat GetColorFormat() const;

		/**
		*  @brief
		*    Returns the number of components per pixel
		*
		*  @return
		*    The number of components per pixel
		*/
		inline PLCore::uint32 GetComponentsPerPixel() const;

		/**
		*  @brief
		*    Returns the number of bytes per pixel component
		*
		*  @return
		*    The number of bytes per pixel component
		*/
		inline PLCore::uint32 GetBytesPerPixelComponent() const;

		/**
		*  @brief
		*    Returns the number of bytes per pixel
		*
		*  @return
		*    The number of bytes per pixel
		*/
		inline PLCore::uint32 GetBytesPerPixel() const;

		/**
		*  @brief
		*    Get compression type
		*
		*  @return
		*    Compression type
		*/
		inline ECompression GetCompression() const;

		/**
		*  @brief
		*    Set compression type
		*
		*  @param[in] nCompression
		*    Compression type
		*
		*  @remarks
		*    This will invalidate the compressed image data buffer
		*/
		inline void SetCompression(ECompression nCompression);

		/**
		*  @brief
		*    Get image size
		*
		*  @return
		*    Size of image
		*/
		inline PLMath::Vector3i GetSize() const;

		/**
		*  @brief
		*    Returns the number of pixels
		*
		*  @return
		*    The number of pixels (width*height, no depth)
		*/
		inline PLCore::uint32 GetNumOfPixels() const;

		/**
		*  @brief
		*    Returns the number of voxels
		*
		*  @return
		*    The number of voxels (width*height*depth)
		*/
		inline PLCore::uint32 GetNumOfVoxels() const;

		/**
		*  @brief
		*    Check if uncompressed or compressed data is available
		*
		*  @return
		*    'true' if image data is available, else 'false'
		*/
		inline bool HasAnyData() const;

		/**
		*  @brief
		*    Check if uncompressed data is available
		*
		*  @return
		*    'true' if image data is available, else 'false'
		*/
		inline bool HasData() const;

		/**
		*  @brief
		*    Get size of image data in bytes
		*
		*  @return
		*    Size of image data in bytes
		*/
		inline PLCore::uint32 GetDataSize() const;

		/**
		*  @brief
		*    Get size of one row of image data in bytes
		*
		*  @return
		*    Size of one row in bytes
		*/
		inline PLCore::uint32 GetBytesPerRow() const;

		/**
		*  @brief
		*    Get size of one xy-plane (z/depth layer) of image data in bytes
		*
		*  @return
		*    Size of one xy-plane in bytes
		*/
		inline PLCore::uint32 GetBytesPerPlane() const;

		/**
		*  @brief
		*    Get image data
		*
		*  @return
		*    Pointer to image data, a null pointer on error
		*
		*  @remarks
		*    If the image contains no uncompressed data, but compressed image data is available the data,
		*    will be decompressed automatically and stored in the decompressed image buffer this method returns.
		*
		*  @note
		*    - Lookout! This method is dangerous and must be used with care! Do always ensure that you never read or write over the data buffer boundings!
		*/
		inline const PLCore::uint8 *GetData() const;
		inline PLCore::uint8 *GetData();

		/**
		*  @brief
		*    Check if compressed data is available
		*
		*  @return
		*    'true' if compressed image data is available, else 'false'
		*/
		inline bool HasCompressedData() const;

		/**
		*  @brief
		*    Get size of compressed image data in bytes
		*
		*  @return
		*    Size of compressed image data in bytes
		*/
		inline PLCore::uint32 GetCompressedDataSize() const;

		/**
		*  @brief
		*    Get compressed image data
		*
		*  @return
		*    Pointer to compressed image data, a null pointer on error
		*
		*  @note
		*    - Lookout! This method is dangerous and must be used with care! Do always ensure that you never read or write over the data buffer boundings!
		*/
		inline const PLCore::uint8 *GetCompressedData() const;
		inline PLCore::uint8 *GetCompressedData();

		/**
		*  @brief
		*    Compress image data
		*
		*  @remarks
		*    If the image contains uncompressed image data, the data will be
		*    compressed and stored in the compressed image buffer.
		*/
		inline bool Compress();

		/**
		*  @brief
		*    Decompress image data
		*
		*  @remarks
		*    If the image contains compressed image data, the data will be
		*    decompressed and stored in the decompressed image buffer.
		*/
		inline bool Decompress();

		/**
		*  @brief
		*    Set color palette
		*
		*  @param[in] pPalette
		*    Pointer to color palette, or a null pointer if the image has no palette
		*/
		inline void SetPalette(ImagePalette *pPalette);

		/**
		*  @brief
		*    Get color palette
		*
		*  @return
		*    Pointer to color palette, or a null pointer if the image has no palette
		*/
		inline const ImagePalette *GetPalette() const;
		inline ImagePalette *GetPalette();

		/**
		*  @brief
		*    Copy provided uncompressed image data into this image buffer
		*
		*  @param[in] pnData
		*    Data to copy into this image buffer, must have enough bytes to fill the whole image! If null pointer, this method does nothing.
		*
		*  @note
		*    - Lookout! This method is dangerous and must be used with care! Do always ensure that your given image data has enough bytes for this image buffer!
		*/
		inline void CopyData(const PLCore::uint8 *pnData);

		/**
		*  @brief
		*    Let this image buffer takeover provided uncompressed image data
		*
		*  @param[in] pnData
		*    Pointer to the image data to be taken over by this image buffer, must have enough bytes to fill the whole image buffer! If null pointer, this method does nothing.
		*
		*  @note
		*    - "Takeover" means that this image buffer will destroy the provided data when it's no longer used!
		*    - Lookout! This method is dangerous and must be used with care! Do always ensure that your given image data has enough bytes for this image buffer!
		*/
		inline void TakeoverData(PLCore::uint8 *pnData);

		/**
		*  @brief
		*    Let this image buffer share provided uncompressed image data
		*
		*  @param[in] pnData
		*    Pointer to the image data to be shared by this image buffer, must have enough bytes to fill the whole image buffer! If null pointer, this method does nothing.
		*
		*  @note
		*    - "Share" means that this image buffer will not destroy the provided data when it's no longer used!
		*    - Please be aware that your provided image data must stay valid during the lifetime of the image buffer instance (and potential image buffer copies!)
		*    - While this method is quite efficient, it's also quite error prone, so be really careful when using this method
		*    - Lookout! This method is dangerous and must be used with care! Do always ensure that your given image data has enough bytes for this image buffer!
		*/
		inline void ShareData(PLCore::uint8 *pnData);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Make new empty image data
		*/
		PLGRAPHICS_API void MakeNewBuffer();

		/**
		*  @brief
		*    Make image data unique
		*/
		PLGRAPHICS_API void MakeBufferUnique();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ImageData *m_pImageData;	/**< Shared image data, always valid! */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLGraphics/Image/ImageBuffer.inl"


#endif // __PLGRAPHICS_IMAGEBUFFER_H__
