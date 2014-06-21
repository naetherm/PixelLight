/*********************************************************\
 *  File: Connection.inl                                 *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get host that owns this connection
*/
inline const Host &Connection::GetHost() const
{
	// Return host
	return *m_pHost;
}

/**
*  @brief
*    Get host that owns this connection
*/
inline Host &Connection::GetHost()
{
	// Return host
	return *m_pHost;
}

/**
*  @brief
*    Get name of connected host
*/
inline String Connection::GetHostname() const
{
	// Return host name
	return m_sHostname;
}

/**
*  @brief
*    Get port
*/
inline uint32 Connection::GetPort() const
{
	// Return port number
	return m_nPort;
}

/**
*  @brief
*    Get socket
*/
inline const Socket &Connection::GetSocket() const
{
	// Return socket
	return m_cSocket;
}

/**
*  @brief
*    Get socket
*/
inline Socket &Connection::GetSocket()
{
	// Return socket
	return m_cSocket;
}

/**
*  @brief
*    Check if connection is active
*/
inline bool Connection::IsConnected() const
{
	// Return state
	return m_bConnected;
}

/**
*  @brief
*    Get receive mode
*/
inline Connection::EReceiveMode Connection::GetReceiveMode() const
{
	// Return receive mode
	return m_nReceiveMode;
}

/**
*  @brief
*    Set receive mode
*/
inline void Connection::SetReceiveMode(EReceiveMode nReceiveMode)
{
	// Set receive mode
	m_nReceiveMode = nReceiveMode;
}

/**
*  @brief
*    Send data
*/
inline int Connection::Send(const char *pBuffer, uint32 nSize) const
{
	// Send data buffer
	return (pBuffer && nSize > 0) ? m_cSocket.Send(pBuffer, nSize) : -1;
}

/**
*  @brief
*    Send string
*/
inline int Connection::Send(const String &sString) const
{
	// Check if string is not empty
	if (sString.GetLength()) {
		// Send string
		// [TODO] What's with Unicode strings?? Check \0 at the end...
		return Send(sString.GetASCII(), sString.GetLength());
	}

	// Error, invalid string
	return -1;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
