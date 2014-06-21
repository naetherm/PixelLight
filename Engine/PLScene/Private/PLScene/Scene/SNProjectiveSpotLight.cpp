/*********************************************************\
 *  File: SNProjectiveSpotLight.cpp                      *
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
#include "PLScene/Scene/SNProjectiveSpotLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNProjectiveSpotLight)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNProjectiveSpotLight::GetProjectedMaterial() const
{
	return m_sProjectedMaterial;
}

void SNProjectiveSpotLight::SetProjectedMaterial(const String &sValue)
{
	if (m_sProjectedMaterial != sValue) {
		m_sProjectedMaterial = sValue;
		m_bProjectedMaterial = false;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNProjectiveSpotLight::SNProjectiveSpotLight() :
	ProjectedMaterial(this),
	Flags(this),
	m_sProjectedMaterial("Data/Textures/PLLogo.dds"),
	m_bProjectedMaterial(false)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoCone);
}

/**
*  @brief
*    Destructor
*/
SNProjectiveSpotLight::~SNProjectiveSpotLight()
{
}

/**
*  @brief
*    Returns the handler of the projected material
*/
const MaterialHandler &SNProjectiveSpotLight::GetProjectedMaterialHandler()
{
	if (!m_bProjectedMaterial) {
		m_cProjectedMaterial.SetResource(GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(m_sProjectedMaterial));
		m_bProjectedMaterial = true;
	}
	return m_cProjectedMaterial;
}


//[-------------------------------------------------------]
//[ Public virtual SNLight functions                      ]
//[-------------------------------------------------------]
bool SNProjectiveSpotLight::IsProjectiveSpotLight() const
{
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
