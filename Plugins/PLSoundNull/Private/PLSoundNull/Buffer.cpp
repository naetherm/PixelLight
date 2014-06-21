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
#include "PLSoundNull/SoundManager.h"
#include "PLSoundNull/Source.h"
#include "PLSoundNull/Buffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLSoundNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Buffer::~Buffer()
{
	// Unload sound
	Unload();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Buffer::Buffer(PLSound::SoundManager &cSoundManager, const String &sName, bool bStream) :
	PLSound::Buffer(cSoundManager, sName)
{
	// Load the sound buffer
	if (sName.GetLength())
		LoadBuffer(sName, bStream);
}


//[-------------------------------------------------------]
//[ Public virtual PLSound::Buffer functions              ]
//[-------------------------------------------------------]
bool Buffer::LoadBuffer(const String &sFilename, bool bStream)
{
	// Check parameter
	return (sFilename.GetLength() != 0);
}

bool Buffer::LoadBuffer(const uint8 nData[], uint32 nSize, bool bStream)
{
	// Check parameters
	return (nSize != 0);
}

bool Buffer::IsLoaded() const
{
	return false;
}

bool Buffer::IsStreamed() const
{
	return false;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Buffer::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	return LoadBuffer(sFilename, false);
}

bool Buffer::Unload()
{
	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundNull
