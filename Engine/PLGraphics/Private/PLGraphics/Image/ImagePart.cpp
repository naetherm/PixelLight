/*********************************************************\
 *  File: ImagePart.cpp                                  *
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
#include "PLGraphics/Image/ImageEffect.h"
#include "PLGraphics/Image/ImageBuffer.h"
#include "PLGraphics/Image/ImagePart.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
ImagePart::ImagePart(const ImagePart &cSource) :
	m_nSemantics(cSource.m_nSemantics)
{
	// Copy mipmaps
	for (uint32 i=0; i<cSource.m_lstMipmaps.GetNumOfElements(); i++) {
		// Create copy of mipmap
		ImageBuffer *pImageBuffer = new ImageBuffer(*cSource.m_lstMipmaps[i]);
		m_lstMipmaps.Add(pImageBuffer);
	}
}

/**
*  @brief
*    Assignment operator
*/
ImagePart &ImagePart::operator =(const ImagePart &cSource)
{
	// Clear data
	Clear();

	// Copy data
	m_nSemantics = cSource.m_nSemantics;

	// Copy mipmaps
	for (uint32 i=0; i<cSource.m_lstMipmaps.GetNumOfElements(); i++) {
		// Create copy of mipmap
		ImageBuffer *pImageBuffer = new ImageBuffer(*cSource.m_lstMipmaps[i]);
		m_lstMipmaps.Add(pImageBuffer);
	}

	// Return this
	return *this;
}

/**
*  @brief
*    Clear data
*/
void ImagePart::Clear()
{
	// Destroy mipmaps
	for (uint32 i=0; i<m_lstMipmaps.GetNumOfElements(); i++) {
		delete m_lstMipmaps[i];
	}

	// Clear mipmap list
	m_lstMipmaps.Clear();
}

/**
*  @brief
*    Apply image effect
*/
void ImagePart::ApplyEffect(const ImageEffect &cEffect)
{
	cEffect.Apply(*this);
}

/**
*  @brief
*    Check image for possible problems
*/
ECheckConsistency ImagePart::CheckConsistency() const
{
	// Check if part has at least one mipmap
	if (m_lstMipmaps.GetNumOfElements() == 0) {
		// Part has no image buffers
		return CheckPartEmpty;
	}

	// Check mipmap consistency
	// [TODO] Is every mipmap smaller than the last one?	-> CheckMipmapInconsistent
	// [TODO] Is every mipmap power-of-two?					-> CheckMipmapsNotPowerOfTwo
	// [TODO] Are all mipmaps down to 1x1 defined?			-> CheckMipmapsNot1x1
	const EDataFormat  nDataFormat  = m_lstMipmaps[0]->GetDataFormat();
	const EColorFormat nColorFormat = m_lstMipmaps[0]->GetColorFormat();
	for (uint32 i=0; i<m_lstMipmaps.GetNumOfElements(); i++) {
		// Check consistency of ImageBuffer
		const ECheckConsistency nCheck = m_lstMipmaps[i]->CheckConsistency();
		if (nCheck != CheckOk)
			return nCheck;

		// Check data format
		if (m_lstMipmaps[i]->GetDataFormat() != nDataFormat) {
			// Inconsistent data format
			return CheckInconsistentDataFormat;
		}

		// Check color format
		if (m_lstMipmaps[i]->GetColorFormat() != nColorFormat) {
			// Inconsistent color format
			return CheckInconsistentColorFormat;
		}
	}

	// No problem found
	return CheckOk;
}

/**
*  @brief
*    Build mipmaps
*/
bool ImagePart::BuildMipmaps()
{
	// Destroy all mipmaps - besides the first one (our base map)
	DeleteMipmaps();

	// [TODO] Implement
	return false;
}

/**
*  @brief
*    Create new mipmap
*/
ImageBuffer *ImagePart::CreateMipmap()
{
	// Create mipmap
	ImageBuffer *pBuffer = new ImageBuffer();

	// Add mipmap to list
	m_lstMipmaps.Add(pBuffer);

	// Return mipmap
	return pBuffer;
}

/**
*  @brief
*    Delete mipmap
*/
bool ImagePart::DeleteMipmap(ImageBuffer &cImageBuffer)
{
	// Remove and delete mipmap
	if (m_lstMipmaps.Remove(&cImageBuffer)) {
		delete &cImageBuffer;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Deletes all mipmaps besides the first one (the base map)
*/
void ImagePart::DeleteMipmaps()
{
	// Are there any mipmaps?
	if (m_lstMipmaps.GetNumOfElements()) {
		for (uint32 i=1; i<m_lstMipmaps.GetNumOfElements(); i++)
			delete m_lstMipmaps[i];
		m_lstMipmaps.Resize(1);
	}
}

/**
*  @brief
*    Get size of image data in bytes, including all mipmaps
*/
uint32 ImagePart::GetDataSize() const
{
	// Sum up the data size of all mipmaps
	uint32 nDataSize = 0;
	for (uint32 i=0; i<m_lstMipmaps.GetNumOfElements(); i++)
		nDataSize += m_lstMipmaps[i]->GetDataSize();

	// Done
	return nDataSize;
}

/**
*  @brief
*    Get size of compressed image data in bytes, including all mipmaps
*/
uint32 ImagePart::GetCompressedDataSize() const
{
	// Sum up the compressed data size of all mipmaps
	uint32 nCompressedDataSize = 0;
	for (uint32 i=0; i<m_lstMipmaps.GetNumOfElements(); i++)
		nCompressedDataSize += m_lstMipmaps[i]->GetCompressedDataSize();

	// Done
	return nCompressedDataSize;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
