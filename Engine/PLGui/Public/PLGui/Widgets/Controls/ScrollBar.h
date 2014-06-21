/*********************************************************\
 *  File: ScrollBar.h                                    *
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


#ifndef __PLGUI_SCROLLBAR_H__
#define __PLGUI_SCROLLBAR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2i.h>
#include "PLGui/Gui/Resources/Timer.h"
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
*    Scroll bar widget
*/
class ScrollBar : public AbstractSlider {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, ScrollBar, "PLGui", PLGui::AbstractSlider, "Scroll bar widget")
		// Attributes
		pl_attribute(Orientation,	pl_enum_type(EOrientation),	Horizontal,	ReadWrite,	GetSet,	"Scrollbar orientation",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Slots
		pl_slot_0(OnTimer,	"Timer callback",	"")
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
		PLGUI_API ScrollBar(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ScrollBar();

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
		*    Calculate position of minus button
		*/
		void CalculateMinusButtonPos();

		/**
		*  @brief
		*    Calculate position of plus button
		*/
		void CalculatePlusButtonPos();

		/**
		*  @brief
		*    Calculate position of scrollbar handle
		*/
		void CalculateHandlePos();

		/**
		*  @brief
		*    Get widget state for the minus button
		*
		*  @return
		*    Widget state of minus button (combination of EWidgetState values)
		*/
		PLCore::uint32 GetMinusButtonState();

		/**
		*  @brief
		*    Get widget state for the plus button
		*
		*  @return
		*    Widget state of plus button (combination of EWidgetState values)
		*/
		PLCore::uint32 GetPlusButtonState();

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
		*  @param[in] bSlider
		*    Allow selection of slider?
		*/
		void CheckButtons(const PLMath::Vector2i &vPos, bool bSlider);

		/**
		*  @brief
		*    Called when the timer has fired
		*/
		void OnTimer();


	//[-------------------------------------------------------]
	//[ Protected data types                                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Scroll bar button
		*/
		struct SScrollBarButton {
			PLMath::Vector2i vPos1;
			PLMath::Vector2i vPos2;
			bool			 bMouseOver;
			bool			 bPressed;
		};

		/**
		*  @brief
		*    Scroll bar slider
		*/
		struct SScrollBarSlider {
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
		PLCore::uint64		m_nRepeatInterval;	/**< Interval (in ms) between repeated clicks */

		// Internal data
		SScrollBarButton	m_sButtonMinus;		/**< Button 'minus' */
		SScrollBarButton	m_sButtonPlus;		/**< Button 'plus' */
		SScrollBarSlider	m_sHandle;			/**< Handle */
		PLMath::Vector2i	m_vMousePos;		/**< Last mouse position */
		Timer				m_cTimer;			/**< Timer for repeated clicks */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_SCROLLBAR_H__
