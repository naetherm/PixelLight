/*********************************************************\
 *  File: ClipBoard.cpp                                  *
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
#include "PLGui/Gui/ClipBoard.h"
#include "PLGui/Gui/Data/DataObject.h"
#include "PLGui/Backend/ClipBoardImpl.h"


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
ClipBoard::ClipBoard(Gui &cGui) :
	m_pGui(&cGui),
	m_pImpl(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ClipBoard::~ClipBoard()
{
}

/**
*  @brief
*    Get owner GUI
*/
Gui *ClipBoard::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get implementation
*/
ClipBoardImpl *ClipBoard::GetImpl() const
{
	// Return implementation
	return m_pImpl;
}

/**
*  @brief
*    Get data from clipboard
*/
DataObject ClipBoard::GetData()
{
	// Get data
	return m_pImpl->GetData();
}

/**
*  @brief
*    Save data to clipboard
*/
void ClipBoard::SetData(const DataObject &cData)
{
	// Set data
	m_pImpl->SetData(cData);
}

/**
*  @brief
*    Get text from clipboard
*/
String ClipBoard::GetText()
{
	// Get text
	return m_pImpl->GetData().GetString();
}

/**
*  @brief
*    Save text to clipboard
*/
void ClipBoard::SetText(const String &sText)
{
	// Set text
	DataObject cData(sText);
	m_pImpl->SetData(cData);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initialize clipboard (create backend)
*/
void ClipBoard::InitClipBoard()
{
	// We must have a valid GUI
	if (m_pGui) {
		// Create clipboard implementation
		m_pImpl = m_pGui->CreateClipBoardImpl(*this);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
