/*********************************************************\
 *  File: SceneContext.cpp                               *
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
#include <PLCore/Tools/Timing.h>
#include <PLCore/Tools/Stopwatch.h>
#include <PLCore/Tools/Profiling.h>
#include <PLMath/Graph/GraphPathManager.h>
#include <PLRenderer/RendererContext.h>
#include <PLMesh/MeshManager.h>
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Visibility/VisManager.h"
#include "PLScene/Compositing/SceneRendererManager.h"
#include "PLScene/Scene/SceneContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneContext::SceneContext(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext),
	m_pMeshManager(nullptr),
	m_pGraphPathManager(nullptr),
	m_pRoot(new SceneNodeHandler()),
	m_pSceneRendererManager(nullptr),
	m_pVisManager(nullptr),
	m_bProcessActive(false),
	m_nProcessCounter(0)
{
}

/**
*  @brief
*    Destructor
*/
SceneContext::~SceneContext()
{
	// Get the root scene container (if there's one)
	SceneContainer *pRootContainer = static_cast<SceneContainer*>(m_pRoot->GetElement());
	if (pRootContainer) {
		// Destroy the root scene container in a safe way
		pRootContainer->Clear();
		Cleanup();
		delete pRootContainer;
	}
	Cleanup();
	delete m_pRoot;

	// Destroy the scene renderer manager
	if (m_pSceneRendererManager)
		delete m_pSceneRendererManager;

	// Destroy the visibility manager
	if (m_pVisManager)
		delete m_pVisManager;

	// Destroy the mesh manager
	if (m_pMeshManager)
		delete m_pMeshManager;

	// Destroy the graph path manager
	if (m_pGraphPathManager)
		delete m_pGraphPathManager;

	// We don't destroy 'm_pRendererContext' because we don't own it!
}

/**
*  @brief
*    Returns the used renderer context
*/
RendererContext &SceneContext::GetRendererContext() const
{
	return *m_pRendererContext;
}

/**
*  @brief
*    Returns the mesh manager
*/
MeshManager &SceneContext::GetMeshManager()
{
	if (!m_pMeshManager)
		m_pMeshManager = new MeshManager(m_pRendererContext->GetRenderer());
	return *m_pMeshManager;
}

/**
*  @brief
*    Returns the graph path manager
*/
GraphPathManager &SceneContext::GetGraphPathManager()
{
	if (!m_pGraphPathManager)
		m_pGraphPathManager = new GraphPathManager();
	return *m_pGraphPathManager;
}

/**
*  @brief
*    Returns the root of the scene graph
*/
SceneContainer *SceneContext::GetRoot()
{
	// Create the root scene container now?
	if (!m_pRoot->GetElement()) {
		const PLCore::Class *pClass = PLCore::ClassManager::GetInstance()->GetClass("PLScene::SceneContainer");
		if (pClass) {
			SceneContainer *pContainer = static_cast<SceneContainer*>(pClass->Create());
			m_pRoot->SetElement(pContainer);
			pContainer->m_pSceneContext = this;
			pContainer->SetProtected(true);
			pContainer->SetName("Root");
			pContainer->SetFlags(pContainer->GetFlags() | SceneNode::NoCulling);

			// Done
			return pContainer;
		} else {
			// WOW, something went TOTALLY wrong
			return nullptr;
		}
	}

	// Done
	return static_cast<SceneContainer*>(m_pRoot->GetElement());
}

/**
*  @brief
*    Performs a cleanup-operation (garbage collection)
*/
void SceneContext::Cleanup()
{
	if (m_lstDeleteNodes.GetNumOfElements()) {
		{
			Iterator<SceneNode*> cIterator = m_lstDeleteNodes.GetIterator();
			while (cIterator.HasNext()) {
				// Kill it!
				delete cIterator.Next();
			}
		}
		m_lstDeleteNodes.Clear();
	}
}

/**
*  @brief
*    Method that is called once per update loop
*/
void SceneContext::Update(bool bRespectPause)
{
	// Do only emit the update event when timing currently not paused
	if (!Timing::GetInstance()->IsPaused()) {
		// Perform profiling?
		Profiling *pProfiling = Profiling::GetInstance();
		if (pProfiling->IsActive()) {
			// Start the stopwatch
			Stopwatch cStopwatch(true);

			// Emit event
			EventUpdate();

			// Update the profiling data
			pProfiling->Set("Scene context", "Update time",		 String::Format("%.3f ms", cStopwatch.GetMilliseconds()));
			pProfiling->Set("Scene context", "Updated elements", String::Format("%d", EventUpdate.GetNumOfConnects()));
		} else {
			// Emit event
			EventUpdate();
		}
	}
}

/**
*  @brief
*    Returns the scene renderer manager
*/
SceneRendererManager &SceneContext::GetSceneRendererManager()
{
	if (!m_pSceneRendererManager)
		m_pSceneRendererManager = new SceneRendererManager(*this);
	return *m_pSceneRendererManager;
}

/**
*  @brief
*    Returns the visibility manager
*/
VisManager &SceneContext::GetVisManager()
{
	if (!m_pVisManager)
		m_pVisManager = new VisManager();
	return *m_pVisManager;
}

/**
*  @brief
*    Starts a scene process
*/
bool SceneContext::StartProcess()
{
	if (m_bProcessActive)
		return false; // Error, there's already a process running!
	else {
		m_bProcessActive = true;
		m_nProcessCounter++;

		// Done
		return true;
	}
}

/**
*  @brief
*    Checks whether a scene node was touched
*/
bool SceneContext::IsNodeTouched(SceneNode &cSceneNode) const
{
	return (cSceneNode.m_nCounter == m_nProcessCounter);
}

/**
*  @brief
*    Touches a scene node
*/
bool SceneContext::TouchNode(SceneNode &cSceneNode)
{
	if (m_bProcessActive) {
		if (cSceneNode.m_nCounter == m_nProcessCounter)
			return false; // Error, node was already processed!
		else {
			cSceneNode.m_nCounter = m_nProcessCounter;

			// Done
			return true;
		}
	} else {
		// Error, no valid scene node OR there's currently no process running!
		return false;
	}
}

/**
*  @brief
*    Ends a scene process
*/
bool SceneContext::EndProcess()
{
	if (m_bProcessActive) {
		m_bProcessActive = false;

		// Done
		return true;
	} else {
		// Error, there's currently no process running!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
