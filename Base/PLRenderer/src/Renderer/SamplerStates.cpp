/*********************************************************\
 *  File: SamplerStates.cpp                              *
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
#include "PLRenderer/Renderer/SamplerStates.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SamplerStates, "PLRenderer", PLCore::Object, "Sampler states RTTI wrapper class")
	// Attributes
	pl_attribute_metadata(AddressU,			pl_enum_type(TextureAddressing::Enum),	TextureAddressing::Wrap,	ReadWrite,	"Texture-address mode for the u/s coordinate",											"")
	pl_attribute_metadata(AddressV,			pl_enum_type(TextureAddressing::Enum),	TextureAddressing::Wrap,	ReadWrite,	"Texture-address mode for the v/t coordinate",											"")
	pl_attribute_metadata(AddressW,			pl_enum_type(TextureAddressing::Enum),	TextureAddressing::Wrap,	ReadWrite,	"Texture-address mode for the w/r coordinate",											"")
		// Filter
	pl_attribute_metadata(MagFilter,		pl_enum_type(TextureFiltering::Enum),	TextureFiltering::Linear,	ReadWrite,	"Magnification filter",																	"")
	pl_attribute_metadata(MinFilter,		pl_enum_type(TextureFiltering::Enum),	TextureFiltering::Linear,	ReadWrite,	"Minification filter",																	"")
	pl_attribute_metadata(MipFilter,		pl_enum_type(TextureFiltering::Enum),	TextureFiltering::Linear,	ReadWrite,	"Mipmap filter to use during minification",												"")
		// Filter
	pl_attribute_metadata(MipmapLODBias,	float,									0.0f,						ReadWrite,	"Mipmap level of detail (LOD) bias",													"")
	pl_attribute_metadata(MaxMapLevel,		PLCore::uint32,							1000,						ReadWrite,	"LOD index of largest map to use. Values range from 0 to (n-1) where 0 is the largest",	"")
	pl_attribute_metadata(MaxAnisotropy,	PLCore::uint32,							1,							ReadWrite,	"Maximum anisotropy",																	"")
pl_class_metadata_end(SamplerStates)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SamplerStates::SamplerStates() :
	AddressU(this),
	AddressV(this),
	AddressW(this),
	MagFilter(this),
	MinFilter(this),
	MipFilter(this),
	MipmapLODBias(this),
	MaxMapLevel(this),
	MaxAnisotropy(this)
{
	// Set default sampler states
	// Address modes
	m_nSS[Sampler::AddressU] = TextureAddressing::Wrap;
	m_nSS[Sampler::AddressV] = TextureAddressing::Wrap;
	m_nSS[Sampler::AddressW] = TextureAddressing::Wrap;
	// Filter
	m_nSS[Sampler::MagFilter] = TextureFiltering::Linear;
	m_nSS[Sampler::MinFilter] = TextureFiltering::Linear;
	m_nSS[Sampler::MipFilter] = TextureFiltering::Linear;
	// Misc
	m_nSS[Sampler::MipmapLODBias] = 0;
	m_nSS[Sampler::MaxMapLevel]   = 1000;
	m_nSS[Sampler::MaxAnisotropy] = 1;
}

/**
*  @brief
*    Copy constructor
*/
SamplerStates::SamplerStates(const SamplerStates &cSource) :
	AddressU(this),
	AddressV(this),
	AddressW(this),
	MagFilter(this),
	MinFilter(this),
	MipFilter(this),
	MipmapLODBias(this),
	MaxMapLevel(this),
	MaxAnisotropy(this)
{
	// Copy states
	for (uint32 i=0; i<Sampler::Number; i++)
		m_nSS[i] = cSource.m_nSS[i];
}

/**
*  @brief
*    Destructor
*/
SamplerStates::~SamplerStates()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
