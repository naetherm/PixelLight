/*********************************************************\
 *  File: XmlParsingData.cpp                             *
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
#include "PLCore/Xml/XmlParsingData.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
void XmlParsingData::Stamp(const char *pszNow, XmlBase::EEncoding nEncoding)
{
	// Do nothing if the tab size is 0
	if (m_nTabSize < 1)
		return;

	// Get the current row, column
	int nRow    = m_cCursor.nRow;
	int nColumn = m_cCursor.nColumn;
	const char *pszData = m_pszStamp;

	while (pszData < pszNow) {
		// Treat pszData as unsigned, so we have a happy compiler
		const unsigned char *pU = reinterpret_cast<const unsigned char*>(pszData);

		switch (*pU) {
			case 0:
				// We *should* never get here, but in case we do, don't
				// advance past the terminating zero character, ever
				return;

			case '\r':
				// Bump down to the next line
				++nRow;
				nColumn = 0;

				// Eat the character
				++pszData;

				// Check for \r\n sequence, and treat this as a single character
				if (*pszData == '\n')
					++pszData;
				break;

			case '\n':
				// Bump down to the next line
				++nRow;
				nColumn = 0;

				// Eat the character
				++pszData;

				// Check for \n\r sequence, and treat this as a single
				// character.  (Yes, this bizarre thing does occur still
				// on some arcane platforms...)
				if (*pszData == '\r')
					++pszData;
				break;

			case '\t':
				// Eat the character
				++pszData;

				// Skip to next tab stop
				nColumn = (nColumn / m_nTabSize + 1) * m_nTabSize;
				break;

			case XmlBase::UTF_LEAD_0:
				if (nEncoding == XmlBase::EncodingUTF8) {
					if (*(pszData+1) && *(pszData+2)) {
						// In these cases, don't advance the column. These are 0-width spaces.
						if (*(pU+1)==XmlBase::UTF_LEAD_1 && *(pU+2)==XmlBase::UTF_LEAD_2)
							pszData += 3;
						else if (*(pU+1)==0xbfU && *(pU+2)==0xbeU)
							pszData += 3;
						else if (*(pU+1)==0xbfU && *(pU+2)==0xbfU)
							pszData += 3;
						else
							{ pszData +=3; ++nColumn; }	// A normal character
					}
				} else {
					++pszData;
					++nColumn;
				}
				break;

			default:
				if (nEncoding == XmlBase::EncodingUTF8) {
					// Eat the 1 to 4 byte utf8 character
					int nStep = XmlBase::utf8ByteTable[static_cast<unsigned char>(*pszData)];
					if (nStep == 0)
						nStep = 1;	// Error case from bad encoding, but handle gracefully
					pszData += nStep;

					// Just advance one column, of course
					++nColumn;
				} else {
					++pszData;
					++nColumn;
				}
				break;
		}
	}
	m_cCursor.nRow    = nRow;
	m_cCursor.nColumn = nColumn;
	m_pszStamp = pszData;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
