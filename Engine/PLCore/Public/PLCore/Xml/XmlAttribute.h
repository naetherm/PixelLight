/*********************************************************\
 *  File: XmlAttribute.h                                 *
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


#ifndef __PLCORE_XML_ATTRIBUTE_H__
#define __PLCORE_XML_ATTRIBUTE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlBase.h"


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
*    XML element attribute
*
*  @remarks
*    An attribute is a name-value pair. Elements have an arbitrary
*    number of attributes, each with a unique name.
*/
class XmlAttribute : public XmlBase {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class XmlElement;
	friend class XmlAttributeSet;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API XmlAttribute();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~XmlAttribute();

		/**
		*  @brief
		*    Return the name of this attribute
		*
		*  @return
		*    Name of this attribute
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Set the name of this attribute
		*
		*  @param[in] sName
		*    Name of this attribute to set
		*/
		inline void SetName(const String &sName);

		/**
		*  @brief
		*    Return the value of this attribute
		*
		*  @return
		*    Value of this attribute
		*/
		inline String GetValue() const;

		/**
		*  @brief
		*    Return the value of this attribute, converted to an integer
		*
		*  @return
		*    Value of this attribute, converted to an integer
		*/
		inline int GetIntValue() const;

		/**
		*  @brief
		*    Return the value of this attribute, converted to a double
		*
		*  @return
		*    Value of this attribute, converted to a double
		*/
		inline double GetDoubleValue() const;

		/**
		*  @brief
		*    Examines the value string
		*
		*  @param[out] nValue
		*    Will receive the integer value if all went fine, if not, this variable is not touched
		*
		*  @return
		*    If the attribute value is not an integer, it returns 'WrongType'. Returns
		*    'Success' on success.
		*
		*  @remarks
		*    It is an alternative to the 'GetIntValue()'-function with richer error checking.
		*    If the value is an integer, it is stored in 'value' and the call returns
		*    'Success'. If it is not an integer, it returns 'WrongType'.
		*    A specialized but useful call. Note that for success it returns 'Success',
		*    which is the opposite of almost all other parser calls.
		*/
		PLCORE_API EQueryResult QueryIntValue(int &nValue) const;

		/**
		*  @brief
		*    Examines the value string
		*
		*  @param[out] dValue
		*    Will receive the double value if all went fine, if not, this variable is not touched
		*
		*  @return
		*    If the attribute value is not an integer, it returns 'WrongType'. Returns
		*    'Success' on success.
		*
		*  @see
		*    - QueryIntValue()
		*/
		PLCORE_API EQueryResult QueryDoubleValue(double &dValue) const;

		/**
		*  @brief
		*    Set the value
		*
		*  @param[in] sValue
		*    Attribute value to set
		*/
		inline void SetValue(const String &sValue);

		/**
		*  @brief
		*    Set the value from an integer
		*
		*  @param[in] nValue
		*    Attribute integer value to set
		*/
		inline void SetIntValue(int nValue);

		/**
		*  @brief
		*    Set the value from a double
		*
		*  @param[in] dValue
		*    Attribute double value to set
		*/
		inline void SetDoubleValue(double dValue);

		/**
		*  @brief
		*    Get the next sibling attribute in the DOM
		*
		*  @return
		*    The next sibling attribute in the DOM, a null pointer at end
		*/
		inline XmlAttribute *GetNext();
		inline const XmlAttribute *GetNext() const;

		/**
		*  @brief
		*    Get the previous sibling attribute in the DOM
		*
		*  @return
		*    The previous sibling attribute in the DOM, a null pointer at beginning
		*/
		inline XmlAttribute *GetPrevious();
		inline const XmlAttribute *GetPrevious() const;

		/**
		*  @brief
		*    Compare functions
		*
		*  @param[in] cOther
		*    Other attribute to compare with
		*
		*  @return
		*    'true' if the condition is fulfilled, else 'false'
		*/
		inline bool operator ==(const XmlAttribute &cOther) const;
		inline bool operator <(const XmlAttribute &cOther) const;
		inline bool operator >(const XmlAttribute &cOther) const;


	//[-------------------------------------------------------]
	//[ Public virtual XmlBase functions                      ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual bool Save(File &cFile, uint32 nDepth = 0) override;
		PLCORE_API virtual String ToString(uint32 nDepth = 0) const override;
		PLCORE_API virtual const char *Parse(const char *pszData, XmlParsingData *pData = nullptr, EEncoding nEncoding = EncodingUnknown) override;


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
		XmlAttribute(const XmlAttribute &cSource);

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
		XmlAttribute &operator =(const XmlAttribute &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		XmlDocument  *m_pDocument;			/**< A pointer back to a document, for error reporting, can be a null pointer */
		String		  m_sName;				/**< Name */
		String		  m_sValue;				/**< Value */
		XmlAttribute *m_pPreviousAttribute;	/**< Previous attribute, can be a null pointer */
		XmlAttribute *m_pNextAttribute;		/**< Next attribute, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlAttribute.inl"


#endif // __PLCORE_XML_ATTRIBUTE_H__
