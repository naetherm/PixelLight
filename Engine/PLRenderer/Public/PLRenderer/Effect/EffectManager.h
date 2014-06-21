/*********************************************************\
 *  File: EffectManager.h                                *
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


#ifndef __PLRENDERER_EFFECT_MANAGER_H__
#define __PLRENDERER_EFFECT_MANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/ResourceManager.h>
#include "PLRenderer/Effect/Effect.h"
#include "PLRenderer/Effect/EffectHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Effect;
class EffectHandler;
class RendererContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This is a manager for the effect resource
*
*  @note
*    - Unloads unused resources automatically by default
*    - The public static effect strings within this class are just filenames without any
*      fixed functionality
*/
class EffectManager : public PLCore::ResourceManager<Effect> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RendererContext;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLRENDERER_API const PLCore::String Default;	/**< Default effect, ('Default') dynamically created within the 'EffectManager'-constructor */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the owner renderer context of this manager
		*
		*  @return
		*    Reference to the owner renderer context of this manager
		*/
		inline RendererContext &GetRendererContext() const;

		/**
		*  @brief
		*    Reloads all effects
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool ReloadEffects();

		/**
		*  @brief
		*    Uses a given effect
		*
		*  @param[in] sName
		*    Name of the effect to use
		*
		*  @remarks
		*    This function is calling the Bind() and SetupPass(0) function of the given effect.
		*    The name is in fact the filename of the effect. It's recommended to use only the
		*    predefined names within this class within this function because this resources are
		*    loaded and marked as protected by default. If you use any other effect, ensure that this
		*    effect is protected in order to avoid frequently loading/destruction of the resource in
		*    the worst case. It's recommended to use this function ONLY if you want to set 'some known'
		*    render states without doing this by hand.
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool Use(const PLCore::String &sName = Default);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRendererContext
		*    Owner renderer context
		*/
		EffectManager(RendererContext &cRendererContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EffectManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RendererContext *m_pRendererContext;	/**< Owner renderer context of this manager, always valid! */
		EffectHandler	*m_pDefault;			/**< Default effect handler (always valid pointer!) */


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ResourceManager functions     ]
	//[-------------------------------------------------------]
	private:
		virtual Effect *CreateResource(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Effect/EffectManager.inl"


#endif // __PLRENDERER_EFFECT_MANAGER_H__
