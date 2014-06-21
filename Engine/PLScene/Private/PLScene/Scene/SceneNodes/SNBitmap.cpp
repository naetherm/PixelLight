/*********************************************************\
 *  File: SNBitmap.cpp                                   *
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
#include <PLCore/File/Url.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodes/SNBitmap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNBitmap)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNBitmap::GetMaterial() const
{
	return m_sMaterial;
}

void SNBitmap::SetMaterial(const String &sValue)
{
	if (m_sMaterial != sValue) {
		m_sMaterial = sValue;
		m_bMaterial = false;
	}
}

const Color4 &SNBitmap::GetColor() const
{
	return m_cColor;
}

void SNBitmap::SetColor(const Color4 &cValue)
{
	m_cColor = cValue;
}

const Vector2 &SNBitmap::GetTexelStart() const
{
	return m_vTexelStart;
}

void SNBitmap::SetTexelStart(const Vector2 &vValue)
{
	m_vTexelStart = vValue;
}

const Vector2 &SNBitmap::GetTexelEnd() const
{
	return m_vTexelEnd;
}

void SNBitmap::SetTexelEnd(const Vector2 &vValue)
{
	m_vTexelEnd = vValue;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the handler of the material
*/
const MaterialHandler &SNBitmap::GetMaterialHandler()
{
	if (!m_bMaterial) {
		// Load material
		PLRenderer::Material *pMaterial = GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(m_sMaterial);
		m_cMaterial.SetResource(pMaterial);

		// Check whether this is a material or an effect, if not, we need to set a nice default billboard effect
		if (pMaterial) {
			const String sExtension = Url(pMaterial->GetName()).GetExtension();
			if (sExtension != "mat" && sExtension != "plfx") {
				// Set the billboard default effect
				PLRenderer::Material *pDefaultMaterial = GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(Material.GetDefault());
				if (pDefaultMaterial)
					pMaterial->SetEffect(pDefaultMaterial->GetEffect());
			}
		}
		m_bMaterial = true;
	}
	return m_cMaterial;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNBitmap::SNBitmap() :
	Material(this),
	Color(this),
	TexelStart(this),
	TexelEnd(this),
	Flags(this),
	m_sMaterial("Data/Effects/DefaultBitmap.plfx"),
	m_bMaterial(false),
	m_cColor(1.0f, 1.0f, 1.0f, 1.0f),
	m_vTexelEnd(1.0f, 1.0f)
{
}

/**
*  @brief
*    Destructor
*/
SNBitmap::~SNBitmap()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
