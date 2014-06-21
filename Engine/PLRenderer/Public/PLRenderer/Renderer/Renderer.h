/*********************************************************\
 *  File: Renderer.h                                     *
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


#ifndef __PLRENDERER_RENDERER_H__
#define __PLRENDERER_RENDERER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include <PLGraphics/Color/Color4.h>
#include "PLRenderer/Renderer/Types.h"
#include "PLRenderer/Renderer/SurfaceTextureBuffer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2;
	class Vector4;
	class Rectangle;
}
namespace PLGraphics {
	class Image;
}
namespace PLRenderer {
	class RendererContext;
	class SurfaceWindow;
	class SurfaceWindowHandler;
	class TextureBuffer;
	class TextureBuffer1D;
	class TextureBuffer2D;
	class TextureBuffer2DArray;
	class TextureBuffer3D;
	class TextureBufferCube;
	class IndexBuffer;
	class VertexBuffer;
	class OcclusionQuery;
	class FixedFunctions;
	class DrawHelpers;
	class Program;
	class FontManager;
	class ShaderLanguage;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer main class
*/
class Renderer : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RendererContext;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Renderer mode
		*/
		enum EMode {
			ModeFixedFunctions = 0,	/**< The renderer can only use fixed functions */
			ModeShaders        = 1,	/**< The renderer can only use shaders */
			ModeBoth           = 2	/**< The renderer is allowed to use fixed functions as well as shaders */
		};
		pl_enum(EMode)
			pl_enum_value(ModeFixedFunctions,	"The renderer can only use fixed functions")
			pl_enum_value(ModeShaders,			"The renderer can only use shaders")
			pl_enum_value(ModeBoth,				"The renderer is allowed to use fixed functions as well as shaders")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, Renderer, "PLRenderer", PLCore::Object, "Abstract renderer main class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public virtual Renderer functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether or not the renderer instance was initialized successfully within it's constructor
		*
		*  @return
		*    'true' if the renderer instance was initialized successfully within it's constructor, else 'false'
		*/
		virtual bool IsInitialized() const = 0;

		/**
		*  @brief
		*    Returns the owner renderer context
		*
		*  @return
		*    Reference to the owner renderer context
		*/
		virtual RendererContext &GetRendererContext() const = 0;

		/**
		*  @brief
		*    Returns the renderer API (for example 'OpenGL' or 'Direct3D')
		*
		*  @param[out] pnVersion
		*    Version information, can be a null pointer
		*
		*  @return
		*    Renderer API
		*
		*  @remarks
		*    The content of 'nVersion' depends on the used API.
		*    For 'OpenGL', 'nVersion' can for example be '21' for 'OpenGL 2.1'.
		*    For 'Direct3D', 'nVersion' can for example be '900' for 'Direct3D 9'.
		*/
		virtual PLCore::String GetAPI(PLCore::uint32 *pnVersion = nullptr) const = 0;

		/**
		*  @brief
		*    Returns the renderer vendor (for example 'ATI Technologies Inc.')
		*
		*  @return
		*    Renderer vendor
		*/
		virtual PLCore::String GetVendor() const = 0;

		/**
		*  @brief
		*    Returns the mode the renderer is running in
		*
		*  @return
		*    The mode the renderer is running in
		*/
		virtual EMode GetMode() const = 0;

		/**
		*  @brief
		*    Returns the name of the default shader language of the renderer
		*
		*  @return
		*    The name of the default shader language of the renderer (for example "GLSL" or "Cg"), the string can be empty, too
		*/
		virtual PLCore::String GetDefaultShaderLanguage() const = 0;

		/**
		*  @brief
		*    Returns a shader language instance
		*
		*  @param[in] sShaderLanguage
		*    The name of the shader language (for example "GLSL" or "Cg"), if empty string,
		*    the default renderer shader language is used (see GetDefaultShaderLanguage())
		*
		*  @return
		*    The shader language instance (do NOT delete it!), a null pointer on error
		*/
		virtual ShaderLanguage *GetShaderLanguage(const PLCore::String &sShaderLanguage = "") = 0;

		/**
		*  @brief
		*    Returns the fixed functions renderer interface
		*
		*  @return
		*    The fixed functions renderer interface, can be a null pointer
		*
		*  @note
		*    - A legacy renderer interface for previously build in features in old graphics APIs and GPUs
		*    - A renderer backend is not enforced to implement this interface, so, you have to take it into
		*      account that this function just returns a null pointer
		*    - Do NOT delete the instance of the interface!
		*/
		virtual FixedFunctions *GetFixedFunctions() const = 0;

		/**
		*  @brief
		*    Returns the font manager interface
		*
		*  @return
		*    The font manager interface
		*/
		virtual FontManager &GetFontManager() const = 0;

		/**
		*  @brief
		*    Returns the draw helpers interface
		*
		*  @return
		*    The draw helpers interface
		*/
		virtual DrawHelpers &GetDrawHelpers() const = 0;

		/**
		*  @brief
		*    Backups the device objects
		*
		*  @note
		*    - Used for instance if the display mode is changed to backup/restore
		*      all device objects (like texture buffers, renderer buffers etc.)
		*    - Normally only used inside the renderer backend!
		*/
		virtual void BackupDeviceObjects() = 0;

		/**
		*  @brief
		*    Restores the device objects
		*
		*  @see
		*    - BackupDeviceObjects()
		*/
		virtual void RestoreDeviceObjects() = 0;

		/**
		*  @brief
		*    Returns the number of all available display modes
		*
		*  @return
		*    Number of all available display modes
		*
		*  @remarks
		*    Be careful when using one of the 'all available display modes' directly... because it can
		*    happen that even if a display mode received by the system is listed, it may not work properly
		*    within fullscreen mode and you get for example just a black window.
		*/
		virtual PLCore::uint32 GetNumOfDisplayModes() const = 0;

		/**
		*  @brief
		*    Returns a display mode
		*
		*  @param[in] nIndex
		*    Index of the display mode to return (0..'GetNumOfDisplayModes()')
		*
		*  @return
		*    The requested display mode, (do NOT delete the display mode!) a null pointer on error
		*
		*  @see
		*    - GetNumOfDisplayModes()
		*/
		virtual const DisplayMode *GetDisplayMode(PLCore::uint32 nIndex) const = 0;

		/**
		*  @brief
		*    Returns the renderer capabilities
		*
		*  @return
		*    The renderer capabilities
		*/
		virtual const Capabilities &GetCapabilities() const = 0;

		/**
		*  @brief
		*    Returns whether or not the given size is valid for a 1D texture buffer
		*
		*  @param[in] nSize
		*    Size to check
		*
		*  @return
		*    'true' if the given size is valid, else 'false'
		*
		*  @note
		*    - A size is valid if nSize<=GetCapabilities().nMaxTextureBufferSize and nSize>0 and
		*      the size is a power of two (or GetCapabilities().bTextureBufferNonPowerOfTwo is "true")
		*/
		virtual bool IsValidTextureBuffer1DSize(int nSize) const = 0;

		/**
		*  @brief
		*    Returns whether or not the given size is valid for a 2D texture buffer
		*
		*  @param[in] nSize
		*    Size to check
		*
		*  @return
		*    'true' if the given size is valid, else 'false'
		*
		*  @note
		*    - A size is valid if nSize<=GetCapabilities().nMaxTextureBufferSize and nSize>0 and
		*      the size is a power of two (or GetCapabilities().bTextureBufferNonPowerOfTwo is "true")
		*/
		virtual bool IsValidTextureBuffer2DSize(int nSize) const = 0;

		/**
		*  @brief
		*    Returns whether or not the given size is valid for a rectangle texture buffer
		*
		*  @param[in] nSize
		*    Size to check
		*
		*  @return
		*    'true' if the given size is valid, else 'false'
		*
		*  @note
		*    - A size is valid if nSize<=GetCapabilities().nMaxRectangleTextureBufferSize and nSize>0
		*/
		virtual bool IsValidTextureBufferRectangleSize(int nSize) const = 0;

		/**
		*  @brief
		*    Returns whether or not the given size is valid for a 3D texture buffer
		*
		*  @param[in] nSize
		*    Size to check
		*
		*  @return
		*    'true' if the given size is valid, else 'false'
		*
		*  @note
		*    - A size is valid if nSize<=GetCapabilities().nMax3DTextureBufferSize and nSize>0 and
		*      the size is a power of two (or GetCapabilities().bTextureBufferNonPowerOfTwo is "true")
		*/
		virtual bool IsValidTextureBuffer3DSize(int nSize) const = 0;

		/**
		*  @brief
		*    Returns whether or not the given size is valid for a cube texture buffer
		*
		*  @param[in] nSize
		*    Size to check
		*
		*  @return
		*    'true' if the given size is valid, else 'false'
		*
		*  @note
		*    - A size is valid if nSize<=GetCapabilities().nMaxCubeTextureBufferSize and nSize>0 and
		*      the size is a power of two (or GetCapabilities().bTextureBufferNonPowerOfTwo is "true")
		*/
		virtual bool IsValidTextureBufferCubeSize(int nSize) const = 0;

		/**
		*  @brief
		*    Returns the renderer statistics
		*
		*  @return
		*    The renderer statistics
		*/
		virtual const Statistics &GetStatistics() const = 0;

		/**
		*  @brief
		*    Returns the offset required to line up texel origins with pixels origins
		*
		*  @return
		*    The pixel origin to texel origin offset, usually (0.0, 0.0) or (-0.5, -0.5)
		*
		*  @note
		*    - OpenGL/D3D10/D3D11 have no offset while D3D9 has an offset of -0.5 (see http://msdn.microsoft.com/en-us/library/bb219690%28VS.85%29.aspx)
		*/
		virtual const PLMath::Vector2 &GetTexelToPixelOffset() const = 0;

		/**
		*  @brief
		*    Updates the renderer
		*
		*  @note
		*    - All renderer surfaces of this renderer will be updated
		*/
		virtual void Update() = 0;

		/**
		*  @brief
		*    Resets all render states, texture units etc. to default
		*
		*  @note
		*    - Avoid calling this function frequently because this can be a performance hit
		*    - Internally this functions are called:
		*      SetColor(), ResetRenderStates(), ResetTransformStates(), ResetSamplerStates(), SetTextureBuffer(),
		*      SetIndexBuffer(), SetViewport(), SetScissorRect(), SetColorMask(), SetProgram(),
		*      FixedFunctions::Reset(), DrawHelpers::End2DMode(), DrawHelpers::Set2DZValue()
		*/
		virtual void Reset() = 0;

		//[-------------------------------------------------------]
		//[ Surfaces                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of surfaces
		*
		*  @return
		*    Number of surfaces
		*/
		virtual PLCore::uint32 GetNumOfSurfaces() const = 0;

		/**
		*  @brief
		*    Returns a renderer surface
		*
		*  @param[in] nIndex
		*    Index of the surface to return
		*
		*  @return
		*    The surface at the given index, a null pointer on error
		*/
		virtual Surface *GetSurface(PLCore::uint32 nIndex = 0) const = 0;

		/**
		*  @brief
		*    Adds a renderer surface to the renderer
		*
		*  @param[in] cSurface
		*    Renderer surface to add
		*
		*  @return
		*    'true' if all went fine and the renderer surface was added to the renderer,
		*    else 'false'
		*
		*  @note
		*    - The renderer surface itself is only added to the renderers list of surfaces!
		*/
		virtual bool AddSurface(Surface &cSurface) = 0;

		/**
		*  @brief
		*    Removes a renderer surface from the renderer
		*
		*  @param[in] cSurface
		*    Renderer surface to remove
		*
		*  @return
		*    'true' if all went fine and the renderer surface was removed from the
		*    renderer, else 'false' (maybe the surface isn't in the renderer)
		*
		*  @note
		*    - The renderer surface itself isn't destroyed, it is just removed
		*      from the renderers list of surfaces!
		*/
		virtual bool RemoveSurface(Surface &cSurface) = 0;

		/**
		*  @brief
		*    Creates a surface painter
		*
		*  @param[in] sClass
		*    Class name of the surface painter to create
		*
		*  @return
		*    The created surface painter, a null pointer on error (YOU have to destroy the object!)
		*/
		virtual SurfacePainter *CreateSurfacePainter(const PLCore::String &sClass) = 0;

		//[-------------------------------------------------------]
		//[ Resources                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of resources
		*
		*  @return
		*    Number of resources
		*/
		virtual PLCore::uint32 GetNumOfResources() const = 0;

		/**
		*  @brief
		*    Returns a renderer resource
		*
		*  @param[in] nIndex
		*    Index of the renderer resource to return
		*
		*  @return
		*    The resource at the given index, a null pointer on error
		*/
		virtual Resource *GetResource(PLCore::uint32 nIndex = 0) const = 0;

		/**
		*  @brief
		*    Adds a renderer resource to the renderer
		*
		*  @param[in] cResource
		*    Renderer resource to add
		*
		*  @return
		*    'true' if all went fine and the renderer resource was added to the renderer,
		*    else 'false'
		*
		*  @note
		*    - The renderer resource itself is only added to the renderers list of resources!
		*/
		virtual bool AddResource(Resource &cResource) = 0;

		/**
		*  @brief
		*    Removes a renderer resource from the renderer
		*
		*  @param[in] cResource
		*    Renderer resource to remove
		*
		*  @return
		*    'true' if all went fine and the renderer resource was removed from the
		*    renderer, else 'false' (maybe the resource isn't in the renderer)
		*
		*  @note
		*    - The renderer resource itself isn't destroyed, it is just removed
		*      from the renderers list of resources!
		*/
		virtual bool RemoveResource(Resource &cResource) = 0;

		//[-------------------------------------------------------]
		//[ Create surface/resource                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Creates renderer surface window
		*
		*  @param[in] cHandler
		*    Renderer surface handler the new renderer surface is assigned with
		*  @param[in] nNativeWindowHandle
		*    Handle to the native window the renderer surface is assigned with
		*  @param[in] sDisplayMode
		*    Display mode information
		*  @param[in] bFullscreen
		*    Fullscreen mode?
		*
		*  @return
		*    The created renderer surface, a null pointer on error
		*
		*  @note
		*    - The new created renderer surface is added to this renderer automatically
		*    - The renderer surface handler is required because this kind of surface is assigned
		*      with a window and if the surface or window is lost they have to be informed
		*/
		virtual SurfaceWindow *CreateSurfaceWindow(SurfaceWindowHandler &cHandler, PLCore::handle nNativeWindowHandle, const DisplayMode &sDisplayMode, bool bFullscreen = false) = 0;

		/**
		*  @brief
		*    Creates renderer 2D texture buffer surface
		*
		*  @param[in] vSize
		*    Size of renderer texture buffer surface
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*  @param[in] nFlags
		*    Texture buffer surface flags (see SurfaceTextureBuffer::EFlags)
		*  @param[in] nMaxColorTargets
		*    Maximum number of color render targets. This must be at least 1 - main renderer
		*    target color. Have a look at SetColorRenderTarget()
		*
		*  @return
		*    The created renderer surface, a null pointer on error
		*
		*  @note
		*    - The new created renderer surface is added to this renderer automatically
		*/
		virtual SurfaceTextureBuffer *CreateSurfaceTextureBuffer2D(const PLMath::Vector2i &vSize, TextureBuffer::EPixelFormat nFormat,
																   PLCore::uint32 nFlags = SurfaceTextureBuffer::Depth | SurfaceTextureBuffer::Stencil,
																   PLCore::uint8 nMaxColorTargets = 1) = 0;

		/**
		*  @brief
		*    Creates renderer rectangle texture buffer surface
		*
		*  @param[in] vSize
		*    Size of renderer texture buffer surface
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*  @param[in] nFlags
		*    Texture buffer surface flags (see SurfaceTextureBuffer::EFlags)
		*  @param[in] nMaxColorTargets
		*    Maximum number of color render targets. This must be at least 1 - main renderer
		*    target color. Have a look at SetColorRenderTarget()
		*
		*  @return
		*    The created renderer surface, a null pointer on error
		*
		*  @note
		*    - The new created renderer surface is added to this renderer automatically
		*    - Additional color target texture buffers must be created with TextureBuffer::RenderTarget in order to
		*      work as render targets
		*/
		virtual SurfaceTextureBuffer *CreateSurfaceTextureBufferRectangle(const PLMath::Vector2i &vSize, TextureBuffer::EPixelFormat nFormat,
																		  PLCore::uint32 nFlags = SurfaceTextureBuffer::Depth | SurfaceTextureBuffer::Stencil,
																		  PLCore::uint8 nMaxColorTargets = 1) = 0;

		/**
		*  @brief
		*    Creates renderer cube texture buffer surface
		*
		*  @param[in] nSize
		*    Size of renderer texture buffer surface
		*  @param[in] nFormat
		*    Texture buffer pixel format
		*  @param[in] nFlags
		*    Texture buffer surface flags (see SurfaceTextureBuffer::EFlags)
		*
		*  @return
		*    The created renderer surface, a null pointer on error
		*
		*  @note
		*    - The new created renderer surface is added to this renderer automatically
		*/
		virtual SurfaceTextureBuffer *CreateSurfaceTextureBufferCube(PLCore::uint16 nSize, TextureBuffer::EPixelFormat nFormat,
																	 PLCore::uint32 nFlags = SurfaceTextureBuffer::Depth | SurfaceTextureBuffer::Stencil) = 0;

		/**
		*  @brief
		*    Creates a 1D texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags, TextureBuffer::RenderTarget has no effect (see TextureBuffer::EFlags)
		*
		*  @return
		*    The created 1D renderer texture buffer, a null pointer on error
		*    (maybe the image size is no power of two?)
		*
		*  @note
		*    - If the usage of 'nInternalFormat' failed - for instance texture buffer compression is not
		*      supported for this texture buffer type, the renderer will try to find a valid format by your self.
		*      Use the function TextureBuffer::GetFormat() to get the 'resulting' used format.
		*    - If the given image is pre compressed, but the compressed format is NOT supported by the renderer
		*      the uncompressed image is used instead.
		*    - If an desired internal format is given, the 'Compression' flag (see TextureBuffer::EFlags) is ignored.
		*    - If the 'Mipmaps' flag (see TextureBuffer::EFlags) is set and the given image doesn't provide any mipmaps,
		*      the mipmaps are created automatically. If the image comes with any mipmaps, this mipmaps MUST
		*      define all mipmap levels down to 1x1 otherwise the texture buffer is invalid when you try to use any min
		*      filter that uses mipmaps. (the OpenGL renderer normally uses white color when invalid/incomplete texture buffer is enabled)
		*/
		virtual TextureBuffer1D *CreateTextureBuffer1D(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown, PLCore::uint32 nFlags = TextureBuffer::Mipmaps) = 0;

		/**
		*  @brief
		*    Creates a 2D texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags (see TextureBuffer::EFlags)
		*
		*  @return
		*    The created 2D renderer texture buffer, a null pointer on error
		*    (maybe the image size is no power of two?)
		*
		*  @see
		*    - CreateTextureBuffer1D()
		*/
		virtual TextureBuffer2D *CreateTextureBuffer2D(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown, PLCore::uint32 nFlags = TextureBuffer::Mipmaps) = 0;

		/**
		*  @brief
		*    Creates a 2D array texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags (see TextureBuffer::EFlags)
		*
		*  @return
		*    The created 2D array renderer texture buffer, a null pointer on error
		*    (maybe the image size is no power of two?)
		*
		*  @see
		*    - CreateTextureBuffer1D()
		*/
		virtual TextureBuffer2DArray *CreateTextureBuffer2DArray(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown, PLCore::uint32 nFlags = TextureBuffer::Mipmaps) = 0;

		/**
		*  @brief
		*    Creates a rectangle texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags, TextureBuffer::Mipmaps has no effect (see TextureBuffer::EFlags)
		*
		*  @return
		*    The created rectangle renderer texture buffer, a null pointer on error
		*
		*  @remarks
		*    Please note that it's up to the implementation to decide whether there's a special TextureBufferRectangle implementation
		*    which can deal with non-power-of-two texture data, or if TextureBuffer2D etc. can deal with non-power-of-two texture as well.
		*    (Capabilities::bTextureBufferNonPowerOfTwo = true)
		*    For instance, OpenGL ES 2.0 has no special "rectangle"/"non-power-of-two" textures. GL_TEXTURE_2D can also be used with
		*    non-power-of-two texture data, but non-power-of-two textures have restrictions on the allowed texture wrap modes and filters
		*    (the texture wrap mode must be GL_CLAMP_TO_EDGE and the minification filter must be GL_NEAREST or GL_LINEAR). As a result, usually
		*    when using GLSL, rectangle textures are accessed by using sampler2DRect/texture2DRect instead of sampler2D/texture2D, and the
		*    texture coordinates are not normalized. But OpenGL ES 2.0 makes no difference between this two texture types - as a result,
		*    a lot of special case handling would be required. To avoid this, we decided that if the implementation doesn't make a difference,
		*    "CreateTextureBufferRectangle()" should be able to return TextureBuffer2D or TextureBufferRectangle - this way, no special
		*    case handling is required, just use the textures.

		*  @note
		*    - If rectangle texture buffers are not supported (Capabilities::bTextureBufferRectangle = false)
		*      this function will fail and a null pointer is returned
		*
		*  @see
		*    - CreateTextureBuffer1D()
		*/
		virtual TextureBuffer *CreateTextureBufferRectangle(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown, PLCore::uint32 nFlags = 0) = 0;

		/**
		*  @brief
		*    Creates a 3D texture buffer
		*
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags, TextureBuffer::RenderTarget has no effect (see TextureBuffer::EFlags)
		*
		*  @return
		*    The created 3D renderer texture buffer, a null pointer on error
		*    (maybe the image size is no power of two?)
		*
		*  @note
		*    - If 3D texture buffers are not supported (Capabilities::bTextureBuffer3D = false)
		*      this function will fail and a null pointer is returned
		*
		*  @see
		*    - CreateTextureBuffer1D()
		*/
		virtual TextureBuffer3D *CreateTextureBuffer3D(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown, PLCore::uint32 nFlags = TextureBuffer::Mipmaps) = 0;

		/**
		*  @brief
		*    Creates a cube texture buffer
		*
		*  @param[in] cImage
		*    Cube map image (image with 6 image parts)
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if TextureBuffer::Unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags (see TextureBuffer::EFlags)
		*
		*  @return
		*    The created cube renderer texture buffer, a null pointer on error
		*    (maybe the image size is no power of two?)
		*
		*  @note
		*    - If cube texture buffers are not supported (Capabilities::bTextureBufferCube = false)
		*      this function will fail and a null pointer is returned
		*
		*  @see
		*    - CreateTextureBuffer1D()
		*/
		virtual TextureBufferCube *CreateTextureBufferCube(PLGraphics::Image &cImage, TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown,PLCore::uint32 nFlags = TextureBuffer::Mipmaps) = 0;

		/**
		*  @brief
		*    Creates an index buffer
		*
		*  @return
		*    The created index buffer, a null pointer on error
		*
		*  @remarks
		*    To use index buffers, create an index buffer, lock it, fill it with indices, unlock it, pass it to
		*    SetIndexBuffer(), set up the vertices, set up the vertex shader program, and call DrawIndexedPrimitive() for rendering.
		*/
		virtual IndexBuffer *CreateIndexBuffer() = 0;

		/**
		*  @brief
		*    Creates a vertex buffer
		*
		*  @return
		*     The created vertex buffer, a null pointer on error
		*
		*  @remarks
		*    To use vertex buffers, create an vertex buffer, setup the vertex elements,
		*    allocate the vertex buffer, lock it, fill it with data, unlock it.
		*    For shader based rendering, use the ProgramAttribute interface to assign an vertex buffer attribute to an vertex shader attribute.
		*    For fixed functions rendering, pass the vertex buffer it to FixedFunctions::SetVertexBuffer().
		*/
		virtual VertexBuffer *CreateVertexBuffer() = 0;

		/**
		*  @brief
		*    Creates an occlusion query
		*
		*  @return
		*    The created occlusion query, a null pointer on error
		*
		*  @note
		*    - If occlusion query is not supported by the hardware, an occlusion query object
		*      is returned, too. But the test will always succeed. (see Capabilities::bOcclusionQuery)
		*/
		virtual OcclusionQuery *CreateOcclusionQuery() = 0;

		//[-------------------------------------------------------]
		//[ States                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a default render state
		*
		*  @param[in] nState
		*    Render state to return the default value from
		*
		*  @return
		*    The default state
		*/
		virtual PLCore::uint32 GetDefaultRenderState(RenderState::Enum nState) const = 0;

		/**
		*  @brief
		*    Resets all render states to default
		*
		*  @see
		*    - GetDefaultRenderState()
		*/
		virtual void ResetRenderStates() = 0;

		/**
		*  @brief
		*    Retrieves a render-state value
		*
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the render state enumerated type.
		*
		*  @return
		*    The value of the queried render state variable, < 0 on error
		*
		*  @see
		*    - GetDefaultRenderState()
		*/
		virtual int GetRenderState(RenderState::Enum nState) const = 0;

		/**
		*  @brief
		*    Sets a single render-state parameter
		*
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the render state enumerated type.
		*  @param[in] nValue
		*    New value for the render state to be set. The meaning of
		*    this parameter is dependent on the value specified for nState.
		*    For example, if nState is Shade, the second parameter
		*    must be one member of the Shade enumerated type. (e.g. Shade::Flat)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetDefaultRenderState()
		*/
		virtual bool SetRenderState(RenderState::Enum nState, PLCore::uint32 nValue) = 0;

		/**
		*  @brief
		*    Returns a default sampler state
		*
		*  @param[in] nState
		*    Sampler state to return the default value from
		*
		*  @return
		*    The default state
		*/
		virtual PLCore::uint32 GetDefaultSamplerState(Sampler::Enum nState) const = 0;

		/**
		*  @brief
		*    Resets all sampler states to default
		*
		*  @see
		*    - GetDefaultSamplerState()
		*/
		virtual void ResetSamplerStates() = 0;

		/**
		*  @brief
		*    Retrieves a sampler-state value
		*
		*  @param[in] nStage
		*    Texture stage to get the value from
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the sampler enumerated type.
		*
		*  @return
		*    The value of the queried sampler state variable, < 0 on error
		*
		*  @see
		*    - GetDefaultSamplerState()
		*/
		virtual int GetSamplerState(PLCore::uint32 nStage, Sampler::Enum nState) const = 0;

		/**
		*  @brief
		*    Sets a single sampler-state parameter
		*
		*  @param[in] nStage
		*    Texture stage to set the value
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the sampler enumerated type.
		*  @param[in] nValue
		*    New value for the sampler state to be set. The meaning of
		*    this parameter is dependent on the value specified for nState.
		*    For example, if nState is Sampler::AddressU, the second parameter
		*    must be one member of the Sampler::AddressU enumerated type. (e.g. TextureAddressing::Wrap)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If the state to set is 'Sampler::MipFilter' and the texture buffer at the given texture state
		*      has no mipmaps, the value is internally set to 'TextureFiltering::None' automatically for proper
		*      rendering of the texture buffer.
		*
		*  @see
		*    - GetDefaultSamplerState()
		*/
		virtual bool SetSamplerState(PLCore::uint32 nStage, Sampler::Enum nState, PLCore::uint32 nValue) = 0;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the swap interval (vertical synchronization)
		*
		*  @return
		*    The swap interval
		*
		*  @note
		*    - Please note that the swap interval is only a hint and can be overwritten by the driver settings!
		*/
		virtual PLCore::uint32 GetSwapInterval() const = 0;

		/**
		*  @brief
		*    Sets the swap interval (vertical synchronization)
		*
		*  @param[in] nSwapInterval
		*    The swap interval, default value is 1
		*
		*  @see
		*    - GetSwapInterval()
		*/
		virtual void SetSwapInterval(PLCore::uint32 nSwapInterval = 1) = 0;

		/**
		*  @brief
		*    Begins a scene
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*     (e.g. if BeginScene is called while already in a BeginScene/EndScene pair.
		*     This happens only when BeginScene is called twice without first calling EndScene.
		*
		*  @see
		*    - EndScene()
		*/
		virtual bool BeginScene() = 0;

		/**
		*  @brief
		*    Ends a scene that was begun by calling the BeginScene method
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - When this method succeeds, the scene has been queued up for rendering by the
		*      driver. This is not a synchronous method, so the scene is not guaranteed to
		*      have completed rendering when this method returns.
		*    - When scene rendering begins successfully, you must call this method before
		*      you can call the BeginScene method to start rendering another scene. If a
		*      prior call to BeginScene method fails, the scene did not begin and this method
		*      should not be called.
		*/
		virtual bool EndScene() = 0;

		/**
		*  @brief
		*    Gets the window dimensions of a render target surface onto
		*    which a 3-D volume projects
		*
		*  @param[out] pfMinZ
		*    If not a null pointer, receives minimum z value describing the range of depth values into
		*    which a scene is to be rendered
		*  @param[out] pfMaxZ
		*    If not a null pointer, receives maximum z value describing the range of depth values into
		*    which a scene is to be rendered
		*
		*  @return
		*    The current viewport rectangle
		*
		*  @note
		*    - Rectangle.vMin is the pixel coordinate of the upper-left corner of the
		*      viewport on the render target surface. Unless you want to render to
		*      a subset of the surface, this member can be set to 0.
		*    - Rectangle.vMax define the other corner of the clip volume, in pixels. Unless
		*      you are rendering only to a subset of the surface, this member should be set
		*      to the dimension of the render target surface. If < 0 the dimension of the
		*      current renderer surface is set.
		*    - Usually, applications render to the entire target surface; when rendering on a
		*	   640×480 surface, these cRectangle members from above should be 0, 0, 640, and 480, respectively.
		*    - pfMinZ and pfMaxZ indicate the depth-ranges into which the scene will be rendered and
		*      are not used for clipping. Most applications set these members to 0.0 and 1.0 to enable
		*      the system to render to the entire range of depth values in the depth buffer. In some
		*      cases, you can achieve special effects by using other depth ranges. For instance, to
		*      render a heads-up display in a game, you can set both values to 0.0 to force the system
		*      to render objects in a scene in the foreground, or you might set them both to 1.0 to
		*      render an object that should always be in the background.
		*/
		virtual const PLMath::Rectangle &GetViewport(float *pfMinZ = nullptr, float *pfMaxZ = nullptr) const = 0;

		/**
		*  @brief
		*    Defines the window dimensions of a render target surface onto
		*    which a 3-D volume projects
		*
		*  @param[in] pRectangle
		*    Viewport rectangle, if a null pointer, use the whole render target surface
		*  @param[in] fMinZ
		*    Minimum z value
		*  @param[in] fMaxZ
		*    Maximum z value
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetViewport()
		*/
		virtual bool SetViewport(const PLMath::Rectangle *pRectangle = nullptr, float fMinZ = 0.0f, float fMaxZ = 1.0f) = 0;

		/**
		*  @brief
		*    Gets the scissor rectangle
		*
		*  @return
		*    The current scissor rectangle
		*
		*  @note
		*    - cRectangle.vMin is the pixel coordinate of the upper-left corner of the
		*      scissor rectangle on the render target surface. Unless you want to render to
		*      a subset of the surface, this member can be set to 0.
		*    - cRectangle.vMax define the other corner of the clip volume, in pixels. Unless
		*      you are rendering only to a subset of the surface, this member should be set
		*      to the dimension of the render target surface. If < 0 the dimension of the
		*      current renderer viewport is set.
		*    - The scissor rectangle is used as a rectangular clipping region
		*    - The default scissor rectangle is the full viewport
		*    - The scissor test is enabled and disabled using SetRenderState() with argument
		*/
		virtual const PLMath::Rectangle &GetScissorRect() const = 0;

		/**
		*  @brief
		*    Sets the scissor rectangle
		*
		*  @param[in] pRectangle
		*    Viewport rectangle, if a null pointer, use the whole set viewport
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetScissorRect()
		*/
		virtual bool SetScissorRect(const PLMath::Rectangle *pRectangle = nullptr) = 0;

		/**
		*  @brief
		*    Gets the depth bounds
		*
		*  @param[out] fZMin
		*    Will receive the Z minimum value
		*  @param[out] fZMax
		*    Will receive the Z maximum value
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe not supported by the hardware)
		*
		*  @note
		*    - The depth bounds test is a kind of scissor rect technique to reject
		*      untouched fragments
		*    - Has only an effect if Capabilities::bDepthBoundsTest is true
		*    - If fZMin is 0.0 and fZMax is 1.0 no depth bounds test will be performed (default)
		*/
		virtual bool GetDepthBounds(float &fZMin, float &fZMax) const = 0;

		/**
		*  @brief
		*    Sets the depth bounds
		*
		*  @param[in] fZMin
		*    Z minimum value (range: 0.0-1.0, must be less than or equal to fZMax)
		*  @param[in] fZMax
		*    Z maximum value (range: 0.0-1.0, must be greater than or equal to fZMin)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe not supported by the hardware)
		*
		*  @see
		*    - GetDepthBounds()
		*/
		virtual bool SetDepthBounds(float fZMin = 0.0f, float fZMax = 1.0f) = 0;

		/**
		*  @brief
		*    Returns the color mask
		*
		*  @param[out] bRed
		*    Will receive whether the red component is written
		*  @param[out] bGreen
		*    Will receive whether the green component is written
		*  @param[out] bBlue
		*    Will receive whether the blue component is written
		*  @param[out] bAlpha
		*    Will receive whether the alpha component is written
		*
		*  @note
		*    - With the color mask you can decide which color components are drawn,
		*      if you e.g. only want to fill the depth buffer you can set all components
		*      to false
		*    - If no color components are written, some GPU's are optimized to use for instance the
		*      z buffer much more performant
		*    - By default all color components are written
		*/
		virtual void GetColorMask(bool &bRed, bool &bGreen, bool &bBlue, bool &bAlpha) const = 0;

		/**
		*  @brief
		*    Sets the color mask
		*
		*  @param[in] bRed
		*    Write red component?
		*  @param[in] bGreen
		*    Write green component?
		*  @param[in] bBlue
		*    Write blue component?
		*  @param[in] bAlpha
		*    Write alpha component?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetColorMask()
		*/
		virtual bool SetColorMask(bool bRed = true, bool bGreen = true, bool bBlue = true, bool bAlpha = true) = 0;

		/**
		*  @brief
		*    Clears the viewport to a specified RGBA color, clears the depth buffer,
		*    and erases the stencil buffer
		*
		*  @param[in] nFlags
		*    Flags that indicate what should be cleared. This parameter can be any
		*    combination of the following flags, but at least one flag must be used:
		*    Clear::Color, Clear::ZBuffer and Clear::Stencil, see Clear flags
		*  @param[in] cColor
		*    RGBA clear color (used if Clear::Color is set)
		*  @param[in] fZ
		*    Z clear value. (if Clear::ZBuffer is set)
		*    This parameter can be in the range from 0.0 through 1.0. A value of 0.0
		*    represents the nearest distance to the viewer, and 1.0 the farthest distance.
		*  @param[in] nStencil
		*    Value to clear the stencil-buffer with. This parameter can be in the range from
		*    0 through 2^n–1, where n is the bit depth of the stencil buffer.
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If scissor is disabled or set to the full viewport size some GPU's may be able to clear more efficient
		*    - The color mask has no effect on the clear operation
		*/
		virtual bool Clear(PLCore::uint32 nFlags = Clear::Color | Clear::ZBuffer,
						   const PLGraphics::Color4 &cColor = PLGraphics::Color4::Black, float fZ = 1.0f,
						   PLCore::uint32 nStencil = 0) = 0;

		//[-------------------------------------------------------]
		//[ Get/set current resources                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the current render target
		*
		*  @param[out] pnFace
		*    If not a null pointer, this will receive the current cube map face
		*
		*  @return
		*    Current renderer target, a null pointer if there's no one
		*/
		virtual Surface *GetRenderTarget(PLCore::uint8 *pnFace = nullptr) const = 0;

		/**
		*  @brief
		*    Returns the current color render target
		*
		*  @param[in] nColorIndex
		*    Color index of the color render target to return
		*
		*  @return
		*    Current color renderer target, a null pointer if there's no one
		*/
		virtual TextureBuffer *GetColorRenderTarget(PLCore::uint8 nColorIndex = 0) const = 0;

		/**
		*  @brief
		*    Sets the current render target
		*
		*  @param[in] pSurface
		*    Current renderer target, can be a null pointer
		*  @param[in] nFace
		*    Cube map face to render in (0-5) - only used if 'pSurface' is a cube texture buffer render target
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - SetViewport() and SetScissorRect() are called automatically
		*/
		virtual bool SetRenderTarget(Surface *pSurface, PLCore::uint8 nFace = 0) = 0;

		/**
		*  @brief
		*    Sets the current color render target (for multi render targets)
		*
		*  @param[in] pTextureBuffer
		*    Current color renderer target, can be a null pointer
		*  @param[in] nColorIndex
		*    Color index of this render target, if 0 this will set the main color render target of the
		*    current set render target. (see SetRenderTarget())
		*  @param[in] nFace
		*    Cube map face to render in ([TODO])
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid color index of the current render target?)
		*
		*  @note
		*    - This color render targets MUST have the dimension and format of the render target
		*/
		virtual bool SetColorRenderTarget(TextureBuffer *pTextureBuffer, PLCore::uint8 nColorIndex = 0, PLCore::uint8 nFace = 0) = 0;

		/**
		*  @brief
		*    Sets the current depth render target
		*
		*  @param[in] pTextureBuffer
		*    Current depth renderer target, can be a null pointer
		*  @param[in] nFace
		*    Cube map face to render in ([TODO])
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This depth render targets MUST have the dimension and format of the render target
		*/
		virtual bool SetDepthRenderTarget(TextureBuffer *pTextureBuffer, PLCore::uint8 nFace = 0) = 0;

		/**
		*  @brief
		*    Makes a screenshot of the current render target (see GetRenderTarget())
		*
		*  @param[out] cImage
		*    Image which will receive the screenshot data (usually RGBA unsigned byte)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool MakeScreenshot(PLGraphics::Image &cImage) = 0;

		/**
		*  @brief
		*    Gets the current texture buffer at the given stage
		*
		*  @param[in] nStage
		*    Texture stage
		*
		*  @return
		*    The current texture buffer at the given stage, a null pointer if there's no one
		*/
		virtual TextureBuffer *GetTextureBuffer(PLCore::uint32 nStage) const = 0;

		/**
		*  @brief
		*    Sets the current texture buffer at the given stage
		*
		*  @param[in] nStage
		*    Texture stage. (0 - max texture stages -> see Capabilities::nMaxTextureUnits)
		*    < 0 = set all available texture stages to this setting.
		*  @param[in] pTextureBuffer
		*    The texture buffer which should be set, a null pointer if no texture buffer should be set
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe this is already the current
		*    texture buffer at the given stage)
		*
		*  @see
		*    - GetTransformState(), bOriginal parameter description
		*/
		virtual bool SetTextureBuffer(int nStage = -1, TextureBuffer *pTextureBuffer = nullptr) = 0;

		/**
		*  @brief
		*    Gets the current index buffer
		*
		*  @return
		*    The current index buffer, a null pointer if there's no one
		*/
		virtual IndexBuffer *GetIndexBuffer() const = 0;

		/**
		*  @brief
		*    Sets the current index buffer
		*
		*  @param[in] pIndexBuffer
		*    The index buffer which should be set, a null pointer if no index buffer should be set
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe this is already the current index buffer)
		*/
		virtual bool SetIndexBuffer(IndexBuffer *pIndexBuffer = nullptr) = 0;

		/**
		*  @brief
		*    Returns the currently used program
		*
		*  @return
		*    The currently used program, can be a null pointer
		*/
		virtual Program *GetProgram() const = 0;

		/**
		*  @brief
		*    Sets the currently used program
		*
		*  @param[in] pProgram
		*    Program to use, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false' (invalid program?)
		*/
		virtual bool SetProgram(Program *pProgram = nullptr) = 0;

		//[-------------------------------------------------------]
		//[ Draw call                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Renders the specified geometric primitive, based on an array of vertices
		*
		*  @param[in] nType
		*    Member of the primitive enumerated type, describing the type of primitive to render
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call
		*  @param[in] nNumVertices
		*    Number of vertices used during this call
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - DrawPrimitives() fails if no vertex array is set
		*/
		virtual bool DrawPrimitives(Primitive::Enum nType, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) = 0;

		/**
		*  @brief
		*    Renders the specified geometric primitive, based on indexing into an array of vertices
		*
		*  @param[in] nType
		*    Member of the primitive enumerated type, describing the type of primitive to render, e.g. "Primitive::TriangleList"
		*  @param[in] nMinIndex
		*    Minimum vertex index for vertices used during this call, usually "0"
		*  @param[in] nMaxIndex
		*    Maximum vertex index for vertices used during this call, usually "pVertexBuffer->GetNumOfElements() - 1" when "pVertexBuffer" is your currently used vertex buffer
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call, usually "0"
		*  @param[in] nNumVertices
		*    Number of vertices used during this call, usually "pIndexBuffer->GetNumOfElements()" when "pIndexBuffer" is your currently used index buffer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This method draws indexed primitives from the current set of data input streams
		*    - The nMinIndex and nMaxIndex parameters specify the range of vertex indices used for each DrawIndexedPrimitive()
		*      call. These are used to optimize vertex processing of indexed primitives by processing a sequential range of
		*      vertices prior to indexing into these vertices. It is invalid for any indices used during this call to reference
		*      any vertices outside of this range.
		*    - DrawIndexedPrimitive() fails if no index and/or vertex array is set
		*    - The Primitive::PointList member of the primitive enumerated type is not supported and is not a valid type for this method
		*/
		virtual bool DrawIndexedPrimitives(Primitive::Enum nType, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) = 0;

		/**
		*  @brief
		*    Renders the specified geometric patches, based on an array of vertices
		*
		*  @param[in] nVerticesPerPatch
		*    Number of vertices that will be used to make up a single patch primitive
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call
		*  @param[in] nNumVertices
		*    Number of vertices used during this call
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - DrawPatches() fails if no vertex array is set
		*    - Only relevent when using tessellation control and tessellation evaluation shaders
		*/
		virtual bool DrawPatches(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) = 0;

		/**
		*  @brief
		*    Renders the specified geometric patches, based on indexing into an array of vertices
		*
		*  @param[in] nVerticesPerPatch
		*    Number of vertices that will be used to make up a single patch primitive
		*  @param[in] nMinIndex
		*    Minimum vertex index for vertices used during this call, usually "0"
		*  @param[in] nMaxIndex
		*    Maximum vertex index for vertices used during this call, usually "pVertexBuffer->GetNumOfElements() - 1" when "pVertexBuffer" is your currently used vertex buffer
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call, usually "0"
		*  @param[in] nNumVertices
		*    Number of vertices used during this call, usually "pIndexBuffer->GetNumOfElements()" when "pIndexBuffer" is your currently used index buffer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This method draws indexed patches from the current set of data input streams
		*    - The nMinIndex and nMaxIndex parameters specify the range of vertex indices used for each DrawIndexedPatches()
		*      call. These are used to optimize vertex processing of indexed patches by processing a sequential range of
		*      vertices prior to indexing into these vertices. It is invalid for any indices used during this call to reference
		*      any vertices outside of this range.
		*    - DrawIndexedPatches() fails if no index and/or vertex array is set
		*    - Only relevent when using tessellation control and tessellation evaluation shaders
		*/
		virtual bool DrawIndexedPatches(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices) = 0;

		//[-------------------------------------------------------]
		//[ Draw call with multiple primitive instances           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Renders the specified geometric primitive by using instancing, based on an array of vertices
		*
		*  @param[in] nType
		*    Member of the primitive enumerated type, describing the type of primitive to render
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call
		*  @param[in] nNumVertices
		*    Number of vertices used during this call
		*  @param[in] nNumOfInstances
		*    Number of geometric primitive instances generated by this call
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - DrawPrimitivesInstanced() fails if no vertex array is set
		*/
		virtual bool DrawPrimitivesInstanced(Primitive::Enum nType, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) = 0;

		/**
		*  @brief
		*    Renders the specified geometric primitive by using instancing, based on indexing into an array of vertices
		*
		*  @param[in] nType
		*    Member of the primitive enumerated type, describing the type of primitive to render, e.g. "Primitive::TriangleList"
		*  @param[in] nMinIndex
		*    Minimum vertex index for vertices used during this call, usually "0"
		*  @param[in] nMaxIndex
		*    Maximum vertex index for vertices used during this call, usually "pVertexBuffer->GetNumOfElements() - 1" when "pVertexBuffer" is your currently used vertex buffer
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call, usually "0"
		*  @param[in] nNumVertices
		*    Number of vertices used during this call, usually "pIndexBuffer->GetNumOfElements()" when "pIndexBuffer" is your currently used index buffer
		*  @param[in] nNumOfInstances
		*    Number of geometric primitive instances generated by this call
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This method draws indexed primitives from the current set of data input streams
		*    - The nMinIndex and nMaxIndex parameters specify the range of vertex indices used for each DrawIndexedPrimitivesInstanced()
		*      call. These are used to optimize vertex processing of indexed primitives by processing a sequential range of
		*      vertices prior to indexing into these vertices. It is invalid for any indices used during this call to reference
		*      any vertices outside of this range.
		*    - DrawIndexedPrimitivesInstanced() fails if no index and/or vertex array is set
		*    - The Primitive::PointList member of the primitive enumerated type is not supported and is not a valid type for this method
		*/
		virtual bool DrawIndexedPrimitivesInstanced(Primitive::Enum nType, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) = 0;

		/**
		*  @brief
		*    Renders the specified geometric patches by using instancing, based on an array of vertices
		*
		*  @param[in] nVerticesPerPatch
		*    Number of vertices that will be used to make up a single patch primitive
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call
		*  @param[in] nNumVertices
		*    Number of vertices used during this call
		*  @param[in] nNumOfInstances
		*    Number of geometric primitive instances generated by this call
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - DrawPatchesInstanced() fails if no vertex array is set
		*    - Only relevent when using tessellation control and tessellation evaluation shaders
		*/
		virtual bool DrawPatchesInstanced(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) = 0;

		/**
		*  @brief
		*    Renders the specified geometric patches by using instancing, based on indexing into an array of vertices
		*
		*  @param[in] nVerticesPerPatch
		*    Number of vertices that will be used to make up a single patch primitive
		*  @param[in] nMinIndex
		*    Minimum vertex index for vertices used during this call, usually "0"
		*  @param[in] nMaxIndex
		*    Maximum vertex index for vertices used during this call, usually "pVertexBuffer->GetNumOfElements() - 1" when "pVertexBuffer" is your currently used vertex buffer
		*  @param[in] nStartIndex
		*    Start vertex index for vertices used during this call, usually "0"
		*  @param[in] nNumVertices
		*    Number of vertices used during this call, usually "pIndexBuffer->GetNumOfElements()" when "pIndexBuffer" is your currently used index buffer
		*  @param[in] nNumOfInstances
		*    Number of geometric primitive instances generated by this call
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - This method draws indexed patches from the current set of data input streams
		*    - The nMinIndex and nMaxIndex parameters specify the range of vertex indices used for each DrawIndexedPatchesInstanced()
		*      call. These are used to optimize vertex processing of indexed patches by processing a sequential range of
		*      vertices prior to indexing into these vertices. It is invalid for any indices used during this call to reference
		*      any vertices outside of this range.
		*    - DrawIndexedPatchesInstanced() fails if no index and/or vertex array is set
		*    - Only relevent when using tessellation control and tessellation evaluation shaders
		*/
		virtual bool DrawIndexedPatchesInstanced(PLCore::uint32 nVerticesPerPatch, PLCore::uint32 nMinIndex, PLCore::uint32 nMaxIndex, PLCore::uint32 nStartIndex, PLCore::uint32 nNumVertices, PLCore::uint32 nNumOfInstances) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API Renderer();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Renderer();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Renderer(const Renderer &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Renderer &operator =(const Renderer &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_RENDERER_H__
