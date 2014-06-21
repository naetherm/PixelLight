/*********************************************************\
 *  File: HTMLParser.h                                   *
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


#ifndef __PLCORE_HTMLPARSER_H__
#define __PLCORE_HTMLPARSER_H__
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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class XmlDocument;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    HTML parser
*
*  @remarks
*    This class provides a simple HTML parser. It does NOT comply with any existent HTML standard,
*    but it tries to be as flexible as possible in allowing many typical errors such as mixed HTML
*    and XML style as well as not properly closed tags. So don't expect this class to be able of parsing
*    any existing homepage out there. However, the class can be used to read in an HTML file and
*    produce an XML syntax tree as if it were an XML file. You can then use standard XML classes to
*    access the parsed document tree.
*/
class HTMLParser {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline HTMLParser();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~HTMLParser();

		/**
		*  @brief
		*    Clear all data
		*/
		PLCORE_API void Clear();

		/**
		*  @brief
		*    Load file
		*
		*  @param[in] sFilename
		*    Filename of the HTML document to load in
		*/
		PLCORE_API void Load(const String &sFilename);

		/**
		*  @brief
		*    Get parsed HTML as an XML document
		*
		*  @return
		*    Pointer to XML document, a null pointer if there's currently no document
		*/
		inline XmlDocument *GetXML() const;

		/**
		*  @brief
		*    Get number of errors occurred while parsing the HTML file
		*
		*  @return
		*    Number of errors
		*/
		inline uint32 GetNumOfErrors() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Parse whole HTML file and generate a XML parsing tree
		*
		*  @return
		*    'true' if HTML file could be parsed, 'false' on error
		*/
		bool Parse();

		/**
		*  @brief
		*    Check if there is another token waiting
		*
		*  @return
		*    'true' if next token is available, else 'false'
		*/
		bool HasNextToken();

		/**
		*  @brief
		*    Get next token from HTML parser
		*
		*  @return
		*    Next token, or "" when there are no more tokens
		*/
		String GetNextToken();

		/**
		*  @brief
		*    Returns whether or not the given string is a single tag (no closing tag required to be well-formed!)
		*
		*  @param[in] sTag
		*    Tag to check
		*
		*  @return
		*    'true' the given string is a single tag, else 'false'
		*/
		inline bool IsSingleTag(const String &sTag) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String		 m_sFilename;	/**< File name */
		String		 m_sText;		/**< Content of HTML file */
		String		 m_sTextLower;	/**< Text version in lower case */
		uint32		 m_nPos;		/**< Parsing position */
		String		 m_sToken;		/**< Currently parsed token */
		XmlDocument	*m_pXML;		/**< Parsed HTML, can be a null pointer */
		uint32		 m_nErrors;		/**< Number of errors */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/HTMLParser.inl"


#endif // __PLCORE_HTMLPARSER_H__
