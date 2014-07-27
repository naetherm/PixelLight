/*********************************************************\
 *  File: Rtti.h                                         *
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


#ifndef __PLCORE_REFL_TYPEID_H__
#define __PLCORE_REFL_TYPEID_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Typebase/RawType.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRefl {

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Specialization of StaticTypeId for classes without defined reflection
*
*  @remarks
*    This will throw a compile time error if you try to retrieve reflection type Id
*    of a non-registered type
*/
template <typename T>
struct StaticTypeId
{
	static const char *Get()
	{
		// This will create at least somehow verbose compile-time error
		return T::TYPE_NOT_REGISTERED_IN_REFLECTION();
	}

	enum { Defined = false, Copyable = true };
};

/**
*  @brief
*    Check if the given type has been registered with reflection
*/
template <typename T>
struct HasStaticTypeId
{
	enum Value
	{
		Value = StaticTypeId<PLCore::RawType<T>>::Defined;
	};
};

//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Retirieve the static type Id of the given type
*/
template <typename T>
const char *GetStaticTypeId()
{
	return StaticTypeId<typename PLCore::RawType<T>::Type>::Get();
}

/**
*  @brief
*    Retrieve the static type Id from the specified instance
*/
template <typename T>
const char *GetStaticTypeId(const T&)
{
	return StaticTypeId<typename PLCore::RawType<T>::Type>::Get();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl


#endif // __PLCORE_REFL_RTTI_H__
