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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "ClassMethod.h"

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRefl {

//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create builder for the specified class instance
*/
template <typename T>
ClassBuilder<T>::ClassBuilder(Class &cClss)
	: m_pClass(&cClss)
{
}

/**
*  @brief
*    Declare a method inside the class
*/
template <typename T>
ClassBuilder<T> &ClassBuilder<T>::Method(const PLCore::String &sName, PLCore::FunctionBase *pFn)
{
	// Add method to class
	m_pClass->m_mapMethods.Add(sName, ClassMethod(sName, pFn));

	return *this;
}

/**
*  @brief
*    Declare a field inside the class
*/
template <typename T>
ClassBuilder<T> &ClassBuilder<T>::Field(const PLCore::String &sName, PLCore::uint32 nOffset)
{
	// Add field to class
	m_pClass->m_mapFields.Add(sName, ClassField(sName, nOffset));

	return *this;
}

/**
*  @brief
*    Declare a property inside the class
*/
template <typename T>
ClassBuilder<T> &ClassBuilder<T>::Property(const PLCore::String &sName, PLCore::FunctionBase *pSetter, PLCore::FunctionBase *pGetter)
{
	// Add method to class
	m_pClass->m_mapProperties.Add(sName, ClassProperty(sName, pSetter, pGetter));

	return *this;
}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl
