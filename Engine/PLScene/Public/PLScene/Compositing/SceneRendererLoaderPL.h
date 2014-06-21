/*********************************************************\
 *  File: SceneRendererLoaderPL.h                        *
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


#ifndef __PLSCENE_COMPOSITING_SCENERENDERERLOADER_PL_H__
#define __PLSCENE_COMPOSITING_SCENERENDERERLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Compositing/SceneRendererLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SceneRenderer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene renderer loader implementation for the PixelLight scene renderer XML file format
*
*  @note
*    - If the save function receives a boolean 'NoDefault'-parameter with 'true', (default setting)
*      variables with default values are not saved
*/
class SceneRendererLoaderPL : public SceneRendererLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SceneRendererLoaderPL, "PLScene", PLScene::SceneRendererLoader, "Scene renderer loader implementation for the PixelLight scene renderer XML file format")
		// Properties
		pl_properties
			pl_property("Formats",	"sr,SR")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	SceneRenderer&,	PLCore::File&,			"Load method",												"")
		pl_method_2(Save,		pl_ret_type(bool),	SceneRenderer&,	PLCore::File&,			"Save method. Default values are not written.",				"")
		pl_method_3(SaveParams, pl_ret_type(bool),	SceneRenderer&,	PLCore::File&,	bool,	"Save method. Parameters: No default values as 'bool'.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLS_API bool Load(SceneRenderer &cSceneRenderer, PLCore::File &cFile);
		PLS_API bool Save(SceneRenderer &cSceneRenderer, PLCore::File &cFile);
		PLS_API bool SaveParams(SceneRenderer &cSceneRenderer, PLCore::File &cFile, bool bNoDefault);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SceneRendererLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SceneRendererLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cSceneRenderer
		*    Scene renderer to load into
		*  @param[in] cSceneRendererElement
		*    Scene renderer XML element to read the data from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(SceneRenderer &cSceneRenderer, const PLCore::XmlElement &cSceneRendererElement) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_COMPOSITING_SCENERENDERERLOADER_PL_H__
