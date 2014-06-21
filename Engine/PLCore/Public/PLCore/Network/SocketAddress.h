/*********************************************************\
 *  File: SocketAddress.h                                *
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


#ifndef __PLCORE_NETWORK_SOCKETADDRESS_H__
#define __PLCORE_NETWORK_SOCKETADDRESS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Socket address class encapsulating an IP/internet address
*/
class SocketAddress {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Socket;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API SocketAddress();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nPort
		*    Number of the port to use
		*/
		PLCORE_API SocketAddress(uint32 nPort);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Socket address to copy from
		*/
		PLCORE_API SocketAddress(const SocketAddress &cSource);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sAddress
		*    Host address
		*  @param[in] nPort
		*    Number of the port to use
		*/
		PLCORE_API SocketAddress(const String &sAddress, uint32 nPort);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~SocketAddress();

		/**
		*  @brief
		*    Returns the host address
		*
		*  @return
		*    The host address
		*/
		PLCORE_API String GetHost() const;

		/**
		*  @brief
		*    Sets the host address
		*
		*  @param[in] sAddress
		*    New host address (if empty, set to any host address)
		*/
		PLCORE_API void SetHost(const String &sAddress);

		/**
		*  @brief
		*    Sets the host address by using the name of the host
		*
		*  @param[in] sHostName
		*    Name of the host
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool SetHostByName(const String &sHostName);

		/**
		*  @brief
		*    Returns the host port number
		*
		*  @return
		*    The host port number
		*/
		PLCORE_API uint32 GetPort() const;

		/**
		*  @brief
		*    Sets the host port number
		*
		*  @param[in] nPort
		*    Number of the port to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool SetPort(uint32 nPort);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Socket address to copy from
		*
		*  @return
		*    Reference to this socket address
		*/
		PLCORE_API SocketAddress &operator =(const SocketAddress &cSource);

		/**
		*  @brief
		*    Compare operator
		*
		*  @param[in] cSource
		*    Socket address to compare with
		*
		*  @return
		*    'true' if both socket addresses are equal, else 'false'
		*/
		PLCORE_API bool operator ==(const SocketAddress &cSource) const;

		/**
		*  @brief
		*    Compare operator
		*
		*  @param[in] cSource
		*    Socket address to compare with
		*
		*  @return
		*    'true' if both socket addresses are not equal, else 'false'
		*/
		PLCORE_API bool operator !=(const SocketAddress &cSource) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		struct sockaddr_in *m_pSockAddress;	/**< Socket address (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_NETWORK_SOCKETADDRESS_H__
