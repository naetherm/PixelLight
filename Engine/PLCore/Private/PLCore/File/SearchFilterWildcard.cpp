/*********************************************************\
 *  File: SearchFilterWildcard.cpp                       *
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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/File/SearchFilterWildcard.h"


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
SearchFilterWildcard::SearchFilterWildcard(const String &sFilter) :
	m_sFilter(sFilter)
{
}

/**
*  @brief
*    Destructor
*/
SearchFilterWildcard::~SearchFilterWildcard()
{
}

/**
*  @brief
*    Get filter string
*/
String SearchFilterWildcard::GetFilter() const
{
	return m_sFilter;
}


//[-------------------------------------------------------]
//[ Public virtual SearchFilter functions                 ]
//[-------------------------------------------------------]
bool SearchFilterWildcard::CheckFile(const String &sFilename)
{
	// To be Unicode save, we can't just use m_sFilter[nPosFilter] or sFilename[nPosName], we use GetSubstring() instead

	// Check empty filter (matches all files)
	if (!m_sFilter.GetLength())
		return true; // Match

	// Check file matching
	uint32 nNamePos   = 0;
	uint32 nFilterPos = 0;
	while (nFilterPos < m_sFilter.GetLength()) {
		// Get current symbols
		const String sFilterSymbol = m_sFilter.GetSubstring(nFilterPos, 1);
		const String sNameSymbol   = sFilename.GetSubstring(nNamePos,   1);

		// Check filter symbol
		if (sFilterSymbol == '?') {
			// Any character must follow
			if (sNameSymbol.GetLength() != 1)
				return false; // No match
			nNamePos++;
		} else if (sFilterSymbol == '*') {
			// *, check following *'s or ?'s
			int nMinLength = 0;
			while (m_sFilter.GetSubstring(nFilterPos+1, 1) == '*' || m_sFilter.GetSubstring(nFilterPos+1, 1) == '?') {
				if (m_sFilter.GetSubstring(nFilterPos+1, 1) == '?')
					nMinLength++;
				nFilterPos++;
			}

			// Get postfix index
			const uint32 nPostfix = nFilterPos + 1;			// Start index
				  uint32 nEnd     = m_sFilter.GetLength();	// End index sitting on '\0'

			// Find the first appearance of '*' and update 'nEnd' if something was found
			int nIndex = m_sFilter.IndexOf('*', nPostfix);
			if (nIndex >= 0)
				nEnd = nIndex; // Set the new '\0'

			// Find the first appearance of '?' and update 'nEnd' if something was found
			nIndex = m_sFilter.IndexOf('?', nPostfix);
			if (nIndex >= 0 && static_cast<uint32>(nIndex) < nEnd)
				nEnd = nIndex; // Set the new '\0'

			// Search for postfix in name
			int nFound;
			if (nEnd > nPostfix)
				nFound = sFilename.IndexOf(m_sFilter.GetSubstring(nPostfix, nEnd - nPostfix));
			else
				nFound = sFilename.GetLength();

			// Check result
			if (nFound >= 0 && nFound-static_cast<int>(nNamePos) >= nMinLength) {
				// Postfix found
				nNamePos = nFound;
			} else {
				// No match
				return false;
			}
		} else {
			// Character, must be equal
			if (m_sFilter.GetSubstring(nFilterPos, 1) != sFilename.GetSubstring(nNamePos, 1))
				return false; // No match
			nNamePos++;
		}

		// Next
		nFilterPos++;
	}

	// Check matching
	return (nNamePos == sFilename.GetLength());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
