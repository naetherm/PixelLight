/*********************************************************\
 *  File: Connection.h                                   *
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


#ifndef __PLCORE_CONNECTION_H__
#define __PLCORE_CONNECTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/System/Thread.h"
#include "PLCore/Network/Socket.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Host;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Connection base class
*
*  @remarks
*    This is the most important base class of the PLCore network classes for the 'concrete'
*    communication. This class 'only' offers low-level methods for the communication on a pure
*    abstract data layer, meaning sending of strings or data-arrays without known usage/semantic.
*    Derived connection classes have to implement the concrete content layer.
*    For example a chat-connection offers methods like SendMessage(), SendPrivateMessage(),
*    SlapUser(), defining in this way what the user can do within the network. Within this
*    methods a data-package is created and send by using Connection::Send(). In another usage
*    a game-connection may have something like SynchronizeObject(Object*), GetUpdate(),
*    SendAction(Action*).
*    In short, the task of the derived connection class is to define the concrete protocol
*    and to hide the rest so the application do not have to care about it. The application
*    should 'only' use the methods of the concrete for instance game-connection class - NEVER
*    creating somewhere data-packages and sending them by itself using Connection::Send()!
*/
class Connection : protected Thread {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Host;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Receive mode
		*/
		enum EReceiveMode {
			ReceiveAutomatic = 0,	/**< Data is received automatically (see OnReceive()) */
			ReceiveManual			/**< Data is received manually (see Receive()) */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cHost
		*    Owner host
		*/
		PLCORE_API Connection(Host &cHost);

		/**
		*  @brief
		*    Get host that owns this connection
		*
		*  @return
		*    Host
		*/
		inline const Host &GetHost() const;

		/**
		*  @brief
		*    Get host that owns this connection
		*
		*  @return
		*    Host
		*/
		inline Host &GetHost();

		/**
		*  @brief
		*    Get name of connected host
		*
		*  @return
		*    Name of host
		*/
		inline String GetHostname() const;

		/**
		*  @brief
		*    Get port
		*
		*  @return
		*    Port number
		*/
		inline uint32 GetPort() const;

		/**
		*  @brief
		*    Get socket
		*
		*  @return
		*    The socket object
		*/
		inline const Socket &GetSocket() const;

		/**
		*  @brief
		*    Get socket
		*
		*  @return
		*    The socket object
		*/
		inline Socket &GetSocket();

		/**
		*  @brief
		*    Check if connection is active
		*
		*  @return
		*    'true' if the connection is currently established, else 'false'
		*/
		inline bool IsConnected() const;

		/**
		*  @brief
		*    Disconnect
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no connection established?)
		*/
		PLCORE_API bool Disconnect();

		/**
		*  @brief
		*    Get receive mode
		*
		*  @return
		*    Receive mode
		*/
		inline EReceiveMode GetReceiveMode() const;

		/**
		*  @brief
		*    Set receive mode
		*
		*  @param[in] nReceiveMode
		*    Receive mode
		*
		*  @remarks
		*    If mode is ReceiveAutomatic, data is read from the connection automatically.
		*    If mode is ReceiveManual, no data will be read until the method ReceiveData() has been
		*    called. In both cases, OnReceive() will be called whenever new data has arrived. 
		*    By default, mode is set to ReceiveAutomatic.
		*/
		inline void SetReceiveMode(EReceiveMode nReceiveMode);

		/**
		*  @brief
		*    Send data
		*
		*  @param[in] pBuffer
		*    Data to send, if a null pointer nothing happens
		*  @param[in] nSize
		*    Size in bytes of the buffer to send, MUST be valid!
		*
		*  @return
		*    Total number of bytes sent. Can be less than the requested size
		*    to be sent, negative value on error
		*/
		inline int Send(const char *pBuffer, uint32 nSize) const;

		/**
		*  @brief
		*    Send string
		*
		*  @param[in] sString
		*    String
		*
		*  @return
		*    Total number of bytes sent, negative value on error
		*/
		inline int Send(const String &sString) const;

		/**
		*  @brief
		*    Receives data (blocking request)
		*
		*  @param[out] pBuffer
		*    Buffer that receives the data, if a null pointer, nothing can be received
		*  @param[in]  nSize
		*    Size in bytes of the buffer that receives the data, MUST be valid!
		*
		*  @return
		*    Total number of received bytes, negative value on error
		*
		*  @note
		*    - If there is currently not enough data available, this function will read as much
		*      as possible, meaning that less data can be read than requested
		*    - If more data is waiting to be received as the given buffer is able to store,
		*      you have to call this method multiple times in order to gather all waiting data
		*/
		PLCORE_API int Receive(char *pBuffer, uint32 nSize);

		/**
		*  @brief
		*    Read line of text (blocking request)
		*
		*  @return
		*    Line read
		*/
		PLCORE_API String ReadLine();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Connection();

		/**
		*  @brief
		*    Establish a connection to a host by specifying address and port
		*
		*  @param[in] sHost
		*    Host name
		*  @param[in] nPort
		*    Port number
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Connect(const String &sHost, uint32 nPort);

		/**
		*  @brief
		*    Establish a connection to a host by using a given socket
		*
		*  @param[in] cSocket
		*    Socket of the host to connect to
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Connect(Socket &cSocket);


	//[-------------------------------------------------------]
	//[ Protected virtual Connection functions                ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the connection is established
		*/
		PLCORE_API virtual void OnConnect();

		/**
		*  @brief
		*    Called when the connection is disconnected
		*/
		PLCORE_API virtual void OnDisconnect();

		/**
		*  @brief
		*    Called when the connection receives data
		*
		*  @param[in] pBuffer
		*    Buffer with the received data, a null pointer on terrible error
		*  @param[in] nSize
		*    Number of received bytes within the given buffer (always valid!)
		*
		*  @note
		*    - Do NOT keep a pointer/reference to 'nBuffer' because this buffer is only valid
		*      inside this function
		*/
		PLCORE_API virtual void OnReceive(const char *pBuffer, uint32 nSize);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Connection(const Connection &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Connection &operator =(const Connection &cSource);

		/**
		*  @brief
		*    Receive data
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no connection established?)
		*/
		bool ReceiveData();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Host			*m_pHost;			/**< Pointer to host, NEVER a null pointer! */
		volatile bool	 m_bConnected;		/**< Connection status */
		String			 m_sHostname;		/**< Name of host */
		uint32			 m_nPort;			/**< Port number */
		Socket			 m_cSocket;			/**< Connection socket */
		EReceiveMode	 m_nReceiveMode;	/**< Receive mode */


	//[-------------------------------------------------------]
	//[ Private virtual ThreadFunction functions              ]
	//[-------------------------------------------------------]
	private:
		PLCORE_API virtual int Run() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Network/Connection.inl"


#endif // __PLCORE_CONNECTION_H__
