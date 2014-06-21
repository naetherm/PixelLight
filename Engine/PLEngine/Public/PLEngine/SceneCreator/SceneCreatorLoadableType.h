/*********************************************************\
 *  File: SceneCreatorLoadableType.h                     *
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


#ifndef __PLENGINE_SCENECREATOR_LOADABLETYPE_H__
#define __PLENGINE_SCENECREATOR_LOADABLETYPE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLEngine/SceneCreator/SceneCreator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract loadable type scene creator base class
*
*  @remarks
*    There are situations when someone wants to, for example, see an image or an mesh without writing
*    a lot of source code. Also, knowing all possible loadable types when writing the application may
*    not be possible due to dynamic plugins. A loadable type scene creator can be used to create a ready
*    to-be-used scene by just providing the loadable type and some parameters like e.g. the filename of
*    the resource to view.
*
*    Each loader should have the following properties:
*    - "Type": Loadable type ("Image", "Mesh" and so on)
*/
class SceneCreatorLoadableType : public SceneCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SceneCreatorLoadableType, "PLEngine", PLEngine::SceneCreator, "Abstract loadable type scene creator base class")
		// Properties
		pl_properties
			pl_property("Type",	"Unknown")
		pl_properties_end
		// Attributes
		pl_attribute(Filename,	PLCore::String,	"",	ReadWrite,	DirectValue,	"Resource filename",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Configures a scene by using a given loadable type in order to determine the scene creator to use
		*
		*  @param[in] cSceneContainer
		*    The scene container to add the new scene nodes to (not cleared before filled)
		*  @param[in] sType
		*    Loadable type ("Image", "Mesh" and so on)
		*  @param[in] sFilename
		*    Resource filename
		*  @param[in] sParameters
		*    Optional parameter string (without resource filename)
		*
		*  @return
		*    The preferred camera scene node, can be a null pointer
		*/
		static PL_API PLScene::SNCamera *ConfigureSceneByLoadableType(PLScene::SceneContainer &cSceneContainer, const PLCore::String &sType, const PLCore::String &sFilename, const PLCore::String &sParameters = "");

		/**
		*  @brief
		*    Returns a list of loadable types were it's possible to use a scene creator in order to configure a scene
		*
		*  @param[out] lstLoadableTypes
		*    Receives the loadable types ("Image", "Mesh" and so on, not cleared before filled)
		*/
		static PL_API void GetLoadableTypes(PLCore::Array<PLCore::String> &lstLoadableTypes);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PL_API SceneCreatorLoadableType();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SceneCreatorLoadableType();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_SCENECREATOR_LOADABLETYPE_H__
