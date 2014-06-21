/*********************************************************\
 *  File: XmlTextPrimitive.h                             *
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


#ifndef __PLGUIXMLTEXT_PRIMITIVE_H__
#define __PLGUIXMLTEXT_PRIMITIVE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLMath/Vector2i.h>
#include <PLGraphics/Color/Color4.h>
#include <PLGui/Gui/Resources/Font.h>
#include "PLGuiXmlText/PLGuiXmlText.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class Image;
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
*    XmlText primitive element
*/
class XmlTextPrimitive {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		enum EPrimitiveType {
			PrimitiveText = 0,	/**< Primitive is a word */
			PrimitiveImage,		/**< Primitive is an image */
			PrimitiveBullet		/**< Primitive is a bullet in front of a list element */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		XmlTextPrimitive() :
			m_nType(PrimitiveText),
			m_cTextColor(PLGraphics::Color4::Transparent),
			m_cBgColor(PLGraphics::Color4::Transparent),
			m_nHGap(0),
			m_pImage(nullptr),
			m_nIndent(0),
			m_pFont(nullptr)
		{
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cPrimitive
		*    Primitive to copy
		*/
		XmlTextPrimitive(const XmlTextPrimitive &cPrimitive) :
			m_nType(cPrimitive.m_nType),
			m_cTextColor(cPrimitive.m_cTextColor),
			m_cBgColor(cPrimitive.m_cBgColor),
			m_vSize(cPrimitive.m_vSize),
			m_nHGap(cPrimitive.m_nHGap),
			m_sText(cPrimitive.m_sText),
			m_pImage(cPrimitive.m_pImage),
			m_nIndent(cPrimitive.m_nIndent),
			m_pFont(cPrimitive.m_pFont ? new PLGui::Font(*cPrimitive.m_pFont) : nullptr)
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		~XmlTextPrimitive()
		{
			// Destroy the font
			if (m_pFont)
				delete m_pFont;
		}

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cPrimitive
		*    Primitive to copy
		*/
		inline XmlTextPrimitive &operator =(const XmlTextPrimitive &cPrimitive)
		{
			// Destroy the previous font
			if (m_pFont) {
				delete m_pFont;
				m_pFont = nullptr;
			}

			// Copy data
			m_nType		 = cPrimitive.m_nType;
			m_cTextColor = cPrimitive.m_cTextColor;
			m_cBgColor	 = cPrimitive.m_cBgColor;
			m_vSize		 = cPrimitive.m_vSize;
			m_sText		 = cPrimitive.m_sText;
			m_pImage	 = cPrimitive.m_pImage;
			m_nIndent	 = cPrimitive.m_nIndent;
			m_nHGap		 = cPrimitive.m_nHGap;
			m_pFont		 = cPrimitive.m_pFont ? new PLGui::Font(*cPrimitive.m_pFont) : nullptr;

			// Done
			return *this;
		}

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cPrimitive
		*    Primitive to compare with
		*
		*  @return
		*    'true' if both primitives are equal, else 'false'
		*/
		inline bool operator ==(const XmlTextPrimitive &cPrimitive) const
		{
			// Get the font implementations
			PLGui::FontImpl *pFontImpl			= m_pFont			 ? m_pFont->GetImpl()			 : nullptr;
			PLGui::FontImpl *pPrimitiveFontImpl = cPrimitive.m_pFont ? cPrimitive.m_pFont->GetImpl() : nullptr;

			// Compare
			return (m_nType		 == cPrimitive.m_nType &&
					m_cTextColor == cPrimitive.m_cTextColor &&
					m_cBgColor	 == cPrimitive.m_cBgColor &&
					m_vSize		 == cPrimitive.m_vSize &&
					m_sText		 == cPrimitive.m_sText &&
					m_pImage	 == cPrimitive.m_pImage &&
					m_nIndent	 == cPrimitive.m_nIndent &&
					m_nHGap		 == cPrimitive.m_nHGap &&
					pFontImpl	 == pPrimitiveFontImpl);
		}

		/**
		*  @brief
		*    Return the font
		*
		*  @return
		*    The font, can be a null pointer
		*/
		PLGui::Font *GetFont() const
		{
			return m_pFont;
		}

		/**
		*  @brief
		*    Set the font
		*
		*  @param[in] pFont
		*    Font to set, can be a null pointer
		*/
		void SetFont(PLGui::Font *pFont)
		{
			// Destroy the previous font
			if (m_pFont) {
				delete m_pFont;
				m_pFont = nullptr;
			}

			// Set the new font
			m_pFont = pFont ? new PLGui::Font(*pFont) : nullptr;
		}


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		// Primitive
		EPrimitiveType		 m_nType;			/**< Type of primitive */
		PLGraphics::Color4	 m_cTextColor;		/**< Current text color */
		PLGraphics::Color4	 m_cBgColor;		/**< Current background color */
		PLMath::Vector2i	 m_vSize;			/**< Size of primitive */
		int					 m_nHGap;			/**< Horizontal gap after primitive */

		// PrimitiveText
		PLCore::String	 	m_sText;			/**< Text to print */

		// PrimitiveImage
		const PLGui::Image	*m_pImage;			/**< Image */

		// PrimitiveBullet
		int					 m_nIndent;			/**< Indent at which to draw the bullet */


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGui::Font *m_pFont;	/**< Current font, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText


#endif // __PLGUIXMLTEXT_PRIMITIVE_H__
