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
#include "PLSound/SoundManager.h"
#include "PLSound/ResourceHandler.h"
#include "PLSound/Resource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLSound {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Resource::~Resource()
{
	// Remove resource from sound manager
	m_pSoundManager->RemoveResource(*this);

	// Inform all handlers using this resource
	ClearResourceHandlers();
}

/**
*  @brief
*    Returns the owner sound manager
*/
SoundManager &Resource::GetSoundManager() const
{
	return *m_pSoundManager;
}

/**
*  @brief
*    Returns the resource type
*/
Resource::EType Resource::GetType() const
{
	return m_nType;
}

/**
*  @brief
*    Returns the number of resource handlers
*/
uint32 Resource::GetNumOfResourceHandlers() const
{
	return m_lstSoundHandlers.GetNumOfElements();
}

/**
*  @brief
*    Returns whether a resource handler is using this resource
*/
bool Resource::IsResourceHandler(ResourceHandler &cHandler) const
{
	return m_lstSoundHandlers.IsElement(&cHandler);
}

/**
*  @brief
*    Adds a resource handler
*/
bool Resource::AddResourceHandler(ResourceHandler &cHandler)
{
	// Was this handler already added?
	if (!m_lstSoundHandlers.IsElement(&cHandler)) {
		// Add the handler
		if (m_lstSoundHandlers.Add(&cHandler))
			return false; // Error!
		else {
			if (cHandler.GetResource() != this)
				cHandler.SetResource(this);
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Removes a resource handler
*/
bool Resource::RemoveResourceHandler(ResourceHandler &cHandler)
{
	if (m_lstSoundHandlers.Remove(&cHandler)) {
		cHandler.SetResource(nullptr);

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
void Resource::ClearResourceHandlers()
{
	while (m_lstSoundHandlers.GetNumOfElements())
		RemoveResourceHandler(*m_lstSoundHandlers[0]);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Resource::Resource(SoundManager &cSoundManager, EType nType) :
	m_pSoundManager(&cSoundManager),
	m_nType(nType)
{
	// Add resource to sound manager
	m_pSoundManager->AddResource(*this);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Resource::Resource(const Resource &cSource) :
	m_pSoundManager(&cSource.GetSoundManager()),
	m_nType(cSource.GetType())
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound
