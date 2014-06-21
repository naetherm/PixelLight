/*********************************************************\
 *  File: IOStream.cpp                                   *
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
#include <PLCore/File/File.h>
#include "PLAssimp/IOStream.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
IOStream::~IOStream()
{
	// Are we responsible for the file object?
	if (m_bTakeOver)
		delete m_pFile;	// Destroy the file object (this also automatically closes the file)
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
IOStream::IOStream(File &cFile, bool bTakeOver) :
	m_pFile(&cFile),
	m_bTakeOver(bTakeOver)
{
}


//[-------------------------------------------------------]
//[ Public virtual Assimp::IOStream functions             ]
//[-------------------------------------------------------]
size_t IOStream::Read(void *pvBuffer, size_t pSize, size_t pCount)
{
	return m_pFile->Read(pvBuffer, pSize, pCount);
}

size_t IOStream::Write(const void *pvBuffer, size_t pSize, size_t pCount)
{
	return m_pFile->Write(pvBuffer, pSize, pCount);
}

aiReturn IOStream::Seek(size_t pOffset, aiOrigin pOrigin)
{
	File::ESeek nLocation = File::SeekSet;
	switch (pOrigin) {
		case aiOrigin_SET:
			nLocation = File::SeekSet;
			break;

		case aiOrigin_CUR:
			nLocation = File::SeekCurrent;
			break;

		case aiOrigin_END:
			nLocation = File::SeekEnd;
			break;
	}
	return m_pFile->Seek(pOffset, nLocation) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

size_t IOStream::Tell() const
{
	return m_pFile->Tell();
}

size_t IOStream::FileSize() const
{
	return m_pFile->GetSize();
}

void IOStream::Flush()
{
	m_pFile->Flush();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
IOStream::IOStream() :
	m_pFile(nullptr),
	m_bTakeOver(false)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy constructor
*/
IOStream::IOStream(const IOStream &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
IOStream &IOStream::operator =(const IOStream &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
