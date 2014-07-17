/*********************************************************\
 *  File: UntypedVariant                                 *
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


#ifndef __PLCORE_UNTYPEDVARIANT_H__
#define __PLCORE_UNTYPEDVARIANT_H__
#pragma once

#include "TypeTraits.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Generic holder of variant values
*
*  @remarks
*    This class is able to hold any value without knowing its type. Use it when
*    you need to pass around arbitrary values
*/
template <int STORAGE_SIZE = sizeof(int*)>
class UntypedVariant {

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default ctor
		*/
		UntypedVariant() {}

		/**
		*  @brief
		*    Typed constructor
		*/
		template <typename T>
		UntypedVariant(const T &cVal)
		{
			typedef typename RemoveConst<typename RemoveReference<T>::Type>::Type _Type;
			AccessHelper<_Type, IsBig<_Type>::Value>::Set(cVal, m_pData);
		}

		/**
		*  @brief
		*    Typed constructor
		*/
		//template <typename T>
		//UntypedVariant(const T &&cVal)
		//{
		//	AccessHelper<T, IsBig<T>::Value>::Set(static_cast<T&&>(cVal), m_pData);
		//}

		/**
		*  @brief
		*    Set the variant value
		*/
		template <typename T>
		void Set(const T &cVal)
		{
			typedef typename RemoveConst<typename RemoveReference<T>::Type>::Type _Type;
			AccessHelper<_Type, IsBig<_Type>::Value>::Set(cVal, m_pData);
		}

		/**
		*  @brief
		*    Get the stored value
		*/
		template <typename T>
		const typename RemoveReference<T>::Type &Get() const
		{
			typedef typename RemoveConst<typename RemoveReference<T>::Type>::Type _Type;
			return AccessHelper<_Type, IsBig<_Type>::Value>::Get(m_pData);
		}

		/**
		*  @brief
		*    Get the stored value
		*/
		template <typename T>
		typename RemoveConst<typename RemoveReference<T>::Type>::Type &Get()
		{
			typedef typename RemoveConst<typename RemoveReference<T>::Type>::Type _Type;
			return AccessHelper<_Type, IsBig<_Type>::Value>::Get(m_pData);
		}

		/**
		*  @brief
		*    Destroy the object stored in the variant
		*/
		template <typename T>
		void Destroy()
		{
			typedef typename RemoveConst<typename RemoveReference<T>::Type>::Type _Type;
			AccessHelper<_Type, IsBig<_Type>::Value>::Destroy(m_pData);
		}

		/**
		*  @brief
		*    Equality cannot be tested for untyped variant
		*/
		bool operator==(const UntypedVariant &cOther) const
		{
			return false;
		}

	//[-------------------------------------------------------]
	//[ Private types                                         ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Helper to detect types too big to fit into the internal storage buffer
		*/
		template <typename T>
		struct IsBig {

			static const bool Value = sizeof(T) > STORAGE_SIZE;
		};

		/**
		*  @brief
		*    Helper to provide typed access to the internal storage
		*/
		template <typename T, bool IS_BIG>
		struct AccessHelper;

		template <typename T>
		struct AccessHelper<T, false> {

			static void Set(const T &cVal, unsigned char *pBuffer)
			{
				// [TODO] Here should be placement new
				*((T*)pBuffer) = cVal;
			}

			static void Set(const T &&cVal, unsigned char *pBuffer)
			{
				// [TODO] Here should be placement new
				*((T*)pBuffer) = cVal;
			}

			static T &Get(unsigned char *pBuffer)
			{
				return *((T*)pBuffer);
			}

			static const T &Get(const unsigned char *pBuffer)
			{
				return *((const T*)pBuffer);
			}

			static void Destroy(unsigned char *pBuffer)
			{
				(&Get(pBuffer))->~T();
			}
		};

		template <typename T>
		struct AccessHelper<T, true> {

			static void Set(const T &cVal, unsigned char *pBuffer)
			{
				*((T**)pBuffer) = new T(cVal);
			}

			static void Set(const T &&cVal, unsigned char *pBuffer)
			{
				*((T**)pBuffer) = new T(cVal);
			}

			static T &Get(unsigned char *pBuffer)
			{
				return **((T**)pBuffer);
			}

			static void Destroy(unsigned char *pBuffer)
			{
				delete *((T**)pBuffer)
			}
		};

	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		template <typename T>
		UntypedVariant &operator=(const T &) { return *this; }

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		unsigned char m_pData[STORAGE_SIZE];
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_UNTYPEDVARIANT_H__
