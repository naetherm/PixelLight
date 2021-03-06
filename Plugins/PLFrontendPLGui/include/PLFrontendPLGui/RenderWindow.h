/*********************************************************\
 *  File: RenderWindow.h                                 *
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


#ifndef __PLFRONTENDPLGUI_RENDERWINDOW_H__
#define __PLFRONTENDPLGUI_RENDERWINDOW_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Gui/Resources/Timer.h>
#include <PLGui/Widgets/Windows/Window.h>
#include "PLFrontendPLGui/WindowConnection.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    GUI window that implements a renderer surface
*
*  @remarks
*    Use 'RenderWindow' to create a window which automatically
*    implements a renderer surface handler and creates the window surface.
*
*  @note
*    - By default, it's allowed to switch windows into fullscreen mode using Alt-Return or AltGr-Return
*    - By default, it's allowed to use Alt-Tab
*/
class RenderWindow : public PLGui::Window, public WindowConnection {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLFRONTENDPLGUI_API)
		// Slots
		pl_slot_0_def(RenderWindow,	OnTimer);
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] pParent
		*    Parent widget, can be a null pointer
		*  @param[in] pDisplayMode
		*    Display mode information, can be a null pointer
		*  @param[in] bFullscreen
		*    Initial in fullscreen mode? If yes, 'pDisplayMode' information is required, if not given,
		*    no fullscreen is possible!
		*
		*  @note
		*    - If 'pDisplayMode' is not given, default settings are used
		*    - If no fullscreen mode, just the size from 'pDisplayMode' are used.
		*/
		PLFRONTENDPLGUI_API RenderWindow(PLRenderer::Renderer &cRenderer, PLGui::Widget *pParent = nullptr, const PLRenderer::DisplayMode *pDisplayMode = nullptr, bool bFullscreen = false);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~RenderWindow();


	//[-------------------------------------------------------]
	//[ Protected virtual PLGui::WidgetFunctions functions    ]
	//[-------------------------------------------------------]
	protected:
		PLFRONTENDPLGUI_API virtual void OnDraw(PLGui::Graphics &cGraphics);


	//[-------------------------------------------------------]
	//[ Protected virtual WindowConnection functions          ]
	//[-------------------------------------------------------]
	protected:
		PLFRONTENDPLGUI_API virtual void OnDisplayMode() override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		RenderWindow(const RenderWindow &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		RenderWindow &operator =(const RenderWindow &cSource);

		/**
		*  @brief
		*    Timer callback
		*/
		void OnTimer();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGui::Timer	 m_cTimer;		/**< Timer */
		bool			 m_bBackupAvailable;
		PLMath::Vector2i m_vBackupPos;
		PLMath::Vector2i m_vBackupSize;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui


#endif // __PLFRONTENDPLGUI_RENDERWINDOW_H__
