/*********************************************************\
 *  File: Axis.h                                         *
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


#ifndef __PLINPUT_AXIS_H__
#define __PLINPUT_AXIS_H__
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
*    Axis control
*/
class Axis : public Control {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, Axis, "PLInput", PLInput::Control, "Axis control")
		#ifdef PLINPUT_EXPORTS	// The following is only required when compiling PLInput
			// Methods
			pl_method_0(GetValue,			pl_ret_type(float),					"Returns the axis value.",																																	"")
			pl_method_2(SetValue,			pl_ret_type(void),	float,	bool,	"Set axis value. Current value as first parameter. As second parameter 'true' if the current value is relative, else 'false' if it's a absolute value.",	"")
			pl_method_0(IsValueRelative,	pl_ret_type(bool),					"Returns 'true' if the current value is relative, else 'false' if it's a absolute value.",																	"")
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
		*/
		PLINPUT_API Axis(Controller *pController, const PLCore::String &sName, const PLCore::String &sDescription);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cOther
		*    Other axis
		*/
		PLINPUT_API Axis(const Axis &cOther);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Axis();

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cOther
		*    Axis to compare with
		*
		*  @return
		*    'true', if both axes are equal, else 'false'
		*/
		PLINPUT_API bool operator ==(const Axis &cOther) const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cOther
		*    Other axis
		*
		*  @return
		*    Reference to this axis
		*/
		PLINPUT_API Axis &operator =(const Axis &cOther);

		/**
		*  @brief
		*    Get axis value
		*
		*  @return
		*    Current value
		*
		*  @remarks
		*    Please note that a value can be absolute (for instance the x-axis of a joystick) or
		*    relative (for instance the x-axis of a mouse). While an absolute axis is usually timing
		*    independent, a relative axis just tells you about a state change since the last update.
		*    Therefore, we strongly recommend to always use "IsValueRelative()" to check for the value
		*    type in order to, for instance, multiply a absolute value with the current time difference
		*    since the last frame/update to get correctly timed movement.
		*/
		PLINPUT_API float GetValue() const;

		/**
		*  @brief
		*    Set axis value
		*
		*  @param[in] fValue
		*    Current value
		*  @param[in] bValueRelative
		*    'true' if the current value is relative, else 'false' if it's a absolute value
		*/
		PLINPUT_API void SetValue(float fValue, bool bValueRelative);

		/**
		*  @brief
		*    Return whether the current value is relative or absolute
		*
		*  @return
		*    'true' if the current value is relative, else 'false' if it's a absolute value
		*/
		PLINPUT_API bool IsValueRelative() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fValue;			/**< Value of the axis */
		bool  m_bValueRelative;	/**< Is the current value a relative one? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_AXIS_H__
