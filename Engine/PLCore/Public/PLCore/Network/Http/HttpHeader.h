/*********************************************************\
 *  File: HttpHeader.h                                   *
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


#ifndef __PLCORE_HTTPHEADER_H__
#define __PLCORE_HTTPHEADER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Array.h"
#include "PLCore/Network/Http/Http.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    HTTP header
*/
class HttpHeader {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API HttpHeader();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    HTTP header
		*/
		PLCORE_API HttpHeader(const HttpHeader &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~HttpHeader();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cSource
		*    HTTP header
		*
		*  @return
		*    Reference to this header
		*/
		PLCORE_API HttpHeader &operator =(const HttpHeader &cSource);

		/**
		*  @brief
		*    Clear header
		*/
		PLCORE_API void Clear();

		/**
		*  @brief
		*    Check if HTTP header is complete
		*
		*  @return
		*    'true' if header is complete, else 'false'
		*/
		PLCORE_API bool IsComplete() const;

		/**
		*  @brief
		*    Get header entries
		*
		*  @return
		*    List of header entries
		*/
		inline const Container<String> &GetEntries() const;

		/**
		*  @brief
		*    Add entry
		*
		*  @param[in] sEntry
		*    Header entry
		*/
		inline void AddEntry(const String &sEntry);

		/**
		*  @brief
		*    Parse header
		*
		*  @remarks
		*    This function will parse and interpret all header entries. Call this function
		*    after you have received the header, then you can access the parsed information
		*    via their get-methods (GetSize(), GetContentType(), etc.)
		*/
		PLCORE_API void Parse();

		/**
		*  @brief
		*    Print header entries to console
		*
		*  @param[in] bRaw
		*    If 'true', all header entries will be printed out, otherwise only those which are known by this implementation
		*/
		PLCORE_API void Print(bool bRaw = false) const;

		/**
		*  @brief
		*    Get protocol version
		*
		*  @return
		*    HTTP Protocol version
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline EHttpProtocol GetProtocol() const;

		/**
		*  @brief
		*    Get message type
		*
		*  @return
		*    HTTP message type (request or response)
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline EHttpMessageType GetMessageType() const;

		/**
		*  @brief
		*    Get request
		*
		*  @return
		*    HTTP request type
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline EHttpRequest GetRequest() const;

		/**
		*  @brief
		*    Get request as string
		*
		*  @return
		*    HTTP request type
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		PLCORE_API String GetRequestString() const;

		/**
		*  @brief
		*    Get request URL
		*
		*  @return
		*    HTTP request URL
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetRequestUrl() const;

		/**
		*  @brief
		*    Get connection type
		*
		*  @return
		*    HTTP connection type
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline EHttpConnection GetConnectionType() const;

		/**
		*  @brief
		*    Get status code
		*
		*  @return
		*    Status code
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline uint32 GetStatusCode() const;

		/**
		*  @brief
		*    Get status string
		*
		*  @return
		*    Status string
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetStatusString() const;

		/**
		*  @brief
		*    Get location
		*
		*  @return
		*    Location
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetLocation() const;

		/**
		*  @brief
		*    Get date
		*
		*  @return
		*    Date
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetDate() const;

		/**
		*  @brief
		*    Get server signature
		*
		*  @return
		*    Server signature
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetServer() const;

		/**
		*  @brief
		*    Get client signature
		*
		*  @return
		*    Client signature
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetUserAgent() const;

		/**
		*  @brief
		*    Get authentication type
		*
		*  @return
		*    Authentication type
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline EHttpAuth GetAuthenticationType() const;

		/**
		*  @brief
		*    Get authentication realm
		*
		*  @return
		*    Authentication realm
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetAuthenticationRealm() const;

		/**
		*  @brief
		*    Get authorization string
		*
		*  @return
		*    Authorization string
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetAuthorization() const;

		/**
		*  @brief
		*    Get transfer encoding
		*
		*  @return
		*    Transfer encoding
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetTransferEncoding() const;

		/**
		*  @brief
		*    Check if data is transfered "chunked"
		*
		*  @return
		*    'true' if transfer encoding is "chunked", else 'false'
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline bool IsChunked() const;

		/**
		*  @brief
		*    Get content length
		*
		*  @return
		*    Content length
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline uint32 GetContentLength() const;

		/**
		*  @brief
		*    Get content language
		*
		*  @return
		*    Content language
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetContentLanguage() const;

		/**
		*  @brief
		*    Get content type
		*
		*  @return
		*    Content type
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetContentType() const;

		/**
		*  @brief
		*    Get ETag
		*
		*  @return
		*    ETag for content
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline String GetETag() const;

		/**
		*  @brief
		*    Check if partial data is returned
		*
		*  @return
		*    'true' if partial data is returned, else 'false'
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline bool IsPartial() const;

		/**
		*  @brief
		*    Get start of range
		*
		*  @return
		*    Byte position
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline uint32 GetRangeMin() const;

		/**
		*  @brief
		*    Get end of range
		*
		*  @return
		*    Byte position
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline uint32 GetRangeMax() const;

		/**
		*  @brief
		*    Get total size
		*
		*  @return
		*    Size
		*
		*  @remarks
		*    To access this information, Parse() has to be called first
		*/
		inline uint32 GetRangeTotal() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:	
		Array<String>		m_lstEntries;			/**< Header entries */
		EHttpProtocol		m_nProtocol;			/**< HTTP protocol version */
		EHttpMessageType	m_nMessageType;			/**< Message type (request or response) */
		EHttpRequest		m_nRequest;				/**< Request type */
		String				m_sRequestUrl;			/**< Request URL */
		EHttpConnection		m_nConnection;			/**< Connection type (Close or KeepAlive) */
		EHttpStatus			m_nStatusCode;			/**< Status code */
		String				m_sStatusString;		/**< Status string */
		String				m_sLocation;			/**< Location */
		String				m_sDate;				/**< Date */
		String				m_sServer;				/**< Server signature */
		String				m_sUserAgent;			/**< Client signature */
		EHttpAuth			m_nAuthType;			/**< Authentication type */
		String				m_sAuthRealm;			/**< Authentication realm */
		String				m_sAuthorization;		/**< Authorization string */
		String				m_sTransferEncoding;	/**< Transfer encoding */
		uint32				m_nContentLength;		/**< Content size */
		String				m_sContentLanguage;		/**< Content language */
		String				m_sContentType;			/**< Content type (MIME type) */
		String				m_sETag;				/**< ETag (identifier for content) */
		bool				m_bPartial;				/**< Partial download? */
		uint32				m_nRangeMin;			/**< Start of range (for partial content) */
		uint32				m_nRangeMax;			/**< End of range   (for partial content) */
		uint32				m_nRangeTotal;			/**< Total size     (for partial content) */
	

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Network/Http/HttpHeader.inl"


#endif // __PLCORE_HTTPHEADER_H__
