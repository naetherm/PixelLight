/*********************************************************\
 *  File: RegEx.inl                                      *
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
*    Constructor
*/
inline RegEx::RegEx() :
	m_pPCRE(nullptr),
	m_pExtra(nullptr),
	m_nFlags(MatchCaseSensitive | MatchGreedy),
	m_nPosition(0)
{
}

/**
*  @brief
*    Constructor
*/
inline RegEx::RegEx(const String &sExpression, uint32 nFlags) :
	m_pPCRE(nullptr),
	m_pExtra(nullptr),
	m_nFlags(nFlags),
	m_nPosition(0)
{
	// Set expression
	SetExpression(sExpression, nFlags);
}

/**
*  @brief
*    Copy constructor
*/
inline RegEx::RegEx(const RegEx &cRegEx) :
	m_pPCRE(nullptr),
	m_pExtra(nullptr),
	m_nFlags(cRegEx.m_nFlags),
	m_nPosition(0)
{
	// Copy expression
	SetExpression(cRegEx.m_sExpression, cRegEx.m_nFlags);
}

/**
*  @brief
*    Assignment operator
*/
inline RegEx &RegEx::operator =(const RegEx &cRegEx)
{
	// Copy expression
	SetExpression(cRegEx.m_sExpression, cRegEx.m_nFlags);

	// Return this instance
	return *this;
}

/**
*  @brief
*    Get regular expression
*/
inline String RegEx::GetExpression() const
{
	// Return expression
	return m_sExpression;
}

/**
*  @brief
*    Check if the regular expression is valid
*/
inline bool RegEx::IsValid() const
{
	// Return valid flag
	return (m_pPCRE != nullptr);
}

/**
*  @brief
*    Get mode and encoding flags
*/
inline uint32 RegEx::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Is the expression case sensitive?
*/
inline bool RegEx::IsCaseSensitive() const
{
	return ((m_nFlags & MatchCaseSensitive) != 0);
}

/**
*  @brief
*    Is the expression greedy?
*/
inline bool RegEx::IsGreedy() const
{
	return ((m_nFlags & MatchGreedy) != 0);
}

/**
*  @brief
*    Is ASCII encoding used?
*/
inline bool RegEx::IsASCII() const
{
	return ((m_nFlags & EncodingASCII) != 0);
}

/**
*  @brief
*    Is UTF8 encoding used?
*/
inline bool RegEx::IsUTF8() const
{
	return ((m_nFlags & EncodingUTF8) != 0);
}

/**
*  @brief
*    Get current byte position in the subject string
*/
inline int RegEx::GetPosition() const
{
	return m_nPosition;
}

/**
*  @brief
*    Get the group results of the last check
*/
inline const Array<String> &RegEx::GetResults() const
{
	return m_lstGroups;
}

/**
*  @brief
*    Get a matching group from the last check
*/
inline String RegEx::GetResult(uint32 nIndex) const
{
	return m_lstGroups[nIndex];
}

/**
*  @brief
*    Get the results of the last check by name
*/
inline const Map<String, String> &RegEx::GetNameResults() const
{
	return m_mapGroups;
}

/**
*  @brief
*    Get a matching group from the last check by name
*/
inline String RegEx::GetNameResult(const String &sName) const
{
	return m_mapGroups.Get(sName);
}

/**
*  @brief
*    Get result (sub)string which was matched by the expression
*/
inline String RegEx::GetMatchResult() const
{
	return m_sMatchResult;
}

/**
*  @brief
*    Clear all results
*/
inline void RegEx::ClearResults()
{
	// Clear result string
	m_sMatchResult = "";

	// Clear lists for groups
	m_lstGroups.Reset();
	m_mapGroups.Clear();

	// Reset byte position
	m_nPosition = 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
