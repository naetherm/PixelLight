/*********************************************************\
 *  File: SceneCreator.h                                 *
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


#ifndef __PLENGINE_SCENECREATOR_H__
#define __PLENGINE_SCENECREATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLScene {
	class SNCamera;
	class SceneNode;
	class SceneContainer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract scene creator base class
*/
class SceneCreator : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PL_API)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Configures a generic scene
		*
		*  @param[in] cSceneContainer
		*    The scene container to add the new scene nodes to (not cleared before filled)
		*  @param[in] pPrimarySceneNode
		*    Primary scene node, for example representing the loaded mesh or a box with the loaded material etc., can be a null pointer
		*
		*  @return
		*    The preferred camera scene node, can be a null pointer
		*
		*  @note
		*    - Adds a directional light source scene node named "Light"
		*    - Adds a free camera scene node named "FreeCamera"
		*    - If primary scene node given, adds an orbiter camera scene node named "OrbiterCamera"
		*    - Makes the orbiting camera to the preferred camera scene node, if there's no orbiting camera then the free camera will be used instead
		*/
		static PL_API PLScene::SNCamera *ConfigureGenericScene(PLScene::SceneContainer &cSceneContainer, PLScene::SceneNode *pPrimarySceneNode);

		/**
		*  @brief
		*    Configures a scene
		*
		*  @param[in] cSceneContainer
		*    The scene container to add the new scene nodes to (not cleared before filled)
		*  @param[in] sClass
		*    Name of the scene creator class to use for configuring the scene
		*  @param[in] sParameters
		*    Optional parameter string
		*
		*  @return
		*    The preferred camera scene node, can be a null pointer
		*/
		static PL_API PLScene::SNCamera *ConfigureScene(PLScene::SceneContainer &cSceneContainer, const PLCore::String &sClass, const PLCore::String &sParameters = "");


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PL_API SceneCreator();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SceneCreator();


	//[-------------------------------------------------------]
	//[ Public virtual SceneCreator functions                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Creates the content of a scene container
		*
		*  @param[in] cSceneContainer
		*    Scene container to manipulate (not cleared before filled)
		*
		*  @return
		*    The preferred camera scene node, can be a null pointer
		*/
		PL_API virtual PLScene::SNCamera *Create(PLScene::SceneContainer &cSceneContainer) const = 0;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_SCENECREATOR_H__
