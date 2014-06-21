/*********************************************************\
 *  File: RegEx.cpp                                      *
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


// [TODO] This is just a quick and simple PCRE wrapper. Could need some more attention, e.g.:
//		  - Check the maximum number of values and create a big enough output vector
//		  - Check for unicode strings and use the proper UTF8 functions
//		  - Replace function


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <pcre.h>
#include "PLCore/String/RegEx.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// Maximum number of arguments we can set
static const int MaxGroups	= 11;
static const int VecSize	= (MaxGroups+1) * 3;


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Converts a given wildcard into a regular expression
*/
String RegEx::WildcardToRegEx(const String &sWildcard)
{
	// Begin
	String sString = '^';

	// Convert
	if  (sWildcard.GetFormat() == String::ASCII) {
		// Loop through all characters
		for (uint32 i=0, nLength=sWildcard.GetLength(); i<nLength; i++) {
			const char szCharacter = sWildcard.GetASCII()[i];
			switch (szCharacter) {
				case '*':
					sString += ".*";
					break;

				case '?':
					sString += '.';
					break;

				// Escape special regex-characters
				case '(': case ')': case '[': case ']': case '$':
				case '^': case '.': case '{': case '}': case '|':
				case '\\':
					sString += '\\';
					sString += szCharacter;
					break;

				default:
					sString += szCharacter;
					break;
			}
		}
	} else {
		// Loop through all characters
		for (uint32 i=0, nLength=sWildcard.GetLength(); i<nLength; i++) {
			const wchar_t szCharacter = sWildcard.GetUnicode()[i];
			switch (szCharacter) {
				case L'*':
					sString += L".*";
					break;

				case L'?':
					sString += L'.';
					break;

				// Escape special regex-characters
				case L'(': case L')': case L'[': case L']': case L'$':
				case L'^': case L'.': case L'{': case L'}': case L'|':
				case L'\\':
					sString += L'\\';
					sString += szCharacter;
					break;

				default:
					sString += szCharacter;
					break;
			}
		}
	}

	// End
	sString += '$';

	// Return the regular expression
	return sString;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
RegEx::~RegEx()
{
	// Release expression
	FreePCRE();
}

/**
*  @brief
*    Set regular expression
*/
void RegEx::SetExpression(const String &sExpression, uint32 nFlags)
{
	// Save expression and options
	m_sExpression	= sExpression;
	m_nFlags		= nFlags;

	// Any encoding flags set by the user?
	const bool bEncodingASCII = (m_nFlags & EncodingASCII) != 0;
	const bool bEncodingUTF8  = (m_nFlags & EncodingUTF8)  != 0;

	// ASCII and UTF8 flags set at the same time?
	if (bEncodingASCII && bEncodingUTF8)
		m_nFlags |= ~EncodingASCII; // Remove the ASCII flag!

	// Get the encoding automatically?
	else if (!bEncodingASCII && !bEncodingUTF8)
		m_nFlags |= (sExpression.GetFormat() != String::ASCII) ? EncodingUTF8 : EncodingASCII;

	// Compile expression
	CompilePCRE();
}

/**
*  @brief
*    Analyze the regular expression and store additional internal information
*/
void RegEx::Study()
{
	if (m_pPCRE) {
		const char *pszError;
		m_pExtra = pcre_study(m_pPCRE, 0, &pszError);
	}
}

/**
*  @brief
*    Check if a string matches the given regular expression
*/
bool RegEx::Match(const String &sSubject, uint32 nPosition)
{
	if (m_pPCRE) {
		// Clear results of last operation
		ClearResults();

		// Early escape test: In case we try matching when we are already at the end of the string, we know the result without doing any matches
		if (nPosition < sSubject.GetLength()) {
			// Use ASCII or UTF8 encoding?
			if (m_nFlags & EncodingUTF8) {
				// Match pattern
				int nMatches[VecSize];
				int nMatchCount = pcre_exec(
									m_pPCRE,								// PCRE expression
									m_pExtra,								// Extra data
									sSubject.GetUTF8(),						// Subject string
									sSubject.GetNumOfBytes(String::UTF8),	// Subject length in bytes
									nPosition,								// Start at byte position
									0,										// Default options
									nMatches,								// Output vector with offsets in bytes
									VecSize									// Size of output vector
									);

				// Output vector too small?
				if (nMatchCount == 0) {
					// Allocate big enough output vector and retry
					// [TODO]
					nMatchCount = MaxGroups;
				}

				// Check result
				if (nMatchCount > 0) {
					// Save new byte byte position within the string
					m_nPosition = nMatches[1];

					// Get pointer to subject string
					const char *pszSubject = sSubject.GetUTF8();

					// The first two elements in nMatches describe the result string which was matched by the expression
					// e.g. source string is: "Hallo World!", the expression is "Hal+o" -> first element = "Hallo"
					m_sMatchResult.Copy(&pszSubject[nMatches[0]], nMatches[1] - nMatches[0]);

					// Save matching substrings which have a group description in the expression by index
					// there are only group results when nMatchCount is > 1 and the group results are stored in nMatches starting with index 2
					m_lstGroups.Resize(nMatchCount-1);
					for (int i=1; i<nMatchCount; i++)
						m_lstGroups[i-1].Copy(&pszSubject[nMatches[i*2]], nMatches[i*2+1] - nMatches[i*2]);

					// Get named groups
					int nNameCount;
					pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMECOUNT, &nNameCount);
					const char *pszNameTable;
					pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMETABLE, &pszNameTable);
					int nNameEntrySize;
					pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMEENTRYSIZE, &nNameEntrySize);

					// Return matching substrings by name
					for (int i=0; i<nNameCount; i++) {
						// Get pointer to current entry
						const char *pszEntry = &pszNameTable[i*nNameEntrySize];

						// Get number
						const int nNum = (pszEntry[0] << 8) | pszEntry[1];

						// Get name
						const String sName = String::FromUTF8(&pszEntry[2]);

						// Get substring
						const int nIndex0 = nMatches[nNum*2];
						const int nIndex1 = nMatches[nNum*2+1];

						// Add name->substring to map
						if (nIndex0 >= 0 && nIndex1 >= 0)
							m_mapGroups.Add(sName, String::FromUTF8(&pszSubject[nIndex0], 0, nIndex1 - nIndex0));
						else
							m_mapGroups.Add(sName, "");
					}

					// Done
					return true;
				}
			} else {
				// Match pattern
				int nMatches[VecSize];
				int nMatchCount = pcre_exec(
									m_pPCRE,								// PCRE expression
									m_pExtra,								// Extra data
									sSubject.GetASCII(),					// Subject string
									sSubject.GetNumOfBytes(String::ASCII),	// Subject length in bytes
									nPosition,								// Start at byte position
									0,										// Default options
									nMatches,								// Output vector with offsets in bytes
									VecSize									// Size of output vector
									);

				// Output vector too small?
				if (nMatchCount == 0) {
					// Allocate big enough output vector and retry
					// [TODO]
					nMatchCount = MaxGroups;
				}

				// Check result
				if (nMatchCount > 0) {
					// Save new byte position within the string
					m_nPosition = nMatches[1];

					// Get pointer to subject string
					const char *pszSubject = sSubject.GetASCII();

					// The first two elements in nMatches describe the result string which was matched by the expression
					// e.g. source string is: "Hallo World!", the expression is "Hal+o" -> first element = "Hallo"
					m_sMatchResult.Copy(&pszSubject[nMatches[0]], nMatches[1] - nMatches[0]);

					// Save matching substrings which have a group description in the expression by index
					m_lstGroups.Resize(nMatchCount-1);
					for (int i=1; i<nMatchCount; i++)
						m_lstGroups[i-1].Copy(&pszSubject[nMatches[i*2]], nMatches[i*2+1] - nMatches[i*2]);

					// Get named groups
					int nNameCount;
					pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMECOUNT, &nNameCount);
					const char *pszNameTable;
					pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMETABLE, &pszNameTable);
					int nNameEntrySize;
					pcre_fullinfo(m_pPCRE, m_pExtra, PCRE_INFO_NAMEENTRYSIZE, &nNameEntrySize);

					// Return matching substrings by name
					for (int i=0; i<nNameCount; i++) {
						// Get pointer to current entry
						const char *pszEntry = &pszNameTable[i*nNameEntrySize];

						// Get number
						const int nNum = (pszEntry[0] << 8) | pszEntry[1];

						// Get name
						const String sName = &pszEntry[2];

						// Get substring
						const int nIndex0 = nMatches[nNum*2];
						const int nIndex1 = nMatches[nNum*2+1];

						// Add name->substring to map
						if (nIndex0 >= 0 && nIndex1 >= 0)
							m_mapGroups.Add(sName, String(&pszSubject[nIndex0], true, nIndex1 - nIndex0));
						else
							m_mapGroups.Add(sName, "");
					}

					// Done
					return true;
				}
			}
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Compiles the PCRE expression
*/
void RegEx::CompilePCRE()
{
	// Free former expression
	FreePCRE();

	// Set options
	int nOptions = (IsCaseSensitive() ? 0 : PCRE_CASELESS) | (IsGreedy() ? 0 : PCRE_UNGREEDY);
	if (m_nFlags & Multiline)
		nOptions |= PCRE_MULTILINE;
	if (m_nFlags & DotAll)
		nOptions |= PCRE_DOTALL;

	// Use ASCII or UTF8 encoding?
	if (m_nFlags & EncodingUTF8) {
		const char *pszError;
		int nErrorOffset;

		// Compile regular expression
		m_pPCRE = pcre_compile(m_sExpression.GetUTF8(), nOptions | PCRE_UTF8, &pszError, &nErrorOffset, nullptr);
	} else {
		const char *pszError;
		int nErrorOffset;

		// Compile regular expression
		m_pPCRE = pcre_compile(m_sExpression.GetASCII(), nOptions, &pszError, &nErrorOffset, nullptr);
	}
	if (!m_pPCRE) {
		// Error!
	}
}

/**
*  @brief
*    Deletes the PCRE expression
*/
void RegEx::FreePCRE()
{
	// Free PCRE expression
	if (m_pPCRE) {
		pcre_free(m_pPCRE);
		m_pPCRE  = nullptr;
		m_pExtra = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
