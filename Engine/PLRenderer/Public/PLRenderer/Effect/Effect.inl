/*********************************************************\
 *  File: Effect.inl                                     *
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
*    Returns the effect manager this effect is in
*/
inline EffectManager &Effect::GetEffectManager() const
{
	// There MUST always be a manager!
	return reinterpret_cast<EffectManager&>(*m_pManager);
}

/**
*  @brief
*    Returns the parameter manager of the effect
*/
inline ParameterManager &Effect::GetParameterManager() const
{
	return *m_pParameterManager;
}

/**
*  @brief
*    Returns whether the effect should be blend or not
*/
inline bool Effect::GetBlend() const
{
	return m_bBlend;
}

/**
*  @brief
*    Activates/deactivates the effect blending
*/
inline void Effect::SetBlend(bool bBlend)
{
	m_bBlend = bBlend;
}

/**
*  @brief
*    Returns whether the effect is loaded or not
*/
inline bool Effect::IsLoaded() const
{
	return (m_sFilename.GetLength() != 0);
}


//[-------------------------------------------------------]
//[ General                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the flags
*/
inline PLCore::uint32 Effect::GetFlags() const
{
	return m_nFlags;
}

/**
*  @brief
*    Sets the flags
*/
inline void Effect::SetFlags(PLCore::uint32 nFlags)
{
	m_nFlags = nFlags;
}


//[-------------------------------------------------------]
//[ Technique functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Gets the number of techniques
*/
inline PLCore::uint32 Effect::GetNumOfTechniques() const
{
	return m_lstTechnique.GetNumOfElements();
}

/**
*  @brief
*    Gets a technique
*/
inline EffectTechnique *Effect::GetTechnique(PLCore::uint32 nIndex) const
{
	return m_lstTechnique[nIndex];
}

/**
*  @brief
*    Gets the current selected technique
*/
inline EffectTechnique *Effect::GetSelectedTechnique() const
{
	return m_lstTechnique[m_nSelectedTechnique];
}

/**
*  @brief
*    Gets the index of the current selected technique
*/
inline int Effect::GetSelectedTechniqueIndex() const
{
	return m_nSelectedTechnique;
}

/**
*  @brief
*    Select the current technique by index
*/
inline bool Effect::SelectTechnique(PLCore::uint32 nIndex)
{
	if (m_lstTechnique[nIndex]) {
		m_nSelectedTechnique = nIndex;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
