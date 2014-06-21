/*********************************************************\
 *  File: FontInfo.h                                     *
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


#ifndef __PLGUI_FONTINFO_H__
#define __PLGUI_FONTINFO_H__
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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Font information
*/
class FontInfo {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API FontInfo();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sFilename
		*    Font file name
		*  @param[in] sFamily
		*    Font family name
		*  @param[in] nHeight
		*    Font height
		*  @param[in] nWeight
		*    Font weight (@see EFontWeight)
		*  @param[in] nStyle
		*    Font style
		*/
		PLGUI_API FontInfo(const PLCore::String &sFilename, const PLCore::String &sFamily, PLCore::uint32 nHeight, EFontWeight nWeight, EFontStyle nStyle);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cFontInfo
		*    Font info that is copied
		*/
		PLGUI_API FontInfo(const FontInfo &cFontInfo);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~FontInfo();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cFontInfo
		*    Font info that is copied
		*/
		PLGUI_API FontInfo &operator =(const FontInfo &cFontInfo);

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cFontInfo
		*    Font info that is compared
		*
		*  @return
		*    'true' if fonts are equal, else 'false'
		*/
		PLGUI_API bool operator ==(const FontInfo &cFontInfo) const;

		/**
		*  @brief
		*    Get font file name
		*
		*  @return
		*    File name
		*
		*  @remarks
		*    Will only return a file name if the font file has been added manually
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
		*    Get font height
		*
		*  @return
		*    Font height
		*/
		PLGUI_API PLCore::uint32 GetHeight() const;

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
		*    Get font style
		*
		*  @return
		*    Font style
		*/
		PLGUI_API EFontStyle GetStyle() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String m_sFilename;	/**< Filename for directly loaded fonts */
		PLCore::String m_sFamily;	/**< Font family */
		PLCore::uint32 m_nHeight;	/**< Font height */
		EFontWeight	   m_nWeight;	/**< Font weight */
		EFontStyle	   m_nStyle;	/**< Font style (normal, italics, ...) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_FONTINFO_H__
