/*********************************************************\
 *  File: Cursor.cpp                                     *
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
#include "PLGui/Gui/Resources/Cursor.h"
#include "PLGui/Gui/Gui.h"
#include "PLGui/Backend/CursorImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Cursor::Cursor(Gui &cGui) :
	m_pGui(&cGui),
	m_pCursorImpl(nullptr),
	m_nMouseCursor(CursorArrow)
{
	// Load cursor
	Load(CursorArrow);
}

/**
*  @brief
*    Constructor
*/
Cursor::Cursor(Gui &cGui, EMouseCursor nCursor) :
	m_pGui(&cGui),
	m_pCursorImpl(nullptr),
	m_nMouseCursor(nCursor)
{
	// Load cursor
	Load(nCursor);
}

/**
*  @brief
*    Constructor
*/
Cursor::Cursor(Gui &cGui, const String &sFilename, const Vector2i &vHotspot) :
	m_pGui(&cGui),
	m_pCursorImpl(nullptr),
	m_nMouseCursor(CursorCustom),
	m_sFilename(sFilename)
{
	// Load cursor
	Load(sFilename, vHotspot);
}

/**
*  @brief
*    Copy constructor
*/
Cursor::Cursor(const Cursor &cCursor) :
	m_pGui(cCursor.m_pGui),
	m_pCursorImpl(cCursor.m_pCursorImpl),
	m_nMouseCursor(cCursor.m_nMouseCursor),
	m_sFilename(cCursor.m_sFilename)
{
	// Increase reference count for cursor
	m_pCursorImpl->AddReference();
}

/**
*  @brief
*    Destructor
*/
Cursor::~Cursor()
{
	// Release implementation object
	m_pCursorImpl->Release();
}

/**
*  @brief
*    Assignment operator
*/
Cursor &Cursor::operator =(const Cursor &cCursor)
{
	// Release cursor
	if (m_pCursorImpl) {
		m_pCursorImpl->Release();
		m_pCursorImpl = nullptr;
	}

	// Copy cursor
	m_pCursorImpl	= cCursor.m_pCursorImpl;
	m_nMouseCursor	= cCursor.m_nMouseCursor;
	m_sFilename		= cCursor.m_sFilename;
	if (m_pCursorImpl) {
		m_pCursorImpl->AddReference();
	}

	// Return reference to this cursor
	return *this;
}

/**
*  @brief
*    Get owner GUI
*/
Gui *Cursor::GetGui() const
{
	// Return GUI
	return m_pGui;
}

/**
*  @brief
*    Get implementation
*/
CursorImpl *Cursor::GetImpl() const
{
	// Return cursor implementation
	return m_pCursorImpl;
}

/**
*  @brief
*    Load cursor
*/
void Cursor::Load(EMouseCursor nCursor)
{
	// Release cursor
	if (m_pCursorImpl) {
		m_pCursorImpl->Release();
		m_pCursorImpl = nullptr;
	}

	// Store cursor data
	m_nMouseCursor	= (nCursor == CursorCustom ? CursorArrow : nCursor);
	m_sFilename		= "";

	// Create cursor
	m_pCursorImpl = m_pGui->CreateCursorImpl(*this);
	m_pCursorImpl->AddReference();

	// Load cursor
	m_pCursorImpl->Load(m_nMouseCursor);
}

/**
*  @brief
*    Load cursor
*/
void Cursor::Load(const String &sFilename, const Vector2i &vHotspot)
{
	// Release cursor
	if (m_pCursorImpl) {
		m_pCursorImpl->Release();
		m_pCursorImpl = nullptr;
	}

	// Store cursor data
	m_nMouseCursor	= CursorCustom;
	m_sFilename		= sFilename;

	// Create cursor
	m_pCursorImpl = m_pGui->CreateCursorImpl(*this);
	m_pCursorImpl->AddReference();

	// Load cursor
	m_pCursorImpl->Load(m_sFilename, vHotspot);
}

/**
*  @brief
*    Get cursor ID
*/
EMouseCursor Cursor::GetCursorID() const
{
	// Return cursor ID
	return m_nMouseCursor;
}

/**
*  @brief
*    Get cursor filename
*/
String Cursor::GetFilename() const
{
	// Return filename
	return m_sFilename;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
