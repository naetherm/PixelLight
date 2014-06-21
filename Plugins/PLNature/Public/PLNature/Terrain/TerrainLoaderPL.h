/*********************************************************\
 *  File: TerrainLoaderPL.h                              *
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


#ifndef __PLNATURE_SCENENODE_TERRAINLOADER_PL_H__
#define __PLNATURE_SCENENODE_TERRAINLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLNature/Terrain/TerrainLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLNature {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SNTerrain;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Terrain loader implementation for the PixelLight terrain XML file format
*/
class TerrainLoaderPL : public TerrainLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLNATURE_RTTI_EXPORT, TerrainLoaderPL, "PLNature", PLNature::TerrainLoader, "Terrain loader implementation for the PixelLight terrain XML file format")
		// Properties
		pl_properties
			pl_property("Formats",	"terrain,TERRAIN")
			pl_property("Load",		"1")
			pl_property("Save",		"0")
		pl_properties_end
		// Methods
		pl_method_2(Load,	pl_ret_type(bool),	SNTerrain&,	PLCore::File&,	"Load method",	"")
		pl_method_2(Save,	pl_ret_type(bool),	SNTerrain&,	PLCore::File&,	"Save method",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLNATURE_API bool Load(SNTerrain &cSNTerrain, PLCore::File &cFile);
		PLNATURE_API bool Save(SNTerrain &cSNTerrain, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default Constructor
		*/
		PLNATURE_API TerrainLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLNATURE_API virtual ~TerrainLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cSNTerrain
		*    Terrain to load into
		*  @param[in] cTerrainElement
		*    Terrain XML element to read the data from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(SNTerrain &cSNTerrain, const PLCore::XmlElement &cTerrainElement) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature


#endif // __PLNATURE_SCENENODE_TERRAINLOADER_PL_H__
