/*********************************************************\
 *  File: TextureBuffer.h                                *
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


#ifndef __PLRENDERER_TEXTUREBUFFER_H__
#define __PLRENDERER_TEXTUREBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3i.h>
#include <PLGraphics/PLGraphics.h>
#include "PLRenderer/Renderer/Resource.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Image;
	class Color4;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer texture buffer resource base class
*
*  @remarks
*    In the literature, for a volume an element is called voxel, and a texture element is called texel.
*    To stay name compatible with PLGraphics::ImageBuffer, we will just use the term pixel.
*/
class TextureBuffer : public Resource {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RendererBackend;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Texture buffer flags
		*/
		enum EFlags {
			Mipmaps      = 1<<0,	/**< Use mipmaps (see GetNumOfMipmaps() for more information) */
			Compression  = 1<<1,	/**< Use texture buffer compression if possible (is ignored if a certain internal format is forced) */
			RenderTarget = 1<<2		/**< This texture buffer can be used as render target */
		};

		/**
		*  @brief
		*    Texture buffer pixel formats
		*/
		enum EPixelFormat {
			// Unsigned formats
			L8            =  0,	/**< 8-bit pixel format, all bits luminance */
			L16           =  1,	/**< 16-bit pixel format, all bits luminance */
			A8            =  2,	/**< 8-bit pixel format, all bits alpha */
			L4A4          =  3,	/**< 8-bit pixel format, 4 bits for luminance and alpha */
			L8A8          =  4,	/**< 16-bit pixel format, 8 bits for luminance and alpha */
			D16           =  5,	/**< 16-bit z-buffer bit depth */
			D24           =  6,	/**< 32-bit z-buffer bit depth using 24 bits for the depth channel */
			D32           =  7,	/**< 32-bit z-buffer bit depth */
			R3G3B2        =  8,	/**< 8-bit pixel format, 3 bits red, 3 bits green and 2 bits blue  */
			R5G6B5        =  9,	/**< 16-bit pixel format, 5 bits red, 6 bits green and 5 bits blue  */
			R5G5B5A1      = 10,	/**< 16-bit pixel format, 5 bits red, 5 bits green, 5 bits blue and 1 bits alpha  */
			R4G4B4A4      = 11,	/**< 16-bit pixel format, 4 bits for red, green, blue and alpha  */
			R8G8B8        = 12,	/**< 24-bit (or 32-bit if 24-bits are not supported by the hardware) pixel format, 8 bits for red, green and blue */
			R8G8B8A8      = 13,	/**< 32-bit pixel format, 8 bits for red, green, blue and alpha */
			R10G10B10A2   = 14,	/**< 32-bit pixel format, 10 bits for red, green, blue and 2 bits for alpha */
			R16G16B16A16  = 15,	/**< 64-bit pixel format, 16 bits for red, green, blue and alpha */
			DXT1          = 16,	/**< DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) */
			DXT3          = 17,	/**< DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) */
			DXT5          = 18,	/**< DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) */
			LATC1         = 19,	/**< 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) */
			LATC2         = 20, /**< 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) */
			// Floating-point formats
			L16F          = 21,	/**< 16-bit float format using 16 bits for luminance */
			L32F          = 22,	/**< 32-bit float format using 32 bits for luminance */
			R16G16B16A16F = 23,	/**< 64-bit float format using 16 bits for the each channel (red, green, blue, alpha) */
			R32G32B32A32F = 24,	/**< 128-bit float format using 32 bits for the each channel (red, green, blue, alpha) */
			// End
			Unknown       = 25	/**< Unknown texture buffer pixel format */
		};
		static const PLCore::uint32 NumOfPixelFormats = 25;	/**< Number of pixel formats */


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the texture buffer pixel format of a given image (PLGraphics::Image)
		*
		*  @param[in] cImage
		*    Image to get the texture buffer pixel format from
		*  @param[in] bNoCompression
		*    Do NOT use texture buffer compression?
		*
		*  @return
		*    The texture buffer pixel format of the given image
		*/
		static PLRENDERER_API EPixelFormat GetFormatFromImage(const PLGraphics::Image &cImage, bool bNoCompression = false);

		/**
		*  @brief
		*    Returns whether or not the given texture buffer format is compressed
		*
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*
		*  @return
		*    'true' if the texture buffer format is compressed, else 'false'
		*/
		static PLRENDERER_API bool IsCompressedFormat(EPixelFormat nFormat);

		/**
		*  @brief
		*    Returns whether or not the given texture buffer format is a depth buffer format
		*
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*
		*  @return
		*    'true' if the texture buffer format is a depth buffer format, else 'false'
		*/
		static PLRENDERER_API bool IsDepthFormat(EPixelFormat nFormat);

		/**
		*  @brief
		*    If the given texture buffer format is compressed, a proper uncompressed texture buffer format will be returned
		*
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*
		*  @return
		*    Uncompressed texture buffer format, if the given format was not compressed, the given format will be returned
		*/
		static PLRENDERER_API EPixelFormat GetUncompressedFormat(EPixelFormat nFormat);

		/**
		*  @brief
		*    Returns whether or not the given texture buffer format is a floating point format
		*
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*
		*  @return
		*    'true' if the texture buffer format is a floating point format, else 'false'
		*/
		static PLRENDERER_API bool IsFloatingPointFormat(EPixelFormat nFormat);

		/**
		*  @brief
		*    Returns the number of components per pixel
		*
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*
		*  @return
		*    The number of components per pixel, 0 if unknown format
		*/
		static PLRENDERER_API PLCore::uint32 GetComponentsPerPixel(EPixelFormat nFormat);

		/**
		*  @brief
		*    Returns the number of bytes per pixel component
		*
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*
		*  @return
		*    The number of bytes per pixel component, 0 if unknown format or no full byte per component
		*/
		static PLRENDERER_API PLCore::uint32 GetBytesPerPixelComponent(EPixelFormat nFormat);

		/**
		*  @brief
		*    Returns the number of bytes per pixel
		*
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*
		*  @return
		*    The number of bytes per pixel, 0 if unknown format or no full byte per pixel (compressed format)
		*/
		static PLRENDERER_API PLCore::uint32 GetBytesPerPixel(EPixelFormat nFormat);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureBuffer();

		/**
		*  @brief
		*    Returns the texture buffer flags
		*
		*  @return
		*    The texture buffer flags (see EFlags)
		*/
		inline PLCore::uint32 GetFlags() const;

		/**
		*  @brief
		*    Returns the texture buffer pixel format
		*
		*  @return
		*    The texture buffer pixel format
		*/
		inline EPixelFormat GetFormat() const;

		/**
		*  @brief
		*    Returns whether or not the used texture buffer format is compressed
		*
		*  @return
		*    'true' if the used texture buffer format is compressed, else 'false'
		*/
		inline bool IsCompressedFormat() const;

		/**
		*  @brief
		*    Returns whether or not the used texture buffer format is a depth buffer format
		*
		*  @return
		*    'true' if the used texture buffer format is a depth buffer format, else 'false'
		*/
		inline bool IsDepthFormat() const;

		/**
		*  @brief
		*    If the used texture buffer format is compressed, a proper uncompressed texture buffer format will be returned
		*
		*  @return
		*    Uncompressed texture buffer format, if the used format was not compressed, the used format will be returned
		*/
		inline EPixelFormat GetUncompressedFormat() const;

		/**
		*  @brief
		*    Returns whether or not the used texture buffer format is a floating point format
		*
		*  @return
		*    'true' if the texture buffer format is a floating point format, else 'false'
		*/
		inline bool IsFloatingPointFormat() const;

		/**
		*  @brief
		*    Returns the number of components per pixel
		*
		*  @return
		*    The number of components per pixel, 0 if unknown format
		*/
		inline PLCore::uint32 GetComponentsPerPixel() const;

		/**
		*  @brief
		*    Returns the number of bytes per pixel component
		*
		*  @return
		*    The number of bytes per pixel component, 0 if unknown format or no full byte per component
		*/
		inline PLCore::uint32 GetBytesPerPixelComponent() const;

		/**
		*  @brief
		*    Returns the number of bytes per pixel
		*
		*  @return
		*    The number of bytes per pixel, 0 if unknown format or no full byte per pixel (compressed format)
		*/
		inline PLCore::uint32 GetBytesPerPixel() const;

		/**
		*  @brief
		*    Returns the number of mipmap levels
		*
		*  @return
		*    The number of mipmap levels, 0 if there are no mipmaps
		*    (maybe not supported or not desired for this texture buffer)
		*
		*  @remarks
		*    Level 0 is the 'original' base image and GetNumOfMipmaps() is the version with the lowest resolution
		*    which is normally 1x1. For a 2D texture buffer, the number of mipmaps down to 1x1 is calculated by
		*    log2(max(width, height)). For a 32x16 texture buffer, this will result in the following additional
		*    mipmaps: 16x8, 8x4, 4x2, 2x1, 1x1
		*
		*    Note that mipmaps are only created/used from a given image if the flag 'Mipmaps' is set.
		*/
		inline PLCore::uint32 GetNumOfMipmaps() const;

		/**
		*  @brief
		*    Returns the texture buffer size as a 3D vector
		*
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*
		*  @return
		*    Texture buffer size as a 3D vector
		*
		*  @note
		*    - Comfort method so one does not have to use the concrete texture buffer type just to get the texture size
		*/
		PLRENDERER_API PLMath::Vector3i GetUniformSize(PLCore::uint32 nMipmap = 0) const;

		/**
		*  @brief
		*    Returns the number of faces
		*
		*  @return
		*    The number of faces, always >0!
		*/
		inline PLCore::uint8 GetNumOfFaces() const;

		/**
		*  @brief
		*    Returns the total number of pixels including all mipmaps
		*
		*  @return
		*    The total number of pixels including all mipmaps
		*/
		PLRENDERER_API PLCore::uint32 GetTotalNumOfPixels() const;

		/**
		*  @brief
		*    Returns the total number of bytes required for the texture buffer data including all mipmaps
		*
		*  @return
		*    The number of bytes required for the texture buffer data including all mipmaps
		*
		*  @note
		*    - If the texture buffer is using a compressed format, compression is also taken into account
		*/
		inline PLCore::uint32 GetTotalNumOfBytes() const;

		/**
		*  @brief
		*    Returns the image (PLGraphics::Image) settings required to be able to store the texture buffer data within an image
		*
		*  @param[out] nDataFormat
		*    Receives the required image data format
		*  @param[out] nColorFormat
		*    Receives the required image color format
		*  @param[out] nCompression
		*    Receives the required image compression
		*  @param[out] nTextureBufferFomat
		*    Receives the required texture buffer format to use within "TextureBuffer::CopyDataTo()" in order to ask the GPU for the texture buffer data
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    This method maps the internal texture buffer format as close as possible to image data format and image color format.
		*    -> We may also need to choose another texture buffer format for the GPU texture buffer data request
		*    -> We can't e.g. just change PLGraphics in order to use the same pixel format scheme as the texture buffer
		*       because PLGraphics has to be generic, while texture buffer has to support special GPU formats usually not
		*       used in generic image processing
		*/
		PLRENDERER_API bool GetFormatForImage(PLGraphics::EDataFormat &nDataFormat, PLGraphics::EColorFormat &nColorFormat, PLGraphics::ECompression &nCompression, EPixelFormat &nTextureBufferFomat) const;

		/**
		*  @brief
		*    Returns the texture buffer data as image
		*
		*  @param[out] cImage
		*    Receives the texture buffer data (no need to allocated the image, this is done automatically, just pass in any image instance)
		*  @param[in]  bMipmaps
		*    Do also take mipmaps into account?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool CopyDataToImage(PLGraphics::Image &cImage, bool bMipmaps = true) const;

		/**
		*  @brief
		*    Downloads the texture buffer content and returns the number of NAN values in it
		*
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*  @param[in] nFace
		*    ID of the texture buffer face which should be downloaded if this is a cube texture buffer
		*
		*  @return
		*    Number of NAN values (pixel components are counted individually)
		*
		*  @remarks
		*    If there are NAN values inside a texture buffer used for rendering, nasty "black boxes" (NVIDIA) or
		*    "white boxes" (ATI) may appear. This function may help you to pinpoint the place were the NANs are produces.
		*
		*  @note
		*    - This function can only be used if the texture buffer format is a floating point one
		*    - This function should only be used for debugging
		*/
		PLRENDERER_API PLCore::uint32 GetNumOfNANValues(PLCore::uint32 nMipmap = 0, PLCore::uint8 nFace = 0) const;

		/**
		*  @brief
		*    Downloads the texture buffer content, replaces pixels containing a NAN value by a given color, and uploads the fixed texture buffer
		*
		*  @param[in] cColor
		*    Color for pixels containing a NAN value
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*  @param[in] nFace
		*    ID of the texture buffer face which should be fixed if this is a cube texture buffer
		*
		*  @return
		*    Number of fixed NAN pixels
		*
		*  @see
		*    - GetNumOfNANValues()
		*/
		PLRENDERER_API PLCore::uint32 FixNANValues(const PLGraphics::Color4 &cColor, PLCore::uint32 nMipmap = 0, PLCore::uint8 nFace = 0);


	//[-------------------------------------------------------]
	//[ Public virtual TextureBuffer functions                ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether or not the texture has a power of two dimension (POT)
		*
		*  @return
		*    'true' if the texture has a power of two dimension, else 'false'
		*
		*  @remarks
		*    Although modern graphics APIs and graphics hardware is often capable to deal with
		*    non power of two dimension, there might be certain restrictions up to unexpected
		*    driver crashes within certain situations. This methods exists to make it easier
		*    to check for this sort of texture dimension. A texture is only power of two when
		*    it's power of two along each axis - except array textures which are considered
		*    to be a stack of 2D textures.
		*/
		virtual bool IsPowerOfTwo() const = 0;

		/**
		*  @brief
		*    Returns the number of pixels of a certain mipmap level
		*
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*
		*  @return
		*    The number of pixels of a certain mipmap level
		*
		*  @note
		*    - If this is a cube texture, the number of pixels of all faces together will be returned
		*/
		virtual PLCore::uint32 GetNumOfPixels(PLCore::uint32 nMipmap = 0) const = 0;

		/**
		*  @brief
		*    Returns the number of bytes required to hold certain mipmap level texture buffer data
		*
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*  @param[in] nFormat
		*    Target texture buffer pixel format, if Unknown, the format of THIS texture buffer is chosen automatically
		*
		*  @return
		*    The number of bytes required to hold certain mipmap level texture buffer data
		*
		*  @note
		*    - If 'nFormat' is a compressed format, compression is also taken into account
		*    - If this is a cube texture, the number of bytes of all faces together will be returned
		*/
		virtual PLCore::uint32 GetNumOfBytes(PLCore::uint32 nMipmap = 0, EPixelFormat nFormat = Unknown) const = 0;

		/**
		*  @brief
		*    Uploads data to the GPU
		*
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*  @param[in] nFormat
		*    Texture buffer pixel format of 'pData', can not be 'Unknown'. If this is a compressed format,
		*    it MUST be the same format this texture buffer has.
		*  @param[in] pData
		*    Pointer to the data to upload, MUST have at least GetNumOfBytes(nMipmap, nFormat) bytes!
		*  @param[in] nFace
		*    ID of the texture buffer face which should be uploaded if this is a cube texture buffer
		*
		*  @return
		*    'true' if all went fine, else 'false' (invalid mipmap level? invalid data?)
		*/
		virtual bool CopyDataFrom(PLCore::uint32 nMipmap, EPixelFormat nFormat, const void *pData, PLCore::uint8 nFace = 0) = 0;

		/**
		*  @brief
		*    Downloads data from the GPU
		*
		*  @param[in]  nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*  @param[in]  nFormat
		*    Texture buffer pixel format of 'pData', can not be 'Unknown'. If this is a compressed format,
		*    it MUST be the same format this texture buffer has.
		*  @param[out] pData
		*    Will receive the downloaded data, MUST have at least GetNumOfBytes(nMipmap, nFormat) bytes!
		*  @param[in] nFace
		*    ID of the texture buffer face which should be downloaded if this is a cube texture buffer
		*
		*  @return
		*    'true' if all went fine, else 'false' (invalid mipmap level? invalid data?)
		*/
		virtual bool CopyDataTo(PLCore::uint32 nMipmap, EPixelFormat nFormat, void *pData, PLCore::uint8 nFace = 0) const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] nType
		*    Resource type
		*  @param[in] nFlags
		*    Texture buffer flags (see EFlags)
		*/
		PLRENDERER_API TextureBuffer(Renderer &cRenderer, EType nType, PLCore::uint32 nFlags);


	//[-------------------------------------------------------]
	//[ Protected virtual TextureBuffer functions             ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Makes this texture buffer to the renderers current texture buffer
		*
		*  @param[in] nStage
		*    Texture stage
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool MakeCurrent(PLCore::uint32 nStage) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::uint32 m_nFlags;			/**< Texture buffer flags (see EFlags) */
		EPixelFormat   m_nFormat;			/**< Texture buffer pixel format */
		PLCore::uint32 m_nNumOfElements;	/**< The number of elements */
		PLCore::uint32 m_nNumOfMipmaps;		/**< Number of mipmap levels (the base level is 0) */
		PLCore::uint32 m_nTotalNumOfBytes;	/**< Total number of bytes required to the texture buffer data including all mipmaps */


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
		TextureBuffer(const TextureBuffer &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		TextureBuffer &operator =(const TextureBuffer &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/TextureBuffer.inl"


#endif // __PLRENDERER_TEXTUREBUFFER_H__
