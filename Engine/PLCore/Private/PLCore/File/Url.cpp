/*********************************************************\
 *  File: Url.cpp                                        *
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
#include "PLCore/Container/Stack.h"
#include "PLCore/String/RegEx.h"
#include "PLCore/File/Url.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the next part of the path
*/
String Url::GetNextPath(uint32 &nPathPos) const
{
	const String sTemp = m_sPath.GetSubstring(nPathPos);
	const int nBacks = sTemp.IndexOf('\\');
	int nSlash = sTemp.IndexOf('/');
	if (nSlash < 0 || (nBacks >= 0 && nBacks < nSlash))
		nSlash = nBacks;
	if (nSlash >= 0) {
		nPathPos += nSlash + 1;
		return sTemp.GetSubstring(0, nSlash);
	} else {
		nPathPos += sTemp.GetLength();
		return "";
	}
}

/**
*  @brief
*    Collapses the URL
*/
Url &Url::Collapse()
{
	// Check if the URL is valid
	if (m_bValid) {
		// Validate paths (Resolve paths like '..' and '.')
		Stack<String> cNameStack;
		uint32 nPos;
		String sName = GetFirstPath(nPos);
		while (sName.GetLength()) {
			// Check next subdir
			const String sTop = cNameStack.Top();
			if (sName.GetLength()) {
				if (sName == ".." && sTop != ".." && (sTop.GetLength() || m_sRoot.GetLength())) {
					// Delete last subdir when '..' occurs
					// Note: m_sRoot.GetLength() is there because e.g. "C:\.." is a valid directory on Windows systems.
					// To avoid directory names like C:\..\..\Programs\", this function will therefore ignore
					// ".." at the beginning of a filename, but ONLY if the root is not empty (relative paths
					// can of cause begin with "..")
					if (sTop.GetLength())
						cNameStack.Pop();
				} else if (sName == '.') {
					// Ignore '.'
				} else {
					// Push subdir on stack
					cNameStack.Push(sName);
				}
			}

			// Read next
			sName = GetNextPath(nPos);
		}

		// Look for .. or . in the filename
		const String sTop = cNameStack.Top();
		if (m_sFilename == ".." && sTop != ".." && (sTop.GetLength() || m_sRoot.GetLength())) {
			cNameStack.Pop();
			m_sFilename = cNameStack.Top();
			cNameStack.Pop();
		} else if (m_sFilename == '.') {
			m_sFilename = cNameStack.Top();
			if (!m_sFilename.GetLength())
				m_sFilename = '.';
			cNameStack.Pop();
		}

		// Put path together
		m_sPath = "";
		while (cNameStack.GetNumOfElements() > 0) {
			m_sPath = cNameStack.Top() + '/' + m_sPath;
			cNameStack.Pop();
		}

		// Compose new path or URL
		m_sUrl = m_sProtocol + m_sRoot + m_sPath + m_sFilename;
	}

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the value of the URL
*/
void Url::SetValue(const String &sUrl)
{
	// Parse the URL according to the following regular expression:
	//   ({Name} '://')? '/'? ({Letter} (':/' | ':\'))? ({Name} ('/' | '\')+)* {Name}?
	//   Where {Name} is any string without '/', '\' and ':'
	//   and   {Letter} is ['a'..'z', A'..'Z']

	// Init URL
	m_sUrl		= "";
	m_sProtocol	= "";
	m_sRoot		= "";
	m_sPath		= "";
	m_sFilename	= "";
	m_bValid	= false;

	// Check some death conditions
	if (sUrl.GetLength()) {
		// Analyze URL
		String sTemp = sUrl;

		// Check for a protocol, like "http://"
		String sProtocol, sRoot;
		int nPos = sTemp.IndexOf("://");
		if (nPos >= 2) { // the protocol name must be at least of length 2 (otherwise C:// is treated as a protocol)
			// Token "://" has been found, now check the protocol
			sProtocol = sUrl.GetSubstring(0, nPos);
			if (!IsName(sProtocol)) {
				// There are errors in the protocol string!
				return;
			}

			// Valid protocol has been found, validate ('Http://' -> 'http')
			sProtocol += "://";
			sProtocol.ToLower();
			sTemp = sTemp.GetSubstring(sProtocol.GetLength());
		} else {
			// UNC (Uniform Naming Convention aka Universal Naming Convention) support
			// UNC = \\<host name>\<share name>[\<object name>]*
			// Example of a UNC directory: "\\MY-PC\Users\Me\UNC_Test"
			// There are three parts within UNC names, here's how they are "mapped" on the current URL class design:
			// - Host/server name   -> Protocol      (from the above example: "\\MY-PC")
			// - Share name         -> Root          (from the above example: "\Users")
			// - Optional file path -> Path/filename (from the above example: "\Me\UNC_Test")
			// Regular expression basing on a discussion at: http://channel9.msdn.com/forums/TechOff/132283-regex-UNC-share
			static RegEx cRegEx("^"								// Start of string
								"("								// Begin register 1
								"\\\\\\\\"						// Two literal backslashes
								"[^/\\\\\\]\\[\":;|<>+=,?* _]+"	// Host/server name -> protocol
								"\\\\"							// One literal backslash
								")"								// End register 1
								"("								// Begin register 2
								"[^/\\\\\\]\\[\":;|<>+=,?*]+"	// Share name -> Root
								")"								// End register 2
								"(("							// Begin register 3
								"\\\\"							// One literal backslash
								"[^\\\\/:*?\"<>|]+"				// Optional file path -> Path/filename
								")*)"							// End register 3
								"\\\\?"							// Optional terminating backslash
								"$"								// End of string
								);
			if (cRegEx.Match(sTemp)) {
				sProtocol	= cRegEx.GetResult(0);	// Host/server name   -> Protocol
				sRoot		= cRegEx.GetResult(1);	// Share name         -> Root
				sTemp		= cRegEx.GetResult(2);	// Optional file path -> Path/filename

				// It's not allowed that there's a slash at the beginning of the path, we could
				// make the regular expression even more complex, but the following works as well...
				sRoot += sTemp.GetASCII()[0];
				sTemp.Delete(0, 1);
			}
		}

		// Check for file root ('/')
		if (!sRoot.GetLength() && (sTemp[static_cast<uint32>(0)] == '/' || sTemp[static_cast<uint32>(0)] == '\\')) {
			// Root sign found
			sRoot = sTemp[static_cast<uint32>(0)];
			sTemp = sTemp.GetSubstring(1);
		}

		// Check for a drive letter, like C:\ or C:/
		if (IsLetter(sTemp[static_cast<uint32>(0)]) && sTemp[static_cast<uint32>(1)] == ':' && (sTemp[static_cast<uint32>(2)] == '/' || sTemp[static_cast<uint32>(2)] == '\\')) {
			// Drive letter found, validate ('c:' -> 'C:')
			sRoot = sTemp.GetSubstring(0, 1).ToUpper() + ":/";
			sTemp = sTemp.GetSubstring(3);
		}

		// Set protocol to "file://", if there is no protocol and the root is not empty
		if (!sProtocol.GetLength() && sRoot.GetLength())
			sProtocol = "file://";

		// Check path
		String sPath, sFilename;
		while (sTemp.GetLength()) {
			// Get '/' or '\'
			const int nBacks = sTemp.IndexOf('\\');
			int nSlash = sTemp.IndexOf('/');
			if (nSlash < 0 || (nBacks >= 0 && nBacks < nSlash))
				nSlash = nBacks;

			// Check name
			const String sName = (nSlash >= 0) ? sTemp.GetSubstring(0, nSlash) : sTemp;
			if (IsName(sName)) {
				// Compose path
				if (nSlash >= 0) {
					// Add to path
					sPath += sName + '/';
				} else {
					// Get filename
					sFilename = sTemp;
				}
			} else {
				// Ignore empty names - removing duplicate slashes is ok
			}

			// Get next part
			if (nSlash >= 0)
				sTemp = sTemp.GetSubstring(nSlash+1);
			else
				sTemp = "";
		}

		// Last checks (Unix path with backslashes)
		if (sRoot != '/' || m_sUrl.IndexOf('\\') < 0) {
			// No errors, save result
			m_bValid	= true;
			m_sProtocol	= sProtocol;
			m_sRoot		= sRoot;
			m_sPath		= sPath;
			m_sFilename	= sFilename;
			m_sUrl		= sProtocol + sRoot + sPath + sFilename;
		}
	}
}

/**
*  @brief
*    Checks if the character is a letter (a..z, A..Z)
*/
inline bool Url::IsLetter(char nChar) const
{
	// Check if the character is a letter
	return ((nChar >= 'a' && nChar <= 'z') || (nChar >= 'A' && nChar <= 'Z'));
}

/**
*  @brief
*    Checks if the string is a valid name (does not contain ':', '/' or '\')
*/
inline bool Url::IsName(const String &sString) const
{
	// The string is not a valid name, if it is empty or if it contains ':', '/' or '\'
	return (sString.GetLength() && !sString.IsSubstring(':') && !sString.IsSubstring('/') && !sString.IsSubstring('\\'));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
