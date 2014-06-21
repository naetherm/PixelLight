/*********************************************************\
 *  File: XmlDocument.inl                                *
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
*    Load a file using the current document value
*/
inline bool XmlDocument::Load(EEncoding nEncoding)
{
	return Load(m_sValue, nEncoding);
}

/**
*  @brief
*    Save a file using the current document value
*/
inline bool XmlDocument::Save()
{
	return Save(m_sValue);
}

/**
*  @brief
*    Get the root element -- the only top level element -- of the document
*/
inline XmlElement *XmlDocument::GetRootElement()
{
	return GetFirstChildElement();
}

inline const XmlElement *XmlDocument::GetRootElement() const
{
	return GetFirstChildElement();
}

/**
*  @brief
*    If an error occurs, error will be set to true
*/
inline bool XmlDocument::Error() const
{
	return m_bError;
}

/**
*  @brief
*    Generally, you probably want the error string ('GetErrorDesc()') - but if you
*    prefer the error ID, this function will fetch it
*/
inline int XmlDocument::GetErrorID() const
{
	return m_nErrorID;
}

/**
*  @brief
*    Returns the location (if known) of the error
*/
inline int XmlDocument::GetErrorRow() const
{
	return m_cErrorCursor.nRow + 1;
}

/**
*  @brief
*    The column where the error occurred
*/
inline int XmlDocument::GetErrorColumn() const
{
	return  m_cErrorCursor.nColumn + 1;
}

/**
*  @brief
*    Returns the tab size
*/
inline uint32 XmlDocument::GetTabSize() const
{
	return m_nTabSize;
}

/**
*  @brief
*    Sets the tab size
*/
inline void XmlDocument::SetTabSize(uint32 nTabSize)
{
	m_nTabSize = nTabSize;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
