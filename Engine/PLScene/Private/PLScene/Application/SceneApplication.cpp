/*********************************************************\
 *  File: SceneApplication.cpp                           *
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
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Application/SceneApplication.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneApplication)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneApplication::SceneApplication(Frontend &cFrontend, const String &sSceneFilename) : RendererApplication(cFrontend, "PLScene::SPScene"),
	m_pSceneContext(nullptr),
	m_pRootScene(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
SceneApplication::~SceneApplication()
{
}

/**
*  @brief
*    Returns the scene context
*/
SceneContext *SceneApplication::GetSceneContext() const
{
	return m_pSceneContext;
}

/**
*  @brief
*    Returns the root scene
*/
SceneContainer *SceneApplication::GetRootScene() const
{
	// Return pointer to root scene container
	return m_pRootScene;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Set root scene
*/
void SceneApplication::SetRootScene(SceneContainer *pRootScene)
{
	// Set pointer to painter
	m_pRootScene = pRootScene;
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractLifecycle functions ]
//[-------------------------------------------------------]
bool SceneApplication::OnStart()
{
	// Call base implementation
	if (RendererApplication::OnStart()) {
		// Get renderer context
		RendererContext *pRendererContext = GetRendererContext();
		if (pRendererContext) {
			// Create scene context
			m_pSceneContext = new SceneContext(*pRendererContext);

			// Create root scene
			OnCreateRootScene();
			if (!m_bRunning)
				return false;

			// Done
			return true;
		}
	}

	// Error
	return false;
}

void SceneApplication::OnStop()
{
	// Destroy the scene context
	if (m_pSceneContext) {
		delete m_pSceneContext;
		m_pSceneContext = nullptr;
	}

	// Call base implementation
	RendererApplication::OnStop();
}


//[-------------------------------------------------------]
//[ Protected virtual PLCore::AbstractFrontend functions  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called to let the frontend update it's states
*/
void SceneApplication::OnUpdate()
{
	// Call base implementation
	RendererApplication::OnUpdate();

	// Update the root scene container - we need to do this before drawing the first frame
	if (m_pSceneContext) {
		m_pSceneContext->Cleanup();

		// Update the scene context
		m_pSceneContext->Update();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual SceneApplication functions          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Function that is called to create the application's root scene
*/
void SceneApplication::OnCreateRootScene()
{
	// [TODO] Load 'm_sSceneFilename' if provided

	// Is there a scene context?
	if (m_pSceneContext) {
		// First, create the scene root container which holds the scene container with our 'concrete' scene within it
		SceneContainer *pRootContainer = m_pSceneContext->GetRoot() ? static_cast<SceneContainer*>(m_pSceneContext->GetRoot()->Create("PLScene::SceneContainer", "RootScene")) : nullptr;
		if (pRootContainer) {
			// Protect this important container!
			pRootContainer->SetProtected(true);

			// Create a scene container with our 'concrete scene'
			SceneNode *pSceneContainerNode = pRootContainer->Create("PLScene::SceneContainer", "Scene");
			if (pSceneContainerNode && pSceneContainerNode->IsContainer()) {
				// Protect this important container!
				pSceneContainerNode->SetProtected(true);
			}
		}

		// Set the root scene
		SetRootScene(pRootContainer);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
