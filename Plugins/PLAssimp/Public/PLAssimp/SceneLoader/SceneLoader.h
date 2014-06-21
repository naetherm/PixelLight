/*********************************************************\
 *  File: SceneLoader.h                                  *
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


#ifndef __PLASSIMP_SCENELOADER_H__
#define __PLASSIMP_SCENELOADER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneLoader/SceneLoader.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLScene {
	class SceneContainer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene loader implementation using Assimp base class
*/
class SceneLoader : public PLScene::SceneLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SceneLoader, "PLAssimp", PLScene::SceneLoader, "Abstract scene loader implementation using Assimp base class")
		// Properties
		pl_properties
			pl_property("Load",	"1")
			pl_property("Save",	"0")
		pl_properties_end
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	PLScene::SceneContainer&,	PLCore::File&,					"Load method. The loaded scene is post processed for maximum quality.",																	"")
		pl_method_3(LoadParams,	pl_ret_type(bool),	PLScene::SceneContainer&,	PLCore::File&,	PLCore::uint8,	"Load method. Parameters: First integer parameter for post processing quality (0=none ... 3=maximum quality but slowest processing).",	"")
		pl_method_2(Save,		pl_ret_type(bool),	PLScene::SceneContainer&,	PLCore::File&,					"Save method",																															"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		bool Load(PLScene::SceneContainer &cContainer, PLCore::File &cFile);
		virtual bool LoadParams(PLScene::SceneContainer &cContainer, PLCore::File &cFile, PLCore::uint8 nQuality) = 0;
		bool Save(PLScene::SceneContainer &cContainer, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		SceneLoader();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SceneLoader();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp


#endif // __PLASSIMP_SCENELOADER_H__
