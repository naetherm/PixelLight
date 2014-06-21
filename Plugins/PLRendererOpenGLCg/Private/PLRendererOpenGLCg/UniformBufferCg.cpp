/*********************************************************\
 *  File: UniformBufferCg.cpp                            *
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
#include "PLRendererOpenGLCg/CgContext.h"
#include "PLRendererOpenGLCg/UniformBufferCg.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
UniformBufferCg::~UniformBufferCg()
{
	Clear();

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferNum--;
}

/**
*  @brief
*    Returns the dynamic data (none UBO)
*/
void *UniformBufferCg::GetDynamicData() const
{
	return m_pCgBuffer ? nullptr : m_pData;
}

/**
*  @brief
*    Returns the Cg buffer
*/
CGbuffer UniformBufferCg::GetCgBuffer() const
{
	return m_pCgBuffer;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
UniformBufferCg::UniformBufferCg(PLRenderer::Renderer &cRenderer, const String &sShaderLanguage) : PLRenderer::UniformBuffer(cRenderer),
	m_sShaderLanguage(sShaderLanguage),
	m_pCgBuffer(nullptr),
	m_pData(nullptr),
	m_pLockedData(nullptr),
	m_bLockReadOnly(false),
	m_bUpdateUBO(false),
	m_nUsageAPI(CG_BUFFER_USAGE_DYNAMIC_DRAW)
{
	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(cRenderer).GetWritableStatistics().nUniformBufferNum++;
}

/**
*  @brief
*    Makes this uniform buffer to the renderers current uniform buffer
*/
bool UniformBufferCg::MakeCurrent()
{
	// Is there an uniform buffer?
	if (!m_pCgBuffer && !m_pData)
		return false; // Error!

	// Bind the uniform buffer
	if (m_pCgBuffer) {
		// Do we need to update the UBO?
		if (m_bUpdateUBO && m_pData) {
			m_bUpdateUBO = false;

			// Upload new data
			cgSetBufferSubData(m_pCgBuffer, 0, m_nSize, m_pData);
		}
	} else if (m_pData) {
		// Nothing to do
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::UniformBuffer functions    ]
//[-------------------------------------------------------]
String UniformBufferCg::GetShaderLanguage() const
{
	return m_sShaderLanguage;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Buffer functions           ]
//[-------------------------------------------------------]
bool UniformBufferCg::IsAllocated() const
{
	return (m_pCgBuffer != nullptr || m_pData != nullptr);
}

bool UniformBufferCg::Allocate(uint32 nElements, PLRenderer::Usage::Enum nUsage, bool bManaged, bool bKeepData)
{
	// Check if we have to reallocate the buffer
	if (m_nSize == nElements && nUsage == m_nUsage && m_bManaged == bManaged)
		return true; // Done

	// Get API dependent usage
 	if (nUsage == PLRenderer::Usage::Static)
		m_nUsageAPI = CG_BUFFER_USAGE_STATIC_DRAW;
	else if (nUsage == PLRenderer::Usage::Dynamic)
		m_nUsageAPI = CG_BUFFER_USAGE_DYNAMIC_DRAW;
	else if (nUsage == PLRenderer::Usage::WriteOnly)
		m_nUsageAPI = CG_BUFFER_USAGE_STREAM_DRAW;
	else if (nUsage != PLRenderer::Usage::Software) {
		m_nUsageAPI = CG_BUFFER_USAGE_DYNAMIC_DRAW;

		// Error!
		return false;
	} else {
		m_nUsageAPI = CG_BUFFER_USAGE_DYNAMIC_DRAW;
	}

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferMem -= m_nSize;

	// Init data
	uint8 *pDataBackup = nullptr;
	uint32 nSizeBackup = m_nSize;
	if (m_pData && ((m_nSize != nElements) || (m_bManaged && !bManaged))) {
		// Backup the current data
		if (bKeepData) {
			pDataBackup = new uint8[m_nSize];
			MemoryManager::Copy(pDataBackup, m_pData, m_nSize);
		}

		// Delete data
		delete [] static_cast<uint8*>(m_pData);
		m_pData = nullptr;
	}
	if (m_pCgBuffer && nUsage == PLRenderer::Usage::Software) {
		if (bKeepData && !m_pData && !pDataBackup && Lock(PLRenderer::Lock::ReadOnly)) {
			// Backup the current data
			pDataBackup = new uint8[m_nSize];
			MemoryManager::Copy(pDataBackup, GetData(), m_nSize);
			Unlock();
		}

		// Delete uniform buffer object
		cgDestroyBuffer(m_pCgBuffer);
		m_pCgBuffer = nullptr;
	}
	ForceUnlock();
	m_nElements  = nElements;
	m_nSize		 = m_nElements;
	m_nUsage	 = nUsage;
	m_bManaged   = bManaged;

	// Create the uniform buffer
	bool bUBO = false;
	if (nUsage != PLRenderer::Usage::Software) {
		// Use UBO
		bUBO = true;
		if (!m_pCgBuffer)
			m_pCgBuffer = cgCreateBuffer(CgContext::GetCgContext(), m_nSize, nullptr, m_nUsageAPI);

		// Error checking
		if (cgGetBufferSize(m_pCgBuffer) <= 0) {
			// Delete uniform buffer object
			cgDestroyBuffer(m_pCgBuffer);
			m_pCgBuffer = nullptr;

			// Error!
			return false;
		}
	}

	// No UBO or managed?
	if (!bUBO || bManaged) {
		if (!m_pData)
			m_pData = new uint8[m_nSize];
		m_bUpdateUBO = true;
	} else {
		m_bUpdateUBO = false;
	}

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
		delete [] pDataBackup;
	}

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferMem += m_nSize;

	// Done
	return true;
}

bool UniformBufferCg::Clear()
{
	if (IsAllocated()) {
		ForceUnlock();
		if (m_pCgBuffer) {
			cgDestroyBuffer(m_pCgBuffer);
			m_pCgBuffer = nullptr;
		}
		if (m_pData) {
			delete [] static_cast<uint8*>(m_pData);
			m_pData = nullptr;
		}

		// Update renderer statistics
		static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferMem -= m_nSize;

		// Init
		m_nElements  = 0;
		m_nSize		 = 0;
		m_nUsage	 = PLRenderer::Usage::Unknown;
		m_nUsageAPI  = CG_BUFFER_USAGE_DYNAMIC_DRAW;
		m_bUpdateUBO = false;
	}

	// Done
	return true;
}

void *UniformBufferCg::Lock(uint32 nFlag)
{
	// Check whether there's an uniform buffer
	if (!m_pCgBuffer && !m_pData)
		return nullptr; // Error!

	// Check whether the uniform buffer is already locked
	m_nLockCount++;
	if (!m_pLockedData) {
		// Get API dependent flag
		CGbufferaccess nFlagAPI;
		if (nFlag == PLRenderer::Lock::ReadOnly) {
			nFlagAPI        = CG_MAP_READ;
			m_bLockReadOnly = true;
		} else if (nFlag == PLRenderer::Lock::WriteOnly) {
			nFlagAPI        = CG_MAP_WRITE;
			m_bLockReadOnly = false;
		} else if (nFlag == PLRenderer::Lock::ReadWrite) {
			nFlagAPI        = CG_MAP_READ_WRITE;
			m_bLockReadOnly = false;
		} else {
			// Error!
			return nullptr;
		}

		// Map the uniform buffer
		static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferLocks++;
		m_nLockStartTime = System::GetInstance()->GetMicroseconds();
		if (m_pData)
			m_pLockedData = m_pData;
		else if (m_pCgBuffer) {
			// Make this uniform buffer to the current one
			MakeCurrent();

			// Map
			m_pLockedData = cgMapBuffer(m_pCgBuffer, nFlagAPI);
		}

		// Lock valid?
		if (!m_pLockedData)
			m_nLockCount--;
	}

	// Return the locked data
	return m_pLockedData;
}

void *UniformBufferCg::GetData()
{
	return m_pLockedData;
}

bool UniformBufferCg::Unlock()
{
	// Check whether data is locked
	if (!m_pLockedData)
		return false; // Error!

	// Do we have to unlock the buffer now?
	m_nLockCount--;
	if (m_nLockCount)
		return true; // Nope, it's still used somewhere else...

	// Unlock the uniform buffer
	if (m_pData) {
		if (m_pCgBuffer && !m_bLockReadOnly) {
			m_bUpdateUBO = true;

			// Make this uniform buffer to the current one
			MakeCurrent();
		}
	} else {
		if (m_pCgBuffer) {
			// Make this uniform buffer to the current one
			MakeCurrent();

			// Unmap
			cgUnmapBuffer(m_pCgBuffer);
		}
	}
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBuffersSetupTime += System::GetInstance()->GetMicroseconds()-m_nLockStartTime;
	m_pLockedData   = nullptr;
	m_bLockReadOnly = false;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Protected virtual PLRenderer::Resource functions      ]
//[-------------------------------------------------------]
void UniformBufferCg::BackupDeviceData(uint8 **ppBackup)
{
	// Backup data
	if (!m_pData && Lock(PLRenderer::Lock::ReadOnly)) {
		*ppBackup = new uint8[m_nSize];
		MemoryManager::Copy(*ppBackup, GetData(), m_nSize);
		ForceUnlock();
	} else {
		*ppBackup = nullptr;
	}

	// Destroy the uniform buffer
	if (m_pCgBuffer) {
		cgDestroyBuffer(m_pCgBuffer);
		m_pCgBuffer = nullptr;
	}
}

void UniformBufferCg::RestoreDeviceData(uint8 **ppBackup)
{
	if (*ppBackup) {
		m_pCgBuffer = cgCreateBuffer(CgContext::GetCgContext(), m_nSize, nullptr, m_nUsageAPI);
		if (Lock(PLRenderer::Lock::WriteOnly)) {
			MemoryManager::Copy(GetData(), *ppBackup, m_nSize);
			Unlock();
		}

		// Delete memory
		delete [] *ppBackup;
	} else {
		if (m_bManaged) {
			m_pCgBuffer = cgCreateBuffer(CgContext::GetCgContext(), m_nSize, nullptr, m_nUsageAPI);
			m_bUpdateUBO = true;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg
