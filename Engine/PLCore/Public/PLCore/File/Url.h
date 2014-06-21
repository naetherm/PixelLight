/*********************************************************\
 *  File: Url.h                                          *
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


#ifndef __PLCORE_URL_H__
#define __PLCORE_URL_H__
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
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Contains a path or URL (Uniform Resource Locator)
*
*  @note
*    - An URL is always stored using a protocol ("file://" for a local path) and "/" as the delimiter.
*    - For getting the URL in native style, use the specific functions (GetWindowsPath(), GetUnixPath(), GetNativePath())
*    - If there's a "/" at the end of a given path it will be kept, but no "/" is added automatically
*/
class Url {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline Url();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cUrl
		*    URL to copy
		*/
		inline Url(const Url &cUrl);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sUrl
		*    URL as string
		*/
		inline Url(const String &sUrl);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Url();

		/**
		*  @brief
		*    Copy an URL
		*
		*  @param[in] cUrl
		*    URL to copy
		*
		*  @return
		*    Reference to this URL
		*/
		inline Url &operator =(const Url &cUrl);

		/**
		*  @brief
		*    Copy an URL (passed as a string)
		*
		*  @param[in] sUrl
		*    String containing URL to copy
		*
		*  @return
		*    Reference to this URL
		*/
		inline Url &operator =(const String &sUrl);

		/**
		*  @brief
		*    Concatenate an URL
		*
		*  @param[in] cUrl
		*    URL to add
		*
		*  @return
		*    Resulting URL
		*
		*  @note
		*    - Both URLs must be valid
		*    - The second URL must be relative
		*    - The protocol of the second URL must match the first one or be empty
		*/
		inline Url operator +(const Url &cUrl) const;

		/**
		*  @brief
		*    Concatenate an URL
		*
		*  @param[in] cUrl
		*    URL to add
		*
		*  @return
		*    Reference to this URL
		*
		*  @see
		*    - operator +
		*/
		inline Url &operator +=(const Url &cUrl);

		/**
		*  @brief
		*    Get value in URL notation
		*
		*  @return
		*    Path in URL style
		*
		*  @note
		*    - If the value contains a local file path, a file:// URL is returned
		*    - Any other URL (e.g. http://) is returned directly
		*    - For example "Url("C:\\Programs\\App\\App.exe")" will result in "file://C:/Programs/App/App.exe"
		*    - For example "Url("C:\\Programs\\App\\")" will result in "file://C:/Programs/App/"
		*    - For example "Url("C:\\Programs\\App")" will result in "file://C:/Programs/App"
		*/
		inline String GetUrl() const;

		/**
		*  @brief
		*    Get value in native notation
		*
		*  @return
		*    Path in native path format style
		*
		*  @note
		*    - Returns the URL in native (Windows/Unix) notation, if it is a local path
		*    - If it is another URL (e.g. http://), the URL is returned
		*    - On Windows for example "Url("C:\\Programs\\App\\App.exe")" will result in "C:\Programs\App\App.exe"
		*    - On Windows for example "Url("C:\\Programs\\App\\")" will result in "C:\Programs\App\"
		*    - On Windows for example "Url("C:\\Programs\\App")" will result in "C:\Programs\App"
		*    - On Linux for example "Url("C:\\Programs\\App\\App.exe")" will result in "C:/Programs/App/App.exe"
		*    - On Linux for example "Url("C:\\Programs\\App\\")" will result in "C:/Programs/App/"
		*    - On Linux for example "Url("C:\\Programs\\App")" will result in "C:/Programs/App"
		*/
		inline String GetNativePath() const;

		/**
		*  @brief
		*    Get value in Windows notation
		*
		*  @return
		*    Path in Windows format style
		*
		*  @note
		*    - If the value contains a valid Windows or Unix file path or an file:// URL, it is returned in Windows style
		*    - If the value contains another URL (e.g. http://), the URL is returned without being converted
		*    - For example "Url("C:\\Programs\\App\\App.exe")" will result in "C:\Programs\App\App.exe"
		*    - For example "Url("C:\\Programs\\App\\")" will result in "C:\Programs\App\"
		*    - For example "Url("C:\\Programs\\App")" will result in "C:\Programs\App"
		*/
		inline String GetWindowsPath() const;

		/**
		*  @brief
		*    Get value in Unix notation
		*
		*  @return
		*    Path in Unix format style
		*
		*  @note
		*    - If the value contains a valid Windows or Unix file path or an file:// URL, it is returned in Unix style
		*    - If the value contains another URL (e.g. http://), the URL is returned without being converted
		*    - For example "Url("C:\\Programs\\App\\App.exe")" will result in "C:/Programs/App/App.exe"
		*    - For example "Url("C:\\Programs\\App\\")" will result in "C:/Programs/App/"
		*    - For example "Url("C:\\Programs\\App")" will result in "C:/Programs/App"
		*/
		inline String GetUnixPath() const;

		/**
		*  @brief
		*    Returns if the URL is valid
		*
		*  @return
		*    'true' if the value is valid, else 'false'
		*/
		inline bool IsValid() const;

		/**
		*  @brief
		*    Check if the URL is a valid native file path
		*
		*  @return
		*    'true', if the path is valid in the native path format style
		*
		*  @note
		*    - A wrapper for IsValidWindowsPath()/IsValidUnixPath() based on the used system
		*/
		inline bool IsValidNativePath() const;

		/**
		*  @brief
		*    Check if the URL is valid as a Windows file path
		*
		*  @return
		*    'true', if the path is valid in Windows format style
		*
		*  @note
		*    - 'false', if the protocol is any other than "file://" or empty
		*    - 'false', if the file root is any other than "X:/" or empty ("/" is invalid!)
		*/
		inline bool IsValidWindowsPath() const;

		/**
		*  @brief
		*    Check if the URL is valid as a Unix file path
		*
		*  @return
		*    'true', if the path is valid in Unix format style
		*
		*  @note
		*    - 'false', if the protocol is any other than "file://" or empty
		*    - 'false', if the file root is any other than "/" or empty ("X:/" is invalid!)
		*/
		inline bool IsValidUnixPath() const;

		/**
		*  @brief
		*    Returns if the URL is empty
		*
		*  @return
		*    'true' if the path is empty, else 'false'
		*
		*  @note
		*    - The protocol is ignored
		*/
		inline bool IsEmpty() const;

		/**
		*  @brief
		*    Returns if the URL is absolute
		*
		*  @return
		*    'true' if the path is absolute, else 'false'
		*/
		inline bool IsAbsolute() const;

		/**
		*  @brief
		*    Returns if the URL is relative
		*
		*  @return
		*    'true' if the path is relative, else 'false'
		*/
		inline bool IsRelative() const;

		/**
		*  @brief
		*    Returns if the URL targets a directory
		*
		*  @return
		*    'true' if the path targets a directory, else 'false'
		*
		*  @note
		*    - Returns true, if the URL ends with "/" which indicates a directory
		*    - This is a consideration on pure syntax level - it does not mean that the path pointed
		*      to is really a directory or that it even exists!
		*/
		inline bool IsDirectory() const;

		/**
		*  @brief
		*    Returns the protocol part
		*
		*  @return
		*    Protocol part (e.g. "file://" or "http://")
		*/
		inline String GetProtocol() const;

		/**
		*  @brief
		*    Returns the root part
		*
		*  @return
		*    Root part (e.g. "C:/" or "/")
		*/
		inline String GetRoot() const;

		/**
		*  @brief
		*    Returns the path (without root and filename)
		*
		*  @return
		*    Path (e.g. "Programs/")
		*/
		inline String GetPath() const;

		/**
		*  @brief
		*    Returns the filename (including it's extension)
		*
		*  @return
		*    Filename (e.g. "readme.txt")
		*/
		inline String GetFilename() const;

		/**
		*  @brief
		*    Returns the path without the filename
		*
		*  @return
		*    Path without filename (e.g. "C:/Programs/App/App.exe"->"C:/Programs/App/")
		*
		*  @note
		*    - The protocol is omitted for a local path (e.g. the result will not be "file://C:/Programs/App/")
		*/
		inline String CutFilename() const;

		/**
		*  @brief
		*    Returns the filename without it's complete extension (one may also call the result "basename)
		*
		*  @remarks
		*  @verbatim
		*   Example showing the difference between "GetTitle()", "GetCompleteTitle()", "GetExtension()", "CutExtension()", "GetCompleteExtension()" and "CutCompleteExtension()":
		*     Url cUrl("C:\\Programs\\App\\archive.tar.gz");
		*     String sTitle                = cUrl.GetTitle();              // Result is "archive"
		*     String sCompleteTitle        = cUrl.GetCompleteTitle();      // Result is "archive.tar"
		*     String sExtension            = cUrl.GetExtension();          // Result is "gz"
		*     String sCutExtension         = cUrl.CutExtension();          // Result is "C:\Programs\App\archive.tar"
		*     String sCompleteExtension    = cUrl.GetCompleteExtension();  // Result is "tar.gz"
		*     String sCutCompleteExtension = cUrl.CutCompleteExtension();  // Result is "C:\Programs\App\archive"
		*  @endverbatim
		*
		*  @return
		*    Filename without complete extension (e.g. "readme" if the filename was "readme.txt", "archive" if the filename was "archive.tar.gz")
		*/
		inline String GetTitle() const;

		/**
		*  @brief
		*    Returns the filename without it's extension (one may also call the result "basename)
		*
		*  @return
		*    Filename without extension (e.g. "readme" if the filename was "readme.txt", "archive.tar" if the filename was "archive.tar.gz")
		*
		*  @see
		*    - "GetTitle()" for an usage example
		*/
		inline String GetCompleteTitle() const;

		/**
		*  @brief
		*    Returns the file extension (aka "suffix")
		*
		*  @return
		*    File extension (e.g. "txt" if the filename was "readme.txt", "gz" if the filename was "archive.tar.gz")
		*
		*  @see
		*    - "GetTitle()" for an usage example
		*/
		inline String GetExtension() const;

		/**
		*  @brief
		*    Returns the path and filename without extension
		*
		*  @return
		*    Path and filename without extension (e.g. "C:\Programs\App\App.exe"->"C:\Programs\App\App" or "C:\Programs\App\archive.tar.gz"->"C:\Programs\App\archive.tar")
		*
		*  @see
		*    - "GetTitle()" for an usage example
		*/
		inline String CutExtension() const;

		/**
		*  @brief
		*    Returns the complete file extension (aka "suffix")
		*
		*  @return
		*    File extension (e.g. "txt" if the filename was "readme.txt", "tar.gz" if the filename was "archive.tar.gz")
		*
		*  @see
		*    - "GetTitle()" for an usage example
		*/
		inline String GetCompleteExtension() const;

		/**
		*  @brief
		*    Returns the path and filename without complete extension
		*
		*  @return
		*    Path and filename without complete extension (e.g. "C:\Programs\App\App.exe"->"C:\Programs\App\App" or "C:\Programs\App\archive.tar.gz"->"C:\Programs\App\archive")
		*
		*  @see
		*    - "GetTitle()" for an usage example
		*/
		inline String CutCompleteExtension() const;

		/**
		*  @brief
		*    Returns the first part of the path
		*
		*  @param[out] nPathPos
		*    Will receive the new position within the path
		*
		*  @return
		*    First path
		*
		*  @note
		*    - Ignores the root (e.g. "C:/" or "/")
		*    - Example: "test/foo.bar"->"test"
		*/
		inline String GetFirstPath(uint32 &nPathPos) const;

		/**
		*  @brief
		*    Returns the first part of the path
		*
		*  @return
		*    First path
		*
		*  @note
		*    - Ignores the root (e.g. "C:/" or "/")
		*    - Example: "test/foo.bar"->"test"
		*/
		inline String GetFirstPath() const;

		/**
		*  @brief
		*    Returns the next part of the path
		*
		*  @param[in, out] nPathPos
		*    Position within the path
		*
		*  @return
		*    First path
		*
		*  @note
		*    - Call GetFirstPath() first
		*/
		PLCORE_API String GetNextPath(uint32 &nPathPos) const;

		/**
		*  @brief
		*    Collapses the URL
		*
		*  @return
		*    Reference to this object
		*
		*  @note
		*    - Collapsing means, '..' and '.' inside the path will be resolved (on a syntactical level only!)
		*    - If the URL is invalid, the function will fail
		*/
		PLCORE_API Url &Collapse();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets the value of the URL
		*
		*  @param[in] sUrl
		*    New URL value
		*/
		PLCORE_API void SetValue(const String &sUrl);

		/**
		*  @brief
		*    Checks if the character is a letter (a..z, A..Z)
		*
		*  @param[in] nChar
		*    Character to check
		*
		*  @return
		*    'true', if the character is a letter, else 'false'
		*/
		inline bool IsLetter(char nChar) const;

		/**
		*  @brief
		*    Checks if the string is a valid name (does not contain ':', '/' or '\')
		*
		*  @param[in] sString
		*    String to check
		*
		*  @return
		*    'true', if the string is a valid name, else 'false'
		*/
		inline bool IsName(const String &sString) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String m_sUrl;		/**< Path or URL */
		String m_sProtocol;	/**< Protocol part (e.g. "http://") */
		String m_sRoot;		/**< Root part (e.g. "/" or "C:\") */
		String m_sPath;		/**< Path (e.g. "Programs\") */
		String m_sFilename;	/**< Filename (e.g. "readme.txt") */
		bool   m_bValid;	/**< Is the URL valid? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/File/Url.inl"


#endif // __PLCORE_URL_H__
