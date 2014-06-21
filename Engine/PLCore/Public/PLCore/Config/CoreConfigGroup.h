/*********************************************************\
 *  File: CoreConfigGroup.h                              *
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


#ifndef __PLCORE_CONFIGGROUP_H__
#define __PLCORE_CONFIGGROUP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Config/Config.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PLCore general configuration 'collection' class
*/
class CoreConfigGroup : public ConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, CoreConfigGroup, "PLCore", PLCore::ConfigGroup, "PLCore general configuration 'collection' class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		CoreConfigGroup();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CoreConfigGroup();


};

/**
*  @brief
*    PLCore configuration classes
*/
class CoreConfig : public CoreConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, CoreConfig, "PLCore", PLCore::CoreConfigGroup, "PLCore configuration classes")
		// Attributes
		pl_attribute(FirstRun,	bool,	true,	ReadWrite,	DirectValue,	"Is this the first application start?",																	"")
		pl_attribute(Language,	String,	"",		ReadWrite,	DirectValue,	"Current used language, if empty the current set OS locale language is used (for instance 'German'",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API CoreConfig();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~CoreConfig();


};

/**
*  @brief
*    PLCore frontend configuration classes
*/
class FrontendConfig : public CoreConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, FrontendConfig, "PLCore", PLCore::CoreConfigGroup, "PLCore frontend configuration classes")
		// Attributes
		pl_attribute(X,			int,	0,		ReadWrite,	DirectValue,	"X position of the frontend (in screen coordinates)",	"")
		pl_attribute(Y,			int,	0,		ReadWrite,	DirectValue,	"Y position of the frontend (in screen coordinates)",	"")
		pl_attribute(Width,		uint32,	800,	ReadWrite,	DirectValue,	"Width of the frontend",								"")
		pl_attribute(Height,	uint32,	600,	ReadWrite,	DirectValue,	"Height of the frontend",								"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API FrontendConfig();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~FrontendConfig();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CONFIGGROUP_H__
