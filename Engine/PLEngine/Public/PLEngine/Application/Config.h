/*********************************************************\
 *  File: Config.h                                       *
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


#ifndef __PLENGINE_CONFIG_H__
#define __PLENGINE_CONFIG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Config/Config.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Engine configuration 'collection' class
*
*  @remarks
*    ALL engine configuration classes are derived from this class.
*    Use cConfig.Save("Config/Engine.cfg", "PLEngine::ConfigGroup") to save
*    ONLY all engine configuration classes.
*/
class ConfigGroup : public PLCore::ConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, ConfigGroup, "PLEngine", PLCore::ConfigGroup, "Engine configuration 'collection' class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PL_API ConfigGroup();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~ConfigGroup();


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
		ConfigGroup(const ConfigGroup &cSource);

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
		ConfigGroup &operator =(const ConfigGroup &cSource);


};

/**
*  @brief
*    Engine configuration classes
*/
class Config : public ConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, Config, "PLEngine", PLEngine::ConfigGroup, "Engine configuration classes")
		// Attributes
		pl_attribute(ConsoleActivated,	bool,	true,	ReadWrite,	DirectValue,	"Is the console active?",										"")
		pl_attribute(MaxTimeDifference,	float,	0.15f,	ReadWrite,	GetSet,			"The maximum time difference since the last frame in seconds",	"Min='0.0001'")
		pl_attribute(FPSLimit,			float,	0.0f,	ReadWrite,	GetSet,			"FPS limit, 0 if there's no FPS limitation",					"Min='0.0'")
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
		PL_API Config();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~Config();


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PL_API float GetMaxTimeDifference() const;
		PL_API void SetMaxTimeDifference(float fValue) const;
		PL_API float GetFPSLimit() const;
		PL_API void SetFPSLimit(float fValue) const;


};


/**
*  @brief
*    Engine debug configuration classes
*/
class DebugConfig : public ConfigGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, DebugConfig, "PLEngine", PLEngine::ConfigGroup, "Engine debug configuration classes")
		// Attributes
		pl_attribute(ShowFPS,				bool,	false,	ReadWrite,	DirectValue,	"Show FPS?",				"")
		pl_attribute(ShowCoordinateAxis,	bool,	false,	ReadWrite,	DirectValue,	"Show coordinate axis?",	"")
		pl_attribute(ShowXZPlane,			bool,	false,	ReadWrite,	DirectValue,	"Show x/z plane?",			"")
		pl_attribute(ShowXYPlane,			bool,	false,	ReadWrite,	DirectValue,	"Show x/y plane?",			"")
		pl_attribute(ShowYZPlane,			bool,	false,	ReadWrite,	DirectValue,	"Show y/z plane?",			"")
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
		PL_API DebugConfig();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~DebugConfig();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_CONFIG_H__
