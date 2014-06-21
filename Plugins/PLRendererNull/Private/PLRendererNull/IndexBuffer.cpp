/*********************************************************\
 *  File: IndexBuffer.cpp                                *
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
#include <PLCore/System/System.h>
#include <PLRenderer/Renderer/Backend/RendererBackend.h>
#include "PLRendererNull/IndexBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
IndexBuffer::~IndexBuffer()
{
	Clear();

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nIndexBufferNum--;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
IndexBuffer::IndexBuffer(PLRenderer::Renderer &cRenderer) : PLRenderer::IndexBuffer(cRenderer),
	m_pData(nullptr),
	m_pLockedData(nullptr)
{
	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(cRenderer).GetWritableStatistics().nIndexBufferNum++;
}

/**
*  @brief
*    Makes this index buffer to the renderers current index buffer
*/
bool IndexBuffer::MakeCurrent()
{
	// Check if there's an index buffer
	return (m_pData != nullptr);
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Buffer functions           ]
//[-------------------------------------------------------]
bool IndexBuffer::IsAllocated() const
{
	return (m_pData != nullptr);
}

bool IndexBuffer::Allocate(uint32 nElements, PLRenderer::Usage::Enum nUsage, bool bManaged, bool bKeepData)
{
	// Get API dependent type
	uint32 nElementSizeAPI;
	if (m_nElementType == UInt)
		nElementSizeAPI = sizeof(uint32);
	else if (m_nElementType == UShort)
		nElementSizeAPI = sizeof(uint16);
	else if (m_nElementType == UByte)
		nElementSizeAPI = sizeof(uint8);
	else
		return false; // Error!

	// Check if we have to reallocate the buffer
	if (m_nSize == nElementSizeAPI*nElements && nUsage == m_nUsage && m_bManaged == bManaged)
		return true; // Done

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nIndexBufferMem -= m_nSize;

	// Init data
	uint8 *pDataBackup = nullptr;
	uint32 nSizeBackup = m_nSize;
	if (m_pData && m_nSize != nElementSizeAPI*nElements) {
		// Backup the current data
		if (bKeepData) {
			pDataBackup = new uint8[m_nSize];
			MemoryManager::Copy(pDataBackup, m_pData, m_nSize);
		}

		// Delete data
		if (m_nElementType == UInt)
			delete [] static_cast<uint32*>(m_pData);
		else if (m_nElementType == UShort)
			delete [] static_cast<uint16*>(m_pData);
		else
			delete [] static_cast<uint8*>(m_pData);
		m_pData = nullptr;
	}
	ForceUnlock();
	m_nElements = nElements;
	m_nUsage    = nUsage;
	m_bManaged  = bManaged;
	m_nSize     = nElementSizeAPI*m_nElements;

	// Create the index buffer
	if (!m_pData)
		m_pData = new uint8[m_nSize];

	// Restore old data if required
	if (pDataBackup) {
		// We can just copy the old data in...
		if (Lock(PLRenderer::Lock::WriteOnly)) {
			uint32 nSize = nSizeBackup;
			if (nSize > m_nSize)
				nSize = m_nSize;
			MemoryManager::Copy(GetData(), pDataBackup, nSize);
			Unlock();
		}

		// Cleanup
		if (m_nElementType == UInt)
			delete [] static_cast<uint32*>(m_pData);
		else if (m_nElementType == UShort)
			delete [] static_cast<uint16*>(m_pData);
		else
			delete [] static_cast<uint8*>(m_pData);
	}

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nIndexBufferMem += m_nSize;

	// Done
	return true;
}

bool IndexBuffer::Clear()
{
	ForceUnlock();
	if (m_pData) {
		if (m_nElementType == UInt)
			delete [] static_cast<uint32*>(m_pData);
		else if (m_nElementType == UShort)
			delete [] static_cast<uint16*>(m_pData);
		else
			delete [] static_cast<uint8*>(m_pData);
		m_pData = nullptr;
	} else {
		return false; // Error!
	}

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nIndexBufferMem -= m_nSize;

	// Init
	m_nElements = 0;
	m_nSize     = 0;
	m_nUsage    = PLRenderer::Usage::Unknown;

	// Done
	return true;
}

void *IndexBuffer::Lock(uint32 nFlag)
{
	// Check whether there's an index buffer
	if (!m_pData)
		return nullptr; // Error!

	// Check whether the index buffer is already locked
	m_nLockCount++;
	if (m_pLockedData)
		return m_pLockedData; // Return the locked data

	// Lock the index buffer
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nIndexBufferLocks++;
	m_nLockStartTime = System::GetInstance()->GetMicroseconds();
	m_pLockedData = m_pData;

	// Return the index buffer data
	return m_pLockedData;
}

void *IndexBuffer::GetData()
{
	return m_pLockedData;
}

bool IndexBuffer::Unlock()
{
	// Check whether data is locked
	if (!m_pLockedData)
		return false; // Error!

	// Do we have to unlock the buffer now?
	m_nLockCount--;
	if (m_nLockCount)
		return true; // Nope, it's still used somewhere else...

	// Unlock the index buffer
	m_pLockedData = nullptr;
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nIndexBuffersSetupTime += System::GetInstance()->GetMicroseconds()-m_nLockStartTime;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull
