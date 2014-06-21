/*********************************************************\
 *  File: XmlNode.h                                      *
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


#ifndef __PLCORE_XML_NODE_H__
#define __PLCORE_XML_NODE_H__
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
class XmlText;
class XmlUnknown;
class XmlComment;
class XmlElement;
class XmlDocument;
class XmlDeclaration;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract XML node
*/
class XmlNode : public XmlBase {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    The supported types of XML nodes. (All the
		*    unsupported types are picked up by 'Unknown'.)
		*/
		enum ENodeType {
			Document,		/**< Document */
			Element,		/**< Element */
			Comment,		/**< Comment */
			Unknown,		/**< Unknown */
			Text,			/**< Text */
			Declaration,	/**< Declaration */
			NumOfTypes		/**< Number of types */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~XmlNode();

		/**
		*  @brief
		*    The meaning of 'value' changes for the specific type of 'XmlNode'
		*
		*  @return
		*    Node value
		*
		*  @remarks
		*    @verbatim
		*    Document:  Filename of the XML file
		*    Element:   Name of the element
		*    Comment:   The comment text
		*    Unknown:   The tag contents
		*    Text:      The text string
		*    @endverbatim
		*    The subclasses will wrap this function.
		*/
		inline String GetValue() const;

		/**
		*  @brief
		*    Changes the value of the node
		*
		*  @param[in] sValue
		*    New node value
		*
		*  @see
		*    - GetValue()
		*/
		inline void SetValue(const String &sValue);

		/**
		*  @brief
		*    Delete all the children of this node
		*
		*  @note
		*    - Does not affect 'this'
		*/
		PLCORE_API void Clear();

		/**
		*  @brief
		*    One step up the DOM
		*
		*  @return
		*    The parent node, a null pointer on error
		*/
		inline XmlNode *GetParent();
		inline const XmlNode *GetParent() const;

		/**
		*  @brief
		*    Returns the first child of this node
		*
		*  @return
		*    The first child of this node, a null pointer if there are no children
		*/
		inline XmlNode *GetFirstChild();
		inline const XmlNode *GetFirstChild() const;

		/**
		*  @brief
		*    Returns the first child of this node with the matching 'value'
		*
		*  @param[in] sValue
		*    Value to look for
		*
		*  @return
		*    The first child of this node with the matching 'value', a null pointer if none found
		*/
		PLCORE_API XmlNode *GetFirstChild(const String &sValue);
		PLCORE_API const XmlNode *GetFirstChild(const String &sValue) const;

		/**
		*  @brief
		*    Returns the last child of this node
		*
		*  @return
		*    The last child of this node, a null pointer if there are no children
		*/
		inline XmlNode *GetLastChild();
		inline const XmlNode *GetLastChild() const;

		/**
		*  @brief
		*    Returns the last child of this node with the matching 'value'
		*
		*  @param[in] sValue
		*    Value to look for
		*
		*  @return
		*    The last child of this node with the matching 'value', a null pointer if none found
		*/
		PLCORE_API XmlNode *GetLastChild(const String &sValue);
		PLCORE_API const XmlNode *GetLastChild(const String &sValue) const;

		/**
		*  @brief
		*    An alternate way to walk the children of a node
		*
		*  @param[in] pPrevious
		*    Previous node, if a null pointer the first child node is returned
		*
		*  @return
		*    Next node, a null pointer if there's no next node
		*
		*  @remarks
		*    One way to iterate over nodes is:
		*    @verbatim
		*    for (XmlNode *pChild=pParent->GetFirstChild(); pChild; pChild=Child->GetNextSibling())
		*    @endverbatim
		*
		*    'IterateChildren()' does the same thing with the syntax:
		*    @verbatim
		*    XmlNode *pChild = pParent->IterateChildren(pChild);
		*    while (pChild)
		*    @endverbatim
		*
		*    'IterateChildren()' takes the previous child as input and finds
		*    the next one. If the previous child is a null pointer, it returns the
		*    first. 'IterateChildren()' will return a null pointer when done.
		*/
		inline XmlNode *IterateChildren(XmlNode *pPrevious);
		inline const XmlNode *IterateChildren(const XmlNode *pPrevious) const;

		/**
		*  @brief
		*    This flavor of 'IterateChildren()' searches for children with a particular 'value'
		*
		*  @param[in] sValue
		*    Value to look for
		*  @param[in] pPrevious
		*    Previous node, if a null pointer the first child node is returned
		*
		*  @return
		*    Next node, a null pointer if there's no next node
		*/
		inline XmlNode *IterateChildren(const String &sValue, XmlNode *pPrevious);
		inline const XmlNode *IterateChildren(const String &sValue, const XmlNode *pPrevious) const;

		/**
		*  @brief
		*    Add a new node related to this (adds a child past the last child)
		*
		*  @param[in] cAddThis
		*    Node to insert
		*
		*  @return
		*    Returns a pointer to the new object or a null pointer if an error occurred
		*/
		PLCORE_API XmlNode *InsertEndChild(const XmlNode &cAddThis);

		/**
		*  @brief
		*    Add a new node related to this (adds a child past the last child)
		*
		*  @param[in] cAddThis
		*    Node to add
		*
		*  @return
		*    Pointer to the given 'cAddThis', a null pointer on error (on error, 'cAddThis' will be deleted)
		*
		*  @remarks
		*    The node to be added is passed by reference, and will be henceforth owned
		*    (and deleted) by the XML implementation. This method is efficient and avoids
		*    an extra copy, but should be used with care as it uses a different memory
		*    model than the other insert functions.
		*/
		PLCORE_API XmlNode *LinkEndChild(XmlNode &cAddThis);

		/**
		*  @brief
		*    Add a new node related to this (adds a child before the specified child)
		*
		*  @param[in] cBeforeThis
		*    Node before this insertion
		*  @param[in] cAddThis
		*    Node to insert
		*
		*  @return
		*    Returns a pointer to the new object or a null pointer if an error occurred
		*/
		PLCORE_API XmlNode *InsertBeforeChild(XmlNode &cBeforeThis, const XmlNode &cAddThis);

		/**
		*  @brief
		*    Add a new node related to this (adds a child after the specified child)
		*
		*  @param[in] cAfterThis
		*    Node after this insertion
		*  @param[in] cAddThis
		*    Node to insert
		*
		*  @return
		*    Returns a pointer to the new object or a null pointer if an error occurred
		*/
		PLCORE_API XmlNode *InsertAfterChild(XmlNode &cAfterThis, const XmlNode &cAddThis);

		/**
		*  @brief
		*    Replace a child of this node
		*
		*  @param[in] cReplaceThis
		*    Node which should be replaced (on success, destroyed automatically)
		*  @param[in] cWithThis
		*    New node replacing the old
		*
		*  @return
		*    Returns a pointer to the new object or a null pointer if an error occurred
		*/
		PLCORE_API XmlNode *ReplaceChild(XmlNode &cReplaceThis, const XmlNode &cWithThis);

		/**
		*  @brief
		*    Delete a child of this node
		*
		*  @param[in] cRemoveThis
		*    Node to remove (on success, destroyed automatically)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool RemoveChild(XmlNode &cRemoveThis);

		/**
		*  @brief
		*    Navigate to a sibling node
		*
		*  @return
		*    The sibling node, a null pointer if there's no sibling node
		*/
		inline XmlNode *GetPreviousSibling();
		inline const XmlNode *GetPreviousSibling() const;

		/**
		*  @brief
		*    Navigate to a sibling node with the given 'value'
		*
		*  @param[in] sValue
		*    Value to look for
		*
		*  @return
		*    The sibling node, a null pointer if there's no sibling node
		*/
		PLCORE_API XmlNode *GetPreviousSibling(const String &sValue);
		PLCORE_API const XmlNode *GetPreviousSibling(const String &sValue) const;

		/**
		*  @brief
		*    Navigate to a sibling node
		*
		*  @return
		*    The sibling node, a null pointer if there's no sibling node
		*/
		inline XmlNode *GetNextSibling();
		inline const XmlNode *GetNextSibling() const;

		/**
		*  @brief
		*    Navigate to a sibling node with the given 'value'
		*
		*  @param[in] sValue
		*    Value to look for
		*
		*  @return
		*    The sibling node, a null pointer if there's no sibling node
		*/
		PLCORE_API XmlNode *GetNextSibling(const String &sValue);
		PLCORE_API const XmlNode *GetNextSibling(const String &sValue) const;

		/**
		*  @brief
		*    Convenience function to get through elements
		*
		*  @remarks
		*    Calls 'NextSibling()' and 'ToElement()'. Will skip all non-element nodes.
		*
		*  @return
		*    Returns a null pointer if there is not another element
		*/
		inline XmlElement *GetNextSiblingElement();
		PLCORE_API const XmlElement *GetNextSiblingElement() const;

		/**
		*  @brief
		*    Convenience function to get through elements
		*
		*  @param[in] sValue
		*    Value to look for
		*
		*  @remarks
		*    Calls 'NextSibling()' and 'ToElement()'. Will skip all non-element nodes.
		*
		*  @return
		*    Returns a null pointer if there is not another element
		*/
		PLCORE_API XmlElement *GetNextSiblingElement(const String &sValue);
		PLCORE_API const XmlElement *GetNextSiblingElement(const String &sValue) const;

		/**
		*  @brief
		*    Convenience function to get through elements
		*
		*  @return
		*    Returns a null pointer if there is not another element
		*/
		inline XmlElement *GetFirstChildElement();
		PLCORE_API const XmlElement *GetFirstChildElement() const;

		/**
		*  @brief
		*    Convenience function to get through elements
		*
		*  @param[in] sValue
		*    Value to look for
		*
		*  @return
		*    Returns a null pointer if there is not another element
		*/
		inline XmlElement *GetFirstChildElement(const String &sValue);
		PLCORE_API const XmlElement *GetFirstChildElement(const String &sValue) const;

		/**
		*  @brief
		*    Query the type (as an enumerated value, above) of this node
		*
		*  @return
		*    Type of this node
		*/
		inline ENodeType GetType() const;

		/**
		*  @brief
		*    Return a pointer to the document this node lives in
		*
		*  @return
		*    Pointer to the document this node lives in, a null pointer if not in a document
		*/
		inline XmlDocument *GetDocument();
		PLCORE_API const XmlDocument *GetDocument() const;

		/**
		*  @brief
		*    Returns true if this node has no children
		*
		*  @return
		*    'true' if this node has no children, else 'false'
		*/
		inline bool NoChildren() const;

		/**
		*  @brief
		*    Cast functions, will return a null pointer if the given node is not from the requested type
		*/
		inline XmlDocument *ToDocument();
		inline const XmlDocument *ToDocument() const;
		inline XmlElement *ToElement();
		inline const XmlElement *ToElement() const;
		inline XmlComment *ToComment();
		inline const XmlComment *ToComment() const;
		inline XmlUnknown *ToUnknown();
		inline const XmlUnknown *ToUnknown() const;
		inline XmlText *ToText();
		inline const XmlText *ToText() const;
		inline XmlDeclaration *ToDeclaration();
		inline const XmlDeclaration *ToDeclaration() const;


	//[-------------------------------------------------------]
	//[ Public virtual XmlNode functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create an exact duplicate of this node and return it
		*
		*  @return
		*    The clone, a null pointer on error
		*
		*  @note
		*    - The memory must be deleted by the caller
		*/
		virtual XmlNode *Clone() const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nType
		*    Type of the node
		*/
		XmlNode(ENodeType nType);

		/**
		*  @brief
		*    Figure out what is at *pszData, and parse it
		*
		*  @param[in] pszData
		*    Start position
		*  @param[in] nEncoding
		*    Encoding
		*
		*  @return
		*    Returns a null pointer if it is not an XML node, else it returns a created node instance (you're responsible for it's destruction)
		*/
		XmlNode *Identify(const char *pszData, EEncoding nEncoding);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		XmlNode   *m_pParent;			/**< Parent node, can be a null pointer */
		ENodeType  m_nType;				/**< Type of the node */
		XmlNode   *m_pFirstChild;		/**< First child, can be a null pointer */
		XmlNode   *m_pLastChild;		/**< Last child, can be a null pointer */
		String	   m_sValue;			/**< Value */
		XmlNode   *m_pPreviousSibling;	/**< Previous sibling, can be a null pointer */
		XmlNode   *m_pNextSibling;		/**< Previous sibling, can be a null pointer */


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
		XmlNode(const XmlNode &cSource);

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
		XmlNode &operator =(const XmlNode &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Xml/XmlNode.inl"


#endif // __PLCORE_XML_NODE_H__
