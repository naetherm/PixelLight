/*********************************************************\
 *  File: SceneRendererTool.cpp                          *
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
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/SurfacePainter.h>
#include <PLScene/Compositing/SceneRenderer.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContainer.h>
#include "PLEngine/Tools/SceneRendererTool.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(SceneRendererTool)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneRendererTool::SceneRendererTool(SurfacePainter *pSurfacePainter) :
	m_pSurfacePainter(pSurfacePainter)
{
}

/**
*  @brief
*    Destructor
*/
SceneRendererTool::~SceneRendererTool()
{
}

/**
*  @brief
*    Get surface painter
*/
SurfacePainter *SceneRendererTool::GetPainter() const
{
	// Return surface painter
	return m_pSurfacePainter;
}

/**
*  @brief
*    Set surface painter
*/
void SceneRendererTool::SetPainter(SurfacePainter *pSurfacePainter)
{
	// Set surface painter
	m_pSurfacePainter = pSurfacePainter;
}

/**
*  @brief
*    Returns the used scene renderer instance
*/
SceneRenderer *SceneRendererTool::GetSceneRenderer() const
{
	// Get the painter
	if (m_pSurfacePainter && m_pSurfacePainter->IsInstanceOf("PLScene::SPScene")) {
		// Return the default scene renderer
		return static_cast<SPScene*>(m_pSurfacePainter)->GetDefaultSceneRenderer();
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Sets the used scene renderer
*/
bool SceneRendererTool::SetSceneRenderer(SceneContainer *pSceneContainer, const String &sFilename, const String &sFallbackFilename)
{
	// Is there a surface painter and is it an "SPScene"-instance?
	if (pSceneContainer && m_pSurfacePainter && m_pSurfacePainter->IsInstanceOf("PLScene::SPScene")) {
		// Get and setup the "SPScene"-instance of the surface painter
		SPScene *pSPScene = static_cast<SPScene*>(m_pSurfacePainter);
		pSPScene->SetRootContainer(pSceneContainer->GetContainer());
		pSPScene->SetSceneContainer(pSceneContainer);

		// Check renderer API and version number, if legacy hardware is used we can only
		// use a quite primitive scene renderer!
		String sSceneRendererFilename = sFilename;
		{
			// [TODO] Detect required capabilities of a scene renderer? (important)

			// OpenGL
			uint32 nVersion = 0;
			if (m_pSurfacePainter->GetRenderer().GetAPI(&nVersion) == "OpenGL") {
				if (nVersion < 14) {
					sSceneRendererFilename = sFallbackFilename;
					PL_LOG(Warning, "Your graphics card is too old to support proper shader rendering. "
									"At least a OpenGL 1.4 compatible graphics card is recommended. A fallback fill be used.")
				}

			// OpenGL ES 2.0
			} else if (m_pSurfacePainter->GetRenderer().GetAPI(&nVersion) == "OpenGL ES 2.0") {
				// [TODO] Remove this build in tests
				if (sSceneRendererFilename == "Deferred.sr" || sSceneRendererFilename == "FixedFunctions.sr")
					sSceneRendererFilename = "Forward.sr";

			// Direct3D
			} else if (m_pSurfacePainter->GetRenderer().GetAPI(&nVersion) == "Direct3D") {
				if (nVersion < 900) {
					sSceneRendererFilename = sFallbackFilename;
					PL_LOG(Warning, "Your graphics card is too old to support proper shader rendering. "
									"At least a Direct3D 9 compatible graphics card is recommended. A fallback fill be used.")
				}
			}
		}

		// Set the desired default scene renderer, but first, set to 'none' so we have default settings
		// within our default scene renderer
		pSPScene->SetDefaultSceneRenderer("");
		pSPScene->SetDefaultSceneRenderer(sSceneRendererFilename);

		// Check the default scene renderer, maybe someone gave us an 'invalid' one...
		if (!pSPScene->GetDefaultSceneRenderer()) {
			// Jap, it's an 'invalid' one -> write an error into the log...
			PL_LOG(Error, "Failed to set scene renderer '" + sFilename + "'! '" + sFallbackFilename + "' is used as fallback.")

			// ... and use instead an usually always available very basic scene renderer
			pSPScene->SetDefaultSceneRenderer(sFallbackFilename);
		}

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the number of scene renderer passes
*/
uint32 SceneRendererTool::GetNumOfPasses() const
{
	// Get the scene renderer
	SceneRenderer *pSceneRenderer = GetSceneRenderer();
	if (pSceneRenderer) {
		// Return the requested scene renderer pass
		return pSceneRenderer->GetNumOfElements();
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Gets a scene renderer pass by index
*/
SceneRendererPass *SceneRendererTool::GetPassByIndex(uint32 nIndex) const
{
	// Get the scene renderer
	SceneRenderer *pSceneRenderer = GetSceneRenderer();
	if (pSceneRenderer) {
		// Return the requested scene renderer pass
		return pSceneRenderer->GetByIndex(nIndex);
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Gets a scene renderer pass by name
*/
SceneRendererPass *SceneRendererTool::GetPassByName(const String &sName) const
{
	// Get the scene renderer
	SceneRenderer *pSceneRenderer = GetSceneRenderer();
	if (pSceneRenderer) {
		// Return the requested scene renderer pass
		return pSceneRenderer->GetByName(sName);
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Gets a scene renderer pass attribute
*/
DynVar *SceneRendererTool::GetPassAttribute(const String &sSceneRendererPassName, const String &sAttributeName) const
{
	// Get the scene renderer pass
	SceneRendererPass *pSceneRendererPass = GetPassByName(sSceneRendererPassName);
	if (pSceneRendererPass) {
		// Get the attribute
		return pSceneRendererPass->GetAttribute(sAttributeName);
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Sets a scene renderer pass attribute value using a string
*/
bool SceneRendererTool::SetPassAttribute(const String &sSceneRendererPassName, const String &sAttributeName, const String &sValue) const
{
	// Get the scene renderer pass
	SceneRendererPass *pSceneRendererPass = GetPassByName(sSceneRendererPassName);
	if (pSceneRendererPass) {
		// Get the attribute
		DynVar *pDynVar = pSceneRendererPass->GetAttribute(sAttributeName);
		if (pDynVar) {
			// Set the value of the attribute using a string
			pDynVar->SetString(sValue);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Sets scene renderer pass attribute values using a string
*/
uint32 SceneRendererTool::SetAttribute(const String &sAttributeName, const String &sValue) const
{
	uint32 nNumOfSetAttributes = 0;

	// Get the scene renderer
	SceneRenderer *pSceneRenderer = GetSceneRenderer();
	if (pSceneRenderer) {
		// Loop through all scene renderer passes
		for (uint32 nPass=0; nPass<pSceneRenderer->GetNumOfElements(); nPass++) {
			// Get the current scene renderer pass
			SceneRendererPass *pSceneRendererPass = pSceneRenderer->GetByIndex(nPass);
			if (pSceneRendererPass) {
				// Get the attribute
				DynVar *pDynVar = pSceneRendererPass->GetAttribute(sAttributeName);
				if (pDynVar) {
					// Set the value of the attribute using a string
					pDynVar->SetString(sValue);

					// Update the number of set attributes
					nNumOfSetAttributes++;
				}
			}
		}
	}

	// Done
	return nNumOfSetAttributes;
}

/**
*  @brief
*    Sets scene renderer pass attribute values using a string
*/
void SceneRendererTool::SetValues(const String &sValues) const
{
	// Get the scene renderer
	SceneRenderer *pSceneRenderer = GetSceneRenderer();
	if (pSceneRenderer) {
		// Loop through all scene renderer passes
		for (uint32 nPass=0; nPass<pSceneRenderer->GetNumOfElements(); nPass++) {
			// Get the current scene renderer pass
			SceneRendererPass *pSceneRendererPass = pSceneRenderer->GetByIndex(nPass);
			if (pSceneRendererPass) {
				// Set the values
				pSceneRendererPass->SetValues(sValues);
			}
		}
	}
}

/**
*  @brief
*    Sets all scene renderer pass attribute values to their default value
*/
void SceneRendererTool::SetDefaultValues() const
{
	// Get the scene renderer
	SceneRenderer *pSceneRenderer = GetSceneRenderer();
	if (pSceneRenderer) {
		// Loop through all scene renderer passes
		for (uint32 nPass=0; nPass<pSceneRenderer->GetNumOfElements(); nPass++) {
			// Get the current scene renderer pass
			SceneRendererPass *pSceneRendererPass = pSceneRenderer->GetByIndex(nPass);
			if (pSceneRendererPass) {
				// Set the values
				pSceneRendererPass->SetDefaultValues();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
