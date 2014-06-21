/*********************************************************\
 *  File: ElementManager.inl                             *
 *      Element manager template implementation
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
#include "PLCore/Container/Element.h"


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
String ElementManager<AType>::GetManagerName() const
{
	return m_sManagerName;
}

/**
*  @brief
*    Sets the name of the manager
*/
template <class AType>
bool ElementManager<AType>::SetManagerName(const String &sName)
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
*    Creates a new element
*/
template <class AType>
AType *ElementManager<AType>::Create(const String &sName)
{
	// Check whether this element is already within the manager
	AType *pElement = GetByName(sName);
	if (pElement)
		return pElement;

	// Get name
	if (!sName.GetLength()) {
		// Find an unused element name
		String sNewName = "0";
		for (int i=1; GetByName(sNewName); i++)
			sNewName = i;

		// Create and return the new element
		return CreateElement(sNewName);
	}

	// Create and return the new element
	return CreateElement(sName);
}

/**
*  @brief
*    Adds a element
*/
template <class AType>
bool ElementManager<AType>::Add(AType &cElement)
{
	// Check parameter
	if (cElement.GetManager() && cElement.GetManager() != this)
		return false; // Error!

	// Is the element already within a manager?
	if (!cElement.GetManager()) {
		// Check element name, managed elements MUST have valid names
		if (cElement.GetName().GetLength()) {
			// Check whether there's already a element with this name
			if (m_mapElements.Get(cElement.GetName()))
				return false; // Error!
		} else {
			// Find an unused element name
			String sNewName = "0";
			for (int i=1; GetByName(sNewName); i++)
				sNewName = i;

			// Set this name
			cElement.SetName(sNewName);
		}

		// Add element to list
		if (!m_lstElements.Add(&cElement))
			return false; // Error!
		cElement.m_pManager = this;

		// Add element to hash lists
		m_mapElements.Add(cElement.GetName(), &cElement);
	}

	// Done
	return true;
}

/**
*  @brief
*    Unload a element
*/
template <class AType>
bool ElementManager<AType>::Unload(AType &cElement)
{
	// Parameter check
	if (!cElement.IsProtected()) {
		// Is this a valid element?
		const AType *pElement = m_mapElements.Get(cElement.GetName());
		if (pElement && &cElement == pElement) {
			// Remove the element
			if (m_pStandardElement == &cElement)
				m_pStandardElement = nullptr;
			m_mapElements.Remove(cElement.GetName());
			m_lstElements.Remove(&cElement);
			cElement.m_pManager = nullptr;

			// Destroy the element
			cElement.DeleteElement();

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Removes a element
*/
template <class AType> 
bool ElementManager<AType>::Remove(AType &cElement)
{
	// Is this a valid element?
	const AType *pElement = m_mapElements.Get(cElement.GetName());
	if (pElement && &cElement == pElement) {
		// Remove the element
		if (m_pStandardElement == &cElement)
			m_pStandardElement = nullptr;
		m_mapElements.Remove(cElement.GetName());
		m_lstElements.Remove(&cElement);
		cElement.m_pManager = nullptr;

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Unload unused elements
*/
template <class AType>
uint32 ElementManager<AType>::UnloadUnused()
{
	uint32 nUnloaded = 0;

	// Unload unused elements
	for (uint32 i=0; i<m_lstElements.GetNumOfElements(); i++) {
		// Get element
		AType *pElement = m_lstElements[i];

		// Remove element if it isn't used and not protected
		if (!pElement->GetNumOfHandlers() && !pElement->IsProtected()) {
			Remove(pElement);
			pElement->DeleteElement();
			nUnloaded++;
			i--;
		}
	}

	// Return the number of unloaded elements
	return nUnloaded;
}

/**
*  @brief
*    Returns whether unused element are unloaded automatically or not
*/
template <class AType>
bool ElementManager<AType>::GetUnloadUnused() const
{
	return m_bUnloadUnused;
}

/**
*  @brief
*    Sets whether unused element are unloaded automatically or not
*/
template <class AType>
void ElementManager<AType>::SetUnloadUnused(bool bUnloadUnused)
{
	m_bUnloadUnused = bUnloadUnused;
}

/**
*  @brief
*    Get the standard element
*/
template <class AType>
AType *ElementManager<AType>::GetStandard() const
{
	// Return standard element
	return m_pStandardElement;
}

/**
*  @brief
*    Set the standard element
*/
template <class AType>
bool ElementManager<AType>::SetStandard(AType *pElement)
{
	// Is this a valid element?
	if (pElement) {
		const AType *pFoundElement = m_mapElements.Get(pElement->GetName());
		if (pFoundElement && pElement == pFoundElement) {
			// If unused elements should be destroyed, check whether the previous standard
			// element is now unused (if there's one :)
			if (m_bUnloadUnused && m_pStandardElement && !m_pStandardElement->GetNumOfHandlers())
				m_pStandardElement->DeleteElement(); // Kill this element NOW
		} else {
			// Error!
			return false;
		}
	}

	// Set new standard element
	m_pStandardElement = pElement;

	// Done
	return true;
}

/**
*  @brief
*    Returns the number of elements within the manager
*/
template <class AType>
uint32 ElementManager<AType>::GetNumOfElements() const
{
	return m_lstElements.GetNumOfElements();
}


//[-------------------------------------------------------]
//[ Public virtual ElementHandler functions               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clear the manager, delete all elements
*/
template <class AType>
bool ElementManager<AType>::Clear(bool bProtectedToo)
{
	// Clear
	if (bProtectedToo) {
		while (m_lstElements.GetNumOfElements()) {
			AType *pElement = m_lstElements[0];
			pElement->DeleteElement();
		}
	} else {
		for (uint32 i=0; i<m_lstElements.GetNumOfElements(); i++) {
			AType *pElement = m_lstElements[i];
			if (!pElement->IsProtected()) {
				pElement->DeleteElement();
				i--;
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Initializes the manager
*/
template <class AType>
bool ElementManager<AType>::Init()
{
	// Standard implementation, please overwrite in derived class
	return true; // Done
}

/**
*  @brief
*    De-initializes the manager
*/
template <class AType>
bool ElementManager<AType>::DeInit()
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
bool ElementManager<AType>::Update()
{
	// Standard implementation, please overwrite in derived class
	return true; // Done
}

/**
*  @brief
*    Copy operator
*/
template <class AType>
ElementManager<AType> &ElementManager<AType>::operator =(const ElementManager<AType> &cSource)
{
	// Setup manager
	SetManagerName(cSource.GetManagerName());
	Clear();

	// Copy elements
	for (uint32 i=0; i<cSource.GetNumOfElements(); i++) {
		const AType *pSourceElement = cSource.GetByIndex(i);
		AType *pElement = Create(static_cast<const Element<AType>*>(pSourceElement)->GetName());
		*pElement = *pSourceElement;
	}

	// Return pointer
	return *this;
}

/**
*  @brief
*    Returns the element at the given index
*/
template <class AType>
AType *ElementManager<AType>::GetByIndex(uint32 nIndex) const
{
	return m_lstElements[nIndex];
}

/**
*  @brief
*    Returns the index of the given element
*/
template <class AType>
int ElementManager<AType>::GetIndex(AType &cElement) const
{
	return m_lstElements.GetIndex(&cElement);
}

/**
*  @brief
*    Moves an element within the element list
*/
template <class AType>
void ElementManager<AType>::MoveElement(uint32 nFromIndex, uint32 nToIndex)
{
	m_lstElements.MoveElement(nFromIndex, nToIndex);
}

/**
*  @brief
*    Returns the element with the given name
*/
template <class AType>
AType *ElementManager<AType>::GetByName(const String &sName) const
{
	// Search for an object with that name
	return m_mapElements.Get(sName);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class AType>
ElementManager<AType>::ElementManager() :
	m_pStandardElement(nullptr),
	m_bUnloadUnused(false)
{
}

/**
*  @brief
*    Destructor
*/
template <class AType>
ElementManager<AType>::~ElementManager()
{
	// Clear manager
	Clear();
}

/**
*  @brief
*    Sets the unique element name
*/
template <class AType>
bool ElementManager<AType>::SetElementName(AType &cElement, const String &sName)
{
	// Set new name?
	if (cElement.m_sName != sName) {
		// Check if the name is already used
		if (GetByName(sName))
			return false; // Error!

		// Remove the element from the name hash list
		bool bManagerElement = false;
		if (cElement.GetManager())
			bManagerElement = m_mapElements.Remove(cElement.m_sName);

		// Set new name
		if (sName.GetLength()) {
			cElement.m_sName = sName;
		} else {
			// Find an unused element name
			String sNewName = "0";
			for (int i=1; GetByName(sNewName); i++)
				sNewName = i;

			// Set this name
			cElement.m_sName = sNewName;
		}

		// Update the element manager name hash list
		if (bManagerElement)
			m_mapElements.Add(cElement.m_sName, &cElement);
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
