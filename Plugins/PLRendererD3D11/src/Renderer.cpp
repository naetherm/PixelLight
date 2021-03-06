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
#include <PLCore/System/SystemWindows.h>
#include <PLGraphics/Image/Image.h>
#include <PLRenderer/Renderer/SurfaceWindowHandler.h>
#include <PLRenderer/Renderer/Backend/DrawHelpersBackend.h>
#include <PLRenderer/Renderer/Backend/FontManagerBackend.h>
#include "PLRendererD3D11/SurfaceWindow.h"
#include "PLRendererD3D11/SurfaceTextureBuffer.h"
#include "PLRendererD3D11/TextureBuffer1D.h"
#include "PLRendererD3D11/TextureBuffer2D.h"
#include "PLRendererD3D11/TextureBufferRectangle.h"
#include "PLRendererD3D11/TextureBuffer3D.h"
#include "PLRendererD3D11/TextureBufferCube.h"
#include "PLRendererD3D11/IndexBuffer.h"
#include "PLRendererD3D11/VertexBuffer.h"
#include "PLRendererD3D11/OcclusionQuery.h"
#include "PLRendererD3D11/Renderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererD3D11 {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Renderer, "PLRendererD3D11", PLRenderer::RendererBackend, "D3D11 renderer backend")
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
Renderer::Renderer(handle nNativeWindowHandle, EMode nMode, uint32 nZBufferBits, uint32 nStencilBits, uint32 nMultisampleAntialiasingSamples, String sDefaultShaderLanguage) : PLRenderer::RendererBackend(ModeShaders),	// There's no fixed functions support in D3D11
	m_pDXGIFactory1(nullptr),
	m_nD3DFeatureLevel(D3D_FEATURE_LEVEL_9_1),
	m_pD3D11DeviceContext(nullptr),
	m_pD3D11Device(nullptr),
	m_pD3D11RenderTargetView(nullptr),
	m_pFontManager(new PLRenderer::FontManagerBackend(*this))
{
	// Ignore the given native window handle

	// This renderer implementation has no shader support at all, so ignore sDefaultShaderLanguage

	// Output log information
	PL_LOG(Info, "Initialize D3D11 renderer")

	// Create the D3D11 device instance
	UINT nDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	#ifdef _DEBUG
		nDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
	HRESULT hResult = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, nDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &m_pD3D11Device, &m_nD3DFeatureLevel, &m_pD3D11DeviceContext);
	if (SUCCEEDED(hResult)) {
		{ // Get the used DXGI factory 1 instance
			// IDXGIDevice1
			IDXGIDevice1 *pDXGIDevice1 = nullptr;
			m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void **>(&pDXGIDevice1));
			if (pDXGIDevice1) {
				// IDXGIAdapter1
				IDXGIAdapter1 *pDXGIAdapter1 = nullptr;
				pDXGIDevice1->GetParent(__uuidof(IDXGIAdapter1), reinterpret_cast<void **>(&pDXGIAdapter1));
				if (pDXGIAdapter1) {
					// IDXGIFactory1
					pDXGIAdapter1->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void **>(&m_pDXGIFactory1));

					// Cleanup
					pDXGIAdapter1->Release();
					pDXGIDevice1->Release();
				} else {
					// Error!
					pDXGIDevice1->Release();
				}
			}
		}
		if (m_pDXGIFactory1) {
			// Get the first available adapter (video card)
			IDXGIAdapter *pAdapter = nullptr;
			hResult = m_pDXGIFactory1->EnumAdapters(0, &pAdapter);
			if (SUCCEEDED(hResult)) {
				// Get a description of the adapter (video card) and write it into the log
				DXGI_ADAPTER_DESC sAdapterDescription;
				if (SUCCEEDED(pAdapter->GetDesc(&sAdapterDescription)))
					PL_LOG(Info, String("Used D3D11 adapter (video card): ") + sAdapterDescription.Description)

				// Setup the renderer capabilities
				SetupCapabilities();

				// Set Z buffer bits and stencil buffer bits capabilities
				m_sCapabilities.nZBufferBits = nZBufferBits;
				m_sCapabilities.nStencilBits = nStencilBits;

				// Init data
				m_fDepthBoundsZMin = 0.0f;
				m_fDepthBoundsZMax = 0.0f;

				// Allocate sampler states
				m_ppnSamplerState = new uint32*[m_sCapabilities.nMaxTextureUnits];
				for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++) {
					m_ppnSamplerState[i] = new uint32[PLRenderer::Sampler::Number];
					MemoryManager::Set(m_ppnSamplerState[i], -1, sizeof(uint32)*PLRenderer::Sampler::Number);
				}

				// Allocate current stuff
				m_ppCurrentTextureBuffer = new PLRenderer::TextureBuffer*[m_sCapabilities.nMaxTextureUnits];
				MemoryManager::Set(m_ppCurrentTextureBuffer, 0, sizeof(PLRenderer::TextureBuffer**)*m_sCapabilities.nMaxTextureUnits);

				// Reset render
				Reset();
			} else {
				// Error!
				PL_LOG(Error, "D3D11: Failed to receive the first available adapter: " + SystemWindows::ErrorCodeToString(hResult))
			}
		} else {
			// Error!
			PL_LOG(Error, "D3D11: Failed to receive the DXGI factory 1 instance of the used device: " + SystemWindows::ErrorCodeToString(hResult))
		}
	} else {
		// Error!
		PL_LOG(Error, "D3D11: Failed to create the D3D11 device instance: " + SystemWindows::ErrorCodeToString(hResult) + " (DirectX end-user runtime installed?)")
	}
}

/**
*  @brief
*    Destructor
*/
Renderer::~Renderer()
{
	// Output log information
	PL_LOG(Info, "De-initialize D3D11 renderer")

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

	// Free sampler states
	if (m_ppnSamplerState) {
		for (uint32 i=0; i<m_sCapabilities.nMaxTextureUnits; i++)
			delete [] m_ppnSamplerState[i];
		delete [] m_ppnSamplerState;
		m_ppnSamplerState = nullptr;
	}

	// Free current stuff
	if (m_ppCurrentTextureBuffer) {
		delete [] m_ppCurrentTextureBuffer;
		m_ppCurrentTextureBuffer = nullptr;
	}

	// Release the D3D11 device
	if (m_pD3D11Device)
		m_pD3D11Device->Release();

	// Release the D3D11 device context
	if (m_pD3D11DeviceContext)
		m_pD3D11DeviceContext->Release();

	// Release the DXGI factory 1 instance
	if (m_pDXGIFactory1)
		m_pDXGIFactory1->Release();
}

/**
*  @brief
*    Returns the DXGI factory 1 instance
*/
IDXGIFactory1 *Renderer::GetDXGIFactory1() const
{
	return m_pDXGIFactory1;
}

/**
*  @brief
*    Returns the used feature level
*/
D3D_FEATURE_LEVEL Renderer::GetD3DFeatureLevel() const
{
	return m_nD3DFeatureLevel;
}

/**
*  @brief
*    Returns the used D3D11 device context
*/
ID3D11DeviceContext	*Renderer::GetD3D11DeviceContext() const
{
	return m_pD3D11DeviceContext;
}

/**
*  @brief
*    Returns the used D3D11 device
*/
ID3D11Device *Renderer::GetD3D11Device() const
{
	return m_pD3D11Device;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Setup the renderer capabilities
*/
void Renderer::SetupCapabilities()
{
	// Total available GPU memory in kilobytes
	m_sCapabilities.nTotalAvailableGPUMemory = 0;
	{ // Get the used DXGI factory 1 instance
		// IDXGIDevice1
		IDXGIDevice1 *pDXGIDevice1 = nullptr;
		m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void **>(&pDXGIDevice1));
		if (pDXGIDevice1) {
			// IDXGIAdapter1
			IDXGIAdapter1 *pDXGIAdapter1 = nullptr;
			pDXGIDevice1->GetParent(__uuidof(IDXGIAdapter1), reinterpret_cast<void **>(&pDXGIAdapter1));
			if (pDXGIAdapter1) {
				DXGI_ADAPTER_DESC sDXGIAdapterDesc;
				pDXGIAdapter1->GetDesc(&sDXGIAdapterDesc);
				m_sCapabilities.nTotalAvailableGPUMemory = sDXGIAdapterDesc.DedicatedVideoMemory;

				// Cleanup
				pDXGIAdapter1->Release();
				pDXGIDevice1->Release();
			} else {
				// Error!
				pDXGIDevice1->Release();
			}
		}
	}

	// [TODO] Check those "?"-points
	// The capabilities are depending on the feature level - data from: http://msdn.microsoft.com/en-us/library/ff476876%28v=vs.85%29.aspx
	switch (m_nD3DFeatureLevel) {
		case D3D_FEATURE_LEVEL_9_1:
			PL_LOG(Info, "D3D: Using feature level: 9_1")
			m_sCapabilities.nMaxColorRenderTargets			= 1;
			m_sCapabilities.nMaxTextureUnits				= 16;		// ?Max Input Slots?
			m_sCapabilities.nMaxAnisotropy					= 2;
			m_sCapabilities.nMaxTessellationFactor			= 0;		// ?
			m_sCapabilities.nMaxTextureBufferSize			= 2048;
			m_sCapabilities.bTextureBufferNonPowerOfTwo		= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.bTextureBuffer2DArray			= false;	// [TODO] 2D array texture buffer support
			m_sCapabilities.nMaxTextureBuffer2DArrayLayers	= 0;		// [TODO] 2D array texture buffer support
			m_sCapabilities.bTextureBufferRectangle			= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.nMaxRectangleTextureBufferSize	= 2048;
			m_sCapabilities.bTextureBuffer3D				= true;
			m_sCapabilities.nMax3DTextureBufferSize			= 256;
			m_sCapabilities.bTextureBufferCube				= true;
			m_sCapabilities.nMaxCubeTextureBufferSize		= 512;
			m_sCapabilities.bStencilWrap					= 0;		// ?
			m_sCapabilities.bTwoSidedStencils				= 0;		// ?
			m_sCapabilities.bDepthBoundsTest				= 0;		// ?
			m_sCapabilities.bPointSprite					= 0;		// ?
			m_sCapabilities.bPointParameters				= 0;		// ?
			m_sCapabilities.bOcclusionQuery					= false;
			m_sCapabilities.bVertexBufferSecondaryColor		= 0;		// ?
			m_sCapabilities.nZBufferBits					= 0;		// ?
			m_sCapabilities.nStencilBits					= 0;		// ?
			m_sCapabilities.nMultisampleAntialiasingSamples	= 0;		// ?
			m_sCapabilities.bInstancing						= false;	// [TODO] Geometric primitive instancing
			break;

		case D3D_FEATURE_LEVEL_9_2:
			PL_LOG(Info, "D3D: Using feature level: 9_2")
			m_sCapabilities.nMaxColorRenderTargets			= 1;
			m_sCapabilities.nMaxTextureUnits				= 16;		// ?Max Input Slots?
			m_sCapabilities.nMaxAnisotropy					= 16;
			m_sCapabilities.nMaxTessellationFactor			= 0;		// ?
			m_sCapabilities.nMaxTextureBufferSize			= 2048;
			m_sCapabilities.bTextureBufferNonPowerOfTwo		= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.bTextureBuffer2DArray			= false;	// [TODO] 2D array texture buffer support
			m_sCapabilities.nMaxTextureBuffer2DArrayLayers	= 0;		// [TODO] 2D array texture buffer support
			m_sCapabilities.bTextureBufferRectangle			= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.nMaxRectangleTextureBufferSize	= 2048;
			m_sCapabilities.bTextureBuffer3D				= true;
			m_sCapabilities.nMax3DTextureBufferSize			= 256;
			m_sCapabilities.bTextureBufferCube				= true;
			m_sCapabilities.nMaxCubeTextureBufferSize		= 512;
			m_sCapabilities.bStencilWrap					= 0;		// ?
			m_sCapabilities.bTwoSidedStencils				= 0;		// ?
			m_sCapabilities.bDepthBoundsTest				= 0;		// ?
			m_sCapabilities.bPointSprite					= 0;		// ?
			m_sCapabilities.bPointParameters				= 0;		// ?
			m_sCapabilities.bOcclusionQuery					= true;
			m_sCapabilities.bVertexBufferSecondaryColor		= 0;		// ?
			m_sCapabilities.nZBufferBits					= 0;		// ?
			m_sCapabilities.nStencilBits					= 0;		// ?
			m_sCapabilities.nMultisampleAntialiasingSamples	= 0;		// ?
			m_sCapabilities.bInstancing						= false;	// [TODO] Geometric primitive instancing
			break;

		case D3D_FEATURE_LEVEL_9_3:
			PL_LOG(Info, "D3D: Using feature level: 9_3")
			m_sCapabilities.nMaxColorRenderTargets			= 4;
			m_sCapabilities.nMaxTextureUnits				= 16;		// ?Max Input Slots?
			m_sCapabilities.nMaxAnisotropy					= 16;
			m_sCapabilities.nMaxTessellationFactor			= 0;		// ?
			m_sCapabilities.nMaxTextureBufferSize			= 4096;
			m_sCapabilities.bTextureBufferNonPowerOfTwo		= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.bTextureBuffer2DArray			= false;	// [TODO] 2D array texture buffer support
			m_sCapabilities.nMaxTextureBuffer2DArrayLayers	= 0;		// [TODO] 2D array texture buffer support
			m_sCapabilities.bTextureBufferRectangle			= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.nMaxRectangleTextureBufferSize	= 4096;
			m_sCapabilities.bTextureBuffer3D				= true;
			m_sCapabilities.nMax3DTextureBufferSize			= 256;
			m_sCapabilities.bTextureBufferCube				= true;
			m_sCapabilities.nMaxCubeTextureBufferSize		= 4096;
			m_sCapabilities.bStencilWrap					= 0;		// ?
			m_sCapabilities.bTwoSidedStencils				= 0;		// ?
			m_sCapabilities.bDepthBoundsTest				= 0;		// ?
			m_sCapabilities.bPointSprite					= 0;		// ?
			m_sCapabilities.bPointParameters				= 0;		// ?
			m_sCapabilities.bOcclusionQuery					= true;
			m_sCapabilities.bVertexBufferSecondaryColor		= 0;		// ?
			m_sCapabilities.nZBufferBits					= 0;		// ?
			m_sCapabilities.nStencilBits					= 0;		// ?
			m_sCapabilities.nMultisampleAntialiasingSamples	= 0;		// ?
			m_sCapabilities.bInstancing						= false;	// [TODO] Geometric primitive instancing
			break;

		case D3D_FEATURE_LEVEL_10_0:
			PL_LOG(Info, "D3D: Using feature level: 10_0")
			m_sCapabilities.nMaxColorRenderTargets			= 8;
			m_sCapabilities.nMaxTextureUnits				= 16;		// ?Max Input Slots?
			m_sCapabilities.nMaxAnisotropy					= 16;
			m_sCapabilities.nMaxTessellationFactor			= 0;		// ?
			m_sCapabilities.nMaxTextureBufferSize			= 8192;
			m_sCapabilities.bTextureBufferNonPowerOfTwo		= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.bTextureBuffer2DArray			= false;	// [TODO] 2D array texture buffer support
			m_sCapabilities.nMaxTextureBuffer2DArrayLayers	= 0;		// [TODO] 2D array texture buffer support
			m_sCapabilities.bTextureBufferRectangle			= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.nMaxRectangleTextureBufferSize	= 8192;
			m_sCapabilities.bTextureBuffer3D				= true;
			m_sCapabilities.nMax3DTextureBufferSize			= 2048;
			m_sCapabilities.bTextureBufferCube				= true;
			m_sCapabilities.nMaxCubeTextureBufferSize		= 8192;
			m_sCapabilities.bStencilWrap					= 0;		// ?
			m_sCapabilities.bTwoSidedStencils				= 0;		// ?
			m_sCapabilities.bDepthBoundsTest				= 0;		// ?
			m_sCapabilities.bPointSprite					= 0;		// ?
			m_sCapabilities.bPointParameters				= 0;		// ?
			m_sCapabilities.bOcclusionQuery					= true;
			m_sCapabilities.bVertexBufferSecondaryColor		= 0;		// ?
			m_sCapabilities.nZBufferBits					= 0;		// ?
			m_sCapabilities.nStencilBits					= 0;		// ?
			m_sCapabilities.nMultisampleAntialiasingSamples	= 0;		// ?
			m_sCapabilities.bInstancing						= false;	// [TODO] Geometric primitive instancing
			break;

		case D3D_FEATURE_LEVEL_10_1:
			PL_LOG(Info, "D3D: Using feature level: 10_1")
			m_sCapabilities.nMaxColorRenderTargets			= 8;
			m_sCapabilities.nMaxTextureUnits				= 32;		// ?Max Input Slots?
			m_sCapabilities.nMaxAnisotropy					= 16;
			m_sCapabilities.nMaxTessellationFactor			= 0;		// ?
			m_sCapabilities.nMaxTextureBufferSize			= 8192;
			m_sCapabilities.bTextureBufferNonPowerOfTwo		= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.bTextureBuffer2DArray			= false;	// [TODO] 2D array texture buffer support
			m_sCapabilities.nMaxTextureBuffer2DArrayLayers	= 0;		// [TODO] 2D array texture buffer support
			m_sCapabilities.bTextureBufferRectangle			= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.nMaxRectangleTextureBufferSize	= 8192;
			m_sCapabilities.bTextureBuffer3D				= true;
			m_sCapabilities.nMax3DTextureBufferSize			= 2048;
			m_sCapabilities.bTextureBufferCube				= true;
			m_sCapabilities.nMaxCubeTextureBufferSize		= 8192;
			m_sCapabilities.bStencilWrap					= 0;		// ?
			m_sCapabilities.bTwoSidedStencils				= 0;		// ?
			m_sCapabilities.bDepthBoundsTest				= 0;		// ?
			m_sCapabilities.bPointSprite					= 0;		// ?
			m_sCapabilities.bPointParameters				= 0;		// ?
			m_sCapabilities.bOcclusionQuery					= true;
			m_sCapabilities.bVertexBufferSecondaryColor		= 0;		// ?
			m_sCapabilities.nZBufferBits					= 0;		// ?
			m_sCapabilities.nStencilBits					= 0;		// ?
			m_sCapabilities.nMultisampleAntialiasingSamples	= 0;		// ?
			m_sCapabilities.bInstancing						= false;	// [TODO] Geometric primitive instancing
			break;

		case D3D_FEATURE_LEVEL_11_0:
			PL_LOG(Info, "D3D: Using feature level: 11_0")
			m_sCapabilities.nMaxColorRenderTargets			= 8;
			m_sCapabilities.nMaxTextureUnits				= 32;		// ?Max Input Slots?
			m_sCapabilities.nMaxAnisotropy					= 16;
			m_sCapabilities.nMaxTessellationFactor			= 0;		// ?
			m_sCapabilities.nMaxTextureBufferSize			= 16384;
			m_sCapabilities.bTextureBufferNonPowerOfTwo		= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.bTextureBuffer2DArray			= false;	// [TODO] 2D array texture buffer support
			m_sCapabilities.nMaxTextureBuffer2DArrayLayers	= 0;		// [TODO] 2D array texture buffer support
			m_sCapabilities.bTextureBufferRectangle			= true;		// Nonpowers-of-2 conditionally
			m_sCapabilities.nMaxRectangleTextureBufferSize	= 16384;
			m_sCapabilities.bTextureBuffer3D				= true;
			m_sCapabilities.nMax3DTextureBufferSize			= 2048;
			m_sCapabilities.bTextureBufferCube				= true;
			m_sCapabilities.nMaxCubeTextureBufferSize		= 8192;
			m_sCapabilities.bStencilWrap					= 0;		// ?
			m_sCapabilities.bTwoSidedStencils				= 0;		// ?
			m_sCapabilities.bDepthBoundsTest				= 0;		// ?
			m_sCapabilities.bPointSprite					= 0;		// ?
			m_sCapabilities.bPointParameters				= 0;		// ?
			m_sCapabilities.bOcclusionQuery					= true;
			m_sCapabilities.bVertexBufferSecondaryColor		= 0;		// ?
			m_sCapabilities.nZBufferBits					= 0;		// ?
			m_sCapabilities.nStencilBits					= 0;		// ?
			m_sCapabilities.nMultisampleAntialiasingSamples	= 0;		// ?
			m_sCapabilities.bInstancing						= false;	// [TODO] Geometric primitive instancing
			break;

		default:
			// ?!
			m_sCapabilities.nMaxColorRenderTargets			= 0;
			m_sCapabilities.nMaxTextureUnits				= 0;
			m_sCapabilities.nMaxAnisotropy					= 0;
			m_sCapabilities.nMaxTessellationFactor			= 0;
			m_sCapabilities.nMaxTextureBufferSize			= 0;
			m_sCapabilities.bTextureBufferNonPowerOfTwo		= 0;
			m_sCapabilities.bTextureBuffer2DArray			= false;
			m_sCapabilities.nMaxTextureBuffer2DArrayLayers	= 0;		// [TODO] 2D array texture buffer support
			m_sCapabilities.bTextureBufferRectangle			= 0;
			m_sCapabilities.nMaxRectangleTextureBufferSize	= 0;
			m_sCapabilities.bTextureBuffer3D				= 0;
			m_sCapabilities.nMax3DTextureBufferSize			= 0;
			m_sCapabilities.bTextureBufferCube				= 0;
			m_sCapabilities.nMaxCubeTextureBufferSize		= 0;
			m_sCapabilities.bStencilWrap					= 0;
			m_sCapabilities.bTwoSidedStencils				= 0;
			m_sCapabilities.bDepthBoundsTest				= 0;
			m_sCapabilities.bPointSprite					= 0;
			m_sCapabilities.bPointParameters				= 0;
			m_sCapabilities.bOcclusionQuery					= 0;
			m_sCapabilities.bVertexBufferSecondaryColor		= 0;
			m_sCapabilities.nZBufferBits					= 0;
			m_sCapabilities.nStencilBits					= 0;
			m_sCapabilities.nMultisampleAntialiasingSamples	= 0;
			m_sCapabilities.bInstancing						= false;
			break;
	}

	// Show renderer capabilities
	ShowRendererCapabilities();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Renderer functions         ]
//[-------------------------------------------------------]
bool Renderer::IsInitialized() const
{
	return (m_pDXGIFactory1 != nullptr && m_pD3D11DeviceContext != nullptr && m_pD3D11Device != nullptr);
}

String Renderer::GetAPI(uint32 *pnVersion) const
{
	// Get version
	if (pnVersion)
		*pnVersion = 0;

	// Get API
	static const String sString = "D3D11";
	return sString;
}

String Renderer::GetVendor() const
{
	static const String sString = "The PixelLight Team";
	return sString;
}

String Renderer::GetDefaultShaderLanguage() const
{
	static const String sString; // No support for shader languages
	return sString;
}

PLRenderer::ShaderLanguage *Renderer::GetShaderLanguage(const String &sShaderLanguage)
{
	// No support for shader languages
	return nullptr;
}

PLRenderer::FixedFunctions *Renderer::GetFixedFunctions() const
{
	// There's no fixed functions support in D3D11
	return nullptr;
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
		// Error!
		return nullptr;
	}
}

PLRenderer::SurfaceTextureBuffer *Renderer::CreateSurfaceTextureBufferRectangle(const Vector2i &vSize, PLRenderer::TextureBuffer::EPixelFormat nFormat, uint32 nFlags, uint8 nMaxColorTargets)
{
	// Check maximum render targets and dimension
	if (nMaxColorTargets && nMaxColorTargets <= m_sCapabilities.nMaxColorRenderTargets && vSize.x && vSize.y &&
		vSize.x <= m_sCapabilities.nMaxRectangleTextureBufferSize && vSize.y <= m_sCapabilities.nMaxRectangleTextureBufferSize) {
		// Create and register renderer surface
		PLRenderer::TextureBuffer *pTextureBuffer = new TextureBufferRectangle(*this, vSize, nFormat, PLRenderer::TextureBuffer::RenderTarget);
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
		return nullptr; // Error!

	// Create the D3D11 1D texture buffer
	return new TextureBuffer1D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer2D *Renderer::CreateTextureBuffer2D(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer2D(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the D3D11 2D texture buffer
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
		return nullptr; // Error!

	// Create the D3D11 rectangle texture buffer
	return new TextureBufferRectangle(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer3D *Renderer::CreateTextureBuffer3D(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer3D(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the D3D11 3D texture buffer
	return new TextureBuffer3D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBufferCube *Renderer::CreateTextureBufferCube(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBufferCube(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the D3D11 cube texture buffer
	return new TextureBufferCube(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::IndexBuffer *Renderer::CreateIndexBuffer()
{
	// Create the D3D11 index buffer
	return new IndexBuffer(*this);
}

PLRenderer::VertexBuffer *Renderer::CreateVertexBuffer()
{
	// Create the D3D11 vertex buffer
	return new VertexBuffer(*this);
}

PLRenderer::OcclusionQuery *Renderer::CreateOcclusionQuery()
{
	// Create the D3D11 occlusion query
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
		// Set the render state
		m_nRenderState[nState] = nValue;
		m_sStatistics.nRenderStateChanges++;
	}

	// All went fine
	return true;
}

bool Renderer::SetSamplerState(uint32 nStage, PLRenderer::Sampler::Enum nState, uint32 nValue)
{
	// Check if the stage is correct and check if the state is a valid sampler member
	if (nStage < m_sCapabilities.nMaxTextureUnits && nState < PLRenderer::Sampler::Number) {
		// Check if this sampler state is already set to this value
		if (m_ppnSamplerState[nStage][nState] != nValue) {
			// Set the sampler state
			m_ppnSamplerState[nStage][nState] = nValue;
			m_sStatistics.nSamplerStateChanges++;
		}

		// All went fine
		return true;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
bool Renderer::BeginScene()
{
	// Done
	return true;
}

bool Renderer::EndScene()
{
	// Done
	return true;
}

bool Renderer::SetViewport(const PLMath::Rectangle *pRectangle, float fMinZ, float fMaxZ)
{
	// Call base implementation
	PLRenderer::RendererBackend::SetViewport(pRectangle);

	// Set viewport and depth range
	if (m_pD3D11DeviceContext) {
		D3D11_VIEWPORT sD3D11Viewport;
		sD3D11Viewport.TopLeftX	= m_cViewportRect.GetX();
		sD3D11Viewport.TopLeftY	= m_cViewportRect.GetY();
		sD3D11Viewport.Width	= m_cViewportRect.GetWidth();
		sD3D11Viewport.Height	= m_cViewportRect.GetHeight();
		sD3D11Viewport.MinDepth	= fMinZ;
		sD3D11Viewport.MaxDepth	= fMaxZ;
		m_pD3D11DeviceContext->RSSetViewports(1, &sD3D11Viewport);
	}

	// Done
	return true;
}

bool Renderer::GetDepthBounds(float &fZMin, float &fZMax) const
{
	// Get data
	fZMin = m_fDepthBoundsZMin;
	fZMax = m_fDepthBoundsZMax;

	// Done
	return true;
}

bool Renderer::SetDepthBounds(float fZMin, float fZMax)
{
	// Set data
	m_fDepthBoundsZMin = fZMin;
	m_fDepthBoundsZMax = fZMax;

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

	// Done
	return true;
}

bool Renderer::Clear(uint32 nFlags, const Color4 &cColor, float fZ, uint32 nStencil)
{
	if (m_pD3D11DeviceContext && m_pD3D11RenderTargetView)
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pD3D11RenderTargetView, cColor.fColor);

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

				// Get the D3D11 render target view
				m_pD3D11RenderTargetView = static_cast<SurfaceWindow*>(pSurface)->GetD3D11RenderTargetView();
			}

			// Make the surface to the current render target
			if (m_cCurrentSurface.GetSurface())
				UnmakeSurfaceCurrent(*m_cCurrentSurface.GetSurface());
			m_cCurrentSurface.SetSurface(pSurface);
			bResult = MakeSurfaceCurrent(*pSurface, nFace);
			if (bResult)
				m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pD3D11RenderTargetView, nullptr);
		} else {
			// There's currently no render target
			m_pD3D11DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
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
/*	// Check parameter
	if (!m_lstSurfaces.IsElement(pSurface))
		return true;
	if (m_cCurrentSurface.GetSurface())
		m_cCurrentSurface.GetSurface()->UnmakeCurrent();
	m_cCurrentSurface.SetSurface(pSurface);

	// Make the surface to the current render target
	const bool bError = pSurface->MakeCurrent(nFace);

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
		if (nStage >= m_sCapabilities.nMaxTextureUnits)
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
		// No index buffer is used
		m_pD3D11DeviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
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
	if (nNumVertices) {
		// Get number of primitives
		uint32 nPrimitiveCount;
		switch (nType) {
			case PLRenderer::Primitive::PointList:	   nPrimitiveCount = nNumVertices;   break;
			case PLRenderer::Primitive::LineList:	   nPrimitiveCount = nNumVertices-1; break;
			case PLRenderer::Primitive::LineStrip:	   nPrimitiveCount = nNumVertices-1; break;
			case PLRenderer::Primitive::TriangleList:  nPrimitiveCount = nNumVertices/3; break;
			case PLRenderer::Primitive::TriangleStrip: nPrimitiveCount = nNumVertices-2; break;
			case PLRenderer::Primitive::TriangleFan:   nPrimitiveCount = nNumVertices-2; break;
			default:								   return false; // Error!
		}

		// Update statistics
		m_sStatistics.nDrawPrimitivCalls++;
		m_sStatistics.nVertices  += nNumVertices;
		m_sStatistics.nTriangles += nPrimitiveCount;
	}

	// Done
	return true;
}

bool Renderer::DrawIndexedPrimitives(PLRenderer::Primitive::Enum nType, uint32 nMinIndex, uint32 nMaxIndex,
									 uint32 nStartIndex, uint32 nNumVertices)
{
	// Index buffer correct?
	if (!m_pCurrentIndexBuffer)
		return false; // Error!

	// Draw something?
	if (nNumVertices) {
		// Get number of primitives
		uint32 nPrimitiveCount;
		switch (nType) {
			case PLRenderer::Primitive::PointList:	   nPrimitiveCount = nNumVertices;   break;
			case PLRenderer::Primitive::LineList:	   nPrimitiveCount = nNumVertices-2; break;
			case PLRenderer::Primitive::LineStrip:	   nPrimitiveCount = nNumVertices-2; break;
			case PLRenderer::Primitive::TriangleList:  nPrimitiveCount = nNumVertices/3; break;
			case PLRenderer::Primitive::TriangleStrip: nPrimitiveCount = nNumVertices-2; break;
			case PLRenderer::Primitive::TriangleFan:   nPrimitiveCount = nNumVertices-2; break;
			default:								   return false; // Error!
		}

		// Update statistics
		m_sStatistics.nDrawPrimitivCalls++;
		m_sStatistics.nVertices  += nNumVertices;
		m_sStatistics.nTriangles += nPrimitiveCount;
	}

	// Done
	return true;
}

bool Renderer::DrawPatches(uint32 nVerticesPerPatch, uint32 nStartIndex, uint32 nNumVertices)
{
	// [TODO] Implement me

	// Error!
	return false;
}

bool Renderer::DrawIndexedPatches(uint32 nVerticesPerPatch, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices)
{
	// [TODO] Implement me

	// Error!
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
} // PLRendererD3D11
