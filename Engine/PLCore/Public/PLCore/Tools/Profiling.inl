/*********************************************************\
 *  File: Profiling.inl                                  *
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
*    Returns the text of the element
*/
inline String ProfileGroupElement::GetText() const
{
	return m_sText;
}

/**
*  @brief
*    Sets the element text
*/
inline bool ProfileGroupElement::SetText(const String &sText)
{
	// Set text
	m_sText = sText;

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
inline ProfileGroupElement &ProfileGroupElement::operator =(const ProfileGroupElement &cSource)
{
	// Copy text
	m_sText = cSource.m_sText;

	// Return this implementation
	return *this;
}




//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the text of the element group
*/
inline String ProfileGroup::GetText() const
{
	return m_sText;
}

/**
*  @brief
*    Sets the element group text
*/
inline bool ProfileGroup::SetText(const String &sText)
{
	// Set text
	m_sText = sText;

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
inline ProfileGroup &ProfileGroup::operator =(const ProfileGroup &cSource)
{
	// Copy text
	m_sText = cSource.m_sText;

	// Call base implementation

	// Return this implementation
	return *this;
}




//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether the profiling system is active or not
*/
inline bool Profiling::IsActive() const
{
	return m_bActive;
}

/**
*  @brief
*    Sets whether the profiling system is active or not
*/
inline void Profiling::SetActive(bool bActive)
{
	m_bActive = bActive;
}

/**
*  @brief
*    Returns the name of the current selected profile group
*/
inline String Profiling::GetSelectedGroup() const
{
	return m_sGroupName;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
