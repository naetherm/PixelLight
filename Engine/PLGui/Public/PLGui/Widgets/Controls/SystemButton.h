/*********************************************************\
 *  File: SystemButton.h                                 *
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


#ifndef __PLGUI_SYSTEMBUTTON_H__
#define __PLGUI_SYSTEMBUTTON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Widgets/Base/AbstractButton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Timer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Button that represents a system button in a title bar (minimize, maximize, ...)
*/
class SystemButton: public AbstractButton {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, SystemButton, "PLGui", PLGui::AbstractButton, "Button that represents a system button in a title bar (minimize, maximize, ...)")
		// Attributes
		pl_attribute(SystemCommand,	pl_enum_type(ESystemCommand),	CommandRestore,	ReadWrite,	GetSet,	"System command that is emitted by the button",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Pointer to the parent widget
		*/
		PLGUI_API SystemButton(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~SystemButton();

		/**
		*  @brief
		*    Get widget state
		*
		*  @return
		*    Widget states as flags (see EWidgetState)
		*/
		PLGUI_API virtual PLCore::uint32 GetWidgetState() const;

		/**
		*  @brief
		*    Get system command that the button represents
		*
		*  @return
		*    System command
		*/
		PLGUI_API ESystemCommand GetSystemCommand() const;

		/**
		*  @brief
		*    Set system command that the button represents
		*
		*  @param[in] nSystemCommand
		*    System command
		*/
		PLGUI_API void SetSystemCommand(ESystemCommand nSystemCommand);


	//[-------------------------------------------------------]
	//[ Private virtual AbstractButton functions              ]
	//[-------------------------------------------------------]
	private:
		PLGUI_API virtual void OnButtonClicked() override;


	//[-------------------------------------------------------]
	//[ Private virtual WidgetFunctions functions             ]
	//[-------------------------------------------------------]
	private:
		PLGUI_API virtual void OnDraw(Graphics &cGraphics) override;
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const override;
		PLGUI_API virtual void OnMouseEnter() override;
		PLGUI_API virtual void OnMouseLeave() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ESystemCommand m_nSystemCommand;	/**< System command that is emitted by the button */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_SYSTEMBUTTON_H__
