/*********************************************************\
 *  File: ModTooltip.cpp                                 *
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
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Modifiers/ModTooltip.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
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
pl_implement_class(ModTooltip)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ModTooltip::ModTooltip() :
	Tooltip(this)
{
}

/**
*  @brief
*    Destructor
*/
ModTooltip::~ModTooltip()
{
}

/**
*  @brief
*    Get tooltip text
*/
String ModTooltip::GetTooltip() const
{
	// Return tooltip text
	return m_sTooltip;
}

/**
*  @brief
*    Set tooltip text
*/
void ModTooltip::SetTooltip(const String &sTooltip)
{
	// Set tooltip text
	m_sTooltip = sTooltip;
}


//[-------------------------------------------------------]
//[ Protected virtual ModTooltip functions                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a tooltip shall be displayed
*/
void ModTooltip::OnShowTooltip()
{
	// Set tooltip position
	Vector2i vTooltipPos = m_pWidget->GetAbsPos() + Vector2i(16, 20);

	// Display tooltip
	m_pWidget->GetGui()->ShowTooltip(vTooltipPos, m_sTooltip);
}


//[-------------------------------------------------------]
//[ Protected virtual Modifier functions                  ]
//[-------------------------------------------------------]
void ModTooltip::OnAttach(Widget &cWidget)
{
}

void ModTooltip::OnDetach(Widget &cWidget)
{
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
void ModTooltip::OnMouseHover()
{
	// Show tooltip
	OnShowTooltip();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
