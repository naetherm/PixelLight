/*********************************************************\
 *  File: TabBar.h                                       *
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


#ifndef __PLGUI_TABBAR_H__
#define __PLGUI_TABBAR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include <PLCore/Base/Event/Event.h>
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Widgets/Controls/TabBarEntry.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tab bar widget
*/
class TabBar : public Widget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, TabBar, "PLGui", PLGui::Widget, "Tab bar widget")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class TabBarEntry;
	friend class TabBarButton;


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<TabBarEntry*>	EventAddTab;		/**< New tab is added */
		PLCore::Event<TabBarEntry*>	EventRemoveTab;		/**< Tab is removed */
		PLCore::Event<TabBarEntry*>	EventCloseTab;		/**< Tab is to be closed */
		PLCore::Event<int>			EventSelectTab;		/**< Current selection has been changed */
		PLCore::Event<>				EventCreateTab;		/**< New tab shall be created */


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
		PLGUI_API TabBar(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TabBar();

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

		/**
		*  @brief
		*    Create a new tab
		*
		*  @remarks
		*    CreateTab creates an event saying that a new tab shall be created.
		*    This does not do anything automatically, so you have to catch the
		*    event and call AddTab() to actually create a new tab.
		*/
		PLGUI_API void CreateTab();

		/**
		*  @brief
		*    Close tab
		*
		*  @param[in] pTab
		*    Tab
		*
		*  @remarks
		*    See CloseTab(PLCore::uint32 nTab)
		*/
		PLGUI_API void CloseTab(TabBarEntry *pTab);

		/**
		*  @brief
		*    Close tab
		*
		*  @param[in] nTab
		*    Tab index
		*
		*  @remarks
		*    CloseTab creates an event saying that the given tab shall
		*    be closed (the same as when the used has actually clicked
		*    on the tab's 'x'-button). This does not automatically remove
		*    the tab but only generate the event. Do actually remove a tab,
		*    use RemoveTab().
		*/
		PLGUI_API void CloseTab(PLCore::uint32 nTab);

		/**
		*  @brief
		*    Get list of tabs
		*
		*  @return
		*    List of tabs
		*/
		PLGUI_API const PLCore::Container<TabBarEntry*> &GetTabs() const;

		/**
		*  @brief
		*    Add tab
		*
		*  @param[in] sText
		*    Tab text
		*
		*  @return
		*    New tab entry
		*/
		PLGUI_API TabBarEntry *AddTab(const PLCore::String &sText);

		/**
		*  @brief
		*    Add tab
		*
		*  @param[in] sText
		*    Tab text
		*  @param[in] cIcon
		*    Image
		*
		*  @return
		*    New tab entry
		*/
		PLGUI_API TabBarEntry *AddTab(const PLCore::String &sText, const Image &cIcon);

		/**
		*  @brief
		*    Remove tab
		*
		*  @param[in] pTab
		*    Tab
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLGUI_API bool RemoveTab(TabBarEntry *pTab);

		/**
		*  @brief
		*    Remove tab
		*
		*  @param[in] nTab
		*    Tab index
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLGUI_API bool RemoveTab(PLCore::uint32 nTab);

		/**
		*  @brief
		*    Get currently selected tab
		*
		*  @return
		*    Index of currently selected tab
		*/
		PLGUI_API int GetSelection() const;

		/**
		*  @brief
		*    Set currently selected tab
		*
		*  @param[in] nTab
		*    Index of currently selected tab
		*/
		PLGUI_API void SetSelection(int nTab);

		/**
		*  @brief
		*    Get scrolling position
		*
		*  @return
		*    Index of first visible tab
		*/
		PLGUI_API int GetScrollingPos() const;

		/**
		*  @brief
		*    Set scrolling position
		*
		*  @param[in] nTab
		*    Index of first visible tab
		*/
		PLGUI_API void SetScrollingPos(int nTab);

		/**
		*  @brief
		*    Get tab that is currently selected by the mouse
		*
		*  @return
		*    Index of currently selected tab
		*/
		PLGUI_API int GetMouseSelection() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Redraw tab
		*
		*  @param[in] nTab
		*    Index of tab
		*/
		PLGUI_API void RedrawTab(int nTab);

		/**
		*  @brief
		*    Set tab that is currently selected by the mouse
		*
		*  @param[in] nTab
		*    Index of currently selected tab
		*/
		PLGUI_API void SetMouseSelection(int nTab);


	//[-------------------------------------------------------]
	//[ Protected virtual Widget functions                    ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const;
		PLGUI_API virtual void OnAdjustContent();


	//[-------------------------------------------------------]
	//[ Protected virtual TabBar functions                    ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a new tab is added
		*
		*  @param[in] pTab
		*    Tab
		*/
		PLGUI_API virtual void OnAddTab(TabBarEntry *pTab);

		/**
		*  @brief
		*    Called when a tab is removed
		*
		*  @param[in] pTab
		*    Tab
		*/
		PLGUI_API virtual void OnRemoveTab(TabBarEntry *pTab);

		/**
		*  @brief
		*    Called when a tab shall be closed
		*
		*  @param[in] pTab
		*    Tab
		*/
		PLGUI_API virtual void OnCloseTab(TabBarEntry *pTab);

		/**
		*  @brief
		*    Called when the current selection has been changed
		*
		*  @param[in] nTab
		*    Index of currently selected tab
		*/
		PLGUI_API virtual void OnSelectTab(int nTab);

		/**
		*  @brief
		*    Called when a new tab shall be created
		*/
		PLGUI_API virtual void OnCreateTab();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	public:
		// TabBar data
		ESide						m_nSide;			/**< Side the tabbar is positioned on */
		PLCore::uint32				m_nOptions;			/**< TabBar options (ETabBarOption) */
		PLCore::uint32				m_nTabWidth;		/**< Tab width */
		PLCore::uint32				m_nTabHeight;		/**< Tab height */
		PLCore::List<TabBarEntry*>	m_lstTabs;			/**< List of tabs */
		int							m_nSelection;		/**< Index of currently selected tab */
		int							m_nMouseSelection;	/**< Index of tab currently selected by the mouse */
		int							m_nScrollingPos;	/**< First displayed tab (scrolling) */

		// Child widgets
		TabBarButton			   *m_pButtonCreate;	/**< Button to create a new tab */
		TabBarButton			   *m_pButtonNext;		/**< Button to scroll (next) */
		TabBarButton			   *m_pButtonPrev;		/**< Button to scroll (previous) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TABBAR_H__
