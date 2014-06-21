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
#include "PLRendererNull/Renderer.h"
#include "PLRendererNull/VertexBuffer.h"
#include "PLRendererNull/FixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FixedFunctions::FixedFunctions(Renderer &cRendererNull) :
	m_pRendererNull(&cRendererNull),
	m_cColor(Color4::White),
	m_ppnTextureStageState(nullptr),
	m_ppnInternalTextureStageState(nullptr),
	m_ppCurrentVertexBuffer(nullptr),
	m_nVertexBufferOffsets(nullptr),
	m_nVertexOffset(0)
{
	{ // Capabilities
		// Maximum number of active lights
		m_sCapabilities.nMaxActiveLights = 0;

		// Maximum number of clip planes
		m_sCapabilities.nMaxClipPlanes = 0;

		// Vertex buffer fog coordinates supported?
		m_sCapabilities.bVertexBufferFogCoord = true;

		// Maximum vertex buffer streams
		m_sCapabilities.nMaxVertexBufferStreams = 8;
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
		m_ppnTextureStageState = new uint32*[m_pRendererNull->GetCapabilities().nMaxTextureUnits];
		for (uint32 i=0; i<m_pRendererNull->GetCapabilities().nMaxTextureUnits; i++) {
			m_ppnTextureStageState[i] = new uint32[TextureStage::Number];
			MemoryManager::Set(m_ppnTextureStageState[i], -1, sizeof(uint32)*TextureStage::Number);
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
			for (uint32 i=0; i<m_pRendererNull->GetCapabilities().nMaxTextureUnits; i++)
				delete [] m_ppnTextureStageState[i];
			delete [] m_ppnTextureStageState;
			m_ppnTextureStageState = nullptr;
		}
	}

	{ // Vertex buffer states
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
}

void FixedFunctions::ResetTransformStates()
{
	// Reset projection matrix
	m_mProj.SetIdentity();

	// Reset view matrix
	m_mView.SetIdentity();

	// Reset world matrix
	m_mWorld.SetIdentity();

	// Reset texture matrices
	for (uint32 i=0; i<m_pRendererNull->GetCapabilities().nMaxTextureUnits; i++)
		m_mTextureMatrix[i].SetIdentity();
}

const Matrix4x4 &FixedFunctions::GetTransformState(Transform::Enum nState, bool bOriginal) const
{
	// Check if the state is a valid Transform::Enum member
	if (nState < Transform::Number) {
		// Get transform state
		switch (nState) {
			case Transform::Projection:
				return m_mProj;

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
				if (nStage < m_pRendererNull->GetCapabilities().nMaxTextureUnits)
					return m_mTextureMatrix[nStage];
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
		// Set matrix
		switch (nState) {
			case Transform::Projection:
				m_mProj = mTrans;
				return true; // Done

			case Transform::View: // View transformation matrix
				m_mView = mTrans;
				return true; // Done

			case Transform::World: // World transformation matrix
				m_mWorld = mTrans;
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
				if (nStage < m_pRendererNull->GetCapabilities().nMaxTextureUnits) {
					m_mTextureMatrix[nStage] = mTrans;
					return true; // Done
				}
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
		// Set matrix
		switch (nState) {
			case Transform::Projection: // Projection transformation matrix
				m_mProj = mTrans;
				return true; // Done

			case Transform::View: // View transformation matrix
				m_mView = mTrans;
				return true; // Done

			case Transform::World: // World transformation matrix
				m_mWorld = mTrans;
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
				if (nStage < m_pRendererNull->GetCapabilities().nMaxTextureUnits) {
					m_mTextureMatrix[nStage] = mTrans;
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
	for (uint32 nStage=0; nStage<m_pRendererNull->GetCapabilities().nMaxTextureUnits; nStage++) {
		for (uint32 i=0; i<TextureStage::Number; i++)
			SetTextureStageState(nStage, static_cast<TextureStage::Enum>(i), m_nDefaultTextureStageState[i]);
	}
}

int FixedFunctions::GetTextureStageState(uint32 nStage, TextureStage::Enum nState) const
{
	// Check if the stage is correct and check if the state is a valid texture stage member
	return (nStage < m_pRendererNull->GetCapabilities().nMaxTextureUnits && nState < TextureStage::Number) ? m_ppnTextureStageState[nStage][nState] : -1;
}

uint32 FixedFunctions::GetDefaultMaterialState(MaterialState::Enum nState) const
{
	return (nState < MaterialState::Number) ? m_nDefaultMaterialState[nState] : 0;
}

bool FixedFunctions::SetTextureStageState(uint32 nStage, TextureStage::Enum nState, uint32 nValue)
{
	// Check if the stage is correct and check if the state is a valid texture stage member
	if (nStage < m_pRendererNull->GetCapabilities().nMaxTextureUnits && nState < TextureStage::Number) {
		// Check if this texture stage state is already set to this value
		if (m_ppnTextureStageState[nStage][nState] != nValue) {
			// Set the texture stage state
			m_ppnTextureStageState[nStage][nState] = nValue;
		}

		// All went fine
		return true;
	} else {
		// Error!
		return false;
	}
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
	if (nLightID >= m_sCapabilities.nMaxActiveLights)
		return false; // Error!

	// Get light state
	return false;
}

bool FixedFunctions::SetLightEnabled(uint32 nLightID, bool bEnabled)
{
	// Check if the index is correct
	if (nLightID >= m_sCapabilities.nMaxActiveLights)
		return false; // Error!

	// Enable/disable the light
	return true;
}

bool FixedFunctions::GetLight(uint32 nLightID, Light &sLight) const
{
	// Check if the index is correct
	if (nLightID >= m_sCapabilities.nMaxActiveLights)
		return false; // Error!

	// Done
	return true;
}

bool FixedFunctions::SetLight(uint32 nLightID, const Light &sLight)
{
	// Check if the index is correct
	if (nLightID >= m_sCapabilities.nMaxActiveLights)
		return false; // Error!

	// Done
	return true;
}

bool FixedFunctions::IsClipPlaneEnabled(uint8 nIndex) const
{
	// Check whether the index and the device is valid
	if (nIndex >= m_sCapabilities.nMaxClipPlanes)
		return false; // Done

	// Done
	return false;
}

bool FixedFunctions::SetClipPlaneEnabled(char nIndex, bool bEnable)
{
	// Set all available clip planes to this setting?
	if (nIndex < 0) {
		for (uint8 i=0; i<m_sCapabilities.nMaxClipPlanes; i++)
			SetClipPlaneEnabled(i, bEnable);
	} else {
		// Check whether the index and the device is valid
		if (nIndex >= static_cast<int>(m_sCapabilities.nMaxClipPlanes))
			return false; // Error!
	}

	// Done
	return true;
}

bool FixedFunctions::GetClipPlane(uint8 nIndex, float &fA, float &fB, float &fC, float &fD) const
{
	// Check whether the index and the device is valid
	if (nIndex >= m_sCapabilities.nMaxClipPlanes)
		return false; // Error!

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
		// Check whether the index and the device is valid
		if (nIndex >= static_cast<int>(m_sCapabilities.nMaxClipPlanes))
			return false; // Error!
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
	PLRenderer::VertexBuffer *pT = m_ppCurrentVertexBuffer[nStreamNumber];
	m_ppCurrentVertexBuffer[nStreamNumber] = pVertexBuffer;
	m_nVertexOffset = nOffset;

	// Disable all client states
	if (!pVertexBuffer || !nStreamNumber) {
		for (uint32 i=1; i<m_sCapabilities.nMaxVertexBufferStreams; i++) {
			m_ppCurrentVertexBuffer[i] = nullptr;
			m_nVertexBufferOffsets[i] = 0;
		}
	}

	// Should an vertex buffer be set?
	if (pVertexBuffer) {
		// Yes, make it current
		if (!static_cast<VertexBuffer*>(pVertexBuffer)->MakeCurrent(nOffset)) {
			m_ppCurrentVertexBuffer[nStreamNumber] = pT;

			// Error!
			return false;
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull
