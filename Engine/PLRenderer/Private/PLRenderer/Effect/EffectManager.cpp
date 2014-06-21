/*********************************************************\
 *  File: EffectManager.cpp                              *
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
#include "PLRenderer/Effect/EffectTechnique.h"
#include "PLRenderer/Effect/EffectHandler.h"
#include "PLRenderer/Effect/EffectManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String EffectManager::Default = "Default";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Reloads all effects
*/
bool EffectManager::ReloadEffects()
{
	bool bResult = true; // No error by default

	for (uint32 i=0; i<m_lstResources.GetNumOfElements(); i++) {
		Effect *pFX = m_lstResources[i];
		if (!pFX->Reload())
			bResult = false; // Error!
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Uses a given effect
*/
bool EffectManager::Use(const String &sName)
{
	Effect *pFX = LoadResource(sName);
	if (pFX) {
		const EffectTechnique *pTechnique = pFX->GetSelectedTechnique();
		if (pTechnique)
			pTechnique->SetupPass(0);

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
EffectManager::EffectManager(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext),
	m_pDefault(new EffectHandler())
{
	PL_LOG(Debug, "Create effect manager")

	// Set manager name
	SetManagerName("Effect manager");

	// Unload unused resources, please
	SetUnloadUnused(true);

	// Create the standard effect
	Effect *pFX = CreateResource(Default);
	if (pFX) {
		// Create the effect
		EffectTechnique *pEffectTechnique = pFX->AddTechnique();
		if (pEffectTechnique) {
			pEffectTechnique->AddPass();
			pFX->SelectTechnique(0);
		}

		// Setup effect
		pFX->SetProtected(true);
		m_pDefault->SetResource(pFX);
		SetStandard(pFX);
	}
}

/**
*  @brief
*    Destructor
*/
EffectManager::~EffectManager()
{
	PL_LOG(Debug, "Destroy effect manager")

	// Cleanup
	delete m_pDefault;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ResourceManager functions     ]
//[-------------------------------------------------------]
Effect *EffectManager::CreateResource(const String &sName)
{
	return new Effect(*this, sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
