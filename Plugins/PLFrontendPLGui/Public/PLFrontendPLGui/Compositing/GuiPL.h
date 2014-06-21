/*********************************************************\
 *  File: GuiPL.h                                        *
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


#ifndef __PLFRONTENDPLGUI_COMPOSITING_GUI_GUI_H__
#define __PLFRONTENDPLGUI_COMPOSITING_GUI_GUI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Bitset.h>
#include <PLGui/Backend/GuiImpl.h>
#include <PLGui/Gui/Base/GuiMessage.h>
#include "PLFrontendPLGui/PLFrontendPLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class CriticalSection;
}
namespace PLMath {
	class Rectangle;
}
namespace PLGui {
	class Timer;
	class Screen;
}
namespace PLRenderer {
	class Renderer;
}
namespace PLInput {
	class Keyboard;
	class VirtualController;
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
*    PixelLight ingame GUI implementation
*/
class GuiPL : public PLGui::GuiImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SNGui;
	friend class WidgetPL;


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLFRONTENDPLGUI_RTTI_EXPORT, GuiPL, "PLFrontendPLGui", PLGui::GuiImpl, "PixelLight ingame GUI implementation")
		// Attributes
		pl_attribute(InputSemantic,	PLCore::String,	"",	ReadWrite,	DirectValue,	"Semantic of this input controller (e.g. \"Camera\")",	"")
		// Constructors
		pl_constructor_1(DefaultConstructor,	PLGui::Gui*,	"Default constructor",	"")
	pl_class_end


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
		PLFRONTENDPLGUI_API GuiPL(PLGui::Gui *pGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~GuiPL();

		/**
		*  @brief
		*    Set screen size
		*
		*  @param[in] vScreenSize
		*    Screen size (default is 1024x768)
		*/
		PLFRONTENDPLGUI_API void SetScreenSize(const PLMath::Vector2i &vScreenSize);

		/**
		*  @brief
		*    Returns the used renderer
		*
		*  @return
		*    The used renderer, a null pointer on error
		*/
		PLFRONTENDPLGUI_API PLRenderer::Renderer *GetRenderer() const;

		/**
		*  @brief
		*    Get virtual input controller
		*
		*  @return
		*    Virtual input controller (can be a null pointer)
		*/
		PLFRONTENDPLGUI_API PLInput::VirtualController *GetInputController() const;

		/**
		*  @brief
		*    Set virtual input controller
		*
		*  @param[in] pInputController
		*    Virtual input controller (can be a null pointer)
		*/
		PLFRONTENDPLGUI_API void SetInputController(PLInput::VirtualController *pInputController);

		/**
		*  @brief
		*    Renders the ingame GUI
		*/
		PLFRONTENDPLGUI_API void Render();

		/**
		*  @brief
		*    Updates the ingame GUI
		*
		*  @note
		*    - Generates for example keyboard and mouse GUI messages
		*/
		PLFRONTENDPLGUI_API void Update();


	//[-------------------------------------------------------]
	//[ Public virtual PLGui::GuiImpl functions               ]
	//[-------------------------------------------------------]
	public:
		virtual bool HasPendingMessages() override;
		virtual void ProcessMessage() override;
		virtual void PostMessage(const PLGui::GuiMessage &cMessage) override;
		virtual void EnumerateScreens(PLCore::List<PLGui::Screen*> &lstScreens) override;
		virtual PLMath::Vector2i GetScreenSize() const override;
		virtual bool HasTaskbar() override;
		virtual void SetMouseVisible(bool bVisible) override;
		virtual void ListFonts(PLCore::List<PLGui::FontInfo> &lstFonts) const override;
		virtual PLGui::WidgetImpl *CreateWidgetImpl(PLGui::Widget &cWidget) const override;
		virtual PLGui::GraphicsImpl *CreateGraphicsImpl(PLGui::Graphics &cGraphics) const override;
		virtual PLGui::ImageImpl *CreateImageImpl(PLGui::Image &cImage) const override;
		virtual PLGui::FontImpl *CreateFontImpl(PLGui::Font &cFont) const override;
		virtual PLGui::CursorImpl *CreateCursorImpl(PLGui::Cursor &cCursor) const override;
		virtual PLGui::TrayIconImpl *CreateTrayIconImpl(PLGui::TrayIcon &cTrayIcon) const override;
		virtual PLGui::ClipBoardImpl *CreateClipBoardImpl(PLGui::ClipBoard &cClipBoard) const override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Add widget to z position sorted list of top-level widgets, by default below all already existing widget
		*
		*  @param[in] pWidget
		*    Widget
		*/
		void AddTopLevelWidget(PLGui::Widget *pWidget);

		/**
		*  @brief
		*    Render a widget recursive
		*
		*  @param[in] cWidget
		*    Widget to render
		*  @param[in] cParentRectangle
		*    Parent scissor rectangle to clip against
		*  @param[in] nTransparency
		*    Transparency mode (see ETransparency)
		*  @param[in] cColor
		*    If nMode is ColorKey, all pixels of the specified color will be transparent.
		*    If nMode is Alpha, only the alpha channel of the color will be used (1.0 = opaque, 0.0 = transparent)
		*/
		void RenderWidget(PLGui::Widget &cWidget, const PLMath::Rectangle &cParentRectangle, PLGui::ETransparency nTransparency, PLGraphics::Color4 cTransparencyColor);

		/**
		*  @brief
		*    Called when the timer has fired
		*/
		void OnTimer();

		/**
		*  @brief
		*    Get the currently pressed key modifiers
		*
		*  @param[in] cKeyboard
		*    Keyboard instance to use
		*
		*  @return
		*    The currently pressed key modifiers
		*/
		PLCore::uint32 GetKeyModifiers(PLInput::Keyboard &cKeyboard) const;

		/**
		*  @brief
		*    Update the ingame GUI keyboard
		*/
		void UpdateKeyboard();

		/**
		*  @brief
		*    Update the ingame GUI mouse
		*/
		void UpdateMouse();

		/**
		*  @brief
		*    Find the widget at the given absolute ingame GUI mouse cursor position
		*
		*  @param[in] vMousePos
		*    Absolute ingame GUI mouse cursor position
		*
		*  @return
		*    The found widget at the given absolute ingame GUI mouse cursor position, a null pointer if no widget was found
		*/
		PLGui::Widget *FindWidgetAtPos(const PLMath::Vector2i &vMousePos) const;

		/**
		*  @brief
		*    Find the widget at the given absolute ingame GUI mouse cursor position recursive
		*
		*  @param[in] vMousePos
		*    Absolute ingame GUI mouse cursor position
		*  @param[in] cParentWidget
		*    Parent widget
		*
		*  @return
		*    The found widget at the given absolute ingame GUI mouse cursor position, a null pointer if no widget was found
		*/
		PLGui::Widget *FindWidgetAtPos(const PLMath::Vector2i &vMousePos, PLGui::Widget &cParentWidget) const;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static const PLCore::uint32 NumOfMouseButtons = 3;	/**< Number of supported mouse buttons */


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerTimer;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGui::Gui						 *m_pGui;									/**< Platform independent GUI, a null pointer on error */
		PLMath::Vector2i				  m_vScreenSize;							/**< Screen size */
		PLRenderer::Renderer			 *m_pRenderer;								/**< Used renderer, a null pointer on error */
		PLCore::CriticalSection			 *m_pMessageQueueCriticalSection;			/**< GUI message queue critical section, always valid! */
		PLCore::List<PLGui::GuiMessage>   m_lstMessageQueue;						/**< GUI message queue */

		// Widgets
		PLGui::Widget					 *m_pMouseOver;								/**< Widget that the mouse has currently entered */
		PLCore::Array<PLGui::Widget*>	  m_lstTopLevelWidgets;						/**< List of z position sorted top-level widgets, smaller number for widgets below other with higher numbers */

		// Input
		PLInput::VirtualController		 *m_pInputController;						/**< Virtual input controller, can be a null pointer */

		// Keyboard
		PLCore::Bitset					  m_lstKeys;								/**< State of keys buttons */
		PLCore::Bitset					  m_lstRepeatKeys;							/**< Repeat state of keys buttons */
		PLGui::Timer					 *m_pKeyTimer;								/**< Key autorepeat timer */

		// Mouse
		PLMath::Vector2i				  m_vSystemMousePos;						/**< System mouse position (not the position within the ingame GUI coordinate system) */
		PLMath::Vector2i				  m_vMousePos;								/**< Ingame GUI mouse position */
		bool							  m_bMouseButtonPressed[NumOfMouseButtons];	/**< Mouse button pressed */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui


#endif // __PLFRONTENDPLGUI_COMPOSITING_GUI_GUI_H__
