/*********************************************************\
 *  File: SoundManager.cpp                               *
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
#include "PLSound/Resource.h"
#include "PLSound/SoundManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLSound {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SoundManager)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
SoundManager::~SoundManager()
{
	// Destroy all resources - the resources unregister automatically from the list, so, this while loop will terminate!
	while (m_lstResources.GetNumOfElements())
		delete m_lstResources[0];
}

/**
*  @brief
*    Returns the sound statistics
*/
const SoundManager::Statistics &SoundManager::GetStatistics() const
{
	return m_sStatistics;
}


//[-------------------------------------------------------]
//[ Resources                                             ]
//[-------------------------------------------------------]
uint32 SoundManager::GetNumOfResources() const
{
	return m_lstResources.GetNumOfElements();
}

Resource *SoundManager::GetResource(uint32 nIndex) const
{
	return m_lstResources[nIndex];
}

bool SoundManager::AddResource(Resource &cResource)
{
	// Add the resource
	if (m_lstResources.Add(&cResource)) {
		// Update statistics
		switch (cResource.GetType()) {
			case Resource::TypeBuffer:
				m_sStatistics.nNumOfBuffers++;
				break;

			case Resource::TypeSource:
				m_sStatistics.nNumOfSources++;
				break;
		}

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

bool SoundManager::RemoveResource(Resource &cResource)
{
	// Remove the resource
	if (m_lstResources.Remove(&cResource)) {
		// Update statistics
		switch (cResource.GetType()) {
			case Resource::TypeBuffer:
				m_sStatistics.nNumOfBuffers--;
				break;

			case Resource::TypeSource:
				m_sStatistics.nNumOfSources--;
				break;
		}

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SoundManager::SoundManager()
{
	// Init data
	MemoryManager::Set(&m_sStatistics, 0, sizeof(Statistics));
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
SoundManager::SoundManager(const SoundManager &cSource)
{
	// Init data
	MemoryManager::Set(&m_sStatistics, 0, sizeof(Statistics));

	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
SoundManager &SoundManager::operator =(const SoundManager &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ResourceManager function      ]
//[-------------------------------------------------------]
Buffer *SoundManager::CreateResource(const String &sName)
{
	// This function MUST be overwritten within the concrete sound backend
	return nullptr; // Error!
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound
