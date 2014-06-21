/*********************************************************\
 *  File: BitmapButton.h                                 *
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


#ifndef __PLGUI_BITMAPBUTTON_H__
#define __PLGUI_BITMAPBUTTON_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Gui/Resources/Image.h"
#include "PLGui/Widgets/Base/AbstractButton.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A button that displays images for each state
*/
class BitmapButton : public AbstractButton {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, BitmapButton, "PLGui", PLGui::AbstractButton, "A button that displays images for each state")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// [TODO] Images...
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
		PLGUI_API BitmapButton(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~BitmapButton();

		/**
		*  @brief
		*    Get image for enabled-state
		*
		*  @return
		*    Image that is displayed when the button is enabled
		*/
		PLGUI_API const Image &GetImageEnabled() const;

		/**
		*  @brief
		*    Set image for enabled-state
		*
		*  @param[in] cImage
		*    Image that is displayed when the button is enabled
		*/
		PLGUI_API void SetImageEnabled(const Image &cImage);

		/**
		*  @brief
		*    Get image for disabled-state
		*
		*  @return
		*    Image that is displayed when the button is disabled
		*/
		PLGUI_API const Image &GetImageDisabled() const;

		/**
		*  @brief
		*    Set image for disabled-state
		*
		*  @param[in] cImage
		*    Image that is displayed when the button is disabled
		*/
		PLGUI_API void SetImageDisabled(const Image &cImage);

		/**
		*  @brief
		*    Get image for selected-state
		*
		*  @return
		*    Image that is displayed when the button is selected
		*/
		PLGUI_API const Image &GetImageSelected() const;

		/**
		*  @brief
		*    Set image for selected-state
		*
		*  @param[in] cImage
		*    Image that is displayed when the button is selected
		*/
		PLGUI_API void SetImageSelected(const Image &cImage);

		/**
		*  @brief
		*    Get image for pressed-state
		*
		*  @return
		*    Image that is displayed when the button is pressed
		*/
		PLGUI_API const Image &GetImagePressed() const;

		/**
		*  @brief
		*    Set image for pressed-state
		*
		*  @param[in] cImage
		*    Image that is displayed when the button is pressed
		*/
		PLGUI_API void SetImagePressed(const Image &cImage);


	//[-------------------------------------------------------]
	//[ Private virtual Widget functions                      ]
	//[-------------------------------------------------------]
	private:
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const override;
		PLGUI_API virtual void OnDraw(Graphics &cGraphics) override;
		PLGUI_API virtual void OnMouseEnter() override;
		PLGUI_API virtual void OnMouseLeave() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Image m_cImageEnabled;	/**< Image that is displayed when the button is enabled */
		Image m_cImageDisabled;	/**< Image that is displayed when the button is disabled */
		Image m_cImageSelected;	/**< Image that is displayed when the button is selected by the mouse */
		Image m_cImagePressed;	/**< Image that is displayed when the button is pressed */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_BITMAPBUTTON_H__
