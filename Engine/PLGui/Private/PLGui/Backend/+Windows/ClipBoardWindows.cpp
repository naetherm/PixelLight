/*********************************************************\
 *  File: ClipBoardWindows.cpp                           *
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
#include "PLGui/+Windows/PLGuiWindowsIncludes.h"
#include "PLGui/Gui/Data/DataObject.h"
#include "PLGui/Backend/+Windows/ClipBoardWindows.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ClipBoardWindows::ClipBoardWindows(ClipBoard &cClipBoard) : ClipBoardImpl(cClipBoard)
{
}

/**
*  @brief
*    Destructor
*/
ClipBoardWindows::~ClipBoardWindows()
{
}


//[-------------------------------------------------------]
//[ Private virtual ClipBoardImpl functions               ]
//[-------------------------------------------------------]
DataObject ClipBoardWindows::GetData()
{
	// Is Unicode text on the clipboard? Even if we copy ASCII into the clipboard this test
	// doesn't fail - we can't figure out if ASCII is enough :(
	if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {
		// First at all, open the clipboard
		if (OpenClipboard(nullptr)) {
			// Get the clipboard text
			String sText;
			HANDLE hData = GetClipboardData(CF_UNICODETEXT);
			if (hData) {
				wchar_t *pszBuffer = static_cast<wchar_t*>(GlobalLock(hData));
				sText = pszBuffer; // We need to copy the text!
				GlobalUnlock(hData);
			}

			// Close the clipboard
			CloseClipboard();

			// Return text
			return DataObject(sText);
		}

	// Is ASCII text on the clipboard?
	} else if (IsClipboardFormatAvailable(CF_TEXT)) {
		// First at all, open the clipboard
		if (OpenClipboard(nullptr)) {
			// Get the clipboard text
			String sText;
			HANDLE hData = GetClipboardData(CF_TEXT);
			if (hData) {
				char *pszBuffer = static_cast<char*>(GlobalLock(hData));
				sText = pszBuffer; // We need to copy the text!
				GlobalUnlock(hData);
			}

			// Close the clipboard
			CloseClipboard();

			// Return text
			return DataObject(sText);
		}
	}

	// Error!
	return DataObject();
}

void ClipBoardWindows::SetData(const DataObject &cData)
{
	// Check format
	if (cData.GetType() == DataString) {
		// Text
		String sText = cData.GetString();
		if (sText.GetLength()) {
			// First at all, open the clipboard
			if (OpenClipboard(0)) {
				// Empty the clipboard
				EmptyClipboard();

				// ASCII or Unicode/UTF8 text?
				if (sText.GetFormat() == String::ASCII) {
					// Allocate a global memory object for the text
					HGLOBAL hClipBuffer = GlobalAlloc(GMEM_MOVEABLE, sText.GetLength() + 1);
					if (hClipBuffer) {
						// Lock the handle and copy the text to the buffer
						char *pszBuffer = static_cast<char*>(GlobalLock(hClipBuffer));
						if (pszBuffer) {
							MemoryManager::Copy(pszBuffer, sText.GetASCII(), sText.GetLength());
							pszBuffer[sText.GetLength()] = '\0'; // Set terminating zero
						}
						GlobalUnlock(hClipBuffer);

						// Place the handle on the clipboard
						SetClipboardData(CF_TEXT, hClipBuffer);
					}
				} else {
					// Allocate a global memory object for the text
					size_t  nCharacterSize = sizeof(wchar_t);
					HGLOBAL hClipBuffer    = GlobalAlloc(GMEM_MOVEABLE, (sText.GetLength() + 1)*nCharacterSize);
					if (hClipBuffer) {
						// Lock the handle and copy the text to the buffer
						wchar_t *pszBuffer = static_cast<wchar_t*>(GlobalLock(hClipBuffer));
						if (pszBuffer) {
							MemoryManager::Copy(pszBuffer, sText.GetUnicode(), static_cast<uint32>(sText.GetLength()*nCharacterSize));
							pszBuffer[sText.GetLength()] = '\0'; // Set terminating zero
						}
						GlobalUnlock(hClipBuffer);

						// Place the handle on the clipboard
						SetClipboardData(CF_UNICODETEXT, hClipBuffer);
					}
				}

				// Close the clipboard
				CloseClipboard();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
