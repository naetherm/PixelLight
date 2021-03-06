/*********************************************************\
 *  File: ModTrapMouse.cpp                               *
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
#include <PLGui/Widgets/Widget.h>
#include "PLFrontendPLGui/ModTrapMouse.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGui;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_class_metadata(ModTrapMouse, "PLFrontendPLGui", PLGui::Modifier, "Modifier which traps the mouse within the widget as soon as it gets active")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(ModTrapMouse)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ModTrapMouse::ModTrapMouse()
{
}

/**
*  @brief
*    Destructor
*/
ModTrapMouse::~ModTrapMouse()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::Modifier functions           ]
//[-------------------------------------------------------]
void ModTrapMouse::OnAttach(Widget &cWidget)
{
}

void ModTrapMouse::OnDetach(Widget &cWidget)
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::WidgetFunctions functions    ]
//[-------------------------------------------------------]
void ModTrapMouse::OnActivate(bool bActivate)
{
	if (GetWidget())
		GetWidget()->GetContentWidget()->SetTrapMouse(bActivate);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
