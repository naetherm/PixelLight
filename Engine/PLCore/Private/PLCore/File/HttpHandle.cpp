/*********************************************************\
 *  File: HttpHandle.cpp                                 *
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
#include "PLCore/File/HttpHandle.h"
#include "PLCore/Tools/Tools.h"


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
HttpHandle::HttpHandle() :
	m_nContentLength(0),
	m_nPosition(0)
{
}

/**
*  @brief
*    Destructor
*/
HttpHandle::~HttpHandle()
{
	// Close connection
	Close();
}

/**
*  @brief
*    Check if the connection is currently open
*/
bool HttpHandle::IsOpen() const
{
	return m_cSocket.IsValid();
}

/**
*  @brief
*    Open the connection
*/
bool HttpHandle::Open(const String &sUrl, const String &sUsername, const String &sPassword)
{
	// Close
	if (IsOpen())
		Close();

	// Save URL
	m_cUrl		= sUrl;
	m_sUsername = sUsername;
	m_sPassword = sPassword;

	// Check URL
	bool bSuccess = false;
	if (m_cUrl.GetProtocol() == "http://") {
		// Get domain and site
		m_sDomain = m_cUrl.GetFirstPath();
		bSuccess = Connect();
	}

	// Check for errors
	if (bSuccess)
		return true;

	// Error!
	m_cSocket.Close();
	return false;
}

/**
*  @brief
*    Close the connection
*/
bool HttpHandle::Close()
{
	// Close socket
	m_cSocket.Close();
	return true;
}

/**
*  @brief
*    Get HTTP response string
*/
String HttpHandle::GetResponse() const
{
	return m_sResponse;
}

/**
*  @brief
*    Get HTTP authenticate string
*/
String HttpHandle::GetAuthenticate() const
{
	return m_sAuthenticate;
}

/**
*  @brief
*    Get HTTP server string
*/
String HttpHandle::GetServer() const
{
	return m_sServer;
}

/**
*  @brief
*    Get HTTP date string
*/
String HttpHandle::GetDate() const
{
	return m_sDate;
}

/**
*  @brief
*    Get HTTP ContentType string
*/
String HttpHandle::GetContentType() const
{
	return m_sContentType;
}

/**
*  @brief
*    Get HTTP content-length
*/
int HttpHandle::GetContentLength() const
{
	return m_nContentLength;
}

/**
*  @brief
*    Reads data from the connection
*/
uint32 HttpHandle::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Check parameters
	if (m_cSocket.IsValid() && pBuffer && nSize && nCount) {
		// Read from socket, until nCount bytes have been processed
		char *pszBuffer = static_cast<char*>(pBuffer);
		uint32 nTotalSize = nSize*nCount, nTotalRead = 0, nRead = 0, nRetry = 0;
		while (nTotalRead < nTotalSize && (nRead > 0 || nRetry < 3)) {
			nRead = m_cSocket.Receive(pszBuffer, nTotalSize - nTotalRead);
			if (nRead > 0) {
				nTotalRead	+= nRead;
				m_nPosition	+= nRead;
				pszBuffer	+= nRead;
				nRetry		 = 0;
			} else {
				nRetry++;
			}
		}
		return nTotalRead / nSize;
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Sets the starting position
*/
bool HttpHandle::Seek(int32 nOffset, File::ESeek nLocation)
{
	// Check socket
	if (m_cSocket.IsValid()) {
		// Calculate offset
		int nOfs;
		if (nLocation == File::SeekCurrent)
			nOfs = m_nPosition + nOffset;
		else if (nLocation == File::SeekEnd)
			nOfs = m_nContentLength + nOffset;
		else if (nLocation == File::SeekSet)
			nOfs = nOffset;
		else
			return false; // Error!

		// This is a slow, dirty hack
		Close();
		if (Connect()) {
			// Is the new position greater than zero?
			if (nOfs) {
				// Read nOfs bytes
				uint8 *pBuffer = new uint8[nOfs];
				Read(pBuffer, 1, nOfs);
				delete [] pBuffer;
				return (m_nPosition == nOfs);
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
int32 HttpHandle::Tell() const
{
	// Return current position in the stream
	return m_nPosition;
}

/**
*  @brief
*    Returns whether end of file has been reached
*/
bool HttpHandle::IsEof() const
{
	// Check for end of file
	return (m_nPosition == m_nContentLength);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Connects to the web server
*/
bool HttpHandle::Connect()
{
	// Recreate socket
	if (!m_cSocket.IsValid())
		m_cSocket = Socket();

	// Connect
	if (m_cSocket.Connect(m_sDomain, 80)) {
		// Compose HTTP request
		String sRequest = "GET " + m_cUrl.GetUrl() + " HTTP/1.0\nUser-Agent: PixelLight HTTP Client\n";
		if (m_sUsername.GetLength())
			sRequest += "Authorization: Basic " + Tools::GetBase64(m_sUsername + ':' + m_sPassword) + '\n';
		sRequest += '\n';

		// Send HTTP request
		if (m_cSocket.Send(sRequest.GetASCII(), sRequest.GetLength()) == static_cast<int>(sRequest.GetLength())) {
			// Read HTTP header
			String sLine = ReadLine();
			while (sLine.GetLength()) {
				if (sLine.GetSubstring(0, 4) == "HTTP")
					m_sResponse = sLine;
				else if (sLine.GetSubstring(0, 18) == "WWW-Authenticate: ")
					m_sAuthenticate = sLine.GetSubstring(18);
				else if (sLine.GetSubstring(0,  8) == "Server: ")
					m_sServer = sLine.GetSubstring(8);
				else if (sLine.GetSubstring(0,  6) == "Date: ")
					m_sDate = sLine.GetSubstring(6);
				else if (sLine.GetSubstring(0, 14) == "Content-Type: ")
					m_sContentType = sLine.GetSubstring(14);
				else if (sLine.GetSubstring(0, 16) == "Content-Length: ")
					m_nContentLength = sLine.GetSubstring(16).GetInt();
				sLine = ReadLine();
			}

			// Done
			m_nPosition = 0;
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Reads a single line from the HTTP header
*/
String HttpHandle::ReadLine()
{
	char szBuffer[256], *pszBuffer = szBuffer;
	while (m_cSocket.Receive(pszBuffer, 1) == 1) {
		if (*pszBuffer == 13) {
			// Ignore '\r'
		} else if (*pszBuffer == 10) {
			// End of line '\n'
			*pszBuffer = 0;
			return szBuffer;
		} else {
			// Next char
			if (pszBuffer < szBuffer + 255)	// Avoid buffer overflow
				pszBuffer++;
		}
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
