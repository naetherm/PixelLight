/*********************************************************\
 *  File: Material.cpp                                   *
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
#include <PLCore/File/Url.h>
#include <PLCore/Tools/LoadableManager.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Material/Parameter.h"
#include "PLRenderer/Texture/Texture.h"
#include "PLRenderer/Texture/TextureHandler.h"
#include "PLRenderer/Effect/EffectTechnique.h"
#include "PLRenderer/Effect/EffectManager.h"
#include "PLRenderer/Material/MaterialManager.h"
#include "PLRenderer/Material/ParameterManager.h"
#include "PLRenderer/Material/MaterialHandler.h"
#include "PLRenderer/Material/Material.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String Material::DiffuseMap			= "DiffuseMap";
const String Material::LightMap				= "LightMap";
const String Material::AmbientOcclusionMap	= "AmbientOcclusionMap";
const String Material::EmissiveMap			= "EmissiveMap";
const String Material::ReflectionMap		= "ReflectionMap";
const String Material::ReflectivityMap		= "ReflectivityMap";
const String Material::NormalMap			= "NormalMap";
const String Material::SpecularMap			= "SpecularMap";
const String Material::HeightMap			= "HeightMap";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Material::~Material()
{
	// First at all: Unload the material
	Unload();

	// Destroy the parameter manager
	delete m_pParameterManager;
}

/**
*  @brief
*    Copy operator
*/
Material &Material::operator =(const Material &cSource)
{
	// [TODO] Copy parameters
//	*m_pParameterManager = *cSource.m_pParameterManager;

	// Copy materials
	RemoveAllMaterials();
	for (uint32 i=0; i<cSource.m_lstMaterials.GetNumOfElements(); i++) {
		Material *pMaterial = cSource.m_lstMaterials.Get(i)->GetResource();
		if (pMaterial)
			AddMaterial(*pMaterial);
	}

	// Copy effect
	SetEffect(cSource.GetEffect());

	// Done
	return *this;
}

/**
*  @brief
*    Gets a material/effect parameter
*/
Parameter *Material::GetParameter(const String &sName) const
{
	// Get the parameter
	Parameter *pParameter = m_pParameterManager->GetParameter(sName);
	if (pParameter)
		return pParameter;
	else {
		// Check materials
		for (uint32 i=0; i<m_lstMaterials.GetNumOfElements(); i++) {
			const Material *pMaterial = m_lstMaterials.Get(i)->GetResource();
			if (pMaterial) {
				pParameter = pMaterial->GetParameter(sName);
				if (pParameter)
					return pParameter;
			}
		}

		// Check effect
		return GetEffect() ? GetEffect()->GetParameterManager().GetParameter(sName) : nullptr;
	}
}

/**
*  @brief
*    Gets a texture buffer
*/
TextureBuffer *Material::GetParameterTextureBuffer(const String &sName) const
{
	// Get the parameter
	const Parameter *pParameter = GetParameter(sName);
	if (pParameter) {
		// Get the texture
		const Texture *pTexture = pParameter->GetValueTexture();
		if (pTexture)
			return pTexture->GetTextureBuffer();
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Reloads all textures used direct or indirect by the material
*/
bool Material::ReloadTextures()
{
	bool bResult = true; // No error by default

	// Are there any parameters?
	uint32 nNumOfParameters = m_pParameterManager->GetNumOfParameters();
	if (nNumOfParameters) {
		// Loop through all parameters and collect the textures we have to reload. In this
		// way we avoid reloading the same texture multiple times if it is used by multiple parameters. :)
		List<Texture*> lstTextures;
		for (uint32 i=0; i<nNumOfParameters; i++) {
			// Get the parameter
			const Parameter *pParameter = m_pParameterManager->GetParameter(i);

			// Is this a texture?
			if (pParameter->GetType() == ParameterManager::TextureBuffer) {
				// Get the texture
				Texture *pTexture = pParameter->GetValueTexture();
				if (pTexture) {
					// Is this texture already within the list?
					if (!lstTextures.IsElement(pTexture))
						lstTextures.Add(pTexture); // Nope, add it right now!
				}
			}
		}

		// We need to collect the textures of the used effect, too
		const Effect *pFX = GetEffect();
		if (pFX && pFX->GetParameterManager().GetNumOfParameters()) {
			nNumOfParameters = pFX->GetParameterManager().GetNumOfParameters();
			for (uint32 i=0; i<nNumOfParameters; i++) {
				// Get the parameter
				const Parameter *pParameter = pFX->GetParameterManager().GetParameter(i);

				// Is this a texture?
				if (pParameter->GetType() == ParameterManager::TextureBuffer) {
					// Get the texture
					Texture *pTexture = pParameter->GetValueTexture();
					if (pTexture) {
						// Is this texture already within the list?
						if (!lstTextures.IsElement(pTexture))
							lstTextures.Add(pTexture); // Nope, add it right now!
					}
				}
			}
		}

		// Reload the collected textures
		Iterator<Texture*> cIterator = lstTextures.GetIterator();
		while (cIterator.HasNext()) {
			if (!cIterator.Next()->Reload())
				bResult = false; // Error!
		}
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Adds a material
*/
bool Material::AddMaterial(Material &cMaterial)
{
	MaterialHandler *pMaterialHandler = new MaterialHandler();
	pMaterialHandler->SetResource(&cMaterial);
	if (m_lstMaterials.Add(pMaterialHandler)) {
		// Connect event handler
		cMaterial.EventParameterChanged.Connect(EventHandlerParameterChanged);

		// Done
		return true;
	} else {
		// Cleanup
		delete pMaterialHandler;

		// Error!
		return false;
	}
}

/**
*  @brief
*    Returns a material
*/
Material *Material::GetMaterial(uint32 nIndex) const
{
	const MaterialHandler *pMaterialHandler = m_lstMaterials.Get(nIndex);
	return pMaterialHandler ? pMaterialHandler->GetResource() : nullptr;
}

/**
*  @brief
*    Removes a material
*/
bool Material::RemoveMaterial(uint32 nIndex)
{
	// Get the material handler
	MaterialHandler *pMaterialHandler = m_lstMaterials.Get(nIndex);
	if (pMaterialHandler) {
		// Disconnect event handler
		Material *pMaterial = pMaterialHandler->GetResource();
		if (pMaterial)
			pMaterial->EventParameterChanged.Disconnect(EventHandlerParameterChanged);

		// Remove the material from the list
		m_lstMaterials.RemoveAtIndex(nIndex);
		delete pMaterialHandler;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Removes all materials
*/
bool Material::RemoveAllMaterials()
{
	// Loop through all material handlers
	for (uint32 i=0; i<m_lstMaterials.GetNumOfElements(); i++) {
		// Get the material handler
		MaterialHandler *pMaterialHandler = m_lstMaterials.Get(i);
		if (pMaterialHandler) {
			// Disconnect event handler
			Material *pMaterial = pMaterialHandler->GetResource();
			if (pMaterial)
				pMaterial->EventParameterChanged.Disconnect(EventHandlerParameterChanged);

			// Destroy the material handler
			delete pMaterialHandler;
		}
	}
	m_lstMaterials.Clear();

	// Done
	return true;
}

/**
*  @brief
*    Returns the effect of the material
*/
Effect *Material::GetEffect() const
{
	return m_pFXHandler ? static_cast<Effect*>(m_pFXHandler->GetResource()) : nullptr;
}

/**
*  @brief
*    Sets the effect of the material
*/
void Material::SetEffect(Effect *pFX)
{
	if (pFX) {
		if (!m_pFXHandler)
			m_pFXHandler = new EffectHandler();
		m_pFXHandler->SetResource(pFX);
	} else {
		if (m_pFXHandler) {
			delete m_pFXHandler;
			m_pFXHandler = nullptr;
		}
	}
}

/**
*  @brief
*    Returns whether the material should be blend or not
*/
bool Material::GetBlend() const
{
	const Effect *pFX = GetEffect();
	return (pFX && pFX->GetBlend());
}

/**
*  @brief
*    Gets the number of render passes the current technique is using
*/
uint32 Material::GetNumOfPasses() const
{
	const Effect *pFX = GetEffect();
	if (pFX) {
		const EffectTechnique *pTechnique = pFX->GetSelectedTechnique();
		if (pTechnique)
			return pTechnique->GetNumOfPasses();
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Setup the given path
*/
bool Material::SetupPass(uint32 nIndex) const
{
	const Effect *pFX = GetEffect();
	if (pFX) {
		const EffectTechnique *pTechnique = pFX->GetSelectedTechnique();
		if (pTechnique)
			pTechnique->SetupPass(nIndex, m_pParameterManager);
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Material::Material(MaterialManager &cManager, const String &sName) : PLCore::Resource<Material>(sName, &cManager),
	EventHandlerParameterChanged(&Material::OnParameterChanged, this),
	m_pParameterManager(new ParameterManager(cManager.GetRendererContext())),
	m_pFXHandler(nullptr)
{
	// Connect event handler
	m_pParameterManager->EventParameterChanged.Connect(EventHandlerParameterChanged);
}

/**
*  @brief
*    Called when a parameter has been changed (created, destroyed, value changed)
*/
void Material::OnParameterChanged(Parameter &cParameter)
{
	// Emit event (=> pass on the event)
	EventParameterChanged(cParameter);
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Material::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Unload old data
	Unload();

	// Get the filename extension for material/effect detection
	// -> There are file formats without an extension, so no extension must also be valid
	const String sExtension = Url(sFilename).GetExtension();

	// Material file given?
	if (LoadableManager::GetInstance()->IsFormatLoadSupported(sExtension, "Material")) {
		// Call base implementation, m_sAbsFilename is set automatically :)
		return PLCore::Resource<Material>::LoadByFilename(sFilename, sParams, sMethod);

	// Effect given?
	} else if (LoadableManager::GetInstance()->IsFormatLoadSupported(sExtension, "Effect")) {
		// Load the effect
		Effect *pFX = GetMaterialManager().GetRendererContext().GetEffectManager().LoadResource(sFilename);
		if (pFX) {
			// Backup the absolute filename of the effect
			m_sUrl = pFX->GetUrl();

			// Set this effect
			SetEffect(pFX);

			// Done
			return true;
		} else {
			// Error!
			return false;
		}
	}

	// If there's no extension, maybe it's an automatic texture?
	if (sFilename.Compare("Create ", 0, 7)) {
		// Create a 'DiffuseMap' texture parameter
		Parameter *pParameter = m_pParameterManager->CreateParameter(ParameterManager::TextureBuffer, DiffuseMap);
		if (pParameter)
			pParameter->SetValueTexture(sFilename);

		// Just backup the given 'filename'
		m_sUrl = sFilename;

		// Done
		return true;
	}

	// Still there? Must be a simple texture... please note that it is NOT a good idea to check to perform a check like this:
	//  } else if (LoadableManager::GetInstance()->IsFormatLoadSupported(sExtension, "Image") ||
	//             LoadableManager::GetInstance()->IsFormatLoadSupported(sExtension, "TextureAni")) {
	// -> because then your're artificially limited to only 'supported' file formats and have for example problems
	// when using dynamic texture that have no or no known 'filename extension'!

	// Create a 'DiffuseMap' texture parameter
	Parameter *pParameter = m_pParameterManager->CreateParameter(ParameterManager::TextureBuffer, DiffuseMap);
	if (pParameter && pParameter->SetValueTexture(sFilename)) {
		const Texture *pTexture = pParameter->GetValueTexture();
		if (pTexture) {
			// Backup the absolute filename of the texture
			m_sUrl = pTexture->GetUrl();
			
			// Done
			return true;
		}
	} else {
		// Error!
		PL_LOG(Error, '\'' + sFilename + "' is no known texture!")
	}

	// Cleanup on error
	m_pParameterManager->RemoveAllParameters();

	// Error!
	return false;
}

bool Material::Reload()
{
	// Call base implementation
	if (!PLCore::Resource<Material>::Reload())
		return false; // Error!

	// Reload textures
	return ReloadTextures();
}

bool Material::Unload()
{
	// Cleanup the material
	RemoveAllMaterials();
	SetEffect();
	m_pParameterManager->RemoveAllParameters();

	// Call base implementation
	return PLCore::Resource<Material>::Unload();
}

String Material::GetLoadableTypeName() const
{
	static const String sString = "Material";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
