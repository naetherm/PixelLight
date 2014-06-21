/*********************************************************\
 *  File: RendererContext.cpp                            *
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
#include <PLCore/Tools/Stopwatch.h>
#include <PLCore/Tools/Profiling.h>
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Effect/EffectManager.h"
#include "PLRenderer/Material/MaterialManager.h"
#include "PLRenderer/RendererContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a renderer context instance
*/
RendererContext *RendererContext::CreateInstance(const String &sBackend, handle nNativeWindowHandle, Renderer::EMode nMode, uint32 nZBufferBits, uint32 nStencilBits, uint32 nMultisampleAntialiasingSamples, const String &sDefaultShaderLanguage)
{
	// Create renderer instance
	const Class *pClass = ClassManager::GetInstance()->GetClass(sBackend);
	if (pClass && pClass->IsDerivedFrom("PLRenderer::Renderer")) {
		Object *pObject = pClass->Create(Params<Object*, handle, pl_enum_type(Renderer::EMode), uint32, uint32, uint32, String>(nNativeWindowHandle, nMode, nZBufferBits, nStencilBits, nMultisampleAntialiasingSamples, sDefaultShaderLanguage));
		if (pObject) {
			// Cast to renderer instance
			Renderer *pRenderer = static_cast<Renderer*>(pObject);

			// Check whether or not the renderer instance was initialized successfully within it's constructor
			if (pRenderer->IsInitialized())
				return &pRenderer->GetRendererContext();
			else
				delete pRenderer;	// It's worthless to us, get rid of it!
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
RendererContext::~RendererContext()
{
	// Destroy the renderer instance and all resource managers
	if (m_pMaterialManager)
		delete m_pMaterialManager;
	if (m_pEffectManager)
		delete m_pEffectManager;
	if (m_pTextureManager)
		delete m_pTextureManager;
	if (m_pRenderer)
		delete m_pRenderer;
}

/**
*  @brief
*    Returns the texture manager of this renderer context
*/
TextureManager &RendererContext::GetTextureManager()
{
	if (!m_pTextureManager)
		m_pTextureManager = new TextureManager(*this);
	return *m_pTextureManager;
}

/**
*  @brief
*    Returns the effect manager of this renderer context
*/
EffectManager &RendererContext::GetEffectManager()
{
	if (!m_pEffectManager)
		m_pEffectManager = new EffectManager(*this);
	return *m_pEffectManager;
}

/**
*  @brief
*    Returns the material manager of this renderer context
*/
MaterialManager &RendererContext::GetMaterialManager()
{
	if (!m_pMaterialManager)
		m_pMaterialManager = new MaterialManager(*this);
	return *m_pMaterialManager;
}

/**
*  @brief
*    Updates the render context
*/
void RendererContext::Update()
{
	// Start stopwatch
	Stopwatch cStopwatch(true);

	// Emit the update event
	EventUpdate();

	// Update renderer
	if (m_pRenderer)
		m_pRenderer->Update();

	// Update profiling information
	Profiling *pProfiling = Profiling::GetInstance();
	if (pProfiling->IsActive()) {
		if (m_pTextureManager)
			pProfiling->Set("Renderer context", "Number of textures", m_pTextureManager->GetNumOfElements());
		if (m_pEffectManager)
			pProfiling->Set("Renderer context", "Number of effects", m_pEffectManager->GetNumOfElements());
		if (m_pMaterialManager)
			pProfiling->Set("Renderer context", "Number of materials", m_pMaterialManager->GetNumOfElements());
		pProfiling->Set("Renderer context", "Update time", String::Format("%.3g ms", cStopwatch.GetMilliseconds()));
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RendererContext::RendererContext(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_pTextureManager(nullptr),
	m_pEffectManager(nullptr),
	m_pMaterialManager(nullptr)
{
}

/**
*  @brief
*    Copy operator
*/
RendererContext &RendererContext::operator =(const RendererContext &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
