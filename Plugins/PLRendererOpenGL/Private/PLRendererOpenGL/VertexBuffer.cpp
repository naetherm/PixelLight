/*********************************************************\
 *  File: VertexBuffer.cpp                               *
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
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/VertexBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
VertexBuffer::~VertexBuffer()
{
	Clear();

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nVertexBufferNum--;
}

/**
*  @brief
*    Returns the OpenGL vertex buffer
*/
uint32 VertexBuffer::GetOpenGLVertexBuffer() const
{
	return m_nVertexBuffer;
}

/**
*  @brief
*    Returns the dynamic buffer (none VBO)
*/
const uint8 *VertexBuffer::GetDynamicBuffer() const
{
	return m_pData;
}

/**
*  @brief
*    Binds and updates the vertex buffer if required
*/
bool VertexBuffer::BindAndUpdate()
{
	// Bind the vertex buffer
	if (m_nVertexBuffer)
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVertexBuffer);

	// Do we need to update the VBO?
	if (m_bUpdateVBO && m_pData) {
		// Upload new data
		if (m_nVertexBuffer)
			glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, m_nSize, m_pData);

		// The data is now up-to-date
		m_bUpdateVBO = false;

		// An update was performed
		return true;
	}

	// No update was performed
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VertexBuffer::VertexBuffer(PLRenderer::Renderer &cRenderer) : PLRenderer::VertexBuffer(cRenderer),
	m_nVertexBuffer(0),
	m_pData(nullptr),
	m_pLockedData(nullptr),
	m_bLockReadOnly(false),
	m_bUpdateVBO(false),
	m_nUsageAPI(0)
{
	// Init data
	MemoryManager::Set(m_nOffset, -1, sizeof(int)*NumOfSemantics*MaxPipelineChannels);

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(cRenderer).GetWritableStatistics().nVertexBufferNum++;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::VertexBuffer functions     ]
//[-------------------------------------------------------]
void *VertexBuffer::GetData(uint32 nIndex, uint32 nSemantic, uint32 nChannel)
{
	// Check whether data is locked
	if (m_pLockedData) {
		// Check whether the index is correct
		if (nIndex < m_nElements) {
			// Check whether the channel and semantic is correct
			if (nChannel < MaxPipelineChannels && nSemantic >= Position && nSemantic <= Binormal) {
				// Return the vertex buffer attribute data
				if (m_nOffset[nSemantic][nChannel] >= 0)
					return static_cast<uint8*>(m_pLockedData)+nIndex*m_nVertexSize+m_nOffset[nSemantic][nChannel];
			}
		}
	}

	// Error!
	return nullptr;
}

Color4 VertexBuffer::GetColor(uint32 nIndex, uint32 nChannel)
{
	// Check whether the index is correct
	if (nIndex < m_nElements) {
		// Check whether the channel is correct
		if (nChannel < 2) {
			// Return the color of the vertex
			const float *pfColor = static_cast<const float*>(GetData(nIndex, Color, nChannel));
			if (pfColor)
				return Color4(pfColor[0], pfColor[1], pfColor[2], pfColor[3]);
		}
	}

	// Error!
	return Color4::White;
}

bool VertexBuffer::SetColor(uint32 nIndex, const Color4 &cColor, uint32 nChannel)
{
	// Check whether the index is correct
	if (nIndex < m_nElements) {
		// Check whether the channel is correct
		if (nChannel < 2) {
			// Set the color of the vertex
			float *pfColor = static_cast<float*>(GetData(nIndex, Color, nChannel));
			if (pfColor) {
				pfColor[0] = cColor.r;
				pfColor[1] = cColor.g;
				pfColor[2] = cColor.b;
				pfColor[3] = cColor.a;

				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::VertexBuffer functions    ]
//[-------------------------------------------------------]
void VertexBuffer::VertexAttributeAdded(Attribute &cAttribute)
{
	// Setup API dependent attribute values
	switch (cAttribute.nType) {
		// Color (legacy API dependent storage which is no longer required when using modern shader based API's, do always use GetColor() and SetColor()!)
		case RGBA:
			cAttribute.nSizeAPI		  = sizeof(float)*4;
			cAttribute.nTypeAPI		  = GL_FLOAT;
			cAttribute.nComponentsAPI = 4;
			break;

		// Float 1 (one component per element, 32 bit floating point per component)
		case Float1:
			cAttribute.nSizeAPI		  = sizeof(float);
			cAttribute.nTypeAPI		  = GL_FLOAT;
			cAttribute.nComponentsAPI = 1;
			break;

		// Float 2 (two components per element, 32 bit floating point per component)
		case Float2:
			cAttribute.nSizeAPI		  = sizeof(float)*2;
			cAttribute.nTypeAPI		  = GL_FLOAT;
			cAttribute.nComponentsAPI = 2;
			break;

		// Float 3 (three components per element, 32 bit floating point per component)
		case Float3:
			cAttribute.nSizeAPI		  = sizeof(float)*3;
			cAttribute.nTypeAPI		  = GL_FLOAT;
			cAttribute.nComponentsAPI = 3;
			break;

		// Float 4 (four components per element, 32 bit floating point per component)
		case Float4:
			cAttribute.nSizeAPI		  = sizeof(float)*4;
			cAttribute.nTypeAPI		  = GL_FLOAT;
			cAttribute.nComponentsAPI = 4;
			break;

		// Short 2 (two components per element, 16 bit integer per component)
		case Short2:
			cAttribute.nSizeAPI		  = sizeof(short)*2;
			cAttribute.nTypeAPI		  = GL_SHORT;
			cAttribute.nComponentsAPI = 2;
			break;

		// Short 4 (four components per element, 16 bit integer per component)
		case Short4:
			cAttribute.nSizeAPI		  = sizeof(short)*4;
			cAttribute.nTypeAPI		  = GL_SHORT;
			cAttribute.nComponentsAPI = 4;
			break;

		// Half 1 (one component per element, 16 bit floating point per component, may not be supported by each API)
		case Half1:
		{
			const Extensions cExtensions = static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions();
			if (cExtensions.IsGL_ARB_half_float_vertex() || cExtensions.IsGL_NV_half_float()) {
				cAttribute.nSizeAPI		  = sizeof(float)/2;
				cAttribute.nTypeAPI		  = GL_HALF_FLOAT_ARB;
				cAttribute.nComponentsAPI = 1;
			}
			break;
		}

		// Half 2 (two components per element, 16 bit floating point per component, may not be supported by each API)
		case Half2:
		{
			const Extensions cExtensions = static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions();
			if (cExtensions.IsGL_ARB_half_float_vertex() || cExtensions.IsGL_NV_half_float()) {
				cAttribute.nSizeAPI		  = sizeof(float)/2*2;
				cAttribute.nTypeAPI		  = GL_HALF_FLOAT_ARB;
				cAttribute.nComponentsAPI = 2;
			}
			break;
		}

		// Half 3 (three components per element, 16 bit floating point per component, may not be supported by each API)
		case Half3:
		{
			const Extensions cExtensions = static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions();
			if (cExtensions.IsGL_ARB_half_float_vertex() || cExtensions.IsGL_NV_half_float()) {
				cAttribute.nSizeAPI		  = sizeof(float)/2*3;
				cAttribute.nTypeAPI		  = GL_HALF_FLOAT_ARB;
				cAttribute.nComponentsAPI = 3;
			}
			break;
		}

		// Half 4 (four components per element, 16 bit floating point per component, may not be supported by each API)
		case Half4:
		{
			const Extensions cExtensions = static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions();
			if (cExtensions.IsGL_ARB_half_float_vertex() || cExtensions.IsGL_NV_half_float()) {
				cAttribute.nSizeAPI		  = sizeof(float)/2*4;
				cAttribute.nTypeAPI		  = GL_HALF_FLOAT_ARB;
				cAttribute.nComponentsAPI = 4;
			}
			break;
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Buffer functions           ]
//[-------------------------------------------------------]
bool VertexBuffer::IsAllocated() const
{
	return (m_nVertexBuffer != 0 || m_pData != nullptr);
}

bool VertexBuffer::Allocate(uint32 nElements, PLRenderer::Usage::Enum nUsage, bool bManaged, bool bKeepData)
{
	// Check if we have to reallocate the buffer
	if (m_nSize != m_nVertexSize*nElements || m_nUsage != nUsage || m_bManaged != bManaged) {
		// Check the vertex buffer size
		if (m_nVertexSize*nElements <= 0)
			return false; // Error!

		// Get extensions instance
		const Extensions &cExtensions = static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions();

		// Vertex buffer extension available?
		if (!cExtensions.IsGL_ARB_vertex_buffer_object()) {
			// Fallback to legacy software implementation
			nUsage = PLRenderer::Usage::Software;
			bManaged = false;
		}

		// Get API dependent usage
		if (nUsage == PLRenderer::Usage::Static)
			m_nUsageAPI = GL_STATIC_DRAW_ARB;
		else if (nUsage == PLRenderer::Usage::Dynamic)
			m_nUsageAPI = GL_DYNAMIC_DRAW_ARB;
		else if (nUsage == PLRenderer::Usage::WriteOnly)
			m_nUsageAPI = GL_STREAM_DRAW_ARB;
		else if (nUsage != PLRenderer::Usage::Software) {
			m_nUsageAPI = 0;

			// Error!
			return false;
		} else {
			m_nUsageAPI = 0;
		}

		// If the buffer is already allocated...
		uint8 *pDataBackup = nullptr;
		uint32 nSizeBackup = m_nSize;
		if (m_pData && ((m_nSize != m_nVertexSize*nElements) || (m_bManaged && !bManaged))) {
			// Backup the current data
			if (bKeepData) {
				pDataBackup = new uint8[m_nSize];
				MemoryManager::Copy(pDataBackup, m_pData, m_nSize);
			}

			// Delete data
			delete [] m_pData;
			m_pData = nullptr;
		}
		if (m_nVertexBuffer && nUsage == PLRenderer::Usage::Software) {
			if (bKeepData && !m_pData && !pDataBackup && Lock(PLRenderer::Lock::ReadOnly)) {
				// Backup the current data
				pDataBackup = new uint8[m_nSize];
				MemoryManager::Copy(pDataBackup, GetData(), m_nSize);
				Unlock();
			}

			// Delete vertex buffer object
			glDeleteBuffersARB(1, &m_nVertexBuffer);
			m_nVertexBuffer = 0;
		}
		ForceUnlock();

		// Update renderer statistics
		static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nVertexBufferMem -= m_nSize;

		// Setup data
		m_nElements  = nElements;
		m_nSize		 = m_nVertexSize*m_nElements;
		m_nUsage	 = nUsage;
		m_bManaged   = bManaged;

		// Create the vertex buffer
		bool bVBO = false;
		if (nUsage != PLRenderer::Usage::Software && cExtensions.IsGL_ARB_vertex_buffer_object()) {
			// Use VBO
			bVBO = true;
			if (!m_nVertexBuffer)
				glGenBuffersARB(1, &m_nVertexBuffer);
			GLint nArrayBufferBackup;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &nArrayBufferBackup);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVertexBuffer);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_nSize, nullptr, m_nUsageAPI);

			// Error checking
			int nArrayObjectSize = 0;
			glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nArrayObjectSize);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, nArrayBufferBackup);
			if (nArrayObjectSize <= 0) {
				// Delete vertex buffer object
				glDeleteBuffersARB(1, &m_nVertexBuffer);
				m_nVertexBuffer = 0;

				// Error!
				return false;
			}
		}

		// No VBO or managed?
		if (!bVBO || bManaged) {
			if (!m_pData)
				m_pData = new uint8[m_nSize];
			m_bUpdateVBO = true;
		} else {
			m_bUpdateVBO = false;
		}

		// Restore old data if required
		if (pDataBackup) {
			// We can just copy the old data in... vertex size CAN'T change in this situation!
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
		static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nVertexBufferMem += m_nSize;
	}

	// Get data attribute offsets
	MemoryManager::Set(m_nOffset, -1, sizeof(int)*NumOfSemantics*MaxPipelineChannels);
	for (uint32 i=0; i<m_lstVertexAttributes.GetNumOfElements(); i++) {
		const Attribute &cAttribute = *m_lstVertexAttributes[i];
		m_nOffset[cAttribute.nSemantic][cAttribute.nChannel] = cAttribute.nOffset;
	}

	// Done
	return true;
}

bool VertexBuffer::Clear()
{
	if (!IsAllocated())
		return false; // Error!
	ForceUnlock();
	if (m_nVertexBuffer) {
		glDeleteBuffersARB(1, &m_nVertexBuffer);
		m_nVertexBuffer = 0;
	}
	if (m_pData) {
		delete [] m_pData;
		m_pData = nullptr;
	}

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nVertexBufferMem -= m_nSize;

	// Init
	m_nElements	 = 0;
	m_nSize		 = 0;
	m_nUsage	 = PLRenderer::Usage::Unknown;
	m_nUsageAPI	 = 0;
	m_bUpdateVBO = false;
	MemoryManager::Set(m_nOffset, -1, sizeof(int)*NumOfSemantics*MaxPipelineChannels);

	// Done
	return true;
}

void *VertexBuffer::Lock(uint32 nFlag)
{
	// Check whether there's a vertex buffer
	if (!m_nVertexBuffer && !m_pData)
		return nullptr; // Error!

	// Check whether the vertex buffer is already locked
	m_nLockCount++;
	if (m_pLockedData)
		return m_pLockedData;

	// Get API dependent flag
	uint32 nFlagAPI;
	if (nFlag == PLRenderer::Lock::ReadOnly) {
		nFlagAPI        = GL_READ_ONLY_ARB;
		m_bLockReadOnly = true;
	} else if (nFlag == PLRenderer::Lock::WriteOnly) {
		nFlagAPI        = GL_WRITE_ONLY_ARB;
		m_bLockReadOnly = false;
	} else if (nFlag == PLRenderer::Lock::ReadWrite) {
		nFlagAPI        = GL_READ_WRITE_ARB;
		m_bLockReadOnly = false;
	} else {
		// Error!
		return nullptr;
	}

	// Map the vertex buffer
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nVertexBufferLocks++;
	m_nLockStartTime = System::GetInstance()->GetMicroseconds();
	if (m_pData)
		m_pLockedData = m_pData;
	else if (m_nVertexBuffer) {
		// Bind and update the vertex buffer if required
		BindAndUpdate();

		// Map
		m_pLockedData = glMapBufferARB(GL_ARRAY_BUFFER_ARB, nFlagAPI);
	}

	// Lock valid?
	if (!m_pLockedData)
		m_nLockCount--;

	// Return the locked data
	return m_pLockedData;
}

void *VertexBuffer::GetData()
{
	return m_pLockedData;
}

bool VertexBuffer::Unlock()
{
	// Check whether data is locked
	if (!m_pLockedData)
		return false; // Error!

	// Do we have to unlock the buffer now?
	m_nLockCount--;
	if (m_nLockCount)
		return true; // Nope, it's still used somewhere else...

	// Unlock the vertex buffer
	if (m_pData) {
		if (m_nVertexBuffer && !m_bLockReadOnly)
			m_bUpdateVBO = true;
	} else {
		if (m_nVertexBuffer) {
			// Bind and update the vertex buffer if required
			BindAndUpdate();

			// Unmap
			glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
		}
	}
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nVertexBuffersSetupTime += System::GetInstance()->GetMicroseconds()-m_nLockStartTime;
	m_pLockedData   = nullptr;
	m_bLockReadOnly = false;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void VertexBuffer::BackupDeviceData(uint8 **ppBackup)
{
	// Backup data
	if (!m_pData && Lock(PLRenderer::Lock::ReadOnly)) {
		*ppBackup = new uint8[m_nSize];
		MemoryManager::Copy(*ppBackup, GetData(), m_nSize);
		ForceUnlock();
	} else {
		*ppBackup = nullptr;
	}

	// Destroy the vertex buffer
	if (m_nVertexBuffer) {
		glDeleteBuffersARB(1, &m_nVertexBuffer);
		m_nVertexBuffer = 0;
	}
}

void VertexBuffer::RestoreDeviceData(uint8 **ppBackup)
{
	if (*ppBackup) {
		GLint nArrayBufferBackup;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &nArrayBufferBackup);
		glGenBuffersARB(1, &m_nVertexBuffer);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVertexBuffer);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_nSize, nullptr, m_nUsageAPI);
		if (Lock(PLRenderer::Lock::WriteOnly)) {
			MemoryManager::Copy(GetData(), *ppBackup, m_nSize);
			Unlock();
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, nArrayBufferBackup);

		// Delete memory
		delete [] *ppBackup;
	} else {
		if (m_bManaged) {
			GLint nArrayBufferBackup;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING_ARB, &nArrayBufferBackup);
			glGenBuffersARB(1, &m_nVertexBuffer);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVertexBuffer);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_nSize, nullptr, m_nUsageAPI);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, nArrayBufferBackup);
			m_bUpdateVBO = true;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
