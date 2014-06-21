/*********************************************************\
 *  File: XmlBase.h                                      *
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


#ifndef __PLCORE_XML_BASE_H__
#define __PLCORE_XML_BASE_H__
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
class File;
class XmlParsingData;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract XML base class
*
*  @remarks
*    'XmlBase' is a base class for every class of the XML parser.
*    It does little except to establish that parser classes can be printed and provide
*    some utility functions. In XML, the document and elements can contain other
*    elements and other types of nodes.
*
*    @verbatim
*    A document can contain: Element (container or leaf)
*                            Comment (leaf)
*                            Unknown (leaf)
*                            Declaration (leaf)
*
*    An element can contain: Element (container or leaf)
*                            Text (leaf)
*                            Attributes (not on tree)
*                            Comment (leaf)
*                            Unknown (leaf)
*
*    A declaration contains: Attributes (not on tree)
*    @endverbatim
*/
class XmlBase {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class XmlParsingData;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Error codes
		*/
		enum {
			NoError,						/**< No error */
			Error,							/**< Error */
			ErrorOpeningFile,				/**< Error opening file */
			ErrorParsingElement,			/**< Error parsing element */
			ErrorFailedToReadElementName,	/**< Error failed to read element name */
			ErrorReadingElementValue,		/**< Error reading element value */
			ErrorReadingAttributes,			/**< Error reading attributes */
			ErrorParsingEmpty,				/**< Error parsing empty */
			ErrorReadingEndTag,				/**< Error reading end tag */
			ErrorParsingUnknown,			/**< Error parsing unknown */
			ErrorParsingComment,			/**< Error parsing comment */
			ErrorParsingDeclaration,		/**< Error parsing declaration */
			ErrorDocumentEmpty,				/**< Error document empty */
			ErrorEmbeddedNull,				/**< Error embedded null */
			ErrorParsingCData,				/**< Error parsing CDATA */
			ErrorDocumentTopOnly,			/**< Error document top only */
			ErrorStringCount				/**< Error string count */
		};

		/**
		*  @brief
		*    Only used by 'Attribute::Query'-functions
		*/
		enum EQueryResult {
			Success,		/**< All went fine */
			NoAttribute,	/**< Attribute does not exist */
			WrongType		/**< Invalid attribute type */
		};

		/**
		*  @brief
		*    Used by the parsing routines
		*/
		enum EEncoding {
			EncodingUnknown,	/**< Unknown encoding (default) */
			EncodingUTF8,		/**< UTF8 encoding */
			EncodingLegacy		/**< Legacy encoding */
		};


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the current white space setting
		*
		*  @return
		*    'true' if white space condensed is set, else 'false'
		*
		*  @remarks
		*    The world does not agree on whether white space should be kept or
		*    not. In order to make everyone happy, these global, static functions
		*    are provided to set whether or not the parser will condense all white space
		*    into a single space or not. The default is to condense. Note changing this
		*    values is not thread safe.
		*/
		static inline bool IsWhiteSpaceCondensed();

		/**
		*  @brief
		*    Set white space handling
		*
		*  @param[in] bCondense
		*    'true' if white space condensed is set, else 'false'
		*
		*  @see
		*    - IsWhiteSpaceCondensed()
		*/
		static inline void SetCondenseWhiteSpace(bool bCondense);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~XmlBase();

		/**
		*  @brief
		*    Return the row position, in the original source file, of this node or attribute
		*
		*  @return
		*    Row position
		*
		*  @remarks
		*    The row and column are 1-based. (That is the first row and first column is
		*    1, 1). If the returns values are 0 or less, then the parser does not have
		*    a row and column value.
		*    Generally, the row and column value will be set when the 'XmlDocument::Load()',
		*    'XmlDocument::Load()', or any 'XmlNode::Parse()' is called. It will NOT be set
		*    when the DOM was created from operator '>>'.
		*    The values reflect the initial load. Once the DOM is modified programmatically
		*    (by adding or changing nodes and attributes) the new values will NOT update to
		*    reflect changes in the document.
		*    There is a minor performance cost to computing the row and column. Computation
		*    can be disabled if 'XmlDocument::SetTabSize()' is called with 0 as the value.
		*
		*  @see
		*    - XmlDocument::SetTabSize()
		*/
		inline int GetRow() const;

		/**
		*  @brief
		*    Return the column position, in the original source file, of this node or attribute
		*
		*  @return
		*    Column position
		*
		*  @see
		*    - GetRow()
		*/
		inline int GetColumn() const;

		/**
		*  @brief
		*    Returns the user data
		*
		*  @return
		*    User data, can be a null pointer
		*
		*  @note
		*    - The user data is not used internally, it's really user only :)
		*/
		inline void *GetUserData() const;

		/**
		*  @brief
		*    Sets the user data
		*
		*  @param[in] pUser
		*    User data, can be a null pointer
		*
		*  @see
		*    - GetUserData()
		*/
		inline void SetUserData(void *pUser);


	//[-------------------------------------------------------]
	//[ Public virtual XmlBase functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Save function
		*
		*  @param[out] cFile
		*    File to write in, must be opened and writable
		*  @param[in]  nDepth
		*    Current depth
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Save(File &cFile, uint32 nDepth = 0) = 0;

		/**
		*  @brief
		*    Output as string function
		*
		*  @param[in] nDepth
		*    Current depth
		*
		*  @return
		*    String containing the XML
		*/
		virtual String ToString(uint32 nDepth = 0) const = 0;

		/**
		*  @brief
		*    Parse the given null terminated block of XML data
		*
		*  @param[in] pszData
		*    Parsing data, if a null pointer, an error will be returned
		*  @param[in] pData
		*    Parsing data, can be a null pointer
		*  @param[in] nEncoding
		*    Encoding
		*
		*  @return
		*    The pointer to the parameter 'pszData' if all went fine, else a null pointer
		*
		*  @remarks
		*    Passing in an encoding to this method (either 'EncodingLegacy' or
		*    'EncodingUTF8' will force the parser to use that encoding, regardless
		*    of what the parser might otherwise try to detect.
		*/
		virtual const char *Parse(const char *pszData, XmlParsingData *pData = nullptr, EEncoding nEncoding = EncodingUnknown) = 0;


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Internal structure for tracking location of items in the XML file
		*/
		struct Cursor {
			Cursor()		{ nRow = nColumn = -1; }
			void Clear()	{ nRow = nColumn = -1; }

			int nRow;		// 0 based
			int nColumn;	// 0 based
		};

		// Bunch of unicode info at:
		//		http://www.unicode.org/faq/utf_bom.html
		// Including the basic of this table, which determines the #bytes in the
		// sequence from the lead byte. 1 placed for invalid sequences --
		// although the result will be junk, pass it through as much as possible.
		// Beware of the non-characters in UTF-8:	
		//				ef bb bf (Microsoft "lead bytes")
		//				ef bf be
		//				ef bf bf 
		static const unsigned char UTF_LEAD_0 = 0xefU;
		static const unsigned char UTF_LEAD_1 = 0xbbU;
		static const unsigned char UTF_LEAD_2 = 0xbfU;


	//[-------------------------------------------------------]
	//[ Protected static functions                            ]
	//[-------------------------------------------------------]
	protected:
		static const char *SkipWhiteSpace(const char *pszData, EEncoding nEncoding);
		static bool IsWhiteSpace(char c);
		static bool IsWhiteSpace(int c);

		/**
		*  @brief
		*    Reads an XML name into the string provided
		*
		*  @param[in]  pszData
		*    Data
		*  @param[out] sName
		*    Read name
		*  @param[in] nEncoding
		*    Encoding
		*
		*  @return
		*    Returns a pointer just past the last character of the name, or 0 if the function has an error
		*/
		static const char *ReadName(const char *pszData, String &sName, EEncoding nEncoding);

		/**
		*  @brief
		*    Reads text
		*
		*  @param[in]  pszData
		*    Where to start
		*  @param[out] sText
		*    The string read
		*  @param[in]  bTrimWhiteSpace
		*    Whether to keep the white space
		*  @param[in]  pszEndTag
		*    What ends this text
		*  @param[in]  bCaseInsensitive
		*    Whether to ignore case in the end tag
		*  @param[in]  nEncoding
		*    The current encoding
		*
		*  @return
		*    Returns a pointer past the given end tag
		*
		*  @note
		*    - Wickedly complex options, but it keeps the (sensitive) code in one place.
		*/
		static const char *ReadText(const char *pszData, String &sText, bool bTrimWhiteSpace, const char *pszEndTag, bool bCaseInsensitive, EEncoding nEncoding);

		// If an entity has been found, transform it into a character.
		static const char *GetEntity(const char *pszData, char *pszValue, int &nLength, EEncoding nEncoding);

		// Get a character, while interpreting entities.
		// The length can be from 0 to 4 bytes.
		static const char *GetChar(const char *pszData, char *pszValue, int &nLength, EEncoding nEncoding);

		// Return true if the next characters in the stream are any of the endTag sequences.
		// Ignore case only works for english, and should only be relied on when comparing
		// to English words: StringEqual(pszData, "version", true) is fine.
		static bool StringEqual(const char *pszData, const char *pszTag, bool bIgnoreCase, EEncoding nEncoding);

		// None of these methods are reliable for any language except English.
		// Good for approximation, not great for accuracy.
		static int IsAlpha(unsigned char nByte);
		static int IsAlphaNum(unsigned char nByte);
		static int ToLower(int nValue, EEncoding nEncoding);
		static void ConvertUTF32ToUTF8(unsigned long nInput, char *pszOutput, int &nLength);

		// Expands entities in a string. Note this should not contain the tag's '<', '>', etc, 
		// or they will be transformed into entities!
		static void EncodeString(const String &sInString, String &sOutString);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		XmlBase();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		void   *m_pUserData;	/**< User data, can be a null pointer */
		Cursor  m_cCursor;		/**< Cursor */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		XmlBase(const XmlBase &cSource);

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
		XmlBase &operator =(const XmlBase &cSource);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static const int utf8ByteTable[256];	/**< Table that returns, for a given lead byte, the total number of bytes in the UTF-8 sequence */
		struct Entity {
			String sString;
			char   nCharacter;
		};
		enum {
			NumOfEntities = 5
		};
		static Entity sEntity[NumOfEntities];
		static PLCORE_API bool bCondenseWhiteSpace;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlBase.inl"


#endif // __PLCORE_XML_BASE_H__
