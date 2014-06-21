/*********************************************************\
 *  File: HttpServer.h                                   *
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


#ifndef __PLCORE_HTTPSERVER_H__
#define __PLCORE_HTTPSERVER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Network/Server.h"
#include "PLCore/Network/Http/Http.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class HttpHeader;
class HttpServerConnection;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Http server
*/
class HttpServer : public Server {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class HttpServerConnection;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API HttpServer();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~HttpServer();


	//[-------------------------------------------------------]
	//[ Protected virtual HttpServer functions                ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    A web server request has come in
		*
		*  @param[in] pConnection
		*    Connection
		*  @param[in] pHeader
		*    HTTP header
		*/
		PLCORE_API virtual void OnRequest(HttpServerConnection *pConnection, const HttpHeader *pHeader);

		/**
		*  @brief
		*    A GET request has come in
		*
		*  @param[in] pConnection
		*    Connection
		*  @param[in] pHeader
		*    HTTP header
		*/
		PLCORE_API virtual void OnGet(HttpServerConnection *pConnection, const HttpHeader *pHeader);

		/**
		*  @brief
		*    An error occurred
		*
		*  @param[in] pConnection
		*    Connection
		*  @param[in] nStatus
		*    HTTP status code
		*/
		PLCORE_API virtual void OnError(HttpServerConnection *pConnection, EHttpStatus nStatus);


	//[-------------------------------------------------------]
	//[ Protected virtual Server functions                    ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual Connection *CreateIncomingConnection() override;
		PLCORE_API virtual void OnConnect(Connection &cConnection) override;
		PLCORE_API virtual void OnDisconnect(Connection &cConnection) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cServer
		*    HTTP server
		*/
		HttpServer(const HttpServer &cServer);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cHttpServer
		*    HTTP server
		*
		*  @return
		*    Reference to this server
		*/
		HttpServer &operator =(const HttpServer &cServer);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_HTTPSERVER_H__
