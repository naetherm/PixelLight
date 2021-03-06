/*********************************************************\
 *  File: GuiButton.h                                    *
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
//[ Preprocessor                                          ]
//[-------------------------------------------------------]
#ifndef __PLSAMPLE_66_GUIBUTTON_H__
#define __PLSAMPLE_66_GUIBUTTON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/SmartPtr.h>
#include "GuiBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGui {
	class Font;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ingame Gui - Button
*/
class GuiButton : public GuiBase {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Signals
		pl_signal_1_def(SignalPressed,	PLCore::uint32)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Text alignment
		*/
		enum ETextAlign {
			AlignLeft,
			AlignRight,
			AlignCenter,
			AlignBlock
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Parent widget, can be a null pointer
		*/
		GuiButton(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Get text
		*
		*  @return
		*    Text displayed in the item
		*/
		PLCore::String GetText() const;

		/**
		*  @brief
		*    Set text
		*
		*  @param[in] sText
		*    Text displayed in the item
		*/
		void SetText(const PLCore::String &sText);

		/**
		*  @brief
		*    Get font
		*
		*  @return
		*    Used font
		*/
		PLGui::Font *GetFont() const;

		/**
		*  @brief
		*    Set font
		*
		*  @param[in] pFont
		*    Used font
		*/
		void SetFont(PLGui::Font *pFont);

		/**
		*  @brief
		*    Get text color
		*
		*  @return
		*    Text color
		*/
		PLGraphics::Color4 GetColor() const;

		/**
		*  @brief
		*    Set text color
		*
		*  @param[in] cColor
		*    Text color
		*/
		void SetColor(const PLGraphics::Color4 &cColor);

		/**
		*  @brief
		*    Get mouse over text color
		*
		*  @return
		*    Mouse over text color
		*/
		PLGraphics::Color4 GetMouseOverColor() const;

		/**
		*  @brief
		*    Set mouse over text color
		*
		*  @param[in] cColor
		*    Mouse over text color
		*/
		void SetMouseOverColor(const PLGraphics::Color4 &cColor);

		/**
		*  @brief
		*    Get text alignment
		*
		*  @return
		*    Text alignment
		*/
		ETextAlign GetAlign() const;

		/**
		*  @brief
		*    Set text alignment
		*
		*  @param[in] nAlign
		*    Text alignment
		*/
		void SetAlign(ETextAlign nAlign);

		/**
		*  @brief
		*    Get background image filename
		*
		*  @return
		*    Background image filename
		*/
		PLCore::String GetBackgroundImageFilename() const;

		/**
		*  @brief
		*    Set background image filename
		*
		*  @param[in] sFilename
		*    Background image filename
		*/
		void SetBackgroundImageFilename(const PLCore::String &sFilename);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GuiButton();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String	 	 m_sText;						/**< Displayed text */
		PLGui::Font			*m_pFont;						/**< Selected font */
		PLGraphics::Color4	 m_cColor;						/**< Text color */
		PLGraphics::Color4	 m_cMouseOverColor;				/**< Mouse over text color */
		ETextAlign			 m_nAlign;						/**< Text alignment */
		bool				 m_bMouseOver;					/**< Is the mouse button currently over this widget? */
		bool				 m_bMouseDown;					/**< Is the mouse button currently down? */
		PLCore::String	 	 m_sBackgroundImageFilename;	/**< Background image filename */
		// Images
		PLCore::SmartPtr<PLGui::Image> m_cBackground;	/**< Background image */


	//[-------------------------------------------------------]
	//[ Public virtual PLGui::Widget functions                ]
	//[-------------------------------------------------------]
	public:
		virtual void OnDraw(PLGui::Graphics &cGraphics) override;
		virtual void OnMouseMove(const PLMath::Vector2i &vPos) override;
		virtual void OnMouseEnter() override;
		virtual void OnMouseLeave() override;
		virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;
		virtual void OnMouseButtonUp(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;


};


#endif // __PLSAMPLE_66_GUIBUTTON_H__
