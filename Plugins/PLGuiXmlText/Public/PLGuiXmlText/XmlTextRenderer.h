/*********************************************************\
 *  File: XmlTextRenderer.h                              *
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


#ifndef __PLGUIXMLTEXT_RENDERER_H__
#define __PLGUIXMLTEXT_RENDERER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include <PLMath/Vector2i.h>
#include <PLGraphics/Color/Color4.h>
#include "PLGuiXmlText/PLGuiXmlText.h"
#include "PLGuiXmlText/XmlTextPrimitive.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class Gui;
	class Graphics;
	class Image;
	class Font;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    XmlText renderer class
*/
class XmlTextRenderer {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Horizontal alignment
		*/
		enum HAlign {
			HAlignUnknown = 0,	/**< Undefined */
			HAlignLeft,			/**< Align text to the left */
			HAlignCenter,		/**< Align text centered on a line */
			HAlignRight,		/**< Align text to the right */
			HAlignJustify		/**< Align text justified in a block */
		};

		/**
		*  @brief
		*    Vertical alignment
		*/
		enum VAlign {
			VAlignUnknown = 0,	/**< Undefined */
			VAlignTop,			/**< Align text to the top of a line */
			VAlignMiddle,		/**< Align text to the middle of a line */
			VAlignBottom		/**< Align text to the bottom of a line */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ General                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Gui to which the XmlTextRenderer element belongs
		*/
		PLGUIXMLTEXT_API XmlTextRenderer(PLGui::Gui &cGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUIXMLTEXT_API ~XmlTextRenderer();

		/**
		*  @brief
		*    Returns the GUI to which the XmlTextRenderer element belongs
		*
		*  @return
		*    The GUI to which the XmlTextRenderer element belongs
		*/
		PLGUIXMLTEXT_API PLGui::Gui &GetGui() const;

		/**
		*  @brief
		*    Start rendering
		*
		*  @param[in] cGraphics
		*    Graphics object into which to render
		*  @param[in] vSize
		*    Current section size
		*/
		PLGUIXMLTEXT_API void Init(PLGui::Graphics &cGraphics, const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Finish rendering
		*/
		PLGUIXMLTEXT_API void Finish();

		/**
		*  @brief
		*    Get graphics object
		*
		*  @return
		*    Pointer to graphics object, can be a null pointer
		*/
		PLGUIXMLTEXT_API PLGui::Graphics *GetGraphics();

		//[-------------------------------------------------------]
		//[ Renderer state                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get position of current section
		*
		*  @return
		*    Position of section
		*/
		PLGUIXMLTEXT_API PLMath::Vector2i GetSectionPos() const;

		/**
		*  @brief
		*    Set position of current section
		*
		*  @param[in] vSectionPos
		*    Position of section
		*/
		PLGUIXMLTEXT_API void SetSectionPos(const PLMath::Vector2i &vSectionPos);

		/**
		*  @brief
		*    Get size of current section
		*
		*  @return
		*    Size of section
		*/
		PLGUIXMLTEXT_API PLMath::Vector2i GetSectionSize() const;

		/**
		*  @brief
		*    Set size of current section
		*
		*  @param[in] vSectionSize
		*    Size of section
		*/
		PLGUIXMLTEXT_API void SetSectionSize(const PLMath::Vector2i &vSectionSize);

		/**
		*  @brief
		*    Get current indentation
		*
		*  @return
		*    Indentation size
		*/
		PLGUIXMLTEXT_API int GetIndent() const;

		/**
		*  @brief
		*    Set current indentation
		*
		*  @param[in] nIndent
		*    Indentation size
		*/
		PLGUIXMLTEXT_API void SetIndent(int nIndent);

		/**
		*  @brief
		*    Get current font
		*
		*  @return
		*    Font, can be a null pointer
		*/
		PLGUIXMLTEXT_API PLGui::Font *GetFont() const;

		/**
		*  @brief
		*    Set current font
		*
		*  @param[in] pFont
		*    Font, XmlTextRenderer takes over the control of the instance
		*
		*  @note
		*    - The currently set font is destroyed automatically
		*/
		PLGUIXMLTEXT_API void SetFont(PLGui::Font *pFont);

		/**
		*  @brief
		*    Set current font
		*
		*  @param[in] sFont
		*    Font name
		*  @param[in] nFontSize
		*    Font size
		*/
		PLGUIXMLTEXT_API void SetFont(const PLCore::String &sFont, int nFontSize);

		/**
		*  @brief
		*    Get current text color
		*
		*  @return
		*    Text color
		*/
		PLGUIXMLTEXT_API PLGraphics::Color4 GetTextColor() const;

		/**
		*  @brief
		*    Set current text color
		*
		*  @param[in] cTextColor
		*    Text color
		*/
		PLGUIXMLTEXT_API void SetTextColor(const PLGraphics::Color4 &cTextColor);

		/**
		*  @brief
		*    Get current background color
		*
		*  @return
		*    Background color
		*/
		PLGUIXMLTEXT_API PLGraphics::Color4 GetBgColor() const;

		/**
		*  @brief
		*    Set current background color
		*
		*  @param[in] cBgColor
		*    Background color
		*/
		PLGUIXMLTEXT_API void SetBgColor(const PLGraphics::Color4 &cBgColor);

		/**
		*  @brief
		*    Get current horizontal alignment
		*
		*  @return
		*    Horizontal alignment
		*/
		PLGUIXMLTEXT_API HAlign GetHAlign() const;

		/**
		*  @brief
		*    Set current horizontal alignment
		*
		*  @param[in] nHAlign
		*    Horizontal alignment
		*/
		PLGUIXMLTEXT_API void SetHAlign(HAlign nHAlign);

		/**
		*  @brief
		*    Get current vertical alignment
		*
		*  @return
		*    Vertical alignment
		*/
		PLGUIXMLTEXT_API VAlign GetVAlign() const;

		/**
		*  @brief
		*    Set current vertical alignment
		*
		*  @param[in] nVAlign
		*    Vertical alignment
		*/
		PLGUIXMLTEXT_API void SetVAlign(VAlign nVAlign);

		//[-------------------------------------------------------]
		//[ Rendering functions                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Draw text to screen
		*
		*  @param[in] sText
		*    Text to render
		*/
		PLGUIXMLTEXT_API void DrawText(const PLCore::String &sText);

		/**
		*  @brief
		*    Draw image to screen
		*
		*  @param[in] pImage
		*    Image to render
		*  @param[in] vSize
		*    Size of image
		*/
		PLGUIXMLTEXT_API void DrawImage(const PLGui::Image *pImage, const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Draw a bullet at the beginning of the current line
		*/
		PLGUIXMLTEXT_API void DrawBullet();

		/**
		*  @brief
		*    End current line and insert a line break
		*
		*  @param[in] bForce
		*    If 'false', a new line will only be created, if the current line is not empty
		*  @param[in] bJustify
		*    If 'true', content on the current line can be aligned justified, otherwise it will be left-aligned
		*/
		PLGUIXMLTEXT_API void AddLineBreak(bool bForce = false, bool bJustify = false);

		/**
		*  @brief
		*    Insert vertical space
		*
		*  @param[in] nHeight
		*    Height of vertical space
		*/
		PLGUIXMLTEXT_API void AddVerticalSpace(int nHeight);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Add primitive to render list
		*
		*  @param[in] cPrimitive
		*    Primitive that is to be rendered
		*/
		void AddPrimitive(const XmlTextPrimitive &cPrimitive);

		/**
		*  @brief
		*    Flush primitive list and render current line
		*
		*  @param[in] bJustify
		*    'true', if the line can be justified (if according style is set)
		*/
		void Flush(bool bJustify);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Renderer data
		PLGui::Gui						*m_pGui;			/**< Gui containing the XmlText element, always valid */
		PLGui::Graphics					*m_pGraphics;		/**< Graphics object into which to render, can be a null pointer, but always valid on rendering */

		// Renderer state
		PLMath::Vector2i				 m_vSectionPos;		/**< Current section position */
		PLMath::Vector2i				 m_vSectionSize;	/**< Current section size */
		int								 m_nIndent;			/**< Current indentation */
		PLGui::Font						*m_pFont;			/**< Current font, can be a null pointer */
		PLGraphics::Color4				 m_cTextColor;		/**< Current text color */
		PLGraphics::Color4				 m_cBgColor;		/**< Current background color */
		HAlign							 m_nHAlign;			/**< Horizontal alignment */
		VAlign							 m_nVAlign;			/**< Vertical alignment */
		PLMath::Vector2i				 m_vPos;			/**< Current text position */

		// Current line
		PLCore::List<XmlTextPrimitive>	 m_lstLine;			/**< Primitives in the current line */
		int								 m_nLineWidth;		/**< Width of current line with spaces */
		int								 m_nLineWidthRaw;	/**< Width of line without spaces */
		int								 m_nLineHeight;		/**< Maximum height of all primitives */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText


#endif // __PLGUIXMLTEXT_RENDERER_H__
