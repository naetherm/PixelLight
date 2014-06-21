/*********************************************************\
 *  File: Buffer.h                                       *
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


#ifndef __PLCORE_BUFFER_H__
#define __PLCORE_BUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Network buffer
*
*  @remarks
*    This buffer can be used within concrete network implementations to manage send/received
*    network data.
*/
class Buffer {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nSize
		*    Size of the buffer in bytes, if 0, no buffer is allocated
		*/
		PLCORE_API Buffer(uint32 nSize = 0);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLCORE_API Buffer(const Buffer &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~Buffer();

		/**
		*  @brief
		*    Get option for removing line delimiters
		*
		*  @return
		*    'true' if line delimiters shall be removed, else 'false'
		*/
		inline bool GetRemoveLineDelimiters() const;

		/**
		*  @brief
		*    Set option for removing line delimiters
		*
		*  @param[in] bRemoveDelimiters
		*    'true' if line delimiters shall be removed, else 'false'
		*/
		inline void SetRemoveLineDelimiters(bool bRemoveDelimiters);

		/**
		*  @brief
		*    Clear buffer
		*/
		PLCORE_API void Clear();

		/**
		*  @brief
		*    Allocates a new buffer (all previous data will be lost)
		*
		*  @param[in] nSize
		*    Size of the buffer in bytes, if 0, no buffer is allocated (same as just Clear())
		*/
		PLCORE_API void Allocate(uint32 nSize);

		/**
		*  @brief
		*    Get current buffer size
		*
		*  @return
		*    The current buffer size in bytes
		*/
		inline uint32 GetSize() const;

		/**
		*  @brief
		*    Add data to the buffer
		*
		*  @param[in] pData
		*    Data to add, if a null pointer nothing happens
		*  @param[in] nSize
		*    Size of the data to add in bytes (MUST be correct!)
		*/
		PLCORE_API void Add(const char *pData, uint32 nSize);

		/**
		*  @brief
		*    Get a line from the buffer
		*
		*  @return
		*    The read line, empty string on error
		*
		*  @note
		*    - The read line is removed from buffer
		*    - \r\n will be replaced by \0.
		*/
		PLCORE_API String GetLine();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		PLCORE_API Buffer &operator =(const Buffer &cSource);

		/**
		*  @brief
		*    Get data
		*
		*  @return
		*    Pointer to buffer
		*/
		inline const char *GetData() const;

		/**
		*  @brief
		*    Get data
		*
		*  @return
		*    Pointer to buffer
		*/
		inline char *GetData();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		char	*m_pBuffer;				/**< Data buffer, can be a null pointer */
		uint32   m_nSize;				/**< Size of buffer */
		uint32   m_nUsed;				/**< Currently used size */
		bool	 m_bRemoveDelimiters;	/**< Remove line delimiters (CR/LF)? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Network/Buffer.inl"


#endif // __PLCORE_BUFFER_H__
