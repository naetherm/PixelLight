/*********************************************************\
 *  File: SNLight.cpp                                    *
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
#include "PLScene/Scene/SNLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNLight, "PLScene", PLScene::SceneNode, "Abstract light scene node base class")
	// Properties
	pl_properties
		pl_property("Icon",	"Data/Textures/IconLight.dds")
	pl_properties_end
	// Methods
	pl_method_0_metadata(IsRenderLight,				pl_ret_type(bool),	"Returns whether this is a render light or not. Returns 'true' if this is a render light, else 'false'.",																		"")
	pl_method_0_metadata(IsEffectLight,				pl_ret_type(bool),	"Returns whether this scene node is a effect light (SNEffectLight) or not. Returns 'true' if this scene node is a effect light, else 'false'.",									"")
	pl_method_0_metadata(IsPointLight,				pl_ret_type(bool),	"Returns whether this scene node is a point light (SNPointLight) or not. Returns 'true' if this scene node is a point light, else 'false'.",									"")
	pl_method_0_metadata(IsProjectivePointLight,	pl_ret_type(bool),	"Returns whether this scene node is a projective point light (SNProjectivePointLight) or not. Returns 'true' if this scene node is a projective point light, else 'false'.",	"")
	pl_method_0_metadata(IsSpotLight,				pl_ret_type(bool),	"Returns whether this scene node is a spot light (SNSpotLight) or not. Returns 'true' if this scene node is a spot light, else 'false'.",										"")
	pl_method_0_metadata(IsProjectiveSpotLight,		pl_ret_type(bool),	"Returns whether this scene node is a projective spot light (SNProjectiveSpotLight) or not. Returns 'true' if this scene node is a projective spot light, else 'false'.",		"")
	pl_method_0_metadata(IsDirectionalLight,		pl_ret_type(bool),	"Returns whether this scene node is a directional light (SNDirectionalLight) or not. Returns 'true' if this scene node is a directional light, else 'false'.",					"")
	// Attributes
	pl_attribute_metadata(Color,			PLGraphics::Color3,					PLGraphics::Color3::White,	ReadWrite,	"Light color (r/g/b)",	"")
	pl_attribute_metadata(CoronaSize,		float,								0.2f,						ReadWrite,	"Corona size",			"")
	pl_attribute_metadata(FlareSize,		float,								0.1f,						ReadWrite,	"Lens flare size",		"")
	pl_attribute_metadata(ScreenBrighten,	float,								0.3f,						ReadWrite,	"Screen brighten",		"")
		// Overwritten SceneNode attributes
	pl_attribute_metadata(Flags,			pl_flag_type_def3(SNLight, EFlags),	0,							ReadWrite,	"Flags",				"")
pl_class_metadata_end(SNLight)


//[-------------------------------------------------------]
//[ Public virtual SNLight functions                      ]
//[-------------------------------------------------------]
bool SNLight::IsRenderLight() const
{
	return false;
}

bool SNLight::IsEffectLight() const
{
	return false;
}

bool SNLight::IsPointLight() const
{
	return false;
}

bool SNLight::IsProjectivePointLight() const
{
	return false;
}

bool SNLight::IsSpotLight() const
{
	return false;
}

bool SNLight::IsProjectiveSpotLight() const
{
	return false;
}

bool SNLight::IsDirectionalLight() const
{
	return false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNLight::SNLight() :
	Color(this),
	CoronaSize(this),
	FlareSize(this),
	ScreenBrighten(this),
	Flags(this)
{
	// Set the internal flag
	m_nInternalFlags |= ClassLight;
}

/**
*  @brief
*    Destructor
*/
SNLight::~SNLight()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
