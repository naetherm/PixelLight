/*********************************************************\
 *  File: HttpClientConnection.h                         *
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


#ifndef __PLCORE_HTTPCLIENTCONNECTION_H__
#define __PLCORE_HTTPCLIENTCONNECTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Network/Connection.h"
#include "PLCore/Network/Buffer.h"
#include "PLCore/Network/Http/HttpHeader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Http client connection
*/
class HttpClientConnection : public Connection {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API HttpClientConnection(Host &cHost);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~HttpClientConnection();

		/**
		*  @brief
		*    Read HTTP header
		*/
		PLCORE_API const HttpHeader &ReadHttpHeader();


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
		Buffer		m_cBuffer;		/**< Receive buffer */
		HttpHeader	m_cHttpHeader;	/**< Received HTTP header */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_HTTPCLIENTCONNECTION_H__
