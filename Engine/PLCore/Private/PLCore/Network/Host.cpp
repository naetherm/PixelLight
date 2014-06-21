/*********************************************************\
 *  File: Host.cpp                                       *
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
#include "PLCore/Network/Connection.h"
#include "PLCore/Network/Host.h"


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


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
Host::Host() :
	m_cServerThread(*this),
	m_bListening(false),
	m_nPort(0),
	m_nMaxConnections(0)
{
}

/**
*  @brief
*    Destructor
*/
Host::~Host()
{
	// Close host
	Close();
}

/**
*  @brief
*    Connect to a server
*/
Connection *Host::Connect(const String &sServer, uint32 nPort)
{
	// Create new connection
	Connection *pConnection = CreateOutgoingConnection();
	if (pConnection) {
		// Establish connection
		if (pConnection->Connect(sServer, nPort)) {
			// Connection created successfully
			AddConnection(pConnection);

			// Return connection
			return pConnection;
		} else {
			// Error: Can't connect
			delete pConnection;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Start listening for new connections
*/
void Host::Listen(uint32 nPort)
{
	// Is port already open?
	if (!m_bListening && nPort > 0) {
		// Save port
		m_nPort = nPort;

		// Create server socket
		m_cSocket.Bind(m_nPort);
		m_cSocket.Listen();
		m_bListening = true;

		// Start listener thread
		m_cServerThread.Start();
	}
}

/**
*  @brief
*    Stop listening for new connections
*/
void Host::ClosePort()
{
	// Is port open?
	if (m_bListening) {
		// [TODO] Do not use "Thread::Terminate()" because it's evil and there are also platforms like Android don't supporting it at all
		// Stop listener thread
		m_cServerThread.Terminate();

		// Close server socket
		m_cSocket.Close();

		// Set port closed
		m_bListening = false;
	}
}

/**
*  @brief
*    Close all connections
*/
void Host::CloseConnections()
{
	// Find connections
	List<Connection*> lstToRemove;
	Iterator<Connection*> cIterator = m_lstConnections.GetIterator();
	while (cIterator.HasNext()) {
		Connection *pConnection = cIterator.Next();
		lstToRemove.Add(pConnection);
	}

	// Remove connections
	cIterator = lstToRemove.GetIterator();
	while (cIterator.HasNext()) {
		Connection *pConnection = cIterator.Next();
		delete pConnection;
	}
	m_lstConnections.Clear();
}

/**
*  @brief
*    Remove inactive connections
*/
void Host::RemoveInactiveConnections()
{
	// Find inactive connections
	List<Connection*> lstToRemove;
	Iterator<Connection*> cIterator = m_lstConnections.GetIterator();
	while (cIterator.HasNext()) {
		Connection *pConnection = cIterator.Next();
		if (!pConnection->IsConnected())
			lstToRemove.Add(pConnection);
	}

	// Remove inactive connections
	cIterator = lstToRemove.GetIterator();
	while (cIterator.HasNext()) {
		Connection *pConnection = cIterator.Next();
		m_lstConnections.Remove(pConnection);
		delete pConnection;
	}
}


//[-------------------------------------------------------]
//[ Protected virtual Host functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Create a new incoming connection
*/
Connection *Host::CreateIncomingConnection()
{
	// To be implemented by derived classes
	return new Connection(*this);
}

/**
*  @brief
*    Create a new outgoing connection
*/
Connection *Host::CreateOutgoingConnection()
{
	// To be implemented by derived classes
	return new Connection(*this);
}

/**
*  @brief
*    Called when a connection is established
*/
void Host::OnConnect(Connection &cConnection)
{
	// To be implemented by derived classes
}

/**
*  @brief
*    Called when a connection is closed
*/
void Host::OnDisconnect(Connection &cConnection)
{
	// To be implemented by derived classes
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Host::Host(const Host &cSource) :
	m_cServerThread(*this),
	m_bListening(false),
	m_nPort(0),
	m_nMaxConnections(0)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Host &Host::operator =(const Host &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Check for new connections
*/
void Host::CheckNewConnections()
{
	// Check for a new connection
	Socket cSocket = m_cSocket.Accept();
	if (cSocket.IsValid()) {
		// Check maximum number of connections
		if (m_lstConnections.GetNumOfElements() < m_nMaxConnections || m_nMaxConnections == 0) {
			// Create new connection
			Connection *pConnection = CreateIncomingConnection();
			if (pConnection) {
				// Establish connection
				if (pConnection->Connect(cSocket)) {
					// Connection created successfully
					AddConnection(pConnection);
				} else {
					// Error: Can't connect
					delete pConnection;
					cSocket.Close();
				}
			} else {
				// Error: Can't create connection
				cSocket.Close();
			}
		} else {
			// Error: too many connections
			cSocket.Close();
		}
	}
}


//[-------------------------------------------------------]
//[ Server thread class                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Host::ServerThread::ServerThread(Host &cHost) :
	m_pHost(&cHost)
{
}

/**
*  @brief
*    Destructor
*/
Host::ServerThread::~ServerThread()
{
}


//[-------------------------------------------------------]
//[ Private virtual Thread functions                      ]
//[-------------------------------------------------------]
int Host::ServerThread::Run()
{
	// Listen for new connections
	bool bTrue = true;
	while (bTrue) {
		// Check for a new connection
		m_pHost->CheckNewConnections();
	}

	// Done
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
