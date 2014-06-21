/*********************************************************\
 *  File: Buffer.cpp                                     *
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
#include <string.h>
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/Network/Buffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Buffer::Buffer(uint32 nSize) :
	m_pBuffer(nullptr),
	m_nSize(0),
	m_nUsed(0),
	m_bRemoveDelimiters(true)
{
	// Allocate buffer?
	if (nSize)
		Allocate(nSize);
}

/**
*  @brief
*    Copy constructor
*/
Buffer::Buffer(const Buffer &cSource) :
	m_pBuffer(cSource.m_nUsed ? new char[cSource.m_nUsed] : nullptr),
	m_nSize(cSource.m_nUsed),
	m_nUsed(cSource.m_nUsed),
	m_bRemoveDelimiters(cSource.m_bRemoveDelimiters)
{
	// Copy buffer data
	if (m_pBuffer)
		MemoryManager::Copy(m_pBuffer, cSource.m_pBuffer, m_nUsed);
}

/**
*  @brief
*    Clear buffer
*/
void Buffer::Clear()
{
	// Release memory
	if (m_pBuffer) {
		delete [] m_pBuffer;
		m_pBuffer = nullptr;
	}

	// Reset
	m_nSize = 0;
	m_nUsed = 0;
}

/**
*  @brief
*    Allocates a new buffer (all previous data will be lost)
*/
void Buffer::Allocate(uint32 nSize)
{
	// Clear buffer first
	if (m_pBuffer)
		Clear();

	// Allocate new buffer
	if (nSize) {
		m_pBuffer = new char[nSize];
		m_nSize   = nSize;
	}
}

/**
*  @brief
*    Add data to the buffer
*/
void Buffer::Add(const char *pData, uint32 nSize)
{
	// Check parameters
	if (pData && nSize) {
		// Enlarge the buffer if necessary
		if (m_nUsed+nSize+1 > m_nSize) {
			// +32 = give some more memory to reduce too frequently allocation
			m_nSize   = m_nUsed + nSize + 32;
			m_pBuffer = static_cast<char*>(MemoryManager::Reallocator(m_pBuffer, m_nSize));
		}

		// Add data to buffer
		MemoryManager::Copy(m_pBuffer + m_nUsed, pData, nSize);
		m_pBuffer[m_nUsed+nSize] = 0;
		m_nUsed += nSize;
	}
}

/**
*  @brief
*    Get a line from the buffer
*/
String Buffer::GetLine()
{
	// Is there a data buffer?
	if (m_pBuffer) {
		// Search for a line delimiter
		const char *pszEOL = strstr(m_pBuffer, "\n");
		if (pszEOL) {
			// Allocate new string
			const int nSize = static_cast<int>(pszEOL - m_pBuffer);
			char *pszString = new char[nSize+1];
			MemoryManager::Copy(pszString, m_pBuffer, nSize);
			pszString[nSize] = '\0';

			// Remove line from buffer
			const int nRemaining = m_nUsed - nSize - 1;
			if (nRemaining > 0)
				MemoryManager::Copy(m_pBuffer, m_pBuffer + nSize + 1, nRemaining);
			else
				m_pBuffer[0] = 0;
			m_nUsed = nRemaining;

			// Save string. The string will take control over the created buffer
			String sLine = String(pszString, false, nSize);
			if (m_bRemoveDelimiters)
				sLine.RemoveLineEndings();

			// Return string
			return sLine;
		}
	}

	// Error, no valid line found
	return "";
}

/**
*  @brief
*    Copy operator
*/
Buffer &Buffer::operator =(const Buffer &cSource)
{
	// Release memory
	if (m_pBuffer) {
		delete [] m_pBuffer;
		m_pBuffer = nullptr;
	}

	// Set data
	m_nSize = cSource.m_nUsed;
	m_nUsed = cSource.m_nUsed;
	m_bRemoveDelimiters = cSource.m_bRemoveDelimiters;

	// Copy data
	if (m_nUsed) {
		m_pBuffer = new char[m_nUsed];
		MemoryManager::Copy(m_pBuffer, cSource.m_pBuffer, m_nUsed);
	}

	// Return this instance
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
