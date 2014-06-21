/*********************************************************\
 *  File: Tokenizer.inl                                  *
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
inline Tokenizer::~Tokenizer()
{
	// Stop tokenizer
	Stop();
}

/**
*  @brief
*    Get delimiters (characters that a treated as whitespace)
*/
inline String Tokenizer::GetDelimiters() const
{
	return m_sDelimiters;
}

/**
*  @brief
*    Set delimiters (characters that a treated as whitespace)
*/
inline void Tokenizer::SetDelimiters(const String &sDelimiters)
{
	m_sDelimiters = sDelimiters;
}

/**
*  @brief
*    Get single characters
*/
inline String Tokenizer::GetSingleChars() const
{
	return m_sSingleChars;
}

/**
*  @brief
*    Set single characters
*/
inline void Tokenizer::SetSingleChars(const String &sSingleChars)
{
	m_sSingleChars = sSingleChars;
}

/**
*  @brief
*    Get characters that are used for quotes
*/
inline String Tokenizer::GetQuotes() const
{
	return m_sQuotes;
}

/**
*  @brief
*    Set characters that are used for quotes
*/
inline void Tokenizer::SetQuotes(const String &sQuotes)
{
	m_sQuotes = sQuotes;
}

/**
*  @brief
*    Get the string that starts a multi-line comment
*/
inline String Tokenizer::GetCommentStartTag() const
{
	return m_sCommentStartTag;
}

/**
*  @brief
*    Set the string that starts a multi-line comment
*/
inline void Tokenizer::SetCommentStartTag(const String &sCommentStartTag)
{
	m_sCommentStartTag = sCommentStartTag;
}

/**
*  @brief
*    Get the string that ends a multi-line comment
*/
inline String Tokenizer::GetCommentEndTag() const
{
	return m_sCommentEndTag;
}

/**
*  @brief
*    Set the string that ends a multi-line comment
*/
inline void Tokenizer::SetCommentEndTag(const String &sCommentEndTag)
{
	m_sCommentEndTag = sCommentEndTag;
}

/**
*  @brief
*    Get the string that starts a single-line comment
*/
inline String Tokenizer::GetSingleLineComment() const
{
	return m_sSingleLineComment;
}

/**
*  @brief
*    Set the string that starts a single-line comment
*/
inline void Tokenizer::SetSingleLineComment(const String &sSingleLineComment)
{
	m_sSingleLineComment = sSingleLineComment;
}

/**
*  @brief
*    Get case sensitivity flag
*/
inline bool Tokenizer::IsCaseSensitive() const
{
	return m_bCaseSensitive;
}

/**
*  @brief
*    Set case sensitivity flag
*/
inline void Tokenizer::SetCaseSensitive(bool bCaseSensitive)
{
	m_bCaseSensitive = bCaseSensitive;
}

/**
*  @brief
*    Returns the current token
*/
inline String Tokenizer::GetToken() const
{
	// Return current token
	return m_sToken;
}

/**
*  @brief
*    Compares the current token with a given string
*/
inline bool Tokenizer::CompareToken(const String &sExpected)
{
	return (m_bCaseSensitive ? m_sToken.Compare(sExpected) : m_sToken.CompareNoCase(sExpected));
}

/**
*  @brief
*    Returns the current position in the stream
*/
inline uint32 Tokenizer::GetPosition() const
{
	// Return current position
	return m_nPosition;
}

/**
*  @brief
*    Returns the current line (counted by '\n' occurrences)
*/
inline uint32 Tokenizer::GetLine() const
{
	// Return current line
	return m_nLine;
}

/**
*  @brief
*    Deletes the last saved state from the stack
*/
inline void Tokenizer::DropState()
{
	// Is the stack empty?
	if (m_cStateStack.GetNumOfElements() > 0) {
		// Pop saved state from stack
		m_cStateStack.Pop();
	}
}

/**
*  @brief
*    Expects the next token to be a number and returns it as an integer value
*/
inline bool Tokenizer::ParseNumber(int &nNumber)
{
	const String sToken = GetNextToken();
	if (sToken.GetLength()) {
		nNumber = sToken.GetInt();

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Expects the next token to be a floating point number and returns it as a float value
*/
inline bool Tokenizer::ParseNumber(float &fNumber)
{
	const String sToken = GetNextToken();
	if (sToken.GetLength()) {
		fNumber = sToken.GetFloat();

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Expects the next token to be a floating point number and returns it as a double value
*/
inline bool Tokenizer::ParseNumber(double &dNumber)
{
	const String sToken = GetNextToken();
	if (sToken.GetLength()) {
		dNumber = sToken.GetDouble();

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Expects the next tokens to be an equation and returns it
*/
inline bool Tokenizer::ParseEquation(String &sName, int &nValue, const String &sEquation)
{
	String sValue;
	if (ParseEquation(sName, sValue, sEquation)) {
		nValue = sValue.GetInt();

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Expects the next tokens to be an equation and returns it
*/
inline bool Tokenizer::ParseEquation(String &sName, float &fValue, const String &sEquation)
{
	String sValue;
	if (ParseEquation(sName, sValue, sEquation)) {
		fValue = sValue.GetFloat();

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Expects the next tokens to be an equation and returns it
*/
inline bool Tokenizer::ParseEquation(String &sName, double &dValue, const String &sEquation)
{
	String sValue;
	if (ParseEquation(sName, sValue, sEquation)) {
		dValue = sValue.GetDouble();

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
