/*********************************************************\
 *  File: QPLSceneContext.cpp                            *
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
#include <PLCore/Log/Log.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/SurfacePainter.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SNCamera.h>
#include <PLScene/Scene/SceneQuery.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodes/SNKeyValue.h>
#include <PLScene/Scene/SceneQueries/SQEnumerate.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/QPLSceneContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLFrontendQt {


QPLSceneContext::QPLSceneContext(const QString &sPainterName, RendererContext *pRendererContext) : QPLContext(sPainterName, pRendererContext),
	m_pSceneContext(nullptr),
	m_pRootScene(nullptr),
	m_pScene(nullptr),
	m_pCamera(nullptr),
	EventHandlerSceneNode(&QPLSceneContext::OnSceneNode, this),
	m_pFirstFoundCamera(nullptr),
	m_sSceneRenderer("Forward.sr")
{
}

QPLSceneContext::~QPLSceneContext()
{
	if (m_pSceneContext)
		delete m_pSceneContext;
}

void QPLSceneContext::Update()
{
	// Update the root scene container - we need to do this before drawing the first frame
	UpdateSceneContext();
	QPLContext::Update();
}

SurfacePainter *QPLSceneContext::CreateSurfacePainter()
{
	SurfacePainter *pPainter = QPLContext::CreateSurfacePainter();
	if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene") && m_pRootScene && m_pScene) {
		SPScene *pSPScene = static_cast<SPScene*>(pPainter);
		pSPScene->SetRootContainer(m_pRootScene);
		pSPScene->SetSceneContainer(m_pScene);
		if (m_pCamera)
			pSPScene->SetCamera(m_pCamera);
	}

	return pPainter;
}

SceneContext *QPLSceneContext::GetSceneContext()
{
	return m_pSceneContext;
}

SceneContainer *QPLSceneContext::GetScene()
{
	return m_pScene;
}

SceneContainer *QPLSceneContext::GetRootScene()
{
	return m_pRootScene;
}

SceneNode *QPLSceneContext::GetNodeFromScene(const String &nodeName)
{
	SceneContainer *pScene = GetScene();
	return pScene ? pScene->GetByName(nodeName) : nullptr;
}

void QPLSceneContext::SetCamera(SNCamera *pCamera)
{
	m_pCamera = pCamera;
}

void QPLSceneContext::InitScene()
{
	RendererContext *pContext = GetRendererContext();
	if (pContext) {
		m_pSceneContext = new SceneContext(*pContext);
		CreateRootScene();
	} else {
		PL_LOG(Error, "RenderContext not created")
	}
}

void QPLSceneContext::UpdateSceneContext()
{
	if (m_pSceneContext) {
		m_pSceneContext->Cleanup();
		m_pSceneContext->Update();
	}
}

void QPLSceneContext::CreateRootScene()
{
	if (m_pSceneContext) {
		// First, create the scene root container which holds the scene container with our 'concrete' scene within it
		SceneContainer *pRootContainer = m_pSceneContext->GetRoot() ? (SceneContainer*)m_pSceneContext->GetRoot()->Create("PLScene::SceneContainer", "RootScene") : nullptr;
		if (pRootContainer) {
			// Protect this important container!
			pRootContainer->SetProtected(true);

			// Create a scene container with our 'concrete scene'
			SceneNode *pSceneContainerNode = pRootContainer->Create("PLScene::SceneContainer", "Scene");
			if (pSceneContainerNode && pSceneContainerNode->IsContainer()) {
				// Protect this important container!
				pSceneContainerNode->SetProtected(true);

				// Init Scene
				OnCreateScene((SceneContainer&)*pSceneContainerNode);
			}
		}

		// Set the root scene
		SetRootScene(pRootContainer);
	}
}

void QPLSceneContext::SetRootScene(SceneContainer *pRootScene)
{
	// Set pointer to painter
	m_pRootScene = pRootScene;
}

bool QPLSceneContext::LoadSceneFromFile(const QString &sSceneFile)
{
	// Get the scene container holding our scene
	SceneContainer *pContainer = GetScene();
	if (!pContainer)
		return false; // Error! (should NEVER happen...)

	// Emit a signal that a context change starts
	emit QPLContext::BeginContextChange();

	// Clear the old scene
	pContainer->Clear();

	// Cleanup the scene context right now to ensure that all 'delete this'-marked scene nodes are really gone!
	// If this is not done, we may get problems with for example the names of dynamic textures because there may
	// occur name conflicts if multiple render-to-texture scene containers want to render into a same named texture...
	// Topics like 'the new scene is using resources that are already loaded in' must be handled on another level, e.g.
	// by delaying the unloading of currently unreferenced resources.
	if (m_pSceneContext)
		m_pSceneContext->Cleanup();

	// Load the scene
	bool bResult = pContainer->LoadByFilename(QtStringAdapter::QtToPL(sSceneFile));
	if (!bResult) {
		emit QPLContext::EndContextChange();
		return bResult;
	}

	// Reset vars, otherwise a already destroyed camera might be used
	m_pFirstFoundCamera = nullptr;
	m_sStartCamera = '\0';

	// Assign the first found camera scene node to your surface listener and look for
	// known key/value data scene nodes
	SQEnumerate *pSceneQuery = static_cast<SQEnumerate*>(pContainer->CreateQuery("PLScene::SQEnumerate"));
	if (pSceneQuery) {
		// Connect event handler
		pSceneQuery->SignalSceneNode.Connect(EventHandlerSceneNode);

		// Perform the query
		pSceneQuery->PerformQuery();

		// Destroy the query
		pContainer->DestroyQuery(*pSceneQuery);
	}

	// Is there a given start camera?
	SceneNode *pCamera = nullptr;
	if (m_sStartCamera.GetLength()) {
		SceneNode *pStartCamera = pContainer->GetByName(m_sStartCamera);
		if (pStartCamera && pStartCamera->IsCamera())
			pCamera = pStartCamera;
	}

	// Use the first found camera?
	if (!pCamera)
		pCamera = m_pFirstFoundCamera;

	// Activate the current used camera by default
	if (pCamera)
		pCamera->SetActive(true);

	// Assign this camera to the scene renderer and to the application
	SetCamera(reinterpret_cast<SNCamera*>(pCamera));

	// Emit signal that context was changed
	emit QPLContext::ContextChanged();

	// Emit signal that the context change is done
	emit QPLContext::EndContextChange();

	return bResult;
}

void QPLSceneContext::SetSceneRenderer(const String &sSceneRenderer)
{
	m_sSceneRenderer = sSceneRenderer;
}
const String &QPLSceneContext::GetSceneRenderer()
{
	return m_sSceneRenderer;
}

void QPLSceneContext::OnSceneNode(SceneQuery &cQuery, SceneNode &cSceneNode)
{
	// Is this a camera?
	if (cSceneNode.IsCamera()) {
		if (!m_pFirstFoundCamera)
			m_pFirstFoundCamera = &cSceneNode;

		// Deactivate all camera's by default
		cSceneNode.SetActive(false);

	// Key/value data scene node?
	} else if (cSceneNode.IsInstanceOf("PLScene::SNKeyValue")) {
		const SNKeyValue &cKeyValue = static_cast<const SNKeyValue&>(cSceneNode);

		// SceneRenderer
		/* if (cKeyValue.Key.GetString() == "SceneRenderer") {
			m_sDefaultSceneRenderer = cKeyValue.Value.GetString();

		// SceneRendererVariables
		} else if (cKeyValue.Key.GetString() == "SceneRendererVariables") {
			m_lstPostKeys.Add(&cKeyValue);

		// ClearColor
		} else if (cKeyValue.Key.GetString() == "ClearColor") {
			// Get scene surface painter
			SurfacePainter *pPainter = GetPainter();
			if (pPainter && pPainter->IsInstanceOf("PLScene::SPScene")) {
				SceneRenderer *pSceneRenderer = static_cast<SPScene*>(pPainter)->GetDefaultSceneRenderer();
				if (pSceneRenderer) {
					SceneRendererPass *pSceneRendererPass = pSceneRenderer->Get("Begin");
					if (pSceneRendererPass)
						pSceneRendererPass->SetAttribute("ColorClear", cKeyValue.Value.GetString());
				}
			}

		// StartCamera
		} else*/ if (cKeyValue.Key.Get() == "StartCamera") {
			m_sStartCamera = cKeyValue.Value.Get();
		}

	// Load screen scene node?
	} /*else if (cSceneNode.IsInstanceOf("PLEngine::SNLoadScreenBase")) {
		m_bHasLoadScreen = true;
	}*/
}

void QPLSceneContext::PLSceneContextCleanUp()
{
	if(m_pSceneContext)
		m_pSceneContext->Cleanup();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
