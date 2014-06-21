/*********************************************************\
 *  File: FontLinux.h                                    *
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


#ifndef __PLGUI_FONTLINUX_H__
#define __PLGUI_FONTLINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Backend/FontImpl.h"
#include "PLGui/PLGuiLinuxIncludes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Linux font implementation
*/
class FontLinux : public FontImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFont
		*    Reference to platform independent font
		*/
		PLGUI_API FontLinux(Font &cFont);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~FontLinux();

		/**
		*  @brief
		*    Get X font
		*
		*  @return
		*    Pointer to font, can be a null pointer
		*/
		PLGUI_API XFontStruct *GetXFont() const;

		/**
		*  @brief
		*    Get font height
		*
		*  @return
		*    Font height
		*/
		PLGUI_API PLCore::uint32 GetHeight() const;


	//[-------------------------------------------------------]
	//[ Public virtual FontImpl functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadFont(const PLCore::String &sFamily, PLCore::uint32 nHeight, PLCore::uint32 nWeight, EFontStyle nStyle) override;
		virtual bool LoadFont(const PLCore::String &sFilename, PLCore::uint32 nHeight) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Display		   *m_pDisplay;	/**< X Display */
		XFontStruct	   *m_pXFont;	/**< Pointer to X-Font, can be a null pointer */
		PLCore::uint32  m_nHeight;	/**< Font height */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_FONTLINUX_H__
