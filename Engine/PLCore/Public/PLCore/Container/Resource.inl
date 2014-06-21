/*********************************************************\
 *  File: Resource.inl                                   *
 *      Resource template implementation
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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/ResourceHandler.h"
#include "PLCore/Container/ResourceManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class AType>
Resource<AType>::Resource(const String &sName, ResourceManager<AType> *pManager) :
	m_pManager(nullptr),
	m_bProtected(false)
{
	// Set unique resource name
	if (pManager)
		pManager->SetResourceName(static_cast<AType&>(*this), sName);
	else
		m_sName = sName;

	// Add this resource to the manager
	if (pManager)
		pManager->Add(static_cast<AType&>(*this));
}

/**
*  @brief
*    Returns the resource manager using this resource
*/
template <class AType>
ResourceManager<AType> *Resource<AType>::GetManager() const
{
	return m_pManager;
}

/**
*  @brief
*    Returns the unique resource name
*/
template <class AType>
String Resource<AType>::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Returns whether the resource is protected or not
*/
template <class AType>
bool Resource<AType>::IsProtected() const
{
	return m_bProtected;
}

/**
*  @brief
*    Sets whether the resource is protected or not
*/
template <class AType>
void Resource<AType>::SetProtected(bool bProtected)
{
	m_bProtected = bProtected;
}

/**
*  @brief
*    Returns the ID of the resource within the resource manager
*/
template <class AType>
int Resource<AType>::GetID() const
{
	// Is this resource managed?
	if (m_pManager) {
		// Try to find this resource within the manager
		for (uint32 i=0; i<m_pManager->GetNumOfElements(); i++) {
			if (m_pManager->GetByIndex(i) == this)
				return i;
		}
	}

	// HHHhhmmmmnn, resource not found within the manager (?!!)
	return -1;
}


//[-------------------------------------------------------]
//[ Handler functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of resource handlers
*/
template <class AType>
uint32 Resource<AType>::GetNumOfHandlers() const
{
	return m_lstHandlers.GetNumOfElements();
}

/**
*  @brief
*    Returns a resource handler
*/
template <class AType>
ResourceHandler<AType> *Resource<AType>::GetHandler(uint32 nIndex) const
{
	return m_lstHandlers[nIndex];
}


//[-------------------------------------------------------]
//[ Public virtual Resource functions                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destroys the resource
*/
template <class AType>
bool Resource<AType>::Delete(bool bProtectedToo)
{
	// Protected?
	if (bProtectedToo || !m_bProtected) {
		DeleteResource();

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Sets the unique resource name
*/
template <class AType>
bool Resource<AType>::SetName(const String &sName)
{
	if (m_pManager)
		return m_pManager->SetResourceName(static_cast<AType&>(*this), sName);
	else {
		m_sName = sName;

		// Done
		return true;
	}
}

/**
*  @brief
*    Copy operator
*/
template <class AType>
Resource<AType> &Resource<AType>::operator =(const Resource<AType> &cSource)
{
	// Copy
	m_bProtected = cSource.m_bProtected;

	// Return pointer
	return *this;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
template <class AType>
Resource<AType>::~Resource()
{
	// Inform the handlers
	for (uint32 i=0; i<m_lstHandlers.GetNumOfElements(); i++)
		m_lstHandlers[i]->m_pResource = nullptr;

	// Clear the list
	m_lstHandlers.Clear();

	// Remove from resource manager
	if (m_pManager)
		m_pManager->Remove(static_cast<AType&>(*this));
}


//[-------------------------------------------------------]
//[ Private virtual Resource functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Deletes a resource
*/
template <class AType>
void Resource<AType>::DeleteResource()
{
	// Delete resource
	delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
