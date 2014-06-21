/*********************************************************\
 *  File: FixedFunctions.cpp                             *
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
#include <PLCore/Tools/Tools.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include "PLRendererOpenGL/PLRendererOpenGL.h"
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/VertexBuffer.h"
#include "PLRendererOpenGL/FixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FixedFunctions::FixedFunctions(Renderer &cRendererOpenGL) :
	m_pRendererOpenGL(&cRendererOpenGL),
	m_cColor(Color4::White),
	m_ppnTextureStageState(nullptr),
	m_ppnInternalTextureStageState(nullptr),
	m_pLightEnabled(nullptr),
	m_pLight(nullptr),
	m_pbClipPlane(nullptr),
	m_ppdClipPlane(nullptr),
	m_ppCurrentVertexBuffer(nullptr),
	m_nVertexBufferOffsets(nullptr)
{
	{ // Capabilities
		GLint nGLTemp;

		// Maximum number of active lights
		glGetIntegerv(GL_MAX_LIGHTS, &nGLTemp);
		m_sCapabilities.nMaxActiveLights = static_cast<uint8>(nGLTemp);

		// Maximum number of clip planes
		glGetIntegerv(GL_MAX_CLIP_PLANES, &nGLTemp);
		m_sCapabilities.nMaxClipPlanes = static_cast<uint8>(nGLTemp);

		// Vertex buffer fog coordinates supported?
		const Extensions &cExtensions = m_pRendererOpenGL->GetContext().GetExtensions();
		m_sCapabilities.bVertexBufferFogCoord = (cExtensions.IsGL_ARB_vertex_program() || cExtensions.IsGL_EXT_fog_coord());

		// Maximum vertex buffer streams
		m_sCapabilities.nMaxVertexBufferStreams = 8;
	}

	{ // Wrappers
		// Fog modes
		m_cPLE_FMWrapper.Resize(Fog::Number, false, false);
		m_cPLE_FMWrapper += GL_EXP;		//  0: Fog::Exp
		m_cPLE_FMWrapper += GL_EXP2;	//  1: Fog::Exp2
		m_cPLE_FMWrapper += GL_LINEAR;	//  2: Fog::Linear

		// Shade modes
		m_cPLE_SHADEWrapper.Resize(Shade::Number, false, false);
		m_cPLE_SHADEWrapper += GL_FLAT;		//  0: Shade::Flat
		m_cPLE_SHADEWrapper += GL_SMOOTH;	//  1: Shade::Smooth
	}

	{ // Renderer states
		// Set default renderer states
		// Fog
		m_nDefaultRenderState[RenderState::FogEnable]  = false;
		m_nDefaultRenderState[RenderState::FogColor]   = 0;
		m_nDefaultRenderState[RenderState::FogDensity] = Tools::FloatToUInt32(1.0f);
		m_nDefaultRenderState[RenderState::FogStart]   = Tools::FloatToUInt32(0.0f);
		m_nDefaultRenderState[RenderState::FogEnd]     = Tools::FloatToUInt32(1.0f);
		m_nDefaultRenderState[RenderState::FogMode]    = Fog::Exp;
		// Alpha test
		m_nDefaultRenderState[RenderState::AlphaTestEnable]    = false;
		m_nDefaultRenderState[RenderState::AlphaTestFunction]  = PLRenderer::Compare::GreaterEqual;
		m_nDefaultRenderState[RenderState::AlphaTestReference] = Tools::FloatToUInt32(0.5f);
		// Misc
		m_nDefaultRenderState[RenderState::Lighting]         = false;
		m_nDefaultRenderState[RenderState::Ambient]          = 0;
		m_nDefaultRenderState[RenderState::NormalizeNormals] = true;
		m_nDefaultRenderState[RenderState::ShadeMode]		 = Shade::Smooth;
	}
	MemoryManager::Set(m_nRenderState, -1, sizeof(uint32)*RenderState::Number);

	{ // Texture stage states
		// Set default texture stage states
		m_nDefaultTextureStageState[TextureStage::ColorTexEnv] = TextureEnvironment::Modulate;
		m_nDefaultTextureStageState[TextureStage::AlphaTexEnv] = TextureEnvironment::Modulate;
		m_nDefaultTextureStageState[TextureStage::TexGen]      = TexCoordGen::None;

		// Allocate texture stage states
		m_ppnTextureStageState = new uint32*[m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits];
		for (uint32 i=0; i<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; i++) {
			m_ppnTextureStageState[i] = new uint32[TextureStage::Number];
			MemoryManager::Set(m_ppnTextureStageState[i], -1, sizeof(uint32)*TextureStage::Number);
		}

		// Allocate internal texture stage states
		m_ppnInternalTextureStageState = new uint32*[m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits];
		for (uint32 i=0; i<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; i++) {
			m_ppnInternalTextureStageState[i] = new uint32[TextureStage::Number];
			MemoryManager::Set(m_ppnInternalTextureStageState[i], -1, sizeof(uint32)*TextureStage::Number);
		}
	}

	{ // Material states
		// Set default material states
		m_nDefaultMaterialState[MaterialState::Ambient]   = Color4::ToUInt32(0.2f, 0.2f, 0.2f, 1.0f);
		m_nDefaultMaterialState[MaterialState::Diffuse]   = Color4::ToUInt32(0.8f, 0.8f, 0.8f, 1.0f);
		m_nDefaultMaterialState[MaterialState::Specular]  = Color4::Black.ToUInt32();
		m_nDefaultMaterialState[MaterialState::Emission]  = Color4::Black.ToUInt32();
		m_nDefaultMaterialState[MaterialState::Shininess] = Tools::FloatToUInt32(0.0f);
	}
	MemoryManager::Set(m_nMaterialState, -1, sizeof(uint32)*MaterialState::Number);

	{ // Light states
		// Allocate OpenGL lights
		m_pLightEnabled = new bool[m_sCapabilities.nMaxActiveLights];
		m_pLight		= new Light[m_sCapabilities.nMaxActiveLights];
	}

	{ // Clip plane states
		// Allocate clip planes
		m_pbClipPlane = new bool[m_sCapabilities.nMaxClipPlanes];
		MemoryManager::Set(m_pbClipPlane, false, sizeof(bool)*m_sCapabilities.nMaxClipPlanes);
		m_ppdClipPlane = new GLdouble*[m_sCapabilities.nMaxClipPlanes];

		// Init clip planes
		for (uint32 i=0; i<m_sCapabilities.nMaxClipPlanes; i++) {
			m_ppdClipPlane[i] = new GLdouble[4];
			m_ppdClipPlane[i][0] = m_ppdClipPlane[i][1] = m_ppdClipPlane[i][2] = m_ppdClipPlane[i][3] = 0.0;
		}
	}

	{ // Vertex buffer states
		// Allocate vertex buffer streams
		m_ppCurrentVertexBuffer = new PLRenderer::VertexBuffer*[m_sCapabilities.nMaxVertexBufferStreams];
		MemoryManager::Set(m_ppCurrentVertexBuffer, 0, sizeof(PLRenderer::VertexBuffer**)*m_sCapabilities.nMaxVertexBufferStreams);
		m_nVertexBufferOffsets = new uint32[m_sCapabilities.nMaxVertexBufferStreams];
		MemoryManager::Set(m_nVertexBufferOffsets, 0, sizeof(uint32)*m_sCapabilities.nMaxVertexBufferStreams);
	}
}

/**
*  @brief
*    Destructor
*/
FixedFunctions::~FixedFunctions()
{
	{ // Texture stage states
		// Free texture stage states
		if (m_ppnTextureStageState) {
			for (uint32 i=0; i<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; i++)
				delete [] m_ppnTextureStageState[i];
			delete [] m_ppnTextureStageState;
			m_ppnTextureStageState = nullptr;
		}

		// Free internal texture stage states
		if (m_ppnInternalTextureStageState) {
			for (uint32 i=0; i<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; i++)
				delete [] m_ppnInternalTextureStageState[i];
			delete [] m_ppnInternalTextureStageState;
			m_ppnInternalTextureStageState = nullptr;
		}
	}

	{ // Light states
		if (m_pLightEnabled) {
			delete [] m_pLightEnabled;
			m_pLightEnabled = nullptr;
		}
		if (m_pLight) {
			delete [] m_pLight;
			m_pLight = nullptr;
		}
	}

	{ // Clip plane states
		if (m_pbClipPlane) {
			delete [] m_pbClipPlane;
			m_pbClipPlane = nullptr;
		}
		if (m_ppdClipPlane) {
			for (uint32 i=0; i<m_sCapabilities.nMaxClipPlanes; i++)
				delete [] m_ppdClipPlane[i];
			delete [] m_ppdClipPlane;
			m_ppdClipPlane = nullptr;
		}
	}

	{ // Vertex buffer streams
		// Free vertex buffer streams
		if (m_ppCurrentVertexBuffer) {
			delete [] m_ppCurrentVertexBuffer;
			m_ppCurrentVertexBuffer = nullptr;
		}
		if (m_nVertexBufferOffsets) {
			delete [] m_nVertexBufferOffsets;
			m_nVertexBufferOffsets = nullptr;
		}
	}
}

/**
*  @brief
*    General OpenGL settings
*/
void FixedFunctions::GeneralSettings()
{
	// We need to activate this for the fix pass SetMaterialState() function
	glEnable(GL_COLOR_MATERIAL);

	// Specular highlighting (improves specular lighting)
	if (m_pRendererOpenGL->GetContext().GetExtensions().IsGL_EXT_separate_specular_color())
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
}

/**
*  @brief
*    Restores the device states
*/
void FixedFunctions::RestoreDeviceStates()
{
	// Reset texture buffers
	glMatrixMode(GL_TEXTURE);
	for (uint32 nStage=0; nStage<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; nStage++) {
		if (glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB+nStage);
		glLoadIdentity();
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D_ARRAY_EXT);
		glDisable(GL_TEXTURE_RECTANGLE_ARB);
		glDisable(GL_TEXTURE_CUBE_MAP_ARB);
		glDisable(GL_TEXTURE_3D_EXT);
	}

	// Setup current transform states
	Matrix4x4 mTrans;
	mTrans = GetTransformState(Transform::Projection);
	m_mProj = Matrix4x4::Zero;
	SetTransformState(Transform::Projection, mTrans);
	mTrans = GetTransformState(Transform::View);
	m_mView = Matrix4x4::Zero;
	SetTransformState(Transform::View, mTrans);
	mTrans = GetTransformState(Transform::World);
	m_mWorld = Matrix4x4::Zero;
	SetTransformState(Transform::World, mTrans);
	for (uint32 i=3; i<Transform::Number; i++) {
		mTrans = GetTransformState(static_cast<Transform::Enum>(i));
		m_mTextureMatrix[i - 3] = Matrix4x4::Zero;
		SetTransformState(static_cast<Transform::Enum>(i), mTrans);
	}

	// Texture stage states
	for (uint32 nStage=0; nStage<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; nStage++) {
		for (uint32 i=0; i<TextureStage::Number; i++) {
			uint32 nState = m_ppnInternalTextureStageState[nStage][i];
			m_ppnInternalTextureStageState[nStage][i] = m_ppnInternalTextureStageState[nStage][i] + 1;
			SetTextureStageState(nStage, static_cast<TextureStage::Enum>(i), nState);
		}
	}

	// Material states
	for (uint32 i=0; i<MaterialState::Number; i++) {
		uint32 nState = m_nMaterialState[i];
		m_nMaterialState[i] = m_nMaterialState[i] + 1;
		SetMaterialState(static_cast<MaterialState::Enum>(i), nState);
	}

	// Light states
	Light sLight;
	for (uint32 i=0; i<m_sCapabilities.nMaxActiveLights; i++) {
		GetLight(i, sLight);
		SetLight(i, sLight);
		SetLightEnabled(i, IsLightEnabled(i));
	}

	// Clip plane states
	for (uint32 i=0; i<m_sCapabilities.nMaxClipPlanes; i++) {
		// Is the clip plane enabled?
		if (m_pbClipPlane[i]) {
			glClipPlane(GL_CLIP_PLANE0 + i, m_ppdClipPlane[i]);
			glEnable(GL_CLIP_PLANE0 + i);
		} else {
			glDisable(GL_CLIP_PLANE0 + i);
		}
	}

	// Vertex buffer states
	for (uint32 i=0; i<m_sCapabilities.nMaxVertexBufferStreams; i++) {
		PLRenderer::VertexBuffer *pBuffer = m_ppCurrentVertexBuffer[i];
		uint32 nOffset = m_nVertexBufferOffsets[i];
		m_ppCurrentVertexBuffer[i] = nullptr;
		m_nVertexBufferOffsets[i]  = 0;
		SetVertexBuffer(pBuffer, nOffset, i);
	}
}

/**
*  @brief
*    Updates the current OpenGL projection matrix
*/
void FixedFunctions::UpdateCurrentOpenGLProjectionMatrix()
{
	// Set original projection matrix
	m_mProj = m_mOriginalProj;

	// Apply z bias
	m_mProj.zz -= Tools::UInt32ToFloat(m_pRendererOpenGL->GetRenderState(PLRenderer::RenderState::ZBias));

	// Flip along the y axis?
	PLRenderer::Surface *pSurface = m_pRendererOpenGL->GetRenderTarget();
	if (pSurface && pSurface->IsAPISwapY()) {
		// Flip
		m_mProj.yy = -m_mProj.yy;

		// Update the cull mode render state
		const uint32 nState = m_pRendererOpenGL->GetRenderState(PLRenderer::RenderState::CullMode);
		m_pRendererOpenGL->SetRenderState(PLRenderer::RenderState::CullMode, nState+1);
		m_pRendererOpenGL->SetRenderState(PLRenderer::RenderState::CullMode, nState);
	}

	// Set OpenGL matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m_mProj);
}

/**
*  @brief
*    Every time the view matrix changes we have to update some stuff
*/
void FixedFunctions::ViewMatrixUpdated()
{
	// Update lights
	for (uint32 i=0; i<m_sCapabilities.nMaxActiveLights; i++) {
		// Is the light enabled?
		const uint32 nLightID = GL_LIGHT0 + i;	// Get light ID
		if (glIsEnabled(nLightID)) {
			// Update position
			const Light &sLight = m_pLight[i];
			const float fValue[4] = { sLight.vPosition.x, sLight.vPosition.y, sLight.vPosition.z, 1.0f };
			glLightfv(nLightID, GL_POSITION, fValue);

			// Update direction
			glLightfv(nLightID, GL_SPOT_DIRECTION, sLight.vDirection);
		}
	}

	// Update clip planes
	for (uint32 i=0; i<m_sCapabilities.nMaxClipPlanes; i++) {
		// Is the clip plane enabled?
		if (m_pbClipPlane[i])
			glClipPlane(GL_CLIP_PLANE0+i, m_ppdClipPlane[i]);
	}
}

/**
*  @brief
*    Updates the current OpenGL texture matrix of the given stage
*/
void FixedFunctions::UpdateCurrentOpenGLTextureMatrix(uint32 nStage)
{
	if (m_pRendererOpenGL->GetTextureBuffer(nStage) && m_pRendererOpenGL->GetTextureBuffer(nStage)->GetType() == PLRenderer::Resource::TypeTextureBufferRectangle) {
		PLRenderer::TextureBufferRectangle *pTextureBuffer = static_cast<PLRenderer::TextureBufferRectangle*>(m_pRendererOpenGL->GetTextureBuffer(nStage));
		Matrix4x4 mScale;
		mScale.SetScaleMatrix(static_cast<float>(pTextureBuffer->GetSize().x), static_cast<float>(pTextureBuffer->GetSize().y), 1.0f);

		// If automatic texture generation is reflection map, we need to apply the transpose of the
		// view matrix in order to get the reflection looking correct. Unusual for rectangle textures,
		// but ok, we allow it... ;-)
		if (m_ppnInternalTextureStageState[nStage][FixedFunctions::TextureStage::TexGen] == FixedFunctions::TexCoordGen::ReflectionMap) {
			Matrix4x4 mTransposed(m_mView.xx, m_mView.yx, m_mView.zx, 0.0f,
								  m_mView.xy, m_mView.yy, m_mView.zy, 0.0f,
								  m_mView.xz, m_mView.yz, m_mView.zz, 0.0f,
								  0.0f,       0.0f,       0.0f,       1.0f);
			m_mTextureMatrix[nStage] = m_mOriginalTexture[nStage]*mTransposed*mScale;
		} else {
			m_mTextureMatrix[nStage] = m_mOriginalTexture[nStage]*mScale;
		}
	} else {
		// If automatic texture generation is reflection map, we need to apply the transpose of the
		// view matrix in order to get the reflection looking correct
		if (m_ppnInternalTextureStageState[nStage][FixedFunctions::TextureStage::TexGen] == FixedFunctions::TexCoordGen::ReflectionMap) {
			Matrix4x4 mTransposed(m_mView.xx, m_mView.yx, m_mView.zx, 0.0f,
								  m_mView.xy, m_mView.yy, m_mView.zy, 0.0f,
								  m_mView.xz, m_mView.yz, m_mView.zz, 0.0f,
								  0.0f,       0.0f,       0.0f,       1.0f);
			m_mTextureMatrix[nStage] = m_mOriginalTexture[nStage]*mTransposed;
		} else {
			m_mTextureMatrix[nStage] = m_mOriginalTexture[nStage];
		}
	}

	// Set texture matrix
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(m_mTextureMatrix[nStage]);
}

/**
*  @brief
*    Updates the current OpenGL texture matrix of the given stage
*/
void FixedFunctions::UpdateShaderOpenGLTextureMatrix(uint32 nStage)
{
	// Scale texture matrix a rectangle texture buffer is used - we always want to work with normalized
	// texture coordinates :)
	if (m_pRendererOpenGL->GetTextureBuffer(nStage) &&
		m_pRendererOpenGL->GetTextureBuffer(nStage)->GetType() == PLRenderer::Resource::TypeTextureBufferRectangle) {
		PLRenderer::TextureBufferRectangle *pT = static_cast<PLRenderer::TextureBufferRectangle*>(m_pRendererOpenGL->GetTextureBuffer(nStage));
		Matrix4x4 mScale;
		mScale.SetScaleMatrix(static_cast<float>(pT->GetSize().x), static_cast<float>(pT->GetSize().y), 1.0f);
		glMatrixMode(GL_TEXTURE);
		m_mTextureMatrix[nStage] = m_mOriginalTexture[nStage]*mScale;
		glLoadMatrixf(m_mTextureMatrix[nStage]);
	} else {
		// If the previous texture buffer was a rectangle one, now reset the texture matrix
		if ((m_pRendererOpenGL->m_ppPrevTextureBuffer[nStage] && m_pRendererOpenGL->m_ppPrevTextureBuffer[nStage]->GetType() == PLRenderer::Resource::TypeTextureBufferRectangle) ||
			(m_pRendererOpenGL->m_ppPrevTextureBuffer[nStage] && !m_pRendererOpenGL->GetTextureBuffer(nStage))) {
			glMatrixMode(GL_TEXTURE);
			m_mTextureMatrix[nStage] = m_mOriginalTexture[nStage];
			glLoadMatrixf(m_mTextureMatrix[nStage]);
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::FixedFunctions functions   ]
//[-------------------------------------------------------]
const FixedFunctions::Capabilities &FixedFunctions::GetCapabilities() const
{
	return m_sCapabilities;
}

void FixedFunctions::Reset()
{
	ResetRenderStates();
	SetColor();
	ResetTransformStates();
	ResetTextureStageStates();
	ResetMaterialStates();
	ResetLights();
	SetClipPlaneEnabled();
	SetClipPlane();
	SetVertexBuffer();
}

uint32 FixedFunctions::GetDefaultRenderState(RenderState::Enum nState) const
{
	return (nState < RenderState::Number) ? m_nDefaultRenderState[nState] : 0;
}

void FixedFunctions::ResetRenderStates()
{
	// Set renderer states to this default settings
	for (uint32 i=0; i<RenderState::Number; i++)
		SetRenderState(static_cast<RenderState::Enum>(i), m_nDefaultRenderState[i]);
}

int FixedFunctions::GetRenderState(RenderState::Enum nState) const
{
	// Check if the state is a valid render state member
	return (nState < RenderState::Number) ? m_nRenderState[nState] : -1;
}

bool FixedFunctions::SetRenderState(RenderState::Enum nState, uint32 nValue)
{
	// Check if the state is a valid render state member
	if (nState >= RenderState::Number)
		return false; // Error!

	// Check if this render state is already set to this value
	if (m_nRenderState[nState] != nValue) {
		// Set the render state
		m_nRenderState[nState] = nValue;

		// Set render state
		switch (nState) {
		// Fog
			case RenderState::FogEnable:
				if (nValue == 0)
					glDisable(GL_FOG);
				else if (nValue == 1)
					glEnable(GL_FOG);
				else
					return false; // Error, invalid value!
				break;

			case RenderState::FogColor:
			{
				const float fFogColor[4] = { Color4::RedFromUInt32(nValue),
											 Color4::GreenFromUInt32(nValue),
											 Color4::BlueFromUInt32(nValue),
											 Color4::AlphaFromUInt32(nValue) };
				glFogfv(GL_FOG_COLOR, fFogColor);
				break;
			}

			case RenderState::FogDensity:
				glFogf(GL_FOG_DENSITY, Tools::UInt32ToFloat(nValue));
				break;

			case RenderState::FogStart:
				glFogf(GL_FOG_START, Tools::UInt32ToFloat(nValue));
				break;

			case RenderState::FogEnd:
				glFogf(GL_FOG_END, Tools::UInt32ToFloat(nValue));
				break;

			case RenderState::FogMode:
			{
				const uint32 &nAPIValue = m_cPLE_FMWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null) {
					glFogi(GL_FOG_MODE, nAPIValue);
				} else {
					return false; // Error, invalid value!
				}
				break;
			}

		// Alpha test
			case RenderState::AlphaTestEnable:
				if (nValue == 0)
					glDisable(GL_ALPHA_TEST);
				else if (nValue == 1)
					glEnable(GL_ALPHA_TEST);
				else
					return false; // Error, invalid value!
				break;

			case RenderState::AlphaTestFunction:
			{
				const uint32 &nAPIValue = m_pRendererOpenGL->m_cPLE_CMPWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null) {
					glAlphaFunc(nAPIValue, Tools::UInt32ToFloat(GetRenderState(RenderState::AlphaTestReference)));
				} else {
					return false; // Error, invalid value!
				}
				break;
			}

			case RenderState::AlphaTestReference:
			{
				const uint32 &nAPIValue = m_pRendererOpenGL->m_cPLE_CMPWrapper[GetRenderState(RenderState::AlphaTestFunction)];
				if (&nAPIValue != &Array<uint32>::Null) {
					glAlphaFunc(nAPIValue, Tools::UInt32ToFloat(nValue));
				} else {
					return false; // Error, invalid value!
				}
				break;
			}

		// Misc
			case RenderState::Lighting:
				if (nValue == 0)
					glDisable(GL_LIGHTING);
				else if (nValue == 1)
					glEnable(GL_LIGHTING);
				else
					return false; // Error, invalid value!
				break;

			case RenderState::Ambient:
			{
				const float fAmbientColor[4] = { Color4::RedFromUInt32(nValue),
												 Color4::GreenFromUInt32(nValue),
												 Color4::BlueFromUInt32(nValue),
												 Color4::AlphaFromUInt32(nValue) };
				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fAmbientColor);
				break;
			}

			case RenderState::NormalizeNormals:
				if (nValue == 0)
					glDisable(GL_NORMALIZE);
				else if (nValue == 1)
					glEnable(GL_NORMALIZE);
				else
					return false; // Error, invalid value!
				break;

			case RenderState::ShadeMode:
			{
				const uint32 &nAPIValue = m_cPLE_SHADEWrapper[nValue];
				if (&nAPIValue != &Array<uint32>::Null) {
					glShadeModel(nAPIValue);
				} else {
					return false; // Error, invalid value!
				}
				break;
			}

			case RenderState::Number:
			case RenderState::Unknown:
			default:
				return false; // Error, invalid render state!
		}
	}

	// All went fine
	return true;
}

Color4 FixedFunctions::GetColor() const
{
	return m_cColor;
}

void FixedFunctions::SetColor(const Color4 &cColor)
{
	m_cColor = cColor;
	glColor4fv(m_cColor);
}

void FixedFunctions::ResetTransformStates()
{
	// Reset projection matrix
	m_mOriginalProj.SetIdentity();
	UpdateCurrentOpenGLProjectionMatrix();

	// Reset view matrix
	m_mView.SetIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m_mView);
	ViewMatrixUpdated();

	// Reset world matrix
	m_mWorld.SetIdentity();

	// Reset texture matrices
	for (uint32 nStage=0; nStage<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; nStage++) {
		// Activate the correct OpenGL texture stage
		if (glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB + nStage);

		m_mOriginalTexture[nStage].SetIdentity();
		UpdateCurrentOpenGLTextureMatrix(nStage);
	}
}

const Matrix4x4 &FixedFunctions::GetTransformState(Transform::Enum nState, bool bOriginal) const
{
	// Check if the state is a valid Transform::Enum member
	if (nState < Transform::Number) {
		// Get transform state
		switch (nState) {
			case Transform::Projection:
				return bOriginal ? m_mOriginalProj : m_mProj;

			case Transform::View:
				return m_mView;

			case Transform::World:
				return m_mWorld;

			case Transform::Texture0:
			case Transform::Texture1:
			case Transform::Texture2:
			case Transform::Texture3:
			case Transform::Texture4:
			case Transform::Texture5:
			case Transform::Texture6:
			case Transform::Texture7:
			case Transform::Number:
			case Transform::Unknown:
			default: // Texture transformation matrix
			{
				const uint32 nStage = nState - Transform::Texture0;
				if (nStage < m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits)
					return bOriginal ? m_mOriginalTexture[nStage] : m_mTextureMatrix[nStage];
				break;
			}
		}
	}

	// Error!
	return Matrix4x4::Identity;
}

bool FixedFunctions::SetTransformState(Transform::Enum nState, const Matrix3x4 &mTrans)
{
	// Check if the state is a valid Transform::Enum member
	if (nState < Transform::Number) {
		// Set matrix, we check the matrix before setting it which gives a bit more performance
		switch (nState) {
			case Transform::Projection: // Projection transformation matrix
				if (m_mOriginalProj != mTrans) {
					m_mOriginalProj = mTrans;
					UpdateCurrentOpenGLProjectionMatrix();
				}
				return true; // Done

			case Transform::View: // View transformation matrix
				if (m_mView != mTrans) {
					m_mView = mTrans;
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(m_mView*m_mWorld);
					ViewMatrixUpdated();
				}
				return true; // Done

			case Transform::World: // World transformation matrix
				if (m_mWorld != mTrans) {
					m_mWorld = mTrans;
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(m_mView*m_mWorld);
				}
				return true; // Done

			case Transform::Texture0:
			case Transform::Texture1:
			case Transform::Texture2:
			case Transform::Texture3:
			case Transform::Texture4:
			case Transform::Texture5:
			case Transform::Texture6:
			case Transform::Texture7:
			case Transform::Number:
			case Transform::Unknown:
			default: // Texture transformation matrix
			{
				const uint32 nStage = nState - Transform::Texture0;
				if (nStage < m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits) {
					if (m_mOriginalTexture[nStage] != mTrans) {
						// Activate the correct OpenGL texture stage
						if (glActiveTextureARB)
							glActiveTextureARB(GL_TEXTURE0_ARB + nStage);

						// Scale texture matrix a rectangle texture is used - we always want to work with normalized
						// texture coordinates :)
						m_mOriginalTexture[nStage] = mTrans;
						UpdateCurrentOpenGLTextureMatrix(nStage);
					}
					return true; // Done
				}
				break;
			}
		}
	}

	// Error!
	return false;
}

bool FixedFunctions::SetTransformState(Transform::Enum nState, const Matrix4x4 &mTrans)
{
	// Check if the state is a valid Transform::Enum member
	if (nState < Transform::Number) {
		// Set matrix, we check the matrix before setting it which gives a bit more performance
		switch (nState) {
			case Transform::Projection: // Projection transformation matrix
				if (m_mOriginalProj != mTrans) {
					m_mOriginalProj = mTrans;
					UpdateCurrentOpenGLProjectionMatrix();
				}
				return true; // Done

			case Transform::View: // View transformation matrix
				if (m_mView != mTrans) {
					m_mView = mTrans;
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(m_mView*m_mWorld);
					ViewMatrixUpdated();
				}
				return true; // Done

			case Transform::World: // World transformation matrix
				if (m_mWorld != mTrans) {
					m_mWorld = mTrans;
					glMatrixMode(GL_MODELVIEW);
					glLoadMatrixf(m_mView*m_mWorld);
				}
				return true; // Done

			case Transform::Texture0:
			case Transform::Texture1:
			case Transform::Texture2:
			case Transform::Texture3:
			case Transform::Texture4:
			case Transform::Texture5:
			case Transform::Texture6:
			case Transform::Texture7:
			case Transform::Number:
			case Transform::Unknown:
			default: // Texture transformation matrix
			{
				const uint32 nStage = nState - Transform::Texture0;
				if (nStage < m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits) {
					if (m_mOriginalTexture[nStage] != mTrans) {
						// Activate the correct OpenGL texture stage
						if (glActiveTextureARB)
							glActiveTextureARB(GL_TEXTURE0_ARB + nStage);

						// Scale texture matrix a rectangle texture is used - we always want to work with normalized
						// texture coordinates :)
						m_mOriginalTexture[nStage] = mTrans;
						UpdateCurrentOpenGLTextureMatrix(nStage);
					}
					return true; // Done
				}
				break;
			}
		}
	}

	// Error!
	return false;
}

uint32 FixedFunctions::GetDefaultTextureStageState(TextureStage::Enum nState) const
{
	return (nState < TextureStage::Number) ? m_nDefaultTextureStageState[nState] : 0;
}

void FixedFunctions::ResetTextureStageStates()
{
	// Set texture stage states to this default settings
	for (uint32 nStage=0; nStage<m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits; nStage++) {
		for (uint32 i=0; i<TextureStage::Number; i++)
			SetTextureStageState(nStage, static_cast<TextureStage::Enum>(i), m_nDefaultTextureStageState[i]);
	}
}

int FixedFunctions::GetTextureStageState(uint32 nStage, TextureStage::Enum nState) const
{
	// Check if the stage is correct and check if the state is a valid texture stage member
	return (nStage < m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits && nState < TextureStage::Number) ? m_ppnTextureStageState[nStage][nState] : -1;
}

// Internal helper function
void ConfigureTexEnv(FixedFunctions::TextureEnvironment::Enum nTexEnv, bool bIsRGB)
{
	// Color/alpha check
	uint32 nCombineMode, nSource0;
	uint32 nOperand0, nSrcColor;
	if (bIsRGB) {
		nCombineMode = GL_COMBINE_RGB_ARB;
		nSource0	 = GL_SOURCE0_RGB_ARB;
		nOperand0	 = GL_OPERAND0_RGB_ARB;
		nSrcColor	 = GL_SRC_COLOR;
	} else {
		nCombineMode = GL_COMBINE_ALPHA_ARB;
		nSource0	 = GL_SOURCE0_ALPHA_ARB;
		nOperand0	 = GL_OPERAND0_ALPHA_ARB;
		nSrcColor	 = GL_SRC_ALPHA;
	}
	const uint32 nSource1  = nSource0  + 1;
	const uint32 nSource2  = nSource1  + 1;
	const uint32 nOperand1 = nOperand0 + 1;
	const uint32 nOperand2 = nOperand1 + 1;

	// Setup environment mode
	switch (nTexEnv) {
		case FixedFunctions::TextureEnvironment::Add:
			// Mode
			glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_ADD);
			// 0
			glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
			// 1
			glTexEnvi(GL_TEXTURE_ENV, nSource1,		GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, nOperand1,	nSrcColor);
			break;

		case FixedFunctions::TextureEnvironment::Replace:
			// Mode
			glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_REPLACE);
			// 0
			glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
			break;

		case FixedFunctions::TextureEnvironment::Modulate:
			// Mode
			glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_MODULATE);
			// 0
			glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
			// 1
			glTexEnvi(GL_TEXTURE_ENV, nSource1,		GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, nOperand1,	nSrcColor);
			break;

		case FixedFunctions::TextureEnvironment::PassThru:
			// Mode
			glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_REPLACE);
			// 0
			glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
			break;

		case FixedFunctions::TextureEnvironment::Dot3:
			// Only valid for color!
			if (bIsRGB) {
				// Mode
				glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_DOT3_RGBA_ARB);
				// 0
				glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_PREVIOUS_ARB);
				glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
				// 1
				glTexEnvi(GL_TEXTURE_ENV, nSource1,		GL_TEXTURE);
				glTexEnvi(GL_TEXTURE_ENV, nOperand1,	nSrcColor);
			}
			break;

		case FixedFunctions::TextureEnvironment::Interpolate:
			// Mode
			glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_INTERPOLATE_ARB);
			// 0
			glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
			// 1
			glTexEnvi(GL_TEXTURE_ENV, nSource1,		GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand1,	nSrcColor);
			// 2
			glTexEnvi(GL_TEXTURE_ENV, nSource2,		GL_PRIMARY_COLOR_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand2,	nSrcColor);
			break;

		case FixedFunctions::TextureEnvironment::InterpolatePrimary:
			// Mode
			glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_INTERPOLATE_ARB);
			// 0
			glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_PRIMARY_COLOR_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
			// 1
			glTexEnvi(GL_TEXTURE_ENV, nSource1,		GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand1,	nSrcColor);
			// 2
			glTexEnvi(GL_TEXTURE_ENV, nSource2,		GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, nOperand2,	nSrcColor);
			break;

		case FixedFunctions::TextureEnvironment::InterpolateTexAlpha:
			// Mode
			glTexEnvi(GL_TEXTURE_ENV, nCombineMode, GL_INTERPOLATE_ARB);
			// 0
			glTexEnvi(GL_TEXTURE_ENV, nSource0,		GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, nOperand0,	nSrcColor);
			// 1
			glTexEnvi(GL_TEXTURE_ENV, nSource1,		GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, nOperand1,	nSrcColor);
			// 2
			glTexEnvi(GL_TEXTURE_ENV, nSource2,		GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, nOperand2,	GL_SRC_ALPHA);
			break;

		case FixedFunctions::TextureEnvironment::Number:
		case FixedFunctions::TextureEnvironment::Unknown:
		default:
			break;
	}
}

bool FixedFunctions::SetTextureStageState(uint32 nStage, TextureStage::Enum nState, uint32 nValue)
{
	// Check if the stage is correct and check if the state is a valid texture stage member
	if (nStage >= m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits || nState >= TextureStage::Number)
		return false; // Error!

	// Set the texture stage state
	m_ppnTextureStageState[nStage][nState] = nValue;

	// Check if this texture stage state is already set to this value
	if (m_ppnInternalTextureStageState[nStage][nState] != nValue) {
		// Is reflection map currently active?
		const bool bReflectionMap = (m_ppnInternalTextureStageState[nStage][TextureStage::TexGen] == TexCoordGen::ReflectionMap);

		// Set the texture stage state
		const uint32 nOldValue = m_ppnInternalTextureStageState[nStage][nState];
		m_ppnInternalTextureStageState[nStage][nState] = nValue;

		// Activate the correct OpenGL texture stage
		if (glActiveTextureARB)
			glActiveTextureARB(GL_TEXTURE0_ARB + nStage);

		// Set texture stage state
		switch (nState) {
			case TextureStage::ColorTexEnv:
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				ConfigureTexEnv(static_cast<TextureEnvironment::Enum>(nValue), true);
				break;

			case TextureStage::AlphaTexEnv:
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
				ConfigureTexEnv(static_cast<TextureEnvironment::Enum>(nValue), false);
				break;

			case TextureStage::TexGen:
				// Enable/disable texture coordinate generation
				if (nValue == TexCoordGen::None) {
					glDisable(GL_TEXTURE_GEN_S);
					glDisable(GL_TEXTURE_GEN_T);
					glDisable(GL_TEXTURE_GEN_R);
				} else {
					// Setup texture coordinate generation
					switch (nValue) {
						case TexCoordGen::ObjectLinear:
							glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
							glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
							glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
							break;

						case TexCoordGen::EyeLinear:
							glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
							glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
							glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
							break;

						case TexCoordGen::ReflectionMap:
							glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
							glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
							glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
							break;

						case TexCoordGen::NormalMap:
							glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_ARB);
							glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_ARB);
							glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_ARB);
							break;

						case TexCoordGen::SphereMap:
							glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
							glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
							glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
							break;

						default:
							break;
					}

					if (nOldValue == TexCoordGen::None) {
						glEnable(GL_TEXTURE_GEN_S);
						glEnable(GL_TEXTURE_GEN_T);
						glEnable(GL_TEXTURE_GEN_R);
					}
				}
				break;

			case TextureStage::Number:
			case TextureStage::Unknown:
			default:
				return false; // Invalid texture stage state!
		}

		// Reflection map state changed?
		if ((bReflectionMap && m_ppnInternalTextureStageState[nStage][TextureStage::TexGen] != TexCoordGen::ReflectionMap) ||
			(!bReflectionMap && m_ppnInternalTextureStageState[nStage][TextureStage::TexGen] == TexCoordGen::ReflectionMap))
			UpdateCurrentOpenGLTextureMatrix(nStage);
	}

	// All went fine
	return true;
}

uint32 FixedFunctions::GetDefaultMaterialState(MaterialState::Enum nState) const
{
	return (nState < MaterialState::Number) ? m_nDefaultMaterialState[nState] : 0;
}

void FixedFunctions::ResetMaterialStates()
{
	// Set material states to this default settings
	for (uint32 i=0; i<MaterialState::Number; i++)
		SetMaterialState(static_cast<MaterialState::Enum>(i), m_nDefaultMaterialState[i]);
}

int FixedFunctions::GetMaterialState(MaterialState::Enum nState) const
{
	// Check if the state is a valid material state member
	return (nState < MaterialState::Number) ? m_nMaterialState[nState] : -1;
}

bool FixedFunctions::SetMaterialState(MaterialState::Enum nState, uint32 nValue)
{
	// Check if the state is a valid material state member
	if (nState >= MaterialState::Number)
		return false; // Error!

	// Check if this material state is already set to this value
	if (m_nMaterialState[nState] != nValue) {
		// Set material state
		switch (nState) {
			case MaterialState::Ambient:
			{
				const float fValue[4] = { Color4::RedFromUInt32(nValue),
										  Color4::GreenFromUInt32(nValue),
										  Color4::BlueFromUInt32(nValue),
										  Color4::AlphaFromUInt32(nValue) };
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fValue);
				break;
			}

			case MaterialState::Diffuse:
			{
				const float fValue[4] = { Color4::RedFromUInt32(nValue),
										  Color4::GreenFromUInt32(nValue),
										  Color4::BlueFromUInt32(nValue),
										  Color4::AlphaFromUInt32(nValue) };
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fValue);
				break;
			}

			case MaterialState::Specular:
			{
				const float fValue[4] = { Color4::RedFromUInt32(nValue),
										  Color4::GreenFromUInt32(nValue),
										  Color4::BlueFromUInt32(nValue),
										  Color4::AlphaFromUInt32(nValue) };
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fValue);
				break;
			}

			case MaterialState::Emission:
			{
				const float fValue[4] = { Color4::RedFromUInt32(nValue),
										  Color4::GreenFromUInt32(nValue),
										  Color4::BlueFromUInt32(nValue),
										  Color4::AlphaFromUInt32(nValue) };
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fValue);
				break;
			}

			case MaterialState::Shininess:
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Tools::UInt32ToFloat(nValue));
				break;

			case MaterialState::Number:
			case MaterialState::Unknown:
			default:
				return false; // Invalid sampler state!
		}

		// Set the material state
		m_nMaterialState[nState] = nValue;
	}

	// All went fine
	return true;
}

void FixedFunctions::GetDefaultLightSettings(Light &sLight) const
{
	sLight.nType = LightType::Point;
	sLight.cAmbient.SetRGBA(0.0f, 0.0f, 0.0f, 0.0f);
	sLight.cDiffuse.SetRGBA(0.0f, 0.0f, 0.0f, 0.0f);
	sLight.cSpecular.SetRGBA(0.0f, 0.0f, 0.0f, 0.0f);
	sLight.vPosition.SetXYZ(0.0f, 0.0f, 0.0f);
	sLight.vDirection.SetXYZ(0.0f, 0.0f, 0.0f);
	sLight.fCutOff				 = 0.0f;
	sLight.fConstantAttenuation  = 1.0f;
	sLight.fLinearAttenuation	 = 0.0f;
	sLight.fQuadraticAttenuation = 0.0f;
}

void FixedFunctions::ResetLights()
{
	// Get default light settings
	Light sLight;
	GetDefaultLightSettings(sLight);

	// Reset lights
	for (uint32 i=0; i<m_sCapabilities.nMaxActiveLights; i++) {
		SetLightEnabled(i, false);
		SetLight(i, sLight);
	}
}

bool FixedFunctions::IsLightEnabled(uint32 nLightID) const
{
	// Check if the index is correct
	return (nLightID < m_sCapabilities.nMaxActiveLights && m_pLightEnabled[nLightID]);
}

bool FixedFunctions::SetLightEnabled(uint32 nLightID, bool bEnabled)
{
	// Check if the index is correct
	if (nLightID >= m_sCapabilities.nMaxActiveLights)
		return false; // Error!

	// Enable/disable the light
	if (bEnabled)
		glEnable(GL_LIGHT0 + nLightID);
	else
		glDisable(GL_LIGHT0 + nLightID);
	m_pLightEnabled[nLightID] = bEnabled;

	// Done
	return true;
}

bool FixedFunctions::GetLight(uint32 nLightID, Light &sLight) const
{
	// Check if the index is correct
	if (nLightID >= m_sCapabilities.nMaxActiveLights)
		return false; // Error!

	// Get OpenGL light
	MemoryManager::Copy(&sLight, &m_pLight[nLightID], sizeof(Light));

	// Done
	return true;
}

bool FixedFunctions::SetLight(uint32 nLightID, const Light &sLight)
{
	// Check if the index is correct
	if (nLightID >= m_sCapabilities.nMaxActiveLights)
		return false; // Error!

	// Setup OpenGL light
	const uint32 nID = GL_LIGHT0 + nLightID;
	glLightfv(nID, GL_AMBIENT,  sLight.cAmbient);
	glLightfv(nID, GL_DIFFUSE,  sLight.cDiffuse);
	glLightfv(nID, GL_SPECULAR, sLight.cSpecular);
	switch (sLight.nType) {
		case LightType::Point:
		{
			const float fValue[4] = { sLight.vPosition.x, sLight.vPosition.y, sLight.vPosition.z, 1.0f };
			glLightfv(nID, GL_POSITION,    fValue);
			glLightf (nID, GL_SPOT_CUTOFF, 180.0f);
			break;
		}

		case LightType::Spot:
		{
			const float fValue[4] = { sLight.vPosition.x, sLight.vPosition.y, sLight.vPosition.z, 1.0f };
			glLightfv(nID, GL_POSITION,       fValue);
			glLightf (nID, GL_SPOT_CUTOFF,    sLight.fCutOff);
			glLightfv(nID, GL_SPOT_DIRECTION, sLight.vDirection);
			break;
		}

		case LightType::Directional:
		{
			const float fValue[4] = { sLight.vDirection.x, sLight.vDirection.y, sLight.vDirection.z, 0.0f };
			glLightfv(nID, GL_POSITION, fValue);
			break;
		}

		case LightType::Number:
		case LightType::Unknown:
		default:
			return false; // Invalid light state!
	}
	glLightf(nID, GL_CONSTANT_ATTENUATION,  sLight.fConstantAttenuation);
	glLightf(nID, GL_LINEAR_ATTENUATION,    sLight.fLinearAttenuation);
	glLightf(nID, GL_QUADRATIC_ATTENUATION, sLight.fQuadraticAttenuation);
	MemoryManager::Copy(&m_pLight[nLightID], &sLight, sizeof(Light));

	// Done
	return true;
}

bool FixedFunctions::IsClipPlaneEnabled(uint8 nIndex) const
{
	// Check whether the index is valid
	return (nIndex < m_sCapabilities.nMaxClipPlanes) && m_pbClipPlane[nIndex];
}

bool FixedFunctions::SetClipPlaneEnabled(char nIndex, bool bEnable)
{
	// Set all available clip planes to this setting?
	if (nIndex < 0) {
		for (uint8 i=0; i<m_sCapabilities.nMaxClipPlanes; i++)
			SetClipPlaneEnabled(i, bEnable);
	} else {
		// Check whether the index is valid
		if (nIndex >= static_cast<int>(m_sCapabilities.nMaxClipPlanes))
			return false; // Error!

		// Enable/disable clip plane
		if (bEnable)
			glEnable(GL_CLIP_PLANE0 + nIndex);
		else
			glDisable(GL_CLIP_PLANE0 + nIndex);
		m_pbClipPlane[static_cast<uint32>(nIndex)] = bEnable;
	}

	// Done
	return true;
}

bool FixedFunctions::GetClipPlane(uint8 nIndex, float &fA, float &fB, float &fC, float &fD) const
{
	// Check whether the index is valid
	if (nIndex >= m_sCapabilities.nMaxClipPlanes)
		return false; // Error!

	// Get the clip plane
	fA = static_cast<float>(m_ppdClipPlane[nIndex][0]);
	fB = static_cast<float>(m_ppdClipPlane[nIndex][1]);
	fC = static_cast<float>(m_ppdClipPlane[nIndex][2]);
	fD = static_cast<float>(m_ppdClipPlane[nIndex][3]);

	// Done
	return true;
}

bool FixedFunctions::SetClipPlane(char nIndex, float fA, float fB, float fC, float fD)
{
	// Set all available clip planes to this setting?
	if (nIndex < 0) {
		for (uint8 i=0; i<m_sCapabilities.nMaxClipPlanes; i++)
			SetClipPlane(i, fA, fB, fC, fD);
	} else {
		// Check whether the index is valid
		if (nIndex < 0 || nIndex >= static_cast<int>(m_sCapabilities.nMaxClipPlanes))
			return false; // Error!

		// Set the clip plane
		m_ppdClipPlane[static_cast<uint32>(nIndex)][0] = fA;
		m_ppdClipPlane[static_cast<uint32>(nIndex)][1] = fB;
		m_ppdClipPlane[static_cast<uint32>(nIndex)][2] = fC;
		m_ppdClipPlane[static_cast<uint32>(nIndex)][3] = fD;
		glClipPlane(GL_CLIP_PLANE0 + nIndex, m_ppdClipPlane[static_cast<uint32>(nIndex)]);
	}

	// Done
	return true;
}

PLRenderer::VertexBuffer *FixedFunctions::GetVertexBuffer(uint32 nStreamNumber, uint32 *pnOffset) const
{
	if (nStreamNumber >= m_sCapabilities.nMaxVertexBufferStreams || !m_ppCurrentVertexBuffer)
		return nullptr; // Error!
	if (pnOffset && m_nVertexBufferOffsets)
		*pnOffset = m_nVertexBufferOffsets[nStreamNumber];
	return m_ppCurrentVertexBuffer[nStreamNumber];
}

bool FixedFunctions::SetVertexBuffer(PLRenderer::VertexBuffer *pVertexBuffer, uint32 nOffset, uint32 nStreamNumber)
{
	// Valid stream number?
	if (nStreamNumber >= m_sCapabilities.nMaxVertexBufferStreams)
		return false; // Error!

	// Is this vertex buffer already set?
	if (nStreamNumber != 0) {
		if (m_ppCurrentVertexBuffer[nStreamNumber] == pVertexBuffer &&
			m_nVertexBufferOffsets[nStreamNumber] == nOffset)
			return false; // Error!
	}

	// Make this vertex buffer to the renderers current one
	m_nVertexBufferOffsets[nStreamNumber] = nOffset;
	m_ppCurrentVertexBuffer[nStreamNumber] = pVertexBuffer;

	// Get extensions instance
	const Extensions &cExtensions = m_pRendererOpenGL->GetContext().GetExtensions();

	// Disable all client states
	if (!pVertexBuffer || !nStreamNumber) {
		for (uint32 i=1; i<m_sCapabilities.nMaxVertexBufferStreams; i++) {
			m_ppCurrentVertexBuffer[i] = nullptr;
			m_nVertexBufferOffsets[i] = 0;
		}
		glDisableClientState(GL_VERTEX_ARRAY);	// PLRenderer::VertexBuffer::Position
		glDisableClientState(GL_NORMAL_ARRAY);	// PLRenderer::VertexBuffer::Normal
		glDisableClientState(GL_COLOR_ARRAY);	// PLRenderer::VertexBuffer::Color
		if (cExtensions.IsGL_EXT_secondary_color())
			glDisableClientState(GL_SECONDARY_COLOR_ARRAY);	// PLRenderer::VertexBuffer::Color
		if (!cExtensions.IsGL_ARB_vertex_program() && cExtensions.IsGL_EXT_fog_coord()) // Hm, no vertex program extension - try to use the fog coordinate extension instead
			glDisableClientState(GL_FOG_COORDINATE_ARRAY_EXT);	// PLRenderer::VertexBuffer::FogCoord
		uint32 nNumOfTextureUnits = m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits;
		for (uint32 j=0; j<nNumOfTextureUnits; j++) {
			if (glClientActiveTextureARB)
				glClientActiveTextureARB(GL_TEXTURE0_ARB+j);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if (cExtensions.IsGL_ARB_vertex_program()) {
			glDisableVertexAttribArrayARB(1);	// PLRenderer::VertexBuffer::BlendWeight
			glDisableVertexAttribArrayARB(5);	// PLRenderer::VertexBuffer::FogCoord
			glDisableVertexAttribArrayARB(6);	// PLRenderer::VertexBuffer::PointSize
			glDisableVertexAttribArrayARB(7);	// PLRenderer::VertexBuffer::BlendIndices
			glDisableVertexAttribArrayARB(14);	// PLRenderer::VertexBuffer::Tangent
			glDisableVertexAttribArrayARB(15);	// PLRenderer::VertexBuffer::Binormal
		}
		if (cExtensions.IsGL_ARB_vertex_buffer_object())
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}

	// Should an vertex buffer be set?
	if (pVertexBuffer) {
		VertexBuffer *pVertexBufferOpenGL = static_cast<VertexBuffer*>(pVertexBuffer);

		// Bind and update the vertex buffer if required
		pVertexBufferOpenGL->BindAndUpdate();

		// Setup buffer offset
		const uint32 nVertexSize  = pVertexBufferOpenGL->GetVertexSize();
		const uint32 nBytesOffset = nVertexSize*nOffset; // Get the vertex offset in bytes
		const uint8 *pData		  = pVertexBufferOpenGL->GetOpenGLVertexBuffer() ? nullptr : pVertexBufferOpenGL->GetDynamicBuffer();

		// Define an offset helper macro just used inside this function
		#define BUFFER_OFFSET(i) (reinterpret_cast<const char*>(pData)+(i+nBytesOffset))

		// Set vertex elements
		for (uint32 i=0; i<pVertexBufferOpenGL->GetNumOfVertexAttributes(); i++) {
			// Get vertex attribute
			const VertexBuffer::Attribute &cAttribute = *pVertexBufferOpenGL->GetVertexAttribute(i);

			// Set declaration
			switch (cAttribute.nSemantic) {
				case VertexBuffer::Position: // Positions
					glVertexPointer(cAttribute.nComponentsAPI, cAttribute.nTypeAPI, nVertexSize, BUFFER_OFFSET(cAttribute.nOffset));
					glEnableClientState(GL_VERTEX_ARRAY);
					break;

				case VertexBuffer::BlendWeight: // Blend weights
					if (cExtensions.IsGL_ARB_vertex_program()) {
						glVertexAttribPointerARB(
							1,								// Weights are vertex attribute 1
							cAttribute.nComponentsAPI,
							cAttribute.nTypeAPI,
							GL_FALSE,						// Normalization disabled
							nVertexSize,
							BUFFER_OFFSET(cAttribute.nOffset));
						glEnableVertexAttribArrayARB(1);
					}
					break;

				case VertexBuffer::Normal: // Normals
					glNormalPointer(cAttribute.nTypeAPI, nVertexSize, BUFFER_OFFSET(cAttribute.nOffset));
					glEnableClientState(GL_NORMAL_ARRAY);
					break;

				case VertexBuffer::Color: // Colors
					if (cAttribute.nChannel) {
						if (cExtensions.IsGL_EXT_secondary_color()) {
							glSecondaryColorPointerEXT(cAttribute.nComponentsAPI, cAttribute.nTypeAPI, nVertexSize, BUFFER_OFFSET(cAttribute.nOffset));
							glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
						}
					} else {
						glColorPointer(cAttribute.nComponentsAPI, cAttribute.nTypeAPI, nVertexSize, BUFFER_OFFSET(cAttribute.nOffset));
						glEnableClientState(GL_COLOR_ARRAY);
					}
					break;

				case VertexBuffer::FogCoord: // Fog coordinates
					if (cExtensions.IsGL_ARB_vertex_program()) {
						glVertexAttribPointerARB(
							5,								// Fog coordinates are vertex attribute 5
							cAttribute.nComponentsAPI,
							cAttribute.nTypeAPI,
							GL_FALSE,						// Normalization disabled
							nVertexSize,
							BUFFER_OFFSET(cAttribute.nOffset));
						glEnableVertexAttribArrayARB(5);

					// Hm, no vertex program extension - try to use the fog coordinate extension instead (type MUST be Float1!)
					} else if (cExtensions.IsGL_EXT_fog_coord() && cAttribute.nTypeAPI == VertexBuffer::Float1) {
						glFogCoordPointerEXT(cAttribute.nTypeAPI, nVertexSize, BUFFER_OFFSET(cAttribute.nOffset));
						glEnableClientState(GL_FOG_COORDINATE_ARRAY_EXT);
					}
					break;

				case VertexBuffer::PointSize: // Point sprite sizes
					if (cExtensions.IsGL_ARB_vertex_program()) {
						glVertexAttribPointerARB(
							6,								// Point sprite size are vertex attribute 6
							cAttribute.nComponentsAPI,
							cAttribute.nTypeAPI,
							GL_FALSE,						// Normalization disabled
							nVertexSize,
							BUFFER_OFFSET(cAttribute.nOffset));
						glEnableVertexAttribArrayARB(6);
					}
					break;

				case VertexBuffer::BlendIndices: // Blend indices
					if (cExtensions.IsGL_ARB_vertex_program()) {
						glVertexAttribPointerARB(
							7,								// Matrix indices are vertex attribute 7
							cAttribute.nComponentsAPI,
							cAttribute.nTypeAPI,
							GL_FALSE,						// Normalization disabled
							nVertexSize,
							BUFFER_OFFSET(cAttribute.nOffset));
						glEnableVertexAttribArrayARB(7);
					}
					break;

				case VertexBuffer::TexCoord: // Texture coordinates
				{
					// Set texture coordinate layer
					const uint32 nChannel			= cAttribute.nChannel;
					const uint32 nNumOfTextureUnits = m_pRendererOpenGL->GetCapabilities().nMaxTextureUnits;
					if (nChannel < nNumOfTextureUnits) {
						if (!cAttribute.nChannel) {
							for (uint32 j=0; j<nNumOfTextureUnits; j++) {
								if (glClientActiveTextureARB)
									glClientActiveTextureARB(GL_TEXTURE0_ARB+j);
								glTexCoordPointer(cAttribute.nComponentsAPI, cAttribute.nTypeAPI, nVertexSize, BUFFER_OFFSET(cAttribute.nOffset));
								glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							}
						} else {
							if (glClientActiveTextureARB)
								glClientActiveTextureARB(GL_TEXTURE0_ARB+cAttribute.nChannel);
							glTexCoordPointer(cAttribute.nComponentsAPI, cAttribute.nTypeAPI, nVertexSize, BUFFER_OFFSET(cAttribute.nOffset));
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						}
					}
					break;
				}

				case VertexBuffer::Tangent: // Tangents
					if (cExtensions.IsGL_ARB_vertex_program()) {
						glVertexAttribPointerARB(
							14,								// Tangent are vertex attribute 14
							cAttribute.nComponentsAPI,
							cAttribute.nTypeAPI,
							GL_FALSE,						// Normalization disabled
							nVertexSize,
							BUFFER_OFFSET(cAttribute.nOffset));
						glEnableVertexAttribArrayARB(14);
					}
					break;

				case VertexBuffer::Binormal: // Binormals
					if (cExtensions.IsGL_ARB_vertex_program()) {
						glVertexAttribPointerARB(
							15,								// Binormal are vertex attribute 15
							cAttribute.nComponentsAPI,
							cAttribute.nTypeAPI,
							GL_FALSE,						// Normalization disabled
							nVertexSize,
							BUFFER_OFFSET(cAttribute.nOffset));
						glEnableVertexAttribArrayARB(15);
					}
					break;
			}
		}

		// Undefine your offset helper macro because its just used inside this function
		#undef BUFFER_OFFSET
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
