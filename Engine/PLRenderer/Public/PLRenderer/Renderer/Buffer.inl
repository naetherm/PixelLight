/*********************************************************\
 *  File: Buffer.inl                                     *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of buffer elements
*/
inline PLCore::uint32 Buffer::GetNumOfElements() const
{
	return m_nElements;
}

/**
*  @brief
*    Returns the buffer size
*/
inline PLCore::uint32 Buffer::GetSize() const
{
	return m_nSize;
}

/**
*  @brief
*    Returns the usage flag
*/
inline Usage::Enum Buffer::GetUsage() const
{
	return m_nUsage;
}

/**
*  @brief
*    Returns whether the buffer is managed or not
*/
inline bool Buffer::IsManaged() const
{
	return m_bManaged;
}

/**
*  @brief
*    Returns the lock count
*/
inline PLCore::uint16 Buffer::GetLockCount() const
{
	return m_nLockCount;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Forces immediately vertex buffer unlock
*/
inline bool Buffer::ForceUnlock()
{
	// Is the buffer locked?
	if (!m_nLockCount)
		return false; // Error!

	// Force unlock now!
	m_nLockCount = 1;
	return Unlock();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
