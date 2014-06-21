/*********************************************************\
 *  File: MaterialManager.cpp                            *
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
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Material/MaterialManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Reloads all materials
*/
bool MaterialManager::ReloadMaterials()
{
	bool bResult = true; // No error by default

	for (uint32 i=0; i<m_lstResources.GetNumOfElements(); i++) {
		Material *pMaterial = m_lstResources[i];
		if (!pMaterial->Reload())
			bResult = false; // Error!
	}

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MaterialManager::MaterialManager(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext)
{
	PL_LOG(Debug, "Create material manager")

	// Set manager name
	SetManagerName("Material manager");

	// Unload unused resources, please
	SetUnloadUnused(true);

	// Load the standard material
	const TextureManager &cTM = cRendererContext.GetTextureManager();
	Material *pMaterial = LoadResource(cTM.GetStandard() ? cTM.GetStandard()->GetFilename() : TextureManager::Default);
	if (!pMaterial)
		pMaterial = CreateResource("StandardMaterial");
	if (pMaterial) {
		pMaterial->SetProtected(true);
		SetStandard(pMaterial);
	}
}

/**
*  @brief
*    Destructor
*/
MaterialManager::~MaterialManager()
{
	PL_LOG(Debug, "Destroy material manager")
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ResourceManager functions     ]
//[-------------------------------------------------------]
Material *MaterialManager::CreateResource(const String &sName)
{
	return new Material(*this, sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
