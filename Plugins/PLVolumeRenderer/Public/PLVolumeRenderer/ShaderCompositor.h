/*********************************************************\
 *  File: ShaderCompositor.h                             *
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


#ifndef __PLVOLUMERENDERER_SHADERCOMPOSITOR_H__
#define __PLVOLUMERENDERER_SHADERCOMPOSITOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLVolumeRenderer/PLVolumeRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
}
namespace PLScene {
	class SQCull;
	class VisNode;
}
namespace PLVolumeRenderer {
	class SRPVolume;
	class ShaderFunction;
	class ShaderComposition;
	class ShaderCompositionID;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract shader compositor base class
*/
class ShaderCompositor : public PLCore::Object {


	// [TODO]
	friend class SRPVolume;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderCompositor, "PLVolumeRenderer", PLCore::Object, "Abstract shader compositor base class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderCompositor();

		// [TODO] Just a test
		PLVOLUMERENDERER_API ShaderFunction *GetShaderFunction(const PLCore::String &sClassName, PLCore::uint8 nNumOfClipPlanes = 0, PLCore::uint8 nNumOfDepthTextures = 0);

		/**
		*  @brief
		*    Clears the cache
		*
		*  @note
		*    - Destroys all cached shader composition instances
		*    - Destroys all cached shader function instances
		*/
		PLVOLUMERENDERER_API void ClearCache();

		/**
		*  @brief
		*    Returns a shader composition instance by using the provided shader composition ID
		*
		*  @param[in] cShaderCompositionID
		*    Shader composition ID
		*
		*  @return
		*    The shader composition instance, null pointer on error
		*/
		PLVOLUMERENDERER_API ShaderComposition *GetComposition(const ShaderCompositionID &cShaderCompositionID) const;


	//[-------------------------------------------------------]
	//[ Public virtual ShaderCompositor functions             ]
	//[-------------------------------------------------------]
	public:
		// [TODO] Just a test
		// Do not destroy the returned instance, can be a null pointer
		virtual ShaderComposition *Generate(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUMERENDERER_API ShaderCompositor();

		/**
		*  @brief
		*    Creates a shader composition instance
		*
		*  @param[in] cShaderCompositionID
		*    Shader composition ID, must be valid!
		*
		*  @return
		*    The created shader composition instance, null pointer on error
		*/
		PLVOLUMERENDERER_API ShaderComposition *CreateShaderComposition(const ShaderCompositionID &cShaderCompositionID);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<ShaderFunction*>						m_lstShaderFunction;		/**< Shader function instances array */
		PLCore::HashMap<PLCore::String, ShaderFunction*>	m_mapShaderFunction;		/**< Shader function instances map */
		PLCore::Array<ShaderComposition*>					m_lstShaderCompositions;	/**< Shader compositions array (only valid shader composition are stored in here) */
		PLCore::HashMap<PLCore::uint32, ShaderComposition*> m_mapShaderCompositions;	/**< Shader compositions map (only valid shader composition are stored in here) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERCOMPOSITOR_H__
