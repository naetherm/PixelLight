/*********************************************************\
 *  File: Font.h                                         *
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


#ifndef __PLRENDERER_FONT_H__
#define __PLRENDERER_FONT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLMath/Vector2.h>
#include <PLMath/Vector2i.h>
#include "PLRenderer/Renderer/Resource.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Matrix4x4;
}
namespace PLGraphics {
	class Color4;
}
namespace PLRenderer {
	class FontGlyph;
	class FontManager;
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
*    Abstract renderer font resource base class
*
*  @remarks
*
*    Height       Ascender    Internal leading
*      |            |         |
*      |   |    |   |             |
*      |   |    |   |             |
*      |   |----|   |             |
*      |   |    |   |             |
*    0 |___|____|___|_____________|___ Baseline 
*      |0               |         |
*      |                |         |
*                   Descender    Size
*
*    Term definitions:
*    - Height = Ascender + Descender
*    - Size   = Height - Internal leading
*
*  @note
*    - For italic/bold fonts, you have to specify the corresponding font variation file of the font file
*/
class Font : public Resource {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  brief
		*    Draw flags
		*/
		enum EDrawFlags {
			CenterText    = 1<<0,	/**< Center the text */
			UnderlineText = 1<<1,	/**< Underline the text */
			CrossoutText  = 1<<2,	/**< Cross out the text */
			Mipmapping    = 1<<3	/**< Use mipmapping (may blur the font in an ugly way) */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Font();

		/**
		*  @brief
		*    Returns the creating font manager
		*
		*  @return
		*    The creating font manager
		*/
		inline FontManager &GetFontManager() const;

		/**
		*  @brief
		*    Returns the filename of the font file the data came from
		*
		*  @return
		*    The filename of the font file the data came from
		*/
		inline PLCore::String GetFilename() const;

		/**
		*  @brief
		*    Returns the nominal font size in points (72 points per inch)
		*
		*  @return
		*    The nominal font size in points, for example 12
		*/
		inline PLCore::uint32 GetSize() const;

		/**
		*  @brief
		*    Returns the horizontal and vertical resolution in DPI
		*
		*  @return
		*    The horizontal and vertical resolution in DPI, for example 96
		*/
		inline PLCore::uint32 GetResolution() const;

		/**
		*  @brief
		*    Returns the font size in pixels
		*
		*  @return
		*    The font size in pixels
		*
		*  @remarks
		*    Example: Arial 12pt at 96 dpi:
		*
		*                          Font size in points             12
		*    Font size in pixels = ------------------- × 96 dpi = ---- × 96 = 16 pixels
		*                          72 points per inch              72
		*/
		inline PLCore::uint32 GetSizeInPixels() const;

		/**
		*  @brief
		*    Returns the font (line) height in pixels
		*
		*  @return
		*    The font height in pixels
		*
		*  @see
		*    - GetSizeInPixels()
		*/
		inline PLCore::uint32 GetHeightInPixels() const;


	//[-------------------------------------------------------]
	//[ Public virtual Font functions                         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets the font size
		*
		*  @param[in] nSize
		*    Nominal font size in points, for example 12 (72 points per inch)
		*  @param[in] nResolution
		*    The horizontal and vertical resolution in DPI, for example 96
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - When overwriting this method, it's highly recommended to call this base implementation, too
		*/
		PLRENDERER_API virtual bool SetSize(PLCore::uint32 nSize = 12, PLCore::uint32 nResolution = 96);

		/**
		*  @brief
		*    Returns whether or not this font is valid
		*
		*  @return
		*    'true' if the font is valid, else 'false'
		*/
		virtual bool IsValid() const = 0;

		/**
		*  @brief
		*    Returns the ascender in font units
		*
		*  @return
		*    The ascender in font units
		*/
		virtual float GetAscender() const = 0;

		/**
		*  @brief
		*    Returns the descender in font units
		*
		*  @return
		*    The descender in font units (negative value)
		*/
		virtual float GetDescender() const = 0;

		/**
		*  @brief
		*    Returns the height in font units
		*
		*  @return
		*    The height in font units
		*/
		virtual float GetHeight() const = 0;

		/**
		*  @brief
		*    Returns the width of a text (in font units) drawn with this font
		*
		*  @param[in] sText
		*    Text to 'draw'
		*
		*  @return
		*    The width of a text (in font units) drawn with this font
		*
		*  @note
		*    - When overwriting this method, it's highly recommended to call this base implementation, too
		*/
		PLRENDERER_API virtual float GetTextWidth(const PLCore::String &sText);

		/**
		*  @brief
		*    Draws a text
		*
		*  @param[in] sText
		*    Text to draw
		*  @param[in] cColor
		*    Text color
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] vScale
		*    Font scale, there are situations were it's more comfortable to have a special scale variable as just mObjectSpaceToClipSpace with all build in
		*  @param[in] vBias
		*    Font bias (position offset), there are situations were it's more comfortable to have a special bias variable as just mObjectSpaceToClipSpace with all build in
		*  @param[in] nFlags
		*    Draw flags, see EDrawFlags
		*/
		virtual void Draw(const PLCore::String &sText, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, const PLMath::Vector2 &vScale = PLMath::Vector2::One, const PLMath::Vector2 &vBias = PLMath::Vector2::Zero, PLCore::uint32 nFlags = 0) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default
		*
		*  @param[in] cFontManager
		*    Creating font manager
		*  @param[in] sFilename
		*    Filename of the font file the data came from, used to reference the new font by filename
		*/
		PLRENDERER_API Font(FontManager &cFontManager, const PLCore::String &sFilename);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Font(const Font &cSource);

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
		Font &operator =(const Font &cSource);

		/**
		*  @brief
		*    Destroys the currently active glyphs
		*/
		PLRENDERER_API void DestroyGlyphs();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		FontManager				  *m_pFontManager;	/**< Creating font manager, always valid! */
		PLCore::String			   m_sFilename;		/**< The filename of the font file the data came from */
		PLCore::uint32			   m_nSize;			/**< Font size in points, for example 12 (72 points per inch) */
		PLCore::uint32			   m_nResolution;	/**< The horizontal and vertical resolution in DPI, for example 96 */
		PLCore::Array<FontGlyph*>  m_lstGlyphs;		/**< List of currently active glyphs ("character code -> glyph instance") */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Font.inl"


#endif // __PLRENDERER_FONT_H__
