/*********************************************************\
 *  File: DefaultValue.h                                 *
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


#ifndef __PLCORE_DEFAULT_VALUE_H__
#define __PLCORE_DEFAULT_VALUE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "DynamicObject.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Base for reflected items able to hold some kind of "default value"
*/
class DefaultValue {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default ctor
		*/
		DefaultValue();

		/**
		*  @brief
		*    Value ctor
		*/
		DefaultValue(const DynamicObject &cValue);

		/**
		*  @brief
		*    Retrieve the default value
		*/
		inline const DynamicObject &GetDefaultValue() const;

		/**
		*  @brief
		*    Set the default value
		*/
		inline void SetDefaultValue(const DynamicObject &cValue);


	//[-------------------------------------------------------]
	//[ Public virtual DefaultValue functions                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Restore the default value on the object
		*
		*  @param cObject[in]
		*    The object to set the value on
		*/
		virtual void RestoreDefaultValue(const DynamicObject &cObject) const = 0;

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DynamicObject m_cValue;		/**< The default value holder */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "DefaultValue.inl"


#endif // __PLCORE_DEFAULT_VALUE_H__
