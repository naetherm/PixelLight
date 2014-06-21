/*********************************************************\
 *  File: FontGlyphTexture.h                             *
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


#ifndef __PLRENDEREROPENGL_FONTGLYPHTEXTURE_H__
#define __PLRENDEREROPENGL_FONTGLYPHTEXTURE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/FontGlyphTexture.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FontTexture;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL renderer font glyph texture
*/
class FontGlyphTexture : public PLRenderer::FontGlyphTexture {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFontTexture
		*    Owner texture font
		*  @param[in] nCharacterCode
		*    Character code
		*  @param[in] vPosition
		*    Position offset of the glyph inside the font texture atlas in pixel
		*
		*  @note
		*    - The font texture atlas must be the current active and bound OpenGL texture
		*/
		FontGlyphTexture(FontTexture &cFontTexture, unsigned long nCharacterCode, const PLMath::Vector2i &vPosition);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FontGlyphTexture();

		/**
		*  @brief
		*    Returns the normalized minimum glyph texture coordinate inside the glyph texture atlas
		*
		*  @return
		*    The normalized minimum glyph texture coordinate inside the glyph texture atlas
		*/
		const PLMath::Vector2 &GetTexCoordMin() const;

		/**
		*  @brief
		*    Returns the normalized maximum glyph texture coordinate inside the glyph texture atlas
		*
		*  @return
		*    The normalized maximum glyph texture coordinate inside the glyph texture atlas
		*/
		const PLMath::Vector2 &GetTexCoordMax() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector2 m_vTexCoordMin;	/**< Normalized minimum glyph texture coordinate inside the glyph texture atlas */
		PLMath::Vector2 m_vTexCoordMax;	/**< Normalized maximum glyph texture coordinate inside the glyph texture atlas */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_FONTGLYPHTEXTURE_H__
