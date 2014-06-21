/*********************************************************\
 *  File: ConfigLoaderPL.h                               *
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


#ifndef __PLCORE_CONFIGLOADER_PL_H__
#define __PLCORE_CONFIGLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Config/ConfigLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Config;
class XmlElement;
class ConfigGroup;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Config loader implementation for the PixelLight config XML file format
*/
class ConfigLoaderPL : public ConfigLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, ConfigLoaderPL, "PLCore", PLCore::ConfigLoader, "Config loader implementation for the PixelLight config XML file format")
		// Properties
		pl_properties
			pl_property("Formats",	"cfg,CFG")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	Config&,		File&,					"Load method",																						"")
		pl_method_2(Save,		pl_ret_type(bool),	const Config&,	File&,					"Save method. Saves all configuration groups and includes default values, too.",					"")
		pl_method_4(SaveParams,	pl_ret_type(bool),	const Config&,	File&,	String,	bool,	"Save method. Parameters: Configuration group to save as 'String', no default values as 'bool'.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API bool Load(Config &cConfig, File &cFile);
		PLCORE_API bool Save(const Config &cConfig, File &cFile);
		PLCORE_API bool SaveParams(const Config &cConfig, File &cFile, String sGroup, bool bNoDefault);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API ConfigLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ConfigLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cConfig
		*    Configuration to load into
		*  @param[in] cConfigElement
		*    Configuration XML element to read the data from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(Config &cConfig, const XmlElement &cConfigElement) const;

		/**
		*  @brief
		*    Save configuration group settings
		*
		*  @param[in] cConfigElement
		*    Configuration element to write in
		*  @param[in] cGroup
		*    Configuration group to save
		*  @param[in] bNoDefault
		*    If 'true', variables with default values will be ignored
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool SaveGroup(XmlElement &cConfigElement, const ConfigGroup &cGroup, bool bNoDefault = false) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_CONFIGLOADER_PL_H__
