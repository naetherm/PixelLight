/*********************************************************\
 *  File: SocketAddress.cpp                              *
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
	#undef SetPort // We need to undefine this else a linker error occurs... if not in this project then in another one
#endif
#ifdef LINUX
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>
#endif
#include "PLCore/Core/MemoryManager.h"
#include "PLCore/Network/SocketAddress.h"


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
SocketAddress::SocketAddress() :
	m_pSockAddress(new sockaddr_in)
{
	// Set up socket address
	m_pSockAddress->sin_family		= AF_INET;
	m_pSockAddress->sin_port		= 0;
	m_pSockAddress->sin_addr.s_addr	= 0;
}

/**
*  @brief
*    Constructor
*/
SocketAddress::SocketAddress(uint32 nPort) :
	m_pSockAddress(new sockaddr_in)
{
	// Set up socket address
	m_pSockAddress->sin_family		= AF_INET;
	m_pSockAddress->sin_port		= htons(static_cast<u_short>(nPort));
	m_pSockAddress->sin_addr.s_addr = INADDR_ANY;
}

/**
*  @brief
*    Copy constructor
*/
SocketAddress::SocketAddress(const SocketAddress &cSource) :
	m_pSockAddress(new sockaddr_in)
{
	*m_pSockAddress = *cSource.m_pSockAddress;
}

/**
*  @brief
*    Constructor
*/
SocketAddress::SocketAddress(const String &sHost, uint32 nPort) :
	m_pSockAddress(new sockaddr_in)
{
	// Set up socket address
	m_pSockAddress->sin_family      = AF_INET;
	m_pSockAddress->sin_port        = htons(static_cast<u_short>(nPort));
	m_pSockAddress->sin_addr.s_addr = INADDR_ANY;

	// Get host address
	if (sHost.GetLength()) {
		m_pSockAddress->sin_addr.s_addr = inet_addr(sHost);
		if (m_pSockAddress->sin_addr.s_addr == INADDR_NONE) {
			// [TODO] Obsolete function 'gethostbyname', use the 'getnameinfo' function
			// Try to resolve host
			hostent *pHost = gethostbyname(sHost);
			if (pHost)
				MemoryManager::Copy(reinterpret_cast<char*>(&m_pSockAddress->sin_addr.s_addr), pHost->h_addr, 4);
		}
	}
}

/**
*  @brief
*    Destructor
*/
SocketAddress::~SocketAddress()
{
	delete m_pSockAddress;
}

/**
*  @brief
*    Returns the host address
*/
String SocketAddress::GetHost() const
{
	// Return host address
	return inet_ntoa(m_pSockAddress->sin_addr);
}

/**
*  @brief
*    Sets the host address
*/
void SocketAddress::SetHost(const String &sHost)
{
	// If host address valid set host address, else set to any host address
	m_pSockAddress->sin_addr.s_addr = sHost.GetLength() ? inet_addr(sHost) : INADDR_ANY;
}

/**
*  @brief
*    Sets the host address by using the name of the host
*/
bool SocketAddress::SetHostByName(const String &sHostName)
{
	// [TODO] Obsolete function 'gethostbyname', use the 'getnameinfo' function
	// Get host address
	hostent *pHost = gethostbyname(sHostName);
	if (pHost) {
		// Host found
		m_pSockAddress->sin_addr.s_addr = *reinterpret_cast<unsigned long*>(pHost->h_addr);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Returns the host port number
*/
uint32 SocketAddress::GetPort() const
{
	// Get port number
	return ntohs(m_pSockAddress->sin_port);
}

/**
*  @brief
*    Sets the host port number
*/
bool SocketAddress::SetPort(uint32 nPort)
{
	// Set port number
	m_pSockAddress->sin_port = htons(static_cast<u_short>(nPort));

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
SocketAddress &SocketAddress::operator =(const SocketAddress &cSource)
{
	*m_pSockAddress = *cSource.m_pSockAddress;

	// Return this
	return *this;
}

/**
*  @brief
*    Compare operator
*/
bool SocketAddress::operator ==(const SocketAddress &cSource) const
{
	return (m_pSockAddress->sin_addr.s_addr == cSource.m_pSockAddress->sin_addr.s_addr &&
			m_pSockAddress->sin_port		== cSource.m_pSockAddress->sin_port);
}

/**
*  @brief
*    Compare operator
*/
bool SocketAddress::operator !=(const SocketAddress &cSource) const
{
	return (m_pSockAddress->sin_addr.s_addr != cSource.m_pSockAddress->sin_addr.s_addr ||
			m_pSockAddress->sin_port		!= cSource.m_pSockAddress->sin_port);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
