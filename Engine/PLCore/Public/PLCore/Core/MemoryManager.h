/*********************************************************\
 *  File: MemoryManager.h                                *
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


#ifndef __PLCORE_MEMORYMANAGER_H__
#define __PLCORE_MEMORYMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Static memory manager class
*
*  @remarks
*    By using this memory manager together with the overwritten variations of global 'new' and
*    'delete' operators we avoid some nasty memory problems when using for instance multiple
*    dynamic libraries. ALL memory allocation/deallocation happens inside the PLCore module and
*    goes 'through' this memory manager, so we can also create statistics, perform security checks
*    and so on.
*    When using the PixelLight way dealing with for instance application entry points (PLMain) or
*    modules (pl_module) by including 'PLCore/ModuleMain.h' or 'PLCore/Main.h' this
*    memory manager is 'activated' automatically. If not (why didn't you use this comfortable way? :)
*    you need to include 'PLCore/Core/MemoryManager.inl' once per project.
*    There are multiple allocation/deallocation types supported - also some legacy types that are only
*    supported for optimal compatibility.
*
*  @note
*    - The __FUNCTION__ information is not used, because some systems don't support it and we may
*      get some conflicts with the overloaded new-operators of 'Microsoft's memory tracker'
*    - There are some simple wrapper functions you can use to avoid for instance nasty
*      standard header includes when they are undesired
*    - We intentionally do NOT offer memory macros (like an overwritten 'malloc') because experience
*      shows that they are making more troubles than adding advantages!
*    - Try to avoid the allocation/deallocation legacy types (Malloc, Calloc, Realloc, Free) to avoid
*      possible memory problems!
*    - Do NOT mix allocation/deallocation types like using 'delete []' to deallocate memory allocated
*      'with (none array) 'new'!
*    - To get information about for instance the current free physical memory have a look at the 'System'-class
*/
class MemoryManager {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Allocation/deallocation types
		*/
		enum EType {
			Unknown,		/**< ? */
			New,			/**< new <type name> */
			NewArray,		/**< new [<number of elements>] <type name> */
			Delete,			/**< delete <pointer> */
			DeleteArray,	/**< delete [] <pointer> */
			// The following legacy types are only supported for optimal compatibility, try to avoid them!
			Malloc,			/**< malloc(<bytes to allocate>), legacy type */
			Calloc,			/**< calloc(<number of elements>, <bytes per element>), legacy type */
			Realloc,		/**< realloc(<pointer to previously memory>, <new size in bytes>), legacy type */
			Free			/**< free(<pointer>), legacy type */
		};


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Copies memory
		*
		*  @param[in] pDestination
		*    Destination buffer (MUST be valid!)
		*  @param[in] pSource
		*    Source buffer (MUST be valid!)
		*  @param[in] nNumOfBytes
		*    Number of bytes to copy
		*
		*  @note
		*    - 'memcpy'-wrapper
		*    -  The memory areas should not overlap
		*/
		static inline void Copy(void *pDestination, const void *pSource, uint32 nNumOfBytes);

		/**
		*  @brief
		*    Moves memory
		*
		*  @param[in] pDestination
		*    Destination buffer (MUST be valid!)
		*  @param[in] pSource
		*    Source buffer (MUST be valid!)
		*  @param[in] nNumOfBytes
		*    Number of bytes to copy
		*
		*  @note
		*    - 'memmove'-wrapper
		*    -  The memory areas may overlap
		*/
		static inline void Move(void *pDestination, const void *pSource, uint32 nNumOfBytes);

		/**
		*  @brief
		*    Sets memory to a specified character
		*
		*  @param[in] pDestination
		*    Destination buffer (MUST be valid!)
		*  @param[in] nCharacter
		*    Character to set
		*  @param[in] nNumOfBytes
		*    Number of bytes to set
		*
		*  @note
		*    - 'memset'-wrapper
		*/
		static inline void Set(void *pDestination, int nCharacter, uint32 nNumOfBytes);

		/**
		*  @brief
		*    Compares memory
		*
		*  @param[in] pFirstBuffer
		*    First buffer (MUST be valid!)
		*  @param[in] pSecondBuffer
		*    Second buffer (MUST be valid!)
		*  @param[in] nNumOfBytes
		*    Number of bytes to compare
		*
		*  @return
		*    < 0 = First buffer is less than second buffer
		*      0 = First buffer identical to second buffer
		*    > 0 = First buffer is greater than second buffer
		*
		*  @note
		*    - 'memcmp'-wrapper
		*/
		static inline int Compare(const void *pFirstBuffer, const void *pSecondBuffer, uint32 nNumOfBytes);

		/**
		*  @brief
		*    Reallocates memory
		*
		*  @param[in] pAddress
		*    Previous memory, can be a null pointer (if not a null pointer, it MUST be valid!)
		*  @param[in] nNumOfBytes
		*    Number of bytes to allocate
		*  @param[in] nType
		*    Allocator type
		*  @param[in] pszSourceFile
		*    Source file, can be a null pointer
		*  @param[in] nSourceLine
		*    Source line
		*
		*  @return
		*    The new allocated memory, a null pointer on error
		*/
		static inline void *Reallocator(void *pAddress, size_t nNumOfBytes, EType nType = NewArray, const char *pszSourceFile = nullptr, int nSourceLine = -1);

		/**
		*  @brief
		*    Allocates new memory
		*
		*  @param[in] nType
		*    Allocator type
		*  @param[in] nNumOfBytes
		*    Number of bytes to allocate
		*  @param[in] pszSourceFile
		*    Source file, can be a null pointer
		*  @param[in] nSourceLine
		*    Source line
		*
		*  @return
		*    The new allocated memory, a null pointer on error
		*
		*  @note
		*    - Normally there's no reason using this function directly, use the 'new' keyword instead
		*/
		static inline void *Allocator(EType nType, size_t nNumOfBytes, const char *pszSourceFile = nullptr, int nSourceLine = -1);

		/**
		*  @brief
		*    Frees memory
		*
		*  @param[in] nType
		*    Allocator type
		*  @param[in] pAddress
		*    Memory to free (MUST be valid!)
		*  @param[in] pszSourceFile
		*    Source file, can be a null pointer
		*  @param[in] nSourceLine
		*    Source line
		*
		*  @note
		*    - Normally there's no reason using this function directly, use the 'delete' keyword instead
		*/
		static inline void Deallocator(EType nType, void *pAddress, const char *pszSourceFile = nullptr, int nSourceLine = -1);


};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Core/MemoryManager.inl"


//[-------------------------------------------------------]
//[ Variations of global 'new' and 'delete' operators     ]
//[-------------------------------------------------------]
void *operator new(size_t nNumOfBytes);
void *operator new[](size_t nNumOfBytes);
void  operator delete(void *pAddress);
void  operator delete[](void *pAddress);
// This two special new-operators are also used by 'Microsoft's memory tracker', so there's no undesired interfering
void *operator new(size_t nNumOfBytes, const char *pszSourceFile, int nSourceLine);
void *operator new[](size_t nNumOfBytes, const char *pszSourceFile, int nSourceLine);
void  operator delete(void *pAddress, const char *pszSourceFile, int nSourceLine);
void  operator delete[](void *pAddress, const char *pszSourceFile, int nSourceLine);


#endif // __PLCORE_MEMORYMANAGER_H__
