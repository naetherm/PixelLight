/*********************************************************\
 *  File: FontPL.cpp                                     *
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
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Resources/Font.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/FontManager.h>
#include "PLFrontendPLGui/Compositing/GuiPL.h"
#include "PLFrontendPLGui/Compositing/FontPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGui;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FontPL::FontPL(Font &cFont) : FontImpl(cFont),
	m_pFont(&cFont)
{
}

/**
*  @brief
*    Destructor
*/
FontPL::~FontPL()
{
	// Do not destroy the renderer font, we might need it again...
}

/**
*  @brief
*    Returns the renderer font
*/
PLRenderer::Font *FontPL::GetRendererFont()
{
	// Get the renderer font
	PLRenderer::Font *pRendererFont = static_cast<PLRenderer::Font*>(m_cRendererFont.GetResource());

	// Is the renderer font already there?
	if (!pRendererFont) {
		// Get the PixelLight ingame GUI implementation
		GuiPL *pGuiPL = static_cast<GuiPL*>(m_pFont->GetGui()->GetImpl());
		if (pGuiPL) {
			// Get the renderer instance
			PLRenderer::Renderer *pRenderer = pGuiPL->GetRenderer();
			if (pRenderer) {
				pRendererFont = reinterpret_cast<PLRenderer::Font*>(pRenderer->GetFontManager().GetFontTexture(m_pFont->GetFilename(), m_pFont->GetHeight()));
				m_cRendererFont.SetResource(pRendererFont);
			}
		}
	}

	// Return the renderer font
	return pRendererFont;
}


//[-------------------------------------------------------]
//[ Public virtual PLGui::FontImpl functions              ]
//[-------------------------------------------------------]
bool FontPL::LoadFont(const String &sFamily, uint32 nHeight, uint32 nWeight, EFontStyle nStyle)
{
	// Not supported within the PixelLight ingame GUI

	// Error!
	return false;
}

bool FontPL::LoadFont(const String &sFilename, uint32 nHeight)
{
	// Do not destroy the renderer font, we might need it again...

	// Just set the font pointer to null, the font manager will do the rest
	m_cRendererFont.SetResource(nullptr);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
