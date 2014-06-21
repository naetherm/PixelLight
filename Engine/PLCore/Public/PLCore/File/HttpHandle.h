/*********************************************************\
 *  File: HttpHandle.h                                   *
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


#ifndef __PLCORE_FILE_HTTPHANDLE_H__
#define __PLCORE_FILE_HTTPHANDLE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/File/File.h"
#include "PLCore/Network/Socket.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This class represents a tiny HTTP (Hypertext Transfer Protocol) client to download files via HTTP
*
*  @remarks
*    You don't need to use it directly, as you can also use the HTTP protocol through the
*    File/Directory classes, e.g.: File("http://www.pixellight.org/welcome.html")
*
*  @note
*    - Port: 80/TCP
*/
class HttpHandle {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API HttpHandle();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API ~HttpHandle();

		/**
		*  @brief
		*    Check if the connection is currently open
		*
		*  @return
		*    'true' if the connection is currently open, else 'false'
		*/
		PLCORE_API bool IsOpen() const;

		/**
		*  @brief
		*    Open the connection
		*
		*  @param[in] sUrl
		*    URL
		*  @param[in] sUsername
		*    Username
		*  @param[in] sPassword
		*    Password
		*
		*  @return
		*    'true', if the file could be opened, 'false' on error
		*/
		PLCORE_API bool Open(const String &sUrl, const String &sUsername, const String &sPassword);

		/**
		*  @brief
		*    Close the connection
		*
		*  @return
		*    'true', if the file could be closed, 'false' on error
		*/
		PLCORE_API bool Close();

		/**
		*  @brief
		*    Get HTTP response string
		*
		*  @return
		*    HTTP response
		*/
		PLCORE_API String GetResponse() const;

		/**
		*  @brief
		*    Get HTTP authenticate string
		*
		*  @return
		*    HTTP authenticate-field
		*/
		PLCORE_API String GetAuthenticate() const;

		/**
		*  @brief
		*    Get HTTP server string
		*
		*  @return
		*    HTTP server-field
		*/
		PLCORE_API String GetServer() const;

		/**
		*  @brief
		*    Get HTTP date string
		*
		*  @return
		*    HTTP date-field
		*/
		PLCORE_API String GetDate() const;

		/**
		*  @brief
		*    Get HTTP ContentType string
		*
		*  @return
		*    HTTP ContentType-field
		*/
		PLCORE_API String GetContentType() const;

		/**
		*  @brief
		*    Get HTTP content-length
		*
		*  @return
		*    HTTP ContentLength-field
		*/
		PLCORE_API int GetContentLength() const;

		/**
		*  @brief
		*    Reads data from the connection
		*
		*  @param[out] pBuffer
		*    Buffer to store the data (MUST be valid and large enough!)
		*  @param[in]  nSize
		*    Item size in bytes
		*  @param[in]  nCount
		*    Number of items to read
		*
		*  @return
		*    Number of full read items, if != 'nCount' an error occurred
		*/
		PLCORE_API uint32 Read(void *pBuffer, uint32 nSize, uint32 nCount);

		/**
		*  @brief
		*    Sets the starting position
		*
		*  @param[in] nOffset
		*    File offset in bytes relative to the given location
		*  @param[in] nLocation
		*    Location
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Seek(int32 nOffset, File::ESeek nLocation);

		/**
		*  @brief
		*    Returns the file position
		*
		*  @return
		*    Current position within the file, or < 0 on error
		*/
		PLCORE_API int32 Tell() const;

		/**
		*  @brief
		*    Returns whether end of file has been reached
		*
		*  @return
		*    'true', if the end of the file has been reached, else 'false'
		*/
		PLCORE_API bool IsEof() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Connects to the web server
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		bool Connect();

		/**
		*  @brief
		*    Reads a single line from the HTTP header
		*
		*  @return
		*    Next line from the header, "" at the end
		*/
		String ReadLine();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Socket	m_cSocket;			/**< Socket */
		Url		m_cUrl;				/**< Current URL */
		String	m_sDomain;			/**< Domain part of the URL */
		String	m_sUsername;		/**< Username */
		String	m_sPassword;		/**< Password */
		String	m_sResponse;		/**< HTTP Response */
		String	m_sAuthenticate;	/**< HTTP WWW-Authenticate-field */
		String	m_sServer;			/**< HTTP Server-field */
		String	m_sDate;			/**< HTTP Date-field */
		String	m_sContentType;		/**< HTTP ContentType-field */
		int		m_nContentLength;	/**< HTTP ContentLength-field */
		int		m_nPosition;		/**< Current stream position */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FILE_HTTPHANDLE_H__
