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
#include <PLGraphics/Image/Image.h>
#include <PLRenderer/Renderer/SurfaceWindowHandler.h>
#include <PLRenderer/Renderer/Backend/DrawHelpersBackend.h>
#include <PLRenderer/Renderer/Backend/FontManagerBackend.h>
#include "PLRendererNull/SurfaceWindow.h"
#include "PLRendererNull/SurfaceTextureBuffer.h"
#include "PLRendererNull/TextureBuffer1D.h"
#include "PLRendererNull/TextureBuffer2D.h"
#include "PLRendererNull/TextureBuffer2DArray.h"
#include "PLRendererNull/TextureBufferRectangle.h"
#include "PLRendererNull/TextureBuffer3D.h"
#include "PLRendererNull/TextureBufferCube.h"
#include "PLRendererNull/IndexBuffer.h"
#include "PLRendererNull/VertexBuffer.h"
#include "PLRendererNull/OcclusionQuery.h"
#include "PLRendererNull/FixedFunctions.h"
#include "PLRendererNull/Renderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Renderer)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Renderer::Renderer(handle nNativeWindowHandle, EMode nMode, uint32 nZBufferBits, uint32 nStencilBits, uint32 nMultisampleAntialiasingSamples, String sDefaultShaderLanguage) : PLRenderer::RendererBackend(ModeFixedFunctions),	// Only fixed functions mode is supported... a kind of *g*
	m_pFixedFunctions(nullptr),
	m_pFontManager(new PLRenderer::FontManagerBackend(*this))
{
	// Ignore the given native window handle

	// This renderer implementation has no shader support at all, so ignore sDefaultShaderLanguage

	// Output log information
	PL_LOG(Info, "Initialize null renderer")

	// Set Z buffer bits and stencil buffer bits capabilities
	m_sCapabilities.nZBufferBits = nZBufferBits;
	m_sCapabilities.nStencilBits = nStencilBits;

	// Init data
	m_fDepthBoundsZMin = 0.0f;
	m_fDepthBoundsZMax = 0.0f;

	// Setup the renderer capabilities
	SetupCapabilities();

	// Fixed functions
	m_pFixedFunctions = new FixedFunctions(*this);

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
}

/**
*  @brief
*    Destructor
*/
Renderer::~Renderer()
{
	// Output log information
	PL_LOG(Info, "De-initialize null renderer")

	// Destroy the fixed functions implementation
	delete m_pFixedFunctions;

	// Destroy all renderer surfaces of this renderer
	while (m_lstSurfaces.GetNumOfElements())
		delete m_lstSurfaces[0];

	// Clear list of display modes
	for (uint32 i=0; i<m_lstDisplayModeList.GetNumOfElements(); i++)
		delete m_lstDisplayModeList[i];
	m_lstDisplayModeList.Clear();

	// Destroy the Null font manager
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

	// Maximum number of color render targets
	m_sCapabilities.nMaxColorRenderTargets = 1;

	// Maximum number of texture units
	m_sCapabilities.nMaxTextureUnits = 8;

	// Maximum anisotropy
	m_sCapabilities.nMaxAnisotropy = 16;

	// Maximum tessellation factor
	m_sCapabilities.nMaxTessellationFactor = 1;

	// Maximum texture buffer size
	m_sCapabilities.nMaxTextureBufferSize = 4096;

	// Non power of two texture buffers supported?
	m_sCapabilities.bTextureBufferNonPowerOfTwo = true;

	// 2D array texture buffer support
	m_sCapabilities.bTextureBuffer2DArray          = true;
	m_sCapabilities.nMaxTextureBuffer2DArrayLayers = 512;

	// Rectangle texture buffers supported?
	m_sCapabilities.bTextureBufferRectangle = true;

	// Maximum rectangle texture buffer size
	m_sCapabilities.nMaxRectangleTextureBufferSize = 4096;

	// 3D texture buffers supported?
	m_sCapabilities.bTextureBuffer3D = true;

	// Maximum 3D texture buffer size
	m_sCapabilities.nMax3DTextureBufferSize = 512;

	// Cube texture buffers supported?
	m_sCapabilities.bTextureBufferCube = true;

	// Maximum cube texture buffer size
	m_sCapabilities.nMaxCubeTextureBufferSize = 2048;

	// Stencil wrap supported?
	m_sCapabilities.bStencilWrap = false;

	// Two sided stencils supported?
	m_sCapabilities.bTwoSidedStencils = false;

	// Depth bounds test supported?
	m_sCapabilities.bDepthBoundsTest = true;

	// Point sprite supported?
	m_sCapabilities.bPointSprite = true;

	// Point parameters supported?
	m_sCapabilities.bPointParameters = true;

	// Occlusion query supported
	m_sCapabilities.bOcclusionQuery = false;

	// Vertex buffer secondary color supported?
	m_sCapabilities.bVertexBufferSecondaryColor = true;

	// Multisample antialiasing samples
	m_sCapabilities.nMultisampleAntialiasingSamples = 0;

	// Geometric primitive instancing
	m_sCapabilities.bInstancing = true;

	// Show renderer capabilities
	ShowRendererCapabilities();
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Renderer functions         ]
//[-------------------------------------------------------]
bool Renderer::IsInitialized() const
{
	return true;
}

String Renderer::GetAPI(uint32 *pnVersion) const
{
	// Get version
	if (pnVersion)
		*pnVersion = 0;

	// Get API
	static const String sString = "Null";
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
	if (nMaxColorTargets && nMaxColorTargets <= m_sCapabilities.nMaxColorRenderTargets && IsValidTextureBufferRectangleSize(vSize.x) && IsValidTextureBufferRectangleSize(vSize.y)) {
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

	// Create the null 1D texture buffer
	return new TextureBuffer1D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer2D *Renderer::CreateTextureBuffer2D(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer2D(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the null 2D texture buffer
	return new TextureBuffer2D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer2DArray *Renderer::CreateTextureBuffer2DArray(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer2DArray(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the null 2D array texture buffer
	return new TextureBuffer2DArray(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer *Renderer::CreateTextureBufferRectangle(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!m_sCapabilities.bTextureBufferRectangle || !CheckTextureBufferRectangle(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the null rectangle texture buffer
	return new TextureBufferRectangle(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBuffer3D *Renderer::CreateTextureBuffer3D(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBuffer3D(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the null 3D texture buffer
	return new TextureBuffer3D(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::TextureBufferCube *Renderer::CreateTextureBufferCube(Image &cImage, PLRenderer::TextureBuffer::EPixelFormat nInternalFormat, uint32 nFlags)
{
	// Check texture buffer
	if (!CheckTextureBufferCube(cImage, nInternalFormat))
		return nullptr; // Error!

	// Create the null cube texture buffer
	return new TextureBufferCube(*this, cImage, nInternalFormat, nFlags);
}

PLRenderer::IndexBuffer *Renderer::CreateIndexBuffer()
{
	// Create the null index buffer
	return new IndexBuffer(*this);
}

PLRenderer::VertexBuffer *Renderer::CreateVertexBuffer()
{
	// Create the null vertex buffer
	return new VertexBuffer(*this);
}

PLRenderer::OcclusionQuery *Renderer::CreateOcclusionQuery()
{
	// Create the null occlusion query
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
	// There's nothing we can clear :D

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Get/set current resources                             ]
//[-------------------------------------------------------]
bool Renderer::SetRenderTarget(PLRenderer::Surface *pSurface, uint8 nFace)
{
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
			// Error!?!
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
	const bool bError = MakeSurfaceCurrent(*pSurface, nFace);

	// Setup viewport and scissor rectangle
	SetViewport();
	SetScissorRect();

	// Done
	return bError;
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
	}

	// Done
	return true;
}

bool Renderer::DrawPatches(uint32 nVerticesPerPatch, uint32 nStartIndex, uint32 nNumVertices)
{
	// [TODO] Calculate number of generated triangles?

	// Done
	return true;
}

bool Renderer::DrawIndexedPatches(uint32 nVerticesPerPatch, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices)
{
	// [TODO] Calculate number of generated triangles?

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Draw call with multiple primitive instances           ]
//[-------------------------------------------------------]
bool Renderer::DrawPrimitivesInstanced(PLRenderer::Primitive::Enum nType, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// Draw something?
	if (nNumVertices) {
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
	}

	// Done
	return true;
}

bool Renderer::DrawIndexedPrimitivesInstanced(PLRenderer::Primitive::Enum nType, uint32 nMinIndex, uint32 nMaxIndex, uint32 nStartIndex, uint32 nNumVertices, uint32 nNumOfInstances)
{
	// Index buffer correct?
	if (!m_pCurrentIndexBuffer)
		return false; // Error!

	// Draw something?
	if (nNumVertices) {
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
	}

	// Done
	return true;
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
} // PLRendererNull
