/*********************************************************\
 *  File: StringBufferManager.h                          *
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


#ifndef __PLCORE_STRINGBUFFERMANAGER_H__
#define __PLCORE_STRINGBUFFERMANAGER_H__
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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class StringBuffer;
class StringBufferUTF8;
class StringBufferASCII;
class StringBufferUnicode;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    String buffer manager
*/
class StringBufferManager {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class String;
	friend class StringBuffer;
	friend class StringBufferUTF8;
	friend class StringBufferASCII;
	friend class StringBufferUnicode;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 NumOfReservedCharacters		= 64;	/**< Number of reserved characters for future use to add when allocating a new string buffer */
		static const uint32 MaxStringReuseLength		= 256;	/**< We don't want to keep alive long strings for later reuse... */
		static const uint32 MaxStringsPerReuseLength	= 4;	/**< Number of reusable strings per string length */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		StringBufferManager();

		/**
		*  @brief
		*    Destructor
		*/
		~StringBufferManager();

		/**
		*  @brief
		*    Returns an instance of a ASCII string buffer
		*
		*  @param[in] nLength
		*    Minimum internal length of the string buffer (excluding the terminating zero, MUST be valid!)
		*
		*  @return
		*    Instance of a ASCII string buffer, a null pointer on error
		*
		*  @note
		*    - The length of the string buffer is 0, but it has already reserved memory for at least "nLength" characters (+ an additional terminating zero)
		*/
		StringBufferASCII *GetStringBufferASCII(uint32 nLength);

		/**
		*  @brief
		*    Returns an instance of a unicode string buffer
		*
		*  @param[in] nLength
		*    Minimum internal length of the string buffer (excluding the terminating zero, MUST be valid!)
		*
		*  @return
		*    Instance of a unicode string buffer, a null pointer on error
		*
		*  @note
		*    - The length of the string buffer is 0, but it has already reserved memory for at least "nLength" characters (+ an additional terminating zero)
		*/
		StringBufferUnicode *GetStringBufferUnicode(uint32 nLength);

		/**
		*  @brief
		*    Releases an instance of a string buffer
		*
		*  @param[in] cStringBuffer
		*    String buffer instance to release
		*/
		PLCORE_API void ReleaseStringBuffer(StringBuffer &cStringBuffer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		StringBufferASCII   **m_pStringBufferASCII;		/**< Reusable ASCII string buffers, can be a null pointer */
		StringBufferUnicode **m_pStringBufferUnicode;	/**< Reusable unicode string buffers, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


#endif // __PLCORE_STRINGBUFFERMANAGER_H__
