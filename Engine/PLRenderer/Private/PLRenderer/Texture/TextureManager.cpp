/*********************************************************\
 *  File: TextureManager.cpp                             *
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
#include <PLCore/Tools/Profiling.h>
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/ResourceHandler.h"
#include "PLRenderer/Texture/Creator/TextureCreator.h"
#include "PLRenderer/Texture/Texture.h"
#include "PLRenderer/Texture/TextureManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String TextureManager::Default = "Data/Textures/Default.dds";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the texture quality
*/
void TextureManager::SetTextureQuality(float fQuality)
{
	m_fTextureQuality = fQuality;

	// Ensure the value is inside the specified range
	if (m_fTextureQuality < 0.0f)
		m_fTextureQuality = 0.0f;
	else if (m_fTextureQuality > 1.0f)
		m_fTextureQuality = 1.0f;
}

/**
*  @brief
*    Reloads all textures
*/
bool TextureManager::ReloadTextures()
{
	bool bResult = true; // No error by default

	// Reload every texture
	for (uint32 i=0; i<m_lstResources.GetNumOfElements(); i++) {
		Texture *pTexture = m_lstResources[i];
		if (!pTexture->LoadByFilename(pTexture->GetName()))
			bResult = false; // Error!
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Creates a new texture
*/
Texture *TextureManager::CreateTexture(const String &sName, TextureBuffer &cTextureBuffer)
{
	// Create the texture instance
	Texture *pTexture = new Texture(*this, sName);

	// The new texture is just sharing the given texture buffer, so when the new texture get's destroyed the given texture buffer stays alive
	cTextureBuffer.AddHandler(*pTexture->m_pTextureBufferHandler);
	pTexture->m_bShareTextureBuffer = true;

	// Done
	return pTexture;
}

/**
*  @brief
*    Creates a texture resource using a texture creator
*/
Texture *TextureManager::CreateTexture(const String &sName, const String &sParameters)
{
	// To keep things as fast as possible we store a pointer to the base class
	// (no hash map operation + quite efficient internal string comparison)
	static const Class *pBaseClass = ClassManager::GetInstance()->GetClass("PLRenderer::TextureCreator");

	// Create the texture creator
	if (pBaseClass) {
		const Class *pClass = ClassManager::GetInstance()->GetClass(sName);
		if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
			TextureCreator *pTC = static_cast<TextureCreator*>(pClass->Create());
			if (pTC) {
				// Set parameters
				if (sParameters.GetLength())
					pTC->SetValues(sParameters);

				// Create the texture
				Texture *pTexture = pTC->Create(*this, nullptr);

				// Destroy the texture creator
				delete pTC;

				// Return the created texture
				return pTexture;
			}
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureManager::TextureManager(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext),
	m_fTextureQuality(1.0f),
	m_bTextureFitLower(true),
	m_bTextureMipmapsAllowed(true),
	m_bTextureCompressionAllowed(true)
{
	PL_LOG(Debug, "Create texture manager")

	// Set manager name
	SetManagerName("Texture manager");

	// Unload unused resources, please
	SetUnloadUnused(true);

	// Load the standard texture
	Texture *pTexture = LoadResource(Default);
	if (pTexture) {
		pTexture->SetProtected(true);
		SetStandard(pTexture);
	}
}

/**
*  @brief
*    Destructor
*/
TextureManager::~TextureManager()
{
	PL_LOG(Debug, "Destroy texture manager")

	// Clear all textures
	Clear();
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ResourceManager functions     ]
//[-------------------------------------------------------]
Texture *TextureManager::CreateResource(const String &sName)
{
	return new Texture(*this, sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
