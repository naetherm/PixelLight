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
#include "Destructor.h"


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
		UntypedVariant(T cVal, bool bAutoDestroy = false)
		{
			typedef typename TypeStorage<T>::StorageType Type;
			AccessHelper<T, IsBig<Type>::Value>::Set(cVal, m_pData);

			if (bAutoDestroy)
			{
				m_cDestructor = Destructor::Create<Type, IsBig<Type>::Value>();
			}
		}

		/**
		*  @brief
		*    Destructor
		*/
		~UntypedVariant()
		{
			Destroy();
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
		void Set(T cVal, bool bAutoDestroy = false)
		{
			// We are possibly overriding the stored value, thus we should destroy the old one if
			// it was set.
			Destroy();

			typedef typename TypeStorage<T>::StorageType Type;
			AccessHelper<T, IsBig<Type>::Value>::Set(cVal, m_pData);

			if (bAutoDestroy)
			{
				m_cDestructor = Destructor::Create<Type, IsBig<Type>::Value>();
			}
		}

		/**
		*  @brief
		*    Get the stored value
		*/
		template <typename T>
		const T Get() const
		{
			typedef typename TypeStorage<T>::StorageType Type;
			return AccessHelper<T, IsBig<Type>::Value>::Get(m_pData);
		}

		/**
		*  @brief
		*    Get the stored value
		*/
		template <typename T>
		T Get()
		{
			typedef typename TypeStorage<T>::StorageType Type;
			return AccessHelper<T, IsBig<Type>::Value>::Get(m_pData);
		}

		/**
		*  @brief
		*    Get the stored value as pointer
		*/
		template <typename T>
		const T &GetRef() const
		{
			typedef typename TypeStorage<T>::StorageType Type;
			return AccessHelper<T, IsBig<Type>::Value>::GetRef(m_pData);
		}

		/**
		*  @brief
		*    Get the stored value as pointer
		*/
		template <typename T>
		T &GetRef()
		{
			typedef typename TypeStorage<T>::StorageType Type;
			return AccessHelper<T, IsBig<Type>::Value>::GetRef(m_pData);
		}

		/**
		*  @brief
		*    Destroy the object stored in the variant
		*/
		void Destroy()
		{
			m_cDestructor.Destroy(m_pData);

			// Reset the destructor!
			m_cDestructor = Destructor();
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

			typedef TypeStorage<T> Storage;

			static void Set(T cVal, unsigned char *pBuffer)
			{
				// [TODO] Arrays are not supported
				typedef typename Storage::StorageType Type;
				new (pBuffer) Type(Storage::Store(cVal));
			}

			static T Get(unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::Restore(*((Type*)pBuffer));
			}

			static const T Get(const unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::Restore(*((const Type*)pBuffer));
			}

			static T &GetRef(unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::RestoreRef(*((Type*)pBuffer));
			}

			static const T &GetRef(const unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::RestoreRef(*((const Type*)pBuffer));
			}
		};

		template <typename T>
		struct AccessHelper<T, true> {

			typedef TypeStorage<T> Storage;

			static void Set(const T &cVal, unsigned char *pBuffer)
			{
				// [TODO] Arrays are not supported
				typedef typename Storage::StorageType Type;
				*((Type**)pBuffer) = new Type(Storage::Store(cVal));
			}

			static T Get(unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::Restore(**((Type**)pBuffer));
			}

			static const T Get(const unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::Restore(**((const Type**)pBuffer));
			}

			static T &GetRef(unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::RestoreRef(**((Type**)pBuffer));
			}

			static const T &GetRef(const unsigned char *pBuffer)
			{
				typedef typename Storage::StorageType Type;
				return Storage::RestoreRef(**((const Type**)pBuffer));
			}
		};

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		unsigned char	m_pData[STORAGE_SIZE];		/**< Untyped data buffer */
		Destructor		m_cDestructor;				/**< Untyped destructor*/
};

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_UNTYPEDVARIANT_H__
