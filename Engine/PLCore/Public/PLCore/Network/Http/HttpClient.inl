/*********************************************************\
 *  File: HttpClient.inl                                 *
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
//[-------------------------------------------------------]
//[ Options                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get client signature
*/
inline EClientSignature HttpClient::GetClientSignature() const
{
	// Return client signature
	return m_nClientSignature;
}

/**
*  @brief
*    Set client signature
*/
inline void HttpClient::SetClientSignature(EClientSignature nClientSignature)
{
	// Set client signature
	m_nClientSignature = nClientSignature;
}

/**
*  @brief
*    Get protocol version
*/
inline EHttpProtocol HttpClient::GetProtocol() const
{
	// Return protocol
	return m_nHttpProtocol;
}

/**
*  @brief
*    Set protocol version
*/
inline void HttpClient::SetProtocol(EHttpProtocol nProtocol)
{
	// Set protocol
	m_nHttpProtocol = nProtocol;
}

/**
*  @brief
*    Get connection type
*/
inline EHttpConnection HttpClient::GetConnectionType() const
{
	// Return connection type
	return m_nConnection;
}

/**
*  @brief
*    Set connection type
*/
inline void HttpClient::SetConnectionType(EHttpConnection nConnection)
{
	// Set connection type
	m_nConnection = nConnection;
}

/**
*  @brief
*    Get authentication
*/
inline EHttpAuth HttpClient::GetAuthentication() const
{
	// Return authentication
	return m_nHttpAuth;
}

/**
*  @brief
*    Set authentication
*/
inline void HttpClient::SetAuthentication(EHttpAuth nAuth, const String &sUsername, const String &sPassword)
{
	// Set authentication
	m_nHttpAuth = nAuth;

	// Set username and password
	m_sUsername = sUsername;
	m_sPassword = sPassword;
}

//[-------------------------------------------------------]
//[ Functions                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if a connection is open
*/
inline bool HttpClient::IsConnected() const
{
	// Return if a connection is active
	return (m_pConnection != nullptr);
}

/**
*  @brief
*    Get HTTP header sent from server
*/
inline const HttpHeader &HttpClient::GetHeader() const
{
	// Return header
	return m_cHttpHeader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
