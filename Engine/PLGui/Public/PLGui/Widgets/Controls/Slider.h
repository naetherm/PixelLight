/*********************************************************\
 *  File: Slider.h                                       *
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


#ifndef __PLGUI_SLIDER_H__
#define __PLGUI_SLIDER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include "PLGui/Widgets/Base/AbstractSlider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Slider widget
*/
class Slider : public AbstractSlider {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, Slider, "PLGui", PLGui::AbstractSlider, "Slider widget")
		// Attributes
		pl_attribute(Orientation,	pl_enum_type(EOrientation),	Horizontal,	ReadWrite,	GetSet,	"Slider orientation",	"")
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
		PLGUI_API Slider(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Slider();

		/**
		*  @brief
		*    Get orientation
		*
		*  @return
		*    Orientation
		*/
		PLGUI_API EOrientation GetOrientation() const;

		/**
		*  @brief
		*    Set orientation
		*
		*  @param[in] nOrientation
		*    Orientation
		*/
		PLGUI_API void SetOrientation(EOrientation nOrientation);


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractSlider functions            ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnChangeValue(int nValue) override;


	//[-------------------------------------------------------]
	//[ Public virtual Widget functions                       ]
	//[-------------------------------------------------------]
	public:
		PLGUI_API virtual void OnDraw(Graphics &cGraphics) override;
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const override;
		PLGUI_API virtual void OnDisable() override;
		PLGUI_API virtual void OnMouseLeave() override;
		PLGUI_API virtual void OnMouseMove(const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonDown(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;
		PLGUI_API virtual void OnMouseButtonUp(PLCore::uint32 nButton, const PLMath::Vector2i &vPos) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Calculate position of slider handle
		*/
		void CalculateHandlePos();

		/**
		*  @brief
		*    Get widget state for the handle
		*
		*  @return
		*    Widget state of handle (combination of EWidgetState values)
		*/
		PLCore::uint32 GetHandleState();

		/**
		*  @brief
		*    Check if a button has been pressed
		*
		*  @param[in] vPos
		*    Mouse position
		*/
		void CheckButtons(const PLMath::Vector2i &vPos);


	//[-------------------------------------------------------]
	//[ Protected data types                                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Slider handle
		*/
		struct SSliderHandle {
			PLMath::Vector2i vPos1;
			PLMath::Vector2i vPos2;
			bool			 bMouseOver;
			bool			 bPressed;
			int				 nMinPos;
			int				 nMaxPos;
		};


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Options
		EOrientation		m_nOrientation;		/**< Scrollbar orientation */

		// Internal data
		SSliderHandle		m_sHandle;			/**< Handle */
		PLMath::Vector2i	m_vMousePos;		/**< Last mouse position */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_SLIDER_H__
