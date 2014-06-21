/*********************************************************\
 *  File: Socket.cpp                                     *
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
#ifdef WIN32
	#include <winsock.h>
#endif
#ifdef LINUX
	#include <netdb.h>
	#include <fcntl.h>	// For "fcntl()"
	#include <unistd.h>
	#include <arpa/inet.h>
#endif
#include "PLCore/Network/Socket.h"


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
#ifdef WIN32
	#define socklen_t int
#endif
#ifdef LINUX
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR   -1
#endif


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
Socket::Socket() :
	m_nSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
{
}

/**
*  @brief
*    Returns whether the socket is currently valid
*/
bool Socket::IsValid() const
{
	return (m_nSocket != INVALID_SOCKET);
}

/**
*  @brief
*    Closes the socket
*/
bool Socket::Close()
{
	// Valid socket?
	if (m_nSocket != INVALID_SOCKET) {
		// Close server socket
		#ifdef WIN32
			const int nResult = closesocket(m_nSocket);
		#else
			const int nResult = close(m_nSocket);
		#endif

		// Reset socket address
		m_cSocketAddress = SocketAddress();
		m_nSocket        = INVALID_SOCKET;

		// Done
		return !nResult;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Establishes a connection to a host by using a given socked address
*/
bool Socket::Connect(const SocketAddress &cSocketAddress)
{
	return (m_nSocket != INVALID_SOCKET && connect(m_nSocket, reinterpret_cast<sockaddr*>(cSocketAddress.m_pSockAddress), sizeof(*cSocketAddress.m_pSockAddress)) != SOCKET_ERROR);
}

/**
*  @brief
*    Associate local address with socket
*/
bool Socket::Bind(const SocketAddress &cSocketAddress)
{
	// Valid socket?
	if (m_nSocket != INVALID_SOCKET) {
		// Bind socket to a socket address
		m_cSocketAddress = cSocketAddress;
		return (bind(m_nSocket, reinterpret_cast<sockaddr*>(m_cSocketAddress.m_pSockAddress), sizeof(*m_cSocketAddress.m_pSockAddress)) != SOCKET_ERROR);
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Mark a socket as accepting connections
*/
bool Socket::Listen(int nMaxQueue) const
{
	// If valid socket, listen for new connections
	return (m_nSocket != INVALID_SOCKET && listen(m_nSocket, nMaxQueue > 0 ? nMaxQueue : SOMAXCONN) != SOCKET_ERROR);
}

/**
*  @brief
*    Accept a connection on a socket (blocking)
*/
Socket Socket::Accept() const
{
	Socket cSocket(INVALID_SOCKET);

	// Valid socket?
	if (m_nSocket != INVALID_SOCKET) {
		// Accept a new connection
		socklen_t nSize = sizeof(*cSocket.m_cSocketAddress.m_pSockAddress);
		cSocket.m_nSocket = accept(m_nSocket, reinterpret_cast<sockaddr*>(cSocket.m_cSocketAddress.m_pSockAddress), &nSize);
	}

	// Return new socket
	return cSocket;
}

/**
*  @brief
*    Sends data
*/
int Socket::Send(const char *pBuffer, uint32 nSize) const
{
	return ((pBuffer && m_nSocket != INVALID_SOCKET) ? send(m_nSocket, pBuffer, nSize, 0) : -1);
}

/**
*  @brief
*    Returns whether or not data is waiting to be received (non-blocking request)
*/
bool Socket::IsDataWaiting() const
{
	// Valid socket?
	if (m_nSocket != INVALID_SOCKET) {
		#ifdef WIN32
			// Bring the socket into a non-blocking mode
			u_long nNonBlocking = 1;
			ioctlsocket(m_nSocket, FIONBIO, &nNonBlocking);

			// Are any bytes waiting? (just look, don't touch)
			// -> "recv" returns the number of bytes received and copied into the buffer (so, the upper limit is our buffer size)
			char nBuffer = 0;
			const int nNumOfBytes = recv(m_nSocket, &nBuffer, 1, MSG_PEEK);

			// Bring the socket back into a blocking mode
			nNonBlocking = 0;
			ioctlsocket(m_nSocket, FIONBIO, &nNonBlocking);

			// Return the number of bytes waiting to be received
			return (nNumOfBytes > 0);
		#else
			// Get the currently set socket flags
			const int nFlags = fcntl(m_nSocket, F_GETFL, 0);

			// Are any bytes waiting? (just look, don't touch)
			// -> "recv" returns the number of bytes received and copied into the buffer (so, the upper limit is our buffer size)
			fcntl(m_nSocket, F_SETFL, nFlags | O_NONBLOCK);

			// Get the number of waiting bytes (just look, don't touch)
			char nBuffer = 0;
			const int nNumOfBytes = recv(m_nSocket, &nBuffer, 1, MSG_PEEK);

			// Bring the socket back into a blocking mode
			fcntl(m_nSocket, F_SETFL, nFlags);

			// Return the number of bytes waiting to be received
			return (nNumOfBytes > 0);
		#endif
	}

	// Error! No data is waiting to be received...
	return false;
}

/**
*  @brief
*    Receives data (blocking request)
*/
int Socket::Receive(char *pBuffer, uint32 nSize) const
{
	// "recv" returns the number of bytes received and copied into the buffer (so, the upper limit is our buffer size)
	return ((pBuffer && m_nSocket != INVALID_SOCKET) ? recv(m_nSocket, pBuffer, nSize, 0) : -1);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
