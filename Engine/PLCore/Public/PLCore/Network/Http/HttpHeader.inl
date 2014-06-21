/*********************************************************\
 *  File: HttpHeader.inl                                 *
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
*    Destructor
*/
inline HttpHeader::~HttpHeader()
{
}

/**
*  @brief
*    Get header entries
*/
inline const Container<String> &HttpHeader::GetEntries() const
{
	// Return entries
	return m_lstEntries;
}

/**
*  @brief
*    Add header entry
*/
inline void HttpHeader::AddEntry(const String &sEntry)
{
	// Add header entry to list
	String sLine = sEntry;
	sLine.RemoveLineEndings();
	m_lstEntries.Add(sLine);
}

/**
*  @brief
*    Get protocol version
*/
inline EHttpProtocol HttpHeader::GetProtocol() const
{
	// Return protocol version
	return m_nProtocol;
}

/**
*  @brief
*    Get message type
*/
inline EHttpMessageType HttpHeader::GetMessageType() const
{
	// Return message type
	return m_nMessageType;
}

/**
*  @brief
*    Get request
*/
inline EHttpRequest HttpHeader::GetRequest() const
{
	// Return HTTP request
	return m_nRequest;
}

/**
*  @brief
*    Get request URL
*/
inline String HttpHeader::GetRequestUrl() const
{
	// Return request URL
	return m_sRequestUrl;
}

/**
*  @brief
*    Get connection type
*/
inline EHttpConnection HttpHeader::GetConnectionType() const
{
	// Return connection type
	return m_nConnection;
}

/**
*  @brief
*    Get status code
*/
inline uint32 HttpHeader::GetStatusCode() const
{
	// Return status code
	return m_nStatusCode;
}

/**
*  @brief
*    Get status string
*/
inline String HttpHeader::GetStatusString() const
{
	// Return status string
	return m_sStatusString;
}

/**
*  @brief
*    Get location
*/
inline String HttpHeader::GetLocation() const
{
	// Return location
	return m_sLocation;
}

/**
*  @brief
*    Get date
*/
inline String HttpHeader::GetDate() const
{
	// Return date
	return m_sDate;
}

/**
*  @brief
*    Get server signature
*/
inline String HttpHeader::GetServer() const
{
	// Return server signature
	return m_sServer;
}

/**
*  @brief
*    Get client signature
*/
inline String HttpHeader::GetUserAgent() const
{
	// Return client signature
	return m_sUserAgent;
}

/**
*  @brief
*    Get authentication type
*/
inline EHttpAuth HttpHeader::GetAuthenticationType() const
{
	// Return authentication type
	return m_nAuthType;
}

/**
*  @brief
*    Get authentication realm
*/
inline String HttpHeader::GetAuthenticationRealm() const
{
	// Return authentication realm
	return m_sAuthRealm;
}

/**
*  @brief
*    Get authorization string
*/
inline String HttpHeader::GetAuthorization() const
{
	// Return authorization string
	return m_sAuthorization;
}

/**
*  @brief
*    Get transfer encoding
*/
inline String HttpHeader::GetTransferEncoding() const
{
	// Return transfer encoding
	return m_sTransferEncoding;
}

/**
*  @brief
*    Check if data is transfered "chunked"
*/
inline bool HttpHeader::IsChunked() const
{
	// Return if transfer encoding is set to "chunked"
	return m_sTransferEncoding.IsSubstring("chunked");
}

/**
*  @brief
*    Get content length
*/
inline uint32 HttpHeader::GetContentLength() const
{
	// Return content length
	return m_nContentLength;
}

/**
*  @brief
*    Get content language
*/
inline String HttpHeader::GetContentLanguage() const
{
	// Return content language
	return m_sContentLanguage;
}

/**
*  @brief
*    Get content type
*/
inline String HttpHeader::GetContentType() const
{
	// Return content type
	return m_sContentType;
}

/**
*  @brief
*    Get ETag
*/
inline String HttpHeader::GetETag() const
{
	// Return ETag
	return m_sETag;
}

/**
*  @brief
*    Check if partial data is returned
*/
inline bool HttpHeader::IsPartial() const
{
	// Return if partial data is received
	return m_bPartial;
}

/**
*  @brief
*    Get start of range
*/
inline uint32 HttpHeader::GetRangeMin() const
{
	// Return start of range
	return m_nRangeMin;
}

/**
*  @brief
*    Get end of range
*/
inline uint32 HttpHeader::GetRangeMax() const
{
	// Return end of range
	return m_nRangeMax;
}

/**
*  @brief
*    Get total size
*/
inline uint32 HttpHeader::GetRangeTotal() const
{
	// Return total size
	return m_nRangeTotal;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
