/*********************************************************\
 *  File: SceneCreator.cpp                               *
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
#include <PLCore/Base/Class.h>
#include <PLScene/Scene/SceneContainer.h>
#include "PLEngine/SceneCreator/SceneCreator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneCreator)


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Configures a generic scene
*/
SNCamera *SceneCreator::ConfigureGenericScene(SceneContainer &cSceneContainer, SceneNode *pPrimarySceneNode)
{
	// Add a directional light source scene node named "Light"
	cSceneContainer.Create("PLScene::SNDirectionalLight", "Light", "Rotation=\"45 0 0\"");

	// Add a free camera scene node named "FreeCamera"
	SceneNode *pFreeCamera = cSceneContainer.Create("PLScene::SNCamera", "FreeCamera");
	if (pFreeCamera) {
		// Add a controller modifier so we can look around the camera by using a default control
		pFreeCamera->AddModifier("PLEngine::SNMEgoLookController");

		// Add a controller modifier so we can move around the camera by using a default control
		pFreeCamera->AddModifier("PLEngine::SNMMoveController");
	}

	// If primary scene node given, add an orbiter camera scene node named "OrbiterCamera"
	SceneNode *pOrbitingCamera = pPrimarySceneNode ? cSceneContainer.Create("PLScene::SNCamera", "OrbiterCamera") : nullptr;
	if (pOrbitingCamera) {
		// Get the axis align bounding box (in 'scene container space') from the primary scene node
		const AABoundingBox &cAABoundingBox = pPrimarySceneNode->GetContainerAABoundingBox();

		// Get orbiting distance, use the axis align bounding box (in 'scene container space') from the primary scene node as hint
		const float fDistance = cAABoundingBox.GetLongestAxisLength()*2;

		// Calculate the orbiting offset, use the axis align bounding box (in 'scene container space') from the primary scene node as hint
		const Vector3 vOffset = cAABoundingBox.GetCenter();

		// Add a controller modifier so we can orbiting around the camera by using a default control
		pOrbitingCamera->AddModifier("PLEngine::SNMEgoOrbitingController", "Target=\"" + pPrimarySceneNode->GetAbsoluteName() + "\" Distance=\"" + fDistance + "\" Offset=\"" + vOffset.ToString() + '\"');

		// Give the free camera the same position and rotation as used for the orbiting camera
		if (pFreeCamera)
			pFreeCamera->GetTransform() = pOrbitingCamera->GetTransform();
	}

	// Return the preferred camera scene node
	return reinterpret_cast<SNCamera*>(pOrbitingCamera ? pOrbitingCamera : pFreeCamera);
}

/**
*  @brief
*    Configures a scene
*/
SNCamera *SceneCreator::ConfigureScene(SceneContainer &cSceneContainer, const String &sClass, const String &sParameters)
{
	// Check parameter
	if (sClass.GetLength()) {
		// To keep things as fast as possible we store a pointer to the base class
		// (no hash map operation + quite efficient internal string comparison)
		static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLEngine::SceneCreator");
		if (pBaseClass) {
			const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
			if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
				// Create the scene creator instance
				SceneCreator *pSceneCreator = static_cast<SceneCreator*>(pClass->Create());
				if (pSceneCreator) {
					// In case parameters are provided, set them
					if (sParameters.GetLength())
						pSceneCreator->SetValues(sParameters);

					// Configure the scene
					SNCamera *pSNCamera = pSceneCreator->Create(cSceneContainer);

					// Destroy the scene creator instance
					delete pSceneCreator;

					// Return the preferred camera scene node
					return pSNCamera;
				}
			}
		}
	}

	// Error
	return nullptr;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SceneCreator::SceneCreator()
{
}

/**
*  @brief
*    Destructor
*/
SceneCreator::~SceneCreator()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
