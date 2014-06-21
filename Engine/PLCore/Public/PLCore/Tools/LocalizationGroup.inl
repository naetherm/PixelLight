/*********************************************************\
 *  File: LocalizationGroup.inl                          *
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
*    Returns the name of the group
*/
inline String LocalizationGroup::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Returns the language used as 'from-key'
*/
inline String LocalizationGroup::GetFromLanguage() const
{
	return m_sFromLanguage;
}

/**
*  @brief
*    Sets the language used as 'from-key'
*/
inline void LocalizationGroup::SetFromLanguage(const String &sFrom)
{
	m_sFromLanguage = sFrom;
}

/**
*  @brief
*    Returns the language used as 'to-key'
*/
inline String LocalizationGroup::GetToLanguage() const
{
	return m_sToLanguage;
}

/**
*  @brief
*    Sets the language used as 'to-key'
*/
inline void LocalizationGroup::SetToLanguage(const String &sTo)
{
	m_sToLanguage = sTo;
}

/**
*  @brief
*    Returns the number of texts
*/
inline uint32 LocalizationGroup::GetNumOfTexts() const
{
	return m_lstTexts.GetNumOfElements();
}

/**
*  @brief
*    Returns a text by index
*/
inline LocalizationText *LocalizationGroup::GetText(uint32 nIndex) const
{
	return m_lstTexts[nIndex];
}

/**
*  @brief
*    Returns a text by name
*/
inline LocalizationText *LocalizationGroup::GetText(const String &sName) const
{
	return m_mapTexts.Get(sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
