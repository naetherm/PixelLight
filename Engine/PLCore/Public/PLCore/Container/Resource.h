/*********************************************************\
 *  File: Resource.h                                     *
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


#ifndef __PLCORE_RESOURCE_H__
#define __PLCORE_RESOURCE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Array.h"
#include "PLCore/Tools/Loadable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class AType> class ResourceHandler;
template <class AType> class ResourceManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Resource template
*/
template <class AType> class Resource : public Loadable {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ResourceManager<AType>;
	friend class ResourceHandler<AType>;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Resource name to set
		*  @param[in] pManager
		*    Resource manager using this resource, can be a null pointer
		*/
		Resource(const String &sName = "", ResourceManager<AType> *pManager = nullptr);

		/**
		*  @brief
		*    Returns the resource manager using this resource
		*
		*  @return
		*    Resource manager using this resource or a null pointer
		*/
		ResourceManager<AType> *GetManager() const;

		/**
		*  @brief
		*   Returns the unique resource name
		*
		*  @return
		*    Unique resource name
		*
		*  @note
		*    - Because within a resource manager a resource is managed using
		*      it's name, this resource name MUST be unique!
		*/
		String GetName() const;

		/**
		*  @brief
		*    Returns whether the resource is protected or not
		*
		*  @return
		*    'true' if the resource is protected, else 'false'
		*
		*  @note
		*    - Protected resource are only removed if their manager is cleared!
		*/
		bool IsProtected() const;

		/**
		*  @brief
		*    Sets whether the resource is protected or not
		*
		*  @param[in] bProtected
		*    Is the resource protected?
		*
		*  @see
		*    - IsProtected()
		*/
		void SetProtected(bool bProtected = false);

		/**
		*  @brief
		*    Returns the ID of the resource within the resource manager
		*
		*  @return
		*    ID of the resource within the resource manager, -1 if there was an error
		*    (maybe this resource is not within a resource manager)
		*
		*  @note
		*    - The resource ID isn't fixed, if you remove some resources from the manager
		*      it's possible that the resource ID is changed!
		*/
		int GetID() const;

		//[-------------------------------------------------------]
		//[ Handler functions                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of resource handlers
		*
		*  @return
		*    The number of resource handlers
		*
		*  @note
		*    - The handlers will be informed if the resource was lost
		*/
		uint32 GetNumOfHandlers() const;

		/**
		*  @brief
		*    Returns a resource handler
		*
		*  @param[in] nIndex
		*    Index of the resource handler to return
		*
		*  @return
		*    The requested resource handler, a null pointer on error
		*/
		ResourceHandler<AType> *GetHandler(uint32 nIndex) const;


	//[-------------------------------------------------------]
	//[ Public virtual Resource functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destroys the resource
		*
		*  @param[in] bProtectedToo
		*    Do also destroy the resource if it is protected?
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the resource is protected?)
		*
		*  @note
		*    - Whether the resource is destroyed within this function or just added for
		*      instance to a kind of 'destruction list' is implementation dependent.
		*/
		virtual bool Delete(bool bProtectedToo = false);

		/**
		*  @brief
		*    Sets the resource name
		*
		*  @param[in] sName
		*    New resource name
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the name is already used)
		*
		*  @see
		*    - GetName()
		*
		*  @note
		*    - Overload this function if it's for instance not allowed to change the name of the resource
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
		virtual Resource<AType> &operator =(const Resource<AType> &cSource);


	//[-------------------------------------------------------]
	//[ Private virtual Resource functions                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Deletes the resource
		*/
		virtual void DeleteResource();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Resource();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		ResourceManager<AType>			*m_pManager;		/**< Resource manager, can be a null pointer */
		String							 m_sName;			/**< Resource name */
		bool							 m_bProtected;		/**< Is the resource protected? */
		Array<ResourceHandler<AType>*>	 m_lstHandlers;		/**< Resource handler list */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/Resource.inl"


#endif // __PLCORE_RESOURCE_H__
