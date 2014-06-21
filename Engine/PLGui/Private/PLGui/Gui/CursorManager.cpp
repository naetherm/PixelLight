/*********************************************************\
 *  File: CursorManager.cpp                              *
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
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/CursorManager.h"
#include "PLGui/Backend/GuiImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
CursorManager::CursorManager(Gui &cGui) :
	m_pGui(&cGui)
{
	// Create array for standard cursors
	for (int i=0; i<Cursor_Count; i++) {
		m_pCursors[i] = nullptr;
	}
}

/**
*  @brief
*    Destructor
*/
CursorManager::~CursorManager()
{
	// Destroy standard cursors
	for (int i=0; i<Cursor_Count; i++) {
		if (m_pCursors[i]) delete m_pCursors[i];
	}
}

/**
*  @brief
*    Get owner GUI
*/
Gui *CursorManager::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get standard cursor
*/
Cursor *CursorManager::GetCursor(EMouseCursor nCursor)
{
	// Get pointer to standard cursor
	int nIndex = nCursor;
	if (!m_pCursors[nIndex]) {
		// Create cursor
		m_pCursors[nIndex] = new Cursor(*m_pGui);
		m_pCursors[nIndex]->Load(nCursor);
	}

	// Return cursor
	return m_pCursors[nIndex];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
