/*********************************************************\
 *  File: Chunk.inl                                      *
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
*    Returns the semantic
*/
inline Chunk::ESemantic Chunk::GetSemantic() const
{
	return m_nSemantic;
}

/**
*  @brief
*    Sets the semantic
*/
inline void Chunk::SetSemantic(ESemantic nSemantic)
{
	m_nSemantic = nSemantic;
}

/**
*  @brief
*    Returns the chunk data
*/
inline uint8 *Chunk::GetData()
{
	return m_pData;
}

/**
*  @brief
*    Returns the chunk data
*/
inline const uint8 *Chunk::GetData() const
{
	return m_pData;
}

/**
*  @brief
*    Returns the chunk element component data
*/
inline uint8 *Chunk::GetElementComponentData(uint32 nElement, uint32 nComponent)
{
	return &m_pData[nElement*GetNumOfBytesPerElement() + nComponent*GetNumOfBytesPerElementComponent()];
}

/**
*  @brief
*    Returns the chunk element component data
*/
inline const uint8 *Chunk::GetElementComponentData(uint32 nElement, uint32 nComponent) const
{
	return &m_pData[nElement*GetNumOfBytesPerElement() + nComponent*GetNumOfBytesPerElementComponent()];
}

/**
*  @brief
*    Returns the element type
*/
inline Chunk::EElementType Chunk::GetElementType() const
{
	return m_nElementType;
}

/**
*  @brief
*    Returns the number of components per element
*/
inline uint32 Chunk::GetNumOfComponentsPerElement() const
{
	return m_nNumOfComponentsPerElement;
}

/**
*  @brief
*    Returns the number of elements
*/
inline uint32 Chunk::GetNumOfElements() const
{
	return m_nNumOfElements;
}

/**
*  @brief
*    Returns the number of bytes per element
*/
inline uint32 Chunk::GetNumOfBytesPerElement() const
{
	return GetNumOfBytesPerElementComponent()*m_nNumOfComponentsPerElement;
}

/**
*  @brief
*    Returns the total number of bytes of the chunk data
*/
inline uint32 Chunk::GetTotalNumOfBytes() const
{
	return GetNumOfBytesPerElement()*m_nNumOfElements;
}

/**
*  @brief
*    Returns the total number of components (all elements summed up) within the chunk
*/
inline uint32 Chunk::GetTotalNumOfComponents() const
{
	return m_nNumOfComponentsPerElement*m_nNumOfElements;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
