/*********************************************************\
 *  File: MemoryManager.inl                              *
 *      Memory manager inline implementation
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
//[ Debugging options for Microsoft Visual Studio         ]
//[-------------------------------------------------------]
// <<nostepinto>> PLCore::MemoryManager::.*
// <<nostepinto>> operator delete.*
// <<nostepinto>> operator new.*


// For performance reasons we include so called "standard headers" in here even if they are not standardized across all platforms.
// Usually the used headers don't introduce definition conflicts across the platforms, so, this should be fine... but it's still
// highly recommended to use this "MemoryManager" helper class in case there are issues on a new platform. Changing this one header
// is easier than changing all source codes of all projects.


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <stdlib.h>	// For "realloc()", "malloc()" and "free()"
#include <string.h>	// For "memcpy()", "memmove()", "memset()" and "memcmp()"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copies memory
*/
inline void MemoryManager::Copy(void *pDestination, const void *pSource, uint32 nNumOfBytes)
{
	memcpy(pDestination, pSource, nNumOfBytes);
}

/**
*  @brief
*    Moves memory
*/
inline void MemoryManager::Move(void *pDestination, const void *pSource, uint32 nNumOfBytes)
{
	memmove(pDestination, pSource, nNumOfBytes);
}

/**
*  @brief
*    Sets memory to a specified character
*/
inline void MemoryManager::Set(void *pDestination, int nCharacter, uint32 nNumOfBytes)
{
	memset(pDestination, nCharacter, nNumOfBytes);
}

/**
*  @brief
*    Compares memory
*/
inline int MemoryManager::Compare(const void *pFirstBuffer, const void *pSecondBuffer, uint32 nNumOfBytes)
{
	return memcmp(pFirstBuffer, pSecondBuffer, nNumOfBytes);
}

/**
*  @brief
*    Reallocates memory
*/
inline void *MemoryManager::Reallocator(void *pAddress, size_t nNumOfBytes, EType nType, const char *pszSourceFile, int nSourceLine)
{
	return realloc(pAddress, nNumOfBytes);
}

/**
*  @brief
*    Allocates new memory
*/
inline void *MemoryManager::Allocator(EType nType, size_t nNumOfBytes, const char *pszSourceFile, int nSourceLine)
{
	return malloc(nNumOfBytes);
}

/**
*  @brief
*    Frees memory
*/
inline void MemoryManager::Deallocator(EType nType, void *pAddress, const char *pszSourceFile, int nSourceLine)
{
	free(pAddress);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
