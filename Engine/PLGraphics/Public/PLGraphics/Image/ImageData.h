/*********************************************************\
 *  File: ImageData.h                                    *
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


#ifndef __PLGRAPHICS_IMAGEDATA_H__
#define __PLGRAPHICS_IMAGEDATA_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/RefCount.h>
#include <PLMath/Vector3i.h>
#include "PLGraphics/PLGraphics.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ImagePalette;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Image data, contains the actual data of an image buffer
*
*  @note
*    - Implementation of the proxy design pattern, this class is the real subject
*/
class ImageData : protected PLCore::RefCount<ImageData> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ImageBuffer;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		inline ImageData();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ImageData(const ImageData &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ImageData();

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
		PLGRAPHICS_API ImageData &operator =(const ImageData &cSource);

		/**
		*  @brief
		*    Create image
		*
		*  @param[in] nDataFormat
		*    Desired data format
		*  @param[in] nColorFormat
		*    Desired color format
		*  @param[in] vSize
		*    Image size
		*  @param[in] nCompression
		*    Compression type
		*
		*  @remarks
		*    Please note that a call to Create() will not immediately create the image buffer itself.
		*    The image buffer is created on the first call to GetData() or GetCompressedData() respectively.
		*/
		PLGRAPHICS_API void CreateImage(EDataFormat nDataFormat, EColorFormat nColorFormat, const PLMath::Vector3i &vSize, ECompression nCompression = CompressionNone);

		/**
		*  @brief
		*    Clear data
		*/
		PLGRAPHICS_API void Clear();

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
		PLGRAPHICS_API void CreateTestImage(ETestImage nTestImage = TestImage2DSimple);

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
		PLGRAPHICS_API void SetCompression(ECompression nCompression);

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
		*    Get size of image data
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
		*/
		PLGRAPHICS_API const PLCore::uint8 *GetData() const;
		PLGRAPHICS_API PLCore::uint8 *GetData();

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
		*    Get size of compressed image data
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
		*/
		PLGRAPHICS_API const PLCore::uint8 *GetCompressedData() const;
		PLGRAPHICS_API PLCore::uint8 *GetCompressedData();

		/**
		*  @brief
		*    Compress image data
		*
		*  @remarks
		*    If the image contains uncompressed image data, the data will be
		*    compressed and stored in the compressed image buffer.
		*/
		PLGRAPHICS_API bool Compress();

		/**
		*  @brief
		*    Decompress image data
		*
		*  @remarks
		*    If the image contains compressed image data, the data will be
		*    decompressed and stored in the decompressed image buffer.
		*/
		PLGRAPHICS_API bool Decompress();

		/**
		*  @brief
		*    Set color palette
		*
		*  @param[in] pPalette
		*    Pointer to color palette, or a null pointer if the image has no palette
		*/
		PLGRAPHICS_API void SetPalette(ImagePalette *pPalette);

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
		*    Copy provided uncompressed image data into this image data
		*
		*  @param[in] pnData
		*    Data to copy into this image data, must have enough bytes to fill the whole image! The given pointer must be valid!
		*/
		PLGRAPHICS_API void CopyData(const PLCore::uint8 *pnData);

		/**
		*  @brief
		*    Let this image data takeover provided uncompressed image data
		*
		*  @param[in] pnData
		*    Pointer to the image data to be taken over by this image data, must have enough bytes to fill the whole image data! The given pointer must be valid!
		*/
		PLGRAPHICS_API void TakeoverData(PLCore::uint8 *pnData);

		/**
		*  @brief
		*    Let this image data share provided uncompressed image data
		*
		*  @param[in] pnData
		*    Pointer to the image data to be shared by this image data, must have enough bytes to fill the whole image data! The given pointer must be valid!
		*/
		PLGRAPHICS_API void ShareData(PLCore::uint8 *pnData);

		/**
		*  @brief
		*    Create image buffer
		*
		*  @remarks
		*    The buffer is only created, if it doesn't exist before. So it's safe to call it
		*    in order to get a valid buffer before accessing it, this won't destroy anything.
		*/
		inline void CreateBuffer();

		/**
		*  @brief
		*    Create compressed image buffer
		*
		*  @remarks
		*    The buffer is only created, if it doesn't exist before. So it's safe to call it
		*    in order to get a valid buffer before accessing it, this won't destroy anything.
		*/
		inline void CreateCompressedBuffer();

		/**
		*  @brief
		*    Destroy image buffer
		*/
		inline void DestroyBuffer();

		/**
		*  @brief
		*    Destroy compressed image buffer
		*/
		inline void DestroyCompressedBuffer();

		/**
		*  @brief
		*    Calculates the size of the compressed image buffer
		*/
		void CalculateCompressedImageBufferSize();

		/**
		*  @brief
		*    Decodes a DXT color block
		*
		*  @param[out] pnDestination
		*    Receives the decompressed data
		*  @param[in]  pnSource
		*    Compressed source data
		*  @param[in]  nBlockWidth
		*    Width of the block, not allowed to be above 4 pixel
		*  @param[in]  nBlockHeight
		*    Height of the block, not allowed to be above 4 pixel
		*  @param[in]  nXOffset
		*    Current x position offset inside the image
		*  @param[in]  nYOffset
		*    Current y position offset inside the image
		*  @param[in]  nCompression
		*    Used compression
		*  @param[in]  nRedIndex
		*    Index of the red color component
		*  @param[in]  nBlueIndex
		*    Index of the blue color component
		*
		*  @note
		*    - Please note that in this method, there's no destination buffer overflow test, so correct the block size if required before calling this method!
		*/
		void DecodeDXTColorBlock(PLCore::uint8 *pnDestination, const PLCore::uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset, ECompression nCompression, int nRedIndex, int nBlueIndex) const;

		/**
		*  @brief
		*    Decodes a DXT3 alpha block
		*
		*  @param[out] pnDestination
		*    Receives the decompressed data
		*  @param[in]  pnSource
		*    Compressed source data
		*  @param[in]  nBlockWidth
		*    Width of the block, not allowed to be above 4 pixel
		*  @param[in]  nBlockHeight
		*    Height of the block, not allowed to be above 4 pixel
		*  @param[in]  nXOffset
		*    Current x position offset inside the image
		*  @param[in]  nYOffset
		*    Current y position offset inside the image
		*
		*  @note
		*    - Please note that in this method, there's no destination buffer overflow test, so correct the block size if required before calling this method!
		*/
		void DecodeDXT3AlphaBlock(PLCore::uint8 *pnDestination, const PLCore::uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset) const;

		/**
		*  @brief
		*    Decodes a DXT5 alpha block
		*
		*  @param[out] pnDestination
		*    Receives the decompressed data
		*  @param[in]  pnSource
		*    Compressed source data
		*  @param[in]  nBlockWidth
		*    Width of the block, not allowed to be above 4 pixel
		*  @param[in]  nBlockHeight
		*    Height of the block, not allowed to be above 4 pixel
		*  @param[in]  nXOffset
		*    Current x position offset inside the image
		*  @param[in]  nYOffset
		*    Current y position offset inside the image
		*
		*  @note
		*    - Please note that in this method, there's no destination buffer overflow test, so correct the block size if required before calling this method!
		*/
		void DecodeDXT5AlphaBlock(PLCore::uint8 *pnDestination, const PLCore::uint8 *pnSource, int nBlockWidth, int nBlockHeight, int nXOffset, int nYOffset) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EDataFormat		 m_nDataFormat;		/**< Data format of the image */
		EColorFormat	 m_nColorFormat;	/**< Color format of the image */
		ECompression	 m_nCompression;	/**< Compression format */
		PLMath::Vector3i m_vSize;			/**< Size of the image */
		PLCore::uint8	*m_pData;			/**< Uncompressed image data, can be a null pointer */
		PLCore::uint32	 m_nDataSize;		/**< Size of uncompressed image data in bytes */
		bool			 m_bDataShared;		/**< If 'true', the uncompressed image data is just shared and this image data instance is not allowed to deleted it (the user keeps the image data ownership) */
		PLCore::uint8	*m_pCompressedData;	/**< Compressed image data, can be a null pointer */
		PLCore::uint32	 m_nCompressedSize;	/**< Size of compressed image data in bytes */
		ImagePalette	*m_pPalette;		/**< Color palette, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLGraphics/Image/ImageData.inl"


#endif // __PLGRAPHICS_IMAGEDATA_H__
