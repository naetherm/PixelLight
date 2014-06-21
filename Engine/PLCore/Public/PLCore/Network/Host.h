/*********************************************************\
 *  File: Host.h                                         *
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


#ifndef __PLCORE_HOST_H__
#define __PLCORE_HOST_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/List.h"
#include "PLCore/System/Thread.h"
#include "PLCore/Network/Socket.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Connection;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Network host base class
*
*  @remarks
*    Base class for a 'network partner'. That is any node in a network, which can open or accept connection
*    to one or more other nodes. In a classic client/server architecture, one host is the client
*    and another is the server to which all clients connect. In other network architectures, such
*    as peer-to-peer, the network consists of several nodes of the same type, that connect to one another.
*/
class Host {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	public:
		friend class Connection;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API Host();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Host();

		/**
		*  @brief
		*    Connect to a server
		*
		*  @param[in] sServerName
		*    Server name
		*  @param[in] nPort
		*    Port number
		*
		*  @return
		*    Connection, a null pointer on error
		*/
		PLCORE_API Connection *Connect(const String &sServerName, uint32 nPort);

		/**
		*  @brief
		*    Check if the host is currently listening for new connections
		*
		*  @return
		*    'true' if host is listening for new connections, else 'false'
		*/
		inline bool IsListening() const;

		/**
		*  @brief
		*    Start listening for new connections
		*
		*  @param[in] nPort
		*    Port at which the host is listening
		*
		*  @remarks
		*    This function will start a listener thread and then return, so this function
		*    does not block the caller thread. When a connection is opened, it will call
		*    the virtual function CreateConnection() and then OnConnection() upon a successful
		*    connect. You can use SetMaxConnection() to set the maximum number of connections.
		*/
		PLCORE_API void Listen(uint32 nPort);

		/**
		*  @brief
		*    Stop listening for new connections
		*
		*  @remarks
		*    This function will stop the host listening for new connection, but all current connections
		*    will still remain open. See CloseConnection() and Close() for closing the host completely.
		*/
		PLCORE_API void ClosePort();

		/**
		*  @brief
		*    Close all connections
		*
		*  @remarks
		*    This function closes all active connections, but does not stop listening for new
		*    connections. See StopListen() and Close() for closing the host completely.
		*/
		PLCORE_API void CloseConnections();

		/**
		*  @brief
		*    Close host (close all connections and stop listening for new connections)
		*/
		inline void Close();

		/**
		*  @brief
		*    Returns the currently used port number
		*
		*  @return
		*    Number of the currently used port
		*/
		inline uint32 GetPort() const;

		/**
		*  @brief
		*    Returns the maximum number of allowed connections
		*
		*  @return
		*    Maximum number of allowed connections
		*/
		inline uint32 GetMaxConnections() const;

		/**
		*  @brief
		*    Sets the maximum number of allowed connections
		*
		*  @param[in] nMaxConnections
		*    Maximum number of allowed connections
		*/
		inline void SetMaxConnections(uint32 nMaxConnections);

		/**
		*  @brief
		*    Get list of active connection
		*
		*  @return
		*    List of connections
		*/
		inline const Container<Connection*> &GetConnections() const;

		/**
		*  @brief
		*    Remove inactive connections
		*/
		PLCORE_API void RemoveInactiveConnections();


	//[-------------------------------------------------------]
	//[ Protected virtual Host functions                      ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Create a new incoming connection
		*
		*  @return
		*    Connection
		*/
		PLCORE_API virtual Connection *CreateIncomingConnection();

		/**
		*  @brief
		*    Create a new outgoing connection
		*
		*  @return
		*    Connection
		*/
		PLCORE_API virtual Connection *CreateOutgoingConnection();

		/**
		*  @brief
		*    Called when a connection is established
		*
		*  @param[in] cConnection
		*    The new connection
		*/
		PLCORE_API virtual void OnConnect(Connection &cConnection);

		/**
		*  @brief
		*    Called when a connection is closed
		*
		*  @param[in] cConnection
		*    The closed connection
		*/
		PLCORE_API virtual void OnDisconnect(Connection &cConnection);


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
		Host(const Host &cSource);

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
		Host &operator =(const Host &cSource);

		/**
		*  @brief
		*    Check for new connections
		*/
		void CheckNewConnections();

		/**
		*  @brief
		*    Add connection
		*
		*  @param[in] pConnection
		*    Connection
		*/
		inline void AddConnection(Connection *pConnection);

		/**
		*  @brief
		*    Removes connection
		*
		*  @param[in] cConnection
		*    Connection
		*/
		inline void RemoveConnection(Connection *pConnection);


	//[-------------------------------------------------------]
	//[ Private data types                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Server thread class
		*/
		class ServerThread : public Thread {


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
			friend class Host;


			//[-------------------------------------------------------]
			//[ Private functions                                     ]
			//[-------------------------------------------------------]
			private:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] cHost
				*    Host
				*/
				ServerThread(Host &cHost);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~ServerThread();


			//[-------------------------------------------------------]
			//[ Private virtual Thread functions                      ]
			//[-------------------------------------------------------]
			private:
				virtual int Run();


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				Host *m_pHost;	/**< Host that owns this thread */


		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ServerThread		m_cServerThread;	/**< Thread that looks for new connections */
		bool				m_bListening;		/**< Host listening for connections? */
		uint32				m_nPort;			/**< Number of the used port */
		uint32				m_nMaxConnections;	/**< Maximum number of allowed connections */
		List<Connection*>	m_lstConnections;	/**< List of connections */
		Socket				m_cSocket;			/**< Server socket */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Network/Host.inl"


#endif // __PLCORE_HOST_H__
