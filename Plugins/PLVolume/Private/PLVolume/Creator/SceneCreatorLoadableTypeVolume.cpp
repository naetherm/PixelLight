/*********************************************************\
 *  File: SceneCreatorLoadableTypeVolume.cpp             *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneContainer.h>
#include <PLEngine/Application/EngineApplication.h>
#include "PLVolume/Creator/SceneCreatorLoadableTypeVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneCreatorLoadableTypeVolume)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SceneCreatorLoadableTypeVolume::SceneCreatorLoadableTypeVolume()
{
}

/**
*  @brief
*    Destructor
*/
SceneCreatorLoadableTypeVolume::~SceneCreatorLoadableTypeVolume()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneCreator functions                 ]
//[-------------------------------------------------------]
SNCamera *SceneCreatorLoadableTypeVolume::Create(SceneContainer &cSceneContainer) const
{
	{ // [HACK][TODO] Scene creator: Make it possible to specify preferred scene renderer
		const PLCore::String sFilename = "Volume.sr";
		static_cast<PLEngine::EngineApplication*>(PLCore::CoreApplication::GetApplication())->GetSceneRendererTool().SetSceneRenderer(&cSceneContainer, sFilename, sFilename);
	}

	// Create a scene node representing the volume, most volumes look better with a 180 degree rotation around the z-axis
	SceneNode *pPrimarySceneNode = cSceneContainer.Create("PLVolume::SNVolume", "Volume", "Rotation=\"0 0 180\" VolumeFilename=\"" + Filename.Get() + '\"');

	// Configure a generic scene and return the preferred camera scene node
	SNCamera *pCamera = SceneCreator::ConfigureGenericScene(cSceneContainer, pPrimarySceneNode);

	// When using volume rendering, a common use case is to move the camera really close to or even into the volume
	// -> The default value of the camera z-near plane is 0.1 because this value has proven
	//    to be useful when trying to cover many use cases with a single default value
	// -> When using a small scaled volume, a z-near plane value of 0.1 may cause the premature clipping of the
	//    volume, so use a smaller value instead
	{ // Update free camera
		SceneNode *pFreeCamera = cSceneContainer.GetByName("FreeCamera");
		if (pFreeCamera)
			pFreeCamera->SetAttribute("ZNear", Var<float>(0.01f));
	}
	{ // Update orbiter camera
		SceneNode *pOrbiterCamera = cSceneContainer.GetByName("OrbiterCamera");
		if (pOrbiterCamera)
			pOrbiterCamera->SetAttribute("ZNear", Var<float>(0.01f));
	}

	// Done
	return pCamera;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
