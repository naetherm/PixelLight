/*********************************************************\
 *  File: Buffer.cpp                                     *
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
#include "PLSound/Source.h"
#include "PLSound/SoundManager.h"
#include "PLSound/Buffer.h"


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
Buffer::~Buffer()
{
	// Unload all sources using this buffer
	while (m_lstSources.GetNumOfElements())
		m_lstSources[0]->Unload();
}

/**
*  @brief
*    Returns the number of sources
*/
uint32 Buffer::GetNumOfSources() const
{
	return m_lstSources.GetNumOfElements();
}

/**
*  @brief
*    Returns a sound source
*/
const Source *Buffer::GetSource(uint32 nIndex) const
{
	return m_lstSources[nIndex];
}

/**
*  @brief
*    Returns a sound source
*/
Source *Buffer::GetSource(uint32 nIndex)
{
	return m_lstSources[nIndex];
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*   Constructor
*/
Buffer::Buffer(SoundManager &cSoundManager, const String &sName) :
	PLSound::Resource(cSoundManager, TypeBuffer), PLCore::Resource<Buffer>(sName, &cSoundManager)
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Buffer::Buffer(const Buffer &cSource) :
	PLSound::Resource(cSource.GetSoundManager(), cSource.GetType()), PLCore::Resource<Buffer>(cSource.GetName(), &cSource.GetSoundManager())
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound
