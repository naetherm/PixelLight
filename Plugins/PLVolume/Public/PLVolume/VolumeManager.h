/*********************************************************\
 *  File: VolumeManager.h                                *
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


#ifndef __PLVOLUME_VOLUMEMANAGER_H__
#define __PLVOLUME_VOLUMEMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/Singleton.h>
#include <PLCore/Container/ResourceManager.h>
#include "PLVolume/Volume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This is a manager for the volume resource
*
*  @note
*    - Unloads unused resources automatically by default
*/
class VolumeManager : public PLCore::ResourceManager<Volume>, public PLCore::Singleton<VolumeManager> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class PLCore::Singleton<VolumeManager>;


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		static PLVOLUME_API VolumeManager *GetInstance();
		static PLVOLUME_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLVOLUME_API VolumeManager();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~VolumeManager();


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ResourceManager functions     ]
	//[-------------------------------------------------------]
	private:
		virtual Volume *CreateResource(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_VOLUMEMANAGER_H__
