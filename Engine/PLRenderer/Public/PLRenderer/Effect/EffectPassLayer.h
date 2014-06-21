/*********************************************************\
 *  File: EffectPassLayer.h                              *
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


#ifndef __PLRENDERER_EFFECT_EFFECTPASSLAYER_H__
#define __PLRENDERER_EFFECT_EFFECTPASSLAYER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/SamplerStates.h"
#include "PLRenderer/Renderer/FixedFunctionsTextureStageStates.h"
#include "PLRenderer/Texture/Texture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Parameter;
class EffectPass;
class ParameterManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Effect pass layer class
*/
class EffectPassLayer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class EffectPass;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the layer owner
		*
		*  @return
		*    The layer owner
		*/
		inline EffectPass &GetPass() const;

		/**
		*  @brief
		*    Makes the texture buffer of the texture handlers texture to the current renderer texture buffer
		*
		*  @param[in] nStage
		*    Which texture stage?
		*  @param[in] pParameterManager
		*    Parameters set instead of existing effect parameters, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - The texture handlers texture transformation matrix is also set!
		*    - If there's no texture parameter name or an invalid one no texture is bound
		*/
		PLRENDERER_API bool Bind(PLCore::uint32 nStage, ParameterManager *pParameterManager = nullptr) const;

		/**
		*  @brief
		*    Returns the name of the used texture parameter
		*
		*  @return
		*    The name of the used texture parameter
		*/
		inline PLCore::String GetTexture() const;

		/**
		*  @brief
		*    Sets the name of the used texture parameter
		*
		*  @param[in] sTexture
		*    The name of the used texture parameter
		*/
		inline void SetTexture(const PLCore::String &sTexture);

		/**
		*  @brief
		*    Returns the sampler states
		*
		*  @return
		*    The sampler states
		*/
		inline SamplerStates &GetSamplerStates();
		inline const SamplerStates &GetSamplerStates() const;

		/**
		*  @brief
		*    Returns the fixed functions texture stage states
		*
		*  @return
		*    The fixed functions texture stage states
		*/
		inline FixedFunctionsTextureStageStates &GetFixedFunctionsTextureStageStates();
		inline const FixedFunctionsTextureStageStates &GetFixedFunctionsTextureStageStates() const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This instance
		*/
		inline EffectPassLayer &operator =(const EffectPassLayer &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFXPass
		*    Layer owner
		*/
		inline EffectPassLayer(EffectPass &cFXPass);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~EffectPassLayer();

		/**
		*  @brief
		*    Binds a texture
		*
		*  @param[in] pParameter
		*    Texture parameter to bind, if a null pointer, nothing happens...
		*  @param[in] nStage
		*    Stage to bind at, if a shader is used, this information is ignored
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool BindTexture(const Parameter *pParameter, PLCore::uint32 nStage) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EffectPass						 *m_pFXPass;							/**< Layer owner (NEVER a null pointer!) */
		PLCore::String					  m_sTexture;							/**< The name of the used texture parameter */
		SamplerStates					  m_cSamplerStates;						/**< Sampler states */
		FixedFunctionsTextureStageStates  m_cFixedFunctionsTextureStageStates;	/**< Fixed functions texture stage states */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Effect/EffectPassLayer.inl"


#endif // __PLRENDERER_EFFECT_EFFECTPASSLAYER_H__
