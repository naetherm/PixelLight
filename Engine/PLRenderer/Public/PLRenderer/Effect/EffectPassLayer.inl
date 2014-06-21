/*********************************************************\
 *  File: EffectPassLayer.inl                            *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the layer owner
*/
inline EffectPass &EffectPassLayer::GetPass() const
{
	return *m_pFXPass;
}

/**
*  @brief
*    Returns the name of the used texture parameter
*/
inline PLCore::String EffectPassLayer::GetTexture() const
{
	return m_sTexture;
}

/**
*  @brief
*    Sets the name of the used texture parameter
*/
inline void EffectPassLayer::SetTexture(const PLCore::String &sTexture)
{
	m_sTexture = sTexture;
}

/**
*  @brief
*    Returns the sampler states
*/
inline SamplerStates &EffectPassLayer::GetSamplerStates()
{
	return m_cSamplerStates;
}

inline const SamplerStates &EffectPassLayer::GetSamplerStates() const
{
	return m_cSamplerStates;
}

/**
*  @brief
*    Returns the fixed functions texture stage states
*/
inline FixedFunctionsTextureStageStates &EffectPassLayer::GetFixedFunctionsTextureStageStates()
{
	return m_cFixedFunctionsTextureStageStates;
}

inline const FixedFunctionsTextureStageStates &EffectPassLayer::GetFixedFunctionsTextureStageStates() const
{
	return m_cFixedFunctionsTextureStageStates;
}

/**
*  @brief
*    Copy operator
*/
inline EffectPassLayer &EffectPassLayer::operator =(const EffectPassLayer &cSource)
{
	// Texture
	m_sTexture = cSource.m_sTexture;

	// Copy states
	m_cSamplerStates				    = cSource.m_cSamplerStates;
	m_cFixedFunctionsTextureStageStates = cSource.m_cFixedFunctionsTextureStageStates;

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline EffectPassLayer::EffectPassLayer(EffectPass &cFXPass) :
	m_pFXPass(&cFXPass)
{
}

/**
*  @brief
*    Destructor
*/
inline EffectPassLayer::~EffectPassLayer()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
