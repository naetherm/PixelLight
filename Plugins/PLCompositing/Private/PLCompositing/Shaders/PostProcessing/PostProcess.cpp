/*********************************************************\
 *  File: PostProcess.cpp                                *
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
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLCompositing/Shaders/PostProcessing/PostProcessManager.h"
#include "PLCompositing/Shaders/PostProcessing/PostProcess.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PostProcess::PostProcess(const String &sName, PostProcessManager &cManager) :
	Element<PostProcess>(sName, &cManager),
	m_bActive(true),
	m_pMaterialHandler(new MaterialHandler())
{
	// Load material
	if (sName.GetLength())
		m_pMaterialHandler->SetResource(cManager.GetRendererContext().GetMaterialManager().LoadResource(sName));
}

/**
*  @brief
*    Destructor
*/
PostProcess::~PostProcess()
{
	// Destroy material handler
	delete m_pMaterialHandler;
}

/**
*  @brief
*    Returns whether the post process is active or not
*/
bool PostProcess::IsActive() const
{
	return m_bActive;
}

/**
*  @brief
*    Sets whether the post process is active or not
*/
void PostProcess::SetActive(bool bActive)
{
	m_bActive = bActive;
}

/**
*  @brief
*    Returns the post process material
*/
Material *PostProcess::GetMaterial() const
{
	return m_pMaterialHandler->GetResource();
}

/**
*  @brief
*    Sets the post process material
*/
void PostProcess::SetMaterial(Material *pMaterial)
{
	m_pMaterialHandler->SetResource(pMaterial);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
