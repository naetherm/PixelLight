/*********************************************************\
 *  File: MyRTTIClass.h                                  *
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


#ifndef __PLSAMPLE_44_SCRIPTOOP_MYRTTICLASS_H__
#define __PLSAMPLE_44_SCRIPTOOP_MYRTTICLASS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sample RTTI class, don't take it to serious
*/
class MyRTTIClass : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Attributes
		pl_attribute_getset		(MyRTTIClass,	Name,	PLCore::String,		"Bob",	ReadWrite)
		pl_attribute_directvalue(				Level,	int,				1,		ReadWrite)
		// Signals
		pl_signal_1_def(MySignal,	PLCore::String)
		// Slots
		pl_slot_1_def(MyRTTIClass,	OnMyEvent,	PLCore::String)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::String GetName();
		void SetName(const PLCore::String &sName);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MyRTTIClass();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MyRTTIClass();

		/**
		*  @brief
		*    Returns 42
		*
		*  @return
		*    42
		*/
		int Return42() const;

		/**
		*  @brief
		*    Ignores the provided parameter
		*
		*  @param[in] fValue
		*    Parameter to ignore
		*/
		void IgnoreTheParameter(float fValue) const;

		/**
		*  @brief
		*    Says something wise
		*/
		void SaySomethingWise() const;

		/**
		*  @brief
		*    Returns a pointer to this instance
		*
		*  @return
		*    A pointer to this instance, always valid!
		*/
		MyRTTIClass *GetSelf();

		/**
		*  @brief
		*    Called when my event was emitted
		*
		*  @param[in] sMessage
		*    Message
		*/
		void OnMyEvent(PLCore::String sMessage);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sName;


};


#endif // __PLSAMPLE_44_SCRIPTOOP_MYRTTICLASS_H__
