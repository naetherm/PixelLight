/*********************************************************\
 *  File: EffectPass.cpp                                 *
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
#include <PLCore/Tools/LoadableManager.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/Program.h"
#include "PLRenderer/Renderer/VertexShader.h"
#include "PLRenderer/Renderer/ShaderLanguage.h"
#include "PLRenderer/Renderer/FragmentShader.h"
#include "PLRenderer/Material/Parameter.h"
#include "PLRenderer/Material/ParameterManager.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Effect/EffectManager.h"
#include "PLRenderer/Effect/EffectTechnique.h"
#include "PLRenderer/Effect/EffectPassLayer.h"
#include "PLRenderer/Effect/EffectPass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Binds the pass
*/
bool EffectPass::Bind(ParameterManager *pParameterManager)
{
	// Get renderer
	Renderer &cRenderer = m_pTechnique->GetEffect().GetEffectManager().GetRendererContext().GetRenderer();

	// Set render states
	for (uint32 i=0; i<RenderState::ScissorTestEnable; i++)
		cRenderer.SetRenderState(static_cast<RenderState::Enum>(i), m_cRenderStates.Get(static_cast<RenderState::Enum>(i)));

	// Fixed functions
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		// Set render states
		for (uint32 i=0; i<FixedFunctions::RenderState::Number; i++)
			pFixedFunctions->SetRenderState(static_cast<FixedFunctions::RenderState::Enum>(i), m_cFixedFunctionsRenderStates.Get(static_cast<FixedFunctions::RenderState::Enum>(i)));
		pFixedFunctions->SetColor(m_cColor);

		// Set material states
		for (uint32 i=0; i<FixedFunctions::MaterialState::Number; i++)
			pFixedFunctions->SetMaterialState(static_cast<FixedFunctions::MaterialState::Enum>(i), m_nMaterialState[i]);
	}

	{ // Bind shaders
		// Get the used GPU program
		Program *pProgram = GetProgram();

		// If there's a GPU program, make it to the currently used one
		cRenderer.SetProgram(pProgram);
		if (pProgram) {
			// Set shader parameters
			if (pParameterManager) {
				// First, use the given parameter manager
				for (uint32 i=0; i<pParameterManager->GetNumOfParameters(); i++) {
					// Get parameter
					const Parameter *pParameter = pParameterManager->GetParameter(i);
					if (pParameter && pParameter->GetType() != Parameters::TextureBuffer) 
						pParameter->SetManagerParameterValue(*pProgram, pParameter->GetName());
				}

				// Use the effect parameters if not already set by the given parameter manager
				const uint32 nNumOfParameters = GetTechnique().GetEffect().GetParameterManager().GetNumOfParameters();
				for (uint32 i=0; i<nNumOfParameters; i++) {
					// Get parameter, ignore texture buffer parameters
					const Parameter *pParameter = GetTechnique().GetEffect().GetParameterManager().GetParameter(i);
					if (pParameter && pParameter->GetType() != Parameters::TextureBuffer) {
						// Already set by the given parameter manager?
						const String sName = pParameter->GetName();
						if (!pParameterManager->IsParameter(sName))
							pParameter->SetManagerParameterValue(*pProgram, sName);
					}
				}
			} else {
				const uint32 nNumOfParameters = GetTechnique().GetEffect().GetParameterManager().GetNumOfParameters();
				for (uint32 i=0; i<nNumOfParameters; i++) {
					// Get parameter
					const Parameter *pParameter = GetTechnique().GetEffect().GetParameterManager().GetParameter(i);
					if (pParameter && pParameter->GetType() != Parameters::TextureBuffer)
						pParameter->SetManagerParameterValue(*pProgram, pParameter->GetName());
				}
			}
		}
	}

	{ // Setup textures
		uint32 i;

		if (m_lstLayers.GetNumOfElements()) {
			for (i=0; i<m_lstLayers.GetNumOfElements(); i++)
				m_lstLayers[i]->Bind(i, pParameterManager);
		} else {
			// Fixed functions
			if (pFixedFunctions)
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::Texture0, Matrix4x4::Identity);
			i = 0;
		}
		for (; i<static_cast<uint32>(cRenderer.GetCapabilities().nMaxTextureUnits); i++) {
			cRenderer.SetTextureBuffer(i);

			// Set sampler states
			for (uint32 nState=0; nState<Sampler::Number; nState++)
				cRenderer.SetSamplerState(i, static_cast<Sampler::Enum>(nState), cRenderer.GetDefaultSamplerState(static_cast<Sampler::Enum>(nState)));

			// Fixed functions
			if (pFixedFunctions) {
				pFixedFunctions->SetTransformState(static_cast<FixedFunctions::Transform::Enum>(static_cast<uint32>(FixedFunctions::Transform::Texture0) + i), Matrix4x4::Identity);

				// Set texture stage states
				for (uint32 nState=0; nState<FixedFunctions::TextureStage::Number; nState++)
					pFixedFunctions->SetTextureStageState(i, static_cast<FixedFunctions::TextureStage::Enum>(nState), pFixedFunctions->GetDefaultTextureStageState(static_cast<FixedFunctions::TextureStage::Enum>(nState)));
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
EffectPass &EffectPass::operator =(const EffectPass &cSource)
{
	// Copy pass name
	m_sName = cSource.GetName();

	// Copy general color
	m_cColor = cSource.m_cColor;

	// Copy render states
	m_cRenderStates = cSource.m_cRenderStates;

	// Copy material states
	for (uint32 i=0; i<FixedFunctions::MaterialState::Number; i++)
		m_nMaterialState[i] = cSource.GetMaterialState(static_cast<FixedFunctions::MaterialState::Enum>(i));

	// Layers
	for (uint32 i=0; i<m_lstLayers.GetNumOfElements(); i++)
		delete m_lstLayers[i];
	m_lstLayers.Clear();
	for (uint32 i=0; i<cSource.GetNumOfLayers(); i++) {
		EffectPassLayer *pFXPassLayer = new EffectPassLayer(*this);
		*pFXPassLayer = *cSource.GetLayer(i);
		m_lstLayers.Add(pFXPassLayer);
	}

	/*
	// [TODO] New shader interface
	// Vertex shader
	if (cSource.GetVertexShader()) {
		if (!m_pVertexShader)
			m_pVertexShader = new ShaderHandler();
		m_pVertexShader->SetResource(cSource.GetVertexShader()->GetResource());
	} else {
		if (m_pVertexShader) {
			delete m_pVertexShader;
			m_pVertexShader = nullptr;
		}
	}

	// Fragment shader
	if (cSource.GetFragmentShader()) {
		if (!m_pFragmentShader)
			m_pFragmentShader = new ShaderHandler();
		m_pFragmentShader->SetResource(cSource.GetFragmentShader()->GetResource());
	} else {
		if (m_pFragmentShader) {
			delete m_pFragmentShader;
			m_pFragmentShader = nullptr;
		}
	}
*/
	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ States                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Resets all material states to default
*/
void EffectPass::ResetMaterialStates()
{
	// Get renderer
	Renderer &cRenderer = m_pTechnique->GetEffect().GetEffectManager().GetRendererContext().GetRenderer();

	// Get the default material state settings
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		for (uint32 i=0; i<FixedFunctions::MaterialState::Number; i++)
			m_nMaterialState[i] = pFixedFunctions->GetDefaultMaterialState(static_cast<FixedFunctions::MaterialState::Enum>(i));
	} else {
		for (uint32 i=0; i<FixedFunctions::MaterialState::Number; i++)
			m_nMaterialState[i] = 0;
	}
}


//[-------------------------------------------------------]
//[ Layers                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a new layer
*/
EffectPassLayer *EffectPass::AddLayer()
{
	EffectPassLayer *ppFXPassLayer = new EffectPassLayer(*this);
	m_lstLayers.Add(ppFXPassLayer);
	return ppFXPassLayer;
}

/**
*  @brief
*    Removes a layer
*/
bool EffectPass::RemoveLayer(uint32 nIndex)
{
	EffectPassLayer *pEffectPassLayer = m_lstLayers[nIndex];
	if (!pEffectPassLayer)
		return false; // Error!
	delete pEffectPassLayer;
	m_lstLayers.RemoveAtIndex(nIndex);

	// Done
	return true;
}

/**
*  @brief
*    Removes all layers
*/
void EffectPass::RemoveAllLayers()
{
	// Delete layers
	for (uint32 i=0; i<m_lstLayers.GetNumOfElements(); i++)
		delete m_lstLayers[i];
	m_lstLayers.Clear();
}


//[-------------------------------------------------------]
//[ Shaders                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loads a vertex shader
*/
bool EffectPass::LoadVertexShader(const String &sFilename, const String &sShaderLanguage, const String &sProfile)
{
	// Check parameters
	if (sFilename.GetLength()) {
		// Destroy old shader
		if (m_pVertexShader) {
			delete m_pVertexShader;
			m_pVertexShader = nullptr;
		}

		// Get the shader language to use
		ShaderLanguage *pShaderLanguage = m_pTechnique->GetEffect().GetEffectManager().GetRendererContext().GetRenderer().GetShaderLanguage(sShaderLanguage);
		if (pShaderLanguage) {
			// Load in the shader source code
			const String sShaderSourceCode = LoadableManager::GetInstance()->LoadStringFromFile(sFilename);
			if (sShaderSourceCode.GetLength()) {
				// Create the shader instances
				m_pVertexShader = pShaderLanguage->CreateVertexShader(sShaderSourceCode, sProfile);
				if (m_pVertexShader) {
					// Destroy the current GPU program
					if (m_pProgram) {
						delete m_pProgram;
						m_pProgram = nullptr;
					}

					// Done
					return true;
				}
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Loads a fragment shader
*/
bool EffectPass::LoadFragmentShader(const String &sFilename, const String &sShaderLanguage, const String &sProfile)
{
	// Check parameters
	if (sFilename.GetLength()) {
		// Destroy old shader
		if (m_pFragmentShader) {
			delete m_pFragmentShader;
			m_pFragmentShader = nullptr;
		}

		// Get the shader language to use
		ShaderLanguage *pShaderLanguage = m_pTechnique->GetEffect().GetEffectManager().GetRendererContext().GetRenderer().GetShaderLanguage(sShaderLanguage);
		if (pShaderLanguage) {
			// Load in the shader source code
			const String sShaderSourceCode = LoadableManager::GetInstance()->LoadStringFromFile(sFilename);
			if (sShaderSourceCode.GetLength()) {
				// Create the shader instances
				m_pFragmentShader = pShaderLanguage->CreateFragmentShader(sShaderSourceCode, sProfile);
				if (m_pFragmentShader) {
					// Destroy the current GPU program
					if (m_pProgram) {
						delete m_pProgram;
						m_pProgram = nullptr;
					}

					// Done
					return true;
				}
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the used GPU program
*/
Program *EffectPass::GetProgram()
{
	// Create the GPU program right now?
	if (!m_pProgram && m_pVertexShader && m_pFragmentShader) {
		// Get the shader language to use
		ShaderLanguage *pShaderLanguage = m_pVertexShader->GetRenderer().GetShaderLanguage(m_pVertexShader->GetShaderLanguage());
		if (pShaderLanguage) {
			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pProgram = pShaderLanguage->CreateProgram(m_pVertexShader, m_pFragmentShader);
		}
	}

	// Done
	return m_pProgram;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
EffectPass::EffectPass(EffectTechnique &cTechnique) :
	m_pTechnique(&cTechnique),
	m_sName("Pass"),
	m_cColor(Color4::White),
	m_pVertexShader(nullptr),
	m_pFragmentShader(nullptr),
	m_pProgram(nullptr)
{
	ResetMaterialStates();
}

/**
*  @brief
*    Destructor
*/
EffectPass::~EffectPass()
{
	// Remove all layers
	RemoveAllLayers();

	// Delete shaders
	if (m_pProgram)
		delete m_pProgram;
	if (m_pVertexShader)
		delete m_pVertexShader;
	if (m_pFragmentShader)
		delete m_pFragmentShader;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
