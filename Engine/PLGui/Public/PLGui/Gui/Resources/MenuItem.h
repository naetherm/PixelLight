/*********************************************************\
 *  File: MenuItem.h                                     *
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


#ifndef __PLGUI_MENUITEM_H__
#define __PLGUI_MENUITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLGui/Gui/Resources/Image.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Represents an item in a menu structure
*/
class MenuItem {


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
		PLGUI_API MenuItem(Gui &cGui);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] sText
		*    Text of the menu item
		*/
		PLGUI_API MenuItem(Gui &cGui, const PLCore::String &sText);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] sText
		*    Text of the menu item
		*  @param[in] cIcon
		*    Icon of the menu item
		*/
		PLGUI_API MenuItem(Gui &cGui, const PLCore::String &sText, const Image &cIcon);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~MenuItem();

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object (never a null pointer)
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get type of menu item
		*
		*  @return
		*    Menu item type
		*/
		PLGUI_API EMenuItemType GetType() const;

		/**
		*  @brief
		*    Get command ID
		*
		*  @return
		*    Command ID of the menu item
		*/
		PLGUI_API PLCore::uint32 GetID() const;

		/**
		*  @brief
		*    Set command ID
		*
		*  @param[in] nID
		*    Command ID of the menu item
		*/
		PLGUI_API void SetID(PLCore::uint32 nID);

		/**
		*  @brief
		*    Get item text
		*
		*  @return
		*    Text of the menu item
		*/
		PLGUI_API PLCore::String GetText() const;

		/**
		*  @brief
		*    Set item text
		*
		*  @param[in] sText
		*    Text of the menu item
		*/
		PLGUI_API void SetText(const PLCore::String &sText);

		/**
		*  @brief
		*    Get icon
		*
		*  @return
		*    Icon for the menu item
		*/
		PLGUI_API const Image &GetIcon() const;

		/**
		*  @brief
		*    Set icon
		*
		*  @param[in] cIcon
		*    Icon for the menu item
		*/
		PLGUI_API void SetIcon(const Image &cIcon);

		/**
		*  @brief
		*    Get shortcut of menu item (e.g. if text is "&Test" -> 'T' is the shortcut)
		*
		*  @return
		*    Character that can be pressed to select the menu item
		*/
		PLGUI_API char GetShortcut() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] nType
		*    Type of menu item
		*/
		PLGUI_API MenuItem(Gui &cGui, EMenuItemType nType);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui			   *m_pGui;			/**< Pointer to owner GUI */
		EMenuItemType	m_nType;		/**< Menu item type */
		PLCore::uint32	m_nCommandID;	/**< Command ID */
		PLCore::String	m_sText;		/**< Text that is displayed */
		Image			m_cIcon;		/**< Icon */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MENUITEM_H__
