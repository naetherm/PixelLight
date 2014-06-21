/*********************************************************\
 *  File: TransferControlPoint.inl                       *
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
namespace PLVolume {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
inline TransferControlPoint::TransferControlPoint() :
	m_nIsoValue(0),
	m_fValue(0.0f)
{
}

/**
*  @brief
*    Copy constructor
*/
inline TransferControlPoint::TransferControlPoint(const TransferControlPoint &cSource) :
	m_nIsoValue(cSource.m_nIsoValue),
	m_fValue(cSource.m_fValue)
{
}

/**
*  @brief
*    Constructor for iso-value + value
*/
inline TransferControlPoint::TransferControlPoint(PLCore::uint32 nIsoValue, float fValue) :
	m_nIsoValue(nIsoValue),
	m_fValue(fValue)
{
}

/**
*  @brief
*    Destructor
*/
inline TransferControlPoint::~TransferControlPoint()
{
}

/**
*  @brief
*    Copy operator
*/
inline TransferControlPoint &TransferControlPoint::operator =(const TransferControlPoint &cSource)
{
	// Copy data
	m_nIsoValue = cSource.m_nIsoValue;
	m_fValue    = cSource.m_fValue;

	// Return a reference to this instance
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
inline bool TransferControlPoint::operator ==(const TransferControlPoint &cOther) const
{
	// Compare data (no epsilon environment in here)
	return (m_nIsoValue == cOther.m_nIsoValue && m_fValue == cOther.m_fValue);
}

/**
*  @brief
*    Returns the iso-value
*/
inline PLCore::uint32 TransferControlPoint::GetIsoValue() const
{
	return m_nIsoValue;
}

/**
*  @brief
*    Sets the iso-value
*/
inline void TransferControlPoint::SetIsoValue(PLCore::uint32 nIsoValue)
{
	m_nIsoValue = nIsoValue;
}

/**
*  @brief
*    Returns the value
*/
float TransferControlPoint::GetValue() const
{
	return m_fValue;
}

/**
*  @brief
*    Sets the value
*/
inline void TransferControlPoint::SetValue(float fValue)
{
	m_fValue = fValue;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
