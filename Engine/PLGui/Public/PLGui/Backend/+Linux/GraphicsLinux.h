/*********************************************************\
 *  File: GraphicsLinux.h                                *
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


#ifndef __PLGUI_GRAPHICSLINUX_H__
#define __PLGUI_GRAPHICSLINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Backend/GraphicsImpl.h"
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
*    Linux graphics implementation
*/
class GraphicsLinux : public GraphicsImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pDisplay
		*    Pointer to X Display
		*  @param[in] nScreen
		*    Index of screen
		*  @param[in] nNativeWindowHandle
		*    System native window handle
		*/
		PLGUI_API GraphicsLinux(::Display *pDisplay, int nScreen, ::Window nNativeWindowHandle);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGraphics
		*    Reference to platform independent graphics
		*/
		PLGUI_API GraphicsLinux(Graphics &cGraphics);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~GraphicsLinux();


	//[-------------------------------------------------------]
	//[ Public virtual GraphicsImpl functions                 ]
	//[-------------------------------------------------------]
	public:
		virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nWidth = 1) override;
		virtual void DrawRect(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0, PLCore::uint32 nWidth = 1) override;
		virtual void DrawBox(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0) override;
		virtual void DrawGradientBox(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2) override;
		virtual void DrawImage(const Image &cImage, const PLMath::Vector2i &vPos, const PLMath::Vector2i &vSize) override;
		virtual void DrawText(const Font &cFont, const PLGraphics::Color4 &cTextColor, const PLGraphics::Color4 &cBkColor, const PLMath::Vector2i &vPos, const PLCore::String &sText) override;
		virtual PLCore::uint32 GetTextWidth(const Font &cFont, const PLCore::String &sText) override;
		virtual PLCore::uint32 GetTextHeight(const Font &cFont, const PLCore::String &sText) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		::Display	*m_pDisplay;			/**< X Display */
		int			 m_nScreen;				/**< X screen */
		::Window	 m_nNativeWindowHandle;	/**< Native window that is used */
		int			 m_nColorDepth;			/**< Color depth (e.g. 16 or 24) */
		::GC		 m_sGC;					/**< Graphics context */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GRAPHICSLINUX_H__
