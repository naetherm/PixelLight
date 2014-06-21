/*********************************************************\
 *  File: HttpServerConnection.h                         *
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


#ifndef __PLCORE_HTTPSERVERCONNECTION_H__
#define __PLCORE_HTTPSERVERCONNECTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Network/Buffer.h"
#include "PLCore/Network/Connection.h"
#include "PLCore/Network/Http/HttpHeader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class HttpServer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Http server connection
*/
class HttpServerConnection : public Connection {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cServer
		*    HTTP server
		*/
		PLCORE_API HttpServerConnection(HttpServer &cServer);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~HttpServerConnection();

		/**
		*  @brief
		*    Send a file to the client
		*
		*  @param[in] nStatus
		*    HTTP status code
		*  @param[in] sFilename
		*    Filename
		*/
		PLCORE_API void SendFile(EHttpStatus nStatus, const String &sFilename);

		/**
		*  @brief
		*    Send data to the client
		*
		*  @param[in] nStatus
		*    HTTP status code
		*  @param[in] sMimeType
		*    MIME type
		*  @param[in] sContent
		*    Data to send
		*/
		PLCORE_API void SendData(EHttpStatus nStatus, const String &sMimeType, const String &sContent);

		/**
		*  @brief
		*    Send a redirect
		*
		*  @param[in] sLocation
		*    New location
		*/
		PLCORE_API void SendRedirect(const String &sLocation);

		/**
		*  @brief
		*    Send an error
		*
		*  @param[in] nStatus
		*    HTTP status code
		*/
		PLCORE_API void SendError(EHttpStatus nStatus);

		/**
		*  @brief
		*    Send HTTP header
		*
		*  @param[in] nStatus
		*    HTTP status code
		*  @param[in] sMimeType
		*    MIME type
		*  @param[in] nLength
		*    Content length (0 for not sending the content-length at all)
		*/
		PLCORE_API void SendHeader(EHttpStatus nStatus, const String &sMimeType, uint32 nLength);


	//[-------------------------------------------------------]
	//[ Protected virtual HttpServerConnection functions      ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    A HTTP request has been received
		*
		*  @param[in] cHttpHeader
		*    HTTP header
		*/
		PLCORE_API virtual void OnHttpRequest(const HttpHeader &cHttpHeader);


	//[-------------------------------------------------------]
	//[ Protected virtual Connection functions                ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual void OnConnect() override;
		PLCORE_API virtual void OnDisconnect() override;
		PLCORE_API virtual void OnReceive(const char *pBuffer, uint32 nSize) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		HttpServer	*m_pServer;		/**< HTTP server */
		Buffer		 m_cBuffer;		/**< Receive buffer */
		HttpHeader	 m_cHttpHeader;	/**< Received HTTP header */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_HTTPSERVERCONNECTION_H__
