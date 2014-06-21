/*********************************************************\
 *  File: HttpServer.cpp                                 *
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
#include "PLCore/Network/Http/MimeTypeManager.h"
#include "PLCore/Network/Http/HttpServerConnection.h"
#include "PLCore/Network/Http/HttpServer.h"


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
HttpServer::HttpServer()
{
}

/**
*  @brief
*    Destructor
*/
HttpServer::~HttpServer()
{
}


//[-------------------------------------------------------]
//[ Protected virtual HttpServer functions                ]
//[-------------------------------------------------------]
/**
*  @brief
*    A web server request has come in
*/
void HttpServer::OnRequest(HttpServerConnection *pConnection, const HttpHeader *pHeader)
{
	// Get request type
	switch (pHeader->GetRequest()) {
		// GET
		case HttpGet:
			OnGet(pConnection, pHeader);
			break;

		// Not implemented functions
		case HttpPost:
		case HttpHead:
		case HttpPut:
		case HttpDelete:
		case HttpTrace:
		case HttpOptions:
		case HttpConnect:
			pConnection->SendError(Http_501_NotImplemented);
			break;
	}
}

/**
*  @brief
*    A GET request has come in
*/
void HttpServer::OnGet(HttpServerConnection *pConnection, const HttpHeader *pHeader)
{
	// Get what?
	const String sUrl = pHeader->GetRequestUrl();

	// Send page
	if (sUrl == '/') {
		// Create a default page
		const String sPage = "<html><head><title>PixelLight Web server</title></head><body><h1>Welcome</h1>Welcome to PixelLight web server</body></html>";

		// Send default page
		pConnection->SendData(Http_200_OK, "text/html", sPage);
	} else {
		// Send file not found
		pConnection->SendError(Http_404_NotFound);
	}
}

/**
*  @brief
*    An error occurred
*/
void HttpServer::OnError(HttpServerConnection *pConnection, EHttpStatus nStatus)
{
	// Get error code
	const String sError = Http::GetStatusString(nStatus);

	// Create a dynamic page containing the error code
	const String sPage = "<html><head><title>" + sError + "</title></head><body><h1>" + sError + "</h1></body></html>";

	// Send error page
	pConnection->SendData(nStatus, "text/html", sPage);
}


//[-------------------------------------------------------]
//[ Protected virtual Server functions                    ]
//[-------------------------------------------------------]
Connection *HttpServer::CreateIncomingConnection()
{
	// Create web server connection
	return new HttpServerConnection(*this);
}

void HttpServer::OnConnect(Connection &cConnection)
{
}

void HttpServer::OnDisconnect(Connection &cConnection)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
HttpServer::HttpServer(const HttpServer &cMessage)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Copy operator
*/
HttpServer &HttpServer::operator =(const HttpServer &cServer)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
