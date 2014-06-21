/*********************************************************\
 *  File: AbstractToggleButton.h                         *
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


#ifndef __PLGUI_ABSTRACTTOGGLEBUTTON_H__
#define __PLGUI_ABSTRACTTOGGLEBUTTON_H__
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
class ButtonGroup;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for toggle buttons (two or three states)
*/
class AbstractToggleButton : public AbstractButton {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, AbstractToggleButton, "PLGui", PLGui::AbstractButton, "Abstract base class for toggle buttons (two or three states)")
		// Attributes
		pl_attribute(PartiallyAllowed,	bool,						false,		ReadWrite,	GetSet,	"Is the third state (PartiallyChecked) allowed?",	"")
		pl_attribute(State,				pl_enum_type(ECheckState),	NotChecked,	ReadWrite,	GetSet,	"Current check state",								"")
		// Signals
		pl_signal_1(SignalToggled,	ECheckState,	"The button has been toggled",	"")
		// Slots
		pl_slot_1(OnGroupActivate,	AbstractToggleButton*,	"Group callback",	"")
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
		PLGUI_API AbstractToggleButton(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~AbstractToggleButton();

		/**
		*  @brief
		*    Get group that the toggle button belongs to
		*
		*  @return
		*    Pointer to button group (can be a null pointer)
		*/
		PLGUI_API ButtonGroup *GetGroup() const;

		/**
		*  @brief
		*    Set group that the toggle button belongs to
		*
		*  @param[in] pGroup
		*    Pointer to button group
		*/
		PLGUI_API void SetGroup(ButtonGroup *pGroup);

		/**
		*  @brief
		*    Check if the toggle button can be checked partially (third state)
		*
		*  @return
		*    'true', if the state PartiallyChecked is allowed, else 'false'
		*/
		PLGUI_API bool GetPartiallyAllowed() const;

		/**
		*  @brief
		*    Set if the toggle button can be checked partially (third state)
		*
		*  @param[in] bPartiallyAllowed
		*    'true', if the state PartiallyChecked is allowed, else 'false'
		*/
		PLGUI_API void SetPartiallyAllowed(bool bPartiallyAllowed);

		/**
		*  @brief
		*    Get checked-state of button
		*
		*  @return
		*    Check state
		*/
		PLGUI_API ECheckState GetState() const;

		/**
		*  @brief
		*    Set checked-state of button
		*
		*  @param[in] nState
		*    Check state
		*
		*  @remarks
		*    This function also emits a checked-event when called.
		*/
		PLGUI_API void SetState(ECheckState nState);

		/**
		*  @brief
		*    Toggle button
		*
		*  @remarks
		*    Toggles the state of the button in the following way:
		*      NotChecked     -> Checked
		*      PartialChecked -> Checked
		*      Checked        -> NotChecked
		*    This function also emits a checked-event when called.
		*/
		PLGUI_API void Toggle();


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractToggleButton functions      ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnButtonToggled(ECheckState nChecked);


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractButton functions            ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnButtonClicked() override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    This function is called when a button of the group is activated
		*
		*  @param[in] pButton
		*    Pointer to button that is being activated
		*/
		void OnGroupActivate(AbstractToggleButton *pButton);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		ButtonGroup	*m_pGroup;				/**< Pointer to button group, can be a null pointer */
		bool		 m_bPartiallyAllowed;	/**< Is the third state (PartiallyChecked) allowed? */
		ECheckState  m_nState;				/**< Current check state */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_ABSTRACTTOGGLEBUTTON_H__
