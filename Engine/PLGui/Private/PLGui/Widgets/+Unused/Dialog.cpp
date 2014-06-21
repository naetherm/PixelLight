/*********************************************************\
 *  File: Dialog.cpp                                     *
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
#include "PLGui/Widgets/Windows/Dialog.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Dialog)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Dialog::Dialog(Widget *pParent) : Window(pParent),
	m_nResult(0)
{
	// Set title
	SetTitle("Dialog Box");

	// Dialogs are usually not resizable
	SetResizable(false);

	// Dialogs only have the 'Close' button
	SetSystemButtons(SystemButtonClose);
}

/**
*  @brief
*    Destructor
*/
Dialog::~Dialog()
{
}

/**
*  @brief
*    Show dialog
*/
void Dialog::ShowDialog()
{
	// Emit event
	EventShowDialog.Emit();

	// Make dialog visible
	SetVisible(true);
}

/**
*  @brief
*    Close dialog
*/
void Dialog::CloseDialog(int nResult)
{
	// Set result value
	m_nResult = nResult;

	// Call virtual function
	OnCloseDialog(m_nResult);
}

/**
*  @brief
*    Get result value
*/
int Dialog::GetResult() const
{
	// Return result value
	return m_nResult;
}


//[-------------------------------------------------------]
//[ Protected virtual Widget functions                    ]
//[-------------------------------------------------------]
void Dialog::OnClose()
{
	// Call base implementation
	ContainerWidget::OnClose();

	// Close dialog
	CloseDialog(-1);
}


//[-------------------------------------------------------]
//[ Public virtual Dialog functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when dialog has been closed
*/
void Dialog::OnCloseDialog(int nResult)
{
	// Emit event
	EventCloseDialog.Emit(m_nResult);

	// Close dialog
	Destroy();
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
