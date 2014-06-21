/*********************************************************\
 *  File: TextureBuffer3D.cpp                            *
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
#include <PLCore/Log/Log.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/TextureBuffer3D.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
TextureBuffer3D::~TextureBuffer3D()
{
	if (m_nOpenGLTexture)
		glDeleteTextures(1, &m_nOpenGLTexture);

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nTextureBuffersNum--;
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nTextureBuffersMem -= GetTotalNumOfBytes();
}

/**
*  @brief
*    Returns the OpenGL resource ID of the texture buffer
*/
uint32 TextureBuffer3D::GetOpenGLTexture() const
{
	return m_nOpenGLTexture;
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
void TextureBuffer3D::InitialUploadVolumeData(Renderer &cRendererOpenGL, const ImageBuffer &cImageBuffer, const Vector3i &vSize, bool bUsePreCompressedData, bool bCompressedFormat, EPixelFormat nImageFormat, GLenum nOpenGLTarget, GLint nOpenGLLevel, GLenum nOpenGLInternalformat, GLenum nOpenGLFormat, GLenum nOpenGLType, EPixelFormat &nPrimaryImageFormat)
{
	// Upload the texture buffer
	if (bUsePreCompressedData && cImageBuffer.HasCompressedData()) {
		// Pass thru the already compressed data to the GPU
		CompressedTexImage3D(cRendererOpenGL, nOpenGLTarget, nOpenGLLevel, nOpenGLInternalformat, vSize, cImageBuffer.GetCompressedDataSize(), cImageBuffer.GetCompressedData());
	} else {
		// When trying to upload a 512x512x1743 8 bit (435,75 MiB) uncompressed volume to the GPU using one "glTexImage3DEXT"-call and asking for
		// LATC1 as internal format (resulting in 217,875 MiB), the result was a crash inside the graphics driver.
		// -> Used system: "ATI Mobility Radeon HD 4850" (512 MiB) using Catalyst 11.11 and Windows 7 64 bit
		// -> Using 512x512x128 voxel (32,00 MiB) was working
		// -> Using 512x512x255 voxel (63,75 MiB) was working as well
		// -> Using 512x512x256 voxel (64,00 MiB) or more -> Crash inside the graphics driver
		// -> So, to be on the safe side, do not upload big volume textures using one "glTexImage3DEXT"-call,
		//    split the upload into multiple "glTexSubImage3DEXT"-calls
		// -> Hopefully 32,00 MiB per "glTexSubImage3DEXT"-call is fine across various graphics cards
		// -> On NVIDIA GeForce GTX 285 (2048 MiB) using driver version 285.62 WHQL and Windows 7 64 bit,
		//    there were no such issues and it was possible to upload the data in once single call
		static const uint32 nMaximumNumberOfBytesPerCall = 512*512*128;

		// Is the "GL_NV_texture_compression_vtc"-extension there?
		// -> If so, we're probably on a NVIDIA GPU, disable this hack
		// -> Has the texture data a critical size?
		if (cImageBuffer.HasAnyData() && cImageBuffer.GetDataSize() > nMaximumNumberOfBytesPerCall &&
			(!cRendererOpenGL.GetContext().GetExtensions().IsGL_NV_texture_compression_vtc() ||
			  cImageBuffer.GetDataSize()/1024 > cRendererOpenGL.GetCapabilities().nTotalAvailableGPUMemory)) {	// Total available GPU memory is in kilobytes
			// Split the upload into multiple "glTexSubImage3DEXT"-calls to be on the safe side

			// Update the texture data
			if (cImageBuffer.HasAnyData()) {
				// Get the number of bytes per depth layer
				const uint32 nBytesPerPlane = cImageBuffer.GetBytesPerPlane();

				// Get a pointer to the start position of the given image data
				const uint8 *pImageData = cImageBuffer.GetData();

				// Lookout!
				// -> The "GL_SGIS_generate_mipmap"-extension (http://www.opengl.org/registry/specs/SGIS/generate_mipmap.txt) specification states
				// "
				//  If GENERATE_MIPMAP_SGIS is enabled, the side effect occurs whenever
				//  any change is made to the interior or edge image values of the base
				//  level texture array.  The side effect is computation of a complete
				//  set of mipmap arrays, all derived from the modified base level array.
				// "
				// -> We really have to disable the automatic mipmap generation for the following split, else mipmaps are generated
				//    for every single uploaded depth layer... which may take ages on large volume data...
				GLint nGL_GENERATE_MIPMAP_SGIS = 0;
				if (!nOpenGLLevel && cRendererOpenGL.GetContext().GetExtensions().IsGL_SGIS_generate_mipmap()) {
					// Get current state
					glGetTexParameteriv(GL_TEXTURE_3D_EXT, GL_GENERATE_MIPMAP_SGIS, &nGL_GENERATE_MIPMAP_SGIS);

					// Disable automatic mipmap generation
					if (nGL_GENERATE_MIPMAP_SGIS)
						glTexParameteri(GL_TEXTURE_3D_EXT, GL_GENERATE_MIPMAP_SGIS, 0);
				}

				// Ask the GPU to allocate the texture
				glTexImage3DEXT(nOpenGLTarget, nOpenGLLevel, nOpenGLInternalformat, vSize.x, vSize.y, vSize.z, 0, nOpenGLFormat, nOpenGLType, nullptr);

				// Add each depth layer by using a separate OpenGL API call
				for (int i=0; i<vSize.z-1; i++) {
					// Update the image data of the current depth layer
					glTexSubImage3DEXT(nOpenGLTarget, nOpenGLLevel, 0, 0, i, vSize.x, vSize.y, 1, nOpenGLFormat, nOpenGLType, pImageData);
					pImageData += nBytesPerPlane;
				}

				// Ok, we're now about to upload the last depth layer, reactivate automatic mipmap generation if required
				if (nGL_GENERATE_MIPMAP_SGIS)
					glTexParameteri(GL_TEXTURE_3D_EXT, GL_GENERATE_MIPMAP_SGIS, 1);

				// Update the image data of the last depth layer
				glTexSubImage3DEXT(nOpenGLTarget, nOpenGLLevel, 0, 0, vSize.z-1, vSize.x, vSize.y, 1, nOpenGLFormat, nOpenGLType, pImageData);
			} else {
				// Ask the GPU to allocate the texture
				glTexImage3DEXT(nOpenGLTarget, nOpenGLLevel, nOpenGLInternalformat, vSize.x, vSize.y, vSize.z, 0, nOpenGLFormat, nOpenGLType, nullptr);
			}
		} else {
			// We can do all in one simple "glTexImage3DEXT"-call
			glTexImage3DEXT(nOpenGLTarget, nOpenGLLevel, nOpenGLInternalformat, vSize.x, vSize.y, vSize.z, 0, nOpenGLFormat, nOpenGLType, cImageBuffer.HasAnyData() ? cImageBuffer.GetData() : nullptr);
		}
	}

	// If compressed internal format, check whether all went fine
	if (bCompressedFormat) {
		GLint nCompressed;
		glGetTexLevelParameteriv(nOpenGLTarget, nOpenGLLevel, GL_TEXTURE_COMPRESSED_ARB, &nCompressed);
		if (!nCompressed) {
			// There was an error, use no compression as fallback
			nPrimaryImageFormat = nImageFormat;
			const uint32 *pAPIPixelFormatFallback = cRendererOpenGL.GetAPIPixelFormat(nPrimaryImageFormat);
			if (pAPIPixelFormatFallback) {
				// ... aaaaannd again...
				InitialUploadVolumeData(cRendererOpenGL, cImageBuffer, vSize, false, false, nImageFormat, nOpenGLTarget, nOpenGLLevel, *pAPIPixelFormatFallback, nOpenGLFormat, nOpenGLType, nPrimaryImageFormat);
			}
		}
	}
}

// "glCompressedTexImage3DARB"-version handling some special behavior across different GPU's
void TextureBuffer3D::CompressedTexImage3D(const Renderer &cRendererOpenGL, GLenum nOpenGLTarget, GLint nOpenGLLevel, GLenum nOpenGLInternalformat, const Vector3i &vSize, uint32 nCompressedDataSize, const uint8 *pCompressedData)
{
	// Reason for the existence of this method:
	// When uploading e.g. LATC1 compressed 3D textures on AMD/ATI GPU's, there are no issues. When doing the same on NVIDIA GPU's
	// e.g. a rendered volume using this data looks pretty odd. The issue 13 in the "EXT_texture_compression_latc"-specification
	// (http://www.opengl.org/registry/specs/EXT/texture_compression_latc.txt) explains this weird behavior:
	// "
	//   13) Should these formats be allowed to specify 3D texture images
	//   when NV_texture_compression_vtc is supported?

	//   RESOLVED: The NV_texture_compression_vtc stacks 4x4 blocks into
	//   4x4x4 bricks.  It may be more desirable to represent compressed
	//   3D textures as simply slices of 4x4 blocks.

	//   However the NV_texture_compression_vtc extension expects
	//   data passed to the glCompressedTexImage commands to be "bricked"
	//   rather than blocked slices.
	// "
	// So, we need to check for this extension. If it's there, we need to reorder the texture data before uploading
	// it to the GPU. It's impossible to deactivate this NVIDIA "feature", so we have to life with it.

	// Is the "GL_NV_texture_compression_vtc"-extension there?
	if (vSize.z > 1 && cRendererOpenGL.GetContext().GetExtensions().IsGL_NV_texture_compression_vtc()) {
		// Go the complicated way
		// -> This is not tuned for maximum performance, so we're allocating/deallocating a temporary
		//    buffer in here in order to keep this nasty behavior handling as local as possible

		// Pointer to the original compressed data
		const uint8 *pCompressedDataCurrent = pCompressedData;

		// Allocate memory for the reordered compressed data
		uint8 *pReorderedData = new uint8[nCompressedDataSize];

		// Reorder the compressed data as described within http://www.nvidia.com/dev_content/nvopenglspecs/GL_NV_texture_compression_vtc.txt
		// (quoted in here because it's quite important to understand how it's working in detail)
		// "
		//  VTC Compressed Texture Image Formats
		//
		//  Each VTC compression format is similar to a corresponding S3TC
		//  compression format, but where an S3TC block encodes a 4x4 block of
		//  texels, a VTC block encodes a 4x4x1, 4x4x2, or 4x4x4 block of texels.
		//  If the depth of the image is four or greater, 4x4x4 blocks are used,
		//  and if the depth is 1 or 2, 4x4x1 or 4x4x2 blocks are used.
		//  The size in bytes of a VTC image with dimensions w, h, and d is:
		//
		//    ceil(w/4) * ceil(h/4) * d * blocksize,
		//
		//  where blocksize is the size of an analogous 4x4 S3TC block and is
		//  either 8 or 16 bytes.
		//
		//  The block containing a texel at location (x,y,z) starts at an offset
		//  inside the image of:
		//
		//    blocksize * min(d,4) * (floor(x/4) + ceil(w/4) * (floor(y/4) + ceil(h/4) * floor(z/4)))
		//
		//  bytes.
		//
		//  A 4x4x1 block of each of the four formats is stored in exactly the
		//  same way that a 4x4 block of the analogous S3TC format is stored.
		//
		//  A 4x4x2 or 4x4x4 block is stored as two or four consecutive 4x4
		//  blocks of the analogous S3TC format, one for each layer inside the
		//  block.  For example, a 4x4x2 DXT1 block consists of 16 bytes in
		//  total.  The first 8 bytes encode the texels at locations (0,0,0)
		//  through (3,3,0), and the second 8 bytes encode the texels at
		//  locations (0,0,1) through (3,3,1).
		// "

		// Get the block depth
		const uint32 nBlockDepth = (vSize.z > 2) ? 4 : 2;

		// Blocksize, either 8 or 16 bytes
		const uint32 nNumOfBytesPerBlock = (nOpenGLInternalformat == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || nOpenGLInternalformat == GL_COMPRESSED_LUMINANCE_LATC1_EXT) ? 8 : 16;

		// Get the number of bytes per depth layer block (e.g. four depth layers are within a VTC block)
		const uint32 nNumOfBytesPerLayerBlock = nCompressedDataSize/vSize.z*nBlockDepth;

		// Get the number of blocks along the x-axis and y-axis
		const int nNumOfXBlocks = (vSize.x + 3)/4;
		const int nNumOfYBlocks = (vSize.y + 3)/4;

		// Loop through all depth layers along the z-axis
		for (int z=0; z<vSize.z; z++) {
			// Get start position inside the current depth layer block
			const uint32 nFloorZ = static_cast<uint32>(Math::Floor(z/static_cast<float>(nBlockDepth)));
			const uint32 nBlockStartOffset = nFloorZ*nNumOfBytesPerLayerBlock;

			// Take care of the last block layer
			const uint32 nCurrentZEnd = nFloorZ*nBlockDepth + nBlockDepth;
			const uint32 nCurrentBlockDepth = (static_cast<int>(nCurrentZEnd) > vSize.z) ? (nBlockDepth - (nCurrentZEnd - vSize.z)) : nBlockDepth;

			// Loop through all blocks along the y-axis
			for (int y=0; y<nNumOfYBlocks; y++) {
				// Loop through all block along the x-axis
				for (int x=0; x<nNumOfXBlocks; x++) {
					// Get start position inside the current depth layer block
					uint32 nBlockOffset = nBlockStartOffset;

					// Add the index of the block inside the current depth layer block
					nBlockOffset += (x + y*nNumOfXBlocks)*nNumOfBytesPerBlock*nCurrentBlockDepth;

					// Add the depth offset
					nBlockOffset += (z%nCurrentBlockDepth)*nNumOfBytesPerBlock;

					// Copy the block to the new location
					MemoryManager::Copy(pReorderedData + nBlockOffset, pCompressedDataCurrent, nNumOfBytesPerBlock);

					// Update the original compressed data pointer
					pCompressedDataCurrent += nNumOfBytesPerBlock;
				}
			}
		}

		// Finally, upload the reordered compressed data
		glCompressedTexImage3DARB(nOpenGLTarget, nOpenGLLevel, nOpenGLInternalformat, vSize.x, vSize.y, vSize.z, 0, nCompressedDataSize, pReorderedData);

		// Deallocate memory of the reordered compressed data
		delete [] pReorderedData;
	} else {
		// Go the simple way
		glCompressedTexImage3DARB(nOpenGLTarget, nOpenGLLevel, nOpenGLInternalformat, vSize.x, vSize.y, vSize.z, 0, nCompressedDataSize, pCompressedData);
	}
}

// "glGetCompressedTexImageARB"-version handling some special behavior across different GPU's
void TextureBuffer3D::GetCompressedTexImage(const Renderer &cRendererOpenGL, GLenum nOpenGLTarget, GLint nOpenGLLevel, EPixelFormat nFormat, const Vector3i &vSize, uint8 *pCompressedData)
{
	// See "TextureBuffer3D::CompressedTexImage3D()" for detailed comments, this in here is nearly the same implementation

	// Is the "GL_NV_texture_compression_vtc"-extension there?
	if (vSize.z > 1 && cRendererOpenGL.GetContext().GetExtensions().IsGL_NV_texture_compression_vtc()) {
		// Go the complicated way
		// -> This is not tuned for maximum performance, so we're allocating/deallocating a temporary
		//    buffer in here in order to keep this nasty behavior handling as local as possible

		// Get the block depth
		const uint32 nBlockDepth = (vSize.z > 2) ? 4 : 2;

		// Blocksize, either 8 or 16 bytes
		const uint32 nNumOfBytesPerBlock = (nFormat == DXT1 || nFormat == LATC1) ? 8 : 16;

		// Pointer to the resulting compressed data
		uint8 *pCompressedDataCurrent = pCompressedData;

		// Calculate the size of the compressed data
		// http://www.nvidia.com/dev_content/nvopenglspecs/GL_NV_texture_compression_vtc.txt
		// "
		//  The size in bytes of a VTC image with dimensions w, h, and d is:
		//
		//    ceil(w/4) * ceil(h/4) * d * blocksize,
		// "
		const uint32 nCompressedDataSize = static_cast<uint32>(Math::Ceil(vSize.x/4.0f))*static_cast<uint32>(Math::Ceil(vSize.y/4.0f))*vSize.z*nNumOfBytesPerBlock;

		// Allocate memory for the reordered compressed data we're receiving from the GPU
		uint8 *pReorderedData = new uint8[nCompressedDataSize];

		// Download the reordered compressed data from the GPU
		glGetCompressedTexImageARB(nOpenGLTarget, nOpenGLLevel, pReorderedData);

		// Get the number of bytes per depth layer block (e.g. four depth layers are within a VTC block)
		const uint32 nNumOfBytesPerLayerBlock = nCompressedDataSize/vSize.z*nBlockDepth;

		// Get the number of blocks along the x-axis and y-axis
		const int nNumOfXBlocks = (vSize.x + 3)/4;
		const int nNumOfYBlocks = (vSize.y + 3)/4;

		// Loop through all depth layers along the z-axis
		for (int z=0; z<vSize.z; z++) {
			// Get start position inside the current depth layer block
			const uint32 nFloorZ = static_cast<uint32>(Math::Floor(z/static_cast<float>(nBlockDepth)));
			const uint32 nBlockStartOffset = nFloorZ*nNumOfBytesPerLayerBlock;

			// Take care of the last block layer
			const uint32 nCurrentZEnd = nFloorZ*nBlockDepth + nBlockDepth;
			const uint32 nCurrentBlockDepth = (static_cast<int>(nCurrentZEnd) > vSize.z) ? (nBlockDepth - (nCurrentZEnd - vSize.z)) : nBlockDepth;

			// Loop through all blocks along the y-axis
			for (int y=0; y<nNumOfYBlocks; y++) {
				// Loop through all block along the x-axis
				for (int x=0; x<nNumOfXBlocks; x++) {
					// Get start position inside the current depth layer block
					uint32 nBlockOffset = nBlockStartOffset;

					// Add the index of the block inside the current depth layer block
					nBlockOffset += (x + y*nNumOfXBlocks)*nNumOfBytesPerBlock*nCurrentBlockDepth;

					// Add the depth offset
					nBlockOffset += (z%nCurrentBlockDepth)*nNumOfBytesPerBlock;

					// Copy the block to the new location
					MemoryManager::Copy(pCompressedDataCurrent, pReorderedData + nBlockOffset, nNumOfBytesPerBlock);

					// Update the original compressed data pointer
					pCompressedDataCurrent += nNumOfBytesPerBlock;
				}
			}
		}

		// Deallocate memory of the reordered compressed data
		delete [] pReorderedData;
	} else {
		// Go the simple way
		glGetCompressedTexImageARB(nOpenGLTarget, nOpenGLLevel, pCompressedData);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureBuffer3D::TextureBuffer3D(PLRenderer::Renderer &cRenderer, Image &cImage, EPixelFormat nInternalFormat, uint32 nFlags) :
	PLRenderer::TextureBuffer3D(cRenderer, nFlags),
	m_nOpenGLTexture(0)
{
	// Get the OpenGL renderer instance
	Renderer &cRendererOpenGL = static_cast<Renderer&>(cRenderer);

	// Update renderer statistics
	cRendererOpenGL.GetWritableStatistics().nTextureBuffersNum++;

	// Initialize sampler states
	MemoryManager::Set(m_nSamplerState, PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);

	// Choose the texture buffer pixel formats which should be used
	EPixelFormat nImageFormat;
	bool bUsePreCompressedData;
	m_nFormat = cRendererOpenGL.ChooseFormats(cImage, nInternalFormat, nFlags, nImageFormat, bUsePreCompressedData);

	// Get the first image part
	const ImagePart *pImagePart = cImage.GetPart(0);
	if (pImagePart) {
		// Get the first image buffer
		const ImageBuffer *pImageBuffer = pImagePart->GetMipmap(0);
		if (pImageBuffer) {
			// Get API pixel format
			const uint32 *pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
			if (pAPIPixelFormat) {
				// Is this a compressed texture buffer pixel format?
				const bool bCompressedFormat = IsCompressedFormat();

				// Get uncompressed image format information
				EPixelFormat nImageFormatUncompressed     = GetFormatFromImage(cImage, true);
				uint32		 nAPIImageFormatUncompressed  = cRendererOpenGL.GetOpenGLPixelFormat(nImageFormatUncompressed);
				uint32		 nImageDataFormatUncompressed = cRendererOpenGL.GetOpenGLDataFormat(nImageFormatUncompressed);

				// Set the size
				m_vSize = pImageBuffer->GetSize();

				// Create OpenGL texture buffer
				glGenTextures(1, &m_nOpenGLTexture);
				glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

				// Upload the texture buffer
				m_nNumOfMipmaps = pImagePart->GetNumOfMipmaps() - 1;
				const bool bMipmaps = nFlags & Mipmaps;
				if (!m_nNumOfMipmaps && bMipmaps) {
					// Calculate the number of mipmaps
					m_nNumOfMipmaps = static_cast<uint32>(Math::Log2(static_cast<float>(Math::Max(Math::Max(m_vSize.x, m_vSize.y), m_vSize.z))));

					// Try to build mipmaps automatically on the GPU
					if (cRendererOpenGL.GetContext().GetExtensions().IsGL_SGIS_generate_mipmap()) {
						glTexParameteri(GL_TEXTURE_3D_EXT, GL_GENERATE_MIPMAP_SGIS, true);

						// Upload
						InitialUploadVolumeData(cRendererOpenGL, *pImageBuffer, m_vSize, bUsePreCompressedData, bCompressedFormat, nImageFormat, GL_TEXTURE_3D_EXT, 0, *pAPIPixelFormat, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, m_nFormat);
					} else {
						// [TODO] It looks like that gluBuild3DMipmaps() is NOT supported under Windows :(
						m_nNumOfMipmaps = 0;
						glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, *pAPIPixelFormat, m_vSize.x, m_vSize.y, m_vSize.z, 0, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pImageBuffer->HasAnyData() ? pImageBuffer->GetData() : nullptr);

						// No pre compressed image data can be used
						// gluBuild3DMipmaps(GL_TEXTURE_3D_EXT, *pAPIPixelFormat, m_vSize.x, m_vSize.y, m_vSize.z,
						//				  nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pImageBuffer->HasAnyData() ? pImageBuffer->GetData() : nullptr);

						// If compressed internal format, check whether all went fine
						// if (bCompressedFormat) {
						// 	GLint nCompressed;
						// 	glGetTexLevelParameteriv(GL_TEXTURE_3D_EXT, 0, GL_TEXTURE_COMPRESSED_ARB, &nCompressed);
						// 	if (!nCompressed) { // There was an error, use no compression
						// 		m_nFormat = nImageFormat;
						// 		const uint32 *pAPIPixelFormatFallback = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
						// 		if (pAPIPixelFormatFallback) {
						// 			gluBuild3DMipmaps(GL_TEXTURE_3D_EXT, *pAPIPixelFormatFallback, m_vSize.x, m_vSize.y, m_vSize.z,
						// 							  nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pImageBuffer->HasAnyData() ? pImageBuffer->GetData() : nullptr);
						// 		}
						// 	}
						// }
					}

					// Calculate the total number of bytes this texture buffer requires
					for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++)
						m_nTotalNumOfBytes += GetNumOfBytes(nLevel);
				} else {
					// Ignore mipmaps?
					if (!bMipmaps)
						m_nNumOfMipmaps = 0; // Jep, we do not want to have any mipmaps for this texture buffer

					// Use given mipmaps
					Vector3i vSize;
					uint32 nLevel = 0;
					for (; nLevel<=m_nNumOfMipmaps; nLevel++) {
						// Get the mipmap image buffer
						const ImageBuffer *pMipmapImageBuffer = pImagePart->GetMipmap(nLevel);
						if (pMipmapImageBuffer) {
							// Get the size of this mipmap level
							vSize = pMipmapImageBuffer->GetSize();

							// Upload
							InitialUploadVolumeData(cRendererOpenGL, *pMipmapImageBuffer, vSize, bUsePreCompressedData, bCompressedFormat, nImageFormat, GL_TEXTURE_3D_EXT, nLevel, *pAPIPixelFormat, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, m_nFormat);

							// Update the total number of bytes this texture buffer requires
							m_nTotalNumOfBytes += GetNumOfBytes(nLevel);
						}
					}

					// We have to define all mipmap levels down to 1x1x1 otherwise the texture buffer is invalid when we try to use any min
					// filter that uses mipmaps. OpenGL "normally" uses white color when invalid/incomplete texture buffer is enabled.
					if (bMipmaps && (vSize.x != 1 || vSize.y != 1 || vSize.z != 1)) {
						// Write a warning into the log
						PL_LOG(Warning, String::Format("Lowest mipmap is %dx%dx%d, but should be 1x1x1! Missing mipmap levels will be white!", vSize.x, vSize.y, vSize.z))

						// If we don't define all mipmap levels down to 1x1x1 'mysterious' graphics bugs may occur were it is not
						// always easy to pinpoint the problem directly to the mipmaps. So, to avoid frustration during bug finding,
						// we just create the missing mipmap levels down to 1x1x1 with a white color - so it's possible to 'see' which texture
						// isn't correct without reading the log message from above. (for some people it appears to be hard to read logs *g*)
						char *pszBuffer = nullptr;
						do {
							// First at all, 'half' the current dimension
							if (vSize.x > 1)
								vSize.x /= 2;
							if (vSize.y > 1)
								vSize.y /= 2;
							if (vSize.z > 1)
								vSize.z /= 2;

							// Update the total number of mipmap levels
							m_nNumOfMipmaps++;

							// Update the total number of bytes this texture buffer requires
							const uint32 nNumOfBytes = GetNumOfBytes(nLevel);
							m_nTotalNumOfBytes += nNumOfBytes;

							// Allocate memory for your white buffer and set it to "white" (we only do this once for the larges one)
							if (!pszBuffer) {
								pszBuffer = new char[nNumOfBytes];
								MemoryManager::Set(pszBuffer, 255, nNumOfBytes);
							}

							// Upload the texture buffer
							glTexImage3DEXT(GL_TEXTURE_3D_EXT, nLevel, *pAPIPixelFormat, vSize.x, vSize.y, vSize.z, 0, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pszBuffer);

							// Update the mipmap level counter
							nLevel++;
						} while (vSize.x != 1 || vSize.y != 1 || vSize.z != 1);

						// Cleanup your white buffer
						if (pszBuffer)
							delete [] pszBuffer;
					}
				}

				// Update renderer statistics
				cRendererOpenGL.GetWritableStatistics().nTextureBuffersMem += GetTotalNumOfBytes();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::TextureBuffer functions    ]
//[-------------------------------------------------------]
bool TextureBuffer3D::CopyDataFrom(uint32 nMipmap, EPixelFormat nFormat, const void *pData, uint8 nFace)
{
	// Check parameters
	if (nMipmap <= m_nNumOfMipmaps && nFormat != Unknown && pData && !nFace) {
		// Get the OpenGL renderer instance
		const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

		// Get API pixel format
		const uint32 *pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
		if (pAPIPixelFormat) {
			// Get the size of this mipmap level
			const Vector3i vSize = GetSize(nMipmap);

			// Compressed format?
			if (IsCompressedFormat(nFormat)) {
				// Internal format MUST be the same as the given format
				if (m_nFormat != nFormat)
					return false; // Error!

				// Bind this texture buffer
				glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

				// Get the number of bytes
				const uint32 nNumOfBytes = GetNumOfBytes(nMipmap);

				// Upload
				CompressedTexImage3D(cRendererOpenGL, GL_TEXTURE_3D_EXT, nMipmap, *pAPIPixelFormat, vSize, nNumOfBytes, static_cast<const uint8*>(pData));
			} else {
				// Bind this texture buffer
				glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

				// Upload
				const uint32 nPixelFormat = cRendererOpenGL.GetOpenGLPixelFormat(nFormat);
				const uint32 nDataFormat  = cRendererOpenGL.GetOpenGLDataFormat(nFormat);
				glTexImage3DEXT(GL_TEXTURE_3D_EXT, nMipmap, *pAPIPixelFormat, vSize.x, vSize.y, vSize.z, 0, nPixelFormat, nDataFormat, pData);
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool TextureBuffer3D::CopyDataTo(uint32 nMipmap, EPixelFormat nFormat, void *pData, uint8 nFace) const
{
	// Check parameters
	if (nMipmap > m_nNumOfMipmaps || nFormat == Unknown || !pData || nFace)
		return false; // Error!

	// Compressed format?
	if (IsCompressedFormat(nFormat)) {
		// Internal format MUST be the same as the given format
		if (m_nFormat != nFormat)
			return false; // Error!

		// Bind this texture buffer
		glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

		// Download
		GetCompressedTexImage(static_cast<Renderer&>(GetRenderer()), GL_TEXTURE_3D_EXT, nMipmap, nFormat, GetSize(nMipmap), static_cast<uint8*>(pData));
	} else {
		// Bind this texture buffer
		glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

		// Get the OpenGL renderer instance
		const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

		// Download
		const uint32 nPixelFormat = cRendererOpenGL.GetOpenGLPixelFormat(nFormat);
		const uint32 nDataFormat  = cRendererOpenGL.GetOpenGLDataFormat(nFormat);
		glGetTexImage(GL_TEXTURE_3D_EXT, nMipmap, nPixelFormat, nDataFormat, pData);
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::TextureBuffer functions   ]
//[-------------------------------------------------------]
bool TextureBuffer3D::MakeCurrent(uint32 nStage)
{
	// Check if there are renderer information
	if (GetRenderer().GetTextureBuffer(nStage) != this) {
		if (!GetRenderer().SetTextureBuffer(nStage, this))
			return true; // Done
	}

	// Make this texture buffer to the renderers current one
	glEnable(GL_TEXTURE_3D_EXT);
	glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void TextureBuffer3D::BackupDeviceData(uint8 **ppBackup)
{
	// Allocate memory
	const uint32 nTotalNumOfBytes = GetTotalNumOfBytes();
	if (nTotalNumOfBytes) {
		*ppBackup = new uint8[nTotalNumOfBytes];
		if (*ppBackup) {
			// Prepare for backup
			glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

			// Backup
			uint8 *pData = *ppBackup;
			if (IsCompressedFormat()) {
				// Loop through all mipmaps
				for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++) {
					// Get the data from the GPU
					// -> When the "GL_NV_texture_compression_vtc"-extension is there, the data blocks have not the same
					//    order "as usual" but this is not interesting in here because we're just caching it in here for the GPU
					glGetCompressedTexImageARB(GL_TEXTURE_3D_EXT, nLevel, pData);

					// Next level, please
					pData += GetNumOfBytes(nLevel);
				}
			} else {
				// Get the OpenGL renderer instance
				const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

				// Get format information
				const uint32 nPixelFormat = cRendererOpenGL.GetOpenGLPixelFormat(m_nFormat);
				const uint32 nDataFormat  = cRendererOpenGL.GetOpenGLDataFormat(m_nFormat);

				// Loop through all mipmaps
				for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++) {
					// Get the data from the GPU
					glGetTexImage(GL_TEXTURE_3D_EXT, nLevel, nPixelFormat, nDataFormat, pData);

					// Next level, please
					pData += GetNumOfBytes(nLevel);
				}
			}

			// Destroy this OpenGL texture buffer
			if (m_nOpenGLTexture) {
				glDeleteTextures(1, &m_nOpenGLTexture);
				m_nOpenGLTexture = 0;
			}
		}
	} else {
		*ppBackup = nullptr;
	}
}

void TextureBuffer3D::RestoreDeviceData(uint8 **ppBackup)
{
	// Restore data
	if (*ppBackup) {
		glGenTextures(1, &m_nOpenGLTexture);
		glBindTexture(GL_TEXTURE_3D_EXT, m_nOpenGLTexture);

		// Get the OpenGL renderer instance
		const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

		// Get API pixel format
		const uint32 nPixelFormat = cRendererOpenGL.GetOpenGLPixelFormat(m_nFormat);
		const uint32 nDataFormat  = cRendererOpenGL.GetOpenGLDataFormat(m_nFormat);
		const uint32 *pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
		if (pAPIPixelFormat) {
			// Is this a compressed texture buffer pixel format?
			const bool bCompressedFormat = IsCompressedFormat();

			// Data pointer
			const uint8 *pData = *ppBackup;

			// Loop through all mipmaps
			for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++) {
				// Get the size of this mipmap level
				const Vector3i vSize = GetSize(nLevel);

				// Get the number of bytes
				const uint32 nNumOfBytes = GetNumOfBytes(nLevel);

				// Upload the texture buffer
				if (bCompressedFormat) {
					// Use directly "glCompressedTexImage3DARB()" instead of our helper method "CompressedTexImage3D()"
					// -> When the "GL_NV_texture_compression_vtc"-extension is there, the data blocks have not the same
					//    order "as usual" but this is not interesting in here because we're just caching it in here for the GPU
					glCompressedTexImage3DARB(GL_TEXTURE_3D_EXT, nLevel, *pAPIPixelFormat, vSize.x, vSize.y, vSize.z, 0, nNumOfBytes, pData);
				} else {
					glTexImage3DEXT(GL_TEXTURE_3D_EXT, nLevel, *pAPIPixelFormat, vSize.x, vSize.y, vSize.z, 0, nPixelFormat, nDataFormat, pData);
				}

				// Next level, please
				pData += nNumOfBytes;
			}
		}

		// Delete memory
		delete [] *ppBackup;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
