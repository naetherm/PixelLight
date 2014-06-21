/*********************************************************\
 *  File: ImagePalette.inl                               *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline ImagePalette::ImagePalette() :
	m_pData(nullptr),
	m_nSize(0),
	m_nColors(0)
{
}

/**
*  @brief
*    Destructor
*/
inline ImagePalette::~ImagePalette()
{
	// Clear data
	Clear();
}

/**
*  @brief
*    Clear data
*/
inline void ImagePalette::Clear()
{
	// Delete palette
	if (m_pData)
		delete [] m_pData;

	// Reset data
	m_pData   = nullptr;
	m_nSize   = 0;
	m_nColors = 0;
	m_mapColors.Clear();
}

/**
*  @brief
*    Get number of colors
*/
inline PLCore::uint32 ImagePalette::GetNumOfColors() const
{
	// Return number of colors
	return m_nColors;
}

/**
*  @brief
*    Get color
*/
inline Color3 ImagePalette::GetColor(PLCore::uint32 nIndex) const
{
	// Is index valid?
	if (nIndex < m_nColors)
		return Color3(m_pData[nIndex*3], m_pData[nIndex*3+1], m_pData[nIndex*3+2]);

	// Invalid index
	return Color3::Null;
}

/**
*  @brief
*    Add color
*/
inline PLCore::uint32 ImagePalette::AddColor(const Color3 &cColor)
{
	// Add color to next free place in palette
	const PLCore::uint32 nIndex = m_nColors;
	SetColor(nIndex, cColor);

	// Return color index
	return nIndex;
}

/**
*  @brief
*    Rebuild color index
*/
inline void ImagePalette::RebuildColorIndex()
{
	// Clear hash map. It will be rebuilt next time GetColorIndex() is called
	m_mapColors.Clear();
}

/**
*  @brief
*    Get color palette data
*/
inline PLCore::uint8 *ImagePalette::GetData() const
{
	// Return palette data
	return m_pData;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
