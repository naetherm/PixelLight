/*********************************************************\
 *  File: Button.h                                       *
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


#ifndef __PLINPUT_BUTTON_H__
#define __PLINPUT_BUTTON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Input/Controls/Control.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Button control
*/
class Button : public Control {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, Button, "PLInput", PLInput::Control, "Button control")
		#ifdef PLINPUT_EXPORTS	// The following is only required when compiling PLInput
			// Methods
			pl_method_0(GetCharacter,	pl_ret_type(char),			"Returns the character associated with the button, '\0' if none.",																				"")
			pl_method_0(IsPressed,		pl_ret_type(bool),			"Returns 'true', if the button is currently pressed, else 'false'.",																			"")
			pl_method_1(SetPressed,		pl_ret_type(void),	bool,	"Set button status. 'true' as first parameter, if the button is pressed, else 'false'.",														"")
			pl_method_0(IsHit,			pl_ret_type(bool),			"Returns 'true', if the button has been hit since the last call of this function, else 'false'.",												"")
			pl_method_0(CheckHit,		pl_ret_type(bool),			"Check if the button has been hit. 'true' as first parameter, if the button has been hit since the last call of this function, else 'false'.",	"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pController
		*    Owning controller, can, but shouldn't be a null pointer
		*  @param[in] sName
		*    Control name
		*  @param[in] sDescription
		*    Control description
		*  @param[in] nCharacter
		*    Character associated with the button, '\0' if none
		*/
		PLINPUT_API Button(Controller *pController, const PLCore::String &sName, const PLCore::String &sDescription, char nCharacter = '\0');

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cOther
		*    Other button
		*/
		PLINPUT_API Button(const Button &cOther);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Button();

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cOther
		*    Button to compare with
		*
		*  @return
		*    'true', if both buttons are equal, else 'false'
		*/
		PLINPUT_API bool operator ==(const Button &cOther) const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cOther
		*    Other button
		*
		*  @return
		*    Reference to this button
		*/
		PLINPUT_API Button &operator =(const Button &cOther);

		/**
		*  @brief
		*    Get character
		*
		*  @return
		*    Character associated with the button, '\0' if none
		*/
		PLINPUT_API char GetCharacter() const;

		/**
		*  @brief
		*    Get button status
		*
		*  @return
		*    'true', if the button is currently pressed, else 'false'
		*/
		PLINPUT_API bool IsPressed() const;

		/**
		*  @brief
		*    Set button status
		*
		*  @param[in] bPressed
		*    'true', if the button is pressed, else 'false'
		*/
		PLINPUT_API void SetPressed(bool bPressed);

		/**
		*  @brief
		*    Check if the button has been hit
		*
		*  @return
		*    'true', if the button has been hit since the last call of this function, else 'false'
		*
		*  @remarks
		*    This will return the hit-state of the button and also reset it immediately (so the next call
		*    to IsHit() will return false). If you only want to check, but not reset the hit-state of
		*    a button, you should call CheckHit().
		*/
		PLINPUT_API bool IsHit();

		/**
		*  @brief
		*    Check if the button has been hit
		*
		*  @return
		*    'true', if the button has been hit since the last call of this function, else 'false'
		*
		*  @remarks
		*    This function will NOT reset the hit-state after being called (see IsHit()).
		*/
		PLINPUT_API bool CheckHit();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Button info
		char m_nCharacter;	/**< Character associated with the button, '\0' if none */

		// Button status
		bool m_bPressed;	/**< Is the button currently pressed? */
		bool m_bHit;		/**< Has the button been hit in the meantime? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_BUTTON_H__
