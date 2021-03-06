/*********************************************************\
 *  File: Renderer.cpp                                   *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/Tools.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLGraphics/Image/ImageEffects.h>
#include <PLRenderer/Renderer/SurfaceWindowHandler.h>
#include <PLRenderer/Renderer/Backend/DrawHelpersBackend.h>
#ifdef DISABLE_FONT_SUPPORT
	#include <PLRenderer/Renderer/Backend/FontManagerBackend.h>
#else
	#include "PLRendererOpenGL/FontManager.h"
#endif
#include "PLRendererOpenGL/Program.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/SurfaceTextureBuffer.h"
#include "PLRendererOpenGL/TextureBuffer1D.h"
#include "PLRendererOpenGL/TextureBuffer2D.h"
#include "PLRendererOpenGL/TextureBuffer2DArray.h"
#include "PLRendererOpenGL/TextureBufferRectangle.h"
#include "PLRendererOpenGL/TextureBuffer3D.h"
#include "PLRendererOpenGL/TextureBufferCube.h"
#include "PLRendererOpenGL/ShaderLanguage.h"
#include "PLRendererOpenGL/IndexBuffer.h"
#include "PLRendererOpenGL/VertexBuffer.h"
#include "PLRendererOpenGL/OcclusionQuery.h"
#ifdef WIN32
	#include "PLRendererOpenGL/Windows/ContextWindows.h"
#elif APPLE
//	#include "PLRendererOpenGL/MacOSX/ContextMacOSX_X11.h"		// We can also reuse the PLFrontendOS X11 implementation from the Linux port
	#include "PLRendererOpenGL/MacOSX/ContextMacOSX_Cocoa.h"	// This is what we really want to use on Mac OS X to stay out of compatibility trouble
#elif LINUX
	#include "PLRendererOpenGL/Linux/ContextLinux.h"
#endif
#include "PLRendererOpenGL/FixedFunctions.h"
#include "PLRendererOpenGL/Renderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Renderer, "PLRendererOpenGL", PLRenderer::RendererBackend, "OpenGL 1.1 + extensions renderer backend")
	// Constructors
	pl_constructor_6_metadata(DefaultConstructor,	PLCore::handle,	pl_enum_type_def(EMode),	PLCore::uint32,	PLCore::uint32,	PLCore::uint32,	PLCore::String,	"Constructor with renderer mode, Z buffer bits, stencil buffer bits, the number of multisample antialiasing samples per pixel and the default shader language as parameter",	"")
pl_class_metadata_end(Renderer)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Renderer::Renderer(handle nNativeWindowHandle, EMode nMode, uint32 nZBufferBits, uint32 nStencilBits, uint32 nMultisampleAntialiasingSamples, String sDefaultShaderLanguage) : PLRenderer::RendererBackend(nMode),
	m_bInitialized(false),
	m_pContext(nullptr),
	m_pFixedFunctions(nullptr),
	#ifdef DISABLE_FONT_SUPPORT
		m_pFontManager(new PLRenderer::FontManagerBackend(*this)),
	#else
		m_pFontManager(new FontManager(*this)),
	#endif
	m_bSceneRendering(false),
	m_nMultisampleAntialiasingSamples(nMultisampleAntialiasingSamples),
	m_nTextureBufferTypes(nullptr),
	m_ppPrevTextureBuffer(nullptr)
{
	// Ignore the given native window handle

	// Output log information
	#ifdef DISABLE_FONT_SUPPORT
		PL_LOG(Info, "Initialize OpenGL renderer (font support is disabled)")
	#else
		PL_LOG(Info, "Initialize OpenGL renderer")
	#endif

	// Shaders allowed?
	if (GetMode() != ModeFixedFunctions) {
		// Register the available OpenGL shader languages
		ClassManager::GetInstance()->GetClasses(m_lstShaderLanguages, "PLRendererOpenGL::ShaderLanguage", PLCore::Recursive, PLCore::NoBase, PLCore::NoAbstract);
		if (m_lstShaderLanguages.GetNumOfElements()) {
			// Show a log message
			PL_LOG(Info, "Available OpenGL shader languages:")

			// Loop through all found classes
			ConstIterator<const Class*> cIterator = m_lstShaderLanguages.GetConstIterator();
			while (cIterator.HasNext()) {
				// Get the class instance
				const Class *pClass = cIterator.Next();

				// Get the name of the shader language
				const String sShaderLanguage = pClass->GetProperties().Get("ShaderLanguage");

				// Show a log message
				PL_LOG(Info, "- " + sShaderLanguage + " (Class: " + pClass->GetClassName() + ')')

				// Add to shader language map
				m_mapShaderLanguages.Add(sShaderLanguage, pClass);
			}
		} else {
			// There should be at least the shader language GLSL... but to be on the save side, we handle this case here, too
			PL_LOG(Info, "There are no OpenGL shader languages available")
		}

		// If the given desired default shader language is valid, use it - else use GLSL as default shader language
		m_sDefaultShaderLanguage = m_mapShaderLanguages.Get(sDefaultShaderLanguage) ? sDefaultShaderLanguage : "GLSL";
	}

	// Set Z buffer bits and stencil buffer bits capabilities
	m_sCapabilities.nZBufferBits = nZBufferBits;
	m_sCapabilities.nStencilBits = nStencilBits;

	// Init data
	m_bSceneRendering = false;

	// The most important first: Create OpenGL render context
	PL_LOG(Info, "Create OpenGL render context")
	m_pContext = CreateContext();

	// Were we able to get a OpenGL render context?
	if (m_pContext->IsValid()) {
		Extensions &cExtensions = m_pContext->GetExtensions();

		// Check multisample antialiasing support
		if (cExtensions.IsWGL_ARB_multisample() && cExtensions.IsGL_ARB_multisample()) {
			if (m_nMultisampleAntialiasingSamples == 1)
				m_nMultisampleAntialiasingSamples = 0;	// Multisample antialiasing with just one sample per per pixel isn't real multisample, isn't it? :D
		} else {
			// Sorry, there's no multisample antialiasing support
			m_nMultisampleAntialiasingSamples = 0;
		}
		m_sCapabilities.nMultisampleAntialiasingSamples = nMultisampleAntialiasingSamples;

		// Show some OpenGL information
		PL_LOG(Info, "OpenGL information")
		PL_LOG(Info, String("Version: ")	   + reinterpret_cast<const char*>(glGetString(GL_VERSION)))
		PL_LOG(Info, String("Vendor info: ")   + reinterpret_cast<const char*>(glGetString(GL_VENDOR)))
		PL_LOG(Info, String("Renderer info: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER)))

		// Show general OpenGL information
		cExtensions.ShowGeneralOpenGLInformation();

		// Create a list of all available display modes
		if (m_pContext->QueryDisplayModes(m_lstDisplayModeList)) {
			// Setup the renderer capabilities
			SetupCapabilities();

			// Fixed functions allowed?
			if (GetMode() != ModeShaders)
				m_pFixedFunctions = new FixedFunctions(*this);

			// General OpenGL settings
			GeneralSettings();

			// Init wrappers
			InitWrappers();

			// Allocate sampler states
			m_ppnSamplerState = new uint32*[m_sCapabilities.nMaxTextureUnits];
			for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++) {
				m_ppnSamplerState[i] = new uint32[PLRenderer::Sampler::Number];
				MemoryManager::Set(m_ppnSamplerState[i], -1, sizeof(uint32)*PLRenderer::Sampler::Number);
			}

			// Allocate internal sampler states
			m_ppnInternalSamplerState = new uint32*[m_sCapabilities.nMaxTextureUnits];
			for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++) {
				m_ppnInternalSamplerState[i] = new uint32[PLRenderer::Sampler::Number];
				MemoryManager::Set(m_ppnInternalSamplerState[i], -1, sizeof(uint32)*PLRenderer::Sampler::Number);
			}

			// Allocate current stuff
			m_ppCurrentTextureBuffer = new PLRenderer::TextureBuffer*[m_sCapabilities.nMaxTextureUnits];
			m_ppPrevTextureBuffer    = new PLRenderer::TextureBuffer*[m_sCapabilities.nMaxTextureUnits];
			m_nTextureBufferTypes    = new uint32[m_sCapabilities.nMaxTextureUnits];
			MemoryManager::Set(m_nTextureBufferTypes, 0, sizeof(uint32)*m_sCapabilities.nMaxTextureUnits);
			MemoryManager::Set(m_ppCurrentTextureBuffer, 0, sizeof(PLRenderer::TextureBuffer**)*m_sCapabilities.nMaxTextureUnits);
			MemoryManager::Set(m_ppPrevTextureBuffer, 0, sizeof(PLRenderer::TextureBuffer**)*m_sCapabilities.nMaxTextureUnits);

			// Init color mask
			m_bColorMask[0] = true;
			m_bColorMask[1] = true;
			m_bColorMask[2] = true;
			m_bColorMask[3] = true;

			// Setup current render states
			for (uint32 i=0; i<PLRenderer::RenderState::Number; i++) {
				uint32 nState = m_nRenderState[i];
				m_nRenderState[i] = m_nRenderState[i]+1;
				SetRenderState(static_cast<PLRenderer::RenderState::Enum>(i), nState);
			}

			// Reset render
			Reset();

			// Done
			m_bInitialized = true;
		} else {
			PL_LOG(Error, "Can't create OpenGL render context")
			delete m_pContext;
			m_pContext = nullptr;
		}
	} else {
		// Error!
		PL_LOG(Error, "Can't create OpenGL render context")
		delete m_pContext;
		m_pContext = nullptr;
	}
}

/**
*  @brief
*    Destructor
*/
Renderer::~Renderer()
{
	// Is the renderer initialized?
	if (m_pContext) {
		// Output log information
		PL_LOG(Info, "De-initialize OpenGL renderer")

		// Destroy the fixed functions implementation
		if (m_pFixedFunctions) {
			delete m_pFixedFunctions;
			m_pFixedFunctions = nullptr;
		}

		// Destroy all renderer surfaces of this renderer
		while (m_lstSurfaces.GetNumOfElements())
			delete m_lstSurfaces[0];

		// Clear list of display modes
		for (uint32 i=0; i<m_lstDisplayModeList.GetNumOfElements(); i++)
			delete m_lstDisplayModeList[i];
		m_lstDisplayModeList.Clear();

		// Destroy the OpenGL renderer font manager while there's still an active OpenGL context (... font textures...)
		delete static_cast<PLRenderer::FontManagerBackend*>(m_pFontManager);
		m_pFontManager = nullptr;

		// Destroy the draw helpers instance
		delete m_pDrawHelpers;
		m_pDrawHelpers = nullptr;

		// Destroy all renderer resources of this renderer
		while (m_lstResources.GetNumOfElements())
			delete m_lstResources[0];

		// Free sampler states
		if (m_ppnSamplerState) {
			for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++)
				delete [] m_ppnSamplerState[i];
			delete [] m_ppnSamplerState;
			m_ppnSamplerState = nullptr;
		}

		// Free internal sampler states
		if (m_ppnInternalSamplerState) {
			for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++)
				delete [] m_ppnInternalSamplerState[i];
			delete [] m_ppnInternalSamplerState;
			m_ppnInternalSamplerState = nullptr;
		}

		// Free current stuff
		if (m_ppCurrentTextureBuffer) {
			delete [] m_ppCurrentTextureBuffer;
			m_ppCurrentTextureBuffer = nullptr;
		}
		if (m_ppPrevTextureBuffer) {
			delete [] m_ppPrevTextureBuffer;
			m_ppPrevTextureBuffer = nullptr;
		}
		if (m_nTextureBufferTypes) {
			delete [] m_nTextureBufferTypes;
			m_nTextureBufferTypes = nullptr;
		}

		// Destroy the shader language instances
		for (uint32 i=0; i<m_lstShaderLanguageInstances.GetNumOfElements(); i++)
			delete m_lstShaderLanguageInstances[i];
		m_lstShaderLanguageInstances.Clear();
		m_mapShaderLanguageInstances.Clear();

		// Destroy the OpenGL render context
		delete m_pContext;
		m_pContext = nullptr;
	}
}

/**
*  @brief
*    Returns the OpenGL render context
*/
Context &Renderer::GetContext() const
{
	return *m_pContext;
}

/**
*  @brief
*    Returns the used multisample antialiasing samples per pixel
*/
uint32 Renderer::GetMultisampleAntialiasingSamples() const
{
	return m_nMultisampleAntialiasingSamples;
}

/**
*  @brief
*    Returns a PL format as OpenGL format
*/
uint32 Renderer::GetOpenGLPixelFormat(PLRenderer::TextureBuffer::EPixelFormat nFormat) const
{
	switch (nFormat) {
		case PLRenderer::TextureBuffer::L8:
		case PLRenderer::TextureBuffer::L16:
		case PLRenderer::TextureBuffer::L16F:
		case PLRenderer::TextureBuffer::L32F:
		case PLRenderer::TextureBuffer::LATC1:
			return GL_RED;

		case PLRenderer::TextureBuffer::A8:
			return GL_ALPHA;

		case PLRenderer::TextureBuffer::L4A4:
		case PLRenderer::TextureBuffer::L8A8:
		case PLRenderer::TextureBuffer::LATC2:
			return GL_LUMINANCE_ALPHA;

		case PLRenderer::TextureBuffer::D16:
		case PLRenderer::TextureBuffer::D24:
		case PLRenderer::TextureBuffer::D32:
			return GL_DEPTH_COMPONENT;

		case PLRenderer::TextureBuffer::R8G8B8:
		case PLRenderer::TextureBuffer::R3G3B2:
		case PLRenderer::TextureBuffer::R5G6B5:
		case PLRenderer::TextureBuffer::DXT1:
			return GL_RGB;

		case PLRenderer::TextureBuffer::R5G5B5A1:
		case PLRenderer::TextureBuffer::R4G4B4A4:
		case PLRenderer::TextureBuffer::R8G8B8A8:
		case PLRenderer::TextureBuffer::R10G10B10A2:
		case PLRenderer::TextureBuffer::R16G16B16A16:
		case PLRenderer::TextureBuffer::DXT3:
		case PLRenderer::TextureBuffer::DXT5:
		case PLRenderer::TextureBuffer::R16G16B16A16F:
		case PLRenderer::TextureBuffer::R32G32B32A32F:
		case PLRenderer::TextureBuffer::Unknown:
		default:
			return GL_RGBA;
	}
}

/**
*  @brief
*    Returns a PL data format as OpenGL data format
*/
uint32 Renderer::GetOpenGLDataFormat(PLRenderer::TextureBuffer::EPixelFormat nFormat) const
{
	switch (nFormat) {
		case PLRenderer::TextureBuffer::D16:
			return GL_UNSIGNED_SHORT;

		case PLRenderer::TextureBuffer::D24:
		case PLRenderer::TextureBuffer::D32:
			return GL_UNSIGNED_INT;

		case PLRenderer::TextureBuffer::L8:
		case PLRenderer::TextureBuffer::A8:
		case PLRenderer::TextureBuffer::L4A4:
		case PLRenderer::TextureBuffer::L8A8:
		case PLRenderer::TextureBuffer::R3G3B2:
		case PLRenderer::TextureBuffer::R5G6B5:
		case PLRenderer::TextureBuffer::R5G5B5A1:
		case PLRenderer::TextureBuffer::R4G4B4A4:
		case PLRenderer::TextureBuffer::R8G8B8:
		case PLRenderer::TextureBuffer::R8G8B8A8:
		case PLRenderer::TextureBuffer::R10G10B10A2:
		case PLRenderer::TextureBuffer::DXT1:
		case PLRenderer::TextureBuffer::DXT3:
		case PLRenderer::TextureBuffer::DXT5:
		case PLRenderer::TextureBuffer::LATC1:
		case PLRenderer::TextureBuffer::LATC2:
			return GL_UNSIGNED_BYTE;

		case PLRenderer::TextureBuffer::L16:
		case PLRenderer::TextureBuffer::R16G16B16A16:
			return GL_UNSIGNED_SHORT;

		case PLRenderer::TextureBuffer::L16F:
		case PLRenderer::TextureBuffer::R16G16B16A16F:
			return GL_HALF_FLOAT_ARB;

		case PLRenderer::TextureBuffer::L32F:
		case PLRenderer::TextureBuffer::R32G32B32A32F:
			return GL_FLOAT;

		case PLRenderer::TextureBuffer::Unknown:
		default:
			return GL_UNSIGNED_BYTE;
	}
}

/**
*  @brief
*    Chooses the image source and internal texture buffer formats depending on various input parameters
*/
PLRenderer::TextureBuffer::EPixelFormat Renderer::ChooseFormats(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags,
																PLRenderer::TextureBuffer::EPixelFormat &nImageFormat, bool &bUsePreCompressedData) const
{
	PLRenderer::TextureBuffer::EPixelFormat nChosenInternalFormat = PLRenderer::TextureBuffer::Unknown;
	const Extensions &cExtensions = m_pContext->GetExtensions();

	// Get image pixel format
	nImageFormat = PLRenderer::TextureBuffer::GetFormatFromImage(cImage, !(nFlags & PLRenderer::TextureBuffer::Compression));
	bUsePreCompressedData = false;
	if (PLRenderer::TextureBuffer::IsCompressedFormat(nImageFormat)) {
		// If the given image is pre compressed, but the hardware does not support the used compression format
		// we have to use the uncompressed image instead.
		if (!cExtensions.IsGL_ARB_texture_compression() ||
			(nImageFormat <= PLRenderer::TextureBuffer::DXT5  && !cExtensions.IsGL_EXT_texture_compression_s3tc()) ||
			(nImageFormat == PLRenderer::TextureBuffer::LATC1 && !cExtensions.IsGL_EXT_texture_compression_latc()) ||
			(nImageFormat == PLRenderer::TextureBuffer::LATC2 && !cExtensions.IsGL_EXT_texture_compression_latc() && !cExtensions.IsGL_ATI_texture_compression_3dc())) {
			// Do not use texture buffer compression
			nImageFormat = PLRenderer::TextureBuffer::GetFormatFromImage(cImage, true);
		} else {
			// Use the pre compressed data
			bUsePreCompressedData = true;
		}
	}

	// Get internal pixel format
	if (nInternalFormat != PLRenderer::TextureBuffer::Unknown) {
		nChosenInternalFormat = nInternalFormat;
		if (PLRenderer::TextureBuffer::IsCompressedFormat(nChosenInternalFormat) &&
			((nChosenInternalFormat <= PLRenderer::TextureBuffer::DXT5  && !cExtensions.IsGL_EXT_texture_compression_s3tc()) ||
			 (nChosenInternalFormat == PLRenderer::TextureBuffer::LATC1 && !cExtensions.IsGL_EXT_texture_compression_latc()) ||
			 (nChosenInternalFormat == PLRenderer::TextureBuffer::LATC2 && !cExtensions.IsGL_EXT_texture_compression_latc() && !cExtensions.IsGL_ATI_texture_compression_3dc()))) {
			// Hm, the user want's to use a certain compressed format, but the desired format is NOT available...
			// we have to choose a fallback format.
			switch (nChosenInternalFormat) {
				case PLRenderer::TextureBuffer::LATC1:
					nChosenInternalFormat = PLRenderer::TextureBuffer::L8;
					break;

				case PLRenderer::TextureBuffer::LATC2:
					nChosenInternalFormat = PLRenderer::TextureBuffer::L8A8;
					break;

				case PLRenderer::TextureBuffer::DXT1:
					nChosenInternalFormat = PLRenderer::TextureBuffer::R8G8B8;
					break;

				case PLRenderer::TextureBuffer::DXT3:
				case PLRenderer::TextureBuffer::DXT5:
					nChosenInternalFormat = PLRenderer::TextureBuffer::R8G8B8A8;
					break;

				case PLRenderer::TextureBuffer::L8:
				case PLRenderer::TextureBuffer::L16:
				case PLRenderer::TextureBuffer::A8:
				case PLRenderer::TextureBuffer::L4A4:
				case PLRenderer::TextureBuffer::L8A8:
				case PLRenderer::TextureBuffer::D16:
				case PLRenderer::TextureBuffer::D24:
				case PLRenderer::TextureBuffer::D32:
				case PLRenderer::TextureBuffer::R3G3B2:
				case PLRenderer::TextureBuffer::R5G6B5:
				case PLRenderer::TextureBuffer::R5G5B5A1:
				case PLRenderer::TextureBuffer::R4G4B4A4:
				case PLRenderer::TextureBuffer::R8G8B8:
				case PLRenderer::TextureBuffer::R8G8B8A8:
				case PLRenderer::TextureBuffer::R10G10B10A2:
				case PLRenderer::TextureBuffer::R16G16B16A16:
				case PLRenderer::TextureBuffer::L16F:
				case PLRenderer::TextureBuffer::L32F:
				case PLRenderer::TextureBuffer::R16G16B16A16F:
				case PLRenderer::TextureBuffer::R32G32B32A32F:
				case PLRenderer::TextureBuffer::Unknown:
				default:
					// Only compressed formats need to be handled in this switch
					break;
			}
		}

		// Can we still use the pre compressed image data? (if available)
		if (bUsePreCompressedData && nChosenInternalFormat != nImageFormat) {
			// Nope, sorry, the user want to use another internal format :(
			bUsePreCompressedData = false;
			nImageFormat = PLRenderer::TextureBuffer::GetFormatFromImage(cImage, true);
		}
	} else {
		if (nImageFormat == PLRenderer::TextureBuffer::Unknown)
			return PLRenderer::TextureBuffer::Unknown; // Error!
		nChosenInternalFormat = nImageFormat;

		// Use compression?
		if ((nFlags & PLRenderer::TextureBuffer::Compression) && !bUsePreCompressedData) {
			// Get the first image buffer and ensure that the data format is "byte" (the supported compression formats only support byte)
			ImageBuffer *pImageBuffer = cImage.GetBuffer();
			if (pImageBuffer && pImageBuffer->GetDataFormat() == DataByte) {
				switch (pImageBuffer->GetComponentsPerPixel()) {
					case 1:
						if (cExtensions.IsGL_EXT_texture_compression_latc())
							nChosenInternalFormat = PLRenderer::TextureBuffer::LATC1;
						break;

					case 2:
						if (cExtensions.IsGL_EXT_texture_compression_latc() || cExtensions.IsGL_ATI_texture_compression_3dc())
							nChosenInternalFormat = PLRenderer::TextureBuffer::LATC2;
						break;

					case 3:
						if (cExtensions.IsGL_EXT_texture_compression_s3tc())
							nChosenInternalFormat = PLRenderer::TextureBuffer::DXT1;
						break;

					case 4:
						if (cExtensions.IsGL_EXT_texture_compression_s3tc())
							nChosenInternalFormat = PLRenderer::TextureBuffer::DXT5;
						break;
				}
			}
		}
	}

	// Return the chosen internal format
	return nChosenInternalFormat;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a OpenGL render context instance
*/
Context *Renderer::CreateContext()
{
	#ifdef WIN32
		return new ContextWindows(*this, m_nMultisampleAntialiasingSamples);
	#elif APPLE
		// We can also reuse the PLFrontendOS X11 implementation from the Linux port
		// return new ContextMacOSX_X11(*this, m_nMultisampleAntialiasingSamples);

		// This is what we really want to use on Mac OS X to stay out of compatibility trouble
		return new ContextMacOSX_Cocoa(*this, m_nMultisampleAntialiasingSamples);
	#elif LINUX
		return new ContextLinux(*this);
	#endif
}

/**
*  @brief
*    Initializes all wrappers
*/
void Renderer::InitWrappers()
{
	const Extensions &cExtensions = m_pContext->GetExtensions();

	// Fill modes
	m_cPLE_FILLWrapper.Resize(PLRenderer::Fill::Number, false, false);
	m_cPLE_FILLWrapper += GL_POINT;	//  0: PLRenderer::Fill::Point
	m_cPLE_FILLWrapper += GL_LINE;	//  1: PLRenderer::Fill::Line
	m_cPLE_FILLWrapper += GL_FILL;	//  2: PLRenderer::Fill::Solid

	// Cull modes
	m_cPLE_CULLWrapper.Resize(PLRenderer::Cull::Number, false, false);
	m_cPLE_CULLWrapper += 0;		//  0: PLRenderer::Cull::None (glDisable(GL_CULL_FACE)/glEnable(GL_CULL_FACE) is used)
	m_cPLE_CULLWrapper += GL_CW;	//  1: PLRenderer::Cull::CW
	m_cPLE_CULLWrapper += GL_CCW;	//  2: PLRenderer::Cull::CCW

	// Blend functions
	m_cPLE_BLENDWrapper.Resize(PLRenderer::BlendFunc::Number, false, false);
	m_cPLE_BLENDWrapper += GL_ZERO;					//  0: PLRenderer::BlendFunc::Zero
	m_cPLE_BLENDWrapper += GL_ONE;					//  1: PLRenderer::BlendFunc::One
	m_cPLE_BLENDWrapper += GL_SRC_COLOR;			//  2: PLRenderer::BlendFunc::SrcColor
	m_cPLE_BLENDWrapper += GL_ONE_MINUS_SRC_COLOR;	//  3: PLRenderer::BlendFunc::InvSrcColor
	m_cPLE_BLENDWrapper += GL_SRC_ALPHA;			//  4: PLRenderer::BlendFunc::SrcAlpha
	m_cPLE_BLENDWrapper += GL_ONE_MINUS_SRC_ALPHA;	//  5: PLRenderer::BlendFunc::InvSrcAlpha
	m_cPLE_BLENDWrapper += GL_SRC_ALPHA_SATURATE;	//  6: PLRenderer::BlendFunc::SrcAlphaSat
	m_cPLE_BLENDWrapper += GL_DST_COLOR;			//  7: PLRenderer::BlendFunc::DstColor
	m_cPLE_BLENDWrapper += GL_ONE_MINUS_DST_COLOR;	//  8: PLRenderer::BlendFunc::InvDstColor
	m_cPLE_BLENDWrapper += GL_DST_ALPHA;			//  9: PLRenderer::BlendFunc::DstAlpha
	m_cPLE_BLENDWrapper += GL_ONE_MINUS_DST_ALPHA;	// 10: PLRenderer::BlendFunc::InvDstAlpha

	// Comparison functions
	m_cPLE_CMPWrapper.Resize(PLRenderer::Compare::Number, false, false);
	m_cPLE_CMPWrapper += GL_NEVER;		//  0: PLRenderer::Compare::Never
	m_cPLE_CMPWrapper += GL_LESS;		//  1: PLRenderer::Compare::Less
	m_cPLE_CMPWrapper += GL_EQUAL;		//  2: PLRenderer::Compare::Equal
	m_cPLE_CMPWrapper += GL_LEQUAL;		//  3: PLRenderer::Compare::LessEqual
	m_cPLE_CMPWrapper += GL_GREATER;	//  4: PLRenderer::Compare::Greater
	m_cPLE_CMPWrapper += GL_NOTEQUAL;	//  5: PLRenderer::Compare::NotEqual
	m_cPLE_CMPWrapper += GL_GEQUAL;		//  6: PLRenderer::Compare::GreaterEqual
	m_cPLE_CMPWrapper += GL_ALWAYS;		//  7: PLRenderer::Compare::Always

	// Stencil operations
	m_cPLE_SOPWrapper.Resize(PLRenderer::StencilOp::Number, false, false);
	m_cPLE_SOPWrapper += GL_KEEP;			//  0: PLRenderer::StencilOp::Keep
	m_cPLE_SOPWrapper += GL_ZERO;			//  1: PLRenderer::StencilOp::Zero
	m_cPLE_SOPWrapper += GL_REPLACE;		//  2: PLRenderer::StencilOp::Replace
	m_cPLE_SOPWrapper += GL_INCR;			//  3: PLRenderer::StencilOp::Incr
	m_cPLE_SOPWrapper += GL_DECR;			//  4: PLRenderer::StencilOp::Decr
	m_cPLE_SOPWrapper += GL_INCR_WRAP_EXT;	//  5: PLRenderer::StencilOp::IncrWrap
	m_cPLE_SOPWrapper += GL_DECR_WRAP_EXT;	//  6: PLRenderer::StencilOp::DecrWrap
	m_cPLE_SOPWrapper += GL_INVERT;			//  7: PLRenderer::StencilOp::Invert

	// Primitive types
	m_cPLE_PTWrapper.Resize(PLRenderer::Primitive::Number, false, false);
	m_cPLE_PTWrapper += GL_POINTS;			//  0: PLRenderer::Primitive::PointList
	m_cPLE_PTWrapper += GL_LINES;			//  1: PLRenderer::Primitive::LineList
	m_cPLE_PTWrapper += GL_LINE_STRIP;		//  2: PLRenderer::Primitive::LineStrip
	m_cPLE_PTWrapper += GL_TRIANGLES;		//  3: PLRenderer::Primitive::TriangleList
	m_cPLE_PTWrapper += GL_TRIANGLE_STRIP;	//  4: PLRenderer::Primitive::TriangleStrip
	m_cPLE_PTWrapper += GL_TRIANGLE_FAN;	//  5: PLRenderer::Primitive::TriangleFan

	// Texture-addressing modes
	m_cPLE_TAWrapper.Resize(PLRenderer::TextureAddressing::Number, false, false);
	m_cPLE_TAWrapper += GL_CLAMP_TO_EDGE;		//  0: PLRenderer::TextureAddressing::Clamp
	m_cPLE_TAWrapper += GL_CLAMP_TO_BORDER;		//  1: PLRenderer::TextureAddressing::Border
	m_cPLE_TAWrapper += GL_REPEAT;				//  2: PLRenderer::TextureAddressing::Wrap
	m_cPLE_TAWrapper += GL_MIRRORED_REPEAT_ARB;	//  3: PLRenderer::TextureAddressing::Mirror

	// Texture buffer pixel formats (internal)
	m_cPLE_TPFWrapper.Resize(PLRenderer::TextureBuffer::NumOfPixelFormats, false, false);
	m_cPLE_TPFWrapper += GL_INTENSITY8;									//  0: PLRenderer::TextureBuffer::L8
	m_cPLE_TPFWrapper += GL_INTENSITY16;								//  1: PLRenderer::TextureBuffer::L16
	m_cPLE_TPFWrapper += GL_ALPHA8;										//  2: PLRenderer::TextureBuffer::A8
	m_cPLE_TPFWrapper += GL_LUMINANCE4_ALPHA4;							//  3: PLRenderer::TextureBuffer::L4A4
	m_cPLE_TPFWrapper += GL_LUMINANCE8_ALPHA8;							//  4: PLRenderer::TextureBuffer::L8A8
	m_cPLE_TPFWrapper += GL_DEPTH_COMPONENT16;							//  5: PLRenderer::TextureBuffer::D16
	m_cPLE_TPFWrapper += GL_DEPTH_COMPONENT24;							//  6: PLRenderer::TextureBuffer::D24
	m_cPLE_TPFWrapper += GL_DEPTH_COMPONENT32;							//  7: PLRenderer::TextureBuffer::D32
	m_cPLE_TPFWrapper += GL_R3_G3_B2;									//  8: PLRenderer::TextureBuffer::R3G3B2
	m_cPLE_TPFWrapper += GL_RGB5;										//  9: PLRenderer::TextureBuffer::R5G6B5
	m_cPLE_TPFWrapper += GL_RGB5_A1;									// 10: PLRenderer::TextureBuffer::R5G5B5A1
	m_cPLE_TPFWrapper += GL_RGBA4;										// 11: PLRenderer::TextureBuffer::R4G4B4A4
	m_cPLE_TPFWrapper += GL_RGB8;										// 12: PLRenderer::TextureBuffer::R8G8B8
	m_cPLE_TPFWrapper += GL_RGBA8;										// 13: PLRenderer::TextureBuffer::R8G8B8A8
	m_cPLE_TPFWrapper += GL_RGB10_A2;									// 14: PLRenderer::TextureBuffer::R10G10B10A2
	m_cPLE_TPFWrapper += GL_RGBA16;										// 15: PLRenderer::TextureBuffer::R16G16B16A16
	m_cPLE_TPFWrapper += GL_COMPRESSED_RGB_S3TC_DXT1_EXT;				// 16: PLRenderer::TextureBuffer::DXT1
	m_cPLE_TPFWrapper += GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;				// 17: PLRenderer::TextureBuffer::DXT3
	m_cPLE_TPFWrapper += GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;				// 18: PLRenderer::TextureBuffer::DXT5
	if (cExtensions.IsGL_EXT_texture_compression_latc())
		m_cPLE_TPFWrapper += GL_COMPRESSED_LUMINANCE_LATC1_EXT;			// 19: PLRenderer::TextureBuffer::LATC1
	else
		m_cPLE_TPFWrapper += 0;											// 19: PLRenderer::TextureBuffer::LATC1
	if (cExtensions.IsGL_EXT_texture_compression_latc())
		m_cPLE_TPFWrapper += GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT;	// 20: PLRenderer::TextureBuffer::LATC2
	else if (cExtensions.IsGL_ATI_texture_compression_3dc())
		m_cPLE_TPFWrapper += GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI;		// 20: PLRenderer::TextureBuffer::LATC2
	else
		m_cPLE_TPFWrapper += 0;											// 20: PLRenderer::TextureBuffer::LATC2

	// ARB float pixel format
	if (cExtensions.IsGL_ARB_texture_float()) {
		m_cPLE_TPFWrapper += GL_LUMINANCE16F_ARB;	// 21: PLRenderer::TextureBuffer::R16F
		m_cPLE_TPFWrapper += GL_LUMINANCE32F_ARB;	// 22: PLRenderer::TextureBuffer::R32F
		m_cPLE_TPFWrapper += GL_RGBA16F_ARB;		// 23: PLRenderer::TextureBuffer::R16G16B16A16F
		m_cPLE_TPFWrapper += GL_RGBA32F_ARB;		// 24: PLRenderer::TextureBuffer::R32G32B32A32F
	} else {
		// ATI float pixel format
		if (cExtensions.IsWGL_ATI_pixel_format_float()) {
			m_cPLE_TPFWrapper += GL_LUMINANCE_FLOAT16_ATI;	// 20: PLRenderer::TextureBuffer::R16F
			m_cPLE_TPFWrapper += GL_LUMINANCE_FLOAT32_ATI;	// 21: PLRenderer::TextureBuffer::R32F
			m_cPLE_TPFWrapper += GL_RGBA_FLOAT16_ATI;		// 22: PLRenderer::TextureBuffer::R16G16B16A16F
			m_cPLE_TPFWrapper += GL_RGBA_FLOAT32_ATI;		// 23: PLRenderer::TextureBuffer::R32G32B32A32F
		} else {
			// NVIDIA float pixel format
			if (cExtensions.IsWGL_NV_float_buffer()) {
				m_cPLE_TPFWrapper += GL_FLOAT_R16_NV;		// 20: PLRenderer::TextureBuffer::R16F
				m_cPLE_TPFWrapper += GL_FLOAT_R32_NV;		// 21: PLRenderer::TextureBuffer::R32F
				m_cPLE_TPFWrapper += GL_FLOAT_RGBA16_NV;	// 22: PLRenderer::TextureBuffer::R16G16B16A16F
				m_cPLE_TPFWrapper += GL_FLOAT_RGBA32_NV;	// 23: PLRenderer::TextureBuffer::R32G32B32A32F
			} else { // Fixed point fallback
				m_cPLE_TPFWrapper += GL_INTENSITY8;	// 20: PLRenderer::TextureBuffer::R16F
				m_cPLE_TPFWrapper += GL_INTENSITY8;	// 21: PLRenderer::TextureBuffer::R32F
				m_cPLE_TPFWrapper += GL_RGBA8;		// 22: PLRenderer::TextureBuffer::R16G16B16A16F
				m_cPLE_TPFWrapper += GL_RGBA8;		// 23: PLRenderer::TextureBuffer::R32G32B32A32F
			}
		}
	}
}

/**
*  @brief
*    Setup the renderer capabilities
*/
void Renderer::SetupCapabilities()
{
	const Extensions &cExtensions = m_pContext->GetExtensions();
	GLint nGLTemp;

	// Total available GPU memory in kilobytes
	m_sCapabilities.nTotalAvailableGPUMemory = 0;
	if (cExtensions.IsGL_ATI_meminfo()) {
		// I was unable to get WGL_AMD_gpu_association up and running on my "ATI Mobility Radeon HD 4850" using Catalyst 11.11 on Windows 7 64 bit, so, GL_ATI_meminfo must do the job in here

		// http://www.opengl.org/registry/specs/ATI/meminfo.txt says:
		// [0] - total memory free in the pool
		// [1] - largest available free block in the pool
		// [2] - total auxiliary memory free
		// [3] - largest auxiliary free block
		GLint nFreeMemory[4];

		// We're only interested in "Total available GPU memory in kilobytes", not in technical details
		// -> Get the highest one
		glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, nFreeMemory);
		m_sCapabilities.nTotalAvailableGPUMemory = nFreeMemory[0];
		glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, nFreeMemory);
		if (m_sCapabilities.nTotalAvailableGPUMemory < static_cast<uint32>(nFreeMemory[0]))
			m_sCapabilities.nTotalAvailableGPUMemory = nFreeMemory[0];
		glGetIntegerv(GL_RENDERBUFFER_FREE_MEMORY_ATI, nFreeMemory);
		if (m_sCapabilities.nTotalAvailableGPUMemory < static_cast<uint32>(nFreeMemory[0]))
			m_sCapabilities.nTotalAvailableGPUMemory = nFreeMemory[0];
	} else if (cExtensions.IsGL_NVX_gpu_memory_info()) {
		GLint nAvailableMemory = 0;
		// #define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
		glGetIntegerv(0x9048, &nAvailableMemory);
		m_sCapabilities.nTotalAvailableGPUMemory = nAvailableMemory;
	}

	// Maximum number of color render targets
	if (cExtensions.IsGL_ARB_draw_buffers())
		glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &nGLTemp);
	else if (cExtensions.IsGL_ATI_draw_buffers())
		glGetIntegerv(GL_MAX_DRAW_BUFFERS_ATI, &nGLTemp);
	else
		nGLTemp = 1;
	// At the moment, this render backend supports up to 16 color render targets
	m_sCapabilities.nMaxColorRenderTargets = (nGLTemp > 16) ? 16 : static_cast<uint8>(nGLTemp);

	// Maximum number of texture units
	if (cExtensions.IsGL_ARB_multitexture()) {
		// [TODO] 'Texture Units' out-of-date
		// Do not use "GL_MAX_TEXTURE_UNITS_ARB", use GL_MAX_TEXTURE_IMAGE_UNITS_ARB! Texture units were
		// decoupled into texture image units and texture coordinates. (with GL_MAX_TEXTURE_COORDS_ARB)
		// -> "http://developer.nvidia.com/object/General_FAQ.html"
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &nGLTemp);

		m_sCapabilities.nMaxTextureUnits = static_cast<uint8>(nGLTemp);
	} else {
		m_sCapabilities.nMaxTextureUnits = 1;
	}

	// Maximum anisotropy
	glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &nGLTemp);
	m_sCapabilities.nMaxAnisotropy = static_cast<uint16>(nGLTemp);

	// Maximum tessellation factor
	m_sCapabilities.nMaxTessellationFactor = cExtensions.IsGL_AMD_vertex_shader_tessellator() ? 15 : 1;

	// Maximum texture buffer size
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &nGLTemp);
	m_sCapabilities.nMaxTextureBufferSize = static_cast<uint16>(nGLTemp);

	// Non power of two texture buffers supported?
	m_sCapabilities.bTextureBufferNonPowerOfTwo = cExtensions.IsGL_ARB_texture_non_power_of_two();

	// 2D array texture buffers supported?
	m_sCapabilities.bTextureBuffer2DArray = cExtensions.IsGL_EXT_texture_array();
	if (m_sCapabilities.bTextureBuffer2DArray) {
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS_EXT, &nGLTemp);
		m_sCapabilities.nMaxTextureBuffer2DArrayLayers = static_cast<uint16>(nGLTemp);
	} else {
		m_sCapabilities.nMaxTextureBuffer2DArrayLayers = 0;
	}

	// Rectangle texture buffers supported?
	// GL_EXT_texture_rectangle, GL_NV_texture_rectangle and GL_ARB_texture_rectangle ONLY differ within their name :)
	m_sCapabilities.bTextureBufferRectangle = (cExtensions.IsGL_EXT_texture_rectangle() || cExtensions.IsGL_NV_texture_rectangle() || cExtensions.IsGL_ARB_texture_rectangle());

	// Maximum rectangle texture buffer size
	glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB, &nGLTemp);
	m_sCapabilities.nMaxRectangleTextureBufferSize = static_cast<uint16>(nGLTemp);

	// 3D texture buffers supported?
	m_sCapabilities.bTextureBuffer3D = cExtensions.IsGL_EXT_texture3D();

	// Maximum 3D texture buffer size
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &nGLTemp);
	m_sCapabilities.nMax3DTextureBufferSize = static_cast<uint16>(nGLTemp);

	// Cube texture buffers supported?
	m_sCapabilities.bTextureBufferCube = (cExtensions.IsGL_ARB_texture_cube_map() || cExtensions.IsGL_EXT_texture_cube_map());

	// Maximum cube texture buffer size
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, &nGLTemp);
	m_sCapabilities.nMaxCubeTextureBufferSize = static_cast<uint16>(nGLTemp);

	// Stencil wrap supported?
	m_sCapabilities.bStencilWrap = cExtensions.IsGL_EXT_stencil_wrap();

	// Two sided stencils supported?
	m_sCapabilities.bTwoSidedStencils = (cExtensions.IsGL_EXT_stencil_two_side() || cExtensions.IsGL_ATI_separate_stencil());

	// Depth bounds test supported?
	m_sCapabilities.bDepthBoundsTest = cExtensions.IsGL_EXT_depth_bounds_test();

	// Point sprite supported?
	m_sCapabilities.bPointSprite = cExtensions.IsGL_ARB_point_sprite();

	// Point parameters supported?
	m_sCapabilities.bPointParameters = cExtensions.IsGL_ARB_point_parameters();

	// Occlusion query supported
	m_sCapabilities.bOcclusionQuery = (cExtensions.IsGL_ARB_occlusion_query() || (cExtensions.IsGL_NV_occlusion_query() && cExtensions.IsGL_HP_occlusion_test()));

	// Vertex buffer secondary color supported?
	m_sCapabilities.bVertexBufferSecondaryColor = cExtensions.IsGL_EXT_secondary_color();

	// Geometric primitive instancing supported?
	m_sCapabilities.bInstancing = cExtensions.IsGL_ARB_draw_instanced();

	// Show renderer capabilities
	ShowRendererCapabilities();
}

/**
*  @brief
*    General OpenGL settings
*/
void Renderer::GeneralSettings()
{
	const Extensions &cExtensions = m_pContext->GetExtensions();

	// The default value of '4' is NOT desired and would make tricky problems (memory leaks and so on)
	glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
	glPixelStorei(GL_UNPACK_SKIP_ROWS,   0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH,  0);
	glPixelStorei(GL_UNPACK_SWAP_BYTES,  GL_FALSE);
	glPixelStorei(GL_PACK_ALIGNMENT,     1);
	glPixelStorei(GL_PACK_SKIP_ROWS,     0);
	glPixelStorei(GL_PACK_SKIP_PIXELS,   0);
	glPixelStorei(GL_PACK_ROW_LENGTH,    0);
	glPixelStorei(GL_PACK_SWAP_BYTES,    GL_FALSE);

	// Point parameters supported?
	if (m_sCapabilities.bPointParameters) {
		// [TEST]
		// The alpha of a point is calculated to allow the fading of points
		// instead of shrinking them past a defined threshold size. The threshold
		// is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to
		// the minimum and maximum point sizes.
		glPointParameterfARB(GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f);
	}

	// If there are vertex shaders, enable point size writing for them
	// => If this is not done, "gl_PointSize" in GLSL and "PSIZE" in Cg have no effect
	//    while the fixed functions point size attenuation has no effect either...
	// The following comes from http://www.opengl.org/registry/specs/ARB/vertex_program.txt
	// "(42) How does PointSize work with vertex programs?
	//       RESOLVED:  If VERTEX_PROGRAM_POINT_SIZE_ARB is disabled, the size of
	//       points is determined by the PointSize state and is not attenuated, even
	//       if EXT_point_parameters is supported.  If enabled, the point size is the
	//       point size result value, and is clamped to implementation-dependent
	//       point size limits during point rasterization."
	if (cExtensions.IsGL_ARB_vertex_program())
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);

	// No value clamping when using shaders, PLEASE!
	if (cExtensions.IsGL_ARB_color_buffer_float()) {
		glClampColorARB(GL_CLAMP_VERTEX_COLOR_ARB,   GL_FIXED_ONLY_ARB);
		glClampColorARB(GL_CLAMP_FRAGMENT_COLOR_ARB, GL_FIXED_ONLY_ARB);
		glClampColorARB(GL_CLAMP_READ_COLOR_ARB,     GL_FIXED_ONLY_ARB);
	}

	// Fixed functions
	if (m_pFixedFunctions)
		m_pFixedFunctions->GeneralSettings();
}

/**
*  @brief
*    Sampler state filter mode helper function
*/
GLuint Renderer::GetCombinedMinMipFilter(uint32 nStage)
{
	// Has the current texture buffer any mipmaps?
	if (m_ppCurrentTextureBuffer[nStage] && !m_ppCurrentTextureBuffer[nStage]->GetNumOfMipmaps()) {
		// Nope, we can't use mipmappig. Check internal sampler state...
		switch (m_ppnInternalSamplerState[nStage][PLRenderer::Sampler::MinFilter]) {
			case PLRenderer::TextureFiltering::Anisotropic:
			case PLRenderer::TextureFiltering::Linear:
				// Linear min
				return GL_LINEAR;

			case PLRenderer::TextureFiltering::Point:
			case PLRenderer::TextureFiltering::None:
				// Nearest min
				return GL_NEAREST;
		}
	} else {
		// Check internal sampler state
		switch (m_ppnInternalSamplerState[nStage][PLRenderer::Sampler::MinFilter]) {
			case PLRenderer::TextureFiltering::Anisotropic:
			case PLRenderer::TextureFiltering::Linear:
				switch (m_ppnInternalSamplerState[nStage][PLRenderer::Sampler::MipFilter]) {
					case PLRenderer::TextureFiltering::Anisotropic:
					case PLRenderer::TextureFiltering::Linear:
						// Linear min, linear mip
						return GL_LINEAR_MIPMAP_LINEAR;

					case PLRenderer::TextureFiltering::Point:
						// Linear min, point mip
						return GL_LINEAR_MIPMAP_NEAREST;

					case PLRenderer::TextureFiltering::None:
						// Linear min, no mip
						return GL_LINEAR;
				}
				break;

			case PLRenderer::TextureFiltering::Point:
			case PLRenderer::TextureFiltering::None:
				switch (m_ppnInternalSamplerState[nStage][PLRenderer::Sampler::MipFilter]) {
					case PLRenderer::TextureFiltering::Anisotropic:
					case PLRenderer::TextureFiltering::Linear:
						// Nearest min, linear mip
						return GL_NEAREST_MIPMAP_LINEAR;

					case PLRenderer::TextureFiltering::Point:
						// Nearest min, point mip
						return GL_NEAREST_MIPMAP_NEAREST;

					case PLRenderer::TextureFiltering::None:
						// Nearest min, no mip
						return GL_NEAREST;
				}
				break;
		}
	}

	// Fallback, should never get here!
	return GL_NEAREST;
}

/**
*  @brief
*    Restores the device states
*/
void Renderer::RestoreDeviceStates()
{
	// Setup current render states
	for (uint32 i=0; i<PLRenderer::RenderState::Number; i++) {
		uint32 nState = m_nRenderState[i];
		m_nRenderState[i] = m_nRenderState[i]+1;
		SetRenderState(static_cast<PLRenderer::RenderState::Enum>(i), nState);
	}

	// Reset texture buffers
	for (uint32 nStage=0; nStage<m_sCapabilities.nMaxTextureUnits; nStage++) {
		PLRenderer::TextureBuffer *pTextureBuffer = GetTextureBuffer(nStage);
		SetTextureBuffer(nStage, nullptr);
		m_ppPrevTextureBuffer[nStage] = nullptr;
		SetTextureBuffer(nStage, pTextureBuffer);
	}

	// Setup current sampler states
	for (uint32 nStage=0; nStage<m_sCapabilities.nMaxTextureUnits; nStage++) {
		for (uint32 i=0; i<PLRenderer::Sampler::Number; i++) {
			// Backup current state
			uint32 nState = m_ppnSamplerState[nStage][i];
			
			// Make this state 'dirty'
			m_ppnSamplerState[nStage][i]++;
			m_ppnInternalSamplerState[nStage][i] = m_ppnSamplerState[nStage][i] + 1;

			// Set state
			SetSamplerState(nStage, static_cast<PLRenderer::Sampler::Enum>(i), nState);
		}
	}

	// Setup current sampler states of all texture resources
	Iterator<PLRenderer::Resource*> cIterator = m_lstResources.GetIterator();
	while (cIterator.HasNext()) {
		// Initialize sampler states
		PLRenderer::Resource *pResource = cIterator.Next();
		switch (pResource->GetType()) {
			case PLRenderer::Resource::TypeTextureBuffer1D:
				MemoryManager::Set(&static_cast<TextureBuffer1D*>(pResource)->m_nSamplerState[0], PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);
				break;

			case PLRenderer::Resource::TypeTextureBuffer2D:
				MemoryManager::Set(&static_cast<TextureBuffer2D*>(pResource)->m_nSamplerState[0], PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);
				break;

			case PLRenderer::Resource::TypeTextureBuffer2DArray:
				MemoryManager::Set(&static_cast<TextureBuffer2DArray*>(pResource)->m_nSamplerState[0], PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);
				break;

			case PLRenderer::Resource::TypeTextureBufferRectangle:
				MemoryManager::Set(&static_cast<TextureBufferRectangle*>(pResource)->m_nSamplerState[0], PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);
				break;

			case PLRenderer::Resource::TypeTextureBuffer3D:
				MemoryManager::Set(&static_cast<TextureBuffer3D*>(pResource)->m_nSamplerState[0], PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);
				break;

			case PLRenderer::Resource::TypeTextureBufferCube:
				MemoryManager::Set(&static_cast<TextureBufferCube*>(pResource)->m_nSamplerState[0], PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);
				break;

			case PLRenderer::Resource::TypeIndexBuffer:
			case PLRenderer::Resource::TypeVertexBuffer:
			case PLRenderer::Resource::TypeUniformBuffer:
			case PLRenderer::Resource::TypeOcclusionQuery:
			case PLRenderer::Resource::TypeVertexShader:
			case PLRenderer::Resource::TypeTessellationControlShader:
			case PLRenderer::Resource::TypeTessellationEvaluationShader:
			case PLRenderer::Resource::TypeGeometryShader:
			case PLRenderer::Resource::TypeFragmentShader:
			case PLRenderer::Resource::TypeProgram:
			case PLRenderer::Resource::TypeFont:
			default:
				// Only texture buffer types need to be handled in this switch
				break;
		}
	}

	// Fixed functions
	if (m_pFixedFunctions)
		m_pFixedFunctions->RestoreDeviceStates();

	// Set color mask
	glColorMask(m_bColorMask[0], m_bColorMask[1], m_bColorMask[2], m_bColorMask[3]);

	{ // Reset index buffer
		PLRenderer::IndexBuffer *pBuffer = m_pCurrentIndexBuffer;
		m_pCurrentIndexBuffer = nullptr;
		SetIndexBuffer(pBuffer);
	}

	{ // Reset program
		PLRenderer::Program *pProgram = static_cast<PLRenderer::Program*>(m_cProgramHandler.GetResource());
		m_cProgramHandler.SetResource(nullptr);
		SetProgram(pProgram);
	}

	// General OpenGL settings
	GeneralSettings();

	// Setup viewport and scissor rectangle
	SetViewport();
	SetScissorRect();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Renderer functions         ]
//[-------------------------------------------------------]
bool Renderer::IsInitialized() const
{
	return m_bInitialized;
}

String Renderer::GetAPI(uint32 *pnVersion) const
{
	// Get version
	if (pnVersion) {
		// Set version to 0 as default
		*pnVersion = 0;

		// Get version (for example '2.1.3' -> we are only interested in '2.1' because
		// the third number is not really normed)
		String sVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		if (sVersion.GetLength()) {
			// Major
			int nIndex = sVersion.IndexOf('.');
			if (nIndex > -1) {
				// Delete the dot
				sVersion.Delete(nIndex, 1);

				// Minor
				nIndex = sVersion.IndexOf('.');
				if (nIndex > -1) {
					// Get the version string
					String sMajorMinorVersion = sVersion.GetSubstring(0, nIndex);

					// Version string to version number
					*pnVersion = sMajorMinorVersion.GetInt();
				}
			}
		}
	}

	// Get API
	static const String sString = "OpenGL";
	return sString;
}

String Renderer::GetVendor() const
{
	return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

String Renderer::GetDefaultShaderLanguage() const
{
	return m_sDefaultShaderLanguage;
}

PLRenderer::ShaderLanguage *Renderer::GetShaderLanguage(const String &sShaderLanguage)
{
	// Check the renderer mode
	if (GetMode() != ModeFixedFunctions) {
		// Get the shader language to use
		const String &sUsedShaderLanguage = sShaderLanguage.GetLength() ? sShaderLanguage : m_sDefaultShaderLanguage;

		// Is there already an instance of this shader language?
		ShaderLanguage *pShaderLanguage = m_mapShaderLanguageInstances.Get(sUsedShaderLanguage);
		if (!pShaderLanguage) {
			// Get the RTTI class of the shader language
			const Class *pClass = m_mapShaderLanguages.Get(sUsedShaderLanguage);
			if (pClass) {
				// Create an instance
				pShaderLanguage = static_cast<ShaderLanguage*>(pClass->Create(Params<Object*, Renderer&>(*this)));
				if (pShaderLanguage) {
					// Register the instance
					m_lstShaderLanguageInstances.Add(pShaderLanguage);
					m_mapShaderLanguageInstances.Add(sUsedShaderLanguage, pShaderLanguage);
				}
			}
		}

		// Done
		return pShaderLanguage;
	}

	// Error!
	return nullptr;
}

PLRenderer::FixedFunctions *Renderer::GetFixedFunctions() const
{
	return m_pFixedFunctions;
}

PLRenderer::FontManager &Renderer::GetFontManager() const
{
	return *m_pFontManager;
}

void Renderer::BackupDeviceObjects()
{
	// Call base function
	PLRenderer::RendererBackend::BackupDeviceObjects();

	// Destroy the context
	delete m_pContext;
	m_pContext = nullptr;
}

void Renderer::RestoreDeviceObjects()
{
	// Create a new OpenGL render context
	m_pContext = CreateContext();

	// Call base function
	PLRenderer::RendererBackend::RestoreDeviceObjects();

	// Restore device states
	RestoreDeviceStates();
}


//[-------------------------------------------------------]
//[ Create surface/resource                               ]
//[-------------------------------------------------------]
PLRenderer::SurfaceWindow *Renderer::CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen)
{
	// Is the renderer initialized and is the surface window handler valid?
	if (cHandler.GetRenderer() == this) {
		// Create and register renderer surface
		PLRenderer::SurfaceWindow *pRendererSurface = m_pContext->CreateSurfaceWindow(cHandler, nNativeWindowHandle, sDisplayMode, bFullscreen);
		m_lstSurfaces.Add(reinterpret_cast<PLRenderer::Surface*>(pRendererSurface));

		// Return created renderer surface
		return pRendererSurface;
	}

	// Error!
	return nullptr;
}

PLRenderer::SurfaceTextureBuffer *Renderer::CreateSurfaceTextureBuffer2D(const Vector2i &vSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, uint32 nFlags, uint8 nMaxColorTargets)
{
	// Check maximum render targets and dimension
	if (nMaxColorTargets && nMaxColorTargets <= m_sCapabilities.nMaxColorRenderTargets && IsValidTextureBuffer2DSize(vSize.x) && IsValidTextureBuffer2DSize(vSize.y)) {
		// Create and register renderer surface
		uint32 nTextureBufferFlags = PLRenderer::TextureBuffer::RenderTarget;
		if (nFlags & PLRenderer::SurfaceTextureBuffer::Mipmaps)
			nTextureBufferFlags |= PLRenderer::TextureBuffer::Mipmaps;
		PLRenderer::TextureBuffer *pTextureBuffer = new TextureBuffer2D(*this, vSize, nFormat, nTextureBufferFlags);
		PLRenderer::SurfaceTextureBuffer *pRendererSurface = new SurfaceTextureBuffer(*this, *pTextureBuffer, nFlags, nMaxColorTargets);
		m_lstSurfaces.Add(pRendererSurface);

		// Return created renderer surface
		return pRendererSurface;
	} else {
		// Error!
		return nullptr;
	}
}

PLRenderer::SurfaceTextureBuffer *Renderer::CreateSurfaceTextureBufferRectangle(const Vector2i &vSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, uint32 nFlags, uint8 nMaxColorTargets)
{
	// Check maximum render targets and dimension
	if (nMaxColorTargets && nMaxColorTargets <= m_sCapabilities.nMaxColorRenderTargets && IsValidTextureBufferRectangleSize(vSize.x) && IsValidTextureBufferRectangleSize(vSize.y)) {
		// Create and register renderer surface
		uint32 nTextureBufferFlags = PLRenderer::TextureBuffer::RenderTarget;
		if (nFlags & PLRenderer::SurfaceTextureBuffer::Mipmaps)
			nTextureBufferFlags |= PLRenderer::TextureBuffer::Mipmaps;
		PLRenderer::TextureBuffer *pTextureBuffer = new TextureBufferRectangle(*this, vSize, nFormat, nTextureBufferFlags);
		PLRenderer::SurfaceTextureBuffer *pRendererSurface = new SurfaceTextureBuffer(*this, *pTextureBuffer, nFlags, nMaxColorTargets);
		m_lstSurfaces.Add(pRendererSurface);

		// Return created renderer surface
		return pRendererSurface;
	} else {
		// Error!
		return nullptr;
	}
}

PLRenderer::SurfaceTextureBuffer *Renderer::CreateSurfaceTextureBufferCube(uint16 nSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, uint32 nFlags)
{
	// Valid dimension?
	if (!IsValidTextureBufferCubeSize(nSize))
		return nullptr; // Error!

	// Create and register renderer surface
	uint32 nTextureBufferFlags = PLRenderer::TextureBuffer::RenderTarget;
	if (nFlags & PLRenderer::SurfaceTextureBuffer::Mipmaps)
		nTextureBufferFlags |= PLRenderer::TextureBuffer::Mipmaps;
	PLRenderer::TextureBuffer *pTextureBuffer = new TextureBufferCube(*this, nSize, nFormat, nTextureBufferFlags);
	PLRenderer::SurfaceTextureBuffer *pRendererSurface = new SurfaceTextureBuffer(*this, *pTextureBuffer, nFlags, 1);
	m_lstSurfaces.Add(pRendererSurface);

	// Return created renderer surface
	return pRendererSurface;
}

PLRenderer::TextureBuffer1D *Renderer::CreateTextureBuffer1D(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer1D(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the OpenGL 1D texture buffer
	return new TextureBuffer1D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer2D *Renderer::CreateTextureBuffer2D(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer2D(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the OpenGL 2D texture buffer
	return new TextureBuffer2D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer2DArray *Renderer::CreateTextureBuffer2DArray(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer - The "TextureBuffer2DArray"-implementation is using 3D functions and compression as well
	if (!m_sCapabilities.bTextureBuffer2DArray || !m_sCapabilities.bTextureBuffer3D || !m_pContext->GetExtensions().IsGL_ARB_texture_compression() || !CheckTextureBuffer2DArray(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the OpenGL 2D array texture buffer
	return new TextureBuffer2DArray(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer *Renderer::CreateTextureBufferRectangle(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!m_sCapabilities.bTextureBufferRectangle || !CheckTextureBufferRectangle(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the OpenGL rectangle texture buffer
	return new TextureBufferRectangle(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer3D *Renderer::CreateTextureBuffer3D(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!m_sCapabilities.bTextureBuffer3D || !CheckTextureBuffer3D(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the OpenGL 3D texture buffer
	return new TextureBuffer3D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBufferCube *Renderer::CreateTextureBufferCube(PLGraphics::Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!m_sCapabilities.bTextureBufferCube || !CheckTextureBufferCube(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the OpenGL cube texture buffer
	return new TextureBufferCube(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::IndexBuffer *Renderer::CreateIndexBuffer()
{
	// Create the OpenGL index buffer
	return new IndexBuffer(*this);
}

PLRenderer::VertexBuffer *Renderer::CreateVertexBuffer()
{
	// Create the OpenGL vertex buffer
	return new VertexBuffer(*this);
}

PLRenderer::OcclusionQuery *Renderer::CreateOcclusionQuery()
{
	// Create the OpenGL occlusion query
	return new OcclusionQuery(*this);
}


//[-------------------------------------------------------]
//[ States                                                ]
//[-------------------------------------------------------]
bool Renderer::SetRenderState(PLRenderer::RenderState::Enum nState, uint32 nValue)
{
	// Check if the state is a valid render state member
	if (nState >= PLRenderer::RenderState::Number)
		return false; // Error!

	// Check if this render state is already set to this value
	if (m_nRenderState[nState] != nValue) {
		const Extensions &cExtensions = m_pContext->GetExtensions();

		// Set the render state
		m_nRenderState[nState] = nValue;
		m_sStatistics.nRenderStateChanges++;

		// Set render state - because there are that many render states, split the states binary tree like
		if (nState < PLRenderer::RenderState::PointSize) {
			if (nState < PLRenderer::RenderState::BlendEnable) {
				switch (nState) {
				// Modes
					case PLRenderer::RenderState::FillMode:
						if (GetRenderState(PLRenderer::RenderState::FixedFillMode) == PLRenderer::Fill::Unknown) {
							const uint32 &nAPIValue = m_cPLE_FILLWrapper[nValue];
							if (&nAPIValue != &Array<uint32>::Null)
								glPolygonMode(GL_FRONT_AND_BACK, nAPIValue);
							else
								return false; // Error, invalid value!
						}
						break;

					case PLRenderer::RenderState::CullMode:
						if (nValue == PLRenderer::Cull::None)
							glDisable(GL_CULL_FACE);
						else {
							// Invert cull mode?
							bool bAPISwapY = false;
							if (m_cCurrentSurface.GetSurface())
								bAPISwapY = m_cCurrentSurface.GetSurface()->IsAPISwapY();
							if ((!bAPISwapY &&  GetRenderState(PLRenderer::RenderState::InvCullMode)) || (bAPISwapY && !GetRenderState(PLRenderer::RenderState::InvCullMode)))
								nValue = (nValue == PLRenderer::Cull::CW) ? PLRenderer::Cull::CCW : PLRenderer::Cull::CW;

							// Set the state
							glEnable(GL_CULL_FACE);
							const uint32 &nAPIValue = m_cPLE_CULLWrapper[nValue];
							if (&nAPIValue != &Array<uint32>::Null)
								glFrontFace(nAPIValue);
							else
								return false; // Error, invalid value!
						}
						break;

				// Z buffer
					case PLRenderer::RenderState::ZEnable:
						if (nValue == 0)
							glDisable(GL_DEPTH_TEST);
						else if (nValue == 1)
							glEnable(GL_DEPTH_TEST);
						else
							return false; // Error, invalid value!
						break;

					case PLRenderer::RenderState::ZWriteEnable:
						if (nValue == 0)
							glDepthMask(false);
						else if (nValue == 1)
							glDepthMask(true);
						else
							return false; // Error, invalid value!
						break;

					case PLRenderer::RenderState::ZFunc:
					{
						const uint32 &nAPIValue = m_cPLE_CMPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null)
							glDepthFunc(nAPIValue);
						else
							return false; // Error, invalid value!
						break;
					}

					case PLRenderer::RenderState::ZBias:
						// Fixed function
						if (m_pFixedFunctions)
							m_pFixedFunctions->UpdateCurrentOpenGLProjectionMatrix();
						break;

					case PLRenderer::RenderState::SlopeScaleDepthBias:
						if (!nValue && !GetRenderState(PLRenderer::RenderState::DepthBias)) {
							// The bias is not applied to any line and point primitive within D3D, but OpenGL supports this
							// -> This is a quite useful feature for visualization so we don't want to miss it
							glDisable(GL_POLYGON_OFFSET_POINT);
							glDisable(GL_POLYGON_OFFSET_LINE);

							// Disable polygon offset
							glDisable(GL_POLYGON_OFFSET_FILL);
						} else {
							// The bias is not applied to any line and point primitive within D3D, but OpenGL supports this
							// -> This is a quite useful feature for visualization so we don't want to miss it
							glEnable(GL_POLYGON_OFFSET_POINT);
							glEnable(GL_POLYGON_OFFSET_LINE);

							// Enable polygon offset
							glEnable(GL_POLYGON_OFFSET_FILL);
							glPolygonOffset(Tools::UInt32ToFloat(nValue), Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::DepthBias)));
						}
						break;

					case PLRenderer::RenderState::DepthBias:
						if (!nValue && !GetRenderState(PLRenderer::RenderState::SlopeScaleDepthBias)) {
							// The bias is not applied to any line and point primitive within D3D, but OpenGL supports this
							// -> This is a quite useful feature for visualization so we don't want to miss it
							glDisable(GL_POLYGON_OFFSET_POINT);
							glDisable(GL_POLYGON_OFFSET_LINE);

							// Disable polygon offset
							glDisable(GL_POLYGON_OFFSET_FILL);
						} else {
							// The bias is not applied to any line and point primitive within D3D, but OpenGL supports this
							// -> This is a quite useful feature for visualization so we don't want to miss it
							glEnable(GL_POLYGON_OFFSET_POINT);
							glEnable(GL_POLYGON_OFFSET_LINE);

							// Enable polygon offset
							glEnable(GL_POLYGON_OFFSET_FILL);
							glPolygonOffset(Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::SlopeScaleDepthBias)), Tools::UInt32ToFloat(nValue));
						}
						break;

					case PLRenderer::RenderState::BlendEnable:
					case PLRenderer::RenderState::SrcBlendFunc:
					case PLRenderer::RenderState::DstBlendFunc:
					case PLRenderer::RenderState::StencilEnable:
					case PLRenderer::RenderState::StencilFunc:
					case PLRenderer::RenderState::StencilRef:
					case PLRenderer::RenderState::StencilMask:
					case PLRenderer::RenderState::StencilFail:
					case PLRenderer::RenderState::StencilZFail:
					case PLRenderer::RenderState::StencilPass:
					case PLRenderer::RenderState::TwoSidedStencilMode:
					case PLRenderer::RenderState::CCWStencilFunc:
					case PLRenderer::RenderState::CCWStencilFail:
					case PLRenderer::RenderState::CCWStencilZFail:
					case PLRenderer::RenderState::CCWStencilPass:
					case PLRenderer::RenderState::PointSize:
					case PLRenderer::RenderState::PointScaleEnable:
					case PLRenderer::RenderState::PointSizeMin:
					case PLRenderer::RenderState::PointSizeMax:
					case PLRenderer::RenderState::PointScaleA:
					case PLRenderer::RenderState::PointScaleB:
					case PLRenderer::RenderState::PointScaleC:
					case PLRenderer::RenderState::LineWidth:
					case PLRenderer::RenderState::TessellationFactor:
					case PLRenderer::RenderState::TessellationMode:
					case PLRenderer::RenderState::PointSpriteEnable:
					case PLRenderer::RenderState::DitherEnable:
					case PLRenderer::RenderState::ScissorTestEnable:
					case PLRenderer::RenderState::MultisampleEnable:
					case PLRenderer::RenderState::DepthClamp:
					case PLRenderer::RenderState::InvCullMode:
					case PLRenderer::RenderState::FixedFillMode:
					case PLRenderer::RenderState::Number:
					case PLRenderer::RenderState::Unknown:
					default:
						// Those are handled below
						break;
				}
			} else {
				switch (nState) {
				// Blend
					case PLRenderer::RenderState::BlendEnable:
						if (nValue == 0)
							glDisable(GL_BLEND);
						else if (nValue == 1)
							glEnable(GL_BLEND);
						else
							return false; // Error, invalid value!
						break;

					case PLRenderer::RenderState::SrcBlendFunc:
					{
						const uint32 &nAPIValue1 = m_cPLE_BLENDWrapper[nValue];
						const uint32 &nAPIValue2 = m_cPLE_BLENDWrapper[GetRenderState(PLRenderer::RenderState::DstBlendFunc)];
						if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null))
							glBlendFunc(nAPIValue1, nAPIValue2);
						else
							return false; // Error, invalid value!
						break;
					}

					case PLRenderer::RenderState::DstBlendFunc:
					{
						const uint32 &nAPIValue1 = m_cPLE_BLENDWrapper[nValue];
						const uint32 &nAPIValue2 = m_cPLE_BLENDWrapper[GetRenderState(PLRenderer::RenderState::SrcBlendFunc)];
						if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null))
							glBlendFunc(nAPIValue2, nAPIValue1);
						else
							return false; // Error, invalid value!
						break;
					}

				// Stencil
					case PLRenderer::RenderState::StencilEnable:
						if (nValue == 0)
							glDisable(GL_STENCIL_TEST);
						else if (nValue == 1)
							glEnable(GL_STENCIL_TEST);
						else
							return false; // Error, invalid value!
						break;

					case PLRenderer::RenderState::StencilFunc:
						// For ATI GPU's
						if (cExtensions.IsGL_ATI_separate_stencil()) {
							if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode)) {
								// Two sided
								const uint32 &nAPIValue1 = m_cPLE_CMPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFunc)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null))
									glStencilFuncSeparateATI(nAPIValue2, nAPIValue1, GetRenderState(PLRenderer::RenderState::StencilRef), GetRenderState(PLRenderer::RenderState::StencilMask));
								else
									return false; // Error, invalid value!
							} else {
								// No two sided
								const uint32 &nAPIValue = m_cPLE_CMPWrapper[nValue];
								if (&nAPIValue != &Array<uint32>::Null)
									glStencilFuncSeparateATI(nAPIValue, nAPIValue, GetRenderState(PLRenderer::RenderState::StencilRef), GetRenderState(PLRenderer::RenderState::StencilMask));
								else
									return false; // Error, invalid value!
							}

						// For other GPU's
						} else {
							// Set stencil face if required
							if (m_sCapabilities.bTwoSidedStencils && GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode))
								glActiveStencilFaceEXT(GL_BACK);
							// Set stencil function
							const uint32 &nAPIValue = m_cPLE_CMPWrapper[nValue];
							if (&nAPIValue != &Array<uint32>::Null)
								glStencilFunc(nAPIValue, GetRenderState(PLRenderer::RenderState::StencilRef), GetRenderState(PLRenderer::RenderState::StencilMask));
							else
								return false; // Error, invalid value!
						}
						break;

					case PLRenderer::RenderState::StencilRef:
						// For ATI GPU's
						if (cExtensions.IsGL_ATI_separate_stencil()) {
							if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode)) {
								// Two sided
								const uint32 &nAPIValue1 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
								const uint32 &nAPIValue2 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFunc)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null))
									glStencilFuncSeparateATI(nAPIValue2, nAPIValue1, nValue, GetRenderState(PLRenderer::RenderState::StencilMask));
								else
									return false; // Error, invalid value!
							} else {
								// No two sided
								const uint32 &nAPIValue = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
								if (&nAPIValue != &Array<uint32>::Null)
									glStencilFuncSeparateATI(nAPIValue, nAPIValue, nValue, GetRenderState(PLRenderer::RenderState::StencilMask));
								else
									return false; // Error, invalid value!
							}

						// For other GPU's
						} else {
							// Set stencil function
							const uint32 &nAPIValue = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
							if (&nAPIValue != &Array<uint32>::Null)
								glStencilFunc(nAPIValue, nValue, GetRenderState(PLRenderer::RenderState::StencilMask));
							else
								return false; // Error, invalid value!
						}
						break;

					case PLRenderer::RenderState::StencilMask:
						// For ATI GPU's
						if (cExtensions.IsGL_ATI_separate_stencil()) {
							if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode)) {
								// Two sided
								const uint32 &nAPIValue1 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
								const uint32 &nAPIValue2 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFunc)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null))
									glStencilFuncSeparateATI(nAPIValue2, nAPIValue1, GetRenderState(PLRenderer::RenderState::StencilRef), nValue);
								else
									return false; // Error, invalid value!
							} else {
								// No two sided
								const uint32 &nAPIValue = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
								if (&nAPIValue != &Array<uint32>::Null)
									glStencilFuncSeparateATI(nAPIValue, nAPIValue, GetRenderState(PLRenderer::RenderState::StencilRef), nValue);
								else
									return false; // Error, invalid value!
							}

						// For other GPU's
						} else {
							// Set stencil function
							const uint32 &nAPIValue = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
							if (&nAPIValue != &Array<uint32>::Null)
								glStencilFunc(nAPIValue, GetRenderState(PLRenderer::RenderState::StencilRef), nValue);
							else
								return false; // Error, invalid value!
						}
						break;

					case PLRenderer::RenderState::StencilFail:
						// For ATI GPU's
						if (cExtensions.IsGL_ATI_separate_stencil()) {
							const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
							const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilZFail)];
							const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilPass)];
							if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null)) {
								if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode))
									glStencilOpSeparateATI(GL_BACK, nAPIValue1, nAPIValue2, nAPIValue3);
								else
									glStencilOpSeparateATI(GL_FRONT_AND_BACK, nAPIValue1, nAPIValue2, nAPIValue3);
							} else {
								// Error, invalid value!
								return false;
							}

						// For other GPU's
						} else {
							// Set stencil face if required
							if (m_sCapabilities.bTwoSidedStencils && GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode))
								glActiveStencilFaceEXT(GL_BACK);
							// Set stencil operation
							const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
							const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilZFail)];
							const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilPass)];
							if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
								glStencilOp(nAPIValue1, nAPIValue2, nAPIValue3);
							else
								return false; // Error, invalid value!
						}
						break;

					case PLRenderer::RenderState::StencilZFail:
						// For ATI GPU's
						if (cExtensions.IsGL_ATI_separate_stencil()) {
							const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
							const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilFail)];
							const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilPass)];
							if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null)) {
								if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode))
									glStencilOpSeparateATI(GL_BACK, nAPIValue2, nAPIValue1, nAPIValue3);
								else
									glStencilOpSeparateATI(GL_FRONT_AND_BACK, nAPIValue2, nAPIValue1, nAPIValue3);
							} else {
								// Error, invalid value!
								return false;
							}

						// For other GPU's
						} else {
							// Set stencil face if required
							if (m_sCapabilities.bTwoSidedStencils && GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode))
								glActiveStencilFaceEXT(GL_BACK);
							// Set stencil operation
							const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
							const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilFail)];
							const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilPass)];
							if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
								glStencilOp(nAPIValue2, nAPIValue1, nAPIValue3);
							else
								return false; // Error, invalid value!
						}
						break;

					case PLRenderer::RenderState::StencilPass:
						// For ATI GPU's
						if (cExtensions.IsGL_ATI_separate_stencil()) {
							const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
							const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilFail)];
							const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilZFail)];
							if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null)) {
								if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode))
									glStencilOpSeparateATI(GL_BACK, nAPIValue2, nAPIValue3, nAPIValue1);
								else
									glStencilOpSeparateATI(GL_FRONT_AND_BACK, nAPIValue2, nAPIValue3, nAPIValue1);
							} else {
								// Error, invalid value!
								return false;
							}

						// For other GPU's
						} else {
							// Set stencil face if required
							if (m_sCapabilities.bTwoSidedStencils && GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode))
								glActiveStencilFaceEXT(GL_BACK);
							// Set stencil operation
							const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
							const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilFail)];
							const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilZFail)];
							if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
								glStencilOp(nAPIValue2, nAPIValue3, nAPIValue1);
							else
								return false; // Error, invalid value!
						}
						break;

					case PLRenderer::RenderState::TwoSidedStencilMode:
						if (!m_sCapabilities.bTwoSidedStencils)
							return false; // Error, two sided stencils are not supported!

						// For ATI GPU's
						if (cExtensions.IsGL_ATI_separate_stencil()) {
							{ // No two sided stencil enable/disable required, but set the correct settings for sure
								const uint32 &nAPIValue1 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
								const uint32 &nAPIValue2 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFunc)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null))
									glStencilFuncSeparateATI(nAPIValue2, nAPIValue1, GetRenderState(PLRenderer::RenderState::StencilRef), nValue);
								else
									return false; // Error, invalid value!
							}
							{ // Back
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilFail)];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilZFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilPass)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOpSeparateATI(GL_BACK, nAPIValue1, nAPIValue2, nAPIValue3);
								else
									return false; // Error, invalid value!
							}
							{ // Front and back
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFail)];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilZFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilPass)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOpSeparateATI(GL_FRONT, nAPIValue1, nAPIValue2, nAPIValue3);
								else
									return false; // Error, invalid value!
							}

						// For other GPU's
						} else {
							if (nValue == 0)
								glDisable(GL_STENCIL_TEST_TWO_SIDE_EXT);
							else if (nValue == 1) {
								glEnable(GL_STENCIL_TEST_TWO_SIDE_EXT);
								// Back
								glActiveStencilFaceEXT(GL_BACK);
								{ // Function
									const uint32 &nAPIValue = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
									if (&nAPIValue != &Array<uint32>::Null)
										glStencilFunc(nAPIValue, GetRenderState(PLRenderer::RenderState::StencilRef), GetRenderState(PLRenderer::RenderState::StencilMask));
									else
										return false; // Error, invalid value!
								}
								{ // Operation
									const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilFail)];
									const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilZFail)];
									const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::StencilPass)];
									if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
										glStencilOp(nAPIValue1, nAPIValue2, nAPIValue3);
									else
										return false; // Error, invalid value!
								}
								// Front
								glActiveStencilFaceEXT(GL_FRONT);
								{ // Function
									const uint32 &nAPIValue = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFunc)];
									if (&nAPIValue != &Array<uint32>::Null)
										glStencilFunc(nAPIValue, GetRenderState(PLRenderer::RenderState::StencilRef), GetRenderState(PLRenderer::RenderState::StencilMask));
									else
										return false; // Error, invalid value!
								}
								{ // Operation
									const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFail)];
									const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilZFail)];
									const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilPass)];
									if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
										glStencilOp(nAPIValue1, nAPIValue2, nAPIValue3);
									else
										return false; // Error, invalid value!
								}
							} else {
								// Error, invalid value!
								return false;
							}
						}
						break;

					case PLRenderer::RenderState::CCWStencilFunc:
						if (!m_sCapabilities.bTwoSidedStencils)
							return false; // Error, two sided stencils are not supported!
						if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode)) {
							// For ATI GPU's
							if (cExtensions.IsGL_ATI_separate_stencil()) {
								const uint32 &nAPIValue1 = m_cPLE_CMPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_CMPWrapper[GetRenderState(PLRenderer::RenderState::StencilFunc)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null))
									glStencilFuncSeparateATI(nAPIValue1, nAPIValue2, GetRenderState(PLRenderer::RenderState::StencilRef), GetRenderState(PLRenderer::RenderState::StencilMask));
								else
									return false; // Error, invalid value!

							// For other GPU's
							} else {
								// Set stencil function
								const uint32 &nAPIValue = m_cPLE_CMPWrapper[nValue];
								if (&nAPIValue != &Array<uint32>::Null) {
									// Set stencil face
									glActiveStencilFaceEXT(GL_FRONT);
									// Set stencil function
									glStencilFunc(nAPIValue, GetRenderState(PLRenderer::RenderState::StencilRef), GetRenderState(PLRenderer::RenderState::StencilMask));
								} else {
									// Error, invalid value!
									return false;
								}
							}
						}
						break;

					case PLRenderer::RenderState::CCWStencilFail:
						if (!m_sCapabilities.bTwoSidedStencils)
							return false; // Error, two sided stencils are not supported!
						if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode)) {
							// For ATI GPU's
							if (cExtensions.IsGL_ATI_separate_stencil()) {
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilZFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilPass)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOpSeparateATI(GL_FRONT, nAPIValue1, nAPIValue2, nAPIValue3);
								else
									return false; // Error, invalid value!

							// For other GPU's
							} else {
								// Set stencil face
								glActiveStencilFaceEXT(GL_FRONT);
								// Set stencil operation
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilZFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilPass)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOp(nAPIValue1, nAPIValue2, nAPIValue3);
								else
									return false; // Error, invalid value!
							}
						}
						break;

					case PLRenderer::RenderState::CCWStencilZFail:
						if (!m_sCapabilities.bTwoSidedStencils)
							return false; // Error, two sided stencils are not supported!
						if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode)) {
							// For ATI GPU's
							if (cExtensions.IsGL_ATI_separate_stencil()) {
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilPass)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOpSeparateATI(GL_FRONT, nAPIValue2, nAPIValue1, nAPIValue3);
								else
									return false; // Error, invalid value!

							// For other GPU's
							} else {
								// Set stencil face
								glActiveStencilFaceEXT(GL_FRONT);
								// Set stencil operation
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilPass)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOp(nAPIValue2, nAPIValue1, nAPIValue3);
								else
									return false; // Error, invalid value!
							}
						}
						break;

					case PLRenderer::RenderState::CCWStencilPass:
						if (!m_sCapabilities.bTwoSidedStencils)
							return false; // Error, two sided stencils are not supported!
						if (GetRenderState(PLRenderer::RenderState::TwoSidedStencilMode)) {
							// For ATI GPU's
							if (cExtensions.IsGL_ATI_separate_stencil()) {
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilZFail)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOpSeparateATI(GL_FRONT, nAPIValue2, nAPIValue3, nAPIValue1);
								else
									return false; // Error, invalid value!

							// For other GPU's
							} else {
								// Set stencil face
								glActiveStencilFaceEXT(GL_FRONT);
								// Set stencil operation
								const uint32 &nAPIValue1 = m_cPLE_SOPWrapper[nValue];
								const uint32 &nAPIValue2 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilFail)];
								const uint32 &nAPIValue3 = m_cPLE_SOPWrapper[GetRenderState(PLRenderer::RenderState::CCWStencilZFail)];
								if ((&nAPIValue1 != &Array<uint32>::Null) && (&nAPIValue2 != &Array<uint32>::Null) && (&nAPIValue3 != &Array<uint32>::Null))
									glStencilOp(nAPIValue2, nAPIValue3, nAPIValue1);
								else
									return false; // Error, invalid value!
							}
						}
						break;

					case PLRenderer::RenderState::FillMode:
					case PLRenderer::RenderState::CullMode:
					case PLRenderer::RenderState::ZEnable:
					case PLRenderer::RenderState::ZWriteEnable:
					case PLRenderer::RenderState::ZFunc:
					case PLRenderer::RenderState::ZBias:
					case PLRenderer::RenderState::SlopeScaleDepthBias:
					case PLRenderer::RenderState::DepthBias:
					case PLRenderer::RenderState::PointSize:
					case PLRenderer::RenderState::PointScaleEnable:
					case PLRenderer::RenderState::PointSizeMin:
					case PLRenderer::RenderState::PointSizeMax:
					case PLRenderer::RenderState::PointScaleA:
					case PLRenderer::RenderState::PointScaleB:
					case PLRenderer::RenderState::PointScaleC:
					case PLRenderer::RenderState::LineWidth:
					case PLRenderer::RenderState::TessellationFactor:
					case PLRenderer::RenderState::TessellationMode:
					case PLRenderer::RenderState::PointSpriteEnable:
					case PLRenderer::RenderState::DitherEnable:
					case PLRenderer::RenderState::ScissorTestEnable:
					case PLRenderer::RenderState::MultisampleEnable:
					case PLRenderer::RenderState::DepthClamp:
					case PLRenderer::RenderState::InvCullMode:
					case PLRenderer::RenderState::FixedFillMode:
					case PLRenderer::RenderState::Number:
					case PLRenderer::RenderState::Unknown:
					default:
						// Those are handled above/below
						break;
				}
			}
		} else {
			switch (nState) {
			// Point and line
				case PLRenderer::RenderState::PointSize:
					glPointSize(Tools::UInt32ToFloat(nValue));
					break;

				case PLRenderer::RenderState::PointScaleEnable:
				{
					// Point parameters supported?
					if (!m_sCapabilities.bPointParameters)
						return false; // Error, not supported!

					// Enable or disable point scale?
					if (nValue) {
						// There's no special "enable" function in OpenGL, just set proper point parameters...
						const float fQ[] = { Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleA)),
											 Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleB)),
											 Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleC)) };
						glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, fQ);
					} else {
						// There's no special "disable" function in OpenGL, just set proper point parameters...
						static const float fQ[] = { 1.0f, 0.0f, 0.0f };
						glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, fQ);
					}
					break;
				}

				case PLRenderer::RenderState::PointSizeMin:
					// Point parameters supported?
					if (!m_sCapabilities.bPointParameters)
						return false; // Error, not supported!

					glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, Tools::UInt32ToFloat(nValue));
					break;

				case PLRenderer::RenderState::PointSizeMax:
					// Point parameters supported?
					if (!m_sCapabilities.bPointParameters)
						return false; // Error, not supported!

					glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, Tools::UInt32ToFloat(nValue));
					break;

				case PLRenderer::RenderState::PointScaleA:
					// Point parameters supported?
					if (!m_sCapabilities.bPointParameters)
						return false; // Error, not supported!

					// Point scale enabled? Not not, do not touch the settings!
					if (GetRenderState(PLRenderer::RenderState::PointScaleEnable)) {
						const float fQ[] = { Tools::UInt32ToFloat(nValue),
											 Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleB)),
											 Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleC)) };
						glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, fQ);
					}
					break;

				case PLRenderer::RenderState::PointScaleB:
					// Point parameters supported?
					if (!m_sCapabilities.bPointParameters)
						return false; // Error, not supported!

					// Point scale enabled? Not not, do not touch the settings!
					if (GetRenderState(PLRenderer::RenderState::PointScaleEnable)) {
						const float fQ[] = { Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleA)),
											 Tools::UInt32ToFloat(nValue),
											 Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleC)) };
						glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, fQ);
					}
					break;

				case PLRenderer::RenderState::PointScaleC:
					// Point parameters supported?
					if (!m_sCapabilities.bPointParameters)
						return false; // Error, not supported!

					// Point scale enabled? Not not, do not touch the settings!
					if (GetRenderState(PLRenderer::RenderState::PointScaleEnable)) {
						const float fQ[] = { Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleA)),
											 Tools::UInt32ToFloat(GetRenderState(PLRenderer::RenderState::PointScaleB)),
											 Tools::UInt32ToFloat(nValue) };
						glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, fQ);
					}
					break;

				case PLRenderer::RenderState::LineWidth:
					glLineWidth(Tools::UInt32ToFloat(nValue));
					break;

			// Tessellation
				case PLRenderer::RenderState::TessellationFactor:
					if (!cExtensions.IsGL_AMD_vertex_shader_tessellator())
						return false; // Error, tessellation not supported!
					if (nValue > m_sCapabilities.nMaxTessellationFactor)
						return false; // Error, invalid value!
					glTessellationFactorAMD(Tools::UInt32ToFloat(nValue));
					break;

				case PLRenderer::RenderState::TessellationMode:
					if (!cExtensions.IsGL_AMD_vertex_shader_tessellator())
						return false; // Error, tessellation not supported!
					if (nValue == PLRenderer::TessellationMode::Discrete)
						glTessellationModeAMD(GL_DISCRETE_AMD);
					else if (nValue == PLRenderer::TessellationMode::Continuous)
						glTessellationModeAMD(GL_CONTINUOUS_AMD);
					else
						return false; // Error, invalid value!
					break;

			// Misc
				case PLRenderer::RenderState::PointSpriteEnable:
					// Point sprite supported?
					if (!m_sCapabilities.bPointSprite)
						return false; // Error, not supported!

					if (nValue == 0) {
						glDisable(GL_POINT_SPRITE_ARB);
						glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, false);
					} else if (nValue == 1) {
						glEnable(GL_POINT_SPRITE_ARB);
						glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, true);
					} else {
						// Error, invalid value!
						return false;
					}
					break;

				case PLRenderer::RenderState::DitherEnable:
					if (nValue == 0)
						glDisable(GL_DITHER);
					else if (nValue == 1)
						glEnable(GL_DITHER);
					else
						return false; // Error, invalid value!
					break;

				case PLRenderer::RenderState::ScissorTestEnable:
					if (nValue == 0)
						glDisable(GL_SCISSOR_TEST);
					else if (nValue == 1)
						glEnable(GL_SCISSOR_TEST);
					else
						return false; // Error, invalid value!
					break;

				case PLRenderer::RenderState::MultisampleEnable:
					// Multisample antialiasing samples
					if (m_sCapabilities.nMultisampleAntialiasingSamples < 2)
						return false; // Error, not supported!

					if (nValue == 0) {
						glDisable(GL_MULTISAMPLE_ARB);
					} else if (nValue == 1) {
						glEnable(GL_MULTISAMPLE_ARB);
					} else {
						// Error, invalid value!
						return false;
					}
					break;

				case PLRenderer::RenderState::DepthClamp:
					if (!cExtensions.IsGL_ARB_depth_clamp())
						return false; // Error, depth not supported!

					if (nValue == 0) {
						glDisable(GL_DEPTH_CLAMP);
					} else if (nValue == 1) {
						glEnable(GL_DEPTH_CLAMP);
					} else {
						// Error, invalid value!
						return false;
					}
					break;

				case PLRenderer::RenderState::InvCullMode:
					if (GetRenderState(PLRenderer::RenderState::CullMode) != PLRenderer::Cull::None) {
						// Invert cull mode?
						bool bAPISwapY = false;
						if (m_cCurrentSurface.GetSurface())
							bAPISwapY = m_cCurrentSurface.GetSurface()->IsAPISwapY();
						if ((!bAPISwapY && nValue) || (bAPISwapY && !nValue)) { // Invert current active cull mode
							switch (GetRenderState(PLRenderer::RenderState::CullMode)) {
								case PLRenderer::Cull::CW:
									nValue = PLRenderer::Cull::CCW;
									break;

								case PLRenderer::Cull::CCW:
									nValue = PLRenderer::Cull::CW;
									break;
							}
						} else { // Use current cull mode
							switch (GetRenderState(PLRenderer::RenderState::CullMode)) {
								case PLRenderer::Cull::CW:
									nValue = PLRenderer::Cull::CW;
									break;

								case PLRenderer::Cull::CCW:
									nValue = PLRenderer::Cull::CCW;
									break;
							}
						}

						// Set the state
						const uint32 &nAPIValue = m_cPLE_CULLWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null)
							glFrontFace(nAPIValue);
						else
							return false; // Error, invalid value!
					}
					break;

				case PLRenderer::RenderState::FixedFillMode:
					if (nValue != PLRenderer::Fill::Unknown) {
						const uint32 &nAPIValue = m_cPLE_FILLWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null)
							glPolygonMode(GL_FRONT_AND_BACK, nAPIValue);
						else
							return false; // Error, invalid value!
					} else {
						const uint32 &nAPIValue = m_cPLE_FILLWrapper[GetRenderState(PLRenderer::RenderState::FillMode)];
						if (&nAPIValue != &Array<uint32>::Null)
							glPolygonMode(GL_FRONT_AND_BACK, nAPIValue);
						else
							return false; // Error, invalid value!
					}
					break;

				case PLRenderer::RenderState::FillMode:
				case PLRenderer::RenderState::CullMode:
				case PLRenderer::RenderState::ZEnable:
				case PLRenderer::RenderState::ZWriteEnable:
				case PLRenderer::RenderState::ZFunc:
				case PLRenderer::RenderState::ZBias:
				case PLRenderer::RenderState::SlopeScaleDepthBias:
				case PLRenderer::RenderState::DepthBias:
				case PLRenderer::RenderState::BlendEnable:
				case PLRenderer::RenderState::SrcBlendFunc:
				case PLRenderer::RenderState::DstBlendFunc:	
				case PLRenderer::RenderState::StencilEnable:
				case PLRenderer::RenderState::StencilFunc:
				case PLRenderer::RenderState::StencilRef:
				case PLRenderer::RenderState::StencilMask:
				case PLRenderer::RenderState::StencilFail:
				case PLRenderer::RenderState::StencilZFail:
				case PLRenderer::RenderState::StencilPass:
				case PLRenderer::RenderState::TwoSidedStencilMode:
				case PLRenderer::RenderState::CCWStencilFunc:
				case PLRenderer::RenderState::CCWStencilFail:
				case PLRenderer::RenderState::CCWStencilZFail:
				case PLRenderer::RenderState::CCWStencilPass:
				case PLRenderer::RenderState::Number:
				case PLRenderer::RenderState::Unknown:
				default:
					// Those are handled above
					break;
			}
		}
	}

	// All went fine
	return true;
}

bool Renderer::SetSamplerState(uint32 nStage, PLRenderer::Sampler::Enum nState, uint32 nValue)
{
	// Check if the stage is correct and check if the state is a valid sampler member
	if (nStage >= m_sCapabilities.nMaxTextureUnits || nState >= PLRenderer::Sampler::Number)
		return false; // Error!

	// Set the sampler state
	m_ppnSamplerState[nStage][nState] = nValue;

	// Check if this sampler state is already set to this value and whether currently a correct
	// texture buffer is used
	if (m_ppnInternalSamplerState[nStage][nState] != nValue && m_nTextureBufferTypes[nStage]) {
		// Check whether mipmapping is allowed, if not, set a proper internal texture filter mode
		// for correct rendering...
		if (nState == PLRenderer::Sampler::MipFilter) {
			PLRenderer::TextureBuffer *pTextureBuffer = m_ppCurrentTextureBuffer[nStage];
			if (pTextureBuffer && !(pTextureBuffer->GetFlags() & PLRenderer::TextureBuffer::Mipmaps)) {
				nValue = PLRenderer::TextureFiltering::None;

				// Check if this sampler state is already set to this value
				if (m_ppnInternalSamplerState[nStage][nState] == nValue)
					return true; // Nothing to do here :)
			}
		}

		// Set the sampler state
		m_ppnInternalSamplerState[nStage][nState] = nValue;
		uint32 nType = m_nTextureBufferTypes[nStage];
		m_sStatistics.nSamplerStateChanges++;

		// Activate the correct OpenGL texture stage
		if (glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB+nStage);

		// Set sampler state
		switch (nState) {
		// Address modes
			case PLRenderer::Sampler::AddressU:
			{
				const uint32 &nAPIValue = m_cPLE_TAWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null)
					glTexParameteri(nType, GL_TEXTURE_WRAP_S, nAPIValue);
				else
					return false; // Error, invalid value!
				break;
			}

			case PLRenderer::Sampler::AddressV:
			{
				const uint32 &nAPIValue = m_cPLE_TAWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null)
					glTexParameteri(nType, GL_TEXTURE_WRAP_T, nAPIValue);
				else
					return false; // Error, invalid value!
				break;
			}

			case PLRenderer::Sampler::AddressW:
			{
				const uint32 &nAPIValue = m_cPLE_TAWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null)
					glTexParameteri(nType, GL_TEXTURE_WRAP_R_EXT, nAPIValue);
				else
					return false; // Error, invalid value!
				break;
			}

		// Filter
			case PLRenderer::Sampler::MinFilter:
				// Combine with existing mip filter
				glTexParameteri(nType, GL_TEXTURE_MIN_FILTER, GetCombinedMinMipFilter(nStage));
				break;

			case PLRenderer::Sampler::MagFilter:
				switch (nValue) {
					case PLRenderer::TextureFiltering::Anisotropic: // GL treats linear and aniso the same
					case PLRenderer::TextureFiltering::Linear:
						glTexParameteri(nType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						break;

					case PLRenderer::TextureFiltering::Point:
					case PLRenderer::TextureFiltering::None:
						glTexParameteri(nType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						break;
				}
				break;

			case PLRenderer::Sampler::MipFilter:
				// Combine with existing min filter
				glTexParameteri(nType, GL_TEXTURE_MIN_FILTER, GetCombinedMinMipFilter(nStage));
				break;

		// Misc
			case PLRenderer::Sampler::MipmapLODBias:
				glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, Tools::UInt32ToFloat(nValue));
				break;

			case PLRenderer::Sampler::MaxMapLevel:
				glTexParameteri(nType, GL_TEXTURE_MAX_LEVEL, nValue);
				break;

			case PLRenderer::Sampler::MaxAnisotropy:
				if (m_sCapabilities.nMaxAnisotropy) {
					if (nValue > m_sCapabilities.nMaxAnisotropy)
						nValue = m_sCapabilities.nMaxAnisotropy;
					glTexParameteri(nType, GL_TEXTURE_MAX_ANISOTROPY_EXT, nValue);
					break;
				}
				break;

			case PLRenderer::Sampler::Number:
			case PLRenderer::Sampler::Unknown:
			default:
				return false; // Invalid sampler state!
		}
	}

	// All went fine
	return true;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
bool Renderer::BeginScene()
{
	// Is the scene rendering already active?
	if (m_bSceneRendering)
		return false; // Error!

	// Activate scene rendering
	m_bSceneRendering = true;

	// Done
	return true;
}

bool Renderer::EndScene()
{
	// Is scene rendering active?
	if (!m_bSceneRendering)
		return false; // Error!

	// End scene
	m_bSceneRendering = false;

	// Done
	return true;
}

bool Renderer::SetViewport(const PLMath::Rectangle *pRectangle, float fMinZ, float fMaxZ)
{
	// Call base implementation
	PLRenderer::RendererBackend::SetViewport(pRectangle);

	// OpenGL assumes LOWER-left corner of the viewport rectangle, in pixels
	// and TOP-left corner given - so fit it :)
	GLint nY = static_cast<GLint>(m_cViewportRect.GetY());
	if (m_cCurrentSurface.GetSurface())
		nY = m_cCurrentSurface.GetSurface()->GetSize().y - static_cast<GLint>(m_cViewportRect.vMax.y);

	// Set viewport
	glViewport(static_cast<GLint>(m_cViewportRect.GetX()), nY, static_cast<GLint>(m_cViewportRect.GetWidth()), static_cast<GLint>(m_cViewportRect.GetHeight()));

	// Set depth range
	glDepthRange(fMinZ, fMaxZ);

	// Done
	return true;
}

bool Renderer::SetScissorRect(const PLMath::Rectangle *pRectangle)
{
	// Call base implementation
	PLRenderer::RendererBackend::SetScissorRect(pRectangle);

	// OpenGL assumes LOWER-left corner of the viewport rectangle, in pixels
	// and TOP-left corner given - so fit it :)
	GLint nY = static_cast<GLint>(m_cScissorRect.GetY());
	if (m_cCurrentSurface.GetSurface())
		nY = m_cCurrentSurface.GetSurface()->GetSize().y - static_cast<GLint>(m_cScissorRect.vMax.y);

	// Set scissor rectangle
	glScissor(static_cast<GLint>(m_cScissorRect.GetX()), nY, static_cast<GLint>(m_cScissorRect.GetWidth()), static_cast<GLint>(m_cScissorRect.GetHeight()));

	// Done
	return true;
}

bool Renderer::GetDepthBounds(float &fZMin, float &fZMax) const
{
	if (m_sCapabilities.bDepthBoundsTest) {
		GLfloat fValues[2];
		glGetFloatv(GL_DEPTH_BOUNDS_EXT, fValues);
		fZMin = fValues[0];
		fZMax = fValues[1];

		// Done
		return true;
	} else {
		fZMin = 0.0f;
		fZMax = 1.0f;

		// Error!
		return false;
	}
}

bool Renderer::SetDepthBounds(float fZMin, float fZMax)
{
	if (!m_sCapabilities.bDepthBoundsTest)
		return false; // Error!
	if (fZMin == 0.0f && fZMax == 1.0f)
		glDisable(GL_DEPTH_BOUNDS_TEST_EXT);
	else
		glEnable(GL_DEPTH_BOUNDS_TEST_EXT);
	glDepthBoundsEXT(fZMin, fZMax);

	// Done
	return true;
}

void Renderer::GetColorMask(bool &bRed, bool &bGreen, bool &bBlue, bool &bAlpha) const
{
	// Get color mask
	bRed   = m_bColorMask[0];
	bGreen = m_bColorMask[1];
	bBlue  = m_bColorMask[2];
	bAlpha = m_bColorMask[3];
}

bool Renderer::SetColorMask(bool bRed, bool bGreen, bool bBlue, bool bAlpha)
{
	// Set color mask
	m_bColorMask[0] = bRed;
	m_bColorMask[1] = bGreen;
	m_bColorMask[2] = bBlue;
	m_bColorMask[3] = bAlpha;
	glColorMask(bRed, bGreen, bBlue, bAlpha);

	// Done
	return true;
}

bool Renderer::Clear(uint32 nFlags, const Color4 &cColor, float fZ, uint32 nStencil)
{
	// Get API flags
	uint32 nFlagsAPI = 0;
	if (nFlags & PLRenderer::Clear::Color)
		nFlagsAPI |= GL_COLOR_BUFFER_BIT;
	if (nFlags & PLRenderer::Clear::ZBuffer)
		nFlagsAPI |= GL_DEPTH_BUFFER_BIT;
	if (nFlags & PLRenderer::Clear::Stencil)
		nFlagsAPI |= GL_STENCIL_BUFFER_BIT;

	// Are API flags set?
	if (!nFlagsAPI)
		return false; // Error!

	// Set clear settings
	if (nFlags & PLRenderer::Clear::Color) {
		// [HACK](CO 2012.03.31 *issue only tested on Windows*) Funny GPU driver optimization? For years a simple
		//   "glClearColor(cColor.r, cColor.g, cColor.b, cColor.a);" (e.g. "glClearColor(0.0f, 0.0f, 0.0f, 0.0f)")
		// worked without any issues when rendering within a floating point buffer. A few weeks ago
		// I noticed on a NVIDIA GeForce GTX 285 (up to date driver) that the floating point buffer (color target)
		// was not cleared as soon as nothing was writing into the depth buffer. As soon as something wrote into
		// the depth buffer the color buffer was cleared. Due to lack of time and not having the issue on my
		// AMD ATI Mobility Radeon HD 4850 I didn't look at once into it. Now, with my new AMD Radeon HD 6850M
		// and the same driver version I used on my AMD ATI Mobility Radeon HD 4850, I now had the exact same issue.
		// WOW! GPU graphics programming, one just has to love it. Played around a little bit in order to find a
		// working solution. "glClearColor(0.0f, 0.0f, 0.0f, 0.0f)" didn't work, but as soon as I set
		// "glClearColor(0.0f, 0.0f, 0.0f, 5.96046448e-08f)" with 5.96046448e-08f been the smallest positive half
		// value (rendering into a 16 bit floating point buffer), the issue was gone. I have no idea what's going
		// on and therefore at this point in time I have to assume that it's a funny GPU driver optimization used
		// on modern GPU architectures - used by NVIDIA and AMD (... wouldn't be the first time...). Can this really
		// be or do I miss anything? Anyway, no time to spend more time on this topic right now. It "just" has to
		// work as it did before on not up-to-date graphics cards and/or a little bit older GPU drivers. So I added
		// this ugly hack and the llooong comment in order to explain in detail why this hack exists.
		glClearColor(cColor.r, cColor.g, cColor.b, cColor.a ? cColor.a : 5.96046448e-08f);	// "5.96046448e-08f" = smallest positive half
	}
	uint32 nZWriteEnableT = 0;
	if (nFlags & PLRenderer::Clear::ZBuffer) {
		nZWriteEnableT = GetRenderState(PLRenderer::RenderState::ZWriteEnable);
		SetRenderState(PLRenderer::RenderState::ZWriteEnable, true);
		glClearDepth(fZ);
	}
	if (nFlags & PLRenderer::Clear::Stencil)
		glClearStencil(nStencil);

	// Set color mask to default because OpenGL is using the color mask also for clearing (while DirectX does not :/)
	if (nFlags & PLRenderer::Clear::Color)
		glColorMask(true, true, true, true);

	// Clear
	glClear(nFlagsAPI);

	// Reset to current color mask
	if (nFlags & PLRenderer::Clear::Color) 
		glColorMask(m_bColorMask[0], m_bColorMask[1], m_bColorMask[2], m_bColorMask[3]);

	// Reset z write enable
	if (nFlags & PLRenderer::Clear::ZBuffer)
		SetRenderState(PLRenderer::RenderState::ZWriteEnable, nZWriteEnableT);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Get/set current resources                             ]
//[-------------------------------------------------------]
bool Renderer::SetRenderTarget(PLRenderer::Surface *pSurface, uint8 nFace)
{
	bool bResult = true; // No error by default

	// Is this surface already the current render target?
	if (m_cCurrentSurface.GetSurface() != pSurface || m_nCurrentSurfaceFace != nFace) {
		// Make the dummy rendering context to the current one?
		if (pSurface) {
			// Check parameter
			if (!m_lstSurfaces.IsElement(pSurface))
				return false; // Error!

			// Check face index
			if (pSurface->GetType() == PLRenderer::Surface::TextureBuffer) {
				PLRenderer::SurfaceTextureBuffer *pSurfaceTextureBuffer = static_cast<PLRenderer::SurfaceTextureBuffer*>(pSurface);
				if (pSurfaceTextureBuffer->GetTextureBuffer()) {
					if (pSurfaceTextureBuffer->GetTextureBuffer()->GetType() == PLRenderer::Resource::TypeTextureBufferCube) {
						if (nFace > 5)
							return false; // Error!
					} else {
						if (nFace > 0)
							return false; // Error!
					}
				} else {
					// ??!
					return false;
				}
			} else {
				if (nFace > 0)
					return false; // Error!
			}

			if (m_cCurrentSurface.GetSurface())
				UnmakeSurfaceCurrent(*m_cCurrentSurface.GetSurface());
			m_cCurrentSurface.SetSurface(pSurface);

			// Make the surface to the current render target
			bResult = MakeSurfaceCurrent(*pSurface, nFace);
		} else {
			m_pContext->MakeDummyCurrent();
		}
	}

	// Setup viewport and scissor rectangle
	SetViewport();
	SetScissorRect();

	// Done
	return bResult;
}

bool Renderer::SetColorRenderTarget(PLRenderer::TextureBuffer *pTextureBuffer, uint8 nColorIndex, uint8 nFace)
{
	// Check current surface
	PLRenderer::Surface *pSurface = m_cCurrentSurface.GetSurface();
	if (pSurface && pSurface->GetType() == PLRenderer::Surface::TextureBuffer) {
		// Same texture buffer format?
		PLRenderer::SurfaceTextureBuffer *pSurfaceTextureBuffer = static_cast<PLRenderer::SurfaceTextureBuffer*>(pSurface);
		if (pTextureBuffer && pTextureBuffer->GetFormat() == pSurfaceTextureBuffer->GetFormat()) {
			// Setup
			static_cast<SurfaceTextureBuffer*>(m_cCurrentSurface.GetSurface())->SetColorRenderTarget(nColorIndex, pTextureBuffer);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool Renderer::SetDepthRenderTarget(PLRenderer::TextureBuffer *pTextureBuffer, uint8 nFace)
{
	// Check current surface
	PLRenderer::Surface *pSurface = m_cCurrentSurface.GetSurface();
	if (pSurface && pSurface->GetType() == PLRenderer::Surface::TextureBuffer) {
		// Setup
		static_cast<SurfaceTextureBuffer*>(m_cCurrentSurface.GetSurface())->SetDepthRenderTarget(pTextureBuffer);

		// Done
		return true;
	}

	// Error!
	return false;
}

bool Renderer::MakeScreenshot(PLGraphics::Image &cImage)
{
	// In case the current surface is a texture, we need to 'finish' the current rendering process
	PLRenderer::Surface *pSurface = m_cCurrentSurface.GetSurface();
	if (pSurface && pSurface->GetType() == PLRenderer::Surface::TextureBuffer)
		static_cast<SurfaceTextureBuffer*>(pSurface)->Finish();

	// Get viewport data
	GLint nViewPort[4];
	glGetIntegerv(GL_VIEWPORT, nViewPort);

	// Create image buffer
	cImage.Unload();
	ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
	pImageBuffer->CreateImage(DataByte, ColorRGBA, Vector3i(nViewPort[2], nViewPort[3], 1));

	// Get image data
	glReadPixels(nViewPort[0], nViewPort[1], nViewPort[2], nViewPort[3], GL_RGBA, GL_UNSIGNED_BYTE, pImageBuffer->GetData());

	// We need to flip the y axis because the image center of OpenGL is
	// lower left but the image center of our image class is upper left
	ImageEffects::FlipYAxis().Apply(*pImageBuffer);

	// Done
	return true;
}

bool Renderer::SetTextureBuffer(int nStage, PLRenderer::TextureBuffer *pTextureBuffer)
{
	// Set all available texture stages to this setting?
	if (nStage < 0) {
		for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++)
			SetTextureBuffer(i, pTextureBuffer);
	} else {
		// Check if the stage is correct
		if (nStage >= static_cast<int>(m_sCapabilities.nMaxTextureUnits))
			return false; // Error!

		// Is this texture buffer already set?
		if (m_ppCurrentTextureBuffer[nStage] == pTextureBuffer)
			return false; // Error!

		// Make this texture buffer to the renderers current one
		PLRenderer::TextureBuffer *pPreviousTextureBuffer = m_ppCurrentTextureBuffer[nStage];
		m_ppCurrentTextureBuffer[nStage] = pTextureBuffer;

		// The previous texture buffer needs a backup of the current sampler states
		if (pPreviousTextureBuffer) {
			uint32 *pnSamplerState = nullptr;
			switch (pPreviousTextureBuffer->GetType()) {
				case PLRenderer::Resource::TypeTextureBuffer1D:
					pnSamplerState = &static_cast<TextureBuffer1D*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer2D:
					pnSamplerState = &static_cast<TextureBuffer2D*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer2DArray:
					pnSamplerState = &static_cast<TextureBuffer2DArray*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBufferRectangle:
					pnSamplerState = &static_cast<TextureBufferRectangle*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer3D:
					pnSamplerState = &static_cast<TextureBuffer3D*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBufferCube:
					pnSamplerState = &static_cast<TextureBufferCube*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeIndexBuffer:
				case PLRenderer::Resource::TypeVertexBuffer:
				case PLRenderer::Resource::TypeUniformBuffer:
				case PLRenderer::Resource::TypeOcclusionQuery:
				case PLRenderer::Resource::TypeVertexShader:
				case PLRenderer::Resource::TypeTessellationControlShader:
				case PLRenderer::Resource::TypeTessellationEvaluationShader:
				case PLRenderer::Resource::TypeGeometryShader:
				case PLRenderer::Resource::TypeFragmentShader:
				case PLRenderer::Resource::TypeProgram:
				case PLRenderer::Resource::TypeFont:
					// Error! Invalid type!
					break;
			}

			// Backup internal sampler states
			if (pnSamplerState) {
				// Loop through all sampler states
				for (uint32 i=0; i<PLRenderer::Sampler::Number; i++, pnSamplerState++)
					*pnSamplerState = m_ppnInternalSamplerState[nStage][i];
			}
		}

		// Activate the correct OpenGL texture stage
		if (glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB+nStage);

		// Should a texture buffer be set?
		if (pTextureBuffer) {
			// Bind no texture buffer?
			// [TODO] Optimization is still buggy... :(
			if (1 == 0) {
	//		if (pTextureBuffer == m_ppPrevTextureBuffer[nStage]) {
				// Just reactivate the texture buffer :)
				switch (pTextureBuffer->GetType()) {
					case PLRenderer::Resource::TypeTextureBuffer1D:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_1D;
						glEnable(GL_TEXTURE_1D);
						break;

					case PLRenderer::Resource::TypeTextureBuffer2D:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_2D;
						glEnable(GL_TEXTURE_2D);
						break;

					case PLRenderer::Resource::TypeTextureBuffer2DArray:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_2D_ARRAY_EXT;
						glEnable(GL_TEXTURE_2D_ARRAY_EXT);
						break;

					case PLRenderer::Resource::TypeTextureBufferRectangle:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_RECTANGLE_ARB;
						glEnable(GL_TEXTURE_RECTANGLE_ARB);
						break;

					case PLRenderer::Resource::TypeTextureBuffer3D:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_3D_EXT;
						glEnable(GL_TEXTURE_3D_EXT);
						break;

					case PLRenderer::Resource::TypeTextureBufferCube:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_CUBE_MAP_ARB;
						glEnable(GL_TEXTURE_CUBE_MAP_ARB);
						break;

					case PLRenderer::Resource::TypeIndexBuffer:
					case PLRenderer::Resource::TypeVertexBuffer:
					case PLRenderer::Resource::TypeUniformBuffer:
					case PLRenderer::Resource::TypeOcclusionQuery:
					case PLRenderer::Resource::TypeVertexShader:
					case PLRenderer::Resource::TypeTessellationControlShader:
					case PLRenderer::Resource::TypeTessellationEvaluationShader:
					case PLRenderer::Resource::TypeGeometryShader:
					case PLRenderer::Resource::TypeFragmentShader:
					case PLRenderer::Resource::TypeProgram:
					case PLRenderer::Resource::TypeFont:
						// Error! Invalid type!
						break;
				}
			} else {
				// Yes, make it current
				m_sStatistics.nTextureBufferBinds++;
				if (!MakeTextureBufferCurrent(*pTextureBuffer, nStage)) {
					// [TODO] What now?
	//				m_ppCurrentTextureBuffer[nStage] = pPreviousTextureBuffer;

					// Error!
	//				return false;
				}

				// Set correct texture buffer type at this stage
				uint32 *pnSamplerState = nullptr;
				switch (pTextureBuffer->GetType()) {
					case PLRenderer::Resource::TypeTextureBuffer1D:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_1D;
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_TEXTURE_2D_ARRAY_EXT);
						glDisable(GL_TEXTURE_RECTANGLE_ARB);
						glDisable(GL_TEXTURE_3D_EXT);
						glDisable(GL_TEXTURE_CUBE_MAP_ARB);
						pnSamplerState = &static_cast<TextureBuffer1D*>(pTextureBuffer)->m_nSamplerState[0];
						break;

					case PLRenderer::Resource::TypeTextureBuffer2D:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_2D;
						glDisable(GL_TEXTURE_1D);
						glDisable(GL_TEXTURE_2D_ARRAY_EXT);
						glDisable(GL_TEXTURE_RECTANGLE_ARB);
						glDisable(GL_TEXTURE_3D_EXT);
						glDisable(GL_TEXTURE_CUBE_MAP_ARB);
						pnSamplerState = &static_cast<TextureBuffer2D*>(pTextureBuffer)->m_nSamplerState[0];
						break;

					case PLRenderer::Resource::TypeTextureBuffer2DArray:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_2D_ARRAY_EXT;
						glDisable(GL_TEXTURE_1D);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_TEXTURE_RECTANGLE_ARB);
						glDisable(GL_TEXTURE_3D_EXT);
						glDisable(GL_TEXTURE_CUBE_MAP_ARB);
						pnSamplerState = &static_cast<TextureBuffer2DArray*>(pTextureBuffer)->m_nSamplerState[0];
						break;

					case PLRenderer::Resource::TypeTextureBufferRectangle:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_RECTANGLE_ARB;
						glDisable(GL_TEXTURE_1D);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_TEXTURE_2D_ARRAY_EXT);
						glDisable(GL_TEXTURE_3D_EXT);
						glDisable(GL_TEXTURE_CUBE_MAP_ARB);
						pnSamplerState = &static_cast<TextureBufferRectangle*>(pTextureBuffer)->m_nSamplerState[0];
						break;

					case PLRenderer::Resource::TypeTextureBuffer3D:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_3D_EXT;
						glDisable(GL_TEXTURE_1D);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_TEXTURE_2D_ARRAY_EXT);
						glDisable(GL_TEXTURE_RECTANGLE_ARB);
						glDisable(GL_TEXTURE_CUBE_MAP_ARB);
						pnSamplerState = &static_cast<TextureBuffer3D*>(pTextureBuffer)->m_nSamplerState[0];
						break;

					case PLRenderer::Resource::TypeTextureBufferCube:
						m_nTextureBufferTypes[nStage] = GL_TEXTURE_CUBE_MAP_ARB;
						glDisable(GL_TEXTURE_1D);
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_TEXTURE_2D_ARRAY_EXT);
						glDisable(GL_TEXTURE_RECTANGLE_ARB);
						glDisable(GL_TEXTURE_3D_EXT);
						pnSamplerState = &static_cast<TextureBufferCube*>(pTextureBuffer)->m_nSamplerState[0];
						break;

					case PLRenderer::Resource::TypeIndexBuffer:
					case PLRenderer::Resource::TypeVertexBuffer:
					case PLRenderer::Resource::TypeUniformBuffer:
					case PLRenderer::Resource::TypeOcclusionQuery:
					case PLRenderer::Resource::TypeVertexShader:
					case PLRenderer::Resource::TypeTessellationControlShader:
					case PLRenderer::Resource::TypeTessellationEvaluationShader:
					case PLRenderer::Resource::TypeGeometryShader:
					case PLRenderer::Resource::TypeFragmentShader:
					case PLRenderer::Resource::TypeProgram:
					case PLRenderer::Resource::TypeFont:
						// Error! Invalid type!
						break;
				}

				// Synchronize internal sampler states
				if (pnSamplerState) {
					// Loop through all sampler states
					for (uint32 i=0; i<PLRenderer::Sampler::Number; i++, pnSamplerState++) {
						// Do we need to update this state?
						if (*pnSamplerState != m_ppnInternalSamplerState[nStage][i]) {
							// Make this state 'dirty'
							m_ppnInternalSamplerState[nStage][i]++;

							// Reset the state
							SetSamplerState(nStage, static_cast<PLRenderer::Sampler::Enum>(i), m_ppnSamplerState[nStage][i]);
						}
					}
				}

				// Fixed functions
				if (m_pFixedFunctions) {
					// Loop through all texture stage states
					for (uint32 i=0; i<FixedFunctions::TextureStage::Number; i++) {
						// Make this state 'dirty'
						m_pFixedFunctions->m_ppnInternalTextureStageState[nStage][i]++;

						// Reset the state
						m_pFixedFunctions->SetTextureStageState(nStage, static_cast<FixedFunctions::TextureStage::Enum>(i), m_pFixedFunctions->m_ppnTextureStageState[nStage][i]);
					}
				}
			}
		} else {
			// No, deactivate texture buffer
			glDisable(m_nTextureBufferTypes[nStage]);
			m_nTextureBufferTypes[nStage] = 0; // Dummy value
		}
		if (pPreviousTextureBuffer)
			m_ppPrevTextureBuffer[nStage] = pPreviousTextureBuffer;

		// Fixed functions
		if (m_pFixedFunctions) {
			// Scale texture matrix a rectangle texture buffer is used - we always want to work with normalized
			// texture coordinates :)
			if (m_ppCurrentTextureBuffer[nStage] && m_ppCurrentTextureBuffer[nStage]->GetType() == PLRenderer::Resource::TypeTextureBufferRectangle) {
				m_pFixedFunctions->UpdateCurrentOpenGLTextureMatrix(nStage);
			} else {
				// If the previous texture buffer was a rectangle one, now reset the texture matrix
				if ((m_ppPrevTextureBuffer[nStage] && m_ppPrevTextureBuffer[nStage]->GetType() == PLRenderer::Resource::TypeTextureBufferRectangle) ||
					(m_ppPrevTextureBuffer[nStage] && !m_ppCurrentTextureBuffer[nStage])) {
					m_pFixedFunctions->UpdateCurrentOpenGLTextureMatrix(nStage);
				}
			}
		}
	}

	// Done
	return true;
}

bool Renderer::SetShaderProgramTextureBuffer(int nStage, PLRenderer::TextureBuffer *pTextureBuffer)
{
	// Set all available texture stages to this setting?
	if (nStage < 0) {
		for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++)
			SetTextureBuffer(i, pTextureBuffer);
	} else {
		// Check if the stage is correct
		if (nStage >= static_cast<int>(m_sCapabilities.nMaxTextureUnits))
			return false; // Error!

		// Is this texture buffer already set?
		if (m_ppCurrentTextureBuffer[nStage] == pTextureBuffer)
			return false; // Error!

		// Make this texture buffer to the renderers current one
		PLRenderer::TextureBuffer *pPreviousTextureBuffer = m_ppCurrentTextureBuffer[nStage];
		m_ppCurrentTextureBuffer[nStage] = pTextureBuffer;

		// The previous texture buffer needs a backup of the current sampler states
		if (pPreviousTextureBuffer) {
			uint32 *pnSamplerState = nullptr;
			switch (pPreviousTextureBuffer->GetType()) {
				case PLRenderer::Resource::TypeTextureBuffer1D:
					pnSamplerState = &static_cast<TextureBuffer1D*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer2D:
					pnSamplerState = &static_cast<TextureBuffer2D*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer2DArray:
					pnSamplerState = &static_cast<TextureBuffer2DArray*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBufferRectangle:
					pnSamplerState = &static_cast<TextureBufferRectangle*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer3D:
					pnSamplerState = &static_cast<TextureBuffer3D*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBufferCube:
					pnSamplerState = &static_cast<TextureBufferCube*>(pPreviousTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeIndexBuffer:
				case PLRenderer::Resource::TypeVertexBuffer:
				case PLRenderer::Resource::TypeUniformBuffer:
				case PLRenderer::Resource::TypeOcclusionQuery:
				case PLRenderer::Resource::TypeVertexShader:
				case PLRenderer::Resource::TypeTessellationControlShader:
				case PLRenderer::Resource::TypeTessellationEvaluationShader:
				case PLRenderer::Resource::TypeGeometryShader:
				case PLRenderer::Resource::TypeFragmentShader:
				case PLRenderer::Resource::TypeProgram:
				case PLRenderer::Resource::TypeFont:
					// Error! Invalid type!
					break;
			}

			// Backup internal sampler states
			if (pnSamplerState) {
				// Loop through all sampler states
				for (uint32 i=0; i<PLRenderer::Sampler::Number; i++, pnSamplerState++)
					*pnSamplerState = m_ppnInternalSamplerState[nStage][i];
			}
		}

		// Activate the correct OpenGL texture stage
		if (glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB+nStage);

		// Should a texture buffer be set?
		if (pTextureBuffer) {
			// Yes, make it current
			m_sStatistics.nTextureBufferBinds++;

			// Set correct texture buffer type at this stage
			uint32 *pnSamplerState = nullptr;
			switch (pTextureBuffer->GetType()) {
				case PLRenderer::Resource::TypeTextureBuffer1D:
					m_nTextureBufferTypes[nStage] = GL_TEXTURE_1D;
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_TEXTURE_2D_ARRAY_EXT);
					glDisable(GL_TEXTURE_RECTANGLE_ARB);
					glDisable(GL_TEXTURE_3D_EXT);
					glDisable(GL_TEXTURE_CUBE_MAP_ARB);
					pnSamplerState = &static_cast<TextureBuffer1D*>(pTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer2D:
					m_nTextureBufferTypes[nStage] = GL_TEXTURE_2D;
					glDisable(GL_TEXTURE_1D);
					glDisable(GL_TEXTURE_2D_ARRAY_EXT);
					glDisable(GL_TEXTURE_RECTANGLE_ARB);
					glDisable(GL_TEXTURE_3D_EXT);
					glDisable(GL_TEXTURE_CUBE_MAP_ARB);
					pnSamplerState = &static_cast<TextureBuffer2D*>(pTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer2DArray:
					m_nTextureBufferTypes[nStage] = GL_TEXTURE_2D_ARRAY_EXT;
					glDisable(GL_TEXTURE_1D);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_TEXTURE_RECTANGLE_ARB);
					glDisable(GL_TEXTURE_3D_EXT);
					glDisable(GL_TEXTURE_CUBE_MAP_ARB);
					pnSamplerState = &static_cast<TextureBuffer2DArray*>(pTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBufferRectangle:
					m_nTextureBufferTypes[nStage] = GL_TEXTURE_RECTANGLE_ARB;
					glDisable(GL_TEXTURE_1D);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_TEXTURE_2D_ARRAY_EXT);
					glDisable(GL_TEXTURE_3D_EXT);
					glDisable(GL_TEXTURE_CUBE_MAP_ARB);
					pnSamplerState = &static_cast<TextureBufferRectangle*>(pTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBuffer3D:
					m_nTextureBufferTypes[nStage] = GL_TEXTURE_3D_EXT;
					glDisable(GL_TEXTURE_1D);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_TEXTURE_2D_ARRAY_EXT);
					glDisable(GL_TEXTURE_RECTANGLE_ARB);
					glDisable(GL_TEXTURE_CUBE_MAP_ARB);
					pnSamplerState = &static_cast<TextureBuffer3D*>(pTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeTextureBufferCube:
					m_nTextureBufferTypes[nStage] = GL_TEXTURE_CUBE_MAP_ARB;
					glDisable(GL_TEXTURE_1D);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_TEXTURE_2D_ARRAY_EXT);
					glDisable(GL_TEXTURE_RECTANGLE_ARB);
					glDisable(GL_TEXTURE_3D_EXT);
					pnSamplerState = &static_cast<TextureBufferCube*>(pTextureBuffer)->m_nSamplerState[0];
					break;

				case PLRenderer::Resource::TypeIndexBuffer:
				case PLRenderer::Resource::TypeVertexBuffer:
				case PLRenderer::Resource::TypeUniformBuffer:
				case PLRenderer::Resource::TypeOcclusionQuery:
				case PLRenderer::Resource::TypeVertexShader:
				case PLRenderer::Resource::TypeTessellationControlShader:
				case PLRenderer::Resource::TypeTessellationEvaluationShader:
				case PLRenderer::Resource::TypeGeometryShader:
				case PLRenderer::Resource::TypeFragmentShader:
				case PLRenderer::Resource::TypeProgram:
				case PLRenderer::Resource::TypeFont:
					// Error! Invalid type!
					break;
			}

			// Synchronize internal sampler states
			if (pnSamplerState) {
				// Loop through all sampler states
				for (uint32 i=0; i<PLRenderer::Sampler::Number; i++, pnSamplerState++) {
					// Do we need to update this state?
					if (*pnSamplerState != m_ppnInternalSamplerState[nStage][i]) {
						// Make this state 'dirty'
						m_ppnInternalSamplerState[nStage][i]++;

						// Reset the state
						SetSamplerState(nStage, static_cast<PLRenderer::Sampler::Enum>(i), m_ppnSamplerState[nStage][i]);
					}
				}
			}

			// Fixed functions
			if (m_pFixedFunctions) {
				// Loop through all texture stage states
				for (uint32 i=0; i<FixedFunctions::TextureStage::Number; i++) {
					// Make this state 'dirty'
					m_pFixedFunctions->m_ppnInternalTextureStageState[nStage][i]++;

					// Reset the state
					m_pFixedFunctions->SetTextureStageState(nStage, static_cast<FixedFunctions::TextureStage::Enum>(i), m_pFixedFunctions->m_ppnTextureStageState[nStage][i]);
				}
			}
		} else {
			// No, deactivate texture buffer
			glDisable(m_nTextureBufferTypes[nStage]);
			m_nTextureBufferTypes[nStage] = 0; // Dummy value
		}
		if (pPreviousTextureBuffer)
			m_ppPrevTextureBuffer[nStage] = pPreviousTextureBuffer;

		// Fixed functions
		if (m_pFixedFunctions)
			m_pFixedFunctions->UpdateShaderOpenGLTextureMatrix(nStage);
	}

	// Done
	return true;
}

bool Renderer::SetIndexBuffer(PLRenderer::IndexBuffer *pIndexBuffer)
{
	// Is this index buffer already set?
	if (m_pCurrentIndexBuffer == pIndexBuffer)
		return false; // Error!

	// Make this index buffer to the renderers current one
	m_pCurrentIndexBuffer = pIndexBuffer;

	// Should an index buffer be set?
	if (pIndexBuffer) {
		// Yes, make it current
		if (!static_cast<IndexBuffer*>(pIndexBuffer)->MakeCurrent()) {
			// Now, no index buffer is set...
			m_pCurrentIndexBuffer = nullptr;
			if (m_pContext->GetExtensions().IsGL_ARB_vertex_buffer_object())
				glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

			// Error!
			return false;
		}
	} else {
		// No, deactivate index buffer
		if (m_pContext->GetExtensions().IsGL_ARB_vertex_buffer_object())
			glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}

	// Done
	return true;
}

bool Renderer::SetProgram(PLRenderer::Program *pProgram)
{
	// Is the new program the same one as the current one?
	PLRenderer::Program *pCurrentProgram = static_cast<PLRenderer::Program*>(m_cProgramHandler.GetResource());
	if (pCurrentProgram != pProgram) {
		// Was there a previous program?
		if (pCurrentProgram) {
			static_cast<Program*>(pCurrentProgram)->UnmakeCurrent();

			// [HACK] When using GLSL as Cg profile we need to use 'glUseProgramObjectARB()' to deactivate shaders
			glUseProgramObjectARB(0);
		}

		// Update the program resource handler
		m_cProgramHandler.SetResource(pProgram);

		// Make the new program to the current one
		if (pProgram)
			return static_cast<Program*>(pProgram)->MakeCurrent();
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Draw call                                             ]
//[-------------------------------------------------------]
bool Renderer::DrawPrimitives(PLRenderer::Primitive::Enum nType, uint32 nStartIndex, uint32 nNumVertices)
{
	// Draw something?
	if (!nNumVertices)
		return true; // Done

	// Get number of primitives
	uint32 nPrimitiveCount;
	switch (nType) {
		case PLRenderer::Primitive::PointList:
			nPrimitiveCount = nNumVertices;
			break;

		case PLRenderer::Primitive::LineList:
			nPrimitiveCount = nNumVertices-1;
			break;

		case PLRenderer::Primitive::LineStrip:
			nPrimitiveCount = nNumVertices-1;
			break;

		case PLRenderer::Primitive::TriangleList:
			nPrimitiveCount = nNumVertices/3;
			break;

		case PLRenderer::Primitive::TriangleStrip:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::TriangleFan:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::Number:
		case PLRenderer::Primitive::Unknown:
		default:
			return false; // Error!
	}

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices;
	m_sStatistics.nTriangles += nPrimitiveCount;


	// [FIXME] If this isn't done HERE the texturing for the point sprites will not work!??
	// Specify point sprite texture coordinate replacement mode for each texture unit
	if (nType == PLRenderer::Primitive::PointList && GetRenderState(PLRenderer::RenderState::PointSpriteEnable)) {
		// Point sprite supported?
		if (m_sCapabilities.bPointSprite)
			glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	}

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && m_pContext->GetExtensions().IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nStartIndex, nNumVertices);

		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// Draw primitive
			glDrawArrays(nAPIValue, nStartIndex, nNumVertices);
		} else {
			// Error, invalid value!
			return false;
		}

		glUnlockArraysEXT();
	} else {
		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// Draw primitive
			glDrawArrays(nAPIValue, nStartIndex, nNumVertices);
		} else {
			// Error, invalid value!
			return false;
		}
	}

	// Done
	return true;
}

bool Renderer::DrawIndexedPrimitives(PLRenderer::Primitive::Enum nType, uint32 nMinIndex,
									 uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices)
{
	// Index and vertex buffer correct?
	if (!m_pCurrentIndexBuffer)
		return false; // Error!

	// Draw something?
	if (!nNumVertices)
		return true; // Done

	// Check parameters
	if (nStartIndex+nNumVertices > m_pCurrentIndexBuffer->GetNumOfElements() || nMinIndex > nMaxIndex)
		return false; // Definitely NOT good...

	// Define an offset helper macro just used inside this function
	#define BUFFER_OFFSET(i) (static_cast<char*>(static_cast<IndexBuffer*>(m_pCurrentIndexBuffer)->GetDynamicData())+i)

	// Get API dependent type
	uint32 nTypeSize;
	uint32 nTypeAPI = m_pCurrentIndexBuffer->GetElementType();
	if (nTypeAPI == PLRenderer::IndexBuffer::UInt) {
		nTypeSize = sizeof(uint32);
		nTypeAPI  = GL_UNSIGNED_INT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UShort) {
		nTypeSize = sizeof(uint16);
		nTypeAPI = GL_UNSIGNED_SHORT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UByte) {
		nTypeSize = sizeof(uint8);
		nTypeAPI = GL_UNSIGNED_BYTE;
	} else {
		// Error!
		return false;
	}

	// Get number of primitives
	uint32 nPrimitiveCount;
	switch (nType) {
		case PLRenderer::Primitive::PointList:
			nPrimitiveCount = nNumVertices;
			break;

		case PLRenderer::Primitive::LineList:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::LineStrip:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::TriangleList:
			nPrimitiveCount = nNumVertices/3;
			break;

		case PLRenderer::Primitive::TriangleStrip:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::TriangleFan:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::Number:
		case PLRenderer::Primitive::Unknown:
		default:
			return false; // Error!
	}

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices;
	m_sStatistics.nTriangles += nPrimitiveCount;

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	const Extensions &cExtensions = m_pContext->GetExtensions();
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && cExtensions.IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nMinIndex, nMaxIndex-nMinIndex+1);

		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// On some GPUs we don't have GL_EXT_draw_range_elements or it is only very limited.
			// On my old GeForce4 Ti 4200: max elements vertices = 4096 and max elements indices = 4096
			// ... on my Radion 9600 Mobile the extension can handle much more vertices (2147483647) and indices (65535)
			// ... we check for this to avoid problems...
			if (cExtensions.IsGL_EXT_draw_range_elements() && nNumVertices < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_INDICES_EXT()) &&
				nMaxIndex < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_VERTICES_EXT())) {
				// Draw primitive
				glDrawRangeElementsEXT(nAPIValue, nMinIndex, nMaxIndex, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
			} else {
				// Draw primitive
				glDrawElements(nAPIValue, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
			}
		} else {
			// Error, invalid value!
			return false;
		}

		glUnlockArraysEXT();
	} else {
		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// On some GPUs we don't have GL_EXT_draw_range_elements or it is only very limited.
			// On my old GeForce4 Ti 4200: max elements vertices = 4096 and max elements indices = 4096
			// ... on my Radion 9600 Mobile the extension can handle much more vertices (2147483647) and indices (65535)
			// ... we check for this to avoid problems...
			if (cExtensions.IsGL_EXT_draw_range_elements() && nNumVertices < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_INDICES_EXT()) &&
				nMaxIndex < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_VERTICES_EXT())) {
				// Draw primitive
				glDrawRangeElementsEXT(nAPIValue, nMinIndex, nMaxIndex, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
			} else {
				// Draw primitive
				glDrawElements(nAPIValue, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
			}
		} else {
			// Error, invalid value!
			return false;
		}
	}

	// Undefine your offset helper macro because its just used inside this function
	#undef BUFFER_OFFSET

	// Done
	return true;
}

bool Renderer::DrawPatches(uint32 nVerticesPerPatch, uint32 nStartIndex, uint32 nNumVertices)
{
	// Required extension available?
	const Extensions &cExtensions = m_pContext->GetExtensions();
	if (!cExtensions.IsGL_ARB_tessellation_shader())
		return false; // Error!

	// Draw something?
	if (!nNumVertices)
		return true; // Done

	// Get number of primitives
	// [TODO] Calculate number of generated triangles?
	const uint32 nPrimitiveCount = nNumVertices/3;

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices;
	m_sStatistics.nTriangles += nPrimitiveCount;

	// Set number of vertices that will be used to make up a single patch primitive
	glPatchParameteri(GL_PATCH_VERTICES, nVerticesPerPatch);

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && m_pContext->GetExtensions().IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nStartIndex, nNumVertices);

		// Draw primitive
		glDrawArrays(GL_PATCHES, nStartIndex, nNumVertices);

		glUnlockArraysEXT();
	} else {
		// Draw primitive
		glDrawArrays(GL_PATCHES, nStartIndex, nNumVertices);
	}

	// Done
	return true;
}

bool Renderer::DrawIndexedPatches(uint32 nVerticesPerPatch, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices)
{
	// Index and vertex buffer correct and required extension available?
	const Extensions &cExtensions = m_pContext->GetExtensions();
	if (!m_pCurrentIndexBuffer || !cExtensions.IsGL_ARB_tessellation_shader())
		return false; // Error!

	// Draw something?
	if (!nNumVertices)
		return true; // Done

	// Check parameters
	if (nStartIndex+nNumVertices > m_pCurrentIndexBuffer->GetNumOfElements() || nMinIndex > nMaxIndex)
		return false; // Definitely NOT good...

	// Define an offset helper macro just used inside this function
	#define BUFFER_OFFSET(i) (static_cast<char*>(static_cast<IndexBuffer*>(m_pCurrentIndexBuffer)->GetDynamicData())+i)

	// Get API dependent type
	uint32 nTypeSize;
	uint32 nTypeAPI = m_pCurrentIndexBuffer->GetElementType();
	if (nTypeAPI == PLRenderer::IndexBuffer::UInt) {
		nTypeSize = sizeof(uint32);
		nTypeAPI  = GL_UNSIGNED_INT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UShort) {
		nTypeSize = sizeof(uint16);
		nTypeAPI = GL_UNSIGNED_SHORT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UByte) {
		nTypeSize = sizeof(uint8);
		nTypeAPI = GL_UNSIGNED_BYTE;
	} else {
		// Error!
		return false;
	}

	// Get number of primitives
	// [TODO] Calculate number of generated triangles?
	const uint32 nPrimitiveCount = nNumVertices/3;

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices;
	m_sStatistics.nTriangles += nPrimitiveCount;

	// Set number of vertices that will be used to make up a single patch primitive
	glPatchParameteri(GL_PATCH_VERTICES, nVerticesPerPatch);

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && cExtensions.IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nMinIndex, nMaxIndex-nMinIndex+1);

		// On some GPUs we don't have GL_EXT_draw_range_elements or it is only very limited.
		// On my old GeForce4 Ti 4200: max elements vertices = 4096 and max elements indices = 4096
		// ... on my Radion 9600 Mobile the extension can handle much more vertices (2147483647) and indices (65535)
		// ... we check for this to avoid problems...
		if (cExtensions.IsGL_EXT_draw_range_elements() && nNumVertices < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_INDICES_EXT()) &&
			nMaxIndex < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_VERTICES_EXT())) {
			// Draw primitive
			glDrawRangeElementsEXT(GL_PATCHES, nMinIndex, nMaxIndex, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
		} else {
			// Draw primitive
			glDrawElements(GL_PATCHES, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
		}

		glUnlockArraysEXT();
	} else {
		// On some GPUs we don't have GL_EXT_draw_range_elements or it is only very limited.
		// On my old GeForce4 Ti 4200: max elements vertices = 4096 and max elements indices = 4096
		// ... on my Radion 9600 Mobile the extension can handle much more vertices (2147483647) and indices (65535)
		// ... we check for this to avoid problems...
		if (cExtensions.IsGL_EXT_draw_range_elements() && nNumVertices < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_INDICES_EXT()) &&
			nMaxIndex < static_cast<uint32>(cExtensions.GetGL_MAX_ELEMENTS_VERTICES_EXT())) {
			// Draw primitive
			glDrawRangeElementsEXT(GL_PATCHES, nMinIndex, nMaxIndex, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
		} else {
			// Draw primitive
			glDrawElements(GL_PATCHES, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize));
		}
	}

	// Undefine your offset helper macro because its just used inside this function
	#undef BUFFER_OFFSET

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Draw call with multiple primitive instances           ]
//[-------------------------------------------------------]
bool Renderer::DrawPrimitivesInstanced(PLRenderer::Primitive::Enum nType, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// Required extension available?
	const Extensions &cExtensions = m_pContext->GetExtensions();
	if (!cExtensions.IsGL_ARB_draw_instanced())
		return false; // Error!

	// Draw something?
	if (!nNumVertices || !nNumOfInstances)
		return true; // Done

	// Get number of primitives
	uint32 nPrimitiveCount;
	switch (nType) {
		case PLRenderer::Primitive::PointList:
			nPrimitiveCount = nNumVertices;
			break;

		case PLRenderer::Primitive::LineList:
			nPrimitiveCount = nNumVertices-1;
			break;

		case PLRenderer::Primitive::LineStrip:
			nPrimitiveCount = nNumVertices-1;
			break;

		case PLRenderer::Primitive::TriangleList:
			nPrimitiveCount = nNumVertices/3;
			break;

		case PLRenderer::Primitive::TriangleStrip:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::TriangleFan:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::Number:
		case PLRenderer::Primitive::Unknown:
		default:
			return false; // Error!
	}

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices*nNumOfInstances;
	m_sStatistics.nTriangles += nPrimitiveCount*nNumOfInstances;


	// [FIXME] If this isn't done HERE the texturing for the point sprites will not work!??
	// Specify point sprite texture coordinate replacement mode for each texture unit
	if (nType == PLRenderer::Primitive::PointList && GetRenderState(PLRenderer::RenderState::PointSpriteEnable)) {
		// Point sprite supported?
		if (m_sCapabilities.bPointSprite)
			glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
	}

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && m_pContext->GetExtensions().IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nStartIndex, nNumVertices);

		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// Draw primitive
			glDrawArraysInstancedARB(nAPIValue, nStartIndex, nNumVertices, nNumOfInstances);
		} else {
			// Error, invalid value!
			return false;
		}

		glUnlockArraysEXT();
	} else {
		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// Draw primitive
			glDrawArraysInstancedARB(nAPIValue, nStartIndex, nNumVertices, nNumOfInstances);
		} else {
			// Error, invalid value!
			return false;
		}
	}

	// Done
	return true;
}

bool Renderer::DrawIndexedPrimitivesInstanced(PLRenderer::Primitive::Enum nType, uint32 nMinIndex,
											  uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// Index and vertex buffer correct? Is the required extension available?
	const Extensions &cExtensions = m_pContext->GetExtensions();
	if (!m_pCurrentIndexBuffer || !cExtensions.IsGL_ARB_draw_instanced())
		return false; // Error!

	// Draw something?
	if (!nNumVertices || !nNumOfInstances)
		return true; // Done

	// Check parameters
	if (nStartIndex+nNumVertices > m_pCurrentIndexBuffer->GetNumOfElements() || nMinIndex > nMaxIndex)
		return false; // Definitely NOT good...

	// Define an offset helper macro just used inside this function
	#define BUFFER_OFFSET(i) (static_cast<char*>(static_cast<IndexBuffer*>(m_pCurrentIndexBuffer)->GetDynamicData())+i)

	// Get API dependent type
	uint32 nTypeSize;
	uint32 nTypeAPI = m_pCurrentIndexBuffer->GetElementType();
	if (nTypeAPI == PLRenderer::IndexBuffer::UInt) {
		nTypeSize = sizeof(uint32);
		nTypeAPI  = GL_UNSIGNED_INT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UShort) {
		nTypeSize = sizeof(uint16);
		nTypeAPI = GL_UNSIGNED_SHORT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UByte) {
		nTypeSize = sizeof(uint8);
		nTypeAPI = GL_UNSIGNED_BYTE;
	} else {
		// Error!
		return false;
	}

	// Get number of primitives
	uint32 nPrimitiveCount;
	switch (nType) {
		case PLRenderer::Primitive::PointList:
			nPrimitiveCount = nNumVertices;
			break;

		case PLRenderer::Primitive::LineList:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::LineStrip:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::TriangleList:
			nPrimitiveCount = nNumVertices/3;
			break;

		case PLRenderer::Primitive::TriangleStrip:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::TriangleFan:
			nPrimitiveCount = nNumVertices-2;
			break;

		case PLRenderer::Primitive::Number:
		case PLRenderer::Primitive::Unknown:
		default:
			return false; // Error!
	}

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices*nNumOfInstances;
	m_sStatistics.nTriangles += nPrimitiveCount*nNumOfInstances;

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && cExtensions.IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nMinIndex, nMaxIndex-nMinIndex+1);

		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// GL_EXT_draw_range_elements can't be used together with GL_ARB_draw_instanced

			// Draw instanced primitive
			glDrawElementsInstancedARB(nAPIValue, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize), nNumOfInstances);
		} else {
			// Error, invalid value!
			return false;
		}

		glUnlockArraysEXT();
	} else {
		// Get API primitive type
		const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
		if (&nAPIValue != &Array<uint32>::Null) {
			// GL_EXT_draw_range_elements can't be used together with GL_ARB_draw_instanced

			// Draw instanced primitive
			glDrawElementsInstancedARB(nAPIValue, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize), nNumOfInstances);
		} else {
			// Error, invalid value!
			return false;
		}
	}

	// Undefine your offset helper macro because its just used inside this function
	#undef BUFFER_OFFSET

	// Done
	return true;
}

bool Renderer::DrawPatchesInstanced(uint32 nVerticesPerPatch, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// Required extensions available?
	const Extensions &cExtensions = m_pContext->GetExtensions();
	if (!cExtensions.IsGL_ARB_tessellation_shader() || !cExtensions.IsGL_ARB_draw_instanced())
		return false; // Error!

	// Draw something?
	if (!nNumVertices || !nNumOfInstances)
		return true; // Done

	// Get number of primitives
	// [TODO] Calculate number of generated triangles?
	const uint32 nPrimitiveCount = nNumVertices/3;

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices*nNumOfInstances;
	m_sStatistics.nTriangles += nPrimitiveCount*nNumOfInstances;

	// Set number of vertices that will be used to make up a single patch primitive
	glPatchParameteri(GL_PATCH_VERTICES, nVerticesPerPatch);

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && m_pContext->GetExtensions().IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nStartIndex, nNumVertices);

		// Draw instanced primitive
		glDrawArraysInstancedARB(GL_PATCHES, nStartIndex, nNumVertices, nNumOfInstances);

		glUnlockArraysEXT();
	} else {
		// Draw instanced primitive
		glDrawArraysInstancedARB(GL_PATCHES, nStartIndex, nNumVertices, nNumOfInstances);
	}

	// Done
	return true;
}

bool Renderer::DrawIndexedPatchesInstanced(uint32 nVerticesPerPatch, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// Index and vertex buffer correct and required extensions available?
	const Extensions &cExtensions = m_pContext->GetExtensions();
	if (!m_pCurrentIndexBuffer || !cExtensions.IsGL_ARB_tessellation_shader() || !cExtensions.IsGL_ARB_draw_instanced())
		return false; // Error!

	// Draw something?
	if (!nNumVertices || !nNumOfInstances)
		return true; // Done

	// Check parameters
	if (nStartIndex+nNumVertices > m_pCurrentIndexBuffer->GetNumOfElements() || nMinIndex > nMaxIndex)
		return false; // Definitely NOT good...

	// Define an offset helper macro just used inside this function
	#define BUFFER_OFFSET(i) (static_cast<char*>(static_cast<IndexBuffer*>(m_pCurrentIndexBuffer)->GetDynamicData())+i)

	// Get API dependent type
	uint32 nTypeSize;
	uint32 nTypeAPI = m_pCurrentIndexBuffer->GetElementType();
	if (nTypeAPI == PLRenderer::IndexBuffer::UInt) {
		nTypeSize = sizeof(uint32);
		nTypeAPI  = GL_UNSIGNED_INT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UShort) {
		nTypeSize = sizeof(uint16);
		nTypeAPI = GL_UNSIGNED_SHORT;
	} else if (nTypeAPI == PLRenderer::IndexBuffer::UByte) {
		nTypeSize = sizeof(uint8);
		nTypeAPI = GL_UNSIGNED_BYTE;
	} else {
		// Error!
		return false;
	}

	// Get number of primitives
	// [TODO] Calculate number of generated triangles?
	const uint32 nPrimitiveCount = nNumVertices/3;

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices*nNumOfInstances;
	m_sStatistics.nTriangles += nPrimitiveCount*nNumOfInstances;

	// Set number of vertices that will be used to make up a single patch primitive
	glPatchParameteri(GL_PATCH_VERTICES, nVerticesPerPatch);

	// If the vertex buffer is in software mode, try to use compiled vertex array (CVA) for better performance
	if (m_pFixedFunctions && m_pFixedFunctions->m_ppCurrentVertexBuffer[0] && m_pFixedFunctions->m_ppCurrentVertexBuffer[0]->GetUsage() == PLRenderer::Usage::Software && cExtensions.IsGL_EXT_compiled_vertex_array()) {
		glLockArraysEXT(nMinIndex, nMaxIndex-nMinIndex+1);

		// GL_EXT_draw_range_elements can't be used together with GL_ARB_draw_instanced

		// Draw instanced primitive
		glDrawElementsInstancedARB(GL_PATCHES, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize), nNumOfInstances);

		glUnlockArraysEXT();
	} else {
		// GL_EXT_draw_range_elements can't be used together with GL_ARB_draw_instanced

		// Draw instanced primitive
		glDrawElementsInstancedARB(GL_PATCHES, nNumVertices, nTypeAPI, BUFFER_OFFSET(nStartIndex*nTypeSize), nNumOfInstances);
	}

	// Undefine your offset helper macro because its just used inside this function
	#undef BUFFER_OFFSET

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
