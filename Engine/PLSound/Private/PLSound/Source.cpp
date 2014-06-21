/*********************************************************\
 *  File: Source.cpp                                     *
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
Source::~Source()
{
	// Unload
	Unload();
}

/**
*  @brief
*    Returns the buffer the source is using
*/
Buffer *Source::GetBuffer() const
{
	return static_cast<Buffer*>(m_cBufferHandler.GetResource());
}

/**
*  @brief
*    Returns the source flags
*/
uint32 Source::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Sets the source flags
*/
void Source::SetFlags(uint32 nFlags)
{
	m_nFlags = nFlags;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Source::Source(SoundManager &cSoundManager) : Resource(cSoundManager, Resource::TypeSource),
	m_nFlags(0)
{
}


//[-------------------------------------------------------]
//[ Public virtual Source functions                       ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads the sound source with the given sound buffer
*/
bool Source::Load(Buffer *pBuffer)
{
	// Unload the old buffer
	Unload();

	// Add this source to the buffer
	if (pBuffer) {
		m_cBufferHandler.SetResource(pBuffer);
		pBuffer->m_lstSources.Add(this);
	}

	// Done
	return true;
}

/**
*  @brief
*    Unload the source
*/
void Source::Unload()
{
	// Is a buffer loaded?
	Buffer *pBuffer = static_cast<Buffer*>(m_cBufferHandler.GetResource());
	if (pBuffer) {
		// Remove this source from the buffer
		pBuffer->m_lstSources.Remove(this);
		m_cBufferHandler.SetResource();
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Source::Source(const Source &cSource) : Resource(cSource.GetSoundManager(), cSource.GetType()),
	m_nFlags(cSource.GetFlags())
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Source &Source::operator =(const Source &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound
