/*********************************************************\
 *  File: MessageBox.h                                   *
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


#ifndef __PLGUI_MESSAGEBOX_H__
#define __PLGUI_MESSAGEBOX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLCore/Base/Event/EventHandler.h>
#include "PLGui/Widgets/Windows/Dialog.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Label;
class Button;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Message box
*/
class MessageBox : public Dialog {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, MessageBox, "PLGui", PLGui::Dialog, "Message box")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
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
		*    Pointer to parent widget
		*  @param[in] nType
		*    Message box type
		*  @param[in] sTitle
		*    Message box title
		*  @param[in] sText
		*    Message box text
		*/
		PLGUI_API MessageBox(Widget *pParent = nullptr, EMessageBox nType = MessageBoxOk, const PLCore::String &sTitle = "Message Box", const PLCore::String &sText = "Message");

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nType
		*    Message box type
		*  @param[in] sTitle
		*    Message box title
		*  @param[in] sText
		*    Message box text
		*/
		PLGUI_API MessageBox(EMessageBox nType, const PLCore::String &sTitle, const PLCore::String &sText);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~MessageBox();

		/**
		*  @brief
		*    Get message box type
		*
		*  @return
		*    Type of message box
		*/
		PLGUI_API EMessageBox GetType() const;

		/**
		*  @brief
		*    Get text
		*
		*  @return
		*    Text
		*/
		PLGUI_API PLCore::String GetText() const;

		/**
		*  @brief
		*    Set text
		*
		*  @param[in] sText
		*    Text
		*/
		PLGUI_API void SetText(const PLCore::String &sText);

		/**
		*  @brief
		*    Get button text
		*
		*  @param[in] nButton
		*    Button ID
		*
		*  @return
		*    Text
		*/
		PLGUI_API PLCore::String GetButtonText(EMessageBoxButton nButton) const;

		/**
		*  @brief
		*    Set button text
		*
		*  @param[in] nButton
		*    Button ID
		*  @param[in] sText
		*    Text
		*/
		PLGUI_API void SetButtonText(EMessageBoxButton nButton, const PLCore::String &sText);


	//[-------------------------------------------------------]
	//[ Private virtual Dialog functions                      ]
	//[-------------------------------------------------------]
	private:
		PLGUI_API virtual void OnCloseDialog(int nResult);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create message box
		*
		*  @param[in] nType
		*    Message box type
		*  @param[in] sTitle
		*    Message box title
		*  @param[in] sText
		*    Message box text
		*/
		void CreateMessageBox(EMessageBox nType, const PLCore::String &sTitle, const PLCore::String &sText);

		/**
		*  @brief
		*    Button 'Ok' has been clicked
		*/
		void OnButtonOk();

		/**
		*  @brief
		*    Button 'Cancel' has been clicked
		*/
		void OnButtonCancel();

		/**
		*  @brief
		*    Button 'Yes' has been clicked
		*/
		void OnButtonYes();

		/**
		*  @brief
		*    Button 'No' has been clicked
		*/
		void OnButtonNo();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Event handlers
		PLCore::EventHandler<>	EventHandlerOk;
		PLCore::EventHandler<>	EventHandlerCancel;
		PLCore::EventHandler<>	EventHandlerYes;
		PLCore::EventHandler<>	EventHandlerNo;

		// Data
		EMessageBox			 m_nType;			/**< Message box type */
		PLCore::String		 m_sText;			/**< Message box text */

		// Widgets
		Label				*m_pLabel;			/**< Text label */
		Box					*m_pBox;			/**< Button box */
		Button				*m_pButtonOk;		/**< Button 'Ok' */
		Button				*m_pButtonCancel;	/**< Button 'Cancel' */
		Button				*m_pButtonYes;		/**< Button 'Yes' */
		Button				*m_pButtonNo;		/**< Button 'No' */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MESSAGEBOX_H__
