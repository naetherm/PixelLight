/*********************************************************\
 *  File: RenderWidget.h                                 *
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


#ifndef __PLFRONTENDPLGUI_RENDERWIDGET_H__
#define __PLFRONTENDPLGUI_RENDERWIDGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Widgets/Widget.h>
#include <PLGui/Gui/Resources/Timer.h>
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
*    GUI widget with implements a renderer surface
*
*  @remarks
*    Use 'RenderWidget' to create a widget which automatically
*    implements a renderer surface handler and creates the window surface.
*/
class RenderWidget : public PLGui::Widget, public WindowConnection {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLFRONTENDPLGUI_RTTI_EXPORT, RenderWidget, "PLFrontendPLGui", PLGui::Widget, "GUI widget with implements a renderer surface")
		// Slots
		pl_slot_0(OnTimer,	"Timer callback",	"");
	pl_class_end


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
		*/
		PLFRONTENDPLGUI_API RenderWidget(PLRenderer::Renderer &cRenderer, PLGui::Widget *pParent = nullptr, const PLRenderer::DisplayMode *pDisplayMode = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDPLGUI_API virtual ~RenderWidget();


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
		PLFRONTENDPLGUI_API virtual void OnFullscreenMode() override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLFRONTENDPLGUI_API RenderWidget(const RenderWidget &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
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
		RenderWidget &operator =(const RenderWidget &cSource);

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


#endif // __PLFRONTENDPLGUI_RENDERWIDGET_H__
