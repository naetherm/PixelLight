/*********************************************************\
 *  File: Http.h                                         *
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


#ifndef __PLCORE_HTTP_H__
#define __PLCORE_HTTP_H__
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
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Client signature
*/
enum EClientSignature {
	ClientPixelLight = 0,	/**< Identify as PixelLight HTTP client */
	ClientMozilla			/**< Identify as Mozilla client */
};

/**
*  @brief
*    HTTP protocol version
*/
enum EHttpProtocol {
	Http10 = 0,				/**< HTTP 1.0 protocol version */
	Http11					/**< HTTP 1.1 protocol version */
};

/**
*  @brief
*    HTTP message type
*/
enum EHttpMessageType {
	HttpRequest = 0,		/**< HTTP request */
	HttpResponse			/**< HTTP response */
};

/**
*  @brief
*    HTTP request
*/
enum EHttpRequest{
	HttpGet = 0,			/**< GET-request */
	HttpPost,				/**< POST-request */
	HttpHead,				/**< HEAD-request */
	HttpPut,				/**< PUT-request */
	HttpDelete,				/**< DELETE-request */
	HttpTrace,				/**< TRACE-request */
	HttpOptions,			/**< OPTIONS-request */
	HttpConnect				/**< CONNECT-request */
};

/**
*  @brief
*    HTTP connection type
*/
enum EHttpConnection {
	ConnectionClose = 0,	/**< Close connection after transaction */
	ConnectionKeepAlive		/**< Keep connection open */
};

/**
*  @brief
*    HTTP authentication
*/
enum EHttpAuth {
	NoAuth = 0,		/**< No authentication */
	BasicAuth		/**< Basic authentication */
};

/**
*  @brief
*    HTTP status codes
*/
enum EHttpStatus {
	Http_000_Unknown						=   0,	/**< Unknown status code (invalid) */
	Http_100_Continue						= 100,	/**< 100 Continue */
	Http_101_SwitchingProtocols				= 101,	/**< 101 Switching Protocols */
	Http_200_OK								= 200,	/**< 200 OK */
	Http_201_Created						= 201,	/**< 201 Created */
	Http_202_Accepted						= 202,	/**< 202 Accepted */
	Http_203_NonAuthoritativeInformation	= 203,	/**< 203 Non-Authoritative Information */
	Http_204_NoContent						= 204,	/**< 204 No Content */
	Http_205_ResetContent					= 205,	/**< 205 Reset Content */
	Http_206_PartialContent					= 206,	/**< 206 Partial Content */
	Http_300_MultipleChoices				= 300,	/**< 300 Multiple Choices */
	Http_301_MovedPermanently				= 301,	/**< 301 Moved Permanently */
	Http_302_Found							= 302,	/**< 302 Found */
	Http_303_SeeOther						= 303,	/**< 303 See Other */
	Http_304_NotModified					= 304,	/**< 304 Not Modified */
	Http_305_UseProxy						= 305,	/**< 305 Use Proxy */
	Http_307_TemporaryRedirect				= 307,	/**< 307 Temporary Redirect */
	Http_400_BadRequest						= 400,	/**< 400 Bad Request */
	Http_401_Unauthorized					= 401,	/**< 401 Unauthorized */
	Http_403_Forbidden						= 403,	/**< 403 Forbidden */
	Http_404_NotFound						= 404,	/**< 404 Not Found */
	Http_405_MethodNotAllowed				= 405,	/**< 405 Method Not Allowed */
	Http_406_NotAcceptable					= 406,	/**< 406 Not Acceptable */
	Http_407_ProxyAuthenticationRequired	= 407,	/**< 407 Proxy Authentication Required */
	Http_408_RequestTimeout					= 408,	/**< 408 Request Timeout */
	Http_409_Conflict						= 409,	/**< 409 Conflict */
	Http_410_Gone							= 410,	/**< 410 Gone */
	Http_411_LengthRequired					= 411,	/**< 411 Length Required */
	Http_412_PreconditionFailed				= 412,	/**< 412 Precondition Failed */
	Http_413_RequestEntityTooLarge			= 413,	/**< 413 Request Entity Too Large */
	Http_414_RequestURITooLong				= 414,	/**< 414 Request-URI Too Long */
	Http_415_UnsupportedMediaType			= 415,	/**< 415 Unsupported Media Type */
	Http_416_RequestedRangeNotSatisfiable	= 416,	/**< 416 Requested Range Not Satisfiable */
	Http_417_ExpectationFailed				= 417,	/**< 417 Expectation Failed */
	Http_500_InternalServerError			= 500,	/**< 500 Internal Server Error */
	Http_501_NotImplemented					= 501,	/**< 501 Not Implemented */
	Http_502_BadGateway						= 502,	/**< 502 Bad Gateway */
	Http_503_ServiceUnavailable				= 503,	/**< 503 Service Unavailable */
	Http_504_GatewayTimeout					= 504,	/**< 504 Gateway Timeout */
	Http_505_HTTPVersionNotSupported		= 505	/**< 505 HTTP Version Not Supported */
};


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    HTTP helper class
*/
class Http {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get status string from status code
		*
		*  @param[in] nStatusCode
		*    HTTP status code
		*
		*  @return
		*    Status string
		*
		*  @remarks
		*    Example: GetStatusString(Http_404_NotFound) -> "404 Not Found"
		*/
		static PLCORE_API String GetStatusString(EHttpStatus nStatusCode);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_HTTP_H__
