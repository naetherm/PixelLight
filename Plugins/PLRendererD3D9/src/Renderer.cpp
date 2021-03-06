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
#include <PLMath/Matrix3x4.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/Renderer/SurfaceWindowHandler.h>
#include <PLRenderer/Renderer/Backend/DrawHelpersBackend.h>
#include <PLRenderer/Renderer/Backend/FontManagerBackend.h>
#include "PLRendererD3D9/SurfaceWindow.h"
#include "PLRendererD3D9/SurfaceTextureBuffer.h"
#include "PLRendererD3D9/TextureBuffer1D.h"
#include "PLRendererD3D9/TextureBuffer2D.h"
#include "PLRendererD3D9/TextureBuffer3D.h"
#include "PLRendererD3D9/TextureBufferRectangle.h"
#include "PLRendererD3D9/TextureBufferCube.h"
#include "PLRendererD3D9/IndexBuffer.h"
#include "PLRendererD3D9/VertexBuffer.h"
#include "PLRendererD3D9/OcclusionQuery.h"
#include "PLRendererD3D9/FixedFunctions.h"
#include "PLRendererD3D9/Renderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererD3D9 {


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
LRESULT WINAPI MsgProc(HWND hWnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Renderer, "PLRendererD3D9", PLRenderer::RendererBackend, "Direct3D 9 renderer backend")
	// Constructors
	pl_constructor_6_metadata(DefaultConstructor,	PLCore::handle,	pl_enum_type(EMode),	PLCore::uint32,	PLCore::uint32,	PLCore::uint32,	PLCore::String,	"Constructor with renderer mode, Z buffer bits, stencil buffer bits, the number of multisample antialiasing samples per pixel and the default shader language as parameter",	"")
pl_class_metadata_end(Renderer)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Renderer::Renderer(handle nNativeWindowHandle, EMode nMode, uint32 nZBufferBits, uint32 nStencilBits, uint32 nMultisampleAntialiasingSamples, String sDefaultShaderLanguage) : PLRenderer::RendererBackend(nMode),
	m_pD3D(nullptr),
	m_pFixedFunctions(nullptr),
	m_pFontManager(new PLRenderer::FontManagerBackend(*this))
{
	// Ignore the given native window handle

	// This renderer implementation has just support for Cg as shader language, so ignore sDefaultShaderLanguage

	// Output log information
	PL_LOG(Info, "Initialize Direct3D 9 renderer")

	// Set Z buffer bits and stencil buffer bits capabilities
	m_sCapabilities.nZBufferBits = nZBufferBits;
	m_sCapabilities.nStencilBits = nStencilBits;

	// Init data
	m_pFirstSwapChainUserSurface = nullptr;

	// Create the D3D object and create a list of all available display modes
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D && QueryDisplayModes()) {
		// Create renderer device
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		// Use the current display mode
		D3DDISPLAYMODE mode;
		if (SUCCEEDED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode))) {
			// [TODO] Find correct settings
			/*
			d3dpp.Windowed               = true;
			d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat       = mode.Format;
			d3dpp.EnableAutoDepthStencil = true;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
		*/

			d3dpp.BackBufferFormat		 = mode.Format;
			d3dpp.BackBufferWidth		 = mode.Width;
			d3dpp.BackBufferHeight		 = mode.Height;

			/*
			d3dpp.EnableAutoDepthStencil = true;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		//	d3dpp.Flags                  = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
			d3dpp.Windowed               = true;
			d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat       = mode.Format;
		*/

			d3dpp.Windowed               = true;
			d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat       = mode.Format;
			d3dpp.EnableAutoDepthStencil = true;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
			d3dpp.Flags                  = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
			d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

		/*
			// Enable/disable VSync
			bool bVSync = true;
			if (bVSync) {
				// Enable VSync
				d3dpp.BackBufferCount      = 2;  // Triple-buffering for VSync
				d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
			} else {
				// Disable VSync
				d3dpp.BackBufferCount      = 1;
				d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			}
		*/


		/*
			d3dpp.Windowed               = true;
			d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat       = d3ddm.Format;
			d3dpp.EnableAutoDepthStencil = true;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

			g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp, &g_pd3dDevice );
		*/

			// Register the window class
			WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
				nullptr, nullptr, nullptr, nullptr, nullptr,
				TEXT("PLDirect3D9"), nullptr };
			RegisterClassEx(&wc);

			// Create the application's window
			m_hWnd = CreateWindowA("PLDirect3D9", "PLDirect3D9 renderer device",
				0, 0, mode.Width, mode.Height, 0,
				GetDesktopWindow(), nullptr, nullptr, nullptr);

			// Create renderer device
		//	HRESULT nError = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		//										  D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice);
			HRESULT nError = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
												  D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice);
		//										  D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice);
	//		HRESULT nError = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,

			if (SUCCEEDED(nError)) {
				// Init wrappers
				InitWrappers();

				// Setup the renderer capabilities
				SetupCapabilities();

				// Fixed functions allowed?
				if (GetMode() != ModeShaders)
					m_pFixedFunctions = new FixedFunctions(*this);

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
				MemoryManager::Set(m_ppCurrentTextureBuffer, 0, sizeof(PLRenderer::TextureBuffer**)*m_sCapabilities.nMaxTextureUnits);

				// Setup current render states
				for (uint32 i=0; i<PLRenderer::RenderState::Number; i++) {
					uint32 nState = m_nRenderState[i];
					m_nRenderState[i] = m_nRenderState[i]+1;
					SetRenderState(static_cast<PLRenderer::RenderState::Enum>(i), nState);
				}

				// Reset render
				Reset();
			} else {
				// Error!
				if (nError == D3DERR_INVALIDCALL)
					PL_LOG(Error, "Can't create Direct3D device! (D3DERR_INVALIDCALL)")
				else if (nError == D3DERR_NOTAVAILABLE)
					PL_LOG(Error, "Can't create Direct3D device! (D3DERR_NOTAVAILABLE)")
				else if (nError == D3DERR_OUTOFVIDEOMEMORY)
					PL_LOG(Error, "Can't create Direct3D device! (D3DERR_OUTOFVIDEOMEMORY)")
				else
					PL_LOG(Error, "Can't create Direct3D device!")
			}
		} else {
			m_pD3D->Release();
			m_pD3D = nullptr;
		}
	}
}

/**
*  @brief
*    Destructor
*/
Renderer::~Renderer()
{
	// Is the renderer initialized?
	if (m_pD3D) {
		// Output log information
		PL_LOG(Info, "De-initialize Direct3D 9 renderer")

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

		// Destroy the font manager
		delete static_cast<PLRenderer::FontManagerBackend*>(m_pFontManager);
		m_pFontManager = nullptr;

		// Destroy the draw helpers instance
		delete m_pDrawHelpers;
		m_pDrawHelpers = nullptr;

		// Destroy all renderer resources of this renderer
		while (m_lstResources.GetNumOfElements())
			delete m_lstResources[0];

		// Release the renderer device
		if (m_pDevice) {
			m_pDevice->Release();
			m_pDevice = nullptr;
		}

		// Release the D3D object
		m_pD3D->Release();
		m_pD3D = nullptr;

		// Destroy the top level window
		DestroyWindow(m_hWnd);
		m_hWnd						 = nullptr;
		m_pFirstSwapChainUserSurface = nullptr;
		HMODULE hModuleHandle = GetModuleHandle(nullptr);
		if (hModuleHandle)
			UnregisterClassA("PLDirect3D9", hModuleHandle);

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
	}
}

/**
*  @brief
*    Returns the instance of the IDirect3D9 object
*/
LPDIRECT3D9 Renderer::GetInstance() const
{
	return m_pD3D;
}

/**
*  @brief
*    Returns the Direct3D 9 renderer device
*/
LPDIRECT3DDEVICE9 Renderer::GetDevice() const
{
	return m_pDevice;
}

/**
*  @brief
*    Chooses the image source and internal texture buffer formats depending on various input parameters
*/
PLRenderer::TextureBuffer::EPixelFormat Renderer::ChooseFormats(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags,
																PLRenderer::TextureBuffer::EPixelFormat &nImageFormat, bool &bUsePreCompressedData) const
{
	PLRenderer::TextureBuffer::EPixelFormat nChosenInternalFormat = PLRenderer::TextureBuffer::Unknown;

	// Get image pixel format
	nImageFormat = PLRenderer::TextureBuffer::GetFormatFromImage(cImage, !(nFlags & PLRenderer::TextureBuffer::Compression));
	bUsePreCompressedData = false;
	if (PLRenderer::TextureBuffer::IsCompressedFormat(nImageFormat)) {
		// If the given image is pre compressed, but the hardware does not support the used compression format
		// we have to use the uncompressed image instead.
		bUsePreCompressedData = true; // Use the pre compressed data
		// [TODO] Check whether or not the hardware supports texture buffer compression...
//		if (!IsGL_ARB_texture_compression() ||
//			(nImageFormat <= PLRenderer::TextureBuffer::DXT5 && !IsGL_EXT_texture_compression_s3tc()) ||
//			(nImageFormat == PLRenderer::TextureBuffer::LATC1 && !IsGL_EXT_texture_compression_latc()) ||
//			(nImageFormat == PLRenderer::TextureBuffer::LATC2 && !IsGL_EXT_texture_compression_latc() && !IsGL_ATI_texture_compression_3dc())) {
			// Do not use texture buffer compression
//			nImageFormat = PLRenderer::TextureBuffer::GetFormatFromImage(cImage, true);
//		} else {
//			bUsePreCompressedData = true; // Use the pre compressed data
//		}
	}

	// Get internal pixel format
	if (nInternalFormat != PLRenderer::TextureBuffer::Unknown) {
		nChosenInternalFormat = nInternalFormat;
		// [TODO] Check whether or not the hardware supports texture buffer compression...
/*		if (PLRenderer::TextureBuffer::IsCompressedFormat(nChosenInternalFormat) &&
			((nChosenInternalFormat <= PLRenderer::TextureBuffer::DXT5 && !IsGL_EXT_texture_compression_s3tc()) ||
			 (nChosenInternalFormat == PLRenderer::TextureBuffer::LATC1 && !IsGL_EXT_texture_compression_latc()) ||
			 (nChosenInternalFormat == PLRenderer::TextureBuffer::LATC2 && !IsGL_EXT_texture_compression_latc() && !IsGL_ATI_texture_compression_3dc()))) {
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
			}
		}
*/
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
			// Get the image buffer
			ImageBuffer *pImageBuffer = cImage.GetBuffer();
			if (pImageBuffer) {
				switch (pImageBuffer->GetComponentsPerPixel()) {
					case 1:
						// [TODO] Check whether or not the hardware supports texture buffer compression...
	//					if (IsGL_EXT_texture_compression_latc())
							nChosenInternalFormat = PLRenderer::TextureBuffer::LATC1;
						break;

					case 2:
						// [TODO] Check whether or not the hardware supports texture buffer compression...
	//					if (IsGL_EXT_texture_compression_latc() || IsGL_ATI_texture_compression_3dc())
							nChosenInternalFormat = PLRenderer::TextureBuffer::LATC2;
						break;

					case 3:
						// [TODO] Check whether or not the hardware supports texture buffer compression...
	//					if (IsGL_EXT_texture_compression_s3tc())
							nChosenInternalFormat = PLRenderer::TextureBuffer::DXT1;
						break;

					case 4:
						// [TODO] Check whether or not the hardware supports texture buffer compression...
	//					if (IsGL_EXT_texture_compression_s3tc())
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
*    Initializes all wrappers
*/
void Renderer::InitWrappers()
{
	// Fill modes
	m_cPLE_FILLWrapper.Resize(PLRenderer::Fill::Number, false, false);
	m_cPLE_FILLWrapper += D3DFILL_POINT;		//  0: PLRenderer::Fill::Point
	m_cPLE_FILLWrapper += D3DFILL_WIREFRAME;	//  1: PLRenderer::Fill::Line
	m_cPLE_FILLWrapper += D3DFILL_SOLID;		//  2: PLRenderer::Fill::Solid

	// Cull modes
	m_cPLE_CULLWrapper.Resize(PLRenderer::Cull::Number, false, false);
	m_cPLE_CULLWrapper += D3DCULL_NONE;	//  0: PLRenderer::Cull::None
	m_cPLE_CULLWrapper += D3DCULL_CCW;	//  1: PLRenderer::Cull::CW
	m_cPLE_CULLWrapper += D3DCULL_CW;	//  2: PLRenderer::Cull::CCW

	// Blend functions
	m_cPLE_BLENDWrapper.Resize(PLRenderer::BlendFunc::Number, false, false);
	m_cPLE_BLENDWrapper += D3DBLEND_ZERO;			//  0: PLRenderer::BlendFunc::Zero
	m_cPLE_BLENDWrapper += D3DBLEND_ONE;			//  1: PLRenderer::BlendFunc::One
	m_cPLE_BLENDWrapper += D3DBLEND_SRCCOLOR;		//  2: PLRenderer::BlendFunc::SrcColor
	m_cPLE_BLENDWrapper += D3DBLEND_INVSRCCOLOR;	//  3: PLRenderer::BlendFunc::InvSrcColor
	m_cPLE_BLENDWrapper += D3DBLEND_SRCALPHA;		//  4: PLRenderer::BlendFunc::SrcAlpha
	m_cPLE_BLENDWrapper += D3DBLEND_INVSRCALPHA;	//  5: PLRenderer::BlendFunc::InvSrcAlpha
	m_cPLE_BLENDWrapper += D3DBLEND_SRCALPHASAT;	//  6: PLRenderer::BlendFunc::SrcAlphaSat
	m_cPLE_BLENDWrapper += D3DBLEND_DESTCOLOR;		//  7: PLRenderer::BlendFunc::DstColor
	m_cPLE_BLENDWrapper += D3DBLEND_INVDESTCOLOR;	//  8: PLRenderer::BlendFunc::InvDstColor
	m_cPLE_BLENDWrapper += D3DBLEND_DESTALPHA;		//  9: PLRenderer::BlendFunc::DstAlpha
	m_cPLE_BLENDWrapper += D3DBLEND_INVDESTALPHA;	// 10: PLRenderer::BlendFunc::InvDstAlpha

	// Comparison functions
	m_cPLE_CMPWrapper.Resize(PLRenderer::Compare::Number, false, false);
	m_cPLE_CMPWrapper += D3DCMP_NEVER;			//  0: PLRenderer::Compare::Never
	m_cPLE_CMPWrapper += D3DCMP_LESS;			//  1: PLRenderer::Compare::Less
	m_cPLE_CMPWrapper += D3DCMP_EQUAL;			//  2: PLRenderer::Compare::Equal
	m_cPLE_CMPWrapper += D3DCMP_LESSEQUAL;		//  3: PLRenderer::Compare::LessEqual
	m_cPLE_CMPWrapper += D3DCMP_GREATER;		//  4: PLRenderer::Compare::Greater
	m_cPLE_CMPWrapper += D3DCMP_NOTEQUAL;		//  5: PLRenderer::Compare::NotEqual
	m_cPLE_CMPWrapper += D3DCMP_GREATEREQUAL;	//  6: PLRenderer::Compare::GreaterEqual
	m_cPLE_CMPWrapper += D3DCMP_ALWAYS;			//  7: PLRenderer::Compare::Always

	// Stencil operations
	m_cPLE_SOPWrapper.Resize(PLRenderer::StencilOp::Number, false, false);
	m_cPLE_SOPWrapper += D3DSTENCILOP_KEEP;		//  0: PLRenderer::StencilOp::Keep
	m_cPLE_SOPWrapper += D3DSTENCILOP_ZERO;		//  1: PLRenderer::StencilOp::Zero
	m_cPLE_SOPWrapper += D3DSTENCILOP_REPLACE;	//  2: PLRenderer::StencilOp::Replace
	m_cPLE_SOPWrapper += D3DSTENCILOP_INCRSAT;	//  3: PLRenderer::StencilOp::Incr
	m_cPLE_SOPWrapper += D3DSTENCILOP_DECRSAT;	//  4: PLRenderer::StencilOp::Decr
	m_cPLE_SOPWrapper += D3DSTENCILOP_INCR;		//  5: PLRenderer::StencilOp::IncrWrap
	m_cPLE_SOPWrapper += D3DSTENCILOP_DECR;		//  6: PLRenderer::StencilOp::DecrWrap
	m_cPLE_SOPWrapper += D3DSTENCILOP_INVERT;	//  7: PLRenderer::StencilOp::Invert

	// Primitive types
	m_cPLE_PTWrapper.Resize(PLRenderer::Primitive::Number, false, false);
	m_cPLE_PTWrapper += D3DPT_POINTLIST;		//  0: PLRenderer::Primitive::PointList
	m_cPLE_PTWrapper += D3DPT_LINELIST;			//  1: PLRenderer::Primitive::LineList
	m_cPLE_PTWrapper += D3DPT_LINESTRIP;		//  2: PLRenderer::Primitive::LineStrip
	m_cPLE_PTWrapper += D3DPT_TRIANGLELIST;		//  3: PLRenderer::Primitive::TriangleList
	m_cPLE_PTWrapper += D3DPT_TRIANGLESTRIP;	//  4: PLRenderer::Primitive::TriangleStrip
	m_cPLE_PTWrapper += D3DPT_TRIANGLEFAN;		//  5: PLRenderer::Primitive::TriangleFan

	// Texture-addressing modes
	m_cPLE_TAWrapper.Resize(PLRenderer::TextureAddressing::Number, false, false);
	m_cPLE_TAWrapper += D3DTADDRESS_CLAMP;	//  0: PLRenderer::TextureAddressing::Clamp
	m_cPLE_TAWrapper += D3DTADDRESS_BORDER;	//  1: PLRenderer::TextureAddressing::Border
	m_cPLE_TAWrapper += D3DTADDRESS_WRAP;	//  2: PLRenderer::TextureAddressing::Wrap
	m_cPLE_TAWrapper += D3DTADDRESS_MIRROR;	//  3: PLRenderer::TextureAddressing::Mirror

	// Texture buffer pixel formats
	m_cPLE_TPFWrapper.Resize(PLRenderer::TextureBuffer::NumOfPixelFormats, false, false);
	m_cPLE_TPFWrapper += D3DFMT_L8;													//  0: PLRenderer::TextureBuffer::L8
	m_cPLE_TPFWrapper += D3DFMT_L16;												//  1: PLRenderer::TextureBuffer::L16
	m_cPLE_TPFWrapper += D3DFMT_A8;													//  2: PLRenderer::TextureBuffer::A8
	m_cPLE_TPFWrapper += D3DFMT_A4L4;												//  3: PLRenderer::TextureBuffer::L4A4
	m_cPLE_TPFWrapper += D3DFMT_A8L8;												//  4: PLRenderer::TextureBuffer::L8A8
	m_cPLE_TPFWrapper += D3DFMT_D16;												//  5: PLRenderer::TextureBuffer::D16
	m_cPLE_TPFWrapper += D3DFMT_D24X8;												//  6: PLRenderer::TextureBuffer::D24
	m_cPLE_TPFWrapper += D3DFMT_D32;												//  7: PLRenderer::TextureBuffer::D32
	m_cPLE_TPFWrapper += D3DFMT_R3G3B2;												//  8: PLRenderer::TextureBuffer::R3G3B2
	m_cPLE_TPFWrapper += D3DFMT_R5G6B5;												//  9: PLRenderer::TextureBuffer::R5G6B5
	m_cPLE_TPFWrapper += D3DFMT_A1R5G5B5;											// 10: PLRenderer::TextureBuffer::R5G5B5A1
	m_cPLE_TPFWrapper += D3DFMT_A4R4G4B4;											// 11: PLRenderer::TextureBuffer::R4G4B4A4
	// D3DFMT_R8G8B8 isn't supported widely on current graphics adapters, so we DON'T use it!
	m_cPLE_TPFWrapper += D3DFMT_X8R8G8B8;											// 12: PLRenderer::TextureBuffer::R8G8B8
	m_cPLE_TPFWrapper += D3DFMT_A8R8G8B8;											// 13: PLRenderer::TextureBuffer::R8G8B8A8
	m_cPLE_TPFWrapper += D3DFMT_A2R10G10B10;										// 14: PLRenderer::TextureBuffer::R10G10B10A2
	m_cPLE_TPFWrapper += D3DFMT_A16B16G16R16;										// 15: PLRenderer::TextureBuffer::R16G16B16A16
	m_cPLE_TPFWrapper += D3DFMT_DXT1;												// 16: PLRenderer::TextureBuffer::DXT1
	m_cPLE_TPFWrapper += D3DFMT_DXT3;												// 17: PLRenderer::TextureBuffer::DXT3
	m_cPLE_TPFWrapper += D3DFMT_DXT5;												// 18: PLRenderer::TextureBuffer::DXT5
	m_cPLE_TPFWrapper += static_cast<D3DFORMAT>(MAKEFOURCC('A', 'T', 'I', '1'));	// 19: PLRenderer::TextureBuffer::LATC1
	m_cPLE_TPFWrapper += static_cast<D3DFORMAT>(MAKEFOURCC('A', 'T', 'I', '2'));	// 19: PLRenderer::TextureBuffer::LATC2
	m_cPLE_TPFWrapper += D3DFMT_R16F;												// 20: PLRenderer::TextureBuffer::L16F
	m_cPLE_TPFWrapper += D3DFMT_R32F;												// 21: PLRenderer::TextureBuffer::L32F
	m_cPLE_TPFWrapper += D3DFMT_A16B16G16R16F;										// 22: PLRenderer::TextureBuffer::R16G16B16A16F
	m_cPLE_TPFWrapper += D3DFMT_A32B32G32R32F;										// 23: PLRenderer::TextureBuffer::R32G32B32A32F
}

/**
*  @brief
*    Setup the renderer capabilities
*/
void Renderer::SetupCapabilities()
{
	// Get Direct3D capabilities
	D3DCAPS9 pCaps;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &pCaps);

	// [TODO] Total available GPU memory in kilobytes
	m_sCapabilities.nTotalAvailableGPUMemory = 0;

	// Maximum number of color render targets
	m_sCapabilities.nMaxColorRenderTargets = static_cast<uint8>(pCaps.NumSimultaneousRTs);

	// Maximum number of texture units
	m_sCapabilities.nMaxTextureUnits = static_cast<uint8>(pCaps.MaxSimultaneousTextures);

	// Maximum anisotropy
	m_sCapabilities.nMaxAnisotropy = static_cast<uint16>(pCaps.MaxAnisotropy);

	// Maximum tessellation factor
	// [TODO] ATI extension available
	m_sCapabilities.nMaxTessellationFactor = static_cast<uint8>(1);

	// Maximum texture buffer size
	m_sCapabilities.nMaxTextureBufferSize = static_cast<uint16>(pCaps.MaxTextureWidth);

	// Non power of two texture buffers supported?
	m_sCapabilities.bTextureBufferNonPowerOfTwo = ((pCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL) && (pCaps.TextureCaps & D3DPTEXTURECAPS_POW2));

	// [TODO] 2D array texture buffer support
	m_sCapabilities.bTextureBuffer2DArray = false;
	m_sCapabilities.nMaxTextureBuffer2DArrayLayers = 0;

	// [TODO] Perform a correct check...
	// Rectangle texture buffers supported?
	m_sCapabilities.bTextureBufferRectangle = true;
//	m_sCapabilities.bTextureBufferRectangle = pCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL &&
//											  pCaps.TextureCaps & D3DPTEXTURECAPS_POW2;

	// Maximum rectangle texture buffer size
	m_sCapabilities.nMaxRectangleTextureBufferSize = static_cast<uint16>(pCaps.MaxTextureWidth);

	// 3D texture buffers supported?
	m_sCapabilities.bTextureBuffer3D = (pCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP) != 0;

	// Maximum 3D texture buffer size
	m_sCapabilities.nMax3DTextureBufferSize = static_cast<uint16>(pCaps.MaxVolumeExtent);

	// Cube texture buffers supported?
	m_sCapabilities.bTextureBufferCube = (pCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP) != 0;

	// Maximum cube texture buffer size
	m_sCapabilities.nMaxCubeTextureBufferSize = static_cast<uint16>(pCaps.MaxTextureWidth);

	// Stencil wrap supported?
	if ((pCaps.StencilCaps & D3DSTENCILCAPS_INCR) && (pCaps.StencilCaps & D3DSTENCILCAPS_DECR))
		m_sCapabilities.bStencilWrap = true;
	else
		m_sCapabilities.bStencilWrap = false;

	// Two sided stencils supported?
	m_sCapabilities.bTwoSidedStencils = (pCaps.StencilCaps & D3DSTENCILCAPS_TWOSIDED) != 0;

	// Depth bounds test supported? (it's looks like that Direct3D 9 doesn't have this feature!)
	m_sCapabilities.bDepthBoundsTest = false;

	// Point sprite supported?
	m_sCapabilities.bPointSprite = (pCaps.FVFCaps & D3DFVFCAPS_PSIZE) != 0;

	// [TODO] Point parameters supported?
	m_sCapabilities.bPointParameters = true;

	// [TODO] Occlusion query supported
	m_sCapabilities.bOcclusionQuery = true;

	// Vertex buffer secondary color supported?
	m_sCapabilities.bVertexBufferSecondaryColor = true;

	// Multisample antialiasing samples
	m_sCapabilities.nMultisampleAntialiasingSamples = 0;

	// [TODO] Geometric primitive instancing
	m_sCapabilities.bInstancing = false;

	// Show renderer capabilities
	ShowRendererCapabilities();
}

/**
*  @brief
*    Enumerates all available display modes
*/
bool Renderer::QueryDisplayModes()
{
	D3DDISPLAYMODE sDisplayMode;
	String sTemp;

	// Clear old list of display modes
	for (uint32 i=0; i<m_lstDisplayModeList.GetNumOfElements(); i++)
		delete m_lstDisplayModeList[i];
	m_lstDisplayModeList.Clear();

	// Get list of display modes
	PL_LOG(Info, "Query available display modes")

	// Add 16 bit formats
	uint32 nNumOfAdapterModes = m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_R5G6B5);
	for (uint32 nMode=0; nMode<nNumOfAdapterModes; nMode++) {
		// Get display mode
		m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_R5G6B5, nMode, &sDisplayMode);

		// Get required information
		PLRenderer::DisplayMode *pDisplayMode = new PLRenderer::DisplayMode;
		pDisplayMode->vSize.x    = sDisplayMode.Width;
		pDisplayMode->vSize.y    = sDisplayMode.Height;
		pDisplayMode->nColorBits = 16;
		pDisplayMode->nFrequency = sDisplayMode.RefreshRate;

		// Give out log message
		if (pDisplayMode->nFrequency) {
			sTemp = String::Format("Found: %dx%dx%16 %d Hz", pDisplayMode->vSize.x, pDisplayMode->vSize.y, pDisplayMode->nFrequency);
		} else {
			sTemp = String::Format("Found: %dx%dx%16", pDisplayMode->vSize.x, pDisplayMode->vSize.y);
		}

		// Add found display mode to list
		PL_LOG(Info, sTemp)
		m_lstDisplayModeList.Add(pDisplayMode);
	}

	// Add 32 bit formats
	nNumOfAdapterModes = m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
	for (uint32 nMode=0; nMode<nNumOfAdapterModes; nMode++) {
		// Get display mode
		m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, nMode, &sDisplayMode);

		// Get required information
		PLRenderer::DisplayMode *pDisplayMode = new PLRenderer::DisplayMode;
		pDisplayMode->vSize.x	 = sDisplayMode.Width;
		pDisplayMode->vSize.y	 = sDisplayMode.Height;
		pDisplayMode->nColorBits = 32;
		pDisplayMode->nFrequency = sDisplayMode.RefreshRate;

		// Give out log message
		if (pDisplayMode->nFrequency) {
			sTemp = String::Format("Found: %dx%dx%32 %d Hz", pDisplayMode->vSize.x, pDisplayMode->vSize.y, pDisplayMode->nFrequency);
		} else {
			sTemp = String::Format("Found: %dx%dx%32", pDisplayMode->vSize.x, pDisplayMode->vSize.y);
		}

		// Filter display modes
		if (pDisplayMode->vSize.x < 640 || pDisplayMode->vSize.y < 480) {
			delete pDisplayMode;
			PL_LOG(Info, sTemp + " -> filtered")
		} else {
			// Add found display mode to list
			PL_LOG(Info, sTemp)
			m_lstDisplayModeList.Add(pDisplayMode);
		}
	}

	// Was at least one display mode found?
	if (m_lstDisplayModeList.IsEmpty()) {
		PL_LOG(Error, "No available & supported display modes found!")

		// Error!
		return false;
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Renderer functions         ]
//[-------------------------------------------------------]
bool Renderer::IsInitialized() const
{
	return (m_pD3D != nullptr);
}

String Renderer::GetAPI(uint32 *pnVersion) const
{
	// Get version
	if (pnVersion) {
		if (m_pDevice)
			*pnVersion = D3DXGetDriverLevel(m_pDevice);
		else
			*pnVersion = 0;
	}

	// Get API
	static const String sString = "Direct3D";
	return sString;
}

String Renderer::GetVendor() const
{
	//  [TODO] Implement me
	return "Unknown";
}

String Renderer::GetDefaultShaderLanguage() const
{
	// [TODO] Implement me
	return "";
//	static const String sString = "Cg";
//	return (GetMode() != ModeFixedFunctions) ? sString : "";
}

PLRenderer::ShaderLanguage *Renderer::GetShaderLanguage(const String &sShaderLanguage)
{
	// [TODO] Implement me
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
}

void Renderer::RestoreDeviceObjects()
{
	// Call base function
	PLRenderer::RendererBackend::RestoreDeviceObjects();
}

const Vector2 &Renderer::GetTexelToPixelOffset() const
{
	// Look out, texel offset ahead!
	static const Vector2 vTexelOffset(-0.5f, -0.5f);
	return vTexelOffset;
}


//[-------------------------------------------------------]
//[ Create surface/resource                               ]
//[-------------------------------------------------------]
PLRenderer::SurfaceWindow *Renderer::CreateSurfaceWindow(PLRenderer::SurfaceWindowHandler &cHandler, handle nNativeWindowHandle, const PLRenderer::DisplayMode &sDisplayMode, bool bFullscreen)
{
	// Is the surface window handler valid?
	if (cHandler.GetRenderer() != this)
		return nullptr; // Error!

	// Create and register renderer surface
	PLRenderer::SurfaceWindow *pRendererSurface = new SurfaceWindow(cHandler, nNativeWindowHandle, bFullscreen);
	m_lstSurfaces.Add(pRendererSurface);

	// Return created renderer surface
	return pRendererSurface;
}

PLRenderer::SurfaceTextureBuffer *Renderer::CreateSurfaceTextureBuffer2D(const Vector2i &vSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, uint32 nFlags, uint8 nMaxColorTargets)
{
	// Check maximum render targets and dimension
	if (nMaxColorTargets && nMaxColorTargets <= m_sCapabilities.nMaxColorRenderTargets && IsValidTextureBuffer2DSize(vSize.x) && IsValidTextureBuffer2DSize(vSize.y)) {
		// Create and register renderer surface
		PLRenderer::TextureBuffer *pTextureBuffer = new TextureBuffer2D(*this, vSize, nFormat, PLRenderer::TextureBuffer::RenderTarget);
		PLRenderer::SurfaceTextureBuffer *pRendererSurface = new SurfaceTextureBuffer(*this, *pTextureBuffer, nFlags, nMaxColorTargets);
		m_lstSurfaces.Add(pRendererSurface);

		// Return created renderer surface
		return pRendererSurface;
	} else {
		return nullptr;
	}
}

PLRenderer::SurfaceTextureBuffer *Renderer::CreateSurfaceTextureBufferRectangle(const Vector2i &vSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, uint32 nFlags, uint8 nMaxColorTargets)
{
	// Check maximum render targets and dimension
	if (nMaxColorTargets && nMaxColorTargets <= m_sCapabilities.nMaxColorRenderTargets && IsValidTextureBufferRectangleSize(vSize.x) && IsValidTextureBufferRectangleSize(vSize.y)) {
		// Create and register renderer surface
		PLRenderer::TextureBuffer *pTextureBuffer = new TextureBufferRectangle(*this, vSize, nFormat, PLRenderer::TextureBuffer::RenderTarget);
		PLRenderer::SurfaceTextureBuffer *pRendererSurface = new SurfaceTextureBuffer(*this, *pTextureBuffer, nFlags, nMaxColorTargets);
		m_lstSurfaces.Add(pRendererSurface);

		// Return created renderer surface
		return pRendererSurface;
	} else {
		return nullptr;
	}
}

PLRenderer::SurfaceTextureBuffer *Renderer::CreateSurfaceTextureBufferCube(uint16 nSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, uint32 nFlags)
{
	// Valid dimension?
	if (!IsValidTextureBufferCubeSize(nSize))
		return nullptr;

	// Create and register renderer surface
	PLRenderer::TextureBuffer *pTextureBuffer = new TextureBufferCube(*this, nSize, nFormat, PLRenderer::TextureBuffer::RenderTarget);
	PLRenderer::SurfaceTextureBuffer *pRendererSurface = new SurfaceTextureBuffer(*this, *pTextureBuffer, nFlags, 1);
	m_lstSurfaces.Add(pRendererSurface);

	// Return created renderer surface
	return pRendererSurface;
}

PLRenderer::TextureBuffer1D *Renderer::CreateTextureBuffer1D(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer1D(cImage, nInternalFormat))
		return nullptr;

	// Create the Direct3D 9 1D texture buffer
	return new TextureBuffer1D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer2D *Renderer::CreateTextureBuffer2D(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer2D(cImage, nInternalFormat))
		return nullptr;

	// Create the Direct3D 9 2D texture buffer
	return new TextureBuffer2D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer2DArray *Renderer::CreateTextureBuffer2DArray(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// [TODO] Implement me
	return nullptr;
}

PLRenderer::TextureBuffer *Renderer::CreateTextureBufferRectangle(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!m_sCapabilities.bTextureBufferRectangle || !CheckTextureBufferRectangle(cImage, nInternalFormat))
		return nullptr;

	// Create the Direct3D 9 rectangle texture buffer
	return new TextureBufferRectangle(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer3D *Renderer::CreateTextureBuffer3D(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer3D(cImage, nInternalFormat))
		return nullptr;

	// Create the Direct3D 9 3D texture buffer
	return new TextureBuffer3D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBufferCube *Renderer::CreateTextureBufferCube(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBufferCube(cImage, nInternalFormat))
		return nullptr;

	// Create the Direct3D 9 cube texture buffer
	return new TextureBufferCube(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::IndexBuffer *Renderer::CreateIndexBuffer()
{
	// Create the Direct3D 9 index buffer
	return new IndexBuffer(*this);
}

PLRenderer::VertexBuffer *Renderer::CreateVertexBuffer()
{
	// Create the Direct3D 9 vertex buffer
	return new VertexBuffer(*this);
}

PLRenderer::OcclusionQuery *Renderer::CreateOcclusionQuery()
{
	// Create the Direct3D 9 occlusion query
	return new OcclusionQuery(*this);
}


//[-------------------------------------------------------]
//[ States                                                ]
//[-------------------------------------------------------]
bool Renderer::SetRenderState(PLRenderer::RenderState::Enum nState, uint32 nValue)
{
	// Check if the state is a valid render state member
	if (!m_pDevice || nState >= PLRenderer::RenderState::Number)
		return false; // Error!

	// Check if this render state is already set to this value
	if (m_nRenderState[nState] != nValue) {
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
							if (&nAPIValue != &Array<uint32>::Null) {
								if (m_pDevice->SetRenderState(D3DRS_FILLMODE, nAPIValue) != D3D_OK)
									return false; // Error!
							} else {
								// Error, invalid value!
								return false;
							}
						}
						break;

					case PLRenderer::RenderState::CullMode:
					{
						// Invert cull mode?
						bool bAPISwapY = false;
						if (m_cCurrentSurface.GetSurface())
							bAPISwapY = m_cCurrentSurface.GetSurface()->IsAPISwapY();
						if (!bAPISwapY &&  GetRenderState(PLRenderer::RenderState::InvCullMode) ||
							 bAPISwapY && !GetRenderState(PLRenderer::RenderState::InvCullMode)) {
							if (nValue == PLRenderer::Cull::CW)
								nValue = PLRenderer::Cull::CCW;
							else if (nValue == PLRenderer::Cull::CCW)
								nValue = PLRenderer::Cull::CW;
						}

						// Set the state
						const uint32 &nAPIValue = m_cPLE_CULLWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_CULLMODE, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

				// Z buffer
					case PLRenderer::RenderState::ZEnable:
						if (nValue != 0 && nValue != 1)
							return false; // Error, invalid value!
						if (m_pDevice->SetRenderState(D3DRS_ZENABLE, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::ZWriteEnable:
						if (nValue != 0 && nValue != 1)
							return false; // Error, invalid value!
						if (m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::ZFunc:
					{
						const uint32 &nAPIValue = m_cPLE_CMPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_ZFUNC, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::ZBias:
						if (m_pFixedFunctions && !m_pFixedFunctions->UpdateCurrentD3D9ProjectionMatrix())
							return false; // Error!
						break;

					case PLRenderer::RenderState::SlopeScaleDepthBias:
						if (m_pDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::DepthBias:
						if (m_pDevice->SetRenderState(D3DRS_DEPTHBIAS, nValue) != D3D_OK)
							return false; // Error!
						break;
				}
			} else {
				switch (nState) {
				// Blend
					case PLRenderer::RenderState::BlendEnable:
						if (nValue != 0 && nValue != 1)
							return false; // Error, invalid value!
						if (m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::SrcBlendFunc:
					{
						const uint32 &nAPIValue = m_cPLE_BLENDWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_SRCBLEND, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::DstBlendFunc:
					{
						const uint32 &nAPIValue = m_cPLE_BLENDWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_DESTBLEND, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

				// Stencil
					case PLRenderer::RenderState::StencilEnable:
						if (nValue != 0 && nValue != 1)
							return false; // Error, invalid value!
						if (m_pDevice->SetRenderState(D3DRS_STENCILENABLE, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::StencilFunc:
					{
						const uint32 &nAPIValue = m_cPLE_CMPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_STENCILFUNC, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::StencilRef:
						if (m_pDevice->SetRenderState(D3DRS_STENCILREF, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::StencilMask:
						if (m_pDevice->SetRenderState(D3DRS_STENCILMASK, nValue) != D3D_OK)
							return false; // Error!
						if (m_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::StencilFail:
					{
						const uint32 &nAPIValue = m_cPLE_SOPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_STENCILFAIL, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::StencilZFail:
					{
						const uint32 &nAPIValue = m_cPLE_SOPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::StencilPass:
					{
						const uint32 &nAPIValue = m_cPLE_SOPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_STENCILPASS, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::TwoSidedStencilMode:
						if (nValue != 0 && nValue != 1)
							return false; // Error, invalid value!
						if (m_pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, nValue) != D3D_OK)
							return false; // Error!
						break;

					case PLRenderer::RenderState::CCWStencilFunc:
					{
						const uint32 &nAPIValue = m_cPLE_CMPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::CCWStencilFail:
					{
						const uint32 &nAPIValue = m_cPLE_SOPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::CCWStencilZFail:
					{
						const uint32 &nAPIValue = m_cPLE_SOPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}

					case PLRenderer::RenderState::CCWStencilPass:
					{
						const uint32 &nAPIValue = m_cPLE_SOPWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_CCW_STENCILPASS, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
						break;
					}
				}
			}
		} else {
			switch (nState) {
			// Point and line
				case PLRenderer::RenderState::PointSize:
					if (m_pDevice->SetRenderState(D3DRS_POINTSIZE, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::PointScaleEnable:
					if (nValue != 0 && nValue != 1)
						return false; // Error, invalid value!
					if (m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::PointSizeMin:
					if (m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::PointSizeMax:
					if (m_pDevice->SetRenderState(D3DRS_POINTSIZE_MAX, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::PointScaleA:
					if (m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::PointScaleB:
					if (m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::PointScaleC:
					if (m_pDevice->SetRenderState(D3DRS_POINTSCALE_C, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::LineWidth:
					// Direct3D 9 does not support line width!
					break;

			// Tessellation
				case PLRenderer::RenderState::TessellationFactor:
					// [TODO] Implement
					break;

				case PLRenderer::RenderState::TessellationMode:
					// [TODO] Implement
					break;

			// Misc
				case PLRenderer::RenderState::PointSpriteEnable:
					if (nValue != 0 && nValue != 1)
						return false; // Error, invalid value!
					if (m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::DitherEnable:
					if (nValue != 0 && nValue != 1)
						return false; // Error, invalid value!
					if (m_pDevice->SetRenderState(D3DRS_DITHERENABLE, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::ScissorTestEnable:
					if (nValue != 0 && nValue != 1)
						return false; // Error, invalid value!
					if (m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, nValue) != D3D_OK)
						return false; // Error!
					break;

				case PLRenderer::RenderState::MultisampleEnable:
					// [TODO] Implement
					break;

				case PLRenderer::RenderState::DepthClamp:
					// [TODO] Implement
					break;

				case PLRenderer::RenderState::InvCullMode:
					if (GetRenderState(PLRenderer::RenderState::CullMode) != PLRenderer::Cull::None) {
						// Invert cull mode?
						bool bAPISwapY = false;
						if (m_cCurrentSurface.GetSurface())
							bAPISwapY = m_cCurrentSurface.GetSurface()->IsAPISwapY();
						if (!bAPISwapY && nValue || bAPISwapY && !nValue) { // Invert current active cull mode
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
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_CULLMODE, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
					}
					break;

				case PLRenderer::RenderState::FixedFillMode:
					if (nValue != PLRenderer::Fill::Unknown) {
						const uint32 &nAPIValue = m_cPLE_FILLWrapper[nValue];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_FILLMODE, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
					} else {
						const uint32 &nAPIValue = m_cPLE_FILLWrapper[GetRenderState(PLRenderer::RenderState::FillMode)];
						if (&nAPIValue != &Array<uint32>::Null) {
							if (m_pDevice->SetRenderState(D3DRS_FILLMODE, nAPIValue) != D3D_OK)
								return false; // Error!
						} else {
							// Error, invalid value!
							return false;
						}
					}
					break;

				default:
					return false; // Error, invalid render state!
			}
		}
	}

	// All went fine
	return true;
}

// Internal helper function
uint32 GetD3DFilterMode(uint32 nMode)
{
	switch (nMode) {
		case PLRenderer::TextureFiltering::None:
			return D3DTEXF_NONE;

		case PLRenderer::TextureFiltering::Point:
			return D3DTEXF_POINT;

		case PLRenderer::TextureFiltering::Linear:
			return D3DTEXF_LINEAR;

		case PLRenderer::TextureFiltering::Anisotropic:
			return D3DTEXF_ANISOTROPIC;

		default:
			return D3DTEXF_NONE; // Error!
	}
}

bool Renderer::SetSamplerState(uint32 nStage, PLRenderer::Sampler::Enum nState, uint32 nValue)
{
	// Check if the stage is correct and check if the state is a valid sampler member
	if (nStage >= m_sCapabilities.nMaxTextureUnits || !m_pDevice || nState >= PLRenderer::Sampler::Number)
		return false; // Error!

	// Set the sampler state
	m_ppnSamplerState[nStage][nState] = nValue;

	// Check if this sampler state is already set to this value
	if (m_ppnInternalSamplerState[nStage][nState] != nValue) {
		// Check whether mipmapping is allowed, if not, set a proper internal texture filter mode
		// for correct rendering...
		if (nState == PLRenderer::Sampler::MipFilter) {
			PLRenderer::TextureBuffer *pTextureBuffer = m_ppCurrentTextureBuffer[nStage];
			if (pTextureBuffer && !(pTextureBuffer->GetFlags() & PLRenderer::TextureBuffer::Mipmaps)) {
				nValue = PLRenderer::TextureFiltering::None;

				// Check if this sampler state is already set to this value
				if (m_ppnInternalSamplerState[nStage][nState] == nValue)
					return true; // Done - nothing to do here :)
			}
		}

		// Set the sampler state
		m_ppnInternalSamplerState[nStage][nState] = nValue;
		m_sStatistics.nSamplerStateChanges++;

		// Set sampler state
		switch (nState) {
		// Address modes
			case PLRenderer::Sampler::AddressU:
			{
				const uint32 &nAPIValue = m_cPLE_TAWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null) {
					if (m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSU, nAPIValue) != D3D_OK)
						return false; // Error!
				} else {
					// Error, invalid value!
					return false;
				}
				break;
			}

			case PLRenderer::Sampler::AddressV:
			{
				const uint32 &nAPIValue = m_cPLE_TAWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null) {
					if (m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSV, nAPIValue) != D3D_OK)
						return false; // Error!
				} else {
					// Error, invalid value!
					return false;
				}
				break;
			}

			case PLRenderer::Sampler::AddressW:
			{
				const uint32 &nAPIValue = m_cPLE_TAWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null) {
					if (m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSW, nAPIValue) != D3D_OK)
						return false; // Error!
				} else {
					// Error, invalid value!
					return false;
				}
				break;
			}

		// Filter
			case PLRenderer::Sampler::MagFilter:
				if (m_pDevice->SetSamplerState(nStage, D3DSAMP_MAGFILTER, GetD3DFilterMode(nValue)) != D3D_OK)
					return false; // Error!
				break;

			case PLRenderer::Sampler::MinFilter:
				if (m_pDevice->SetSamplerState(nStage, D3DSAMP_MINFILTER, GetD3DFilterMode(nValue)) != D3D_OK)
					return false; // Error!
				break;

			case PLRenderer::Sampler::MipFilter:
				if (m_pDevice->SetSamplerState(nStage, D3DSAMP_MIPFILTER, GetD3DFilterMode(nValue)) != D3D_OK)
					return false; // Error!
				break;

		// Misc
			case PLRenderer::Sampler::MipmapLODBias:
				if (m_pDevice->SetSamplerState(nStage, D3DSAMP_MIPMAPLODBIAS, nValue) != D3D_OK)
					return false; // Error!
				break;

			case PLRenderer::Sampler::MaxMapLevel:
				if (m_pDevice->SetSamplerState(nStage, D3DSAMP_MAXMIPLEVEL, nValue == 1000 ? 0 : nValue) != D3D_OK)
					return false; // Error!
				break;

			case PLRenderer::Sampler::MaxAnisotropy:
				if (m_pDevice->SetSamplerState(nStage, D3DSAMP_MAXANISOTROPY, nValue) != D3D_OK)
					return false; // Error!
				break;

			default:
				return false; // Error, invalid sampler state!
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
	return (m_pDevice && m_pDevice->BeginScene() == D3D_OK);
}

bool Renderer::EndScene()
{
	return (m_pDevice && m_pDevice->EndScene() == D3D_OK);
}

bool Renderer::SetViewport(const PLMath::Rectangle *pRectangle, float fMinZ, float fMaxZ)
{
	// Check device
	if (m_pDevice) {
		// Call base implementation
		PLRenderer::RendererBackend::SetViewport(pRectangle);

		// Get D3D viewport
		D3DVIEWPORT9 sViewData;
		sViewData.X      = static_cast<DWORD>(m_cViewportRect.GetX());
		sViewData.Y      = static_cast<DWORD>(m_cViewportRect.GetY());
		sViewData.Width  = static_cast<DWORD>(m_cViewportRect.GetWidth());
		sViewData.Height = static_cast<DWORD>(m_cViewportRect.GetHeight());
		sViewData.MinZ   = fMinZ;
		sViewData.MaxZ   = fMaxZ;

		// Set viewport
		if (SUCCEEDED(m_pDevice->SetViewport(&sViewData)))
			return true; // Done
	}

	// Error!
	return false;
}

bool Renderer::SetScissorRect(const PLMath::Rectangle *pRectangle)
{
	// Call base implementation
	PLRenderer::RendererBackend::SetScissorRect(pRectangle);

	// Check device
	if (m_pDevice) {
		// Get scissor rectangle
		uint32 nX      = static_cast<uint32>(m_cViewportRect.GetX());
		uint32 nY      = static_cast<uint32>(m_cViewportRect.GetY());
		uint32 nWidth  = static_cast<uint32>(m_cViewportRect.GetWidth());
		uint32 nHeight = static_cast<uint32>(m_cViewportRect.GetHeight());

		// Set scissor rectangle
		RECT sRect = {nX, nY, nX+nWidth, nY+nHeight};
		if (SUCCEEDED(m_pDevice->SetScissorRect(&sRect)))
			return true; // Done
	}

	// Error!
	return false;
}

bool Renderer::GetDepthBounds(float &fZMin, float &fZMax) const
{
	// Error - not supported!
	return false;
}

bool Renderer::SetDepthBounds(float fZMin, float fZMax)
{
	// Error - not supported!
	return false;
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
	// Check device
	if (!m_pDevice)
		return false; // Error!

	// Set color mask
	m_bColorMask[0] = bRed;
	m_bColorMask[1] = bGreen;
	m_bColorMask[2] = bBlue;
	m_bColorMask[3] = bAlpha;
	uint32 nValue = 0;
	if (bRed)
		nValue |= D3DCOLORWRITEENABLE_RED;
	if (bGreen)
		nValue |= D3DCOLORWRITEENABLE_GREEN;
	if (bBlue)
		nValue |= D3DCOLORWRITEENABLE_BLUE;
	if (bAlpha)
		nValue |= D3DCOLORWRITEENABLE_ALPHA;

	// Make the D3D9 call
	return (m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, nValue) == D3D_OK);
}

bool Renderer::Clear(uint32 nFlags, const Color4 &cColor, float fZ, uint32 nStencil)
{
	// Check device
	if (m_pDevice) {
		// Get API flags
		uint32 nFlagsAPI = 0;
		if (nFlags & PLRenderer::Clear::Color)
			nFlagsAPI |= D3DCLEAR_TARGET;
		if (nFlags & PLRenderer::Clear::ZBuffer)
			nFlagsAPI |= D3DCLEAR_ZBUFFER;
		if (nFlags & PLRenderer::Clear::Stencil)
			nFlagsAPI |= D3DCLEAR_STENCIL;

		// Are API flags set?
		if (nFlagsAPI) {
			// Clear
			return (m_pDevice->Clear(0, nullptr, nFlagsAPI, cColor.ToUInt32(), fZ, nStencil) == D3D_OK);
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Get/set current resources                             ]
//[-------------------------------------------------------]
bool Renderer::SetRenderTarget(PLRenderer::Surface *pSurface, uint8 nFace)
{
	// Check device and parameter
	if (!m_pDevice || !m_lstSurfaces.IsElement(pSurface))
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
			// Error!?!
			return false;
		}
	} else {
		if (nFace > 0)
			return false; // Error!
	}

	PLRenderer::Surface *pSurfaceBackup = m_cCurrentSurface.GetSurface();
	if (m_cCurrentSurface.GetSurface())
		UnmakeSurfaceCurrent(*m_cCurrentSurface.GetSurface());
	m_cCurrentSurface.SetSurface(pSurface);

	// Disable all color render targets
	for (uint32 i=1; i<m_sCapabilities.nMaxColorRenderTargets; i++)
		m_pDevice->SetRenderTarget(i, nullptr);

	// Make the surface to the current render target
	bool bError = MakeSurfaceCurrent(*pSurface, nFace);
	if (bError) {
		// Do we need to update the projection matrix?
		if (m_pFixedFunctions && (!pSurfaceBackup || pSurfaceBackup->IsAPISwapY() != pSurface->IsAPISwapY()))
			m_pFixedFunctions->UpdateCurrentD3D9ProjectionMatrix();
	}

	// Setup viewport and scissor rectangle
	SetViewport();
	SetScissorRect();

	// Done
	return bError;
}

bool Renderer::SetColorRenderTarget(PLRenderer::TextureBuffer *pTextureBuffer, uint8 nColorIndex, uint8 nFace)
{
	if (!pTextureBuffer)
		return true; // Done

	// Get texture surface
	LPDIRECT3DTEXTURE9 pD3D9Texture = static_cast<TextureBuffer2D*>(pTextureBuffer)->GetD3D9Texture();
	if (!pD3D9Texture)
		return false; // Error!
	LPDIRECT3DSURFACE9 pTextureSurface;
	if (pD3D9Texture->GetSurfaceLevel(0, &pTextureSurface) != D3D_OK)
		return false; // Error!
	if (m_pDevice->SetRenderTarget(nColorIndex, pTextureSurface) != D3D_OK) {
//		uint32 i = 0;
	}
	pTextureSurface->Release();

/*	// Check parameter
	if (!m_lstSurfaces.IsElement(pSurface))
		return true;
	if (m_cCurrentSurface.GetSurface())
		m_cCurrentSurface.GetSurface()->UnmakeCurrent();
	m_cCurrentSurface.SetSurface(pSurface);

	// Make the surface to the current render target
	bool bError = pSurface->MakeCurrent(nFace);

	// Done
	return bError;
	*/

	// [TODO] Implement
	return false; // Error!
}

bool Renderer::SetDepthRenderTarget(PLRenderer::TextureBuffer *pTextureBuffer, uint8 nFace)
{
	// [TODO] Implement me

	// Error!
	return false;
}

bool Renderer::MakeScreenshot(Image &cImage)
{
	// [TODO] Fix me!
	// Obtain adapter number and device window
	D3DDEVICE_CREATION_PARAMETERS dcp;
	m_pDevice->GetCreationParameters(&dcp);

	// Obtain width and height for the front buffer surface
	IDirect3D9 *pD3D9;
	m_pDevice->GetDirect3D(&pD3D9);
	D3DDISPLAYMODE dm;
	pD3D9->GetAdapterDisplayMode(dcp.AdapterOrdinal, &dm);

	// Create the front buffer surface
	IDirect3DSurface9 *pSurface;
	HRESULT hr = m_pDevice->CreateOffscreenPlainSurface(dm.Width, dm.Height, D3DFMT_A8R8G8B8,
														D3DPOOL_DEFAULT, &pSurface, nullptr);
	if (FAILED(hr))
		return false; // Error!

	// Get a copy of the front buffer
	IDirect3DSwapChain9 *pSwapChain;
	m_pDevice->GetSwapChain(0, &pSwapChain);
	pSwapChain->GetFrontBufferData(pSurface);

	// Obtain the rectangle into which rendering is drawn
	// It's the client rectangle of the focus window in screen coordinates
	RECT rc;
	GetClientRect(dcp.hFocusWindow, &rc);
	ClientToScreen(dcp.hFocusWindow, LPPOINT(&rc.left));
	ClientToScreen(dcp.hFocusWindow, LPPOINT(&rc.right));

	// Save the front buffer
	D3DLOCKED_RECT cLockedRect;
	if (pSurface->LockRect(&cLockedRect, nullptr, 0) == D3D_OK) {
		// Create image buffer
		cImage.Unload();
		ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
		pImageBuffer->CreateImage(DataByte, ColorRGBA, Vector3i(dm.Width, dm.Height, 1));

		// Copy
		MemoryManager::Copy(pImageBuffer->GetData(), cLockedRect.pBits, pImageBuffer->GetDataSize());

		// Unlock the surface
		pSurface->UnlockRect();
	}
	pSurface->Release();
	pSwapChain->Release();

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
		if (nStage >= static_cast<signed>(m_sCapabilities.nMaxTextureUnits))
			return false; // Error!

		// Is this texture buffer already set?
		if (m_ppCurrentTextureBuffer[nStage] == pTextureBuffer)
			return false; // Error!

		// Make this texture buffer to the renderers current one
		PLRenderer::TextureBuffer *pT = m_ppCurrentTextureBuffer[nStage];
		m_ppCurrentTextureBuffer[nStage] = pTextureBuffer;

		// Should an texture buffer be set?
		if (pTextureBuffer) {
			// Yes, make it current
			m_sStatistics.nTextureBufferBinds++;
			if (!MakeTextureBufferCurrent(*pTextureBuffer, nStage)) {
				m_ppCurrentTextureBuffer[nStage] = pT;

				// Error!
				return false;
			}

			// Is the internal PLRenderer::Sampler::MipFilter state other than the 'official' state?
			if (m_ppnInternalSamplerState[nStage][PLRenderer::Sampler::MipFilter] != m_ppnSamplerState[nStage][PLRenderer::Sampler::MipFilter])
				SetSamplerState(nStage, static_cast<PLRenderer::Sampler::Enum>(PLRenderer::Sampler::MipFilter), m_ppnSamplerState[nStage][PLRenderer::Sampler::MipFilter]);
		} else {
			// No, deactivate texture buffer
			m_pDevice->SetTexture(nStage, nullptr);
		}
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
	PLRenderer::IndexBuffer *pT = m_pCurrentIndexBuffer;
	m_pCurrentIndexBuffer = pIndexBuffer;

	// Should an index buffer be set?
	if (pIndexBuffer) {
		// Yes, make it current
		if (!static_cast<IndexBuffer*>(pIndexBuffer)->MakeCurrent()) {
			m_pCurrentIndexBuffer = pT;

			// Error!
			return false;
		}
	} else {
		// No, deactivate index buffer
		m_pDevice->SetIndices(nullptr);
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

	// Check device
	if (!m_pDevice)
		return false; // Error!

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

		default:
			return false; // Error!
	}

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices;
	m_sStatistics.nTriangles += nPrimitiveCount;

	// Get API primitive type
	const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
	if (&nAPIValue != &Array<uint32>::Null) {
		// Set vertex declaration
		if (m_pFixedFunctions && !m_pFixedFunctions->SetVertexDeclaration())
			return false; // Error!

		// Draw primitive
		return (m_pDevice->DrawPrimitive(static_cast<D3DPRIMITIVETYPE>(nAPIValue), nStartIndex, nPrimitiveCount) == D3D_OK);
	} else {
		// Error, invalid value!
		return false;
	}
}

bool Renderer::DrawIndexedPrimitives(PLRenderer::Primitive::Enum nType, uint32 nMinIndex, uint32 nMaxIndex,
									 uint32 nStartIndex, uint32 nNumVertices)
{
	// Index correct?
	if (!m_pCurrentIndexBuffer)
		return false; // Error!

	// Draw something?
	if (!nNumVertices)
		return true; // Done

	// Check device
	if (!m_pDevice)
		return false; // Error!

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

		default:
			return false; // Error!
	}

	// Update statistics
	m_sStatistics.nDrawPrimitivCalls++;
	m_sStatistics.nVertices  += nNumVertices;
	m_sStatistics.nTriangles += nPrimitiveCount;

	// Get API primitive type
	const uint32 &nAPIValue = m_cPLE_PTWrapper[nType];
	if (&nAPIValue != &Array<uint32>::Null) {
		// Set vertex declaration
		if (m_pFixedFunctions && !m_pFixedFunctions->SetVertexDeclaration())
			return false; // Error!

		// Draw primitive
		return m_pDevice->DrawIndexedPrimitive(static_cast<D3DPRIMITIVETYPE>(nAPIValue),
											   0, // Done in SetStreamSource()
											   nMinIndex, nNumVertices,
											   nStartIndex, nPrimitiveCount) == D3D_OK;
	} else {
		// Error, invalid value!
		return false;
	}
}

bool Renderer::DrawPatches(uint32 nVerticesPerPatch, uint32 nStartIndex, uint32 nNumVertices)
{
	// Error! Tessellation is not supported by DirectX 9!
	return false;
}

bool Renderer::DrawIndexedPatches(uint32 nVerticesPerPatch, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices)
{
	// Error! Tessellation is not supported by DirectX 9!
	return false;
}


//[-------------------------------------------------------]
//[ Draw call with multiple primitive instances           ]
//[-------------------------------------------------------]
bool Renderer::DrawPrimitivesInstanced(PLRenderer::Primitive::Enum nType, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// [TODO] Implement me

	// Error!
	return false;
}

bool Renderer::DrawIndexedPrimitivesInstanced(PLRenderer::Primitive::Enum nType, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// [TODO] Implement me

	// Error!
	return false;
}

bool Renderer::DrawPatchesInstanced(uint32 nVerticesPerPatch, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// [TODO] Implement me

	// Error!
	return false;
}

bool Renderer::DrawIndexedPatchesInstanced(uint32 nVerticesPerPatch, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// [TODO] Implement me

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererD3D9
