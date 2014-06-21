/*********************************************************\
 *  File: HttpClientConnection.cpp                       *
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
#include "PLCore/Network/Http/HttpClientConnection.h"


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
HttpClientConnection::HttpClientConnection(Host &cHost) : Connection(cHost)
{
}

/**
*  @brief
*    Destructor
*/
HttpClientConnection::~HttpClientConnection()
{
}

/**
*  @brief
*    Read HTTP header
*/
const HttpHeader &HttpClientConnection::ReadHttpHeader()
{
	// Reset header
	m_cHttpHeader.Clear();

	// Read until header is complete
	while (IsConnected() && !m_cHttpHeader.IsComplete()) {
		// Disconnect on read error
		if (!ReadLine().GetLength())
			Disconnect();
	}

	// Return header
	return m_cHttpHeader;
}


//[-------------------------------------------------------]
//[ Protected virtual Connection functions                ]
//[-------------------------------------------------------]
void HttpClientConnection::OnConnect()
{
	// Initialize buffer
	m_cBuffer.SetRemoveLineDelimiters(false);
	m_cBuffer.Allocate(4096);

	// Initialize HTTP message
	m_cHttpHeader.Clear();
}

void HttpClientConnection::OnDisconnect()
{
	// Clear
	m_cBuffer.Clear();
	m_cHttpHeader.Clear();
}

void HttpClientConnection::OnReceive(const char *pBuffer, uint32 nSize)
{
	// Data received?
	if (pBuffer && !m_cHttpHeader.IsComplete()) {
		// Add data to buffer
		m_cBuffer.Add(pBuffer, nSize);

		// Read lines from buffer
		String sLine = m_cBuffer.GetLine();
		while (sLine.GetLength()) {
			// Add line to HTTP message
			m_cHttpHeader.AddEntry(sLine);
			if (m_cHttpHeader.IsComplete()) {
				// Parse header information
				m_cHttpHeader.Parse();
			}

			// Next line
			sLine = m_cBuffer.GetLine();
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
