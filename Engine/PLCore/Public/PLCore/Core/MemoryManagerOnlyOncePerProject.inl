/*********************************************************\
 *  File: MemoryManagerOnlyOncePerProject.inl            *
 *      Memory manager inline implementation. This header can ONLY be includes once per project!
 *      (normally automatically included within 'PLCore/ModuleMain.h')
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
#include "PLCore/Core/MemoryManager.h"


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
inline void *operator new(size_t nNumOfBytes)
{
	// ANSI says: allocation requests of 0 bytes will still return a valid value
	if (!nNumOfBytes)
		nNumOfBytes = 1;

	// ANSI says: loop continuously because the error handler could possibly free up some memory
	for (;;) {
		// Try the allocation
		void *pAddress = PLCore::MemoryManager::Allocator(PLCore::MemoryManager::New, nNumOfBytes, "?", 0);
		if (pAddress)
			return pAddress;
	}
}

inline void *operator new[](size_t nNumOfBytes)
{
	// The ANSI standard says that allocation requests of 0 bytes will still return a valid value
	if (!nNumOfBytes)
		nNumOfBytes = 1;

	// ANSI says: loop continuously because the error handler could possibly free up some memory
	for (;;) {
		// Try the allocation
		void *pAddress = PLCore::MemoryManager::Allocator(PLCore::MemoryManager::NewArray, nNumOfBytes, "?", 0);
		if (pAddress)
			return pAddress;
	}
}

inline void operator delete(void *pAddress)
{
	// ANSI says: delete & delete[] allow null pointers (they do nothing)
	if (pAddress)
		PLCore::MemoryManager::Deallocator(PLCore::MemoryManager::Delete, pAddress, "?", 0);
}

inline void operator delete[](void *pAddress)
{
	// ANSI says: delete & delete[] allow null pointers (they do nothing)
	if (pAddress)
		PLCore::MemoryManager::Deallocator(PLCore::MemoryManager::DeleteArray, pAddress, "?", 0);
}

inline void *operator new(size_t nNumOfBytes, const char *pszSourceFile, int nSourceLine)
{
	// ANSI says: allocation requests of 0 bytes will still return a valid value
	if (!nNumOfBytes)
		nNumOfBytes = 1;

	// ANSI says: loop continuously because the error handler could possibly free up some memory
	for (;;) {
		// Try the allocation
		void *pAddress = PLCore::MemoryManager::Allocator(PLCore::MemoryManager::New, nNumOfBytes, pszSourceFile, nSourceLine);
		if (pAddress)
			return pAddress;
	}
}

inline void *operator new[](size_t nNumOfBytes, const char *pszSourceFile, int nSourceLine)
{
	// ANSI says: allocation requests of 0 bytes will still return a valid value
	if (!nNumOfBytes)
		nNumOfBytes = 1;

	// ANSI says: loop continuously because the error handler could possibly free up some memory
	for (;;) {
		// Try the allocation
		void *pAddress = PLCore::MemoryManager::Allocator(PLCore::MemoryManager::New, nNumOfBytes, pszSourceFile, nSourceLine);
		if (pAddress)
			return pAddress;
	}
}

inline void operator delete(void *pAddress, const char *pszSourceFile, int nSourceLine)
{
	// ANSI says: delete & delete[] allow null pointers (they do nothing)
	if (pAddress)
		PLCore::MemoryManager::Deallocator(PLCore::MemoryManager::Delete, pAddress, pszSourceFile, nSourceLine);
}

inline void operator delete[](void *pAddress, const char *pszSourceFile, int nSourceLine)
{
	// ANSI says: delete & delete[] allow null pointers (they do nothing)
	if (pAddress)
		PLCore::MemoryManager::Deallocator(PLCore::MemoryManager::Delete, pAddress, pszSourceFile, nSourceLine);
}
