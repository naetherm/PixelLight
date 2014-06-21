/*********************************************************\
 *  File: TextureHandler.h                               *
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


#ifndef __PLRENDERER_TEXTUREHANDLER_H__
#define __PLRENDERER_TEXTUREHANDLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLCore/Container/ResourceHandler.h>
#include <PLMath/Matrix4x4.h>
#include <PLGraphics/Color/Color4.h>
#include "PLRenderer/PLRenderer.h"
#include "PLRenderer/Texture/Texture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Animation;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Texture handler class
*/
class TextureHandler : public PLCore::ResourceHandler<Texture> {


	//[-------------------------------------------------------]
	//[ Public classes                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Texture animation state
		*/
		class AnimationState {


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
			friend class TextureHandler;


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Returns the texture handlers texture
				*
				*  @return
				*    Pointer to the texture handlers texture, can be a null pointer
				*/
				PLRENDERER_API Texture *GetTexture() const;

				/**
				*  @brief
				*    Returns the texture handler from a texture animation frame
				*
				*  @param[in] nIndex
				*    Index of the texture animation frame the handler should be returned.
				*    < 0 if the texture handler from the current texture animation frame
				*    should be returned
				*
				*  @return
				*    Pointer to the requested texture animation frame texture handlers,
				*    a null pointer if there was an error or if nIndex = < 0 and the animation
				*    is disabled
				*/
				PLRENDERER_API TextureHandler *GetFrameTextureHandler(int nIndex = -1) const;

				/**
				*  @brief
				*    Returns the matrix from a matrix animation frame
				*
				*  @param[in] nIndex
				*    Index of the matrix animation frame the matrix should be returned.
				*    < 0 if the matrix from the current matrix animation frame
				*    should be returned
				*
				*  @return
				*    The requested matrix animation frame matrix, identity matrix if there
				*    was an error or if nIndex = < 0 and the animation is disabled
				*/
				PLRENDERER_API PLMath::Matrix4x4 GetFrameMatrix(int nIndex = -1) const;

				/**
				*  @brief
				*    Returns the texture animation
				*
				*  @return
				*    The texture handlers texture animation
				*/
				inline Animation &GetTextureAnimation();

				/**
				*  @brief
				*    Returns the matrix animation
				*
				*  @return
				*    The texture handlers matrix animation
				*/
				inline Animation &GetMatrixAnimation();

				/**
				*  @brief
				*    Returns the current texture transformation matrix
				*
				*  @return
				*    Current texture transformation matrix
				*/
				inline PLMath::Matrix4x4 &GetMatrix();

				/**
				*  @brief
				*    Returns the color animation
				*
				*  @return
				*    The texture handlers color animation
				*/
				inline Animation &GetColorAnimation();

				/**
				*  @brief
				*    Returns the current texture color
				*
				*  @return
				*    Current texture color
				*/
				inline const PLGraphics::Color4 &GetColor() const;

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
				PLRENDERER_API AnimationState &operator =(const AnimationState &cSource);


			//[-------------------------------------------------------]
			//[ Private functions                                     ]
			//[-------------------------------------------------------]
			private:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] cTextureHandler
				*    Owner texture handler
				*/
				AnimationState(TextureHandler &cTextureHandler);

				/**
				*  @brief
				*    Destructor
				*/
				~AnimationState();

				/**
				*  @brief
				*    Called on update
				*/
				void OnUpdate();


			//[-------------------------------------------------------]
			//[ Private event handlers                                ]
			//[-------------------------------------------------------]
			private:
				PLCore::EventHandler<> EventHandlerUpdate;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				TextureHandler		*m_pTextureHandler;		/**< Owner texture handler (NEVER a null pointer!) */
				Animation			*m_pTextureAnimation;	/**< Texture animation (NEVER a null pointer!) */
				PLMath::Matrix4x4	 m_mTrans;				/**< Current texture transformation matrix */
				Animation			*m_pMatrixAnimation;	/**< Matrix animation (NEVER a null pointer!) */
				PLGraphics::Color4   m_cColor;				/**< Current color */
				Animation			*m_pColorAnimation;		/**< Color animation (NEVER a null pointer!) */


		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API TextureHandler();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureHandler();

		/**
		*  @brief
		*    Load texture
		*
		*  @param[in] cTextureManager
		*    Texture manager to use
		*  @param[in] sFilename
		*    Texture filename. It's also possible to create textures dynamically.
		*    "Create PLRenderer::TextureCreatorASCII3D Name=\"ASCII3D\" ASCIIFilename=\"Data/Textures/PostProcess/Font.dds\""
		*    For instance will use the texture creator class 'PLRenderer::TextureCreatorASCII3D'
		*    to create a texture with the name 'ASCII3D' and some parameters.
		*  @param[in] bReloadTexture
		*    Force texture itself to be reloaded?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool Load(TextureManager &cTextureManager, const PLCore::String &sFilename, bool bReloadTexture = false);

		/**
		*  @brief
		*    Unload texture
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool Unload();

		/**
		*  @brief
		*    Makes the texture handlers texture to the current renderer texture
		*
		*  @param[in] nStage
		*    Which texture stage?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - The texture handlers texture transformation matrix is also set!
		*/
		PLRENDERER_API bool Bind(PLCore::uint32 nStage = 0) const;

		/**
		*  @brief
		*    Returns the texture handlers texture
		*
		*  @return
		*    Pointer to the texture handlers texture, can be a null pointer
		*/
		PLRENDERER_API Texture *GetTexture() const;

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
		PLRENDERER_API TextureHandler &operator =(const TextureHandler &cSource);

		/**
		*  @brief
		*    Returns the texture animation state object (if there's one :)
		*
		*  @return
		*    The texture animation state object, a null pointer if there's no such object
		*/
		inline AnimationState *GetAnimationState() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AnimationState *m_pAnimationState;	/**< Texture animation state, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Texture/TextureHandler.inl"


#endif // __PLRENDERER_TEXTUREHANDLER_H__
