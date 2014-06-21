/*********************************************************\
 *  File: ElementHandler.inl                             *
 *      Element handler template implementation
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
#include "PLCore/Container/ElementManager.h"


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
ElementHandler<AType>::ElementHandler() :
	m_pElement(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
template <class AType>
ElementHandler<AType>::~ElementHandler()
{
	SetElement();
}

/**
*  @brief
*    Returns a pointer to the handlers element
*/
template <class AType>
AType *ElementHandler<AType>::GetElement() const
{
	return m_pElement;
}

/**
*  @brief
*    Sets the handlers element
*/
template <class AType>
bool ElementHandler<AType>::SetElement(AType *pElement)
{
	// Check whether this is the current set source
	if (m_pElement != pElement) {
		// If currently a element is set...
		if (m_pElement) {
			// Remove this handler from the old element
			if (!m_pElement->m_lstHandlers.Remove(this))
				return false; // Error!

			// If unused elements should be destroyed, check whether the previous element
			// is now unused and not protected
			if (!m_pElement->IsProtected() && !m_pElement->m_lstHandlers.GetNumOfElements()) {
				ElementManager<AType> *pManager = m_pElement->GetManager();
				if (pManager && pManager->GetUnloadUnused() && pManager->GetStandard() != m_pElement)
					m_pElement->Delete(); // Kill this element
			}
		}

		// Set new element
		m_pElement = pElement;

		// Add this handler to the new element
		if (m_pElement && !m_pElement->m_lstHandlers.Add(this))
			return false; // Error!
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
