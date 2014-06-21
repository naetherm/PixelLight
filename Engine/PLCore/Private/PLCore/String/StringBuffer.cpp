/*********************************************************\
 *  File: StringBuffer.cpp                               *
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
#include "PLCore/String/StringBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Protected static data                                 ]
//[-------------------------------------------------------]
StringBufferManager StringBuffer::Manager;


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
StringBuffer::StringBuffer(uint32 nLength, uint32 nMaxLength, uint8 nType) :
	m_nRefCount(0),
	m_nLength(nLength),
	m_nMaxLength(nMaxLength),
	m_nType(nType)
{
}

/**
*  @brief
*    Destructor
*/
StringBuffer::~StringBuffer()
{
}

/**
*  @brief
*    Returns the length of the string buffer (excluding the terminating zero)
*/
uint32 StringBuffer::GetLength() const
{
	return m_nLength;
}

/**
*  @brief
*    Increases the reference count
*/
uint32 StringBuffer::AddReference()
{
	// Increment reference count
	m_nRefCount++;

	// Return current reference count
	return m_nRefCount;
}

/**
*  @brief
*    Decreases the reference count
*/
uint32 StringBuffer::Release()
{
	// Decrement reference count
	if (m_nRefCount > 1) {
		m_nRefCount--;

		// Return current reference count
		return m_nRefCount;

	// Destroy object when no references are left
	} else {
		delete this;

		// This object is no longer
		return 0;
	}
}

/**
*  @brief
*    Gets the current reference count
*/
uint32 StringBuffer::GetRefCount() const
{
	// Return current reference count
	return m_nRefCount;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
