/*********************************************************\
 *  File: SNProjectivePointLight.cpp                     *
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
#include "PLScene/Scene/SNProjectivePointLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNProjectivePointLight, "PLScene", PLScene::SNPointLight, "Omni directional projective point light scene node")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(ProjectedMaterial,	PLCore::String,										"Data/Textures/DefaultCubeMap.dds",	ReadWrite,	"Projected material",	"Type='Material Effect Image TextureAni'")
		// Overwritten SceneNode attributes
	pl_attribute_metadata(Flags,				pl_flag_type_def3(SNProjectivePointLight, EFlags),	0,									ReadWrite,	"Flags",				"")
pl_class_metadata_end(SNProjectivePointLight)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNProjectivePointLight::GetProjectedMaterial() const
{
	return m_sProjectedMaterial;
}

void SNProjectivePointLight::SetProjectedMaterial(const String &sValue)
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
SNProjectivePointLight::SNProjectivePointLight() :
	ProjectedMaterial(this),
	Flags(this),
	m_sProjectedMaterial("Data/Textures/DefaultCubeMap.dds"),
	m_bProjectedMaterial(false)
{
}

/**
*  @brief
*    Destructor
*/
SNProjectivePointLight::~SNProjectivePointLight()
{
}

/**
*  @brief
*    Returns the handler of the projected material
*/
const MaterialHandler &SNProjectivePointLight::GetProjectedMaterialHandler()
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
bool SNProjectivePointLight::IsProjectivePointLight() const
{
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
