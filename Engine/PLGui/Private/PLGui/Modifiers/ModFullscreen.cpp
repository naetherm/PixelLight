/*********************************************************\
 *  File: ModFullscreen.cpp                              *
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
#include "PLGui/Gui/Base/Keys.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Modifiers/ModFullscreen.h"


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(ModFullscreen)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ModFullscreen::ModFullscreen()
{
}

/**
*  @brief
*    Destructor
*/
ModFullscreen::~ModFullscreen()
{
}


//[-------------------------------------------------------]
//[ Protected virtual Modifier functions                  ]
//[-------------------------------------------------------]
void ModFullscreen::OnAttach(Widget &cWidget)
{
}

void ModFullscreen::OnDetach(Widget &cWidget)
{
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
void ModFullscreen::OnKeyDown(uint32 nKey, uint32 nModifiers)
{
	// ALT+ENTER?
	if (nKey == PLGUIKEY_RETURN && (nModifiers & PLGUIMOD_ALT) != 0) {
		// Toggle fullscreen mode
		if (m_pWidget->GetWindowState() != StateFullscreen)
			m_pWidget->SetWindowState(StateFullscreen);
		else
			m_pWidget->SetWindowState(StateNormal);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
