/*********************************************************\
 *  File: EffectTechnique.h                              *
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


#ifndef __PLRENDERER_EFFECT_EFFECTTECHNIQUE_H__
#define __PLRENDERER_EFFECT_EFFECTTECHNIQUE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLCore/Container/Array.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Effect;
class EffectPass;
class ParameterManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Effect technique class
*/
class EffectTechnique {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Effect;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the owner effect
		*
		*  @return
		*    Owner effect
		*/
		inline Effect &GetEffect() const;

		/**
		*  @brief
		*    Returns whether the technique is valid or not
		*
		*  @return
		*    'true' if the technique is valid, else 'false'
		*
		*  @remarks
		*    If an technique is invalid for instance because an used
		*    shader isn't supported by the hardware you shouldn't use this
		*    technique because it may produce ugly results!
		*/
		inline bool IsValid() const;

		/**
		*  @brief
		*    Sets whether the technique is valid or not
		*
		*  @param[in] bValid
		*    Technique valid?
		*/
		inline void SetValid(bool bValid);

		/**
		*  @brief
		*    Gets the name of the technique
		*
		*  @return
		*    Name of the technique
		*/
		inline PLCore::String GetName() const;

		/**
		*  @brief
		*    Sets the name of the technique
		*
		*  @param[in] sName
		*    New technique name
		*/
		inline void SetName(const PLCore::String &sName = "");

		/**
		*  @brief
		*    Gets the number of render passes the technique is using
		*
		*  @return
		*    Number of passes
		*/
		inline PLCore::uint32 GetNumOfPasses() const;

		/**
		*  @brief
		*    Adds a pass
		*
		*  @param[in] nIndex
		*    Pass index in the technique where to add the new pass, if < 0 add at the end
		*
		*  @return
		*    The new pass, a null pointer on error
		*/
		PLRENDERER_API EffectPass *AddPass(int nIndex = -1);

		/**
		*  @brief
		*    Removes a pass
		*
		*  @param[in] nIndex
		*    Pass index in the technique to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool RemovePass(PLCore::uint32 nIndex = 0);

		/**
		*  @brief
		*    Removes all passes
		*/
		PLRENDERER_API void RemoveAllPasses();

		/**
		*  @brief
		*    Gets a pass
		*
		*  @param[in] nIndex
		*    Pass index in the technique
		*
		*  @return
		*    The requested pass, a null pointer on error
		*/
		inline EffectPass *GetPass(PLCore::uint32 nIndex = 0) const;

		/**
		*  @brief
		*    Setup the given path
		*
		*  @param[in] nIndex
		*    Index of the path to setup
		*  @param[in] pParameterManager
		*    Parameters set instead of existing effect parameters, if a null pointer, use THIS parameter manager
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool SetupPass(PLCore::uint32 nIndex = 0, ParameterManager *pParameterManager = nullptr) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFX
		*    Owner effect
		*/
		EffectTechnique(Effect &cFX);

		/**
		*  @brief
		*    Destructor
		*/
		~EffectTechnique();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this effect technique
		*/
		EffectTechnique &operator =(const EffectTechnique &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Effect					   *m_pFX;		/**< Owner effect (always valid!) */
		bool						m_bValid;	/**< Is the technique valid? */
		PLCore::String			    m_sName;	/**< Technique name */
		PLCore::Array<EffectPass*>  m_lstPass;	/**< List of passes */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Effect/EffectTechnique.inl"


#endif // __PLRENDERER_EFFECT_EFFECTTECHNIQUE_H__
