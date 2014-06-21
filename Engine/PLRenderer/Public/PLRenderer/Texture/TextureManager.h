/*********************************************************\
 *  File: TextureManager.h                               *
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


#ifndef __PLRENDERER_TEXTUREMANAGER_H__
#define __PLRENDERER_TEXTUREMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/ResourceManager.h>
#include "PLRenderer/Texture/Texture.h"
#include "PLRenderer/Texture/TextureHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class RendererContext;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This is a manager for the texture resource
*
*  @note
*    - Unloads unused resources automatically by default
*/
class TextureManager : public PLCore::ResourceManager<Texture> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class TextureHandler;
	friend class RendererContext;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLRENDERER_API const PLCore::String Default;	/**< Default texture */


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
		*    Returns the texture quality
		*
		*  @return
		*    Texture quality, 1 best, 0 worst
		*
		*  @see
		*    - SetTextureQuality()
		*/
		inline float GetTextureQuality() const;

		/**
		*  @brief
		*    Sets the texture quality
		*
		*  @param[in] fQuality
		*    Texture quality, 1.0 best, 0.0 worst, default is 1.0
		*
		*  @note
		*    - Only has an influence on newly loaded textures, not already loaded ones
		*    - If the given texture quality is outside the specified range, it's clamped automatically to the nearest borders
		*/
		PLRENDERER_API void SetTextureQuality(float fQuality);

		/**
		*  @brief
		*    Returns whether the next lower valid texture size should be taken or
		*    the higher one
		*
		*  @return
		*    If 'true' the next lower valid texture size will be taken, if 'false'
		*    the next higher one is chosen
		*
		*  @remarks
		*    Normally the texture manager will automatically correct invalid texture sizes.
		*    With the texture fit setting you can setup whether the next lower or higher valid texture
		*    size is selected automatically.
		*/
		inline bool GetTextureFit() const;

		/**
		*  @brief
		*    Sets whether the next lower valid texture size should be taken or
		*    the higher one
		*
		*  @param[in] bLower
		*    Should the next lower valid texture size be chosen? If
		*    'false' the next higher will be taken
		*
		*  @see
		*    - GetTextureFit()
		*
		*  @note
		*    - Only has an influence on newly loaded textures, not already loaded ones
		*/
		inline void SetTextureFit(bool bLower = true);

		/**
		*  @brief
		*    Returns whether or not the usage of texture mipmaps is allowed
		*
		*  @return
		*    'true' if the usage of texture mipmaps is allowed, else 'false'
		*
		*  @see
		*    - SetTextureMipmapsAllowed()
		*/
		inline bool AreTextureMipmapsAllowed() const;

		/**
		*  @brief
		*    Sets whether or not the usage of texture mipmaps is allowed
		*
		*  @param[in] bAllowed
		*    'true' if the usage of texture mipmaps is allowed, else 'false', default is 'true'
		*
		*  @note
		*    - Only has an influence on newly loaded textures, not already loaded ones
		*/
		inline void SetTextureMipmapsAllowed(bool bAllowed);

		/**
		*  @brief
		*    Returns whether or not the usage of texture compression is allowed
		*
		*  @return
		*    'true' if the usage of texture compression is allowed, else 'false'
		*
		*  @see
		*    - SetTextureCompressionAllowed()
		*/
		inline bool IsTextureCompressionAllowed() const;

		/**
		*  @brief
		*    Sets whether or not the usage of texture compression is allowed
		*
		*  @param[in] bAllowed
		*    'true' if the usage of texture compression is allowed, else 'false', default is 'true'
		*
		*  @note
		*    - Only has an influence on newly loaded textures, not already loaded ones
		*/
		inline void SetTextureCompressionAllowed(bool bAllowed);

		/**
		*  @brief
		*    Reloads all textures
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool ReloadTextures();

		/**
		*  @brief
		*    Creates a new texture
		*
		*  @param[in] sName
		*    Texture name
		*  @param[in] cTextureBuffer
		*    Texture buffer to use
		*
		*  @return
		*    Pointer to the new texture, a null pointer on error
		*
		*  @remarks
		*    A texture wraps up a texture buffer for more comfortable usage within high-level components. In rare situations
		*    one texture buffer needs to be referenced by multiple textures - this is what this method is used for. When creating
		*    a texture with this method, the given texture buffer is just shared and therefore not automatically destroyed
		*    when the texture gets destroyed. If possible, try to avoid using this method because it's somewhat ugly!
		*/
		PLRENDERER_API Texture *CreateTexture(const PLCore::String &sName, TextureBuffer &cTextureBuffer);

		/**
		*  @brief
		*    Creates a texture resource using a texture creator
		*
		*  @param[in] sName
		*    Texture creator class name (for instance "PLRenderer::TextureCreatorTurbulence3D")
		*  @param[in] sParameters
		*    Texture creator parameters. (for instance "XSize='64' YSize='32'")
		*    This parameters depend on the used texture creator.
		*
		*  @return
		*    Pointer to the created resource, a null pointer if there was an error
		*    (maybe unknown class or the class is not derived from 'PLRenderer::TextureCreator')
		*/
		PLRENDERER_API Texture *CreateTexture(const PLCore::String &sName, const PLCore::String &sParameters);


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
		TextureManager(RendererContext &cRendererContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RendererContext	*m_pRendererContext;			/**< Owner renderer context of this manager, always valid! */
		float			 m_fTextureQuality;				/**< Texture quality, 1.0 best, 0.0 worst, default is 1.0 */
		bool			 m_bTextureFitLower;			/**< Take the next lower valid texture size? */
		bool			 m_bTextureMipmapsAllowed;		/**< Are texture mipmaps allowed? Default is 'true'. */
		bool			 m_bTextureCompressionAllowed;	/**< Is texture compression allowed? Default is 'true'. */


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ResourceManager functions     ]
	//[-------------------------------------------------------]
	private:
		virtual Texture *CreateResource(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Texture/TextureManager.inl"


#endif // __PLRENDERER_TEXTUREMANAGER_H__
