/*********************************************************\
 *  File: VolumeLoaderDDS.h                              *
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


#ifndef __PLVOLUME_VOLUMELOADER_DDS_H__
#define __PLVOLUME_VOLUMELOADER_DDS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolume/Loader/VolumeLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Volume;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Volume loader implementation for the "DDS" volume data file format
*
*  @remarks
*    This file format supports GPU texture compression formats like LATC1 and therefore makes it possible to load in
*    compressed volume data as efficient as possible by directly passing thru the compressed loaded data to the GPU
*    without need for any further processing.
*
*  @note
*    - DirectDraw Surface file format from Microsoft
*    - Use e.g. "The Compressonator" from AMD (http://developer.amd.com/tools/compressonator/pages/default.aspx) to open, view, edit and save the dds-file
*    - BGR(A) color format is automatically converted to RGB(A)
*      This volume loader is in fact just a comfort wrapper around the DDS image loader
*/
class VolumeLoaderDDS : public VolumeLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, VolumeLoaderDDS, "PLVolume", PLVolume::VolumeLoader, "Volume loader implementation for the \"DDS\" volume data file format")
		// Properties
		pl_properties
			pl_property("Formats",	"dds,DDS")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	Volume&,		PLCore::File&,							"Load method, voxel size is set to default",															"")
		pl_method_5(LoadParams,	pl_ret_type(bool),	Volume&,		PLCore::File&,	float,	float,	float,	"Load method. Parameters: First, second and third 'float' parameters are the x, y and z voxel size.",	"")
		pl_method_2(Save,		pl_ret_type(bool),	const Volume&,	PLCore::File&,							"Save method",																							"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		bool Load(Volume &cVolume, PLCore::File &cFile);
		bool LoadParams(Volume &cVolume, PLCore::File &cFile, float fVoxelSizeX, float fVoxelSizeY, float fVoxelSizeZ);
		bool Save(const Volume &cVolume, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VolumeLoaderDDS();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VolumeLoaderDDS();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_VOLUMELOADER_DDS_H__
