/*********************************************************\
 *  File: Image.cpp                                      *
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
#include <PLCore/Core/MemoryManager.h>
#include "PLGraphics/Image/ImageEffect.h"
#include "PLGraphics/Image/Image.h"
#include "PLGraphics/Image/ImagePart.h"
#include "PLGraphics/Image/ImageBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create image
*/
Image Image::CreateImage(EDataFormat nDataFormat, EColorFormat nColorFormat, const Vector3i &vSize, ECompression nCompression)
{
	// Create and allocate the image instance
	Image cImage;
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(nDataFormat, nColorFormat, vSize, nCompression);

	// Return the created image
	return cImage;
}

/**
*  @brief
*    Create image and copy given uncompressed image data
*/
Image Image::CreateImageAndCopyData(EDataFormat nDataFormat, EColorFormat nColorFormat, const Vector3i &vSize, ECompression nCompression, const uint8 *pnData)
{
	// Create and allocate the image instance
	Image cImage;
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(nDataFormat, nColorFormat, vSize, nCompression);

	// Copy the provided data
	pImageBuffer->CopyData(pnData);

	// Return the created image
	return cImage;
}

/**
*  @brief
*    Create image and takeover given uncompressed image data
*/
Image Image::CreateImageAndTakeoverData(EDataFormat nDataFormat, EColorFormat nColorFormat, const Vector3i &vSize, ECompression nCompression, uint8 *pnData)
{
	// Create and allocate the image instance
	Image cImage;
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(nDataFormat, nColorFormat, vSize, nCompression);

	// Takeover the provided data
	pImageBuffer->TakeoverData(pnData);

	// Return the created image
	return cImage;
}

/**
*  @brief
*    Create image and share given uncompressed image data
*/
Image Image::CreateImageAndShareData(EDataFormat nDataFormat, EColorFormat nColorFormat, const Vector3i &vSize, ECompression nCompression, uint8 *pnData)
{
	// Create and allocate the image instance
	Image cImage;
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(nDataFormat, nColorFormat, vSize, nCompression);

	// Share the provided data
	pImageBuffer->ShareData(pnData);

	// Return the created image
	return cImage;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Image::Image()
{
}

/**
*  @brief
*    Copy constructor
*/
Image::Image(const Image &cSource)
{
	// Copy parts
	for (uint32 i=0; i<cSource.m_lstParts.GetNumOfElements(); i++) {
		// Create copy of part
		ImagePart *pImagePart = new ImagePart(*cSource.m_lstParts[i]);
		m_lstParts.Add(pImagePart);
		m_mapParts.Add(pImagePart->GetSemantics(), pImagePart);
	}
}

/**
*  @brief
*    Destructor
*/
Image::~Image()
{
	// Clear data
	Unload();
}

/**
*  @brief
*    Assignment operator
*/
Image &Image::operator =(const Image &cSource)
{
	// Clear data
	Unload();

	// Copy parts
	for (uint32 i=0; i<cSource.m_lstParts.GetNumOfElements(); i++) {
		// Create copy of part
		ImagePart *pImagePart = new ImagePart(*cSource.m_lstParts[i]);
		m_lstParts.Add(pImagePart);
		m_mapParts.Add(pImagePart->GetSemantics(), pImagePart);
	}

	// Copy filename and URL from the Loadable class
	m_sFilename = cSource.m_sFilename;
	m_sUrl      = cSource.m_sUrl;

	// Return this
	return *this;
}

/**
*  @brief
*    Apply image effect
*/
void Image::ApplyEffect(const ImageEffect &cEffect)
{
	cEffect.Apply(*this);
}

/**
*  @brief
*    Check image for possible problems
*/
ECheckConsistency Image::CheckConsistency() const
{
	// Check if image has at least one part
	if (m_lstParts.GetNumOfElements() == 0) {
		// Image has no parts
		return CheckImageEmpty;
	}

	// Check if a cubemap is defined
	bool bCubemap    = false;
	bool bSideOther  = false;
	bool bSideDouble = false;
	bool bSide[6]    = {false, false, false, false, false, false};
	for (uint32 i=0; i<m_lstParts.GetNumOfElements(); i++) {
		// Check cubemap side
		uint32 nSemantics = m_lstParts[i]->GetSemantics();
		switch (nSemantics) {
			case ImagePartCubeSidePosX:
				if (bSide[0])
					bSideDouble = true;
				bSide[0] = true;
				break;

			case ImagePartCubeSideNegX:
				if (bSide[1])
					bSideDouble = true;
				bSide[1] = true;
				break;

			case ImagePartCubeSidePosY:
				if (bSide[2])
					bSideDouble = true;
				bSide[2] = true;
				break;

			case ImagePartCubeSideNegY:
				if (bSide[3])
					bSideDouble = true;
				bSide[3] = true;
				break;

			case ImagePartCubeSidePosZ:
				if (bSide[4])
					bSideDouble = true;
				bSide[4] = true;
				break;

			case ImagePartCubeSideNegZ:
				if (bSide[5])
					bSideDouble = true;
				bSide[5] = true;
				break;

			default:
				bSideOther = true;
				break;
		}
	}
	bCubemap = (bSide[0] || bSide[1] || bSide[2] || bSide[3] || bSide[4] || bSide[5]);

	// Check cubemap consistency
	// [TODO] Are all cubemap sides of the same size?
	if (bCubemap) {
		// A cubemap side is missing
		if (!bSide[0] || !bSide[1] || !bSide[2] || !bSide[3] || !bSide[4] || !bSide[5])
			return CheckCubemapSideMissing;

		// Part found that is not a cubemap side
		if (bSideOther)
			return CheckCubemapSideInvalid;

		// Two parts found that have the same cubemap side ID
		if (bSideDouble)
			return CheckCubemapSideDouble;
	}

	// Check parts consistency
	for (uint32 i=0; i<m_lstParts.GetNumOfElements(); i++) {
		// Check consistency of ImageFrame
		const ECheckConsistency nCheck = m_lstParts[i]->CheckConsistency();
		if (nCheck != CheckOk)
			return nCheck;
	}

	// No problem found
	return CheckOk;
}

/**
*  @brief
*    Create a new image part
*/
ImagePart *Image::CreatePart(uint32 nSemantics)
{
	// Create if semantics ID is already used
	if (m_mapParts.Get(nSemantics) == nullptr) {
		// Create image part
		ImagePart *pPart = new ImagePart();
		pPart->SetSemantics(nSemantics);

		// Add image part to list
		m_lstParts.Add(pPart);
		m_mapParts.Add(nSemantics, pPart);

		// Return image part
		return pPart;
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Delete image part
*/
bool Image::DeletePart(ImagePart &cPart)
{
	// Check part
	ImagePart *pPart = &cPart;
	if (m_lstParts.IsElement(pPart)) {
		// Remove and delete part
		m_lstParts.Remove(pPart);
		m_mapParts.Remove(pPart->GetSemantics());
		delete pPart;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Get image buffer
*/
ImageBuffer *Image::GetBuffer(uint32 nPart, uint32 nMipmap) const
{
	// Get image part
	const ImagePart *pPart = GetPart(nPart);
	if (pPart) {
		// Get mipmap
		ImageBuffer *pBuffer = pPart->GetMipmap(nMipmap);
		if (pBuffer) {
			// Return buffer
			return pBuffer;
		}
	}

	// Invalid buffer requested
	return nullptr;
}

/**
*  @brief
*    Get size of image data in bytes, including all parts and mipmaps
*/
uint32 Image::GetDataSize() const
{
	// Sum up the data size of all mipmaps
	uint32 nDataSize = 0;
	for (uint32 i=0; i<m_lstParts.GetNumOfElements(); i++)
		nDataSize += m_lstParts[i]->GetDataSize();

	// Done
	return nDataSize;
}

/**
*  @brief
*    Get size of compressed image data in bytes, including all parts and mipmaps
*/
uint32 Image::GetCompressedDataSize() const
{
	// Sum up the compressed data size of all mipmaps
	uint32 nCompressedDataSize = 0;
	for (uint32 i=0; i<m_lstParts.GetNumOfElements(); i++)
		nCompressedDataSize += m_lstParts[i]->GetCompressedDataSize();

	// Done
	return nCompressedDataSize;
}

/**
*  @brief
*    Create a test image
*/
void Image::CreateTestImage(ETestImage nTestImage)
{
	// Clear data
	Unload();

	// Get test image ID
	switch (nTestImage) {
		// 2D test images
		case TestImage2DSimple:
		case TestImage2DGradient:
		case TestImage2DCheckers:
			CreatePart()->CreateMipmap()->CreateTestImage(nTestImage);
			break;

		// Cube map
		case TestImageCubeMap:
			// [TODO] implement
			break;
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Image::Unload()
{
	// Destroy parts
	for (uint32 i=0; i<m_lstParts.GetNumOfElements(); i++)
		delete m_lstParts[i];

	// Clear frame list
	m_lstParts.Clear();
	m_mapParts.Clear();

	// Call base implementation
	return Loadable::Unload();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
String Image::GetLoadableTypeName() const
{
	static const String sString = "Image";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
