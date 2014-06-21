/*********************************************************\
 *  File: BufferedReaderFile.cpp                         *
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
#include "PLCore/File/File.h"
#include "PLCore/String/BufferedReaderFile.h"


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
BufferedReaderFile::BufferedReaderFile(File &cFile) :
	m_pFile(&cFile)
{
}

/**
*  @brief
*    Destructor
*/
BufferedReaderFile::~BufferedReaderFile()
{
	Close();
}


//[-------------------------------------------------------]
//[ Public virtual BufferedReader functions               ]
//[-------------------------------------------------------]
void BufferedReaderFile::Close()
{
	// Close stream
	if (m_pFile) {
		m_pFile   = nullptr;
		m_sBuffer = "";
	}
}

bool BufferedReaderFile::IsEof() const
{
	// Return if the stream is valid and there is more data to read
	if (m_sBuffer.GetLength())
		return false;
	else if (m_pFile)
		return m_pFile->IsEof();
	else
		return true;
}

String::EFormat BufferedReaderFile::GetStringFormat() const
{
	return m_sBuffer.GetFormat();
}

int BufferedReaderFile::GetChar()
{
	// Does the buffer contain at least one character?
	if (!m_sBuffer.GetLength()) {
		// No, try to read it from the stream
		if (!ReadFromStream(1))
			return 0;
	}

	// Return the current character
	return (m_sBuffer.GetFormat() == String::ASCII) ? m_sBuffer.GetASCII()[0] : m_sBuffer.GetUnicode()[0];
}

String BufferedReaderFile::GetString(uint32 nSize)
{
	// Does the buffer contain enough characters?
	if (m_sBuffer.GetLength() < nSize) {
		// No, try to read it from the stream
		if (!ReadFromStream(nSize - m_sBuffer.GetLength()))
			return "";
	}

	// Return the buffered string
	return (m_sBuffer.GetLength() > nSize) ? m_sBuffer.GetSubstring(0, nSize) : m_sBuffer;
}

int BufferedReaderFile::ReadChar()
{
	if (m_sBuffer.GetLength()) {
		const int nChar = (m_sBuffer.GetFormat() == String::ASCII) ? m_sBuffer.GetASCII()[0] : m_sBuffer.GetUnicode()[0];
		m_sBuffer = m_sBuffer.GetSubstring(1);
		return nChar;
	} else if (m_pFile) {
		const int nChar = m_pFile->GetC();
		if (nChar >= 0)
			return static_cast<char>(nChar);
	}
	return 0;
}

String BufferedReaderFile::ReadString(uint32 nSize)
{
	String sString;
	if (m_sBuffer.GetLength() >= nSize) {
		sString   = m_sBuffer.GetSubstring(0, nSize);
		m_sBuffer = m_sBuffer.GetSubstring(nSize);
	} else if (ReadFromStream(nSize - m_sBuffer.GetLength())) {
		sString   = m_sBuffer;
		m_sBuffer = "";
	}
	return sString;
}

bool BufferedReaderFile::IsString(const String &sString)
{
	// Get the string length
	const uint32 nStringLength = sString.GetLength();

	// Does the buffer contain enough characters?
	if (m_sBuffer.GetLength() < nStringLength) {
		// No, try to read it from the stream
		if (!ReadFromStream(nStringLength - m_sBuffer.GetLength()))
			return false;
	}

	// Compare
	return m_sBuffer.Compare(sString, 0, nStringLength);
}

bool BufferedReaderFile::IsStringNoCase(const String &sString)
{
	// Get the string length
	const uint32 nStringLength = sString.GetLength();

	// Does the buffer contain enough characters?
	if (m_sBuffer.GetLength() < nStringLength) {
		// No, try to read it from the stream
		if (!ReadFromStream(nStringLength - m_sBuffer.GetLength()))
			return false;
	}

	// Compare
	return m_sBuffer.CompareNoCase(sString, 0, nStringLength);
}

uint32 BufferedReaderFile::Tell() const
{
	return m_pFile ? m_pFile->Tell() - m_sBuffer.GetLength() : 0;
}

bool BufferedReaderFile::Seek(uint32 nPos)
{
	if (m_pFile) {
		m_sBuffer = "";
		return m_pFile->Seek(nPos);
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
BufferedReaderFile::BufferedReaderFile(const BufferedReaderFile &cSource) :
	m_pFile(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
BufferedReaderFile &BufferedReaderFile::operator =(const BufferedReaderFile &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Reads bytes from the stream
*/
bool BufferedReaderFile::ReadFromStream(uint32 nSize)
{
	// Check file
	if (nSize > 0 && m_pFile && !m_pFile->IsEof()) {
		if (nSize == 1) {
			// Read one character
			const int nChar = m_pFile->GetC();
			if (nChar > -1) {
				// ASCII or Unicode?
				m_sBuffer += (m_pFile->GetStringFormat() == String::ASCII) ? String(static_cast<char>(nChar)) : String(static_cast<wchar_t>(nChar));

				// Done
				return true;
			}
		} else {
			// Read more than one character

			// ASCII or Unicode?
			if (m_pFile->GetStringFormat() == String::ASCII) {
				// ASCII
				char *pszString = new char[nSize+1];
				if (m_pFile->Read(pszString, 1, nSize) == nSize) {
					pszString[nSize] = '\0';
					m_sBuffer += pszString;
					delete [] pszString;

					// Done
					return true;
				}
				delete [] pszString;
			} else {
				// Unicode - Read one character after another
				for (uint32 i=0; i<nSize; i++) {
					// Read one character
					const int nChar = m_pFile->GetC();
					if (nChar > -1)
						m_sBuffer += static_cast<wchar_t>(nChar);
				}
			}
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
