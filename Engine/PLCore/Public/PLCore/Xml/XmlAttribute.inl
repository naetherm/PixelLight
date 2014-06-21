/*********************************************************\
 *  File: XmlAttribute.inl                               *
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
*    Return the name of this attribute
*/
inline String XmlAttribute::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Set the name of this attribute
*/
inline void XmlAttribute::SetName(const String &sName)
{
	m_sName = sName;
}

/**
*  @brief
*    Return the value of this attribute
*/
inline String XmlAttribute::GetValue() const
{
	return m_sValue;
}

/**
*  @brief
*    Return the value of this attribute, converted to an integer
*/
inline int XmlAttribute::GetIntValue() const
{
	return m_sValue.GetInt();
}

/**
*  @brief
*    Return the value of this attribute, converted to a double
*/
inline double XmlAttribute::GetDoubleValue() const
{
	return m_sValue.GetDouble();
}

/**
*  @brief
*    Set the value
*/
inline void XmlAttribute::SetValue(const String &sValue)
{
	m_sValue = sValue;
}

/**
*  @brief
*    Set the value from an integer
*/
inline void XmlAttribute::SetIntValue(int nValue)
{
	m_sValue = nValue;
}

/**
*  @brief
*    Set the value from a double
*/
inline void XmlAttribute::SetDoubleValue(double dValue)
{
	m_sValue = dValue;
}

/**
*  @brief
*    Get the next sibling attribute in the DOM
*/
inline XmlAttribute *XmlAttribute::GetNext()
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	return (!m_pNextAttribute->m_sValue.GetLength() && !m_pNextAttribute->m_sName.GetLength()) ? nullptr : m_pNextAttribute;
}

inline const XmlAttribute *XmlAttribute::GetNext() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	return (!m_pNextAttribute->m_sValue.GetLength() && !m_pNextAttribute->m_sName.GetLength()) ? nullptr : m_pNextAttribute;
}

/**
*  @brief
*    Get the previous sibling attribute in the DOM
*/
inline XmlAttribute *XmlAttribute::GetPrevious()
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	return (!m_pPreviousAttribute->m_sValue.GetLength() && !m_pPreviousAttribute->m_sName.GetLength()) ? nullptr : m_pPreviousAttribute;
}

inline const XmlAttribute *XmlAttribute::GetPrevious() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	return (!m_pPreviousAttribute->m_sValue.GetLength() && !m_pPreviousAttribute->m_sName.GetLength()) ? nullptr : m_pPreviousAttribute;
}

inline bool XmlAttribute::operator ==(const XmlAttribute &cOther) const
{
	return (cOther.m_sName == m_sName);
}

inline bool XmlAttribute::operator <(const XmlAttribute &cOther) const
{
	return (m_sName < cOther.m_sName);
}

inline bool XmlAttribute::operator >(const XmlAttribute &cOther) const
{
	return (m_sName > cOther.m_sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
