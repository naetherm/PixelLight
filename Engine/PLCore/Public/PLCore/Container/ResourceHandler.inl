/*********************************************************\
 *  File: ResourceHandler.inl                            *
 *      Resource handler template implementation
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
ResourceHandler<AType>::ResourceHandler() :
	m_pResource(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
template <class AType>
ResourceHandler<AType>::~ResourceHandler()
{
	SetResource();
}

/**
*  @brief
*    Returns a pointer to the handlers resource
*/
template <class AType>
AType *ResourceHandler<AType>::GetResource() const
{
	return m_pResource;
}


//[-------------------------------------------------------]
//[ Public virtual ResourceHandler functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the handlers resource
*/
template <class AType>
bool ResourceHandler<AType>::SetResource(AType *pResource)
{
	// Check whether this is the current set source
	if (m_pResource != pResource) {
		// If currently a resource is set...
		if (m_pResource) {
			// Remove this handler from the old resource
			if (!m_pResource->m_lstHandlers.Remove(this))
				return false; // Error!

			// If unused resources should be destroyed, check whether the previous resource
			// is now unused and not protected
			if (!m_pResource->IsProtected() && !m_pResource->m_lstHandlers.GetNumOfElements()) {
				ResourceManager<AType> *pManager = m_pResource->GetManager();
				if (pManager && pManager->GetUnloadUnused() && pManager->GetStandard() != m_pResource)
					m_pResource->Delete(); // Kill this resource
			}
		}

		// Set new resource
		m_pResource = pResource;

		// Add this handler to the new resource
		if (m_pResource && !m_pResource->m_lstHandlers.Add(this))
			return false; // Error!
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
