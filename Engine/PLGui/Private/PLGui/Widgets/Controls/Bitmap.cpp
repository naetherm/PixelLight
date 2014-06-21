/*********************************************************\
 *  File: Bitmap.cpp                                     *
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
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Gui/Resources/Graphics.h"
#include "PLGui/Widgets/Controls/Bitmap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Bitmap)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Bitmap::Bitmap(Widget *pParent) : Widget(pParent),
	ImageName(this),
	m_cImage(*GetGui())
{
	// Don't accept keyboard focus
	SetFocusStyle(NoFocus);
}

/**
*  @brief
*    Destructor
*/
Bitmap::~Bitmap()
{
}

/**
*  @brief
*    Get image
*/
const Image &Bitmap::GetImage() const
{
	// Return font
	return m_cImage;
}

/**
*  @brief
*    Set image
*/
void Bitmap::SetImage(const Image &cImage)
{
	// Set image
	m_cImage = cImage;

	// Update widget
	UpdateContent();

	// Redraw
	Redraw();
}

/**
*  @brief
*    Get image filename
*/
String Bitmap::GetImageName() const
{
	// Return image filename
	return m_cImage.GetFilename();
}

/**
*  @brief
*    Set image filename
*/
void Bitmap::SetImageName(const String &sImage)
{
	// Load image
	SetImage(Image(*GetGui(), sImage));
}


//[-------------------------------------------------------]
//[ Private virtual Widget functions                      ]
//[-------------------------------------------------------]
PLMath::Vector2i Bitmap::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Call base function
	Widget::OnPreferredSize(vRefSize);

	// Is a valid image set?
	if (!m_cImage.IsEmpty())
		// Give back image size
		return m_cImage.GetSize();
	else
		// Bitmap is empty
		return Vector2i::Zero;
}

void Bitmap::OnDraw(Graphics &cGraphics)
{
	// Call base implementation
	Widget::OnDraw(cGraphics);

	// Draw image
	// [TODO] PLGui: Was m_cImage.GetSize() ?!
	cGraphics.DrawImage(m_cImage, Vector2i::Zero, GetSize());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
