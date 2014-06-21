/*********************************************************\
 *  File: Texture.h                                      *
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


#ifndef __PLRENDERER_TEXTURE_H__
#define __PLRENDERER_TEXTURE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Resource.h>
#include <PLMath/Vector3i.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class TextureBuffer;
class TextureManager;
class ResourceHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Texture resource class
*
*  @remarks
*    This generic texture class encapsulates the concrete renderer textures for easier and
*    universal usage. 'Texture animation' is also 'just a texture'. This class can load the data
*    automatically using internally the 'Image'-class - so all image formats the image class
*    supports can also be used as textures.
*
*    The texture class has own special texture compression information standing 'above' the real
*    renderer texture compression. A good example for the usage of this is 'normal map compression'.
*    Because normal maps don't look good when using classic compression, it's better do don't
*    compress them - or to use some tricks so we don't loose to much quality. So, the compression
*    information just indicates for instance the usage like 'DXT5_xGxR' were a texture is compressed
*    as 'DXT5', but the data is stored in a slightly other way. A scene renderer implementation can
*    use this proved information to tell the shader programs about the special usage of the data.
*    When loading a texture, by default the texture data is used as provided. If a 'tga' image is
*    used, no GPU compression is used. If a 'dds' image provides 'fitting' compressed data, this
*    data is directly send to the GPU.
*
*    To have some control over the loading and usage of a texture, we provide a XML based file format
*    called 'plt'. If you for instance load in a texture called 'Data/Textures/Soldier_normal.dds', we look whether or
*    not there's a file called 'Soldier_normal.plt' within the same directory. If so, we take the information
*    this file contains into account. A 'plt' file for a compressed normal map may look like this:
*        <?xml version="1.0"?>
*        <Texture Version="1">
*            <General Compression="DXT5_xGxR" />
*        </Texture>
*    We can automatically detect whether or not a texture is compressed, but we CAN'T find out automatically
*    HOW the data is organized internally. In the sample above 'DXT5_xGxR' tells us that 'swizzled DXT5' is
*    used. It's the job of a scene renderer or of YOU as programmer to take this information into account.
*/
class Texture : public PLCore::Resource<Texture> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class TextureManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Texture compression formats
		*/
		enum ECompressionFormat {
			Default			= 0,	/**< Data is used as provided (for instance 'tga' is not compressed, 'dds' may be compressed) */
			DXT1			= 1,	/**< DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) */
			DXT3			= 2,	/**< DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) */
			DXT5			= 3,	/**< DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) */
			DXT5_xGxR		= 4,	/**< Swizzled DXT5 (same as above!) format for normal map compression (red stored in alpha, blue unused) */
			LATC1			= 5,	/**< 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) */
			LATC2			= 6,	/**< 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) */
			LATC2_XYSwizzle = 7,	/**< Alternate XY swizzle LATC2 (same as above!) format for normal map compression (texture can be used just like DXT5_xGxR) */
			None			= 8		/**< No compression is used */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Texture();

		/**
		*  @brief
		*    Returns the texture manager this texture is in
		*
		*  @return
		*     The texture manager this texture is in
		*/
		inline TextureManager &GetTextureManager() const;

		/**
		*  @brief
		*    Returns the texture buffer
		*
		*  @return
		*    The texture buffer, there you can e.g. upload other texture data, can be a null pointer
		*/
		PLRENDERER_API TextureBuffer *GetTextureBuffer() const;

		/**
		*  @brief
		*    Sets the texture buffer
		*
		*  @param[in] pTextureBuffer
		*    Texture buffer to set, can be a null pointer
		*/
		PLRENDERER_API void SetTextureBuffer(TextureBuffer *pTextureBuffer = nullptr);

		/**
		*  @brief
		*    Returns the texture compression hint
		*
		*  @return
		*    The texture compression hint
		*
		*  @remarks
		*    The texture compression hint JUST indicates the format or it's special usage.
		*    It must not match the real internal format of the used renderer texture.
		*/
		inline ECompressionFormat GetCompressionHint() const;

		/**
		*  @brief
		*    Sets the texture compression hint
		*
		*  @param[in] nFormat
		*    The texture compression hint
		*
		*  @see
		*    - GetCompressionHint()
		*/
		inline void SetCompressionHint(ECompressionFormat nFormat = Default);

		/**
		*  @brief
		*    Returns the original texture size
		*
		*  @return
		*    The original texture size
		*
		*  @remarks
		*    If a texture dimension can't be used by the GPU, the texture is scaled automatically
		*    during loading. Use the 'GetOriginalSize()'-function to request the original
		*    texture dimension. Use GetResource() to get the internal renderer texture where
		*    you can request more information about the texture on the GPU.
		*/
		inline const PLMath::Vector3i &GetOriginalSize() const;

		/**
		*  @brief
		*    Makes the texture to the current renderer texture
		*
		*  @param[in] nStage
		*    Which texture stage?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool Bind(PLCore::uint32 nStage = 0) const;


	//[-------------------------------------------------------]
	//[ Public virtual Texture functions                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether or not this texture is animated
		*
		*  @return
		*    'true' if this texture is animated, else 'false'
		*
		*  @remarks
		*    If this is an animated texture (tani filename extension) you can cast to 'TextureAni'
		*    to get the texture animation data.
		*/
		PLRENDERER_API virtual bool IsAnimated() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cManager
		*    Texture manager using this resource
		*  @param[in] sName
		*    Resource name to set
		*/
		Texture(TextureManager &cManager, const PLCore::String &sName);

		/**
		*  @brief
		*    Returns a correct texture size
		*
		*  @param[in] nSize
		*    Size to correct
		*  @param[in] nMinSize
		*    Minimum valid size
		*  @param[in] nMaxSize
		*    Maximum valid size
		*  @param[in] bRectangle
		*    Is this a rectangle texture?
		*
		*  @return
		*    The correct texture size
		*/
		PLCore::uint32 GetCorrectTextureSize(PLCore::uint32 nSize, PLCore::uint32 nMinSize, PLCore::uint32 nMaxSize, bool bRectangle) const;

		/**
		*  @brief
		*    Destroys the used texture buffer
		*/
		void DestroyTextureBuffer();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ResourceHandler	   *m_pTextureBufferHandler;	/**< Renderer texture buffer resource handler (NEVER a null pointer!) */
		bool				m_bShareTextureBuffer;		/**< If 'true', do not delete the texture buffer by yourself! */
		ECompressionFormat  m_nCompressionHint;			/**< Compression hint */
		PLMath::Vector3i    m_vOriginalSize;			/**< Original texture size */


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual bool LoadByFilename(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "") override;
		PLRENDERER_API virtual bool SaveByFilename(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "") override;
		PLRENDERER_API virtual bool SaveByFile(PLCore::File &cFile, const PLCore::String &sParams = "", const PLCore::String &sMethod = "") override;
		PLRENDERER_API virtual bool Unload() override;
		PLRENDERER_API virtual PLCore::String GetLoadableTypeName() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Texture/Texture.inl"


#endif // __PLRENDERER_TEXTURE_H__
