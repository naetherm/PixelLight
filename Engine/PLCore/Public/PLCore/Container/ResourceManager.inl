/*********************************************************\
 *  File: ResourceManager.inl                            *
 *      Resource manager template implementation
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Resource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the name of the manager
*/
template <class AType>
String ResourceManager<AType>::GetManagerName() const
{
	return m_sManagerName;
}

/**
*  @brief
*    Sets the name of the manager
*/
template <class AType>
bool ResourceManager<AType>::SetManagerName(const String &sName)
{
	// Check parameter
	if (sName.GetLength()) {
		// Set new name
		m_sManagerName = sName;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Creates a new resource
*/
template <class AType>
AType *ResourceManager<AType>::Create(const String &sName)
{
	// Check whether this resource is already within the manager
	AType *pResource = GetByName(sName);
	if (pResource)
		return pResource;

	// Get name
	if (!sName.GetLength()) {
		// Find an unused resource name
		String sNewName = '0';
		for (int i=1; GetByName(sNewName); i++)
			sNewName = i;

		// Create and return the new resource
		return CreateResource(sNewName);
	}

	// Create and return the new resource
	return CreateResource(sName);
}

/**
*  @brief
*    Unload a resource
*/
template <class AType>
bool ResourceManager<AType>::Unload(AType &cResource)
{
	// Parameter check
	if (!cResource.IsProtected()) {
		// Is this a valid resource?
		const AType *pResource = m_mapResources.Get(cResource.GetName());
		if (pResource && &cResource == pResource) {
			// Emit event
			EventResourceRemoved(cResource);

			// Remove the resource
			if (m_pStandardResource == &cResource)
				m_pStandardResource = nullptr;
			m_mapResources.Remove(cResource.GetName());
			m_lstResources.Remove(&cResource);
			cResource.m_pManager = nullptr;

			// Destroy the resource
			cResource.DeleteResource();

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Unload unused resources
*/
template <class AType>
uint32 ResourceManager<AType>::UnloadUnused()
{
	uint32 nUnloaded = 0;

	// Unload unused resources
	for (uint32 i=0; i<m_lstResources.GetNumOfElements(); i++) {
		// Get resource
		AType *pResource = m_lstResources[i];

		// Remove resource if it isn't used and not protected
		if (!pResource->GetNumOfHandlers() && !pResource->IsProtected()) {
			Remove(*pResource);
			pResource->DeleteResource();
			nUnloaded++;
			i--;
		}
	}

	// Return the number of unloaded resources
	return nUnloaded;
}

/**
*  @brief
*    Returns whether unused resource are unloaded automatically or not
*/
template <class AType>
bool ResourceManager<AType>::GetUnloadUnused() const
{
	return m_bUnloadUnused;
}

/**
*  @brief
*    Sets whether unused resource are unloaded automatically or not
*/
template <class AType>
void ResourceManager<AType>::SetUnloadUnused(bool bUnloadUnused)
{
	m_bUnloadUnused = bUnloadUnused;
}

/**
*  @brief
*    Get the standard resource
*/
template <class AType>
AType *ResourceManager<AType>::GetStandard() const
{
	// Return standard resource
	return m_pStandardResource;
}

/**
*  @brief
*    Set the standard resource
*/
template <class AType>
bool ResourceManager<AType>::SetStandard(AType *pResource)
{
	// Is this a valid resource?
	if (pResource) {
		const AType *pFoundResource = m_mapResources.Get(pResource->GetName());
		if (pFoundResource && pResource == pFoundResource) {
			// If unused resources should be destroyed, check whether the previous standard
			// resource is now unused (if there's one :)
			if (m_bUnloadUnused && m_pStandardResource && !m_pStandardResource->GetNumOfHandlers())
				m_pStandardResource->DeleteResource(); // Kill this resource NOW
		} else {
			// Error!
			return false;
		}
	}

	// Set new standard resource
	m_pStandardResource = pResource;

	// Done
	return true;
}

/**
*  @brief
*    Returns the number of resources within the manager
*/
template <class AType>
uint32 ResourceManager<AType>::GetNumOfElements() const
{
	return m_lstResources.GetNumOfElements();
}


//[-------------------------------------------------------]
//[ Public virtual ResourceManager functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clear the manager, delete all resources
*/
template <class AType>
bool ResourceManager<AType>::Clear(bool bProtectedToo)
{
	// Clear
	if (bProtectedToo) {
		while (m_lstResources.GetNumOfElements()) {
			AType *pResource = m_lstResources[0];
			pResource->DeleteResource();
		}
	} else {
		for (uint32 i=0; i<m_lstResources.GetNumOfElements(); i++) {
			AType *pResource = m_lstResources[i];
			if (!pResource->IsProtected()) {
				pResource->DeleteResource();
				i--;
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Loads a resource
*/
template <class AType>
AType *ResourceManager<AType>::LoadResource(const String &sFilename)
{
	// IS there already a resource with this name?
	AType *pResource = GetByName(sFilename);
	if (pResource)
		return pResource;

	// Create a new resource
	pResource = Create(sFilename);
	if (pResource) {
		// Load the resource
		if (pResource->LoadByFilename(sFilename))
			return pResource; // Return the new resource
		else
			delete pResource;
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Initializes the manager
*/
template <class AType>
bool ResourceManager<AType>::Init()
{
	// Standard implementation, please overwrite in derived class
	return true; // Done
}

/**
*  @brief
*    De-initializes the manager
*/
template <class AType>
bool ResourceManager<AType>::DeInit()
{
	// Standard implementation, please overwrite in derived class
	Clear();
	return true; // Done
}

/**
*  @brief
*    Updates the manager
*/
template <class AType>
bool ResourceManager<AType>::Update()
{
	// Standard implementation, please overwrite in derived class
	return true; // Done
}

/**
*  @brief
*    Copy operator
*/
template <class AType>
ResourceManager<AType> &ResourceManager<AType>::operator =(const ResourceManager<AType> &cSource)
{
	// Setup manager
	SetManagerName(cSource.GetManagerName());
	Clear();

	// Copy resources
	for (uint32 i=0; i<cSource.GetNumOfElements(); i++) {
		const AType *pSourceResource = cSource.GetByIndex(i);
		AType *pResource = Create(static_cast<const Resource<AType>*>(pSourceResource)->GetName());
		*pResource = *pSourceResource;
	}

	// Return pointer
	return *this;
}

/**
*  @brief
*    Returns the resource at the given index
*/
template <class AType>
AType *ResourceManager<AType>::GetByIndex(uint32 nIndex) const
{
	return m_lstResources[nIndex];
}

/**
*  @brief
*    Returns the index of the given resource
*/
template <class AType>
int ResourceManager<AType>::GetIndex(AType &cResource) const
{
	return m_lstResources.GetIndex(&cResource);
}

/**
*  @brief
*    Moves a resource within the resource list
*/
template <class AType>
void ResourceManager<AType>::MoveResource(uint32 nFromIndex, uint32 nToIndex)
{
	m_lstResources.MoveElement(nFromIndex, nToIndex);
}

/**
*  @brief
*    Returns the resource with the given name
*/
template <class AType>
AType *ResourceManager<AType>::GetByName(const String &sName) const
{
	// Search for an object with that name
	return m_mapResources.Get(sName);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class AType>
ResourceManager<AType>::ResourceManager() :
	m_pStandardResource(nullptr),
	m_bUnloadUnused(false)
{
}

/**
*  @brief
*    Destructor
*/
template <class AType>
ResourceManager<AType>::~ResourceManager()
{
	// Clear manager
	Clear();
}

/**
*  @brief
*    Sets the unique resource name
*/
template <class AType>
bool ResourceManager<AType>::SetResourceName(AType &cResource, const String &sName)
{
	// Set new name?
	if (cResource.m_sName != sName) {
		// Check if the name is already used
		if (GetByName(sName))
			return false; // Error!

		// Remove the resource from the name hash list
		bool bManagerElement = false;
		if (cResource.GetManager())
			bManagerElement = m_mapResources.Remove(cResource.m_sName);

		// Set new name
		if (sName.GetLength())
			cResource.m_sName = sName;
		else {
			// Find an unused resource name
			String sNewName = '0';
			for (int i=1; GetByName(sNewName); i++)
				sNewName = i;

			// Set this name
			cResource.m_sName = sNewName;
		}

		// Update the resource manager name hash list
		if (bManagerElement)
			m_mapResources.Add(cResource.m_sName, &cResource);
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a resource
*/
template <class AType>
bool ResourceManager<AType>::Add(AType &cResource)
{
	// Check parameter
	if (cResource.GetManager() && cResource.GetManager() != this)
		return false; // Error!

	// Is this resource already within a manager?
	if (!cResource.GetManager()) {
		// Check resource name, managed resources MUST have valid names
		if (cResource.GetName().GetLength()) {
			// Check whether there's already a resource with this name
			if (m_mapResources.Get(cResource.GetName()))
				return false; // Error!
		} else {
			// Find an unused resource name
			String sNewName = '0';
			for (int i=1; GetByName(sNewName); i++)
				sNewName = i;

			// Set this name
			cResource.SetName(sNewName);
		}

		// Add resource to list
		if (!m_lstResources.Add(&cResource))
			return false; // Error!
		cResource.m_pManager = this;

		// Add resource to hash lists
		m_mapResources.Add(cResource.GetName(), &cResource);
	}

	// Done
	return true;
}

/**
*  @brief
*    Removes a resource
*/
template <class AType> 
bool ResourceManager<AType>::Remove(AType &cResource)
{
	// Is this a valid resource?
	const AType *pResource = m_mapResources.Get(cResource.GetName());
	if (pResource && &cResource == pResource) {
		// Emit event
		EventResourceRemoved(cResource);

		// Remove the resource
		if (m_pStandardResource == &cResource)
			m_pStandardResource = nullptr;
		m_mapResources.Remove(cResource.GetName());
		m_lstResources.Remove(&cResource);
		cResource.m_pManager = nullptr;

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
