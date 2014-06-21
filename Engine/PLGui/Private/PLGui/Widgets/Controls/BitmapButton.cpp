/*********************************************************\
 *  File: BitmapButton.cpp                               *
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
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Widgets/Controls/BitmapButton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(BitmapButton)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
BitmapButton::BitmapButton(Widget *pParent) : AbstractButton(pParent),
	m_cImageEnabled(*GetGui()),
	m_cImageDisabled(*GetGui()),
	m_cImageSelected(*GetGui()),
	m_cImagePressed(*GetGui())
{
	// No background
	SetBackgroundColor(Color4::Transparent);

	// Accept focus
	SetFocusStyle(AcceptFocus);
}

/**
*  @brief
*    Destructor
*/
BitmapButton::~BitmapButton()
{
}

/**
*  @brief
*    Get image for enabled-state
*/
const Image &BitmapButton::GetImageEnabled() const
{
	// Return image
	return m_cImageEnabled;
}

/**
*  @brief
*    Set image for enabled-state
*/
void BitmapButton::SetImageEnabled(const Image &cImage)
{
	// Set image
	m_cImageEnabled = cImage;
}

/**
*  @brief
*    Get image for disabled-state
*/
const Image &BitmapButton::GetImageDisabled() const
{
	// Return image
	return m_cImageDisabled;
}

/**
*  @brief
*    Set image for disabled-state
*/
void BitmapButton::SetImageDisabled(const Image &cImage)
{
	// Set image
	m_cImageDisabled = cImage;
}


/**
*  @brief
*    Get image for selected-state
*/
const Image &BitmapButton::GetImageSelected() const
{
	// Return image
	return m_cImageSelected;
}

/**
*  @brief
*    Set image for selected-state
*/
void BitmapButton::SetImageSelected(const Image &cImage)
{
	// Set image
	m_cImageSelected = cImage;
}


/**
*  @brief
*    Get image for pressed-state
*/
const Image &BitmapButton::GetImagePressed() const
{
	// Return image
	return m_cImagePressed;
}

/**
*  @brief
*    Set image for pressed-state
*/
void BitmapButton::SetImagePressed(const Image &cImage)
{
	// Set image
	m_cImagePressed = cImage;
}



//[-------------------------------------------------------]
//[ Public virtual Widget functions                       ]
//[-------------------------------------------------------]
Vector2i BitmapButton::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Call base function
	AbstractButton::OnPreferredSize(vRefSize);

	// Return image size
	return m_cImage.GetSize();
}

void BitmapButton::OnDraw(Graphics &cGraphics)
{
	// Call base implementation
	AbstractButton::OnDraw(cGraphics);

	// Draw button
	Image *pImage = nullptr;
		 if (!IsEnabled())	pImage = &m_cImageDisabled;
	else if (m_bPressed)	pImage = &m_cImagePressed;
	else if (m_bMouseIn)	pImage = &m_cImageSelected;
	else					pImage = &m_cImageEnabled;
	cGraphics.DrawImage(*pImage, Vector2i::Zero, pImage->GetSize() - Vector2i(1, 1));
}

void BitmapButton::OnMouseEnter()
{
	// Call base function
	AbstractButton::OnMouseEnter();

	// Redraw button
	Redraw();
}

void BitmapButton::OnMouseLeave()
{
	// Call base function
	AbstractButton::OnMouseLeave();

	// Redraw button
	Redraw();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
