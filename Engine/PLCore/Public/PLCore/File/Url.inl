/*********************************************************\
 *  File: Url.inl                                        *
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
*    Constructor
*/
inline Url::Url() :
	m_bValid(false)
{
}

/**
*  @brief
*    Copy constructor
*/
inline Url::Url(const Url &cUrl) :
	m_sUrl(cUrl.m_sUrl),
	m_sProtocol(cUrl.m_sProtocol),
	m_sRoot(cUrl.m_sRoot),
	m_sPath(cUrl.m_sPath),
	m_sFilename(cUrl.m_sFilename),
	m_bValid(cUrl.m_bValid)
{
}

/**
*  @brief
*    Constructor
*/
inline Url::Url(const String &sUrl) :
	m_bValid(false)
{
	// Save path or URL
	SetValue(sUrl);
}

/**
*  @brief
*    Destructor
*/
inline Url::~Url()
{
}

/**
*  @brief
*    Copy an URL
*/
inline Url &Url::operator =(const Url &cUrl)
{
	// Save path or URL
	m_sUrl		= cUrl.m_sUrl;
	m_sProtocol	= cUrl.m_sProtocol;
	m_sRoot		= cUrl.m_sRoot;
	m_sPath		= cUrl.m_sPath;
	m_sFilename	= cUrl.m_sFilename;
	m_bValid	= cUrl.m_bValid;
	return *this;
}

/**
*  @brief
*    Copy an URL (passed as a string)
*/
inline Url &Url::operator =(const String &sUrl)
{
	// Save path or URL
	SetValue(sUrl);
	return *this;
}

/**
*  @brief
*    Concatenate an URL
*/
inline Url Url::operator +(const Url &cUrl) const
{
	// Check if the first URL is valid
	if (m_bValid) {
		// Check if the second URL is valid and can be added
		if ( cUrl.IsValid() &&															// URL must be valid
			(cUrl.GetProtocol() == GetProtocol() || !cUrl.GetProtocol().GetLength()) &&	// Protocols must match
			 cUrl.IsRelative() &&														// URL must be relative
			 IsDirectory()) {															// First URL must not have a filename
			// Compose URLs
			return Url(GetProtocol() + GetRoot() + GetPath() + cUrl.GetPath() + cUrl.GetFilename());
		}

		// Could not add second URL
		return *this;
	}

	// Error, URL is invalid
	return Url();
}

/**
*  @brief
*    Concatenate an URL
*/
inline Url &Url::operator +=(const Url &cUrl)
{
	// Check if the first URL is valid
	if (m_bValid) {
		// Check if the second URL is valid and can be added
		if ( cUrl.IsValid() &&															// URL must be valid
			(cUrl.GetProtocol() == GetProtocol() || !cUrl.GetProtocol().GetLength()) &&	// Protocols must match
			 cUrl.IsRelative() &&														// URL must be relative
			 IsDirectory()) {															// First URL must not have a filename
			// Compose URLs
			SetValue(GetProtocol() + GetRoot() + GetPath() + cUrl.GetPath() + cUrl.GetFilename());
		}
	}

	// Return this URL
	return *this;
}

/**
*  @brief
*    Get value in URL notation
*/
inline String Url::GetUrl() const
{
	return m_sUrl;
}

/**
*  @brief
*    Get value in native notation
*/
inline String Url::GetNativePath() const
{
	#if defined(WIN32)
		return GetWindowsPath();
	#elif defined(LINUX)
		return GetUnixPath();
	#endif
}

/**
*  @brief
*    Get value in Windows notation
*/
inline String Url::GetWindowsPath() const
{
	// Convert to Windows path
	if (m_sProtocol.CompareNoCase("file://") || !m_sProtocol.GetLength()) {
		// Compose Windows path
		String sUrl = m_sRoot + m_sPath + m_sFilename;
		sUrl.Replace('/', '\\');
		return sUrl;
	} else {
		// Cannot convert, return URL instead
		return GetUrl();
	}
}

/**
*  @brief
*    Get value in Unix notation
*/
inline String Url::GetUnixPath() const
{
	// Convert to Unix path
	if (m_sProtocol.CompareNoCase("file://") || !m_sProtocol.GetLength()) {
		// Compose Unix path
		return m_sRoot + m_sPath + m_sFilename;
	} else {
		// Cannot convert, return URL instead
		return GetUrl();
	}
}

/**
*  @brief
*    Returns if the URL is valid
*/
inline bool Url::IsValid() const
{
	// Return whether the URL is valid or not
	return m_bValid;
}

/**
*  @brief
*    Check if the URL is a valid native file path
*/
inline bool Url::IsValidNativePath() const
{
	#if defined(WIN32)
		return IsValidWindowsPath();
	#elif defined(LINUX)
		return IsValidUnixPath();
	#endif
}

/**
*  @brief
*    Check if the URL is valid as a Windows file path
*/
inline bool Url::IsValidWindowsPath() const
{
	return ((m_sProtocol.CompareNoCase("file://") || !m_sProtocol.GetLength()) && (m_sRoot.GetSubstring(1, 2) == ":/" || !m_sRoot.GetLength()));
}

/**
*  @brief
*    Check if the URL is valid as a Unix file path
*/
inline bool Url::IsValidUnixPath() const
{
	return ((m_sProtocol.CompareNoCase("file://") || !m_sProtocol.GetLength()) && (m_sRoot == '/' || !m_sRoot.GetLength()));
}

/**
*  @brief
*    Returns if the URL is empty
*/
inline bool Url::IsEmpty() const
{
	return (m_bValid && !m_sRoot.GetLength() && !m_sPath.GetLength() && !m_sFilename.GetLength());
}

/**
*  @brief
*    Returns if the URL is absolute
*/
inline bool Url::IsAbsolute() const
{
	return (m_bValid && m_sRoot.GetLength());
}

/**
*  @brief
*    Returns if the URL is relative
*/
inline bool Url::IsRelative() const
{
	return (m_bValid && !m_sRoot.GetLength());
}

/**
*  @brief
*    Returns if the URL targets a directory
*/
inline bool Url::IsDirectory() const
{
	return (m_bValid && (m_sRoot.GetLength() || m_sPath.GetLength()) && !m_sFilename.GetLength());
}

/**
*  @brief
*    Returns the protocol part
*/
inline String Url::GetProtocol() const
{
	// Return protocol part
	return m_sProtocol;
}

/**
*  @brief
*    Returns the root part
*/
inline String Url::GetRoot() const
{
	// Return root part
	return m_sRoot;
}

/**
*  @brief
*    Returns the path (without root and filename)
*/
inline String Url::GetPath() const
{
	// Return path
	return m_sPath;
}

/**
*  @brief
*    Returns the filename (including it's extension)
*/
inline String Url::GetFilename() const
{
	// Return filename
	return m_sFilename;
}

/**
*  @brief
*    Returns the path without the filename
*/
inline String Url::CutFilename() const
{
	// Return complete path, omit protocol for local path
	String sPath;
	if (!m_sProtocol.CompareNoCase("file://") && m_sProtocol.GetLength())
		sPath = m_sProtocol;
	sPath += m_sRoot + m_sPath;
	return sPath;
}

/**
*  @brief
*    Returns the filename without it's complete extension (one may also call the result "basename)
*/
inline String Url::GetTitle() const
{
	const int nColon = m_sFilename.IndexOf(".");
	return (nColon >= 0) ? m_sFilename.GetSubstring(0, nColon) : m_sFilename;
}

/**
*  @brief
*    Returns the filename without it's extension (one may also call the result "basename)
*/
inline String Url::GetCompleteTitle() const
{
	const int nColon = m_sFilename.LastIndexOf(".");
	return (nColon >= 0) ? m_sFilename.GetSubstring(0, nColon) : m_sFilename;
}

/**
*  @brief
*    Returns the file extension (aka "suffix")
*/
inline String Url::GetExtension() const
{
	const int nColon = m_sFilename.LastIndexOf(".");
	return (nColon >= 0) ? m_sFilename.GetSubstring(nColon+1) : "";
}

/**
*  @brief
*    Returns the path and filename without extension
*/
inline String Url::CutExtension() const
{
	// Return complete path, omit protocol for local path
	String sPath;
	if (!m_sProtocol.CompareNoCase("file://") && m_sProtocol.GetLength())
		sPath = m_sProtocol;
	sPath += m_sRoot + m_sPath + GetCompleteTitle();
	return sPath;
}

/**
*  @brief
*    Returns the complete file extension (aka "suffix")
*/
inline String Url::GetCompleteExtension() const
{
	const int nColon = m_sFilename.IndexOf(".");
	return (nColon >= 0) ? m_sFilename.GetSubstring(nColon+1) : "";
}

/**
*  @brief
*    Returns the path and filename without complete extension
*/
inline String Url::CutCompleteExtension() const
{
	// Return complete path, omit protocol for local path
	String sPath;
	if (!m_sProtocol.CompareNoCase("file://") && m_sProtocol.GetLength())
		sPath = m_sProtocol;
	sPath += m_sRoot + m_sPath + GetTitle();
	return sPath;
}

/**
*  @brief
*    Returns the first part of the path
*/
inline String Url::GetFirstPath(uint32 &nPathPos) const
{
	nPathPos = 0;
	return GetNextPath(nPathPos);
}

/**
*  @brief
*    Returns the first part of the path
*/
inline String Url::GetFirstPath() const
{
	uint32 nPathPos = 0;
	return GetNextPath(nPathPos);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
