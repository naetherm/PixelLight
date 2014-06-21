/*********************************************************\
 *  File: WindowContainer.h                              *
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


#ifndef __PLGUI_WINDOWCONTAINER_H__
#define __PLGUI_WINDOWCONTAINER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include "PLGui/Widgets/Widget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Base class for window containers
*/
class WindowContainer : public Widget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, WindowContainer, "PLGui", PLGui::Widget, "Base class for window containers")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<Widget*>	EventAddWindow;		/**< Window has been added */
		PLCore::Event<Widget*>	EventRemoveWindow;	/**< Window has been removed */
		PLCore::Event<int>		EventSelectWindow;	/**< Current selection has been changed */


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
		PLGUI_API WindowContainer(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~WindowContainer();

		/**
		*  @brief
		*    Get content widget
		*
		*  @return
		*    Content widget
		*
		*  @remarks
		*    If a widget is a container for other widgets, a container widget is often used as the parent for all of
		*    the child widgets. This is e.g. necessary to separate the "outside" from the "inside", e.g. there may be
		*    other (internal) child widgets, that are also children of the same parent (e.g. buttons in the title bar or
		*    frame widgets). So if you want to insert widgets inside other widgets, you should use the widget returned
		*    by GetContentWidget() as the parent widget, not the widget itself. If a widget don't have a content widget,
		*    it will return a pointer to itself as the content widget.
		*/
		PLGUI_API virtual Widget *GetContentWidget() const;

		/**
		*  @brief
		*    Clear container
		*/
		PLGUI_API void Clear();

		/**
		*  @brief
		*    Get list of windows inside the container
		*
		*  @return
		*    List of windows
		*/
		PLGUI_API const PLCore::Container<Widget*> &GetWindows() const;

		/**
		*  @brief
		*    Add window to container
		*
		*  @param[in] pWindow
		*    Window
		*/
		PLGUI_API void AddWindow(Widget *pWindow);

		/**
		*  @brief
		*    Remove window from container
		*
		*  @param[in] pWindow
		*    Window
		*/
		PLGUI_API void RemoveWindow(Widget *pWindow);

		/**
		*  @brief
		*    Get currently selected window
		*
		*  @return
		*    Index of currently selected window
		*/
		PLGUI_API int GetSelection() const;

		/**
		*  @brief
		*    Set currently selected window
		*
		*  @param[in] nWindow
		*    Index of currently selected window
		*/
		PLGUI_API void SetSelection(int nWindow);

		/**
		*  @brief
		*    Set currently selected window
		*
		*  @param[in] pWindow
		*    Pointer to currently selected window
		*/
		PLGUI_API void SetSelection(Widget *pWindow);


	//[-------------------------------------------------------]
	//[ Protected virtual WindowContainer functions           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a window has been added
		*
		*  @param[in] pWindow
		*    Window
		*/
		PLGUI_API virtual void OnAddWindow(Widget *pWindow);

		/**
		*  @brief
		*    Called when a window has been removed
		*
		*  @param[in] pWindow
		*    Window
		*/
		PLGUI_API virtual void OnRemoveWindow(Widget *pWindow);

		/**
		*  @brief
		*    Called when the current selection has been changed
		*
		*  @param[in] nWindow
		*    Index of currently selected window
		*/
		PLGUI_API virtual void OnSelectWindow(int nWindow);


	//[-------------------------------------------------------]
	//[ Protected virtual Widget functions                    ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnAdjustContent();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	public:
		Widget				  *m_pContentWidget;	/**< Inner widget that contains the content windows */
		PLCore::List<Widget*>  m_lstWindows;		/**< List of windows */
		int					   m_nSelection;		/**< Index of currently selected window */
		Widget				  *m_pSelection;		/**< Pointer to currently selected window */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_WINDOWCONTAINER_H__
