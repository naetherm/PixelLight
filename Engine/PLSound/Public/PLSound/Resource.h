/*********************************************************\
 *  File: Resource.h                                     *
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


#ifndef __PLSOUND_RESOURCE_H__
#define __PLSOUND_RESOURCE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include "PLSound/PLSound.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSound {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SoundManager;
class ResourceHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sound resource
*/
class Resource {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Resource type
		*/
		enum EType {
			TypeBuffer = 0,	/**< Sound buffer */
			TypeSource = 1	/**< Sound source */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLSOUND_API virtual ~Resource();

		/**
		*  @brief
		*    Returns the owner sound manager
		*
		*  @return
		*    The owner sound manager
		*/
		PLSOUND_API SoundManager &GetSoundManager() const;

		/**
		*  @brief
		*    Returns the resource type
		*
		*  @return
		*    Resource type
		*/
		PLSOUND_API EType GetType() const;

		/**
		*  @brief
		*    Returns the number of resource handlers
		*
		*  @return
		*    The number of resource handlers
		*/
		PLSOUND_API PLCore::uint32 GetNumOfResourceHandlers() const;

		/**
		*  @brief
		*    Returns whether a resource handler is using this resource
		*
		*  @param[in] cHandler
		*    Resource handler to check
		*
		*  @return
		*    'true' if the given resource handler is using this resource, else 'false'
		*
		*  @note
		*    - The handlers will be informed if the resource was lost
		*    - The handlers itself are not deleted etc. they are only 'added' to a list
		*/
		PLSOUND_API bool IsResourceHandler(ResourceHandler &cHandler) const;

		/**
		*  @brief
		*    Adds a resource handler
		*
		*  @param[in] cHandler
		*    Resource handler which should be added
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - IsHandler()
		*/
		PLSOUND_API bool AddResourceHandler(ResourceHandler &cHandler);

		/**
		*  @brief
		*    Removes a resource handler
		*
		*  @param[in] cHandler
		*    Resource handler to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - IsHandler()
		*/
		PLSOUND_API bool RemoveResourceHandler(ResourceHandler &cHandler);

		/**
		*  @brief
		*    Clears the list of handlers
		*
		*  @see
		*    - IsHandler()
		*/
		PLSOUND_API void ClearResourceHandlers();


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
		*  @param[in] nType
		*    Resource type
		*/
		PLSOUND_API Resource(SoundManager &cSoundManager, EType nType);


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
		Resource(const Resource &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SoundManager				   *m_pSoundManager;	/**< Owner sound manager (always valid!) */
		EType						    m_nType;			/**< Resource type */
		PLCore::List<ResourceHandler*>  m_lstSoundHandlers;	/**< List of resource handlers */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound


#endif // __PLSOUND_RESOURCE_H__
