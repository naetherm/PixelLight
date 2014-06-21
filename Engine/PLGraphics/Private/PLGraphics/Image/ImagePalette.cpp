/*********************************************************\
 *  File: ImagePalette.cpp                               *
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
#include "PLGraphics/Image/ImagePalette.h"


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
ImagePalette::ImagePalette(const ImagePalette &cSource) :
	m_pData(nullptr),
	m_nSize(0),
	m_nColors(0)
{
	// Copy palette
	if (cSource.m_nSize > 0 && cSource.m_pData) {
		// Create buffer
		m_nSize   = cSource.m_nSize;
		m_nColors = cSource.m_nColors;
		m_pData   = new uint8[m_nSize*3];

		// Copy data
		MemoryManager::Copy(m_pData, cSource.m_pData, m_nSize*3);
	}
}

/**
*  @brief
*    Assignment operator
*/
ImagePalette &ImagePalette::operator =(const ImagePalette &cSource)
{
	// Clear data
	Clear();

	// Copy palette
	if (cSource.m_nSize > 0 && cSource.m_pData) {
		// Create buffer
		m_nSize   = cSource.m_nSize;
		m_nColors = cSource.m_nColors;
		m_pData   = new uint8[m_nSize*3];

		// Copy data
		MemoryManager::Copy(m_pData, cSource.m_pData, m_nSize*3);
	}

	// Return this
	return *this;
}

/**
*  @brief
*    Create palette
*/
void ImagePalette::Create(uint32 nColors)
{
	// Clear palette
	Clear();

	// Check if size is valid
	if (nColors > 0) {
		// Get size as nearest multiple of 256
		m_nSize = nColors / 256;
		if (nColors % 256 > 0)
			m_nSize ++;
		m_nSize *= 256;

		// Create palette data
		m_pData   = new uint8[m_nSize*3];
		m_nColors = nColors;
		MemoryManager::Set(m_pData, 0, m_nSize*3);
	}
}

/**
*  @brief
*    Get index of color
*/
int ImagePalette::GetColorIndex(const Color3 &cColor) const
{
	// Do we need to build the color index?
	if (m_mapColors.GetNumOfElements() == 0) {
		// Build color index
		for (uint32 i=0; i<m_nColors; i++) {
			// Map color to index
			const uint32 nColorInt = cColor.ToUInt32();
			m_mapColors.Add(nColorInt, i);
		}
	}

	// Look up color index
	const uint32 nColorInt = cColor.ToUInt32();
	int nIndex = m_mapColors.Get(nColorInt);

	// If nIndex is 0, it can be either invalid or really index 0, so check if the colors are equal
	if (nIndex == 0) {
		// Check if color is valid
		const Color3 cColorLookup = GetColor(0);
		if (cColorLookup != cColor || cColorLookup == Color3::Null) {
			// Invalid index
			nIndex = -1;
		}
	}

	// Return color index
	return nIndex;
}

/**
*  @brief
*    Set color
*/
void ImagePalette::SetColor(uint32 nIndex, const Color3 &cColor)
{
	// Do we have to resize the palette?
	if (nIndex >= m_nColors)
		Resize(nIndex);

	// Set color
	if (m_pData) {
		m_pData[nIndex*3+0] = cColor.GetRInt();
		m_pData[nIndex*3+1] = cColor.GetGInt();
		m_pData[nIndex*3+2] = cColor.GetBInt();
	}

	// Rebuild color index
	RebuildColorIndex();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Resize palette
*/
void ImagePalette::Resize(uint32 nColors)
{
	// Check if size is valid
	if (nColors > 0) {
		// Get size as nearest multiple of 256
		uint32 nSize = nColors / 256;
		if (nColors % 256 > 0)
			nSize++;
		nSize *= 256;

		// Resize if new size is bigger than old size
		if (nSize > m_nSize) {
			// Create new palette
			uint8 *pData = new uint8[nSize*3];
			MemoryManager::Set(pData, 0, nSize*3);

			// Copy old palette
			if (m_pData != nullptr) {
				// Copy old palette data
				if (m_nSize > 0)
					MemoryManager::Copy(pData, m_pData, m_nSize*3);

				// Destroy old palette data
				delete [] m_pData;
			}

			// Set new data
			m_pData = pData;
			m_nSize = nSize;
		}

		// Set new number of colors
		m_nColors = nColors;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
