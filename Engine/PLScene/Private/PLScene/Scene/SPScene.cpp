/*********************************************************\
 *  File: SPScene.cpp                                    *
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
#include <PLRenderer/Renderer/Renderer.h>
#include "PLScene/Scene/SNCamera.h"
#include "PLScene/Scene/SceneQuery.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Scene/SceneQueryHandler.h"
#include "PLScene/Compositing/SceneRenderer.h"
#include "PLScene/Compositing/SceneRendererHandler.h"
#include "PLScene/Compositing/SceneRendererManager.h"
#include "PLScene/Visibility/SQCull.h"
#include "PLScene/Scene/SPScene.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SPScene)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPScene::SPScene(Renderer &cRenderer) : SurfacePainter(cRenderer),
	m_pRootContainerHandler(new SceneNodeHandler()),
	m_pSceneContainerHandler(new SceneNodeHandler()),
	m_pCullQuery(new SceneQueryHandler()),
	m_pCameraNodeHandler(new SceneNodeHandler()),
	m_sDefaultSceneRenderer("Forward.sr"),
	m_pDefaultSceneRendererHandler(new SceneRendererHandler())
{
}

/**
*  @brief
*    Destructor
*/
SPScene::~SPScene()
{
	// Destroy scene root container handler
	delete m_pRootContainerHandler;

	// Destroy scene container handler
	delete m_pSceneContainerHandler;

	// Destroy the cull query
	if (m_pCullQuery) {
		SceneQuery *pSceneQuery = m_pCullQuery->GetElement();
		if (pSceneQuery)
			pSceneQuery->GetSceneContainer().DestroyQuery(*pSceneQuery);
		delete m_pCullQuery;
	}

	// Destroy camera scene node handler
	delete m_pCameraNodeHandler;

	// Destroy the default scene renderer handler
	delete m_pDefaultSceneRendererHandler;
}

/**
*  @brief
*    Returns the used scene context
*/
SceneContext *SPScene::GetSceneContext() const
{
	SceneContainer *pSceneContainer = static_cast<SceneContainer*>(m_pRootContainerHandler->GetElement());
	return pSceneContainer ? pSceneContainer->GetSceneContext() : nullptr;
}

/**
*  @brief
*    Returns the scene root container
*/
SceneContainer *SPScene::GetRootContainer() const
{
	return static_cast<SceneContainer*>(m_pRootContainerHandler->GetElement());
}

/**
*  @brief
*    Sets the scene root container
*/
bool SPScene::SetRootContainer(SceneContainer *pContainer)
{
	return m_pRootContainerHandler->SetElement(pContainer);
}

/**
*  @brief
*    Returns the scene container
*/
SceneContainer *SPScene::GetSceneContainer() const
{
	return static_cast<SceneContainer*>(m_pSceneContainerHandler->GetElement());
}

/**
*  @brief
*    Sets the scene container
*/
bool SPScene::SetSceneContainer(SceneContainer *pContainer)
{
	// Anything to do in here?
	if (pContainer == GetSceneContainer())
		return true; // Done

	// Destroy the old cull query
	if (m_pCullQuery->GetElement()) {
		SceneQuery *pSceneQuery = m_pCullQuery->GetElement();
		if (pSceneQuery)
			pSceneQuery->GetSceneContainer().DestroyQuery(*pSceneQuery);
	}

	// Create cull query
	SQCull *pCullQuery = static_cast<SQCull*>(m_pCullQuery->GetElement());
	if (!pCullQuery && pContainer) {
		pCullQuery = static_cast<SQCull*>(pContainer->CreateQuery("PLScene::SQCull"));
		if (pCullQuery) {
			m_pCullQuery->SetElement(pCullQuery);

			// Set the cull mode
			pCullQuery->SetMode(SQCull::Frustum);

			// We do not need to inform any scene query listeners
			pCullQuery->SetFlags(0);
		}
	}

	// Set container
	return m_pSceneContainerHandler->SetElement(pContainer);
}

/**
*  @brief
*    Returns the default camera scene node
*/
SNCamera *SPScene::GetCamera() const
{
	return static_cast<SNCamera*>(m_pCameraNodeHandler->GetElement());
}

/**
*  @brief
*    Sets the default camera scene
*/
void SPScene::SetCamera(SNCamera *pCamera)
{
	m_pCameraNodeHandler->SetElement(pCamera);
}

/**
*  @brief
*    Returns the cull query
*/
SQCull *SPScene::GetCullQuery() const
{
	return static_cast<SQCull*>(m_pCullQuery->GetElement());
}

/**
*  @brief
*    Returns the default scene renderer
*/
SceneRenderer *SPScene::GetDefaultSceneRenderer() const
{
	// Get/load the scene renderer
	SceneRenderer *pSceneRenderer = m_pDefaultSceneRendererHandler->GetResource();
	if (!pSceneRenderer && m_sDefaultSceneRenderer.GetLength()) {
		// Get the root scene container
		SceneContainer *pRootContainer = GetRootContainer();
		if (pRootContainer) {
			// Get the scene context
			SceneContext *pSceneContext = pRootContainer->GetSceneContext();
			if (pSceneContext) {
				pSceneRenderer = pSceneContext->GetSceneRendererManager().LoadResource(m_sDefaultSceneRenderer);
				m_pDefaultSceneRendererHandler->SetResource(pSceneRenderer);
			}
		}
	}

	// Return the scene renderer
	return pSceneRenderer;
}

/**
*  @brief
*    Sets the default scene renderer
*/
void SPScene::SetDefaultSceneRenderer(const String &sSceneRenderer)
{
	// Set new scene renderer
	if (m_sDefaultSceneRenderer != sSceneRenderer) {
		m_sDefaultSceneRenderer = sSceneRenderer;
		m_pDefaultSceneRendererHandler->SetResource(nullptr);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Pre draw all scene nodes recursive
*/
void SPScene::DrawPre(Renderer &cRenderer, SceneContainer &cContainer)
{
	// Get the scene container (can be a null pointer)
	SceneContainer *pContainer = GetSceneContainer();

	// Draw parent container
	if (&cContainer != pContainer)
		cContainer.DrawPre(cRenderer);

	// Loop through all nodes
	for (uint32 i=0; i<cContainer.GetNumOfElements(); i++) {
		SceneNode *pNode = cContainer.GetByIndex(i);
		if (pNode != pContainer && pNode->IsVisible() && (pNode->GetDrawFunctionFlags() & SceneNode::UseDrawPre)) {
			if (pNode->IsContainer())
				DrawPre(cRenderer, static_cast<SceneContainer&>(*pNode));
			else
				pNode->DrawPre(cRenderer);
		}
	}
}

/**
*  @brief
*    Solid draw all scene nodes recursive
*/
void SPScene::DrawSolid(Renderer &cRenderer, SceneContainer &cContainer)
{
	// Get the scene container (can be a null pointer)
	SceneContainer *pContainer = GetSceneContainer();

	// Draw parent container
	if (&cContainer != pContainer)
		cContainer.DrawSolid(cRenderer);

	// Loop through all nodes
	for (uint32 i=0; i<cContainer.GetNumOfElements(); i++) {
		SceneNode *pNode = cContainer.GetByIndex(i);
		if (pNode != pContainer && pNode->IsVisible() && (pNode->GetDrawFunctionFlags() & SceneNode::UseDrawSolid)) {
			if (pNode->IsContainer())
				DrawSolid(cRenderer, static_cast<SceneContainer&>(*pNode));
			else
				pNode->DrawSolid(cRenderer);
		}
	}
}

/**
*  @brief
*    Transparent draw all scene nodes recursive
*/
void SPScene::DrawTransparent(Renderer &cRenderer, SceneContainer &cContainer)
{
	// Get the scene container (can be a null pointer)
	SceneContainer *pContainer = GetSceneContainer();

	// Draw parent container
	if (&cContainer != pContainer)
		cContainer.DrawTransparent(cRenderer);

	// Loop through all nodes
	for (uint32 i=0; i<cContainer.GetNumOfElements(); i++) {
		SceneNode *pNode = cContainer.GetByIndex(i);
		if (pNode != pContainer && pNode->IsVisible() && (pNode->GetDrawFunctionFlags() & SceneNode::UseDrawTransparent)) {
			if (pNode->IsContainer())
				DrawTransparent(cRenderer, static_cast<SceneContainer&>(*pNode));
			else
				pNode->DrawTransparent(cRenderer);
		}
	}
}

/**
*  @brief
*    Debug draw all scene nodes recursive
*/
void SPScene::DrawDebug(Renderer &cRenderer, SceneContainer &cContainer)
{
	// Get the scene container (can be a null pointer)
	SceneContainer *pContainer = GetSceneContainer();

	// Draw parent container
	if (&cContainer != pContainer && cContainer.GetDebugFlags() & SceneNode::DebugEnabled)
		cContainer.DrawDebug(cRenderer);

	// Loop through all nodes
	for (uint32 i=0; i<cContainer.GetNumOfElements(); i++) {
		SceneNode *pNode = cContainer.GetByIndex(i);
		if (pNode != pContainer && pNode->IsVisible() && (pNode->GetDrawFunctionFlags() & SceneNode::UseDrawDebug)) {
			if (pNode->IsContainer()) {
				DrawDebug(cRenderer, static_cast<SceneContainer&>(*pNode));
			} else {
				if (pNode->GetDebugFlags() & SceneNode::DebugEnabled)
					pNode->DrawDebug(cRenderer);
			}
		}
	}
}

/**
*  @brief
*    Post draw all scene nodes recursive
*/
void SPScene::DrawPost(Renderer &cRenderer, SceneContainer &cContainer)
{
	// Get the scene container (can be a null pointer)
	SceneContainer *pContainer = GetSceneContainer();

	// Draw parent container
	if (&cContainer != pContainer)
		cContainer.DrawPost(cRenderer);

	// Loop through all nodes
	for (uint32 i=0; i<cContainer.GetNumOfElements(); i++) {
		SceneNode *pNode = cContainer.GetByIndex(i);
		if (pNode != pContainer && pNode->IsVisible() && (pNode->GetDrawFunctionFlags() & SceneNode::UseDrawPost)) {
			if (pNode->IsContainer())
				DrawPost(cRenderer, static_cast<SceneContainer&>(*pNode));
			else
				pNode->DrawPost(cRenderer);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::SurfacePainter functions   ]
//[-------------------------------------------------------]
void SPScene::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Get camera
	SNCamera *pCamera = nullptr;
	SceneRenderer *pSceneRenderer = nullptr;
	if (m_pCameraNodeHandler->GetElement()) {
		pCamera = static_cast<SNCamera*>(m_pCameraNodeHandler->GetElement());
		if (pCamera)
			pSceneRenderer = pCamera->GetSceneRenderer();
	}
	SNCamera::SetCamera(pCamera, &cRenderer);

	// Check the used scene renderer
	if (!pSceneRenderer)
		pSceneRenderer = GetDefaultSceneRenderer();

	// Check the used scene renderer
	if (pSceneRenderer) {
		// Get the root scene container
		SceneContainer *pRootContainer = GetRootContainer();
		if (pRootContainer) {
			// Get the cull query
			SQCull *pCullQuery = static_cast<SQCull*>(m_pCullQuery->GetElement());
			if (pCullQuery) {
				// Set camera (can be a null pointer)
				if (pCamera) {
					// Setup render query
					SceneContainer *pCameraContainer = pCamera->GetContainer();
					pCullQuery->SetCameraContainer((pCameraContainer && pCameraContainer->IsCell()) ? pCameraContainer : nullptr);
					pCullQuery->SetCameraPosition(pCamera->GetTransform().GetPosition());
					pCullQuery->SetViewFrustum(pCamera->GetFrustum(cRenderer.GetViewport()));
					pCullQuery->SetProjectionMatrix(pCamera->GetProjectionMatrix(cRenderer.GetViewport()));
					pCullQuery->SetViewMatrix(pCamera->GetViewMatrix());
					pCullQuery->SetViewProjectionMatrix(pCullQuery->GetProjectionMatrix()*pCamera->GetViewMatrix());
				} else {
					// Set default states
					pCullQuery->SetCameraContainer(nullptr);
					pCullQuery->SetCameraPosition(Vector3::Zero);
					Matrix4x4 mProj;
					mProj.PerspectiveFov(static_cast<float>(90.0f*Math::DegToRad), 1.0f, 0.001f, 10000.0f);
					Frustum cFrustum;
					cFrustum.CreateViewPlanes(mProj, false);
					pCullQuery->SetViewFrustum(cFrustum);
				}

				// Perform the visibility determination
				pCullQuery->PerformQuery();
			}

			// Get the scene container (can be a null pointer)
			SceneContainer *pContainer = GetSceneContainer();

			// Pre all scene nodes
			DrawPre(cRenderer, *pRootContainer);

			// Draw all scene nodes solid
			DrawSolid(cRenderer, *pRootContainer);

			// Draw the scene container (if there's one)
			if (pContainer && pCullQuery)
				pSceneRenderer->DrawScene(cRenderer, *pCullQuery);

			// Draw all scene nodes transparent
			DrawTransparent(cRenderer, *pRootContainer);

			// Debug all scene nodes
			DrawDebug(cRenderer, *pRootContainer);

			// Post all scene nodes
			DrawPost(cRenderer, *pRootContainer);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
