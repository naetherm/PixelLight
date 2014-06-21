/*********************************************************\
 *  File: Tokenizer.cpp                                  *
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


// [TODO]
// - BufferedReader is not ideal - too many strings are allocated, this should
//   not be necessary in case of string buffers.
//   Also BufferedReaderFile should work on stream rather than files - but this is
//   only possible if a general stream interface has been created :-)


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/BufferedReaderFile.h"
#include "PLCore/String/BufferedReaderString.h"
#include "PLCore/String/Tokenizer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
const String Tokenizer::EndOfLine = '\n';


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Tokenizer::Tokenizer() :
	m_sDelimiters(" \t\r\n"),
	m_sSingleChars("{}()[]<*>/=,;\""),
	m_sQuotes("\"\'"),
	m_sCommentStartTag("/*"),
	m_sCommentEndTag("*/"),
	m_sSingleLineComment("//"),
	m_bCaseSensitive(false),
	m_pStream(nullptr),
	m_nPosition(0),
	m_nLine(0),
	m_nParseMode(eEatGarbage)
{
}

/**
*  @brief
*    Starts the tokenizer on a string
*/
void Tokenizer::Start(const String &sBuffer)
{
	// Start tokenizer
	Stop();
	m_pStream = new BufferedReaderString(sBuffer);
}

/**
*  @brief
*    Starts the tokenizer on a file
*/
void Tokenizer::Start(File &cFile)
{
	// Start tokenizer
	Stop();
	m_pStream = new BufferedReaderFile(cFile);
}

/**
*  @brief
*    Stops the tokenizer
*/
void Tokenizer::Stop()
{
	// Stop tokenizer
	if (m_pStream) {
		delete m_pStream;
		m_pStream = nullptr;
	}
	m_sToken     = "";
	m_nPosition  = 0;
	m_nLine      = 0;
	m_nParseMode = eEatGarbage;
	m_sEndTag    = "";
	m_cStateStack.Clear();
}

/**
*  @brief
*    Reads all tokens until the end of the stream
*/
Array<String> Tokenizer::GetTokens()
{
	// Read tokens
	Array<String> cTokens;
	String sToken = GetNextToken();
	while (sToken.GetLength()) {
		// Add token
		cTokens.Add(sToken);

		// Next token
		sToken = GetNextToken();
	}

	// Return array of tokens
	return cTokens;
}

/**
*  @brief
*    Reads the next token from the stream
*/
String Tokenizer::GetNextToken()
{
	// Clear the current token
	m_sToken = "";

	// Valid stream?
	if (m_pStream) {
		// ASCII or Unicode?
		if (m_pStream->GetStringFormat() == String::ASCII) {
			// Read ASCII input
			while (!m_pStream->IsEof()) {
				// Get character
				const char nChar = static_cast<char>(m_pStream->GetChar());

				// Do the work
				switch (m_nParseMode) {
					// Eat garbage until the first character of a word has been found
					case eEatGarbage:
						if (m_sCommentStartTag.GetLength() && StreamIsString(m_sCommentStartTag)) {
							// The beginning of a comment has been found
							StreamRead(m_sCommentStartTag.GetLength());
							m_sEndTag	 = m_sCommentEndTag;
							m_nParseMode = eSkipComment;
						} else if (m_sSingleLineComment.GetLength() && StreamIsString(m_sSingleLineComment)) {
							// The beginning of a comment has been found
							StreamRead(m_sSingleLineComment.GetLength());
							m_sEndTag	 = EndOfLine;
							m_nParseMode = eSkipComment;
						} else if (m_sQuotes.IndexOf(nChar) >= 0) {
							// The beginning of a quote has been found
							StreamRead(1);
							m_sEndTag	 = nChar;
							m_nParseMode = eReadQuote;
						} else if (m_sDelimiters.IndexOf(nChar) >= 0) {
							// Only more garbage found
							StreamRead(1);
						} else if (m_sSingleChars.IndexOf(nChar) >= 0) {
							// A single character word has been found
							m_nParseMode = eSingleChar;
						} else {
							// The beginning of a word has been found
							m_nParseMode = eReadWord;
						}
						break;

					// Read a word until a delimiter has been found
					case eReadWord:
						if (m_sCommentStartTag.GetLength() && StreamIsString(m_sCommentStartTag)) {
							// The beginning of a comment has been found
							StreamRead(m_sCommentStartTag.GetLength());
							m_sEndTag	 = m_sCommentEndTag;
							m_nParseMode = eSkipComment;
						} else if (m_sSingleLineComment.GetLength() && StreamIsString(m_sSingleLineComment)) {
							// The beginning of a comment has been found
							StreamRead(m_sSingleLineComment.GetLength());
							m_sEndTag	 = EndOfLine;
							m_nParseMode = eSkipComment;
						} else if (m_sQuotes.IndexOf(nChar) >= 0) {
							// The beginning of a quote has been found
							StreamRead(1);
							m_sEndTag	 = nChar;
							m_nParseMode = eReadQuote;
						} else if (m_sDelimiters.IndexOf(nChar) >= 0) {
							// A delimiter has been found
							StreamRead(1);
							m_nParseMode = eEatGarbage;
						} else if (m_sSingleChars.IndexOf(nChar) >= 0) {
							// A single character word has been found
							m_nParseMode = eSingleChar;
						} else {
							// Read word
							m_sToken += nChar;
							StreamRead(1);
						}

						// Return word if one has been found
						if (m_nParseMode != eReadWord && m_sToken.GetLength())
							return m_sToken;
						break;

					// Read a single character word
					case eSingleChar:
						// Return the single character word
						StreamRead(1);
						m_nParseMode = eEatGarbage;
						m_sToken	 = nChar;
						return m_sToken;

					// Read the beginning of a quote
					case eReadQuote:
						m_nParseMode = eReadQuoteInside;
						m_sToken	 = m_sEndTag;
						return m_sEndTag;

					// Read until the end quote has been found
					case eReadQuoteInside:
						if (StreamIsString(m_sEndTag)) {
							// Return string inside the quote
							m_nParseMode = eReadQuoteEnd;
							if (m_sToken.GetLength())
								return m_sToken;
						} else {
							m_sToken += nChar;
							StreamRead(1);
						}
						break;

					// Read the end of a quote
					case eReadQuoteEnd:
						StreamRead(m_sEndTag.GetLength());
						m_nParseMode = eEatGarbage;
						m_sToken	 = m_sEndTag;
						return m_sEndTag;

					// Skip a comment until the comment end-tag has been found
					case eSkipComment:
						if (StreamIsString(m_sEndTag)) {
							StreamRead(m_sEndTag.GetLength());
							m_nParseMode = eEatGarbage;
						} else {
							StreamRead(1);
						}
						break;
				}
			}
		} else {
			// Read Unicode input
			while (!m_pStream->IsEof()) {
				// Get character
				const wchar_t nChar = static_cast<wchar_t>(m_pStream->GetChar());

				// Do the work
				switch (m_nParseMode) {
					// Eat garbage until the first character of a word has been found
					case eEatGarbage:
						if (m_sCommentStartTag.GetLength() && StreamIsString(m_sCommentStartTag)) {
							// The beginning of a comment has been found
							StreamRead(m_sCommentStartTag.GetLength());
							m_sEndTag	 = m_sCommentEndTag;
							m_nParseMode = eSkipComment;
						} else if (m_sSingleLineComment.GetLength() && StreamIsString(m_sSingleLineComment)) {
							// The beginning of a comment has been found
							StreamRead(m_sSingleLineComment.GetLength());
							m_sEndTag	 = EndOfLine;
							m_nParseMode = eSkipComment;
						} else if (m_sQuotes.IndexOf(nChar) >= 0) {
							// The beginning of a quote has been found
							StreamRead(1);
							m_sEndTag	 = nChar;
							m_nParseMode = eReadQuote;
						} else if (m_sDelimiters.IndexOf(nChar) >= 0) {
							// Only more garbage found
							StreamRead(1);
						} else if (m_sSingleChars.IndexOf(nChar) >= 0) {
							// A single character word has been found
							m_nParseMode = eSingleChar;
						} else {
							// The beginning of a word has been found
							m_nParseMode = eReadWord;
						}
						break;

					// Read a word until a delimiter has been found
					case eReadWord:
						if (m_sCommentStartTag.GetLength() && StreamIsString(m_sCommentStartTag)) {
							// The beginning of a comment has been found
							StreamRead(m_sCommentStartTag.GetLength());
							m_sEndTag	 = m_sCommentEndTag;
							m_nParseMode = eSkipComment;
						} else if (m_sSingleLineComment.GetLength() && StreamIsString(m_sSingleLineComment)) {
							// The beginning of a comment has been found
							StreamRead(m_sSingleLineComment.GetLength());
							m_sEndTag	 = EndOfLine;
							m_nParseMode = eSkipComment;
						} else if (m_sQuotes.IndexOf(nChar) >= 0) {
							// The beginning of a quote has been found
							StreamRead(1);
							m_sEndTag	 = nChar;
							m_nParseMode = eReadQuote;
						} else if (m_sDelimiters.IndexOf(nChar) >= 0) {
							// A delimiter has been found
							StreamRead(1);
							m_nParseMode = eEatGarbage;
						} else if (m_sSingleChars.IndexOf(nChar) >= 0) {
							// A single character word has been found
							m_nParseMode = eSingleChar;
						} else {
							// Read word
							m_sToken += nChar;
							StreamRead(1);
						}

						// Return word if one has been found
						if (m_nParseMode != eReadWord && m_sToken.GetLength())
							return m_sToken;
						break;

					// Read a single character word
					case eSingleChar:
						// Return the single character word
						StreamRead(1);
						m_nParseMode = eEatGarbage;
						m_sToken	 = nChar;
						return m_sToken;

					// Read the beginning of a quote
					case eReadQuote:
						m_nParseMode = eReadQuoteInside;
						m_sToken	 = m_sEndTag;
						return m_sEndTag;

					// Read until the end quote has been found
					case eReadQuoteInside:
						if (StreamIsString(m_sEndTag)) {
							// Return string inside the quote
							m_nParseMode = eReadQuoteEnd;
							if (m_sToken.GetLength())
								return m_sToken;
						} else {
							m_sToken += nChar;
							StreamRead(1);
						}
						break;

					// Read the end of a quote
					case eReadQuoteEnd:
						StreamRead(m_sEndTag.GetLength());
						m_nParseMode = eEatGarbage;
						m_sToken	 = m_sEndTag;
						return m_sEndTag;

					// Skip a comment until the comment end-tag has been found
					case eSkipComment:
						if (StreamIsString(m_sEndTag)) {
							StreamRead(m_sEndTag.GetLength());
							m_nParseMode = eEatGarbage;
						} else {
							StreamRead(1);
						}
						break;
				}
			}
		}
	}

	// Return token
	return m_sToken;
}

/**
*  @brief
*    Expects the next token to be equal to a given string
*/
bool Tokenizer::ExpectToken(const String &sExpected)
{
	PushState();
	const String sToken = GetNextToken();
	if (!sToken.GetLength() || !CompareToken(sExpected)) {
		PopState();

		// The next token is not equal to the string
		return false;
	} else {
		DropState();

		// The next token is equal to the string
		return true;
	}
}

/**
*  @brief
*    Finds a given token in the stream
*/
bool Tokenizer::FindToken(const String &sExpected)
{
	// Check expected string
	if (sExpected.GetLength()) {
		// Read first token
		PushState();
		String sToken = GetNextToken();

		// Read tokens
		while (sToken.GetLength()) {
			// Token found?
			if (CompareToken(sExpected)) {
				PopState();
				return true;
			}

			// Next token
			DropState();
			PushState();
			sToken = GetNextToken();
		}
		DropState();
	}

	// Not found
	return false;
}

/**
*  @brief
*    Saves the current state of the tokenizer on a state stack
*/
void Tokenizer::PushState()
{
	// Push current state onto the stack
	STokenizerState sState;
	sState.m_sToken		= m_sToken;
	sState.m_nPosition	= m_nPosition;
	sState.m_nLine		= m_nLine;
	sState.m_nParseMode	= m_nParseMode;
	sState.m_sEndTag	= m_sEndTag;
	m_cStateStack.Push(sState);
}

/**
*  @brief
*    Restores the last saved state from the stack
*/
void Tokenizer::PopState()
{
	// Is the stack empty?
	if (m_cStateStack.GetNumOfElements() > 0) {
		// Restore state
		STokenizerState sState;
		m_cStateStack.Pop(&sState);
		m_sToken	 = sState.m_sToken;
		m_nPosition	 = sState.m_nPosition;
		m_nLine		 = sState.m_nLine;
		m_nParseMode = sState.m_nParseMode;
		m_sEndTag	 = sState.m_sEndTag;
		if (m_pStream)
			m_pStream->Seek(m_nPosition);
	}
}

/**
*  @brief
*    Expects the next tokens to be a vector and returns it as an array of strings
*/
bool Tokenizer::ParseVector(Array<String> &cVector, const String &sStart, const String &sEnd, const String &sSeparator)
{
	// Start parsing
	PushState();

	// Expect open bracket
	if (ExpectToken(sStart)) {
		// Read elements
		bool bFirst = true;
		while (!ExpectToken(sEnd)) {
			// Expect separator
			if (bFirst) {
				bFirst = false;
			} else if (sSeparator.GetLength() && !ExpectToken(sSeparator)) {
				PopState();

				// Error!
				return false;
			}

			// Expect an element
			String sElement = GetNextToken();
			if (!sElement.GetLength() || CompareToken(sSeparator) || CompareToken(sEnd)) {
				// Error!
				PopState();
				return false;
			}
			cVector.Add(sElement);
		}

		// Success
		DropState();
		return true;
	} else {
		PopState();

		// Error!
		return false;
	}
}

/**
*  @brief
*    Expects the next tokens to be a vector and returns it as an array of ints
*/
bool Tokenizer::ParseVector(Array<int> &cVector, const String &sStart, const String &sEnd, const String &sSeparator)
{
	// Parse string vector
	Array<String> cStringVector;
	if (!ParseVector(cStringVector, sStart, sEnd, sSeparator))
		return false; // Error!

	// Create int vector
	for (uint32 i=0; i<cStringVector.GetNumOfElements(); i++)
		cVector.Add(cStringVector[i].GetInt());

	// Done
	return true;
}

/**
*  @brief
*    Expects the next tokens to be a vector and returns it as an array of floats
*/
bool Tokenizer::ParseVector(Array<float> &cVector, const String &sStart, const String &sEnd, const String &sSeparator)
{
	// Parse string vector
	Array<String> cStringVector;
	if (!ParseVector(cStringVector, sStart, sEnd, sSeparator))
		return false; // Error!

	// Create int vector
	for (uint32 i=0; i<cStringVector.GetNumOfElements(); i++)
		cVector.Add(cStringVector[i].GetFloat());

	// Done
	return true;
}

/**
*  @brief
*    Expects the next tokens to be a vector and returns it as an array of doubles
*/
bool Tokenizer::ParseVector(Array<double> &cVector, const String &sStart, const String &sEnd, const String &sSeparator)
{
	// Parse string vector
	Array<String> cStringVector;
	if (!ParseVector(cStringVector, sStart, sEnd, sSeparator))
		return false; // Error!

	// Create int vector
	for (uint32 i=0; i<cStringVector.GetNumOfElements(); i++)
		cVector.Add(cStringVector[i].GetDouble());

	// Done
	return true;
}

/**
*  @brief
*    Expects the next tokens to be an equation and returns it
*/
bool Tokenizer::ParseEquation(String &sName, String &sValue, const String &sEquation)
{
	// Start parsing
	PushState();

	// Expect name
	String sToken = GetNextToken();
	if (sToken.GetLength()) {
		// Store name
		sName = sToken;

		// Expect equation
		sToken = GetNextToken();
		if (sToken.GetLength()) {
			if (!sEquation.GetLength() || CompareToken(sEquation)) {
				// Read value
				sToken = GetNextToken();
				if (sToken.GetLength()) {
					// Store value
					sValue = sToken;
					DropState();

					// Done
					return true;
				}
			}
		}
	}

	// An error has occurred
	PopState();
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Tokenizer::Tokenizer(const Tokenizer &cSource) :
	m_sDelimiters(" \t\r\n"),
	m_sSingleChars("{}()[]<*>/=,;\""),
	m_sQuotes("\"\'"),
	m_sCommentStartTag("/*"),
	m_sCommentEndTag("*/"),
	m_sSingleLineComment("//"),
	m_bCaseSensitive(false),
	m_pStream(nullptr),
	m_nPosition(0),
	m_nLine(0),
	m_nParseMode(eEatGarbage)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Tokenizer &Tokenizer::operator =(const Tokenizer &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Checks if the next string in the stream equals the given one
*/
inline bool Tokenizer::StreamIsString(const String &sString)
{
	if (!m_pStream->IsEof())
		return (m_bCaseSensitive ? m_pStream->IsString(sString) : m_pStream->IsStringNoCase(sString));
	else
		return false;
}

/**
*  @brief
*    Reads the next characters of the stream
*/
void Tokenizer::StreamRead(uint32 nCount)
{
	// Read next character(s)
	for (uint32 i=0; i<nCount; i++) {
		if (m_pStream->ReadChar() == '\n')
			m_nLine++;
	}
	m_nPosition += nCount;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
