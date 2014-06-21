/*********************************************************\
 *  File: GuiNull.h                                      *
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


#ifndef __PLGUI_GUINULL_H__
#define __PLGUI_GUINULL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Backend/GuiImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null GUI implementation
*/
class GuiNull : public GuiImpl {

	
	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pGui
		*    Platform independent GUI
		*/
		PLGUI_API GuiNull(Gui *pGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~GuiNull();


	//[-------------------------------------------------------]
	//[ Public virtual GuiImpl functions                      ]
	//[-------------------------------------------------------]
	public:
		virtual bool HasPendingMessages() override;
		virtual void ProcessMessage() override;
		virtual void PostMessage(const GuiMessage &cMessage) override;
		virtual void EnumerateScreens(PLCore::List<Screen*> &lstScreens) override;
		virtual PLMath::Vector2i GetScreenSize() const override;
		virtual bool HasTaskbar() override;
		virtual void SetMouseVisible(bool bVisible) override;
		virtual void ListFonts(PLCore::List<FontInfo> &lstFonts) const override;
		virtual WidgetImpl *CreateWidgetImpl(Widget &cWidget) const override;
		virtual GraphicsImpl *CreateGraphicsImpl(Graphics &cGraphics) const override;
		virtual ImageImpl *CreateImageImpl(Image &cImage) const override;
		virtual FontImpl *CreateFontImpl(Font &cFont) const override;
		virtual CursorImpl *CreateCursorImpl(Cursor &cCursor) const override;
		virtual TrayIconImpl *CreateTrayIconImpl(TrayIcon &cTrayIcon) const override;
		virtual ClipBoardImpl *CreateClipBoardImpl(ClipBoard &cClipBoard) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GUINULL_H__
