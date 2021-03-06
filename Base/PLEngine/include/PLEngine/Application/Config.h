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
	pl_class_def(PL_API)
	pl_class_def_end


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
	pl_class_def(PL_API)
		// Attributes
		pl_attribute_directvalue(			ConsoleActivated,	bool,	true,	ReadWrite)
		pl_attribute_getset		(Config,	MaxTimeDifference,	float,	0.15f,	ReadWrite)
		pl_attribute_getset		(Config,	FPSLimit,			float,	0.0f,	ReadWrite)
	pl_class_def_end


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
	pl_class_def(PL_API)
		// Attributes
		pl_attribute_directvalue(ShowFPS,				bool,	false,	ReadWrite)
		pl_attribute_directvalue(ShowCoordinateAxis,	bool,	false,	ReadWrite)
		pl_attribute_directvalue(ShowXZPlane,			bool,	false,	ReadWrite)
		pl_attribute_directvalue(ShowXYPlane,			bool,	false,	ReadWrite)
		pl_attribute_directvalue(ShowYZPlane,			bool,	false,	ReadWrite)
	pl_class_def_end


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
