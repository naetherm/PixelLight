/*********************************************************\
 *  File: XmlDeclaration.h                               *
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


#ifndef __PLCORE_XML_DECLARATION_H__
#define __PLCORE_XML_DECLARATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    XML declaration node
*
*  @remarks
*    In correct XML the declaration is the first entry in the file.
*    @verbatim
*    <?xml version="1.0" standalone="yes"?>
*    @endverbatim
*
*    The parser will happily read or write files without a declaration,
*    however. There are 3 possible attributes to the declaration:
*    version, encoding, and standalone.
*
*  @note
*    - In this version of the code, the attributes are handled as special cases, not
*      generic attributes, simply because there can only be at most 3 and they are
*      always the same.
*/
class XmlDeclaration : public XmlNode {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class XmlNode;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API XmlDeclaration();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLCORE_API XmlDeclaration(const XmlDeclaration &cSource);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sVersion
		*    Version
		*  @param[in] sEncoding
		*    Encoding
		*  @param[in] sStandalone
		*    Standalone
		*/
		PLCORE_API XmlDeclaration(const String &sVersion, const String &sEncoding, const String &sStandalone);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~XmlDeclaration();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		PLCORE_API XmlDeclaration &operator =(const XmlDeclaration &cSource);

		/**
		*  @brief
		*    Version - will return an empty string if none was found
		*
		*  @return
		*    Version - will return an empty string if none was found
		*/
		inline String GetVersion() const;

		/**
		*  @brief
		*    Encoding - will return an empty string if none was found
		*
		*  @return
		*    Encoding - will return an empty string if none was found
		*/
		inline String GetEncoding() const;

		/**
		*  @brief
		*    Standalone - will return an empty string if none was found
		*
		*  @return
		*    Standalone - will return an empty string if none was found
		*/
		inline String GetStandalone() const;


	//[-------------------------------------------------------]
	//[ Public virtual XmlBase functions                      ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual bool Save(File &cFile, uint32 nDepth = 0) override;
		PLCORE_API virtual String ToString(uint32 nDepth = 0) const override;
		PLCORE_API virtual const char *Parse(const char *pszData, XmlParsingData *pData = nullptr, EEncoding nEncoding = EncodingUnknown) override;


	//[-------------------------------------------------------]
	//[ Public virtual XmlNode functions                      ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual XmlNode *Clone() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String m_sVersion;		/**< Version */
		String m_sEncoding;		/**< Encoding */
		String m_sStandalone;	/**< Standalone */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlDeclaration.inl"


#endif // __PLCORE_XML_DECLARATION_H__
