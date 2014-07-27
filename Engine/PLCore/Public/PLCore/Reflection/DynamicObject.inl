/*********************************************************\
 *  File: DynamicObject.inl                              *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "TypeInfo.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRefl {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dynamic object from an existing instance
*/
template <typename T>
DynamicObject::DynamicObject(T *pInst)
{
	m_pStorage = static_cast<void*>(pInst);
	m_pTypeInfo = GetStaticTypeInfo(&pInst);
}

/**
*  @brief
*    Dynamic object from an existing instance
*/
template <typename T>
DynamicObject::DynamicObject(const T *pInst)
{
	m_pStorage = static_cast<void*>(pInst);
	m_pTypeInfo = GetStaticTypeInfo(&pInst);
}

/**
*  @brief
*    Get the internal object
*/
void *DynamicObject::GetUntyped()
{
	return m_pStorage;
}

/**
*  @brief
*    Get the internal object
*/
const void *DynamicObject::GetUntyped() const
{
	return m_pStorage;
}

/**
*  @brief
*    Get type info of the stored object
*/
const TypeInfo *DynamicObject::GetTypeInfo() const
{
	return m_pTypeInfo;
}

/**
*  @brief
*    Get object as an instance of the specified type
*/
template <typename T>
T *DynamicObject::GetAs()
{
	// [TODO] Retrieve the TypeInfo of T and check if it is compatible with the stored TypeInfo
	return reinterpret_cast<T*>(m_pStorage);
}

/**
*  @brief
*    Get object as an instance of the specified type
*/
template <typename T>
const T *DynamicObject::GetAs() const
{
	// [TODO] Retrieve the TypeInfo of T and check if it is compatible with the stored TypeInfo
	return reinterpret_cast<const T*>(m_pStorage);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl
