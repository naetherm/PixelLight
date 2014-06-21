/*********************************************************\
 *  File: HttpClient.cpp                                 *
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
#include "PLCore/Tools/Tools.h"
#include "PLCore/String/ParseTools.h"
#include "PLCore/Network/Http/HttpClient.h"
#include "PLCore/Network/Http/HttpClientConnection.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ General                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
HttpClient::HttpClient() :
	m_nClientSignature(ClientPixelLight),
	m_nHttpProtocol(Http11),
	m_nConnection(ConnectionClose),
	m_nHttpAuth(NoAuth),
	m_nPort(80),
	m_pConnection(nullptr),
	m_nChunkSize(-1),
	m_nChunkLeft(0)
{
}

/**
*  @brief
*    Destructor
*/
HttpClient::~HttpClient()
{
	// Close connection
	Close();
}

//[-------------------------------------------------------]
//[ Functions                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Open connection to server
*/
bool HttpClient::Open(const String &sAddress, uint32 nPort)
{
	// Close former connection
	Disconnect();

	// Open connection
	m_pConnection = static_cast<HttpClientConnection*>(Connect(sAddress, nPort));
	if (m_pConnection) {
		// Save address
		m_sAddress = sAddress;
		m_nPort    = nPort;

		// Reset data
		m_cHttpHeader.Clear();

		// Connection established
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Close HTTP session
*/
void HttpClient::Disconnect()
{
	// Close connection
	if (m_pConnection) {
		m_pConnection->Disconnect();
		m_pConnection = nullptr;
		m_cHttpHeader.Clear();
	}
}

/**
*  @brief
*    Open a HTTP resource (GET command)
*/
bool HttpClient::Get(const String &sURL)
{
	// Is a connection open?
	if (IsConnected()) {
		// Reset
		m_cHttpHeader.Clear();

		// Send HTTP request
		if (m_nHttpProtocol == Http11)							m_pConnection->Send("GET " + sURL + " HTTP/1.1\r\n");
		else													m_pConnection->Send("GET " + sURL + " HTTP/1.0\r\n");

		// Host
		if (m_nPort != 80)										m_pConnection->Send("Host: " + m_sAddress + ':' + m_nPort + "\r\n");
		else													m_pConnection->Send("Host: " + m_sAddress + "\r\n");

		// User agent
		if (m_nClientSignature == ClientPixelLight)				m_pConnection->Send("User-Agent: PixelLight HTTP Client\r\n");
		else if (m_nClientSignature == ClientMozilla)			m_pConnection->Send("User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3\r\n");

		// Connection
		if (m_nConnection == ConnectionKeepAlive)				m_pConnection->Send("Connection: keep-alive\r\n");
		else													m_pConnection->Send("Connection: close\r\n");

		// Authentication
		if (m_nHttpAuth == BasicAuth)							m_pConnection->Send("Authorization: Basic " + Tools::GetBase64(m_sUsername + ':' + m_sPassword) + "\r\n");

		// Send Header
		m_pConnection->Send("\r\n");

		// Read answer
		m_cHttpHeader = m_pConnection->ReadHttpHeader();

		// Return if header is read successfully
		return m_cHttpHeader.IsComplete();
	}

	// Error!
	return false;
}

/**
*  @brief
*    Open a HTTP resource and get partial data (GET command)
*/
bool HttpClient::GetPartial(const String &sURL, uint32 nPos, int32 nSize)
{
	// Is a connection open and do we use HTTP 1.1? (Partial data is not supported by HTTP 1.0)
	if (IsConnected() && m_nHttpProtocol == Http11) {
		// Reset
		m_cHttpHeader.Clear();

		// Send HTTP request
		if (m_nHttpProtocol == Http11)							m_pConnection->Send("GET " + sURL + " HTTP/1.1\r\n");
		else													m_pConnection->Send("GET " + sURL + " HTTP/1.0\r\n");

		// Host
		if (m_nPort != 80)										m_pConnection->Send("Host: " + m_sAddress + ':' + m_nPort + "\r\n");
		else													m_pConnection->Send("Host: " + m_sAddress + "\r\n");

		// User agent
		if (m_nClientSignature == ClientPixelLight)				m_pConnection->Send("User-Agent: PixelLight HTTP Client\r\n");
		else if (m_nClientSignature == ClientMozilla)			m_pConnection->Send("User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3\r\n");

		// Connection
		if (m_nConnection == ConnectionKeepAlive)				m_pConnection->Send("Connection: keep-alive\r\n");
		else													m_pConnection->Send("Connection: close\r\n");

		// Authentication
		if (m_nHttpAuth == BasicAuth)							m_pConnection->Send("Authorization: Basic " + Tools::GetBase64(m_sUsername + ':' + m_sPassword) + "\r\n");

		// Get partial data
		String sPartial = String("Range: bytes=") + nPos + '-';
		if (nSize > 0)
			sPartial += nPos+nSize-1;
		sPartial += "\r\n";
		m_pConnection->Send(sPartial);

		// Send Header
		m_pConnection->Send("\r\n");

		// Read answer
		m_cHttpHeader = m_pConnection->ReadHttpHeader();

		// Return if header is read successfully
		return m_cHttpHeader.IsComplete();
	}

	// Error!
	return false;
}

/**
*  @brief
*    Open a HTTP resource (POST command)
*/
bool HttpClient::Post(const String &sURL, const String &sPostData)
{
	// Is a connection open?
	if (IsConnected()) {
		// Reset
		m_cHttpHeader.Clear();

		// Send HTTP request
		if (m_nHttpProtocol == Http11)							m_pConnection->Send("POST " + sURL + " HTTP/1.1\r\n");
		else													m_pConnection->Send("POST " + sURL + " HTTP/1.0\r\n");

		// Host
		if (m_nPort != 80)										m_pConnection->Send("Host: " + m_sAddress + ':' + m_nPort + "\r\n");
		else													m_pConnection->Send("Host: " + m_sAddress + "\r\n");

		// User agent
		if (m_nClientSignature == ClientPixelLight)				m_pConnection->Send("User-Agent: PixelLight HTTP Client\r\n");
		else if (m_nClientSignature == ClientMozilla)			m_pConnection->Send("User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3\r\n");

		// Connection
		if (m_nConnection == ConnectionKeepAlive)				m_pConnection->Send("Connection: keep-alive\r\n");
		else													m_pConnection->Send("Connection: close\r\n");

		// Authentication
		if (m_nHttpAuth == BasicAuth)							m_pConnection->Send("Authorization: Basic " + Tools::GetBase64(m_sUsername + ':' + m_sPassword) + "\r\n");

		// Send Header
		m_pConnection->Send("\r\n");

		// Send post data
		m_pConnection->Send(sPostData);

		// Read answer
		m_cHttpHeader = m_pConnection->ReadHttpHeader();

		// Return if header is read successfully
		return m_cHttpHeader.IsComplete();
	}

	// Error!
	return false;
}

/**
*  @brief
*    Delete a HTTP resource (DELETE command)
*/
bool HttpClient::Delete(const String &sURL)
{
	// Is a connection open?
	if (IsConnected()) {
		// Reset
		m_cHttpHeader.Clear();

		// Send HTTP request
		if (m_nHttpProtocol == Http11)							m_pConnection->Send("DELETE " + sURL + " HTTP/1.1\r\n");
		else													m_pConnection->Send("DELETE " + sURL + " HTTP/1.0\r\n");

		// Host
		if (m_nPort != 80)										m_pConnection->Send("Host: " + m_sAddress + ':' + m_nPort + "\r\n");
		else													m_pConnection->Send("Host: " + m_sAddress + "\r\n");

		// User agent
		if (m_nClientSignature == ClientPixelLight)				m_pConnection->Send("User-Agent: PixelLight HTTP Client\r\n");
		else if (m_nClientSignature == ClientMozilla)			m_pConnection->Send("User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; de; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3\r\n");

		// Connection
		if (m_nConnection == ConnectionKeepAlive)				m_pConnection->Send("Connection: keep-alive\r\n");
		else													m_pConnection->Send("Connection: close\r\n");

		// Authentication
		if (m_nHttpAuth == BasicAuth)							m_pConnection->Send("Authorization: Basic " + Tools::GetBase64(m_sUsername + ':' + m_sPassword) + "\r\n");

		// Send Header
		m_pConnection->Send("\r\n");

		// Read answer
		m_cHttpHeader = m_pConnection->ReadHttpHeader();

		// Return if header is read successfully
		return m_cHttpHeader.IsComplete();
	}

	// Error!
	return false;
}

/**
*  @brief
*    Read data from HTTP connection
*/
int HttpClient::Read(char *pBuffer, uint32 nSize)
{
	// Check for chunked transfer
	if (m_cHttpHeader.IsChunked()) {
		// Transfer is chunked. Read header if not already done
		if (m_nChunkSize == -1) {
			// Read chunk header
			String sHeader = m_pConnection->ReadLine().RemoveLineEndings();
			const int nSpace = sHeader.IndexOf(' ');
			if (nSpace > -1)
				sHeader = sHeader.GetSubstring(0, nSpace);

			// Set chunk data
			m_nChunkSize = ParseTools::ParseHexValue(sHeader);
			m_nChunkLeft = m_nChunkSize;
		}

		// Check maximum size that can be read
		int nRead = nSize;
		if (nRead > m_nChunkLeft)
			nRead = m_nChunkLeft;

		// Read data
		const int nResult = m_pConnection->Receive(pBuffer, nRead);
		if (nResult > -1)
			m_nChunkLeft -= nResult;

		// Read end of chunk
		if (m_nChunkLeft <= 0) {
			// Read CRLF
			m_pConnection->ReadLine();

			// Reset chunk data
			m_nChunkSize = -1;
			m_nChunkLeft =  0;
		}

		// Return number of bytes read
		return nResult;
	} else {
		// Not chunked. Read directly from HTTP connection
		return m_pConnection->Receive(pBuffer, nSize);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Client functions                    ]
//[-------------------------------------------------------]
Connection *HttpClient::CreateOutgoingConnection()
{
	// Create web server connection
	HttpClientConnection *pConnection = new HttpClientConnection(*this);

	// We want to read manually, please (content could be huge, so read step by step)
	pConnection->SetReceiveMode(Connection::ReceiveManual);

	// Return connection
	return pConnection;
}

void HttpClient::OnDisconnect(Connection &cConnection)
{
	// Reset connection
	m_pConnection = nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
HttpClient::HttpClient(const HttpClient &cClient)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Copy operator
*/
HttpClient &HttpClient::operator =(const HttpClient &cClient)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
