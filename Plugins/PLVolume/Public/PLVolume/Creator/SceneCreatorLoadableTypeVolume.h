/*********************************************************\
 *  File: SceneCreatorLoadableTypeVolume.h               *
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


#ifndef __PLVOLUME_SCENECREATOR_LOADABLETYPE_IMAGE_H__
#define __PLVOLUME_SCENECREATOR_LOADABLETYPE_IMAGE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLEngine/SceneCreator/SceneCreatorLoadableType.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Volume loadable type scene creator
*/
class SceneCreatorLoadableTypeVolume : public PLEngine::SceneCreatorLoadableType {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, SceneCreatorLoadableTypeVolume, "PLVolume", PLEngine::SceneCreatorLoadableType, "Volume loadable type scene creator")
		// Properties
		pl_properties
			pl_property("Type",	"Volume")
		pl_properties_end
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
		PLVOLUME_API SceneCreatorLoadableTypeVolume();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~SceneCreatorLoadableTypeVolume();


	//[-------------------------------------------------------]
	//[ Public virtual PLEngine::SceneCreator functions       ]
	//[-------------------------------------------------------]
	public:
		PLVOLUME_API virtual PLScene::SNCamera *Create(PLScene::SceneContainer &cSceneContainer) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_SCENECREATOR_LOADABLETYPE_IMAGE_H__
