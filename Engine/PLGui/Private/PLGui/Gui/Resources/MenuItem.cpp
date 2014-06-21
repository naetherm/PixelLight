/*********************************************************\
 *  File: MenuItem.cpp                                   *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Gui/Resources/MenuItem.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MenuItem::MenuItem(Gui &cGui) :
	m_pGui(&cGui),
	m_nType(TypeMenuItem),
	m_nCommandID(0),
	m_cIcon(cGui)
{
}

/**
*  @brief
*    Constructor
*/
MenuItem::MenuItem(Gui &cGui, const String &sText) :
	m_pGui(&cGui),
	m_nType(TypeMenuItem),
	m_nCommandID(0),
	m_sText(sText),
	m_cIcon(cGui)
{
}

/**
*  @brief
*    Constructor
*/
MenuItem::MenuItem(Gui &cGui, const String &sText, const Image &cIcon) :
	m_pGui(&cGui),
	m_nType(TypeMenuItem),
	m_nCommandID(0),
	m_sText(sText),
	m_cIcon(cIcon)
{
}

/**
*  @brief
*    Destructor
*/
MenuItem::~MenuItem()
{
}

/**
*  @brief
*    Get owner GUI
*/
Gui *MenuItem::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get type of menu item
*/
EMenuItemType MenuItem::GetType() const
{
	// Return menu item type
	return m_nType;
}

/**
*  @brief
*    Get command ID
*/
uint32 MenuItem::GetID() const
{
	// Return command ID
	return m_nCommandID;
}

/**
*  @brief
*    Set command ID
*/
void MenuItem::SetID(uint32 nID)
{
	// Set command ID
	m_nCommandID = nID;
}

/**
*  @brief
*    Get item text
*/
String MenuItem::GetText() const
{
	// Return text
	return m_sText;
}

/**
*  @brief
*    Set item text
*/
void MenuItem::SetText(const String &sText)
{
	// Set text
	m_sText = sText;
}

/**
*  @brief
*    Get icon
*/
const Image &MenuItem::GetIcon() const
{
	// Return image
	return m_cIcon;
}

/**
*  @brief
*    Set icon
*/
void MenuItem::SetIcon(const Image &cIcon)
{
	// Set image
	m_cIcon = cIcon;
}

/**
*  @brief
*    Get shortcut of menu item (e.g. if text is "&Test" -> 'T' is the shortcut)
*/
char MenuItem::GetShortcut() const
{
	// Find '&' that marks a shortcut
	int nAmp = m_sText.IndexOf("&");
	if (nAmp >= 0) {
		// Get character after '&'
		String sChar = m_sText.GetSubstring(nAmp+1, 1);
		sChar.ToLower();

		// Return shortcut
		char nChar = sChar.GetASCII()[0];
		return nChar;
	}

	// No shortcut
	return '\0';
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MenuItem::MenuItem(Gui &cGui, EMenuItemType nType) :
	m_pGui(&cGui),
	m_nType(nType),
	m_nCommandID(0),
	m_cIcon(cGui)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
