/*********************************************************\
 *  File: Screen.cpp                                     *
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
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Gui/Screen.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Screen::Screen(Gui *pGui) :
	m_pGui(pGui),
	m_pDesktopWidget(nullptr),
	m_bDefault(false)
{
}

/**
*  @brief
*    Destructor
*/
Screen::~Screen()
{
	// Destroy desktop widget
	if (m_pDesktopWidget) {
		delete m_pDesktopWidget;
	}
}

/**
*  @brief
*    Get owner GUI
*/
Gui *Screen::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get screen name
*/
String Screen::GetName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Set screen name
*/
void Screen::SetName(const String &sName)
{
	// Set name
	m_sName = sName;
}

/**
*  @brief
*    Get screen position
*/
Vector2i Screen::GetPos() const
{
	// Return position
	return m_vPos;
}

/**
*  @brief
*    Set screen position
*/
void Screen::SetPos(const PLMath::Vector2i &vPos)
{
	// Set pos
	m_vPos = vPos;
}

/**
*  @brief
*    Get screen size
*/
Vector2i Screen::GetSize() const
{
	// Return size
	return m_vSize;
}

/**
*  @brief
*    Set screen size
*/
void Screen::SetSize(const Vector2i &vSize)
{
	// Set size
	m_vSize = vSize;
}

/**
*  @brief
*    Check if this is the primary screen
*/
bool Screen::IsDefault() const
{
	// Return default flag
	return m_bDefault;
}

/**
*  @brief
*    Set if this is the default screen
*/
void Screen::SetDefault(bool bDefault)
{
	// Set default flag
	m_bDefault = bDefault;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
