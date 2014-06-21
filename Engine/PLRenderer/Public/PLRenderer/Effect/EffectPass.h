/*********************************************************\
 *  File: EffectPass.h                                   *
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


#ifndef __PLRENDERER_EFFECT_PASS_H__
#define __PLRENDERER_EFFECT_PASS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/FixedFunctions.h"
#include "PLRenderer/Renderer/RenderStates.h"
#include "PLRenderer/Renderer/FixedFunctionsRenderStates.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Program;
class VertexShader;
class FragmentShader;
class EffectPassLayer;
class EffectTechnique;
class ParameterManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A effect pass will set the render states, textures etc.
*    required for proper rendering of the given pass.
*/
class EffectPass {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class EffectTechnique;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the owner effect technique
		*
		*  @return
		*    Owner effect technique
		*/
		inline EffectTechnique &GetTechnique() const;

		/**
		*  @brief
		*    Returns the pass name
		*
		*  @return
		*    Pass name
		*/
		inline PLCore::String GetName() const;

		/**
		*  @brief
		*    Sets the pass name
		*
		*  @param[in] sName
		*    New pass name
		*/
		inline void SetName(const PLCore::String &sName = "Pass");

		/**
		*  @brief
		*    Binds the pass
		*
		*  @param[in] pParameterManager
		*    Parameters set instead of existing effect parameters, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool Bind(ParameterManager *pParameterManager = nullptr);

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
		PLRENDERER_API EffectPass &operator =(const EffectPass &cSource);

		//[-------------------------------------------------------]
		//[ States                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the general color
		*
		*  @return
		*    General color
		*/
		inline const PLGraphics::Color4 &GetColor() const;

		/**
		*  @brief
		*    Sets the general color
		*
		*  @param[in] cColor
		*    New general color
		*/
		inline void SetColor(const PLGraphics::Color4 &cColor = PLGraphics::Color4::White);

		/**
		*  @brief
		*    Returns the render states
		*
		*  @return
		*    The render states
		*/
		inline RenderStates &GetRenderStates();
		inline const RenderStates &GetRenderStates() const;

		/**
		*  @brief
		*    Returns the fixed functions render states
		*
		*  @return
		*    The fixed functions render states
		*/
		inline FixedFunctionsRenderStates &GetFixedFunctionsRenderStates();
		inline const FixedFunctionsRenderStates &GetFixedFunctionsRenderStates() const;

		/**
		*  @brief
		*    Resets all material states to default
		*/
		PLRENDERER_API void ResetMaterialStates();

		/**
		*  @brief
		*    Retrieves a material-state value
		*
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the renderer material enumerated type.
		*
		*  @return
		*    The value of the queried material state variable
		*/
		inline PLCore::uint32 GetMaterialState(FixedFunctions::MaterialState::Enum nState) const;

		/**
		*  @brief
		*    Sets a single material-state parameter
		*
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the renderer material enumerated type.
		*  @param[in] nValue
		*    New value for the material state to be set.
		*    You can also set the value to unknown - then this state is ignored.
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		inline bool SetMaterialState(FixedFunctions::MaterialState::Enum nState, PLCore::uint32 nValue);

		//[-------------------------------------------------------]
		//[ Layers                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of texture layers
		*
		*  @return
		*    Number of texture layers
		*/
		inline PLCore::uint32 GetNumOfLayers() const;

		/**
		*  @brief
		*    Adds a new layer
		*
		*  @return
		*    The new effect pass layer, a null pointer on error
		*/
		PLRENDERER_API EffectPassLayer *AddLayer();

		/**
		*  @brief
		*    Returns a layer
		*
		*  @param[in] nIndex
		*    Layer index
		*
		*  @return
		*    Effect pass layer, a null pointer if there was an error
		*/
		inline EffectPassLayer *GetLayer(PLCore::uint32 nIndex = 0) const;

		/**
		*  @brief
		*    Removes a layer
		*
		*  @param[in] nIndex
		*    Index of the layer to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool RemoveLayer(PLCore::uint32 nIndex = 0);

		/**
		*  @brief
		*    Removes all layers
		*/
		PLRENDERER_API void RemoveAllLayers();

		//[-------------------------------------------------------]
		//[ Shaders                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Loads a vertex shader
		*
		*  @param[in] sFilename
		*    Shader filename
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg")
		*  @param[in] sProfile
		*    Profile that should be used (e.g. "arbvp1")
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool LoadVertexShader(const PLCore::String &sFilename, const PLCore::String &sShaderLanguage, const PLCore::String &sProfile = "");

		/**
		*  @brief
		*    Loads a fragment shader
		*
		*  @param[in] sFilename
		*    Shader filename
		*  @param[in] sShaderLanguage
		*    Shader language to use (for example "GLSL" or "Cg")
		*  @param[in] sProfile
		*    Profile that should be used (e.g. "arbfp1")
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool LoadFragmentShader(const PLCore::String &sFilename, const PLCore::String &sShaderLanguage, const PLCore::String &sProfile = "");

		/**
		*  @brief
		*    Returns the used GPU program
		*
		*  @return
		*    The used GPU program, a null pointer if there's no such GPU program
		*/
		PLRENDERER_API Program *GetProgram();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cTechnique
		*    Owner effect technique
		*/
		EffectPass(EffectTechnique &cTechnique);

		/**
		*  @brief
		*    Destructor
		*/
		~EffectPass();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EffectTechnique   *m_pTechnique;	/**< Owner effect technique (always valid!) */
		PLCore::String	  m_sName;			/**< Pass name */
		// States
		PLGraphics::Color4		   m_cColor;												/**< General color */
		RenderStates			   m_cRenderStates;											/**< Render states */
		FixedFunctionsRenderStates m_cFixedFunctionsRenderStates;							/**< Fixed functions render states */
		PLCore::uint32			   m_nMaterialState[FixedFunctions::MaterialState::Number];	/**< List of material states (see 'Material') */
		// Layers
		PLCore::Array<EffectPassLayer*> m_lstLayers;	/**< Texture layers */
		// Shaders
		VertexShader   *m_pVertexShader;	/**< Used vertex shader, can be a null pointer */
		FragmentShader *m_pFragmentShader;	/**< Used fragment shader, can be a null pointer */
		Program		   *m_pProgram;			/**< GPU program, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Effect/EffectPass.inl"


#endif // __PLRENDERER_EFFECT_PASS_H__
