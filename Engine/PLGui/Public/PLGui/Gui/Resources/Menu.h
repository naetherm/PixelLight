/*********************************************************\
 *  File: Menu.h                                         *
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


#ifndef __PLGUI_MENU_H__
#define __PLGUI_MENU_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include "PLGui/Gui/Resources/MenuItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract representation of a menu structure
*/
class Menu : public MenuItem {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*/
		PLGUI_API Menu(Gui &cGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Menu();

		/**
		*  @brief
		*    Clear menu
		*/
		PLGUI_API void Clear();

		/**
		*  @brief
		*    Get items
		*
		*  @return
		*    List of items
		*/
		PLGUI_API const PLCore::Container<MenuItem*> &GetItems() const;

		/**
		*  @brief
		*    Get item that belongs to a certain shortcut
		*
		*  @param[in] nShortcut
		*    Shortcut (ASCII)
		*
		*  @return
		*    Index of item, or -1
		*/
		PLGUI_API PLCore::int32 GetShortcutItem(char nShortcut) const;

		/**
		*  @brief
		*    Adds a new item to the menu
		*
		*  @param[in] pItem
		*    Pointer to the menu item (must be valid!)
		*  @param[in] bDelete
		*    'true' if the menu item shall be deleted automatically, else 'false'
		*/
		PLGUI_API void AddItem(MenuItem *pItem, bool bDelete = true);

		/**
		*  @brief
		*    Adds a new item to the menu at a specific index
		*
		*  @param[in] nIndex
		*    Index at which the menu item will be inserted
		*  @param[in] pItem
		*    Pointer to the menu item (must be valid!)
		*  @param[in] bDelete
		*    'true' if the menu item shall be deleted automatically, else 'false'
		*
		*  @remarks
		*    If nIndex is greater or equal to the number of elements in the menu,
		*    the new item will be added at the end of the menu.
		*/
		PLGUI_API void AddItemAtIndex(PLCore::uint32 nIndex, MenuItem *pItem, bool bDelete = true);

		/**
		*  @brief
		*    Removes an item from the menu
		*
		*  @param[in] pItem
		*    Pointer to the menu item (must be valid!)
		*/
		PLGUI_API void RemoveItem(MenuItem *pItem);

		/**
		*  @brief
		*    Adds a standard item with a text
		*
		*  @param[in] sText
		*    Text of the menu item
		*
		*  @return
		*    Item
		*/
		PLGUI_API MenuItem *AddItem(const PLCore::String &sText);

		/**
		*  @brief
		*    Adds a standard item with a text and an icon
		*
		*  @param[in] sText
		*    Text of the menu item
		*  @param[in] cIcon
		*    Icon of the menu item
		*
		*  @return
		*    Item
		*/
		PLGUI_API MenuItem *AddItem(const PLCore::String &sText, const Image &cIcon);

		/**
		*  @brief
		*    Adds a separator item to the menu
		*
		*  @return
		*    Item
		*/
		PLGUI_API MenuItem *AddSeparator();

		/**
		*  @brief
		*    Adds a sub menu to the menu
		*
		*  @param[in] pMenu
		*    Pointer to the sub menu (must be valid!)
		*  @param[in] bDelete
		*    'true' if the menu item shall be deleted automatically, else 'false'
		*
		*  @return
		*    Item
		*
		*  @remarks
		*    Usually, bDelete shall be set to 'true', so you don't have to think about deleting
		*    any create menu items at all. Only in rare cases, e.g. if you want a recursive menu
		*    structure (a menu is submenu of itself or one of it's submenus), you have to set bDelete
		*    to false to make sure that the submenu is deleted only once.
		*/
		PLGUI_API MenuItem *AddSubMenu(Menu *pMenu, bool bDelete = true);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::List<MenuItem*> m_lstItems;			/**< List of menu items */
		PLCore::List<MenuItem*> m_lstNoDeletion;	/**< List of menu items that are not automatically deleted */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MENU_H__
