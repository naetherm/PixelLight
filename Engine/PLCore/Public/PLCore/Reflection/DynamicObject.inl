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
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Dynamic object from an existing instance
*/
template <typename T>
DynamicObject::DynamicObject(T cInst)
{
	Set<T>(cInst);
}

/**
*  @brief
*    Set the dynamic object to a new value
*/
template <typename T>
void DynamicObject::Set(T cInst)
{
	m_pStorage.Set<T>(cInst);
	m_pTypeInfo = GetStaticTypeInfo(cInst);
}

/**
*  @brief
*    Get the internal object
*/
const PLCore::UntypedVariant<> &DynamicObject::GetUntyped() const
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
T DynamicObject::GetAs()
{
	// [TODO] Retrieve the TypeInfo of T and check if it is compatible with the stored TypeInfo
	return m_pStorage.Get<T>();
}

/**
*  @brief
*    Get object as an instance of the specified type
*/
template <typename T>
const T DynamicObject::GetAs() const
{
	// [TODO] Retrieve the TypeInfo of T and check if it is compatible with the stored TypeInfo
	return m_pStorage.Get<T>();
}

/**
*  @brief
*    Comparison operator
*/
bool DynamicObject::operator==(const DynamicObject &cOther) const
{
	// [TODO] Untyped objects cannot be compared as of now so this always return false!!!
	return (m_pTypeInfo == cOther.m_pTypeInfo && m_pStorage == cOther.m_pStorage);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
