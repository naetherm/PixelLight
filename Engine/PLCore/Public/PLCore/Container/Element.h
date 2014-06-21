/*********************************************************\
 *  File: Element.h                                      *
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


#ifndef __PLCORE_ELEMENT_H__
#define __PLCORE_ELEMENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/Array.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class AType> class ElementHandler;
template <class AType> class ElementManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Element template
*/
template <class AType> class Element {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ElementManager<AType>;
	friend class ElementHandler<AType>;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] pManager
		*    Element manager using this element, can be a null pointer
		*/
		Element(const String &sName = "", ElementManager<AType> *pManager = nullptr);

		/**
		*  @brief
		*    Returns the element manager using this element
		*
		*  @return
		*    Element manager using this element or a null pointer
		*/
		ElementManager<AType> *GetManager() const;

		/**
		*  @brief
		*   Returns the unique element name
		*
		*  @return
		*    Unique element name
		*
		*  @note
		*    - Because within a element manager a element is managed using
		*      it's name, this element name MUST be unique!
		*/
		String GetName() const;

		/**
		*  @brief
		*    Returns whether the element is protected or not
		*
		*  @return
		*    'true' if the element is protected, else 'false'
		*
		*  @note
		*    - Protected element are only removed if their manager is cleared!
		*/
		bool IsProtected() const;

		/**
		*  @brief
		*    Sets whether the element is protected or not
		*
		*  @param[in] bProtected
		*    Is the element protected?
		*
		*  @see
		*    - IsProtected()
		*/
		void SetProtected(bool bProtected = false);

		/**
		*  @brief
		*    Returns the ID of the element within the resource manager
		*
		*  @return
		*    ID of the element within the element manager, -1 if there was an error
		*    (maybe this element is not within a element manager)
		*
		*  @note
		*    - The element ID isn't fixed, if you remove some elements from the manager
		*      it's possible that the element ID is changed!
		*/
		int GetID() const;

		//[-------------------------------------------------------]
		//[ Handler functions                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of element handlers
		*
		*  @return
		*    The number of element handlers
		*
		*  @note
		*    - The handlers will be informed if the element was lost
		*/
		uint32 GetNumOfHandlers() const;

		/**
		*  @brief
		*    Returns a element handler
		*
		*  @param[in] nIndex
		*    Index of the element handler to return
		*
		*  @return
		*    The requested element handler, a null pointer on error
		*/
		ElementHandler<AType> *GetHandler(uint32 nIndex) const;


	//[-------------------------------------------------------]
	//[ Public virtual Element functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destroys the element
		*
		*  @param[in] bProtectedToo
		*    Do also destroy the element if it is protected?
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the element is protected?)
		*
		*  @note
		*    - Whether the element is destroyed within this function or just added for
		*      instance to a kind of 'destruction list' is implementation dependent.
		*/
		virtual bool Delete(bool bProtectedToo = false);

		/**
		*  @brief
		*    Sets the element name
		*
		*  @param[in] sName
		*    New element name
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the name is already used)
		*
		*  @see
		*    - GetName()
		*
		*  @note
		*    - Overload this function if it's for instance not allowed to change the name of the element
		*/
		virtual bool SetName(const String &sName);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This instance
		*
		*  @note
		*    - Manager, handlers and name are not copied
		*/
		virtual Element<AType> &operator =(const Element<AType> &cSource);


	//[-------------------------------------------------------]
	//[ Private virtual Element functions                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Deletes the element
		*/
		virtual void DeleteElement();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Element();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		ElementManager<AType>			*m_pManager;	/**< Element manager, can be a null pointer */
		String							 m_sName;		/**< Resource name */
		bool							 m_bProtected;	/**< Is the element protected? */
		Array<ElementHandler<AType>*>	 m_lstHandlers;	/**< Element handler list */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/Element.inl"


#endif // __PLCORE_ELEMENT_H__
