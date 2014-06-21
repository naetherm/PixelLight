/*********************************************************\
 *  File: IndexBuffer.inl                                *
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
#include <PLCore/Core/MemoryManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the type of the buffer elements
*/
inline IndexBuffer::EType IndexBuffer::GetElementType() const
{
	return m_nElementType;
}

/**
*  @brief
*    Sets the type of the buffer elements
*/
inline bool IndexBuffer::SetElementType(EType nType)
{
	// Is the index buffer already allocated?
	if (IsAllocated())
		return false; // Error!

	// Set element type
	m_nElementType = nType;

	// Done
	return true;
}

/**
*  @brief
*    Sets the type of the buffer elements automatically by using a given maximum vertex index
*/
inline bool IndexBuffer::SetElementTypeByMaximumIndex(PLCore::uint32 nMaximumIndex)
{
	// Is the index buffer already allocated?
	if (IsAllocated())
		return false; // Error!

	// Set element type
	if (nMaximumIndex <= MaxVertexIndexUByte)
		m_nElementType = UByte;
	else
		m_nElementType = (nMaximumIndex <= MaxVertexIndexUShort) ? UShort : UInt;

	// Done
	return true;
}

/**
*  @brief
*    Copies data from a given buffer into this index buffer
*/
inline void IndexBuffer::CopyDataFrom(const void *pData)
{
	// Lock the index buffer
	if (Lock(Lock::WriteOnly)) {
		// Copy over all the index data at once
		PLCore::MemoryManager::Copy(GetData(), pData, GetSize());

		// Unlock the index buffer
		Unlock();
	}
}

/**
*  @brief
*    Copies data from this index buffer into a given buffer
*/
inline void IndexBuffer::CopyDataTo(void *pData)
{
	// Lock the index buffer
	if (Lock(Lock::ReadOnly)) {
		// Copy over all the index data at once
		PLCore::MemoryManager::Copy(pData, GetData(), GetSize());

		// Unlock the index buffer
		Unlock();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
