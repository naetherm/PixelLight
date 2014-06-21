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


#ifndef __PLSOUND_BUFFER_H__
#define __PLSOUND_BUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Resource.h>
#include "PLSound/Resource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSound {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Source;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract sound buffer resource
*/
class Buffer : public Resource, public PLCore::Resource<Buffer> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Source;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLSOUND_API virtual ~Buffer();

		/**
		*  @brief
		*    Returns the number of sources
		*
		*  @return
		*    Number of sources
		*/
		PLSOUND_API PLCore::uint32 GetNumOfSources() const;

		/**
		*  @brief
		*    Returns a sound source
		*
		*  @param[in] nIndex
		*    Index of the resource to return
		*
		*  @return
		*    The source at the given index, a null pointer on error
		*/
		PLSOUND_API const Source *GetSource(PLCore::uint32 nIndex = 0) const;

		/**
		*  @brief
		*    Returns a sound source
		*
		*  @param[in] nIndex
		*    Index of the resource to return
		*
		*  @return
		*    The source at the given index, a null pointer on error
		*/
		PLSOUND_API Source *GetSource(PLCore::uint32 nIndex = 0);


	//[-------------------------------------------------------]
	//[ Public virtual Buffer functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Loads the sound buffer with the given data from a file
		*
		*  @param[in] sFilename
		*    Sound filename (full path, supported file formats are API dependent)
		*  @param[in] bStream
		*    Stream the file?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Not each sound buffer can be streamed, use IsStreamed()
		*      to check whether streaming is used
		*/
		virtual bool LoadBuffer(const PLCore::String &sFilename, bool bStream) = 0;

		/**
		*  @brief
		*    Loads the sound buffer with the given data from memory
		*
		*  @param[in] nData
		*    Sound data
		*  @param[in] nSize
		*    Number of data bytes (MUST be valid!)
		*  @param[in] bStream
		*    Stream the data?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If bStream is 'true' nData MUST stay valid!
		*    - Not each sound buffer can be streamed, use IsStreamed() to check whether streaming is used
		*/
		virtual bool LoadBuffer(const PLCore::uint8 nData[], PLCore::uint32 nSize, bool bStream = false) = 0;

		/**
		*  @brief
		*    Returns whether the buffer is loaded or not
		*
		*  @return
		*    'true' if the buffer is loaded, else 'false'
		*/
		virtual bool IsLoaded() const = 0;

		/**
		*  @brief
		*    Returns whether the buffer is streamed or not
		*
		*  @return
		*    'true' if the buffer is streamed, else 'false'
		*/
		virtual bool IsStreamed() const = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSoundManager
		*    Owner sound manager
		*  @param[in] sName
		*    Resource name to set
		*/
		PLSOUND_API Buffer(SoundManager &cSoundManager, const PLCore::String &sName = "");


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Buffer(const Buffer &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::List<Source*> m_lstSources;	/**< List of sources using this buffer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound


#endif // __PLSOUND_BUFFER_H__
