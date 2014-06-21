/*********************************************************\
 *  File: Effect.cpp                                     *
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
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Material/Parameter.h"
#include "PLRenderer/Material/ParameterManager.h"
#include "PLRenderer/Effect/EffectTechnique.h"
#include "PLRenderer/Effect/EffectManager.h"
#include "PLRenderer/Effect/EffectPass.h"
#include "PLRenderer/Effect/Effect.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Effect::~Effect()
{
	// First at all: Unload the effect
	Unload();

	// Destroy the parameter manager
	delete m_pParameterManager;
}

/**
*  @brief
*    Copy operator
*/
Effect &Effect::operator =(const Effect &cSource)
{
	// Clear effect
	Unload();
	m_sFilename			 = cSource.m_sFilename;
	m_nFlags			 = cSource.GetFlags();
	m_bBlend			 = cSource.GetBlend();
	m_nSelectedTechnique = cSource.GetSelectedTechniqueIndex();
	for (uint32 i=0; i<cSource.m_lstTechnique.GetNumOfElements(); i++)
		*AddTechnique() = *cSource.m_lstTechnique[i];

	/* [TODO] Check me
	for (uint32 i=0; i<cSource.m_lstParameters.GetNumOfElements(); i++) {
		m_lstParameters.Add();
		*m_lstParameters[i] = *cSource.m_lstParameters[i];
	}
	*/

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Technique functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a technique
*/
EffectTechnique *Effect::AddTechnique(int nTechnique)
{
	EffectTechnique *pTechnique = new EffectTechnique(*this);
	pTechnique->SetName(String("Technique_") + m_lstTechnique.GetNumOfElements());
	m_lstTechnique.AddAtIndex(pTechnique, nTechnique);
	return pTechnique;
}

/**
*  @brief
*    Removes a technique
*/
bool Effect::RemoveTechnique(int nTechnique)
{
	if (nTechnique < 0)
		nTechnique = m_nSelectedTechnique;
	if (nTechnique == m_nSelectedTechnique)
		m_nSelectedTechnique = -1;
	EffectTechnique *pTechnique = m_lstTechnique[nTechnique];
	if (pTechnique) {
		delete pTechnique;
		return m_lstTechnique.RemoveAtIndex(nTechnique);
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Removes all techniques
*/
void Effect::RemoveAllTechniques()
{
	for (uint32 i=0; i<m_lstTechnique.GetNumOfElements(); i++)
		delete m_lstTechnique[i];
	m_lstTechnique.Clear();
	m_nSelectedTechnique = -1;
}

/**
*  @brief
*    Select the current technique by name
*/
bool Effect::SelectTechnique(const String &sName)
{
	// Loop through all techniques
	for (uint32 i=0; i<m_lstTechnique.GetNumOfElements(); i++) {
		if (m_lstTechnique[i]->GetName() == sName) {
			m_nSelectedTechnique = i;

			// Done
			return true;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Effect::Effect(EffectManager &cManager, const String &sName) : PLCore::Resource<Effect>(sName, &cManager),
	m_pParameterManager(new ParameterManager(cManager.GetRendererContext())),
	m_nFlags(0),
	m_bBlend(false),
	m_nSelectedTechnique(-1)
{
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Effect::Reload()
{
	// Call base implementation
	if (!PLCore::Resource<Effect>::Reload())
		return false; // Error!

	// Reload textures
	return m_pParameterManager->ReloadTextures();
}

bool Effect::Unload()
{
	// Remove all parameters
	m_pParameterManager->RemoveAllParameters();

	// Remove all techniques
	RemoveAllTechniques();

	// Reset data
	m_nFlags			 = 0;
	m_bBlend			 = false;
	m_nSelectedTechnique = -1;

	// Call base implementation
	return PLCore::Resource<Effect>::Unload();
}

String Effect::GetLoadableTypeName() const
{
	static const String sString = "Effect";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
