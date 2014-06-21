/*********************************************************\
 *  File: ZipHandle.cpp                                  *
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
#include <zlib.h>
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/File/+Zip/ZipHandle.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
static const int BufferSize			= 16384;	/**< Size of read buffer */
static const int LocalHeaderSize	= 0x1e;		/**< Size of the local header */
static const int CentralDirItemSize = 0x2e;		/**< Size of the central directory item */


//[-------------------------------------------------------]
//[ Encryption functions                                  ]
//[-------------------------------------------------------]
#define CRC_DO1(buf) nCRC = get_crc_table()[(static_cast<int>(nCRC) ^ (*buf++)) & 0xff] ^ (nCRC >> 8);
#define CRC_DO2(buf) CRC_DO1(buf); CRC_DO1(buf);
#define CRC_DO4(buf) CRC_DO2(buf); CRC_DO2(buf);
#define CRC_DO8(buf) CRC_DO4(buf); CRC_DO4(buf);
#define CRC32(c, b) (get_crc_table()[(static_cast<int>(c)^(b))&0xff]^((c)>>8))

uLong __ucrc32(uLong nCRC, const Byte *pBuf, uInt nLen)
{
	if (pBuf == Z_NULL)
		return 0L;
	nCRC = nCRC ^ 0xffffffffL;
	while (nLen >= 8) {
		CRC_DO8(pBuf);
		nLen -= 8;
	}
	if (nLen) {
		do {
			CRC_DO1(pBuf);
		} while (--nLen);
	}
	return nCRC ^ 0xffffffffL;
}

void __Uupdate_keys(uint32 *pnKeys, char c)
{
	pnKeys[0] = CRC32(pnKeys[0],c);
	pnKeys[1] += pnKeys[0] & 0xFF;
	pnKeys[1] = pnKeys[1]*134775813L +1;
	pnKeys[2] = CRC32(pnKeys[2], pnKeys[1] >> 24);
}
char __Udecrypt_byte(uint32 *pnKeys)
{
	const unsigned temp = (static_cast<unsigned>(pnKeys[2]) & 0xffff) | 2;
	return static_cast<char>(((temp * (temp ^ 1)) >> 8) & 0xff);
}
char __zdecode(uint32 *pnKeys, char c)
{
	c ^= __Udecrypt_byte(pnKeys);
	__Uupdate_keys(pnKeys, c);
	return c;
}


//[-------------------------------------------------------]
//[ ZIP file entry                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ZipHandle::ZipEntry::ZipEntry() :
	m_bValid(false),
	m_nVersion(0),
	m_nVersionNeeded(0),
	m_nFlags(0),
	m_nCompressionMethod(0),
	m_nDOSDate(0),
	m_nCRC(0),
	m_nCompressedSize(0),
	m_nUncompressedSize(0),
	m_nSizeFilename(0),
	m_nSizeFileExtra(0),
	m_nSizeFileComment(0),
	m_nDiskNumStart(0),
	m_nInternalAttr(0),
	m_nExternalAttr(0),
	m_nOffsetCurFile(0),
	m_pExtraField(nullptr)
{
}

/**
*  @brief
*    Copy constructor
*/
ZipHandle::ZipEntry::ZipEntry(const ZipEntry &cZipEntry) :
	m_bValid(cZipEntry.m_bValid),
	m_sFilename(cZipEntry.m_sFilename),
	m_sComment(cZipEntry.m_sComment),
	m_nVersion(cZipEntry.m_nVersion),
	m_nVersionNeeded(cZipEntry.m_nVersionNeeded),
	m_nFlags(cZipEntry.m_nFlags),
	m_nCompressionMethod(cZipEntry.m_nCompressionMethod),
	m_nDOSDate(cZipEntry.m_nDOSDate),
	m_nCRC(cZipEntry.m_nCRC),
	m_nCompressedSize(cZipEntry.m_nCompressedSize),
	m_nUncompressedSize(cZipEntry.m_nUncompressedSize),
	m_nSizeFilename(cZipEntry.m_nSizeFilename),
	m_nSizeFileExtra(cZipEntry.m_nSizeFileExtra),
	m_nSizeFileComment(cZipEntry.m_nSizeFileComment),
	m_nDiskNumStart(cZipEntry.m_nDiskNumStart),
	m_nInternalAttr(cZipEntry.m_nInternalAttr),
	m_nExternalAttr(cZipEntry.m_nExternalAttr),
	m_nOffsetCurFile(cZipEntry.m_nOffsetCurFile),
	m_pExtraField(nullptr)
{
	// Copy extra field
	if (m_nSizeFileExtra > 0) {
		m_pExtraField = new uint8[m_nSizeFileExtra];
		MemoryManager::Copy(m_pExtraField, cZipEntry.m_pExtraField, m_nSizeFileExtra);
	}
}

/**
*  @brief
*    Destructor
*/
ZipHandle::ZipEntry::~ZipEntry()
{
	// Clear
	Clear();
}

/**
*  @brief
*    Assignment operator
*/
ZipHandle::ZipEntry &ZipHandle::ZipEntry::operator =(const ZipEntry &cZipEntry)
{
	// Clear first
	Clear();

	// Copy entry
	m_bValid			 = cZipEntry.m_bValid;
	m_sFilename			 = cZipEntry.m_sFilename;
	m_sComment			 = cZipEntry.m_sComment;
	m_nVersion			 = cZipEntry.m_nVersion;
	m_nVersionNeeded	 = cZipEntry.m_nVersionNeeded;
	m_nFlags			 = cZipEntry.m_nFlags;
	m_nCompressionMethod = cZipEntry.m_nCompressionMethod;
	m_nDOSDate			 = cZipEntry.m_nDOSDate;
	m_nCRC				 = cZipEntry.m_nCRC;
	m_nCompressedSize	 = cZipEntry.m_nCompressedSize;
	m_nUncompressedSize  = cZipEntry.m_nUncompressedSize;
	m_nSizeFilename		 = cZipEntry.m_nSizeFilename;
	m_nSizeFileExtra	 = cZipEntry.m_nSizeFileExtra;
	m_nSizeFileComment	 = cZipEntry.m_nSizeFileComment;
	m_nDiskNumStart		 = cZipEntry.m_nDiskNumStart;
	m_nInternalAttr		 = cZipEntry.m_nInternalAttr;
	m_nExternalAttr		 = cZipEntry.m_nExternalAttr;
	m_nOffsetCurFile	 = cZipEntry.m_nOffsetCurFile;
	m_pExtraField		 = nullptr;

	// Copy extra field
	if (m_nSizeFileExtra > 0) {
		m_pExtraField = new uint8[m_nSizeFileExtra];
		MemoryManager::Copy(m_pExtraField, cZipEntry.m_pExtraField, m_nSizeFileExtra);
	}

	// Return this instance
	return *this;
}

/**
*  @brief
*    Clear entry
*/
void ZipHandle::ZipEntry::Clear()
{
	// Delete extra field
	if (m_pExtraField) {
		delete [] m_pExtraField;
		m_pExtraField = nullptr;
	}
}

/**
*  @brief
*    Allocate memory for extra-field
*/
void ZipHandle::ZipEntry::AllocateExtraField(uint16 nSize)
{
	Clear();
	m_nSizeFileExtra = nSize;
	if (m_nSizeFileExtra > 0)
		m_pExtraField = new uint8[m_nSizeFileExtra];
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ZipHandle::ZipHandle() :
	m_bCaseSensitive(true),
	m_bMemBuf(true),
	m_bZipFileValid(false),
	m_nNumEntries(0),
	m_nCommentSize(0),
	m_nBytesBeforeZip(0),
	m_nCentralDirPos(0),
	m_nCentralDirSize(0),
	m_nCentralDirOffset(0),
	m_nDiskNum(0),
	m_nMainDisk(0),
	m_nMainEntries(0),
	m_nCurFile(0),
	m_nCurCentralDirPos(0),
	m_pReadBuffer(new uint8[BufferSize]),
	m_pStream(nullptr),
	m_nPosInZip(0),
	m_nPosLocalExtra(0),
	m_nSizeLocalExtra(0),
	m_nPosInLocalExtra(0),
	m_nCRC32(0),
	m_nCRC32Wait(0),
	m_nReadCompressed(0),
	m_nReadUncompressed(0),
	m_nCompressionMethod(0),
	m_bEncrypted(false),
	m_nEncheadLeft(0),
	m_nCRCEncTest(0)
{
	m_nKeys[0] = 0;
	m_nKeys[1] = 0;
	m_nKeys[2] = 0;
}

/**
*  @brief
*    Destructor
*/
ZipHandle::~ZipHandle()
{
	// Close ZIP-file
	Close();

	// Destroy buffer
	delete [] m_pReadBuffer;
}

/**
*  @brief
*    Check if the ZIP-file is open
*/
bool ZipHandle::IsOpen() const
{
	return m_bZipFileValid;
}

/**
*  @brief
*    Open the ZIP-file
*/
bool ZipHandle::Open(const String &sFilename, const String &sPassword, bool bCaseSensitive, bool bMemBuf, const FileAccess *pAccess)
{
	// Close
	Close();

	// Set new values
	m_cZipFile.Assign(sFilename, pAccess);
	m_bCaseSensitive	 = bCaseSensitive;
	m_bMemBuf			 = bMemBuf;
	m_bZipFileValid		 = false;
	m_nNumEntries		 = 0;
	m_nCommentSize		 = 0;
	m_nBytesBeforeZip	 = 0;
	m_nCentralDirPos	 = 0;
	m_nCentralDirSize	 = 0;
	m_nCentralDirOffset	 = 0;
	m_nDiskNum			 = 0;
	m_nMainDisk			 = 0;
	m_nMainEntries		 = 0;
	m_nCurFile			 = 0;
	m_nCurCentralDirPos	 = 0;
	m_pStream			 = nullptr;
	m_nPosInZip			 = 0;
	m_nPosLocalExtra	 = 0;
	m_nSizeLocalExtra	 = 0;
	m_nPosInLocalExtra	 = 0;
	m_nCRC32			 = 0;
	m_nCRC32Wait		 = 0;
	m_nReadCompressed	 = 0;
	m_nReadUncompressed	 = 0;
	m_nCompressionMethod = 0;
	m_bEncrypted		 = (sPassword.GetLength() != 0);
	m_nEncheadLeft		 = (m_bEncrypted ? 12 : 0);
	m_nCRCEncTest		 = 0;

	// Init encryption data
	m_nKeys[0] = 305419896L;
	m_nKeys[1] = 591751049L;
	m_nKeys[2] = 878082192L;
	for (const char *cp=sPassword; cp && *cp; cp++)
		__Uupdate_keys(m_nKeys, *cp);

	// Open ZIP-file
	m_bZipFileValid = OpenZipFile();
	if (m_bZipFileValid) {
		// Find first file
		return LocateFirstFile();
	}

	// Error!
	m_cZipFile.Close();
	return false;
}

/**
*  @brief
*    Close the ZIP-file
*/
bool ZipHandle::Close()
{
	// Close stream
	CloseFile();

	// Close file
	m_cZipFile.Close();

	// Reset status
	m_bZipFileValid = false;
	return true;
}

/**
*  @brief
*    Returns whether names treated case sensitive
*/
bool ZipHandle::IsCaseSensitive() const
{
	return m_bCaseSensitive;
}


//[-------------------------------------------------------]
//[ Enumerate entries in the ZIP-file                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the pointer to the first entry in the ZIP-file
*/
bool ZipHandle::LocateFirstFile()
{
	// This must stop any read operation
	CloseFile();

	// Seek to central directory
	m_nCurCentralDirPos	= m_nCentralDirOffset;
	m_nCurFile			= 0;
	return ReadCurrentFileInfo();
}

/**
*  @brief
*    Sets the pointer to the next entry in the ZIP-file
*/
bool ZipHandle::LocateNextFile()
{
	// This must stop any read operation
	CloseFile();

	// Check number of files
	if (m_nCurFile + 1 == m_nNumEntries)
		m_cCurFile.Clear();

	// Is the current file valid?
	if (m_cCurFile.m_bValid) {
		// Read information about the file
		m_nCurCentralDirPos += CentralDirItemSize + m_cCurFile.m_nSizeFilename + m_cCurFile.m_nSizeFileExtra + m_cCurFile.m_nSizeFileComment;
		m_nCurFile++;
		return ReadCurrentFileInfo();
	} else {
		// No, return error
		return false;
	}
}

/**
*  @brief
*    Sets the pointer to a specific entry in the ZIP-file
*/
bool ZipHandle::LocateFile(const String &sFilename)
{
	// This must stop any read operation
	CloseFile();

	// Go to first file
	LocateFirstFile();
	if (sFilename.GetLength() == 0)
		return true; // Done

	// Search file
	while (m_cCurFile.m_bValid) {
		// Get current filename (without '/')
		String sCurFile = m_cCurFile.m_sFilename;
		if (sCurFile.GetSubstring(sCurFile.GetLength()-1) == '/')
			sCurFile = sCurFile.GetSubstring(0, sCurFile.GetLength()-1);

		// Check filename
		if (m_bCaseSensitive ? sCurFile == sFilename : sCurFile.CompareNoCase(sFilename)) {
			// Done, file found
			return true;
		}

		// Next file
		LocateNextFile();
	}

	// Error, file not found
	return false;
}

/**
*  @brief
*    Get information about the current entry in the ZIP-file
*/
const ZipHandle::ZipEntry &ZipHandle::GetCurrentFile() const
{
	return m_cCurFile;
}


//[-------------------------------------------------------]
//[ Read file from ZIP-file                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if a file is currently open
*/
bool ZipHandle::IsFileOpen() const
{
	return (m_pStream != nullptr);
}

/**
*  @brief
*    Open the currently selected file
*/
bool ZipHandle::OpenFile()
{
	// Check current file
	if (m_cCurFile.m_bValid) {
		// Check file header
		uint32 nSizeVar;		// Size     of the var field
		uint32 nPosLocalExtra;	// Position of the local extra field
		uint32 nSizeLocalExtra;	// Size     of the local extra field
		uint16 nFlags;			// Flags
		if (CheckFileHeader(nSizeVar, nPosLocalExtra, nSizeLocalExtra, nFlags)) {
			// Init stream to read from the ZIP-file
			m_nPosInZip			 = m_cCurFile.m_nOffsetCurFile + LocalHeaderSize + nSizeVar;
			m_nPosLocalExtra	 = nPosLocalExtra;
			m_nSizeLocalExtra	 = nSizeLocalExtra;
			m_nCRC32Wait		 = m_cCurFile.m_nCRC;
			m_nReadCompressed	 = m_cCurFile.m_nCompressedSize;
			m_nReadUncompressed	 = m_cCurFile.m_nUncompressedSize;
			m_nCompressionMethod = m_cCurFile.m_nCompressionMethod;

			// Encryption data
			bool bExtlochead = ((nFlags&8) != 0);
			if (bExtlochead)
				m_nCRCEncTest = static_cast<char>((m_cCurFile.m_nDOSDate>>8) & 0xff);
			else
				m_nCRCEncTest = static_cast<char>(m_cCurFile.m_nCRC >> 24);

			// Close stream first
			CloseFile();

			// Create new stream
			m_pStream = new z_stream;
			m_pStream->total_out = 0;
			m_pStream->avail_in  = 0;
			m_pStream->zalloc    = static_cast<alloc_func>(nullptr);
			m_pStream->zfree     = static_cast<free_func>(nullptr);
			m_pStream->opaque    = static_cast<voidpf>(nullptr);

			// Init zlib decompression
			if (inflateInit2(m_pStream, -MAX_WBITS) != Z_OK) {
				// Error!
				CloseFile();
				return false;
			}

			// Success
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Close file
*/
bool ZipHandle::CloseFile()
{
	// Delete stream
	if (m_pStream) {
		inflateEnd(m_pStream);
		delete m_pStream;
		m_pStream = nullptr;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Reads data from the file
*/
uint32 ZipHandle::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Check parameters
	if (m_pStream && pBuffer && nSize && nCount) {
		// Init stream
		m_pStream->next_out  = static_cast<Bytef*>(pBuffer);
		m_pStream->avail_out = (static_cast<uInt>(nSize*nCount) < static_cast<uInt>(m_nReadUncompressed)) ? static_cast<uInt>(nSize*nCount) : static_cast<uInt>(m_nReadUncompressed);

		// Read from stream
		int nErr = Z_OK;
		uint32 nReadCount = 0;
		while (m_pStream->avail_out > 0) {
			// Read data
			if (m_pStream->avail_in == 0 && m_nReadCompressed > 0) {
				// Determine size to read
				const uint32 nRead = (BufferSize < m_nReadCompressed) ? BufferSize : m_nReadCompressed;
				if (nRead == 0)
					return 0;

				// Read data
				if (!m_cZipFile.Seek(m_nPosInZip + m_nBytesBeforeZip))
					return 0;
				if (m_cZipFile.Read(m_pReadBuffer, 1, nRead) < nRead)
					return 0;

				// Adjust data
				m_nPosInZip		   += nRead;
				m_nReadCompressed  -= nRead;
				m_pStream->next_in  = static_cast<Bytef*>(m_pReadBuffer);
				m_pStream->avail_in = static_cast<uInt>(nRead);

				// Encryption
				if (m_bEncrypted) {
					char *pszBuffer = reinterpret_cast<char*>(m_pStream->next_in);
					for (uint32 i=0; i<nRead; i++)
						pszBuffer[i] = __zdecode(m_nKeys, pszBuffer[i]);
				}
			}

			// Encryption
			uint32 uDoEncHead = m_nEncheadLeft;
			if (uDoEncHead > m_pStream->avail_in)
				uDoEncHead = m_pStream->avail_in;
			if (uDoEncHead > 0) {
				const char nBufCRC = m_pStream->next_in[uDoEncHead-1];
				m_nReadUncompressed	-= uDoEncHead;
				m_pStream->avail_in	-= uDoEncHead;
				m_pStream->next_in	+= uDoEncHead;
				m_nEncheadLeft		-= uDoEncHead;
				if (!m_nEncheadLeft) {
					if (nBufCRC != m_nCRCEncTest)
						return 0; // Invalid password
				}
			}

			// Check compression method
			if (m_cCurFile.m_nCompressionMethod == 0) {
				// No compression, copy data directly
				const uint32 nCopy = (m_pStream->avail_out < m_pStream->avail_in) ? m_pStream->avail_out : m_pStream->avail_in;
				for (uint32 i=0; i<nCopy; i++)
					*(m_pStream->next_out+i) = *(m_pStream->next_in+i);

				// Adjust data
				m_nCRC32 = crc32(m_nCRC32, m_pStream->next_out, nCopy);
				m_nReadUncompressed	 -= nCopy;
				m_pStream->avail_in	 -= nCopy;
				m_pStream->avail_out -= nCopy;
				m_pStream->next_out	 += nCopy;
				m_pStream->next_in	 += nCopy;
				m_pStream->total_out += nCopy;
				nReadCount			 += nCopy;
			} else {
				// Decompress data
				const uint32 nTotalOutBefore = m_pStream->total_out;
				const Bytef *pBufBefore = m_pStream->next_out;
				nErr = inflate(m_pStream, Z_SYNC_FLUSH);

				// Get size of read data
				const uint32 nTotalOutAfter = m_pStream->total_out;
				const uint32 nOutThis		= nTotalOutAfter - nTotalOutBefore;

				// Adjust data
				m_nCRC32			 = crc32(m_nCRC32, pBufBefore, nOutThis);
				m_nReadUncompressed -= nOutThis;
				nReadCount			+= (nTotalOutAfter - nTotalOutBefore);

				// Check return value
				if (nErr == Z_STREAM_END)
					return nReadCount/nSize;	// End of stream
				if (nErr != Z_OK)
					return 0;					// Error
			}
		}

		// Return number of bytes read
		if (nErr == Z_OK)
			return nReadCount/nSize;
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Sets the starting position
*/
bool ZipHandle::Seek(int32 nOffset, File::ESeek nLocation)
{
	// Check stream
	if (m_pStream) {
		// Calculate offset
		int nOfs;
		if (nLocation == File::SeekCurrent)
			nOfs = Tell() + nOffset;
		else if (nLocation == File::SeekEnd)
			nOfs = m_cCurFile.m_nUncompressedSize + nOffset;
		else if (nLocation == File::SeekSet)
			nOfs = nOffset;
		else
			return false; // Error!

		// [TODO] This is a slow, dirty hack
		CloseFile();
		if (OpenFile()) {
			if (nOfs) {
				uint8 *pBuffer = new uint8[nOfs];
				const int nRead = Read(pBuffer, 1, nOfs);
				delete [] pBuffer;
				return nRead == nOfs;
			} else {
				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the file position
*/
int32 ZipHandle::Tell() const
{
	// Return current position in the stream
	return static_cast<z_off_t>(m_pStream->total_out);
}

/**
*  @brief
*    Returns whether end of file has been reached
*/
bool ZipHandle::IsEof() const
{
	// Check for end of file
	return (m_nReadUncompressed == 0);
}

/**
*  @brief
*    Gets the size of the current file's extra data field
*/
uint32 ZipHandle::GetLocalExtrafieldSize() const
{
	// Return size of the local extra field
	return m_nSizeLocalExtra - m_nPosInLocalExtra;
}

/**
*  @brief
*    Reads the extra data of the current file
*/
bool ZipHandle::GetLocalExtrafield(void *pBuffer, uint32 nSize)
{
	// Check buffer
	if (pBuffer && nSize > 0) {
		// Calculate size to read
		uint32 nReadSize = m_nSizeLocalExtra - m_nPosInLocalExtra;
		if (nSize < nReadSize)
			nReadSize = nSize;

		// Seek to local extrafield
		if (m_cZipFile.Seek(m_nPosLocalExtra +  m_nPosInLocalExtra)) {
			// Read data
			if (m_cZipFile.Read(pBuffer, 1, nReadSize) >= nReadSize)
				return true; // Done
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
*    Copy constructor
*/
ZipHandle::ZipHandle(const ZipHandle &cSource) :
	m_bCaseSensitive(true),
	m_bMemBuf(true),
	m_bZipFileValid(false),
	m_nNumEntries(0),
	m_nCommentSize(0),
	m_nBytesBeforeZip(0),
	m_nCentralDirPos(0),
	m_nCentralDirSize(0),
	m_nCentralDirOffset(0),
	m_nDiskNum(0),
	m_nMainDisk(0),
	m_nMainEntries(0),
	m_nCurFile(0),
	m_nCurCentralDirPos(0),
	m_pReadBuffer(new uint8[BufferSize]),
	m_pStream(nullptr),
	m_nPosInZip(0),
	m_nPosLocalExtra(0),
	m_nSizeLocalExtra(0),
	m_nPosInLocalExtra(0),
	m_nCRC32(0),
	m_nCRC32Wait(0),
	m_nReadCompressed(0),
	m_nReadUncompressed(0),
	m_nCompressionMethod(0),
	m_bEncrypted(false),
	m_nEncheadLeft(0),
	m_nCRCEncTest(0)
{
	// No special implementation because the copy constructor is never used
	m_nKeys[0] = 0;
	m_nKeys[1] = 0;
	m_nKeys[2] = 0;
}

/**
*  @brief
*    Copy operator
*/
ZipHandle &ZipHandle::operator =(const ZipHandle &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Opens the ZIP-file
*/
bool ZipHandle::OpenZipFile()
{
	// Open ZIP-file
	if (!m_cZipFile.Open(File::FileRead | (m_bMemBuf ? File::FileMemBuf : 0)))
		return false;	// Error!

	// Search for central directory
	m_nCentralDirPos = SearchCentralDir();
	if (m_nCentralDirPos == 0)
		return false; // Error!
	if (!m_cZipFile.Seek(m_nCentralDirPos))
		return false; // Error!

	// Local variables
	uint32 nTemp;	// Unused values

	// Ignore signature (checked in SearchCentralDir())
	if (!ReadLong(nTemp))
		return false; // Error!

	// Read the number of disks
	if (!ReadShort(m_nDiskNum))
		return false; // Error!

	// Read disk number with the central directory
	if (!ReadShort(m_nMainDisk))
		return false; // Error!

	// Read number of entries in the central dir on this disk
	if (!ReadShort(m_nNumEntries))
		return false; // Error!

	// Read number of entries in the central dir
	if (!ReadShort(m_nMainEntries))
		return false; // Error!

	// Read the size of the central directory
	if (!ReadLong(m_nCentralDirSize))
		return false; // Error!

	// Read the offset of the central directory
	if (!ReadLong(m_nCentralDirOffset))
		return false; // Error!

	// Read the length of the comment
	if (!ReadShort(m_nCommentSize))
		return false; // Error!

	// Check number of disks (we only support unsplitted ZIP-file)
	if (m_nMainEntries != m_nNumEntries || m_nMainDisk != 0 || m_nDiskNum != 0)
		return false; // Error!

	// Check central directory position
	if (m_nCentralDirPos < m_nCentralDirOffset + m_nCentralDirSize)
		return false; // Error!

	// Calculate start of ZIP-file
	m_nBytesBeforeZip = m_nCentralDirPos - (m_nCentralDirOffset + m_nCentralDirSize);

	// Success
	return true;
}

/**
*  @brief
*    Search for the central directory entry within the ZIP-file
*/
uint32 ZipHandle::SearchCentralDir()
{
	static const uint32 CommentBufferSize = 1024;	/**< Size of the comment buffer in bytes */

	// Get file size
	const int32  nFileSize = m_cZipFile.GetSize();
	const uint32 nMaxRead  = nFileSize < 0xffff ? nFileSize : 0xffff;

	// Create a temporary buffer on the stack
	uint8 nBuffer[CommentBufferSize+4];

	// Try to find the central directory
	uint32 nRead = 4;
	while (nRead < nMaxRead) {
		// Determine read size
		if (nRead + CommentBufferSize > nMaxRead)
			nRead  = nMaxRead;
		else
			nRead += CommentBufferSize;

		// Read data
		const uint32 nReadPos  = nFileSize - nRead;
		const uint32 nReadSize = (CommentBufferSize+4 < nFileSize-nReadPos) ? CommentBufferSize+4 : nFileSize-nReadPos;
		if (m_cZipFile.Seek(nReadPos) && m_cZipFile.Read(nBuffer, 1, nReadSize) >= nReadSize) {
			// Check central directory ID
			for (int i=static_cast<int>(nReadSize)-3; (i--)>0;) {
				if (nBuffer[i]   == 0x50 && nBuffer[i+1] == 0x4b &&
					nBuffer[i+2] == 0x05 && nBuffer[i+3] == 0x06) {
					// Central directory has been found!
					return nReadPos + i;
				}
			}
		}
	}

	// Error, central directory could not be found
	return 0;
}

/**
*  @brief
*    Reads the information about the current file
*/
bool ZipHandle::ReadCurrentFileInfo()
{
	bool bResult = true; // No error by default

	// Seek to current file
	if (!m_cZipFile.Seek(m_nCurCentralDirPos + m_nBytesBeforeZip))
		bResult = false; // Error!

	// Read magic number
	uint32 nMagic;
	if (!ReadLong(nMagic))
		bResult = false; // Error!
	else if (nMagic != 0x02014b50)
		bResult = false; // Error!

	// Read file info
	if (!ReadShort(m_cCurFile.m_nVersion))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nVersionNeeded))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nFlags))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nCompressionMethod))
		bResult = false; // Error!
	if (!ReadLong (m_cCurFile.m_nDOSDate))
		bResult = false; // Error!
	if (!ReadLong (m_cCurFile.m_nCRC))
		bResult = false; // Error!
	if (!ReadLong (m_cCurFile.m_nCompressedSize))
		bResult = false; // Error!
	if (!ReadLong (m_cCurFile.m_nUncompressedSize))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nSizeFilename))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nSizeFileExtra))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nSizeFileComment))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nDiskNumStart))
		bResult = false; // Error!
	if (!ReadShort(m_cCurFile.m_nInternalAttr))
		bResult = false; // Error!
	if (!ReadLong (m_cCurFile.m_nExternalAttr))
		bResult = false; // Error!
	if (!ReadLong (m_cCurFile.m_nOffsetCurFile))
		bResult = false; // Error!
	m_cCurFile.m_cTime.SetDOSDate(m_cCurFile.m_nDOSDate);

	// Read filename
	if (m_cCurFile.m_nSizeFilename) {
		char *pszFilename = new char[m_cCurFile.m_nSizeFilename+1]; // +1 for the terminating zero
		if (m_cZipFile.Read(pszFilename, 1, m_cCurFile.m_nSizeFilename) < m_cCurFile.m_nSizeFilename) {
			m_cCurFile.m_sFilename = "";
			bResult = false; // Error!
			delete [] pszFilename;
		} else {
			// Set terminating zero
			pszFilename[m_cCurFile.m_nSizeFilename] = '\0';

			// The string class takes over the control of the string buffer
			m_cCurFile.m_sFilename = String(pszFilename, false, m_cCurFile.m_nSizeFilename);
		}
	} else {
		m_cCurFile.m_sFilename = "";
	}

	// Read extra field
	m_cCurFile.AllocateExtraField(m_cCurFile.m_nSizeFileExtra);
	if (m_cZipFile.Read(m_cCurFile.m_pExtraField, 1, m_cCurFile.m_nSizeFileExtra) < m_cCurFile.m_nSizeFileExtra)
		bResult = false; // Error!

	// Read file comment
	if (m_cCurFile.m_nSizeFileComment) {
		char *pszComment = new char[m_cCurFile.m_nSizeFileComment+1]; // +1 for the terminating zero
		if (m_cZipFile.Read(pszComment, 1, m_cCurFile.m_nSizeFileComment) < m_cCurFile.m_nSizeFileComment) {
			m_cCurFile.m_sComment = "";
			bResult = false; // Error!
			delete [] pszComment;
		} else {
			// Set terminating zero
			pszComment[m_cCurFile.m_nSizeFileComment] = '\0';

			// The string class takes over the control of the string buffer
			m_cCurFile.m_sComment = String(pszComment, false, m_cCurFile.m_nSizeFileComment);
		}
	} else {
		m_cCurFile.m_sComment = "";
	}

	// Close ZIP-file on error
	if (!bResult)
		m_cZipFile.Close();

	// Set file valid flag
	m_cCurFile.m_bValid = bResult;
	return bResult;
}

/**
*  @brief
*    Checks the local header of the selected file
*/
bool ZipHandle::CheckFileHeader(uint32 &nSizeVar, uint32 &nPosLocalExtra, uint32 &nSizeLocalExtra, uint16 &nFlags)
{
	// Init return values
	nSizeVar        = 0;
	nPosLocalExtra  = 0;
	nSizeLocalExtra = 0;

	// Seek to file header
	if (!m_cZipFile.Seek(m_nBytesBeforeZip + m_cCurFile.m_nOffsetCurFile))
		return false; // Error!

	// Local variables
	uint32 nMagic, nLong;
	uint16 nShort;
	uint16 nSizeFilename, nSizeExtraField;
	bool bResult = true; // No error by default

	// Read magic number
	if (!ReadLong(nMagic))
		bResult = false; // Error!
	else if (nMagic != 0x04034b50)
		bResult = false; // Error!

	// Read flags
	if (!ReadShort(nShort))
		bResult = false; // Error!
	if (!ReadShort(nFlags))
		bResult = false; // Error!

	// Read compression method
	if (!ReadShort(nShort))
		bResult = false; // Error!
	else if (nShort != m_cCurFile.m_nCompressionMethod)
		bResult = false; // Error!

	// Check compression method
	if (m_cCurFile.m_nCompressionMethod != 0 && m_cCurFile.m_nCompressionMethod != Z_DEFLATED)
		bResult = false; // Error!

	// Ignore date/time field
	if (!ReadLong(nLong))
		bResult = false; // Error!

	// Check CRC
	if (!ReadLong(nLong))
		bResult = false; // Error!
	else if (nLong != m_cCurFile.m_nCRC && (nFlags&8) == 0)
		bResult = false; // Error!

	// Check compressed file size
	if (!ReadLong(nLong))
		bResult = false; // Error!
	else if (nLong != m_cCurFile.m_nCompressedSize && (nFlags&8) == 0)
		bResult = false; // Error!

	// Check uncompressed file size
	if (!ReadLong(nLong))
		bResult = false; // Error!
	else if (nLong != m_cCurFile.m_nUncompressedSize && (nFlags&8) == 0)
		bResult = false; // Error!

	// Check size of filename
	if (!ReadShort(nSizeFilename))
		bResult = false; // Error!
	else if (nSizeFilename != m_cCurFile.m_nSizeFilename)
		bResult = false; // Error!

	// Read size of extra field
	if (!ReadShort(nSizeExtraField))
		bResult = false; // Error!

	// Set return values
	nSizeVar		= nSizeFilename + nSizeExtraField;
	nPosLocalExtra  = m_cCurFile.m_nOffsetCurFile + LocalHeaderSize + nSizeFilename;
	nSizeLocalExtra = nSizeExtraField;

	// Done
	return bResult;
}

/**
*  @brief
*    Reads a byte from the ZIP-file
*/
bool ZipHandle::ReadByte(uint8 &nByte)
{
	// Read byte
	const int c = m_cZipFile.GetC();
	if (c != -1) {
		// Return byte
		nByte = static_cast<uint8>(c);

		// Done
		return true;
	}

	// Error!
	nByte = 0;
	return false;
}

/**
*  @brief
*    Reads a unsigned short from the ZIP-file
*/
bool ZipHandle::ReadShort(uint16 &nShort)
{
	// Read bytes
	const int c1 = m_cZipFile.GetC();
	if (c1 != -1) {
		const int c2 = m_cZipFile.GetC();
		if (c2 != -1) {
			// Return short
			nShort = static_cast<uint16>(((static_cast<uint32>(c2) << 8) + c1));

			// Done
			return true;
		}
	}

	// Error!
	nShort = 0;
	return false;
}

/**
*  @brief
*    Reads a unsigned long from the ZIP-file
*/
bool ZipHandle::ReadLong(uint32 &nLong)
{
	// Read bytes
	const int c1 = m_cZipFile.GetC();
	if (c1 != -1) {
		const int c2 = m_cZipFile.GetC();
		if (c2 != -1) {
			const int c3 = m_cZipFile.GetC();
			if (c3 != -1) {
				const int c4 = m_cZipFile.GetC();
				if (c4 != -1) {
					// Return short
					nLong = ((static_cast<uint32>(c4)) << 24) + ((static_cast<uint32>(c3)) << 16) + ((static_cast<uint32>(c2)) << 8) + c1;

					// Done
					return true;
				}
			}
		}
	}

	// Error!
	nLong = 0;
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
