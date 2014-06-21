/*********************************************************\
 *  File: ResourceHandler.h                              *
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


#ifndef __PLCORE_RESOURCE_HANDLER_H__
#define __PLCORE_RESOURCE_HANDLER_H__
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
template <class AType> class Resource;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Resource handler template
*/
template <class AType> class ResourceHandler {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Resource<AType>;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		ResourceHandler();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ResourceHandler();

		/**
		*  @brief
		*    Returns a pointer to the handlers resource
		*
		*  @return
		*    A pointer to the handlers resource, a null pointer if no resource
		*/
		AType *GetResource() const;


	//[-------------------------------------------------------]
	//[ Public virtual ResourceHandler functions              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets the handlers resource
		*
		*  @param[in] pResource
		*    Resource to set, a null pointer to set no element
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If the resource manager should delete unused resources automatically,
		*      (see ResourceManager::GetUnloadUnused()) the previous resource will be
		*      destroyed within this function if the resource is no longer used by any
		*      handlers and if the resource is not marked as protected.
		*/
		virtual bool SetResource(AType *pResource = nullptr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AType *m_pResource; /**< Loaded resource, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/ResourceHandler.inl"


#endif // __PLCORE_RESOURCE_HANDLER_H__
