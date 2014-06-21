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


#ifndef __PLCORE_DEFAULTVALUE_H__
#define __PLCORE_DEFAULTVALUE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Base/Tools/TypeTraits.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
// Enum type
template <typename T>
class EnumType;

// Flag type
template <typename T>
class FlagType;

template <typename T>
struct CheckType;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template <typename T>
class DefaultValue : public DefaultValue< typename CheckType<T>::Type > {
};

template <>
class DefaultValue<bool> {
	public:
		static bool Default() {
			return false;
		}
};

/*
template <>
class DefaultValue<int> {
	public:
		static int Default() {
			return 0;
		}
};
*/

template <>
class DefaultValue<int8> {
	public:
		static int8 Default() {
			return 0;
		}
};

template <>
class DefaultValue<int16> {
	public:
		static int16 Default() {
			return 0;
		}
};

template <>
class DefaultValue<int32> {
	public:
		static int32 Default() {
			return 0;
		}
};

template <>
class DefaultValue<int64> {
	public:
		static int64 Default() {
			return 0;
		}
};

template <>
class DefaultValue<uint8> {
	public:
		static uint8 Default() {
			return 0;
		}
};

template <>
class DefaultValue<uint16> {
	public:
		static uint16 Default() {
			return 0;
		}
};

template <>
class DefaultValue<uint32> {
	public:
		static uint32 Default() {
			return 0;
		}
};

template <>
class DefaultValue<uint64> {
	public:
		static uint64 Default() {
			return 0;
		}
};

template <>
class DefaultValue<float> {
	public:
		static float Default() {
			return 0.0f;
		}
};

template <>
class DefaultValue<double> {
	public:
		static double Default() {
			return 0.0;
		}
};

template <>
class DefaultValue<String> {
	public:
		static String Default() {
			return "";
		}
};

template <typename T>
class DefaultValue<T*> {
	public:
		static T* Default() {
			return nullptr;
		}
};

template <typename T>
class DefaultValue<T&> {
	public:
		static T& Default() {
			return *static_cast<T*>(nullptr);
		}
};

template <typename ENUM>
class DefaultValue< EnumTypePlain<ENUM> > {
	public:
		static ENUM Default() {
			return static_cast<ENUM>(0);
		}
};

template <typename ENUM>
class DefaultValue< EnumType<ENUM> > {
	public:
		// Type
		typedef typename ENUM::_Type _Type;

		static _Type Default() {
			return DefaultValue<_Type>::Default();
		}
};

template <typename ENUM>
class DefaultValue< FlagType<ENUM> > {
	public:
		// Type
		typedef typename ENUM::_Type _Type;

		static _Type Default() {
			return DefaultValue<_Type>::Default();
		}
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_DEFAULTVALUE_H__
