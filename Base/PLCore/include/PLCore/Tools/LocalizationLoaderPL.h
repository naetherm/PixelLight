/*********************************************************\
 *  File: LocalizationLoaderPL.h                         *
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


#ifndef __PLCORE_LOCALIZATIONLOADER_PL_H__
#define __PLCORE_LOCALIZATIONLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/LocalizationLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class LocalizationGroup;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Localization loader implementation for the PixelLight localization XML file format
*/
class LocalizationLoaderPL : public LocalizationLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLCORE_API)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API bool Load(LocalizationGroup &cLocalizationGroup, File &cFile);
		PLCORE_API bool Save(const LocalizationGroup &cLocalizationGroup, File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API LocalizationLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~LocalizationLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cLocalizationGroup
		*    Localization group to load into
		*  @param[in] cLocalizationGroupElement
		*    Localization group XML element to read the data from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(LocalizationGroup &cLocalizationGroup, const XmlElement &cLocalizationElement) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_LOCALIZATIONLOADER_PL_H__
