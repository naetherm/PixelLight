/*********************************************************\
 *  File: Socket.inl                                     *
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
*    Copy constructor
*/
inline Socket::Socket(const Socket &cSocket) :
	m_cSocketAddress(cSocket.m_cSocketAddress),
	m_nSocket(cSocket.m_nSocket)
{
}

/**
*  @brief
*    Constructor
*/
inline Socket::Socket(handle nSocket) :
	m_nSocket(nSocket)
{
}

/**
*  @brief
*    Destructor
*/
inline Socket::~Socket()
{
	// Note that we are NOT allowed to close the current socket in here automatically
	// or we may end up in an ugly mess
}

/**
*  @brief
*    Returns the socket address object
*/
inline const SocketAddress &Socket::GetSocketAddress() const
{
	// Return socket address
	return m_cSocketAddress;
}

/**
*  @brief
*    Establishes a connection to a host by using an address and port
*/
inline bool Socket::Connect(const String &sAddress, uint32 nPort)
{
	return Connect(SocketAddress(sAddress, nPort));
}

/**
*  @brief
*    Associate local address with socket
*/
inline bool Socket::Bind(uint32 nPort)
{
	// Bind socket to port
	return Bind(SocketAddress(nPort));
}

/**
*  @brief
*    Associate local address with socket
*/
inline bool Socket::Bind(const String &sAddress, uint32 nPort)
{
	// Bind socket to address and port
	return Bind(SocketAddress(sAddress, nPort));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
