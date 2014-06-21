/*********************************************************\
 *  File: SNEffectLight.cpp                              *
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
#include "PLScene/Scene/SNEffectLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNEffectLight)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNEffectLight::GetCoronaMaterial() const
{
	return m_sCoronaMaterial;
}

void SNEffectLight::SetCoronaMaterial(const String &sValue)
{
	if (m_sCoronaMaterial != sValue) {
		m_sCoronaMaterial = sValue;
		m_bCoronaMaterial = false;
	}
}

String SNEffectLight::GetFlareMaterial() const
{
	return m_sFlareMaterial;
}

void SNEffectLight::SetFlareMaterial(const String &sValue)
{
	if (m_sFlareMaterial != sValue) {
		m_sFlareMaterial = sValue;
		m_bFlareMaterial = false;
	}
}

String SNEffectLight::GetBlendMaterial() const
{
	return m_sBlendMaterial;
}

void SNEffectLight::SetBlendMaterial(const String &sValue)
{
	if (m_sBlendMaterial != sValue) {
		m_sBlendMaterial = sValue;
		m_bBlendMaterial = false;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNEffectLight::SNEffectLight() :
	CoronaMaterial(this),
	FlareMaterial(this),
	BlendMaterial(this),
	m_bCoronaMaterial(false),
	m_sCoronaMaterial("Data/Effects/DefaultCorona.plfx"),
	m_bFlareMaterial(false),
	m_sFlareMaterial("Data/Effects/DefaultFlare.plfx"),
	m_bBlendMaterial(false),
	m_sBlendMaterial("Data/Effects/DefaultBlend.plfx")
{
}

/**
*  @brief
*    Destructor
*/
SNEffectLight::~SNEffectLight()
{
}

/**
*  @brief
*    Returns the handler of the corona material
*/
const MaterialHandler &SNEffectLight::GetCoronaMaterialHandler()
{
	if (!m_bCoronaMaterial) {
		m_cCoronaMaterial.SetResource(GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(m_sCoronaMaterial));
		m_bCoronaMaterial = true;
	}
	return m_cCoronaMaterial;
}

/**
*  @brief
*    Returns the handler of the flare material
*/
const MaterialHandler &SNEffectLight::GetFlareMaterialHandler()
{
	if (!m_bFlareMaterial) {
		m_cFlareMaterial.SetResource(GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(m_sFlareMaterial));
		m_bFlareMaterial = true;
	}
	return m_cFlareMaterial;
}

/**
*  @brief
*    Returns the handler of the blend material
*/
const MaterialHandler &SNEffectLight::GetBlendMaterialHandler()
{
	if (!m_bBlendMaterial) {
		m_cBlendMaterial.SetResource(GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(m_sBlendMaterial));
		m_bBlendMaterial = true;
	}
	return m_cBlendMaterial;
}


//[-------------------------------------------------------]
//[ Public virtual SNLight functions                      ]
//[-------------------------------------------------------]
bool SNEffectLight::IsEffectLight() const
{
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
