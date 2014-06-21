/*********************************************************\
 *  File: Element.inl                                    *
 *      Element template implementation
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
#include "PLCore/Container/ElementHandler.h"
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
Element<AType>::Element(const String &sName, ElementManager<AType> *pManager) :
	m_pManager(nullptr),
	m_bProtected(false)
{
	// Set unique element name
	if (pManager)
		pManager->SetElementName(static_cast<AType&>(*this), sName);
	else
		m_sName = sName;

	// Add this element to the manager
	if (pManager)
		pManager->Add(static_cast<AType&>(*this));
}

/**
*  @brief
*    Returns the element manager using this element
*/
template <class AType>
ElementManager<AType> *Element<AType>::GetManager() const
{
	return m_pManager;
}

/**
*  @brief
*    Returns the unique element name
*/
template <class AType>
String Element<AType>::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Returns whether the element is protected or not
*/
template <class AType>
bool Element<AType>::IsProtected() const
{
	return m_bProtected;
}

/**
*  @brief
*    Sets whether the element is protected or not
*/
template <class AType>
void Element<AType>::SetProtected(bool bProtected)
{
	m_bProtected = bProtected;
}

/**
*  @brief
*    Returns the ID of the element within the resource manager
*/
template <class AType>
int Element<AType>::GetID() const
{
	// Is this element managed?
	if (m_pManager) {
		// Try to find this element within the manager
		for (uint32 i=0; i<m_pManager->GetNumOfElements(); i++) {
			if (m_pManager->GetByIndex(i) == this)
				return i;
		}
	}

	// HHHhhmmmmnn, element not found within the manager (?!!)
	return -1;
}


//[-------------------------------------------------------]
//[ Handler functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of element handlers
*/
template <class AType>
uint32 Element<AType>::GetNumOfHandlers() const
{
	return m_lstHandlers.GetNumOfElements();
}

/**
*  @brief
*    Returns a element handler
*/
template <class AType>
ElementHandler<AType> *Element<AType>::GetHandler(uint32 nIndex) const
{
	return m_lstHandlers[nIndex];
}


//[-------------------------------------------------------]
//[ Public virtual Element functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destroys the element
*/
template <class AType>
bool Element<AType>::Delete(bool bProtectedToo)
{
	// Protected?
	if (bProtectedToo || !m_bProtected) {
		DeleteElement();

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Sets the unique element name
*/
template <class AType>
bool Element<AType>::SetName(const String &sName)
{
	if (m_pManager)
		return m_pManager->SetElementName(static_cast<AType&>(*this), sName);
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
Element<AType> &Element<AType>::operator =(const Element<AType> &cSource)
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
Element<AType>::~Element()
{
	// Inform the handlers
	for (uint32 i=0; i<m_lstHandlers.GetNumOfElements(); i++)
		m_lstHandlers[i]->m_pElement = nullptr;

	// Clear the list
	m_lstHandlers.Clear();

	// Remove from element manager
	if (m_pManager)
		m_pManager->Remove(static_cast<AType&>(*this));
}


//[-------------------------------------------------------]
//[ Private virtual Element functions                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Deletes a element
*/
template <class AType>
void Element<AType>::DeleteElement()
{
	// Delete element
	delete this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
