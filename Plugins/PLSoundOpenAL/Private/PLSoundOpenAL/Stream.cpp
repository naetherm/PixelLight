/*********************************************************\
 *  File: Stream.cpp                                     *
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
#include "PLSoundOpenAL/Stream.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Stream::Stream(ALuint nSource, const Buffer &cBuffer) :
	m_nSource(nSource),
	m_pBuffer(&cBuffer),
	m_bLooping(false)
{
}

/**
*  @brief
*    Destructor
*/
Stream::~Stream()
{
}

/**
*  @brief
*    Returns the sound source using this stream
*/
ALuint Stream::GetSource() const
{
	return m_nSource;
}

/**
*  @brief
*    Returns the sound buffer this stream is using
*/
const Buffer &Stream::GetBuffer() const
{
	return *m_pBuffer;
}

/**
*  @brief
*    Returns whether the stream is looping or not
*/
bool Stream::IsLooping() const
{
	return m_bLooping;
}

/**
*  @brief
*    Sets whether the stream is looping or not
*/
void Stream::SetLooping(bool bLooping)
{
	m_bLooping = bLooping;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL
