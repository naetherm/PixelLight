/*********************************************************\
 *  File: TabContainer.h                                 *
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


#ifndef __PLGUI_TABCONTAINER_H__
#define __PLGUI_TABCONTAINER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Widgets/Containers/WindowContainer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class TabBar;
class TabBarEntry;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tab container
*/
class TabContainer : public WindowContainer {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, TabContainer, "PLGui", PLGui::WindowContainer, "Tab container")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<> EventCreateWindow;		/**< New window shall be created */


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
		PLGUI_API TabContainer(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TabContainer();

		/**
		*  @brief
		*    Get side the tabbar is positioned on
		*
		*  @return
		*    Side
		*/
		PLGUI_API ESide GetSide() const;

		/**
		*  @brief
		*    Set side the tabbar is positioned on
		*
		*  @param[in] nSide
		*    Side
		*/
		PLGUI_API void SetSide(ESide nSide);

		/**
		*  @brief
		*    Get TabBar options
		*
		*  @return
		*    TabBar options (combination of ETabBarOption values)
		*/
		PLGUI_API PLCore::uint32 GetOptions() const;

		/**
		*  @brief
		*    Set TabBar options
		*
		*  @param[in] nOptions
		*    TabBar options
		*/
		PLGUI_API void SetOptions(PLCore::uint32 nOptions);

		/**
		*  @brief
		*    Get tab width
		*
		*  @return
		*    Tab width
		*
		*  @remarks
		*    see SetTabWidth()
		*/
		PLGUI_API PLCore::uint32 GetTabWidth() const;

		/**
		*  @brief
		*    Set tab width
		*
		*  @param[in] nWidth
		*    Tab width
		*
		*  @note
		*    - If side is SideTop or SideBottom and StaticTabSize is *not* set, tabs have a floating size.
		*      In that case, the width set by nWidth is the maximum size that a tab can have.
		*    - If side is SideLeft or SideRight, or StaticTabSize is set, tabs have a static size.
		*      In that case, every tab will get exactly the width set by nWidth.
		*/
		PLGUI_API void SetTabWidth(PLCore::uint32 nWidth);

		/**
		*  @brief
		*    Get tab height
		*
		*  @return
		*    Tab height
		*/
		PLGUI_API PLCore::uint32 GetTabHeight() const;

		/**
		*  @brief
		*    Set tab height
		*
		*  @param[in] nHeight
		*    Tab height
		*/
		PLGUI_API void SetTabHeight(PLCore::uint32 nHeight);


	//[-------------------------------------------------------]
	//[ Protected virtual TabContainer functions              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a new window shall be created
		*/
		PLGUI_API void OnCreateWindow();


	//[-------------------------------------------------------]
	//[ Protected virtual WindowContainer functions           ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnAddWindow(Widget *pWindow);
		PLGUI_API virtual void OnRemoveWindow(Widget *pWindow);
		PLGUI_API virtual void OnSelectWindow(int nWindow);


	//[-------------------------------------------------------]
	//[ Protected virtual Widget functions                    ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnAdjustContent();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a tab to be is closed
		*
		*  @param[in] pTab
		*    Tab
		*/
		void OnTabBarCloseTab(TabBarEntry *pTab);

		/**
		*  @brief
		*    Called when the current selection has been changed
		*
		*  @param[in] nTab
		*    Index of currently selected tab
		*/
		void OnTabBarSelectTab(int nTab);

		/**
		*  @brief
		*    Called when a new tab shall be created
		*/
		void OnTabBarCreateTab();

		/**
		*  @brief
		*    Find tab that belongs to a certain window
		*
		*  @param[in] pWindow
		*    Window
		*
		*  @return
		*    Tab entry belonging to the window (or a null pointer)
		*/
		TabBarEntry *FindTab(Widget *pWindow) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	public:
		// Event handlers
		PLCore::EventHandler<TabBarEntry*>	EventHandlerCloseTab;
		PLCore::EventHandler<int>			EventHandlerSelectTab;
		PLCore::EventHandler<>				EventHandlerCreateTab;

		// Widgets
		TabBar *m_pTabBar;		/**< Tab-bar */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TABCONTAINER_H__
