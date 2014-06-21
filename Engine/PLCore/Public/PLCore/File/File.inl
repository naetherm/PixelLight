/*********************************************************\
 *  File: File.inl                                       *
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
#include "PLCore/File/FileImpl.h"


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
inline File::File() :
	m_pMemBuf(nullptr),
	m_nMemBufSize(0),
	m_nMemBufPos(0),
	m_nMemBufAccess(0)
{
}

/**
*  @brief
*    Constructor
*/
inline File::File(const String &sUrl, const FileAccess *pAccess) : FileObject(sUrl, pAccess),
	m_pMemBuf(nullptr),
	m_nMemBufSize(0),
	m_nMemBufPos(0),
	m_nMemBufAccess(0)
{
}

/**
*  @brief
*    Constructor
*/
inline File::File(const Url &cUrl, const FileAccess *pAccess) : FileObject(cUrl, pAccess),
	m_pMemBuf(nullptr),
	m_nMemBufSize(0),
	m_nMemBufPos(0),
	m_nMemBufAccess(0)
{
}

/**
*  @brief
*    Constructor for a standard OS stream
*/
inline File::File(FILE *pFile, uint32 nAccess) : FileObject(pFile, nAccess),
	m_pMemBuf(nullptr),
	m_nMemBufSize(0),
	m_nMemBufPos(0),
	m_nMemBufAccess(0)
{
}

/**
*  @brief
*    Constructor
*/
inline File::File(handle hFile) : FileObject(hFile),
	m_pMemBuf(nullptr),
	m_nMemBufSize(0),
	m_nMemBufPos(0),
	m_nMemBufAccess(0)
{
}

/**
*  @brief
*    Destructor
*/
inline File::~File()
{
	// Close memory buffered
	MemBufClose();
}

/**
*  @brief
*    Create file
*/
inline bool File::Create(bool bAlways)
{
	// Memory buffered file?
	if (m_pMemBuf)
		MemBufClose();

	// Create file
	return m_pFileImpl ? m_pFileImpl->CreateNewFile(bAlways) : false;
}

/**
*  @brief
*    Delete file
*/
inline bool File::Delete()
{
	// Memory buffered file?
	if (m_pMemBuf)
		MemBufClose();

	// Delete file
	return m_pFileImpl ? m_pFileImpl->Delete() : false;
}

/**
*  @brief
*    Close the file
*/
inline void File::Close()
{
	// Memory buffered file?
	if (m_pMemBuf)
		MemBufClose();

	// Close file
	if (m_pFileImpl)
		m_pFileImpl->Close();
}

/**
*  @brief
*    Returns whether the file is memory buffered (opened with flag FileMemBuf)
*/
inline bool File::IsMemoryBuffered() const
{
	return (m_pMemBuf != nullptr);
}

/**
*  @brief
*    Returns whether the file is open
*/
inline bool File::IsOpen() const
{
	// Check if file is open
	if (m_pMemBuf)
		return true;
	else if (m_pFileImpl)
		return m_pFileImpl->IsOpen();
	else
		return false;
}

/**
*  @brief
*    Returns whether the file is readable
*/
inline bool File::IsReadable() const
{
	// Check readable
	if (m_pMemBuf)
		return true;
	else if (m_pFileImpl)
		return m_pFileImpl->IsReadable();
	else
		return false;
}

/**
*  @brief
*    Returns whether the file is writable
*/
inline bool File::IsWritable() const
{
	// Check writable
	if (m_pMemBuf)
		return false;
	else if (m_pFileImpl)
		return m_pFileImpl->IsWritable();
	else
		return false;
}

/**
*  @brief
*    Returns the string encoding format to use when dealing with string functions
*/
inline String::EFormat File::GetStringFormat() const
{
	// Check string encoding format
	if (m_pMemBuf)
		return String::ASCII;
	else if (m_pFileImpl)
		return m_pFileImpl->GetStringFormat();
	else
		return String::ASCII;
}

/**
*  @brief
*    Returns whether end of file has been reached
*/
inline bool File::IsEof() const
{
	// Check EOF
	if (m_pMemBuf)
		return (m_nMemBufPos >= m_nMemBufSize);
	else if (m_pFileImpl)
		return m_pFileImpl->IsEof();
	else
		return false;
}

/**
*  @brief
*    Reads a character
*/
inline int File::GetC()
{
	// Read character
	if (m_pMemBuf)
		return MemBufGetC();
	else if (m_pFileImpl)
		return m_pFileImpl->GetC();
	else
		return 0;
}

/**
*  @brief
*    Writes a character
*/
inline bool File::PutC(int nChar)
{
	// Open file
	if (m_pMemBuf)
		return false;
	else if (m_pFileImpl)
		return m_pFileImpl->PutC(nChar);
	else
		return false;
}

/**
*  @brief
*    Reads a string
*/
inline String File::GetS()
{
	// Read string
	if (m_pMemBuf)
		return MemBufGetS();
	else if (m_pFileImpl)
		return m_pFileImpl->GetS();
	else
		return "";
}

/**
*  @brief
*    Writes a string
*/
inline int File::PutS(const String &sString)
{
	// Write string
	if (m_pMemBuf)
		return false;
	else if (m_pFileImpl)
		return m_pFileImpl->PutS(sString);
	else
		return 0;
}

/**
*  @brief
*    Reads the given number of bytes
*/
inline uint32 File::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Read data
	if (m_pMemBuf)
		return MemBufRead(pBuffer, nSize, nCount);
	else if (m_pFileImpl)
		return m_pFileImpl->Read(pBuffer, nSize, nCount);
	else
		return 0;
}

/**
*  @brief
*    Writes the given number of bytes
*/
inline uint32 File::Write(const void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Write data
	if (m_pMemBuf)
		return false;
	else if (m_pFileImpl)
		return m_pFileImpl->Write(pBuffer, nSize, nCount);
	else
		return 0;
}

/**
*  @brief
*    Flushes the file buffer
*/
inline bool File::Flush()
{
	// Flush buffer
	if (m_pMemBuf)
		return false;
	else if (m_pFileImpl)
		return m_pFileImpl->Flush();
	else
		return false;
}

/**
*  @brief
*    Sets the starting position
*/
inline bool File::Seek(int32 nOffset, ESeek nLocation)
{
	// Seek
	if (m_pMemBuf)
		return MemBufSeek(nOffset, nLocation);
	else if (m_pFileImpl)
		return m_pFileImpl->Seek(nOffset, nLocation);
	else
		return false;
}

/**
*  @brief
*    Gets the current position of the file pointer
*/
inline int32 File::Tell() const
{
	// Get seek position
	if (m_pMemBuf)
		return m_nMemBufPos;
	else if (m_pFileImpl)
		return m_pFileImpl->Tell();
	else
		return 0;
}

/**
*  @brief
*    Returns the file size
*/
inline uint32 File::GetSize() const
{
	// Get file size
	if (m_pMemBuf)
		return m_nMemBufSize;
	else if (m_pFileImpl)
		return m_pFileImpl->GetSize();
	else
		return 0;
}

/**
*  @brief
*    Writes a string into the file
*/
inline bool File::Print(const String &sString)
{
	// Write string
	return (PutS(sString) == static_cast<int>(sString.GetLength()));
}

/**
*  @brief
*    Writes a string and a newline into the file
*/
inline bool File::PrintLn(const String &sString)
{
	// Write string and newline
	PutS(sString);
	return PutC('\n');
}

/**
*  @brief
*    Returns a pointer to the buffer for memory buffered file
*/
inline const uint8 *File::GetMemoryBuffer() const
{
	return m_pMemBuf;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
