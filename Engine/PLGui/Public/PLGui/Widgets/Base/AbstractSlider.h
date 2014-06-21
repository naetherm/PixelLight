/*********************************************************\
 *  File: AbstractSlider.h                               *
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


#ifndef __PLGUI_ABSTRACTSLIDER_H__
#define __PLGUI_ABSTRACTSLIDER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Gui/Resources/Timer.h"
#include "PLGui/Widgets/Widget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for all kind of sliders, scrollbars etc.
*/
class AbstractSlider : public Widget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, AbstractSlider, "PLGui", PLGui::Widget, "Abstract base class for all kind of sliders, scrollbars etc.")
		// Attributes
		pl_attribute(MinValue,		int,	1,		ReadWrite,	GetSet,	"Minimum value",		"")
		pl_attribute(MaxValue,		int,	100,	ReadWrite,	GetSet,	"Maximum value",		"")
		pl_attribute(Value,			int,	1,		ReadWrite,	GetSet,	"Current value",		"")
		pl_attribute(StepSize,		int,	10,		ReadWrite,	GetSet,	"Standard step size",	"")
		pl_attribute(StepSizeFast,	int,	50,		ReadWrite,	GetSet,	"Fast step size",		"")
		// Signals
		pl_signal_1(SignalChangeValue,	int,	"The current value has been changed",	"")
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
		PLGUI_API AbstractSlider(Widget *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~AbstractSlider();

		/**
		*  @brief
		*    Get minimum value
		*
		*  @return
		*    Minimum value
		*/
		PLGUI_API int GetMinValue() const;

		/**
		*  @brief
		*    Set minimum value
		*
		*  @param[in] nMinValue
		*    Minimum value
		*/
		PLGUI_API void SetMinValue(int nMinValue);

		/**
		*  @brief
		*    Get maximum value
		*
		*  @return
		*    Maximum value
		*/
		PLGUI_API int GetMaxValue() const;

		/**
		*  @brief
		*    Set maximum value
		*
		*  @param[in] nMaxValue
		*    Maximum value
		*/
		PLGUI_API void SetMaxValue(int nMaxValue);

		/**
		*  @brief
		*    Get current value
		*
		*  @return
		*    Value
		*/
		PLGUI_API int GetValue() const;

		/**
		*  @brief
		*    Set current value
		*
		*  @param[in] nValue
		*    Value
		*/
		PLGUI_API void SetValue(int nValue);

		/**
		*  @brief
		*    Get step size (single step)
		*
		*  @return
		*    Step size
		*/
		PLGUI_API int GetStepSize() const;

		/**
		*  @brief
		*    Set step size (single step)
		*
		*  @param[in] nStepSize
		*    Step size
		*/
		PLGUI_API void SetStepSize(int nStepSize);

		/**
		*  @brief
		*    Get step size (fast step)
		*
		*  @return
		*    Step size
		*/
		PLGUI_API int GetStepSizeFast() const;

		/**
		*  @brief
		*    Set step size (fast step)
		*
		*  @param[in] nStepSize
		*    Step size
		*/
		PLGUI_API void SetStepSizeFast(int nStepSize);

		/**
		*  @brief
		*    Increase value (single step)
		*
		*  @remarks
		*    Uses the value set by SetStepSize()
		*/
		PLGUI_API void Increase();

		/**
		*  @brief
		*    Decrease value (single step)
		*
		*  @remarks
		*    Uses the value set by SetStepSize()
		*/
		PLGUI_API void Decrease();

		/**
		*  @brief
		*    Increase value (fast step)
		*
		*  @remarks
		*    Uses the value set by SetStepSizeFast()
		*/
		PLGUI_API void IncreaseFast();

		/**
		*  @brief
		*    Decrease value (fast step)
		*
		*  @remarks
		*    Uses the value set by SetStepSizeFast()
		*/
		PLGUI_API void DecreaseFast();


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractSlider functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a new value has been set
		*
		*  @param[in] nValue
		*    Value
		*/
		PLGUI_API virtual void OnChangeValue(int nValue);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Data
		int	m_nMinValue;		/**< Minimum value */
		int	m_nMaxValue;		/**< Maximum value */
		int	m_nValue;			/**< Current value */
		int	m_nStepSize;		/**< Step size for single steps */
		int m_nStepSizeFast;	/**< Step size for fast steps (e.g. page up/down) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_ABSTRACTSLIDER_H__
