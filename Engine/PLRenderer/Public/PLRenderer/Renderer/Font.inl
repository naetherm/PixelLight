/*********************************************************\
 *  File: Font.inl                                       *
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
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the creating font manager
*/
inline FontManager &Font::GetFontManager() const
{
	return *m_pFontManager;
}

/**
*  @brief
*    Returns the filename of the font file the data came from
*/
inline PLCore::String Font::GetFilename() const
{
	return m_sFilename;
}

/**
*  @brief
*    Returns the font size in points (72 points per inch)
*/
inline PLCore::uint32 Font::GetSize() const
{
	return m_nSize;
}

/**
*  @brief
*    Returns the horizontal and vertical resolution in DPI
*/
inline PLCore::uint32 Font::GetResolution() const
{
	return m_nResolution;
}

/**
*  @brief
*    Returns the font size in pixels
*/
inline PLCore::uint32 Font::GetSizeInPixels() const
{
	return static_cast<PLCore::uint32>(m_nSize/72.0f*m_nResolution);
}

/**
*  @brief
*    Returns the font height in pixels
*/
inline PLCore::uint32 Font::GetHeightInPixels() const
{
	return static_cast<PLCore::uint32>(GetHeight()/72.0f*m_nResolution);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
