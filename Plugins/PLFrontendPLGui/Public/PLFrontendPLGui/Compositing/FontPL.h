/*********************************************************\
 *  File: FontPL.h                                       *
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


#ifndef __PLFRONTENDPLGUI_COMPOSITING_GUI_FONT_H__
#define __PLFRONTENDPLGUI_COMPOSITING_GUI_FONT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/PLGuiDefinitions.h>
#include <PLGui/Backend/FontImpl.h>
#include <PLRenderer/Renderer/ResourceHandler.h>
#include "PLFrontendPLGui/PLFrontendPLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Font;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight ingame GUI font implementation
*/
class FontPL : public PLGui::FontImpl {


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
		PLFRONTENDPLGUI_API FontPL(PLGui::Font &cFont);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~FontPL();

		/**
		*  @brief
		*    Returns the renderer font
		*
		*  @return
		*    Renderer font, can be a null pointer
		*/
		PLFRONTENDPLGUI_API PLRenderer::Font *GetRendererFont();


	//[-------------------------------------------------------]
	//[ Public virtual PLGui::FontImpl functions              ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadFont(const PLCore::String &sFamily, PLCore::uint32 nHeight, PLCore::uint32 nWeight, PLGui::EFontStyle nStyle) override;
		virtual bool LoadFont(const PLCore::String &sFilename, PLCore::uint32 nHeight) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGui::Font					*m_pFont;			/**< Pointer to the platform independent font object */
		PLRenderer::ResourceHandler  m_cRendererFont;	/**< Renderer font resource handler */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui


#endif // __PLFRONTENDPLGUI_COMPOSITING_GUI_FONT_H__
