/*********************************************************\
 *  File: Box.cpp                                        *
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
#include "PLGui/Widgets/Layout/Box.h"
#include "PLGui/Layout/LayoutBox.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Box)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Box::Box(Widget *pParent, EOrientation nOrientation) : Widget(pParent),
	Orientation(this),
	m_pBoxLayout(nullptr)
{
	// Create layout
	m_pBoxLayout = new LayoutBox();
	m_pBoxLayout->SetOrientation(nOrientation);

	// Set layout
	SetLayout(m_pBoxLayout);

	// Give focus to child windows
	SetFocusStyle(ChildFocus);
}

/**
*  @brief
*    Destructor
*/
Box::~Box()
{
}

/**
*  @brief
*    Get orientation of box
*/
EOrientation Box::GetOrientation() const
{
	// Return orientation
	return m_pBoxLayout->GetOrientation();
}

/**
*  @brief
*    Set orientation of box
*/
void Box::SetOrientation(EOrientation nOrientation)
{
	// Set orientation
	m_pBoxLayout->SetOrientation(nOrientation);
}


//[-------------------------------------------------------]
//[ Private virtual Widget functions                      ]
//[-------------------------------------------------------]
PLMath::Vector2i Box::OnPreferredSize(const Vector2i &vRefSize) const
{
	// Call base function
	Widget::OnPreferredSize(vRefSize);

	// [TODO] Calculate inner size of layout
	return Vector2i(-1, -1);
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
