/*********************************************************\
 *  File: SceneRendererPass.cpp                          *
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
#include <PLRenderer/RendererContext.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Compositing/SceneRenderer.h"
#include "PLScene/Compositing/SceneRendererManager.h"
#include "PLScene/Compositing/SceneRendererPass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneRendererPass)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
uint32 SceneRendererPass::GetFlags() const
{
	return m_nFlags;
}

void SceneRendererPass::SetFlags(uint32 nValue)
{
	// Set the new flags
	m_nFlags = nValue;

	// Update active state
	SetActive(!(m_nFlags & Inactive));
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the scene context the scene renderer pass is in
*/
SceneContext *SceneRendererPass::GetSceneContext() const
{
	// Get the owner scene renderer
	SceneRenderer *pSceneRenderer = static_cast<SceneRenderer*>(GetManager());
	if (pSceneRenderer) {
		return &pSceneRenderer->GetSceneRendererManager().GetSceneContext();
	} else {
		// Error!
		return nullptr;
	}
}

/**
*  @brief
*    Returns the used renderer
*/
Renderer *SceneRendererPass::GetRenderer() const
{
	// Get the owner scene context
	SceneContext *pSceneContext = GetSceneContext();

	// Return the used renderer
	return pSceneContext ? &pSceneContext->GetRendererContext().GetRenderer() : nullptr;
}

/**
*  @brief
*    Returns the first found instance of a scene renderer pass instance within the scene renderer by using a class name
*/
SceneRendererPass *SceneRendererPass::GetFirstInstanceOfSceneRendererPassClass(const String &sClassName) const
{
	// Get the scene renderer we're in
	SceneRenderer *pSceneRenderer = static_cast<SceneRenderer*>(GetManager());
	if (pSceneRenderer) {
		// Loop through all scene renderer pass instances within the scene renderer
		for (uint32 i=0; i<pSceneRenderer->GetNumOfElements(); i++) {
			// Get the pass and check it's class name
			SceneRendererPass *pPass = pSceneRenderer->GetByIndex(i);
			if (pPass && pPass->IsInstanceOf(sClassName)) {
				// Done
				return pPass; 
			}
		}
	}

	// Error, not found!
	return nullptr;
}

/**
*  @brief
*    Is the scene renderer pass active?
*/
bool SceneRendererPass::IsActive() const
{
	return !(m_nFlags & Inactive);
}

/**
*  @brief
*    Sets whether the scene renderer pass is active or not
*/
void SceneRendererPass::SetActive(bool bActive)
{
	// Set the flag
	if (bActive)
		m_nFlags &= ~Inactive;
	else
		m_nFlags |=  Inactive;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneRendererPass::SceneRendererPass() :
	Flags(this),
	Name(this),
	m_nFlags(0)
{
}

/**
*  @brief
*    Destructor
*/
SceneRendererPass::~SceneRendererPass()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
