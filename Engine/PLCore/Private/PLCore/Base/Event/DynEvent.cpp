/*********************************************************\
 *  File: DynEvent.cpp                                   *
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
#include "PLCore/Base/Func/DynParams.h"
#include "PLCore/Base/Event/DynEventHandler.h"
#include "PLCore/Base/Event/DynEvent.h"


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
DynEvent::DynEvent()
{
}

/**
*  @brief
*    Destructor
*/
DynEvent::~DynEvent()
{
	// Force disconnection of event handlers
	const SimpleList<DynEventHandler*>::ListElement *pElement = m_lstHandlers.pFirstElement;
	while (pElement) {
		pElement->Data->m_lstEvents.Remove(this);
		pElement = pElement->pNextElement;
	}
}

/**
*  @brief
*    Connect event handler to the event
*/
void DynEvent::Connect(DynEventHandler &cHandler)
{
	// The handler must not be connected already
	if (!cHandler.m_lstEvents.IsElement(this)) {
		// Signatures must match!
		if (cHandler.GetSignature() == GetSignature()) {
			// Connect handler
			m_lstHandlers.Add(&cHandler);
			cHandler.m_lstEvents.Add(this);
		}
	}
}

/**
*  @brief
*    Disconnect a handler from the event
*/
void DynEvent::Disconnect(DynEventHandler &cHandler)
{
	// Disconnect handler
	if (m_lstHandlers.IsElement(&cHandler)) {
		m_lstHandlers.Remove(&cHandler);
		cHandler.m_lstEvents.Remove(this);
	}
}


//[-------------------------------------------------------]
//[ Public virtual DynEvent functions                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get event descriptor
*/
const EventDesc *DynEvent::GetDesc() const
{
	// No descriptor by default
	return nullptr;
}

/**
*  @brief
*    Create a generic event handler which is compatible with this dynamic event
*/
DynEventHandler *DynEvent::CreateGenericEventHandler(const FUNC &pFunc, void *pUserData) const
{
	// No function
	return nullptr;
}

/**
*  @brief
*    Call function
*/
void DynEvent::Emit(DynParams &cParams) const
{
	// No function
}

/**
*  @brief
*    Call function
*/
void DynEvent::Emit(const DynParams &cParams) const
{
	// No function
}

/**
*  @brief
*    Emit event
*/
void DynEvent::Emit(const String &sParams) const
{
	// No function
}

/**
*  @brief
*    Emit event
*/
void DynEvent::Emit(const XmlElement &cElement) const
{
	// No function
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
