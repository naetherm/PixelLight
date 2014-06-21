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
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
inline Buffer::~Buffer()
{
	// Destroy buffer
	Clear();
}

/**
*  @brief
*    Get option for removing line delimiters
*/
inline bool Buffer::GetRemoveLineDelimiters() const
{
	// Return option
	return m_bRemoveDelimiters;
}

/**
*  @brief
*    Set option for removing line delimiters
*/
inline void Buffer::SetRemoveLineDelimiters(bool bRemoveDelimiters)
{
	// Set option
	m_bRemoveDelimiters = bRemoveDelimiters;
}

/**
*  @brief
*    Get current buffer size
*/
inline uint32 Buffer::GetSize() const
{
	// Return size
	return m_nSize;
}

/**
*  @brief
*    Get data
*/
inline const char *Buffer::GetData() const
{
	// Return data pointer
	return m_pBuffer;
}

/**
*  @brief
*    Get data
*/
inline char *Buffer::GetData()
{
	// Return data pointer
	return m_pBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
