/*********************************************************\
 *  File: RendererBackend.cpp                            *
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
#include <PLCore/Tools/Profiling.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/Surface.h"
#include "PLRenderer/Renderer/IndexBuffer.h"
#include "PLRenderer/Renderer/VertexBuffer.h"
#include "PLRenderer/Renderer/TextureBuffer.h"
#include "PLRenderer/Renderer/FixedFunctions.h"
#include "PLRenderer/Renderer/Backend/DrawHelpersBackendShaders.h"
#include "PLRenderer/Renderer/Backend/DrawHelpersBackendFixedFunctions.h"
#include "PLRenderer/Renderer/Backend/RendererBackend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(RendererBackend)


//[-------------------------------------------------------]
//[ Internal helper function                              ]
//[-------------------------------------------------------]
const char *GetBoolString(bool bValue)
{
	return bValue ? "true" : "false";
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Makes a surface to the renderers current render target
*/
bool RendererBackend::MakeSurfaceCurrent(Surface &cSurface, uint8 nFace)
{
	return cSurface.MakeCurrent(nFace);
}

/**
*  @brief
*    Unmakes a surface from the renderers current render target
*/
bool RendererBackend::UnmakeSurfaceCurrent(Surface &cSurface)
{
	return cSurface.UnmakeCurrent();
}

/**
*  @brief
*    Makes a texture buffer to the renderers current texture buffer
*/
bool RendererBackend::MakeTextureBufferCurrent(TextureBuffer &cTextureBuffer, uint32 nStage)
{
	return cTextureBuffer.MakeCurrent(nStage);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RendererBackend::RendererBackend(EMode nMode) :
	m_pDrawHelpers(nullptr),
	m_nSwapInterval(1),
	m_nMode(nMode),
	m_pRendererContext(nullptr)
{
	// Create the draw helpers instance depending on the renderer mode
	switch (m_nMode) {
		case ModeFixedFunctions:
			m_pDrawHelpers = new DrawHelpersBackendFixedFunctions(*this);
			break;

		case ModeShaders:
			m_pDrawHelpers = new DrawHelpersBackendShaders(*this);
			break;

		case ModeBoth:
		default:
			// The default is shaders based because it's less painful
			m_pDrawHelpers = new DrawHelpersBackendShaders(*this);
			break;
	}

	// Create the renderer context this renderer instance is in, the renderer context will take
	// over the control of everything
	m_pRendererContext = new RendererContext(*this);

	// Init data
	MemoryManager::Set(&m_sCapabilities, 0, sizeof(Capabilities));
	MemoryManager::Set(&m_sStatistics,   0, sizeof(Statistics));
	m_ppDataBackup = nullptr;

	// Init color mask
	m_bColorMask[0] = true;
	m_bColorMask[1] = true;
	m_bColorMask[2] = true;
	m_bColorMask[3] = true;

	// States
	m_ppnSamplerState		  = nullptr;
	m_ppnInternalSamplerState = nullptr;
	{ // Set default renderer states
		m_nDefaultRenderState[RenderState::FillMode] = Fill::Solid;
		m_nDefaultRenderState[RenderState::CullMode] = Cull::CCW;
		// Z buffer
		m_nDefaultRenderState[RenderState::ZEnable]             = true;
		m_nDefaultRenderState[RenderState::ZWriteEnable]        = true;
		m_nDefaultRenderState[RenderState::ZFunc]               = Compare::LessEqual;
		m_nDefaultRenderState[RenderState::ZBias]               = Tools::FloatToUInt32(0.0f);
		m_nDefaultRenderState[RenderState::SlopeScaleDepthBias] = Tools::FloatToUInt32(0.0f);
		m_nDefaultRenderState[RenderState::DepthBias]           = Tools::FloatToUInt32(0.0f);
		// Blend
		m_nDefaultRenderState[RenderState::BlendEnable]  = false;
		m_nDefaultRenderState[RenderState::SrcBlendFunc] = BlendFunc::SrcAlpha;
		m_nDefaultRenderState[RenderState::DstBlendFunc] = BlendFunc::InvSrcAlpha;
		// Stencil
		m_nDefaultRenderState[RenderState::StencilEnable]       = false;
		m_nDefaultRenderState[RenderState::StencilFunc]         = Compare::Always;
		m_nDefaultRenderState[RenderState::StencilRef]          = 0;
		m_nDefaultRenderState[RenderState::StencilMask]         = 0xFFFFFFFF;
		m_nDefaultRenderState[RenderState::StencilFail]         = StencilOp::Keep;
		m_nDefaultRenderState[RenderState::StencilZFail]        = StencilOp::Keep;
		m_nDefaultRenderState[RenderState::StencilPass]         = StencilOp::Keep;
		m_nDefaultRenderState[RenderState::TwoSidedStencilMode] = false;
		m_nDefaultRenderState[RenderState::CCWStencilFunc]      = Compare::Always;
		m_nDefaultRenderState[RenderState::CCWStencilFail]      = StencilOp::Keep;
		m_nDefaultRenderState[RenderState::CCWStencilZFail]     = StencilOp::Keep;
		m_nDefaultRenderState[RenderState::CCWStencilPass]      = StencilOp::Keep;
		// Point and line
		m_nDefaultRenderState[RenderState::PointSize]        = Tools::FloatToUInt32(1.0f);
		m_nDefaultRenderState[RenderState::PointScaleEnable] = false;
		m_nDefaultRenderState[RenderState::PointSizeMin]     = Tools::FloatToUInt32(1.0f);
		m_nDefaultRenderState[RenderState::PointSizeMax]     = Tools::FloatToUInt32(64.0f);
		m_nDefaultRenderState[RenderState::PointScaleA]      = Tools::FloatToUInt32(1.0f);
		m_nDefaultRenderState[RenderState::PointScaleB]      = Tools::FloatToUInt32(0.0f);
		m_nDefaultRenderState[RenderState::PointScaleC]      = Tools::FloatToUInt32(0.0f);
		m_nDefaultRenderState[RenderState::LineWidth]        = Tools::FloatToUInt32(1.0f);
		// Tessellation
		m_nDefaultRenderState[RenderState::TessellationFactor] = 1;
		m_nDefaultRenderState[RenderState::TessellationMode]   = TessellationMode::Discrete;
		// Misc
		m_nDefaultRenderState[RenderState::PointSpriteEnable] = false;
		m_nDefaultRenderState[RenderState::DitherEnable]      = false;
		m_nDefaultRenderState[RenderState::ScissorTestEnable] = false;
		m_nDefaultRenderState[RenderState::MultisampleEnable] = true;
		m_nDefaultRenderState[RenderState::DepthClamp]        = false;
		m_nDefaultRenderState[RenderState::InvCullMode]       = false;
		m_nDefaultRenderState[RenderState::FixedFillMode]     = Fill::Unknown;
	}
	MemoryManager::Copy(m_nRenderState, m_nDefaultRenderState, sizeof(uint32)*RenderState::Number);
	{ // Set default sampler states
		// Address modes
		m_nDefaultSamplerState[Sampler::AddressU] = TextureAddressing::Wrap;
		m_nDefaultSamplerState[Sampler::AddressV] = TextureAddressing::Wrap;
		m_nDefaultSamplerState[Sampler::AddressW] = TextureAddressing::Wrap;
		// Filter
		m_nDefaultSamplerState[Sampler::MagFilter] = TextureFiltering::Linear;
		m_nDefaultSamplerState[Sampler::MinFilter] = TextureFiltering::Linear;
		m_nDefaultSamplerState[Sampler::MipFilter] = TextureFiltering::Linear;
		// Misc
		m_nDefaultSamplerState[Sampler::MipmapLODBias] = 0;
		m_nDefaultSamplerState[Sampler::MaxMapLevel]   = 1000;
		m_nDefaultSamplerState[Sampler::MaxAnisotropy] = 1;
	}

	// Current stuff
	m_fViewPortMinZ				= 0.0f;
	m_fViewPortMaxZ				= 0.0f;
	m_nCurrentSurfaceFace		= 0;
	m_ppCurrentTextureBuffer	= nullptr;
	m_pCurrentIndexBuffer		= nullptr;

	// The rest is done by the API backends!
}

/**
*  @brief
*    Destructor
*/
RendererBackend::~RendererBackend()
{
	// Destroy the draw helpers instance (usually already destroyed in a derived class to have a better shut down flow)
	if (m_pDrawHelpers)
		delete m_pDrawHelpers;

	// Destroy all renderer surfaces of this renderer
	while (m_lstSurfaces.GetNumOfElements())
		delete m_lstSurfaces[0];

	// Destroy all display modes
	for (uint32 i=0; i<m_lstDisplayModeList.GetNumOfElements(); i++)
		delete m_lstDisplayModeList[i];

	// Destroy all renderer resources of this renderer
	while (m_lstResources.GetNumOfElements())
		delete m_lstResources[0];

	// [NOTE] Index and vertex buffer will be destroyed through the renderer automatically
}

/**
*  @brief
*    Writes the renderer capabilities into the log
*/
void RendererBackend::ShowRendererCapabilities() const
{
	PL_LOG(Info, " ")
	PL_LOG(Info, "Renderer capabilities:")
	PL_LOG(Info, String("  TotalAvailableGPUMemory: ")		 + m_sCapabilities.nTotalAvailableGPUMemory + " KiB (" + m_sCapabilities.nTotalAvailableGPUMemory/1024 + " MiB)")
	PL_LOG(Info, String("  MaxColorRenderTargets: ")		 + m_sCapabilities.nMaxColorRenderTargets)
	PL_LOG(Info, String("  MaxTextureUnits: ")				 + m_sCapabilities.nMaxTextureUnits)
	PL_LOG(Info, String("  MaxAnisotropy: ")				 + static_cast<uint32>(m_sCapabilities.nMaxAnisotropy))
	PL_LOG(Info, String("  MaxTessellationFactor: ")		 + m_sCapabilities.nMaxTessellationFactor)
	PL_LOG(Info, String("  MaxTextureBufferSize: ")			 + static_cast<uint32>(m_sCapabilities.nMaxTextureBufferSize))
	PL_LOG(Info, String("  TextureBufferNonPowerOfTwo: ")	 + GetBoolString(m_sCapabilities.bTextureBufferNonPowerOfTwo))
	PL_LOG(Info, String("  TextureBuffer2DArray: ")			 + GetBoolString(m_sCapabilities.bTextureBuffer2DArray))
	PL_LOG(Info, String("  MaxTextureBuffer2DArrayLayers: ") + static_cast<uint32>(m_sCapabilities.nMaxTextureBuffer2DArrayLayers))
	PL_LOG(Info, String("  TextureBufferRectangle: ")		 + GetBoolString(m_sCapabilities.bTextureBufferRectangle))
	PL_LOG(Info, String("  MaxRectangleTextureBufferSize: ") + static_cast<uint32>(m_sCapabilities.nMaxRectangleTextureBufferSize))
	PL_LOG(Info, String("  TextureBuffer3D: ")				 + GetBoolString(m_sCapabilities.bTextureBuffer3D))
	PL_LOG(Info, String("  Max3DTextureBufferSize: ")		 + static_cast<uint32>(m_sCapabilities.nMax3DTextureBufferSize))
	PL_LOG(Info, String("  TextureBufferCube: ")			 + GetBoolString(m_sCapabilities.bTextureBufferCube))
	PL_LOG(Info, String("  MaxCubeTextureBufferSize: ")		 + static_cast<uint32>(m_sCapabilities.nMaxCubeTextureBufferSize))
	PL_LOG(Info, String("  StencilWrap: ")					 + GetBoolString(m_sCapabilities.bStencilWrap))
	PL_LOG(Info, String("  TwoSidedStencils: ")				 + GetBoolString(m_sCapabilities.bTwoSidedStencils))
	PL_LOG(Info, String("  DepthBoundsTest: ")				 + GetBoolString(m_sCapabilities.bDepthBoundsTest))
	PL_LOG(Info, String("  PointSprite: ")					 + GetBoolString(m_sCapabilities.bPointSprite))
	PL_LOG(Info, String("  PointParameters: ")				 + GetBoolString(m_sCapabilities.bPointParameters))
	PL_LOG(Info, String("  OcclusionQuery: ")				 + GetBoolString(m_sCapabilities.bOcclusionQuery))
	PL_LOG(Info, String("  VertexBufferSecondaryColor: ")	 + GetBoolString(m_sCapabilities.bVertexBufferSecondaryColor))
	PL_LOG(Info, String("  Instancing: ")					 + GetBoolString(m_sCapabilities.bInstancing))
	PL_LOG(Info, " ")

	// Fixed functions
	FixedFunctions *pFixedFunctions = GetFixedFunctions();
	if (pFixedFunctions) {
		PL_LOG(Info, "Renderer fixed functions capabilities:")
		PL_LOG(Info, String("  MaxActiveLights: ")			+ pFixedFunctions->GetCapabilities().nMaxActiveLights)
		PL_LOG(Info, String("  MaxClipPlanes: ")			+ pFixedFunctions->GetCapabilities().nMaxClipPlanes)
		PL_LOG(Info, String("  VertexBufferFogCoord: ")		+ GetBoolString(pFixedFunctions->GetCapabilities().bVertexBufferFogCoord))
		PL_LOG(Info, String("  MaxVertexBufferStreams: ")	+ pFixedFunctions->GetCapabilities().nMaxVertexBufferStreams)
		PL_LOG(Info, " ")
	}
}

bool RendererBackend::CheckTextureBuffer1D(Image &cImage, TextureBuffer::EPixelFormat nInternalFormat) const
{
	// Get the first image buffer
	ImageBuffer *pImageBuffer = cImage.GetBuffer();
	if (pImageBuffer) {
		// Valid dimension?
		uint32 nSize = pImageBuffer->GetSize().x;
		if (nSize) {
			if (nSize == 1)
				nSize = pImageBuffer->GetSize().y;
			else {
				if (pImageBuffer->GetSize().y != 1)
					return false; // Error!
			}
			return IsValidTextureBuffer1DSize(nSize);
		}
	}

	// Error!
	return false;
}

bool RendererBackend::CheckTextureBuffer2D(Image &cImage, TextureBuffer::EPixelFormat nInternalFormat) const
{
	// Get the first image buffer
	const ImageBuffer *pImageBuffer = cImage.GetBuffer();

	// Check image buffer dimension
	return (pImageBuffer && IsValidTextureBuffer2DSize(pImageBuffer->GetSize().x) && IsValidTextureBuffer2DSize(pImageBuffer->GetSize().y));
}

bool RendererBackend::CheckTextureBuffer2DArray(Image &cImage, TextureBuffer::EPixelFormat nInternalFormat) const
{
	// Get the first image buffer
	const ImageBuffer *pImageBuffer = cImage.GetBuffer();

	// Check image buffer dimension
	return (pImageBuffer && IsValidTextureBuffer2DSize(pImageBuffer->GetSize().x) && IsValidTextureBuffer2DSize(pImageBuffer->GetSize().y) &&
			pImageBuffer->GetSize().z <= m_sCapabilities.nMaxTextureBuffer2DArrayLayers && pImageBuffer->GetSize().z > 0);
}

bool RendererBackend::CheckTextureBufferRectangle(Image &cImage, TextureBuffer::EPixelFormat nInternalFormat) const
{
	// Get the first image buffer
	const ImageBuffer *pImageBuffer = cImage.GetBuffer();

	// Check image buffer dimension
	return (pImageBuffer && IsValidTextureBufferRectangleSize(pImageBuffer->GetSize().x) && IsValidTextureBufferRectangleSize(pImageBuffer->GetSize().y));
}

bool RendererBackend::CheckTextureBuffer3D(Image &cImage, TextureBuffer::EPixelFormat nInternalFormat) const
{
	// Get the first image buffer
	const ImageBuffer *pImageBuffer = cImage.GetBuffer();

	// Check image buffer dimension
	return (pImageBuffer && IsValidTextureBuffer3DSize(pImageBuffer->GetSize().x) && IsValidTextureBuffer3DSize(pImageBuffer->GetSize().y) && IsValidTextureBuffer3DSize(pImageBuffer->GetSize().z));
}

bool RendererBackend::CheckTextureBufferCube(Image &cImage, TextureBuffer::EPixelFormat nInternalFormat) const
{
	// Get the first image buffer
	const ImageBuffer *pImageBuffer = cImage.GetBuffer();

	// Check image buffer dimension
	return (pImageBuffer && pImageBuffer->GetSize().x == pImageBuffer->GetSize().y && IsValidTextureBufferCubeSize(pImageBuffer->GetSize().x));
}


//[-------------------------------------------------------]
//[ Public virtual Renderer functions                     ]
//[-------------------------------------------------------]
RendererContext &RendererBackend::GetRendererContext() const
{
	return *m_pRendererContext;
}

RendererBackend::EMode RendererBackend::GetMode() const
{
	return m_nMode;
}

DrawHelpers &RendererBackend::GetDrawHelpers() const
{
	return *m_pDrawHelpers;
}

void RendererBackend::BackupDeviceObjects()
{
	// Is there already a backup?
	if (!m_ppDataBackup) {
		// Allocate enough backup slots
		m_ppDataBackup = new uint8*[m_lstResources.GetNumOfElements()];

		// Backup all resource device data
		for (uint32 i=0; i<m_lstResources.GetNumOfElements(); i++)
			m_lstResources[i]->BackupDeviceData(&m_ppDataBackup[i]);

		// Backup all surface device data
		for (uint32 i=0; i<m_lstSurfaces.GetNumOfElements(); i++)
			m_lstSurfaces[i]->BackupDeviceData();
	}
}

void RendererBackend::RestoreDeviceObjects()
{
	// Is there a backup which can be restored?
	if (m_ppDataBackup) {
		// Restore all resource device data
		for (uint32 i=0; i<m_lstResources.GetNumOfElements(); i++)
			m_lstResources[i]->RestoreDeviceData(&m_ppDataBackup[i]);

		// Restore all surface device data
		for (uint32 i=0; i<m_lstSurfaces.GetNumOfElements(); i++)
			m_lstSurfaces[i]->RestoreDeviceData();

		// Destroy backup slots
		delete [] m_ppDataBackup;
		m_ppDataBackup = nullptr;
	}
}

uint32 RendererBackend::GetNumOfDisplayModes() const
{
	return m_lstDisplayModeList.GetNumOfElements();
}

const DisplayMode *RendererBackend::GetDisplayMode(uint32 nIndex) const
{
	return m_lstDisplayModeList[nIndex];
}

const Capabilities &RendererBackend::GetCapabilities() const
{
	return m_sCapabilities;
}

bool RendererBackend::IsValidTextureBuffer1DSize(int nSize) const
{
	return (nSize <= m_sCapabilities.nMaxTextureBufferSize && nSize > 0 && (GetCapabilities().bTextureBufferNonPowerOfTwo || Math::IsPowerOfTwo(nSize)));
}

bool RendererBackend::IsValidTextureBuffer2DSize(int nSize) const
{
	return (nSize <= m_sCapabilities.nMaxTextureBufferSize && nSize > 0 && (GetCapabilities().bTextureBufferNonPowerOfTwo || Math::IsPowerOfTwo(nSize)));
}

bool RendererBackend::IsValidTextureBufferRectangleSize(int nSize) const
{
	return (nSize <= m_sCapabilities.nMaxRectangleTextureBufferSize && nSize > 0);
}

bool RendererBackend::IsValidTextureBuffer3DSize(int nSize) const
{
	return (nSize <= m_sCapabilities.nMax3DTextureBufferSize && nSize > 0 && (GetCapabilities().bTextureBufferNonPowerOfTwo || Math::IsPowerOfTwo(nSize)));
}

bool RendererBackend::IsValidTextureBufferCubeSize(int nSize) const
{
	return (nSize <= m_sCapabilities.nMaxCubeTextureBufferSize && nSize > 0 && (GetCapabilities().bTextureBufferNonPowerOfTwo || Math::IsPowerOfTwo(nSize)));
}

const Statistics &RendererBackend::GetStatistics() const
{
	return m_sStatistics;
}

const Vector2 &RendererBackend::GetTexelToPixelOffset() const
{
	// It looks like that most renderer APIs don't have a texel offset, so this is probably the most useful default implementation
	return Vector2::Zero;
}

void RendererBackend::Update()
{
	// Update profiling
	Profiling *pProfiling = Profiling::GetInstance();
	if (pProfiling->IsActive()) {
		const String sAPI = GetAPI();
		const Statistics &sS = m_sStatistics;
		pProfiling->Set(sAPI, "Number of surfaces",				GetNumOfSurfaces());
		pProfiling->Set(sAPI, "Number of resources",			GetNumOfResources());
		pProfiling->Set(sAPI, "Render state changes",			sS.nRenderStateChanges);
		pProfiling->Set(sAPI, "Sampler state changes",			sS.nSamplerStateChanges);
		pProfiling->Set(sAPI, "Draw primitive calls",			sS.nDrawPrimitivCalls);
		pProfiling->Set(sAPI, "Current triangles",				sS.nTriangles);
		pProfiling->Set(sAPI, "Current vertices",				sS.nVertices);
		pProfiling->Set(sAPI, "Rendering time",					String::Format("%.3f ms",				sS.fRenderingTime));
		// Texture buffers
		pProfiling->Set(sAPI, "Number of texture buffers",		sS.nTextureBuffersNum);
		const float fTextureBuffersMemKB = static_cast<float>(sS.nTextureBuffersMem)/1024.0f;
		pProfiling->Set(sAPI, "Texture buffers memory",			String::Format("%g KB (%g MB)",			fTextureBuffersMemKB, fTextureBuffersMemKB/1024.0f));
		pProfiling->Set(sAPI, "Texture buffer binds",			sS.nTextureBufferBinds);
		// Vertex buffers
		pProfiling->Set(sAPI, "Number of vertex buffers",		sS.nVertexBufferNum);
		const float fVertexBufferMemKB = static_cast<float>(sS.nVertexBufferMem)/1024.0f;
		pProfiling->Set(sAPI, "Vertex buffers memory",			String::Format("%g KB (%g MB)",			fVertexBufferMemKB, fVertexBufferMemKB/1024.0f));
		pProfiling->Set(sAPI, "Vertex buffers update time",		String::Format("%.3f ms (%d locks)",	sS.nVertexBuffersSetupTime/1000.0f, sS.nVertexBufferLocks));
		// Index buffers
		pProfiling->Set(sAPI, "Number of index buffers",		sS.nIndexBufferNum);
		const float fIndexBufferMemKB = static_cast<float>(sS.nIndexBufferMem)/1024.0f;
		pProfiling->Set(sAPI, "Index buffers memory",			String::Format("%g KB (%g MB)",			fIndexBufferMemKB, fIndexBufferMemKB/1024.0f));
		pProfiling->Set(sAPI, "Index buffers update time",		String::Format("%.3f ms (%d locks)",	sS.nIndexBuffersSetupTime/1000.0f, sS.nIndexBufferLocks));
		// Uniform buffers
		pProfiling->Set(sAPI, "Number of uniform buffers",		sS.nUniformBufferNum);
		const float fUniformBufferMemKB = static_cast<float>(sS.nUniformBufferMem)/1024.0f;
		pProfiling->Set(sAPI, "Uniform buffers memory",			String::Format("%g KB (%g MB)",			fUniformBufferMemKB, fUniformBufferMemKB/1024.0f));
		pProfiling->Set(sAPI, "Uniform buffers update time",	String::Format("%.3f ms (%d locks)",	sS.nUniformBuffersSetupTime/1000.0f, sS.nUniformBufferLocks));
	}

	// Reset some statistics
	m_sStatistics.nRenderStateChanges		= 0;
	m_sStatistics.nSamplerStateChanges		= 0;
	m_sStatistics.nDrawPrimitivCalls		= 0;
	m_sStatistics.nVertices					= 0;
	m_sStatistics.nTriangles				= 0;
	m_sStatistics.fRenderingTime			= 0.0f;
	m_sStatistics.nTextureBufferBinds		= 0;
	m_sStatistics.nVertexBuffersSetupTime	= 0;
	m_sStatistics.nVertexBufferLocks		= 0;
	m_sStatistics.nIndexBuffersSetupTime	= 0;
	m_sStatistics.nIndexBufferLocks			= 0;
}

void RendererBackend::Reset()
{
	ResetRenderStates();
	ResetSamplerStates();
	SetTextureBuffer();
	SetIndexBuffer();
	SetViewport();
	SetScissorRect();
	SetColorMask();
	SetProgram();

	// Fixed functions
	FixedFunctions *pFixedFunctions = GetFixedFunctions();
	if (pFixedFunctions)
		pFixedFunctions->Reset();

	// Draw helpers
	GetDrawHelpers().End2DMode();
	GetDrawHelpers().Set2DZValue();
}


//[-------------------------------------------------------]
//[ Surfaces                                              ]
//[-------------------------------------------------------]
uint32 RendererBackend::GetNumOfSurfaces() const
{
	return m_lstSurfaces.GetNumOfElements();
}

Surface *RendererBackend::GetSurface(uint32 nIndex) const
{
	return m_lstSurfaces[nIndex];
}

bool RendererBackend::AddSurface(Surface &cSurface)
{
	return (m_lstSurfaces.Add(&cSurface) != nullptr);
}

bool RendererBackend::RemoveSurface(Surface &cSurface)
{
	// Check whether this surface is the current render target
	if (GetRenderTarget() == &cSurface) {
		// Set an alternative surface ('>1' because 0 is the internal surface)
		SetRenderTarget(m_lstSurfaces.GetNumOfElements() > 1 ? m_lstSurfaces[0] : nullptr);
	}

	// Remove the surface
	return m_lstSurfaces.Remove(&cSurface);
}

SurfacePainter *RendererBackend::CreateSurfacePainter(const String &sClass)
{
	// Get class and check it
	const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
	if (pClass && pClass->IsDerivedFrom("PLRenderer::SurfacePainter")) {
		// Create the surface painter and return it
		return reinterpret_cast<SurfacePainter*>(pClass->Create(Params<Object*, Renderer&>(*this)));
	} else {
		// Error!
		return nullptr;
	}
}


//[-------------------------------------------------------]
//[ Resources                                             ]
//[-------------------------------------------------------]
uint32 RendererBackend::GetNumOfResources() const
{
	return m_lstResources.GetNumOfElements();
}

Resource *RendererBackend::GetResource(uint32 nIndex) const
{
	return m_lstResources[nIndex];
}

bool RendererBackend::AddResource(Resource &cResource)
{
	return (m_lstResources.Add(&cResource) != nullptr);
}

bool RendererBackend::RemoveResource(Resource &cResource)
{
	return m_lstResources.Remove(&cResource);
}


//[-------------------------------------------------------]
//[ States                                                ]
//[-------------------------------------------------------]
// Render states
uint32 RendererBackend::GetDefaultRenderState(RenderState::Enum nState) const
{
	return (nState < RenderState::Number) ? m_nDefaultRenderState[nState] : 0;
}

void RendererBackend::ResetRenderStates()
{
	// Set renderer states to this default settings
	for (uint32 i=0; i<RenderState::Number; i++)
		SetRenderState(static_cast<RenderState::Enum>(i), m_nDefaultRenderState[i]);
}

int RendererBackend::GetRenderState(RenderState::Enum nState) const
{
	// Check if the state is a valid render state member
	return (nState < RenderState::Number) ? m_nRenderState[nState] : -1;
}

// Sampler states
uint32 RendererBackend::GetDefaultSamplerState(Sampler::Enum nState) const
{
	return (nState < Sampler::Number) ? m_nDefaultSamplerState[nState] : 0;
}

void RendererBackend::ResetSamplerStates()
{
	// Set sampler states to this default settings
	for (uint32 nStage=0; nStage<m_sCapabilities.nMaxTextureUnits; nStage++) {
		for (uint32 i=0; i<Sampler::Number; i++)
			SetSamplerState(nStage, static_cast<Sampler::Enum>(i), m_nDefaultSamplerState[i]);
	}
}

int RendererBackend::GetSamplerState(uint32 nStage, Sampler::Enum nState) const
{
	// Check if the stage is correct and check if the state is a valid sampler member
	return (nStage < m_sCapabilities.nMaxTextureUnits && nState < Sampler::Number) ? m_ppnSamplerState[nStage][nState] : -1;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
uint32 RendererBackend::GetSwapInterval() const
{
	return m_nSwapInterval;
}

void RendererBackend::SetSwapInterval(uint32 nSwapInterval)
{
	m_nSwapInterval = nSwapInterval;
}

const Rectangle &RendererBackend::GetViewport(float *pfMinZ, float *pfMaxZ) const
{
	if (pfMinZ)
		*pfMinZ = m_fViewPortMinZ;
	if (pfMaxZ)
		*pfMaxZ = m_fViewPortMaxZ;
	return m_cViewportRect;
}

bool RendererBackend::SetViewport(const Rectangle *pRectangle, float fMinZ, float fMaxZ)
{
	// Set data
	if (pRectangle) {
		if (pRectangle->vMin.x > 0.0f)
			m_cViewportRect.vMin.x = pRectangle->vMin.x;
		else
			m_cViewportRect.vMin.x = 0.0f;
		if (pRectangle->vMin.y > 0.0f)
			m_cViewportRect.vMin.y = pRectangle->vMin.y;
		else
			m_cViewportRect.vMin.y = 0.0f;
		Surface *pSurface = m_cCurrentSurface.GetSurface();
		if (pSurface) {
			if (pRectangle->vMax.x > 0.0f)
				m_cViewportRect.vMax.x = pRectangle->vMax.x;
			else
				m_cViewportRect.vMax.x = m_cViewportRect.vMin.x+pSurface->GetSize().x;
			if (pRectangle->vMax.y > 0.0f)
				m_cViewportRect.vMax.y = pRectangle->vMax.y;
			else
				m_cViewportRect.vMax.y = m_cViewportRect.vMin.y+pSurface->GetSize().y;
		} else {
			if (pRectangle->vMax.x > 0.0f)
				m_cViewportRect.vMax.x = pRectangle->vMax.x;
			else
				m_cViewportRect.vMax.x = m_cViewportRect.vMin.x;
			if (pRectangle->vMax.y > 0.0f)
				m_cViewportRect.vMax.y = pRectangle->vMax.y;
			else
				m_cViewportRect.vMax.y = m_cViewportRect.vMin.y;
		}
	} else {
		m_cViewportRect.vMin.x = 0;
		m_cViewportRect.vMin.y = 0;
		Surface *pSurface = m_cCurrentSurface.GetSurface();
		if (pSurface) {
			m_cViewportRect.vMax.x = static_cast<float>(pSurface->GetSize().x);
			m_cViewportRect.vMax.y = static_cast<float>(pSurface->GetSize().y);
		} else {
			m_cViewportRect.vMax.x = 0;
			m_cViewportRect.vMax.y = 0;
		}
	}
	m_fViewPortMinZ = fMinZ;
	m_fViewPortMaxZ = fMaxZ;

	// Done
	return true;
}

const Rectangle &RendererBackend::GetScissorRect() const
{
	return m_cScissorRect;
}

bool RendererBackend::SetScissorRect(const Rectangle *pRectangle)
{
	// Set data
	if (pRectangle) {
		if (pRectangle->vMin.x > 0.0f)
			m_cScissorRect.vMin.x = pRectangle->vMin.x;
		else
			m_cScissorRect.vMin.x = m_cViewportRect.vMin.x;
		if (pRectangle->vMin.y > 0.0f)
			m_cScissorRect.vMin.y = pRectangle->vMin.y;
		else
			m_cScissorRect.vMin.y = m_cViewportRect.vMin.y;
		if (pRectangle->vMax.x > 0.0f)
			m_cScissorRect.vMax.x = pRectangle->vMax.x;
		else
			m_cScissorRect.vMax.x = m_cViewportRect.vMax.x;
		if (pRectangle->vMax.y > 0.0f)
			m_cScissorRect.vMax.y = pRectangle->vMax.y;
		else
			m_cScissorRect.vMax.y = m_cViewportRect.vMax.y;
	} else {
		m_cScissorRect = m_cViewportRect;
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Get/set current resources                             ]
//[-------------------------------------------------------]
Surface *RendererBackend::GetRenderTarget(uint8 *pnFace) const
{
	if (pnFace)
		*pnFace = m_nCurrentSurfaceFace;
	return m_cCurrentSurface.GetSurface();
}

TextureBuffer *RendererBackend::GetColorRenderTarget(uint8 nColorIndex) const
{
	// Check current surface
	if (!m_cCurrentSurface.GetSurface() || m_cCurrentSurface.GetSurface()->GetType() != Surface::TextureBuffer)
		return nullptr; // Error!

	// Return the color render target
	return reinterpret_cast<TextureBuffer*>(m_cCurrentSurface.GetSurface());
}

TextureBuffer *RendererBackend::GetTextureBuffer(uint32 nStage) const
{
	// Check if the stage is correct
	return (nStage < m_sCapabilities.nMaxTextureUnits) ? m_ppCurrentTextureBuffer[nStage] : nullptr;
}

IndexBuffer *RendererBackend::GetIndexBuffer() const
{
	return m_pCurrentIndexBuffer;
}

Program *RendererBackend::GetProgram() const
{
	return reinterpret_cast<Program*>(m_cProgramHandler.GetResource());
}

bool RendererBackend::SetProgram(Program *pProgram)
{
	// Not implemented
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
