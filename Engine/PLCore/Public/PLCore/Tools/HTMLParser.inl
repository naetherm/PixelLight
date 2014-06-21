/*********************************************************\
 *  File: HTMLParser.inl                                 *
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
inline HTMLParser::HTMLParser() :
	m_nPos(0),
	m_pXML(nullptr),
	m_nErrors(0)
{
}

/**
*  @brief
*    Destructor
*/
inline HTMLParser::~HTMLParser()
{
	// Destroy data
	Clear();
}

/**
*  @brief
*    Get parsed HTML as an XML document
*/
inline XmlDocument *HTMLParser::GetXML() const
{
	// Return HTML tree
	return m_pXML;
}

/**
*  @brief
*    Get number of errors occurring while parsing the HTML file
*/
inline uint32 HTMLParser::GetNumOfErrors() const
{
	// Return number of errors
	return m_nErrors;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether or not the given string is a single tag (no closing tag required to be well-formed!)
*/
inline bool HTMLParser::IsSingleTag(const String &sTag) const
{
	// Identify known tokens that don't need a closing-tag
	return (sTag == "img" || sTag == "br" || sTag == "hr");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
