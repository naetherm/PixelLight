/*********************************************************\
 *  File: ResourceManager.h                              *
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


#ifndef __PLCORE_RESOURCE_MANAGER_H__
#define __PLCORE_RESOURCE_MANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/Array.h"
#include "PLCore/Container/HashMap.h"
#include "PLCore/Base/Event/Event.h"


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
*    Abstract resource manager template
*/
template <class AType> class ResourceManager {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Resource<AType>;


	//[-------------------------------------------------------]
	//[ Events                                                ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<AType&> EventResourceRemoved;	/**< Resource removed event, removed resource as parameter */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the name of the manager
		*
		*  @return
		*    Name of the manager
		*
		*  @note
		*    - The manager name has no special usage, it's just for completeness
		*/
		String GetManagerName() const;

		/**
		*  @brief
		*    Sets the name of the manager
		*
		*  @param[in] sName
		*    New manager name
		*
		*  @return
		*    'true' if all went fine else 'false'
		*
		*  @see
		*    - GetManagerName()
		*/
		bool SetManagerName(const String &sName = "");

		/**
		*  @brief
		*   Clear the manager, delete all resources
		*
		*  @param[in] bProtectedToo
		*    Delete protected resources, too?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Clear(bool bProtectedToo = true);

		/**
		*  @brief
		*    Creates a new resource
		*
		*  @param[in] sName
		*    Resource name, if "" an unused name is set automatically
		*
		*  @return
		*    Pointer to the created resource, a null pointer if there was an error
		*
		*  @note
		*    - If there's already a resource with this name, this resource is returned
		*/
		AType *Create(const String &sName = "");

		/**
		*  @brief
		*    Unload a resource
		*
		*  @param[in] cResource
		*    Resource which should be unloaded
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Protected resources are NOT unloaded!
		*/
		bool Unload(AType &cResource);

		/**
		*  @brief
		*    Unload unused resources
		*
		*  @return
		*    Number of unloaded resources
		*
		*  @see
		*    - Unload()
		*/
		uint32 UnloadUnused();

		/**
		*  @brief
		*    Returns whether unused resource are unloaded automatically or not
		*
		*  @return
		*    'true' if unused resources are unloaded automatically, else 'false'
		*/
		bool GetUnloadUnused() const;

		/**
		*  @brief
		*    Sets whether unused resource are unloaded automatically or not
		*
		*  @param[in] bUnloadUnused
		*    Unload unused resources automatically?
		*/
		void SetUnloadUnused(bool bUnloadUnused = false);

		/**
		*  @brief
		*    Get the standard resource
		*
		*  @return
		*    The standard resource, a null pointer if there's no such resource
		*
		*  @note
		*    - There should always be a standard resource!
		*    - It is recommended that you protect your standard resource
		*/
		AType *GetStandard() const;

		/**
		*  @brief
		*    Set the standard resource
		*
		*  @param[in] pResource
		*    Resource which should be the standard resource, a null pointer to set no such resource
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid resource)
		*
		*  @see
		*    - GetStandard()
		*/
		bool SetStandard(AType *pResource = nullptr);

		/**
		*  @brief
		*    Returns the number of resources within the manager
		*
		*  @return
		*    Number of resources within the manager
		*/
		uint32 GetNumOfElements() const;


	//[-------------------------------------------------------]
	//[ Public virtual ResourceManager functions              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Loads a resource
		*
		*  @param[in] sFilename
		*    Filename of the resource to load
		*
		*  @return
		*    Pointer to the loaded resource, a null pointer on error (maybe the resource can't be load)
		*
		*  @remarks
		*    The resource name is set to the given filename. If there's already a resource with this
		*    (file)name, this resource is returned instead creating a new one. This function is virtual to
		*    enable derived managers to add some more features like automatic resource creation instead of
		*    loading a resource. In this case, 'filenames' normally begin with 'Create ' to indicate automatic
		*    resource creation.
		*/
		virtual AType *LoadResource(const String &sFilename);

		/**
		*  @brief
		*    Initializes the manager
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Init();

		/**
		*  @brief
		*    De-initializes the manager
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool DeInit();

		/**
		*  @brief
		*    Updates the manager
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Update();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This instance
		*/
		virtual ResourceManager<AType> &operator =(const ResourceManager<AType> &cSource);

		/**
		*  @brief
		*    Returns the resource at the given index
		*
		*  @param[in] nIndex
		*    Index of the resource
		*
		*  @return
		*    The corresponding object, a null pointer if there's no match
		*/
		virtual AType *GetByIndex(uint32 nIndex = 0) const;

		/**
		*  @brief
		*    Returns the index of the given resource
		*
		*  @param[in] cResource
		*    The resource for which the index should be retrieved
		*
		*  @return
		*    The index position, <0 on failure (e.g. the resource is no member of this manager)
		*/
		virtual int GetIndex(AType &cResource) const;

		/**
		*  @brief
		*    Moves a resource within the resource list
		*
		*  @param[in] nFromIndex
		*    The index of the resource which should be moved
		*  @param[in] nToIndex
		*    The index to which the resource should be moved
		*
		*  @note
		*    - This methods assumes, that both index values are within 0 and GetNumOfElements()-1
		*/
		virtual void MoveResource(uint32 nFromIndex, uint32 nToIndex);

		/**
		*  @brief
		*    Returns the resource with the given name
		*
		*  @param[in] sName
		*    Resource name
		*
		*  @return
		*    The corresponding resource, a null pointer if there's no match
		*
		*  @note
		*    - You can overload this function to specialize the behavior. This is done
		*      for instance within SceneContainer to be able to use 'absolute names'
		*      like 'Root.MyScene.MyNode', too.
		*/
		virtual AType *GetByName(const String &sName) const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		ResourceManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ResourceManager();

		/**
		*  @brief
		*    Sets the resource name
		*
		*  @param[in] cResource
		*    Resource to set the name
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
		*    - You can overload this function if there are 'reserved' names which are NOT
		*      allowed to be used or if special characters are not allowed
		*/
		bool SetResourceName(AType &cResource, const String &sName);


	//[-------------------------------------------------------]
	//[ Protected virtual ResourceManager functions           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates a new resource
		*
		*  @param[in] sName
		*    Resource name
		*
		*  @return
		*    Pointer to the created resource, a null pointer if there was an error
		*
		*  @note
		*    - This function is used inside the function Create(). If AType is an abstract
		*      class you have to overwrite this function to create an instance of the class.
		*/
		virtual AType *CreateResource(const String &sName = "") = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		String					 m_sManagerName;		/**< Manager name */
		AType					*m_pStandardResource;	/**< Standard resource, can be a null pointer */
		bool					 m_bUnloadUnused;		/**< Unload unused resources? */
		Array<AType*>			 m_lstResources;		/**< Resource list */
		HashMap<String, AType*>	 m_mapResources;		/**< Resource map */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Adds a resource
		*
		*  @param[in] cResource
		*    Resource to add
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe resource is already within another manager)
		*/
		bool Add(AType &cResource);

		/**
		*  @brief
		*    Removes a resource
		*
		*  @param[in] cResource
		*    Resource to remove
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe resource is not within the manager)
		*
		*  @note
		*    - This function will only remove the resource from the manager, but
		*      NOT deleting it!
		*/
		bool Remove(AType &cResource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/ResourceManager.inl"


#endif // __PLCORE_RESOURCE_MANAGER_H__
