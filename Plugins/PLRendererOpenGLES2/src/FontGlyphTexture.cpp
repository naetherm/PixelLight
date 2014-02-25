/*********************************************************\
 *  File: FontGlyphTexture.cpp                           *
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
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <PLMath/Math.h>
#include "PLRendererOpenGLES2/FontTexture.h"
#include "PLRendererOpenGLES2/FontGlyphTexture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRendererOpenGLES2 {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FontGlyphTexture::FontGlyphTexture(FontTexture &cFontTexture, unsigned long nCharacterCode, const Vector2i &vPosition)
{
	// Get the FreeType library face object (aka "The Font")
	FT_Face *pFTFace = cFontTexture.GetFTFace();
	if (pFTFace) {
		// Load the Glyph for the character
		if (!FT_Load_Glyph(*pFTFace, FT_Get_Char_Index(*pFTFace, nCharacterCode), FT_LOAD_DEFAULT)) {
			// Move the glyph within the glyph slot of the face object into a glyph object
			FT_Glyph sFTGlyph;
			if (!FT_Get_Glyph((*pFTFace)->glyph, &sFTGlyph)) {
				// Convert the glyph to a bitmap
				FT_Glyph_To_Bitmap(&sFTGlyph, ft_render_mode_normal, 0, 1);
				FT_BitmapGlyph sFTBitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(sFTGlyph);

				// Get a reference to the glyph bitmap
				FT_Bitmap &sFTBitmap = sFTBitmapGlyph->bitmap;

				// Get the size (in pixel) of the glyph
				m_nSize.Set(sFTBitmap.width, sFTBitmap.rows);

				// Copy the glyph bitmap into the glyph texture atlas (this OpenGL texture must be currently bound!)
				if (m_nSize != Vector2i::Zero)
					glTexSubImage2D(GL_TEXTURE_2D, 0, vPosition.x, vPosition.y, m_nSize.x, m_nSize.y, GL_ALPHA, GL_UNSIGNED_BYTE, sFTBitmap.buffer);

				// Set the distance (in pixel) from the current pen position to the glyph bitmap
				m_vCorner.SetXY(static_cast<float>(sFTBitmapGlyph->left), static_cast<float>(sFTBitmapGlyph->top) - m_nSize.y);

				// Set the pen advance, the FreeType library measures font size in terms of 1/64ths of pixels, so we have to adjust with /64
				m_vPenAdvance.x = static_cast<float>((*pFTFace)->glyph->advance.x)/64.0f;
				m_vPenAdvance.y = static_cast<float>((*pFTFace)->glyph->advance.y)/64.0f;

				// Get the size of the glyph texture atlas
				const Vector2i &vGlyphTextureAtlasSize = cFontTexture.GetGlyphTextureAtlasSize();

				// Calculate the normalized minimum glyph texture coordinate inside the glyph texture atlas
				m_vTexCoordMin.x = static_cast<float>(vPosition.x)/static_cast<float>(vGlyphTextureAtlasSize.x);
				m_vTexCoordMin.y = static_cast<float>(vPosition.y)/static_cast<float>(vGlyphTextureAtlasSize.y);

				// Calculate the normalized maximum glyph texture coordinate inside the glyph texture atlas
				m_vTexCoordMax.x = static_cast<float>(vPosition.x + m_nSize.x)/static_cast<float>(vGlyphTextureAtlasSize.x);
				m_vTexCoordMax.y = static_cast<float>(vPosition.y + m_nSize.y)/static_cast<float>(vGlyphTextureAtlasSize.y);

				// Release the created FreeType glyph
				FT_Done_Glyph(sFTGlyph);
			}
		}
	}
}

/**
*  @brief
*    Destructor
*/
FontGlyphTexture::~FontGlyphTexture()
{
}

/**
*  @brief
*    Returns the normalized minimum glyph texture coordinate inside the glyph texture atlas
*/
const Vector2 &FontGlyphTexture::GetTexCoordMin() const
{
	return m_vTexCoordMin;
}

/**
*  @brief
*    Returns the normalized maximum glyph texture coordinate inside the glyph texture atlas
*/
const Vector2 &FontGlyphTexture::GetTexCoordMax() const
{
	return m_vTexCoordMax;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLES2
