/*********************************************************\
 *  File: EffectTechnique.inl                            *
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
*    Returns the owner effect
*/
inline Effect &EffectTechnique::GetEffect() const
{
	return *m_pFX;
}

/**
*  @brief
*    Returns whether the technique is valid or not
*/
inline bool EffectTechnique::IsValid() const
{
	return m_bValid;
}

/**
*  @brief
*    Sets whether the technique is valid or not
*/
inline void EffectTechnique::SetValid(bool bValid)
{
	m_bValid = bValid;
}

/**
*  @brief
*    Gets the name of the technique
*/
inline PLCore::String EffectTechnique::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Sets the name of the technique
*/
inline void EffectTechnique::SetName(const PLCore::String &sName)
{
	m_sName = sName;
}

/**
*  @brief
*    Gets the number of render passes the technique is using
*/
inline PLCore::uint32 EffectTechnique::GetNumOfPasses() const
{
	return m_lstPass.GetNumOfElements();
}

/**
*  @brief
*    Gets a pass
*/
inline EffectPass *EffectTechnique::GetPass(PLCore::uint32 nIndex) const
{
	return m_lstPass[nIndex];
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
