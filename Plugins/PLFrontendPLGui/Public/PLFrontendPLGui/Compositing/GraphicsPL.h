/*********************************************************\
 *  File: GraphicsPL.h                                   *
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


#ifndef __PLFRONTENDPLGUI_COMPOSITING_GUI_GRAPHICS_H__
#define __PLFRONTENDPLGUI_COMPOSITING_GUI_GRAPHICS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include <PLGui/Backend/GraphicsImpl.h>
#include "PLFrontendPLGui/PLFrontendPLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
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
*    PixelLight ingame GUI graphics implementation
*/
class GraphicsPL : public PLGui::GraphicsImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] vAbsPos
		*    Absolute widget position
		*  @param[in] vScreenSize
		*    Screen size
		*  @param[in] nTransparency
		*    Transparency mode (see ETransparency)
		*  @param[in] cTransparencyColor
		*    If nMode is ColorKey, all pixels of the specified color will be transparent.
		*    If nMode is Alpha, only the alpha channel of the color will be used (1.0 = opaque, 0.0 = transparent)
		*/
		PLFRONTENDPLGUI_API GraphicsPL(PLRenderer::Renderer &cRenderer, const PLMath::Vector2i &vAbsPos, const PLMath::Vector2i &vScreenSize, PLGui::ETransparency nTransparency, const PLGraphics::Color4 &cTransparencyColor);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGraphics
		*    Reference to platform independent graphics
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] vAbsPos
		*    Absolute widget position
		*  @param[in] nTransparency
		*    Transparency mode (see ETransparency)
		*  @param[in] cTransparencyColor
		*    If nMode is ColorKey, all pixels of the specified color will be transparent.
		*    If nMode is Alpha, only the alpha channel of the color will be used (1.0 = opaque, 0.0 = transparent)
		*/
		PLFRONTENDPLGUI_API GraphicsPL(PLGui::Graphics &cGraphics, PLRenderer::Renderer &cRenderer, const PLMath::Vector2i &vAbsPos, PLGui::ETransparency nTransparency, const PLGraphics::Color4 &cTransparencyColor);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~GraphicsPL();


	//[-------------------------------------------------------]
	//[ Public virtual PLGui::GraphicsImpl functions          ]
	//[-------------------------------------------------------]
	public:
		virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nWidth = 1) override;
		virtual void DrawRect(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0, PLCore::uint32 nWidth = 1) override;
		virtual void DrawBox(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0) override;
		virtual void DrawGradientBox(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2) override;
		virtual void DrawImage(const PLGui::Image &cImage, const PLMath::Vector2i &vPos, const PLMath::Vector2i &vSize) override;
		virtual void DrawText(const PLGui::Font &cFont, const PLGraphics::Color4 &cTextColor, const PLGraphics::Color4 &cBkColor, const PLMath::Vector2i &vPos, const PLCore::String &sText) override;
		virtual PLCore::uint32 GetTextWidth(const PLGui::Font &cFont, const PLCore::String &sText) override;
		virtual PLCore::uint32 GetTextHeight(const PLGui::Font &cFont, const PLCore::String &sText) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer *m_pRenderer;			/**< The used renderer, always valid! */
		PLMath::Vector2i	  m_vAbsPos;			/**< Absolute widget position */
		PLMath::Vector2i	  m_vScreenSize;		/**< Screen size */
		PLGui::ETransparency  m_nTransparency;		/**< Widget transparency type */
		PLGraphics::Color4	  m_cTransparencyColor;	/**< Widget color */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui


#endif // __PLFRONTENDPLGUI_COMPOSITING_GUI_GRAPHICS_H__
