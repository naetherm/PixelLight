/*********************************************************\
 *  File: EffectPass.inl                                 *
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
*    Returns the owner effect technique
*/
inline EffectTechnique &EffectPass::GetTechnique() const
{
	return *m_pTechnique;
}

/**
*  @brief
*    Returns the pass name
*/
inline PLCore::String EffectPass::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Sets the pass name
*/
inline void EffectPass::SetName(const PLCore::String &sName)
{
	m_sName = sName;
}


//[-------------------------------------------------------]
//[ States                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the general color
*/
inline const PLGraphics::Color4 &EffectPass::GetColor() const
{
	return m_cColor;
}

/**
*  @brief
*    Sets the general color
*/
inline void EffectPass::SetColor(const PLGraphics::Color4 &cColor)
{
	m_cColor = cColor;
}

/**
*  @brief
*    Returns the render states
*/
inline RenderStates &EffectPass::GetRenderStates()
{
	return m_cRenderStates;
}

inline const RenderStates &EffectPass::GetRenderStates() const
{
	return m_cRenderStates;
}

/**
*  @brief
*    Returns the fixed functions render states
*/
inline FixedFunctionsRenderStates &EffectPass::GetFixedFunctionsRenderStates()
{
	return m_cFixedFunctionsRenderStates;
}

inline const FixedFunctionsRenderStates &EffectPass::GetFixedFunctionsRenderStates() const
{
	return m_cFixedFunctionsRenderStates;
}

/**
*  @brief
*    Retrieves a material-state value
*/
inline PLCore::uint32 EffectPass::GetMaterialState(FixedFunctions::MaterialState::Enum nState) const
{
	// Check if the state is a value renderer material state member
	return (nState < FixedFunctions::MaterialState::Number) ? m_nMaterialState[nState] : 0;
}

/**
*  @brief
*    Sets a single material-state parameter
*/
inline bool EffectPass::SetMaterialState(FixedFunctions::MaterialState::Enum nState, PLCore::uint32 nValue)
{
	// Check if the state is a value renderer material state member
	if (nState >= FixedFunctions::MaterialState::Number)
		return false; // Error!

	// Set material state value
	m_nMaterialState[nState] = nValue;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Layers                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of texture layers
*/
inline PLCore::uint32 EffectPass::GetNumOfLayers() const
{
	return m_lstLayers.GetNumOfElements();
}

/**
*  @brief
*    Returns a layer
*/
EffectPassLayer *EffectPass::GetLayer(PLCore::uint32 nIndex) const
{
	return m_lstLayers[nIndex];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
