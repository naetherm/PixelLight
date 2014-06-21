/*********************************************************\
 *  File: Host.inl                                       *
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
*    Check if the host is currently listening for new connections
*/
inline bool Host::IsListening() const
{
	// Return listening status
	return m_bListening;
}

/**
*  @brief
*    Close host (close all connections and stop listening for new connections)
*/
inline void Host::Close()
{
	// Close port (stop listening)
	ClosePort();

	// Close all connections
	CloseConnections();
}

/**
*  @brief
*    Returns the currently used port number
*/
inline uint32 Host::GetPort() const
{
	// Return host port
	return m_nPort;
}

/**
*  @brief
*    Returns the maximum number of allowed connections
*/
inline uint32 Host::GetMaxConnections() const
{
	// Return maximum number of connections
	return m_nMaxConnections;
}

/**
*  @brief
*    Sets the maximum number of allowed connections
*/
inline void Host::SetMaxConnections(uint32 nMaxConnections)
{
	// Set maximum number of connections
	m_nMaxConnections = nMaxConnections;
}

/**
*  @brief
*    Get list of active connection
*/
inline const Container<Connection*> &Host::GetConnections() const
{
	// Return connection list
	return m_lstConnections;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a connection
*/
inline void Host::AddConnection(Connection *pConnection)
{
	// Check if connection pointer is valid
	if (pConnection) {
		// Add connection
		m_lstConnections.Add(pConnection);
	}
}

/**
*  @brief
*    Removes a connection
*/
inline void Host::RemoveConnection(Connection *pConnection)
{
	// Check if connection pointer is valid
	if (pConnection) {
		// Remove connection
		m_lstConnections.Remove(pConnection);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
