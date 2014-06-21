/*********************************************************\
 *  File: XmlElement.h                                   *
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


#ifndef __PLCORE_XML_ELEMENT_H__
#define __PLCORE_XML_ELEMENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlNode.h"
#include "PLCore/Xml/XmlAttribute.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    The XML element is a container class
*
*  @remarks
*    An element has a value, the element name, and can contain other elements, text,
*    comments, and unknowns. Elements also contain an arbitrary number of attributes.
*/
class XmlElement : public XmlNode {


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
		*    Constructor
		*
		*  @param[in] sValue
		*    Value of this element (value = element tag name)
		*/
		PLCORE_API XmlElement(const String &sValue);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLCORE_API XmlElement(const XmlElement &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~XmlElement();

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
		PLCORE_API XmlElement &operator =(const XmlElement &cSource);

		/**
		*  @brief
		*    Given an attribute name, 'GetAttribute()' returns the value
		*    for the attribute of that name, or empty string if none exists
		*
		*  @param[in] sName
		*    Attribute name
		*
		*  @return
		*    Attribute value, empty on error
		*/
		PLCORE_API String GetAttribute(const String &sName) const;

		/**
		*  @brief
		*    Given an attribute name, 'GetAttribute()' returns the value
		*    for the attribute of that name, or empty string if none exists
		*
		*  @param[in]  sName
		*    Attribute name
		*  @param[out] pnValue
		*    If not a null pointer, this variable will receive the integer value - if the attribute
		*    value can be converted into an integer, if not, this variable is not touched
		*
		*  @return
		*    Attribute value, empty on error
		*
		*  @remarks
		*    If the attribute exists and can be converted to an integer,
		*    the integer value will be put in the return 'pnValue', if 'pnValue'
		*    is not a null pointer.
		*/
		PLCORE_API String GetAttribute(const String &sName, int *pnValue) const;

		/**
		*  @brief
		*    Given an attribute name, 'GetAttribute()' returns the value
		*    for the attribute of that name, or empty string if none exists
		*
		*  @param[in]  sName
		*    Attribute name
		*  @param[out] pdValue
		*    If not a null pointer, this variable will receive the double value - if the attribute
		*    value can be converted into a double, if not, this variable is not touched
		*
		*  @return
		*    Attribute value, empty on error
		*
		*  @remarks
		*    If the attribute exists and can be converted to an double,
		*    the double value will be put in the return 'pdValue', if 'pdValue'
		*    is not a null pointer.
		*/
		PLCORE_API String GetAttribute(const String &sName, double *pdValue) const;

		/**
		*  @brief
		*    Examines the attribute
		*
		*  @param[in]  sName
		*    Attribute name
		*  @param[out] pnValue
		*    If not a null pointer, this variable will receive the integer value - if the attribute
		*    value can be converted into an integer, if not, this variable is not touched
		*
		*  @return
		*    'Success' if all went fine, 'WrongType' if value is not an integer and
		*    'NoAttribute' if the attribute does not exist
		*
		*  @note
		*     - This function is an alternative to the 'GetAttribute()' function with richer
		*       error checking.
		*/
		PLCORE_API EQueryResult QueryIntAttribute(const String &sName, int *pnValue) const;

		/**
		*  @brief
		*    Examines the attribute
		*
		*  @param[in]  sName
		*    Attribute name
		*  @param[out] pfValue
		*    If not a null pointer, this variable will receive the float value - if the attribute
		*    value can be converted into a float, if not, this variable is not touched
		*
		*  @return
		*    'Success' if all went fine, 'WrongType' if value is not a float and
		*    'NoAttribute' if the attribute does not exist
		*
		*  @see
		*    - QueryIntAttribute()
		*/
		PLCORE_API EQueryResult QueryFloatAttribute(const String &sName, float *pfValue) const;

		/**
		*  @brief
		*    Examines the attribute
		*
		*  @param[in]  sName
		*    Attribute name
		*  @param[out] pdValue
		*    If not a null pointer, this variable will receive the double value - if the attribute
		*    value can be converted into a double, if not, this variable is not touched
		*
		*  @return
		*    'Success' if all went fine, 'WrongType' if value is not a double and
		*    'NoAttribute' if the attribute does not exist
		*
		*  @see
		*    - QueryIntAttribute()
		*/
		PLCORE_API EQueryResult QueryDoubleAttribute(const String &sName, double *pdValue) const;

		/**
		*  @brief
		*    Sets an attribute of name to a given value
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] sValue
		*    Attribute value
		*
		*  @remarks
		*    The attribute will be created if it does not exist, or changed if it does.
		*/
		PLCORE_API void SetAttribute(const String &sName, const String &sValue);

		/**
		*  @brief
		*    Sets an attribute of name to a given value
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] nValue
		*    Attribute value
		*
		*  @remarks
		*    The attribute will be created if it does not exist, or changed if it does.
		*/
		PLCORE_API void SetAttribute(const String &sName, int nValue);

		/**
		*  @brief
		*    Sets an attribute of name to a given value
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] dValue
		*    Attribute value
		*
		*  @remarks
		*    The attribute will be created if it does not exist, or changed if it does.
		*/
		PLCORE_API void SetDoubleAttribute(const String &sName, double dValue);

		/**
		*  @brief
		*    Deletes an attribute with the given name
		*
		*  @param[in] sName
		*    Name of the attribute to remove
		*/
		PLCORE_API void RemoveAttribute(const String &sName);

		/**
		*  @brief
		*    Access the first attribute in this element
		*
		*  @return
		*    The first attribute, a null pointer if there's no first attribute
		*/
		inline XmlAttribute *GetFirstAttribute();
		inline const XmlAttribute *GetFirstAttribute() const;

		/**
		*  @brief
		*    Access the last attribute in this element
		*
		*  @return
		*    The last attribute, a null pointer if there's no last attribute
		*/
		inline XmlAttribute *GetLastAttribute();
		inline const XmlAttribute *GetLastAttribute() const;

		/**
		*  @brief
		*    Convenience function for easy access to the text inside an element
		*
		*  @remarks
		*    Although easy and concise, GetText() is limited compared to getting the
		*    XmlText child and accessing it directly.
		*
		*    If the first child of 'this' is a XmlText, the GetText()
		*    returns the character string of the text node, else null is returned.
		*
		*    This is a convenient method for getting the text of simple contained text:
		*    @verbatim
		*    <foo>This is text</foo>
		*    const char *pszText = fooElement->GetText();
		*    @endverbatim
		*
		*    'pszText' will be a pointer to "This is text".
		*
		*    Note that this function can be misleading. If the element foo was created from
		*    this XML:
		*    @verbatim
		*    <foo><b>This is text</b></foo>
		*    @endverbatim
		*
		*    then the value of str would be null. The first child node isn't a text node, it is
		*    another element. From this XML:
		*    @verbatim
		*    <foo>This is <b>text</b></foo>
		*    @endverbatim
		*    GetText() will return "This is ".
		*
		*    WARNING: GetText() accesses a child node - don't become confused with the
		*             similarly named XmlNode::ToText() which is safe type casts on the referenced node.
		*
		*  @return
		*    Text inside an element, or empty string
		*/
		PLCORE_API String GetText() const;


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
	//[ Private classes                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    A internal class used to manage a group of attributes
		*/
		class XmlAttributeSet {
		public:
			XmlAttributeSet();
			~XmlAttributeSet();

			void Add(XmlAttribute &cAttribute);
			void Remove(XmlAttribute &cAttribute);

			const XmlAttribute *GetFirst()	const	{ return ( cSentinel.m_pNextAttribute     == &cSentinel ) ? 0 : cSentinel.m_pNextAttribute; }
			XmlAttribute *GetFirst()				{ return ( cSentinel.m_pNextAttribute     == &cSentinel ) ? 0 : cSentinel.m_pNextAttribute; }
			const XmlAttribute *GetLast() const		{ return ( cSentinel.m_pPreviousAttribute == &cSentinel ) ? 0 : cSentinel.m_pPreviousAttribute; }
			XmlAttribute *GetLast()					{ return ( cSentinel.m_pPreviousAttribute == &cSentinel ) ? 0 : cSentinel.m_pPreviousAttribute; }

			XmlAttribute *Find(const String &sName) const;
			XmlAttribute *FindOrCreate(const String &sName);

		private:
			XmlAttributeSet(const XmlAttributeSet &cSource) { /** Not implemented */ };
			void operator =(const XmlAttributeSet &cSource) { /** Not implemented */ };

			XmlAttribute cSentinel;
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Like clear, but initializes 'this' object as well
		*/
		void ClearThis();

		/**
		*  @brief
		*    Reads the "value" of the element -- another element, or text
		*
		*  @param[in] pszData
		*    Parsing data, if a null pointer, an error will be returned
		*  @param[in] pData
		*    Parsing data, can be a null pointer
		*  @param[in] nEncoding
		*    Encoding
		*
		*  @return
		*    Read value
		*
		*  @note
		*    - This should terminate with the current end tag
		*/
		const char *ReadValue(const char *pszData, XmlParsingData *pData, EEncoding nEncoding);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		XmlAttributeSet m_cAttributeSet;	/**< Attribute set */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlElement.inl"


#endif // __PLCORE_XML_ELEMENT_H__
