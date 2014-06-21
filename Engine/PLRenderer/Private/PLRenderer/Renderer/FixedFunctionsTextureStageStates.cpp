/*********************************************************\
 *  File: FixedFunctionsTextureStageStates.cpp           *
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
#include "PLRenderer/Renderer/FixedFunctionsTextureStageStates.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(FixedFunctionsTextureStageStates)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
FixedFunctions::TextureEnvironment::Enum FixedFunctionsTextureStageStates::GetColorTexEnv() const
{
	return static_cast<FixedFunctions::TextureEnvironment::Enum>(m_nTSS[FixedFunctions::TextureStage::ColorTexEnv]);
}

void FixedFunctionsTextureStageStates::SetColorTexEnv(FixedFunctions::TextureEnvironment::Enum nValue)
{
	m_nTSS[FixedFunctions::TextureStage::ColorTexEnv] = nValue;
}

FixedFunctions::TextureEnvironment::Enum FixedFunctionsTextureStageStates::GetAlphaTexEnv() const
{
	return static_cast<FixedFunctions::TextureEnvironment::Enum>(m_nTSS[FixedFunctions::TextureStage::AlphaTexEnv]);
}

void FixedFunctionsTextureStageStates::SetAlphaTexEnv(FixedFunctions::TextureEnvironment::Enum nValue)
{
	m_nTSS[FixedFunctions::TextureStage::AlphaTexEnv] = nValue;
}

FixedFunctions::TexCoordGen::Enum FixedFunctionsTextureStageStates::GetTexGen() const
{
	return static_cast<FixedFunctions::TexCoordGen::Enum>(m_nTSS[FixedFunctions::TextureStage::TexGen]);
}

void FixedFunctionsTextureStageStates::SetTexGen(FixedFunctions::TexCoordGen::Enum nValue)
{
	m_nTSS[FixedFunctions::TextureStage::TexGen] = nValue;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FixedFunctionsTextureStageStates::FixedFunctionsTextureStageStates() :
	ColorTexEnv(this),
	AlphaTexEnv(this),
	TexGen(this)
{
	// Set default texture stage states
	m_nTSS[FixedFunctions::TextureStage::ColorTexEnv] = FixedFunctions::TextureEnvironment::Modulate;
	m_nTSS[FixedFunctions::TextureStage::AlphaTexEnv] = FixedFunctions::TextureEnvironment::Modulate;
	m_nTSS[FixedFunctions::TextureStage::TexGen]      = FixedFunctions::TexCoordGen::None;
}

/**
*  @brief
*    Copy constructor
*/
FixedFunctionsTextureStageStates::FixedFunctionsTextureStageStates(const FixedFunctionsTextureStageStates &cSource) :
	ColorTexEnv(this),
	AlphaTexEnv(this),
	TexGen(this)
{
	// Copy states
	for (uint32 i=0; i<FixedFunctions::TextureStage::Number; i++)
		m_nTSS[i] = cSource.m_nTSS[i];
}

/**
*  @brief
*    Destructor
*/
FixedFunctionsTextureStageStates::~FixedFunctionsTextureStageStates()
{
}

/**
*  @brief
*    Retrieves a texture stage state value
*/
uint32 FixedFunctionsTextureStageStates::Get(FixedFunctions::TextureStage::Enum nState) const
{
	// Check whether the state is a valid texture stage member
	return (nState < FixedFunctions::TextureStage::Number) ? m_nTSS[nState] : 0;
}

/**
*  @brief
*    Sets a single texture stage state parameter
*/
bool FixedFunctionsTextureStageStates::Set(FixedFunctions::TextureStage::Enum nState, uint32 nValue)
{
	// Check whether the state is a valid texture stage member
	if (nState >= FixedFunctions::TextureStage::Number)
		return false; // Error!

	// Set texture stage state value
	m_nTSS[nState] = nValue;

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
FixedFunctionsTextureStageStates &FixedFunctionsTextureStageStates::operator =(const FixedFunctionsTextureStageStates &cSource)
{
	// Copy states
	for (uint32 i=0; i<FixedFunctions::TextureStage::Number; i++)
		m_nTSS[i] = cSource.m_nTSS[i];

	// Return this
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
