/*********************************************************\
 *  File: EffectTechnique.cpp                            *
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
#include "PLRenderer/Effect/EffectPass.h"
#include "PLRenderer/Effect/EffectTechnique.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a pass
*/
EffectPass *EffectTechnique::AddPass(int nIndex)
{
	// Create pass
	EffectPass *pFXPass = new EffectPass(*this);
	m_lstPass.AddAtIndex(pFXPass, nIndex);

	// Return the new pass
	return pFXPass;
}

/**
*  @brief
*    Removes a pass
*/
bool EffectTechnique::RemovePass(uint32 nIndex)
{
	EffectPass *pFXPass = m_lstPass[nIndex];
	if (pFXPass) {
		delete pFXPass;

		// Done
		return m_lstPass.RemoveAtIndex(nIndex);
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Removes all passes
*/
void EffectTechnique::RemoveAllPasses()
{
	for (uint32 i=0; i<m_lstPass.GetNumOfElements(); i++)
		delete m_lstPass[i];
	m_lstPass.Clear();
}

/**
*  @brief
*    Setup the given path
*/
bool EffectTechnique::SetupPass(uint32 nIndex, ParameterManager *pParameterManager) const
{
	EffectPass *pPass = m_lstPass[nIndex];
	return (pPass && pPass->Bind(pParameterManager));
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
EffectTechnique::EffectTechnique(Effect &cFX) :
	m_pFX(&cFX),
	m_bValid(true)
{
}

/**
*  @brief
*    Destructor
*/
EffectTechnique::~EffectTechnique()
{
	RemoveAllPasses();
}

/**
*  @brief
*    Copy operator
*/
EffectTechnique &EffectTechnique::operator =(const EffectTechnique &cSource)
{
	// Copy attributes
	m_bValid = cSource.m_bValid;
	m_sName  = cSource.m_sName;

	// Copy passes
	uint32 nNumOfPasses = cSource.m_lstPass.GetNumOfElements();
	for (uint32 nPass=0; nPass<nNumOfPasses; nPass++)
		*AddPass() = *cSource.m_lstPass[nPass];

	// Return this
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
