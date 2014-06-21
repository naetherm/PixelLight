/*********************************************************\
 *  File: ScrollWidget.h                                 *
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


#ifndef __PLGUI_SCROLLWIDGET_H__
#define __PLGUI_SCROLLWIDGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Widgets/Containers/ContainerWidget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ScrollBar;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Container that allows it's content to be scrolled
*/
class ScrollWidget : public ContainerWidget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, ScrollWidget, "PLGui", PLGui::ContainerWidget, "Container that allows it's content to be scrolled")
		// Attributes
		pl_attribute(ScrollBarStyleX,	pl_enum_type(EScrollBarStyle),	ScrollBarStyleDynamic,	ReadWrite,	GetSet,	"Style of horizontal scrollbar",	"")
		pl_attribute(ScrollBarStyleY,	pl_enum_type(EScrollBarStyle),	ScrollBarStyleDynamic,	ReadWrite,	GetSet,	"Style of horizontal scrollbar",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Slots
		pl_slot_1(OnInnerWidgetSize,	const PLMath::Vector2i&,	"Size callback",		"")
		pl_slot_1(OnScrollBarXChanged,	int,						"Scrollbar callback",	"")
		pl_slot_1(OnScrollBarYChanged,	int,						"Scrollbar callback",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Pointer to parent widget
		*/
		PLGUI_API ScrollWidget(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ScrollWidget();

		/**
		*  @brief
		*    Get scrollbar style of horizontal scrollbar
		*
		*  @return
		*    Scrollbar style (when to show the scrollbar)
		*/
		PLGUI_API EScrollBarStyle GetScrollBarStyleX() const;

		/**
		*  @brief
		*    Set scrollbar style of horizontal scrollbar
		*
		*  @param[in] nStyle
		*    Scrollbar style (when to show the scrollbar)
		*
		*  @remarks
		*    Use this to define if a scrollbar shall be visible always, never
		*    or be hidden if it is currently not needed.
		*/
		PLGUI_API void SetScrollBarStyleX(EScrollBarStyle nStyle);

		/**
		*  @brief
		*    Get scrollbar style of vertical scrollbar
		*
		*  @return
		*    Scrollbar style (when to show the scrollbar)
		*/
		PLGUI_API EScrollBarStyle GetScrollBarStyleY() const;

		/**
		*  @brief
		*    Set scrollbar style of vertical scrollbar
		*
		*  @param[in] nStyle
		*    Scrollbar style (when to show the scrollbar)
		*
		*  @remarks
		*    Use this to define if a scrollbar shall be visible always, never
		*    or be hidden if it is currently not needed.
		*/
		PLGUI_API void SetScrollBarStyleY(EScrollBarStyle nStyle);

		/**
		*  @brief
		*    Get content widget
		*
		*  @return
		*    Content widget
		*/
		PLGUI_API virtual Widget *GetContentWidget() const;


	//[-------------------------------------------------------]
	//[ Public virtual Widget functions                       ]
	//[-------------------------------------------------------]
	public:
		PLGUI_API virtual void OnSize(const PLMath::Vector2i &vSize) override;
		PLGUI_API virtual void OnAdjustContent() override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the size of the inner window has changed
		*
		*  @param[in] vSize
		*    Size
		*/
		void OnInnerWidgetSize(const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Called when the horizontal scrollbar has changed it's value
		*
		*  @param[in] nX
		*    New X-position
		*/
		void OnScrollBarXChanged(int nX);

		/**
		*  @brief
		*    Called when the vertical scrollbar has changed it's value
		*
		*  @param[in] nY
		*    New Y-position
		*/
		void OnScrollBarYChanged(int nY);

		/**
		*  @brief
		*    Show or hide scroll bars according to the widget size
		*/
		void AdjustScrollBars();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Options
		EScrollBarStyle  m_nScrollBarStyleX;	/**< Scrollbar style for horizontal scrollbar */
		EScrollBarStyle  m_nScrollBarStyleY;	/**< Scrollbar style for vertical scrollbar */

		// Data
		Widget			*m_pInnerWidget;		/**< Innermost widget that is scrolled */
		ScrollBar		*m_pScrollBarX;			/**< Scroll bar (horizontal) */
		ScrollBar		*m_pScrollBarY;			/**< Scroll bar (vertical) */
		bool			 m_bScrollBarX;			/**< Is a horizontal scrollbar visible? */
		bool			 m_bScrollBarY;			/**< Is a vertical scrollbar visible? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_SCROLLWIDGET_H__
