/*********************************************************\
 *  File: SceneRenderer.cpp                              *
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
#include <PLCore/Base/Class.h>
#include "PLScene/Compositing/SceneRendererManager.h"
#include "PLScene/Compositing/SceneRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the scene renderer manager this scene renderer is in
*/
SceneRendererManager &SceneRenderer::GetSceneRendererManager() const
{
	// There MUST always be a manager!
	return static_cast<SceneRendererManager&>(*m_pManager);
}

/**
*  @brief
*    Creates a new scene node
*/
SceneRendererPass *SceneRenderer::Create(const String &sClass, const String &sName, const String &sParameters)
{
	// Check parameter
	if (sClass.GetLength()) {
		// To keep things as fast as possible we store a pointer to the base class
		// (no hash map operation + quite efficient internal string comparison)
		static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLScene::SceneRendererPass");

		// Get scene node plugin
		if (pBaseClass) {
			const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
			if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
				PL_LOG(Debug, "Create scene renderer pass '" + sName + "' of type '" + sClass + '\'')
				SceneRendererPass *pPass = static_cast<SceneRendererPass*>(pClass->Create());
				if (pPass) {
					if (sParameters.GetLength())
						pPass->SetValues(sParameters);
					pPass->SetName(sName);
					Add(*pPass);
				}

				// Return the created scene renderer pass
				return pPass;
			} else { // Plugin not found
				PL_LOG(Error, "Unknown scene renderer pass type: " + sClass)
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
*    Constructor
*/
SceneRenderer::SceneRenderer(SceneRendererManager &cManager, const String &sName) : PLCore::Resource<SceneRenderer>(sName, &cManager)
{
}

/**
*  @brief
*    Destructor
*/
SceneRenderer::~SceneRenderer()
{
	// Emit scene renderer destroyed event
	EventDestroyed();
}


//[-------------------------------------------------------]
//[ Private virtual SceneRenderer functions               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws the scene using this scene renderer
*/
void SceneRenderer::DrawScene(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Draw all scene renderer passes
	for (uint32 i=0; i<GetNumOfElements(); i++) {
		// Get the current pass
		SceneRendererPass *pPass = GetByIndex(i);

		// Do ONLY take it into account if it's valid and active
		if (pPass && pPass->IsActive())
			pPass->Draw(cRenderer, cCullQuery);
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool SceneRenderer::Unload()
{
	// Cleanup
	Clear();

	// Call base implementation
	return PLCore::Resource<SceneRenderer>::Unload();
}

String SceneRenderer::GetLoadableTypeName() const
{
	static const String sString = "SceneRenderer";
	return sString;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
SceneRendererPass *SceneRenderer::CreateElement(const String &sName)
{
	return Create(sName, "");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
