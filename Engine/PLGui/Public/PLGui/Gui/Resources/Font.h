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


#ifndef __PLGUI_FONT_H__
#define __PLGUI_FONT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;
class FontInfo;
class FontImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Font class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Font {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*/
		PLGUI_API Font(Gui &cGui);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] sFamily
		*    Font family name
		*  @param[in] nHeight
		*    Font height
		*  @param[in] nWeight
		*    Font weight (@see EFontWeight)
		*  @param[in] nStyle
		*    Font style
		*/
		PLGUI_API Font(Gui &cGui, const PLCore::String &sFamily, PLCore::uint32 nHeight = 0, EFontWeight nWeight = WeightNormal, EFontStyle nStyle = StyleNormal);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] cFontInfo
		*    Font information
		*/
		PLGUI_API Font(Gui &cGui, const FontInfo &cFontInfo);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cFont
		*    Font that is copied
		*/
		PLGUI_API Font(const Font &cFont);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~Font();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cFont
		*    Font that is copied
		*/
		PLGUI_API Font &operator =(const Font &cFont);

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object (never a null pointer)
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get implementation
		*
		*  @return
		*    Pointer to platform specific implementation
		*/
		PLGUI_API FontImpl *GetImpl() const;

		/**
		*  @brief
		*    Load font
		*
		*  @param[in] sFamily
		*    Font family name
		*  @param[in] nHeight
		*    Font height
		*  @param[in] nWeight
		*    Font weight (@see EFontWeight)
		*  @param[in] nStyle
		*    Font style
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLGUI_API bool Load(const PLCore::String &sFamily, PLCore::uint32 nHeight = 0, EFontWeight nWeight = WeightNormal, EFontStyle nStyle = StyleNormal);

		/**
		*  @brief
		*    Load font from file
		*
		*  @param[in] sFilename
		*    Font filename
		*  @param[in] nHeight
		*    Font height
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLGUI_API bool LoadFromFile(const PLCore::String &sFilename, PLCore::uint32 nHeight = 0);

		/**
		*  @brief
		*    Get font information
		*
		*  @return
		*    Font info
		*/
		PLGUI_API FontInfo GetFontInfo() const;

		/**
		*  @brief
		*    Get font file name
		*
		*  @return
		*    File name
		*
		*  @remarks
		*    Will only return a file name if the font has been created using the according constructor
		*/
		PLGUI_API PLCore::String GetFilename() const;

		/**
		*  @brief
		*    Get font family
		*
		*  @return
		*    Font family name
		*/
		PLGUI_API PLCore::String GetFamily() const;

		/**
		*  @brief
		*    Set font family
		*
		*  @param[in] sFamily
		*    Font family name
		*/
		PLGUI_API void SetFamily(const PLCore::String &sFamily);

		/**
		*  @brief
		*    Get font height
		*
		*  @return
		*    Font height
		*/
		PLGUI_API PLCore::uint32 GetHeight() const;

		/**
		*  @brief
		*    Set font height
		*
		*  @param[in] nHeight
		*    Font height
		*/
		PLGUI_API void SetHeight(PLCore::uint32 nHeight);

		/**
		*  @brief
		*    Get font weight
		*
		*  @return
		*    Font weight
		*/
		PLGUI_API EFontWeight GetWeight() const;

		/**
		*  @brief
		*    Set font weight
		*
		*  @param[in] nWeight
		*    Font weight (@see EFontWeight)
		*/
		PLGUI_API void SetWeight(EFontWeight nWeight);

		/**
		*  @brief
		*    Get font style
		*
		*  @return
		*    Font style
		*/
		PLGUI_API EFontStyle GetStyle() const;

		/**
		*  @brief
		*    Set font style
		*
		*  @param[in] nStyle
		*    Font style
		*/
		PLGUI_API void SetStyle(EFontStyle nStyle);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui			   *m_pGui;			/**< Pointer to GUI */
		FontImpl	   *m_pFontImpl;	/**< Font implementation */
		PLCore::String  m_sFamily;		/**< Font family */
		PLCore::uint32  m_nHeight;		/**< Font height */
		EFontWeight		m_nWeight;		/**< Font weight */
		EFontStyle		m_nStyle;		/**< Font style (normal, italics, ...) */
		PLCore::String  m_sFilename;	/**< Filename for directly loaded fonts */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_FONT_H__
