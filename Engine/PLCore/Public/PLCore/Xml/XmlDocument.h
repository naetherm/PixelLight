/*********************************************************\
 *  File: XmlDocument.h                                  *
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


#ifndef __PLCORE_XML_DOCUMENT_H__
#define __PLCORE_XML_DOCUMENT_H__
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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class XmlParsingData;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    XML (Extensible Markup Language) document node
*
*  @remarks
*    Always the top level node. A document binds together all the
*    XML pieces. It can be saved, loaded, and printed to the screen.
*    The 'value' of a document node is the XML filename.
*
*    The XML DOM parser is basing on TinyXML (http://www.sourceforge.net/projects/tinyxml).
*/
class XmlDocument : public XmlNode {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class XmlText;
	friend class XmlNode;
	friend class XmlUnknown;
	friend class XmlElement;
	friend class XmlComment;
	friend class XmlAttribute;
	friend class XmlDeclaration;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API XmlDocument();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Document name. The name of the document is also the filename of the XML.
		*/
		PLCORE_API XmlDocument(const String &sName);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLCORE_API XmlDocument(const XmlDocument &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~XmlDocument();

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
		PLCORE_API XmlDocument &operator =(const XmlDocument &cSource);

		/**
		*  @brief
		*    Load a file using the current document value
		*
		*  @param[in] nEncoding
		*    Encoding
		*
		*  @return
		*    Returns 'true' if successful, else 'false'
		*
		*  @note
		*    - Will delete any existing document data before loading
		*/
		inline bool Load(EEncoding nEncoding = EncodingUnknown);

		/**
		*  @brief
		*    Load a file using the given filename
		*
		*  @param[in] sFilename
		*    Filename
		*  @param[in] nEncoding
		*    Encoding
		*
		*  @return
		*    Returns 'true' if successful, else 'false'
		*
		*  @note
		*    - The document value is set to 'sFilename'
		*/
		PLCORE_API bool Load(const String &sFilename, EEncoding nEncoding = EncodingUnknown);

		/**
		*  @brief
		*    Loads from a given file
		*
		*  @param[in] cFile
		*    File to read from, must be opened and readable
		*  @param[in] nEncoding
		*    Encoding
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool Load(File &cFile, EEncoding nEncoding = EncodingUnknown);

		/**
		*  @brief
		*    Save a file using the current document value
		*
		*  @return
		*    Returns 'true' if successful, else 'false'
		*/
		inline bool Save();

		/**
		*  @brief
		*    Save a file using the given filename
		*
		*  @param[in] sFilename
		*    Filename
		*
		*  @return
		*    Returns 'true' if successful, else 'false'
		*
		*  @note
		*    - The document value is set to 'sFilename'
		*/
		PLCORE_API bool Save(const String &sFilename);

		/**
		*  @brief
		*    Get the root element -- the only top level element -- of the document
		*
		*  @return
		*    The root element, a null pointer on error
		*
		*  @note
		*    - In well formed XML, there should only be one. This parser is tolerant of
		*      multiple elements at the document level
		*/
		inline XmlElement *GetRootElement();
		inline const XmlElement *GetRootElement() const;

		/**
		*  @brief
		*    If an error occurs, error will be set to true
		*
		*  @return
		*    'true' if an error occurs, else 'false'
		*
		*  @note
		*    - 'GetErrorID()' will contain the integer identifier of the error (not generally useful)
		*    - 'GetErrorDesc()' will return the name of the error. (very useful)
		*    - 'GetErrorRow()' and 'GetErrorColumn()' will return the location of the error (if known)
		*/
		inline bool Error() const;

		/**
		*  @brief
		*    Contains a textual (english) description of the error if one occurs
		*
		*  @param[in] bLocation
		*    Do also add the location (if known) of the error?
		*
		*  @return
		*    Error description
		*/
		PLCORE_API String GetErrorDesc(bool bLocation = true) const;

		/**
		*  @brief
		*    Generally, you probably want the error string ('GetErrorDesc()') - but if you
		*    prefer the error ID, this function will fetch it
		*
		*  @return
		*    Error ID
		*/
		inline int GetErrorID() const;

		/**
		*  @brief
		*    Returns the location (if known) of the error
		*
		*  @remarks
		*    The first column is column 1, and the first row is row 1. A value of 0 means
		*    the row and column wasn't applicable (memory errors, for example, have no
		*    row/column) or the parser lost the error. (An error in the error reporting,
		*    in that case.)
		*
		*  @return
		*    Row the error occurred
		*
		*  @see
		*    - SetTabSize(), GetRow() and GetColumn()
		*/
		inline int GetErrorRow() const;

		/**
		*  @brief
		*    The column where the error occurred
		*
		*  @return
		*    Column the error occurred
		*
		*  @see
		*    - GetErrorRow()
		*/
		inline int GetErrorColumn() const;

		/**
		*  @brief
		*    Returns the tab size
		*
		*  @return
		*    Tab size
		*/
		inline uint32 GetTabSize() const;

		/**
		*  @brief
		*    Sets the tab size
		*
		*  @param[in] nTabSize
		*    New tab size
		*
		*  @remarks
		*    By calling this method, with a tab size greater than 0, the row and column
		*    of each node and attribute is stored when the file is loaded. Very useful
		*    for tracking the DOM back in to the source file.
		*    The tab size is required for calculating the location of nodes. If not
		*    set, the default of 4 is used. The tab size is set per document. Setting
		*    the tab size to 0 disables row/column tracking.
		*    Note that row and column tracking is not supported when using operator '>>'
		*    The tab size needs to be enabled before the parse or load. Correct usage:
		*    @verbatim
		*    XmlDocument cDocument;
		*    cDocument.SetTabSize(8);
		*    cDocument.Load("myfile.xml");
		*    @endverbatim
		*
		*    @see
		*      - GetRow() and GetColumn()
		*/
		inline void SetTabSize(uint32 nTabSize = 4);

		/**
		*  @brief
		*    If you have handled the error, it can be reset with this call
		*
		*  @note
		*    - The error state is automatically cleared if you parse a new XML block
		*/
		PLCORE_API void ClearError();


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
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static const String sErrorString[ErrorStringCount];	/**< Human readable error messages */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets an error
		*
		*  @param[in] nError
		*    Error code
		*  @param[in] pszErrorLocation
		*    Error location
		*  @param[in] pData
		*    Data
		*  @param[in] nEncoding
		*    Encoding
		*/
		void SetError(int nError, const char *pszErrorLocation, XmlParsingData *pData, EEncoding nEncoding);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool   m_bError;			/**< Error detected? */
		int    m_nErrorID;			/**< Error ID */
		String m_sErrorDescription;	/**< Human readable error description */
		Cursor m_cErrorCursor;		/**< Error cursor */
		int    m_nTabSize;			/**< Tab size */
		bool   m_bUseMicrosoftBOM;	/**< The UTF-8 BOM were found when read. Note this, and try to write. */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlDocument.inl"


#endif // __PLCORE_XML_DOCUMENT_H__
