/*********************************************************\
 *  File: Resource.cpp                                   *
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
#include "PLRenderer/Renderer/ResourceHandler.h"
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/Resource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Resource::~Resource()
{
	// Remove resource from renderer
	m_pRenderer->RemoveResource(*this);
	ClearHandlers();
}

/**
*  @brief
*    Adds a resource handler
*/
bool Resource::AddHandler(ResourceHandler &cHandler)
{
	// Is this handler already registered?
	if (!m_lstHandlers.IsElement(&cHandler)) {
		if (m_lstHandlers.Add(&cHandler)) {
			if (cHandler.GetResource() != this)
				cHandler.SetResource(this);
		} else {
			// Error!
			return false;
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Removes a resource handler
*/
bool Resource::RemoveHandler(ResourceHandler &cHandler)
{
	if (m_lstHandlers.Remove(&cHandler)) {
		cHandler.SetResource();

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Clears the list of handlers
*/
void Resource::ClearHandlers()
{
	while (m_lstHandlers.GetNumOfElements())
		RemoveHandler(*m_lstHandlers[0]);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Resource::Resource(Renderer &cRenderer, EType nType) :
	m_pRenderer(&cRenderer),
	m_nType(nType)
{
	// Add resource to renderer
	m_pRenderer->AddResource(*this);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Resource::Resource(const Resource &cSource) :
	m_pRenderer(&cSource.GetRenderer()),
	m_nType(cSource.GetType())
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Resource &Resource::operator =(const Resource &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Protected virtual Resource functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Backups the resource device data
*/
void Resource::BackupDeviceData(uint8 **ppBackup)
{
	// Nothing to do by default
}

/**
*  @brief
*    Restores the resource device data
*/
void Resource::RestoreDeviceData(uint8 **ppBackup)
{
	// Nothing to do by default
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
