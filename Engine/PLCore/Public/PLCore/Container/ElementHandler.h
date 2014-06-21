/*********************************************************\
 *  File: ElementHandler.h                               *
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


#ifndef __PLCORE_ELEMENT_HANDLER_H__
#define __PLCORE_ELEMENT_HANDLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class AType> class Element;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Element handler template
*/
template <class AType> class ElementHandler {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Element<AType>;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		ElementHandler();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ElementHandler();

		/**
		*  @brief
		*    Returns a pointer to the handlers element
		*
		*  @return
		*    A pointer to the handlers element, a null pointer if no element
		*/
		AType *GetElement() const;


	//[-------------------------------------------------------]
	//[ Public virtual ElementHandler functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets the handlers element
		*
		*  @param[in] pElement
		*    Element to set, a null pointer to set no element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If the element manager should delete unused elements automatically,
		*      (see ElementManager::GetUnloadUnused()) the previous element will be
		*      destroyed within this function if the element is no longer used by any
		*      handlers and if the element is not marked as protected.
		*/
		virtual bool SetElement(AType *pElement = nullptr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AType *m_pElement; /**< Loaded element, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/ElementHandler.inl"


#endif // __PLCORE_ELEMENT_HANDLER_H__
