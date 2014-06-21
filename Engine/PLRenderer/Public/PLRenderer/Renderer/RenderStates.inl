/*********************************************************\
 *  File: RendersStates.inl                              *
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
// Modes
inline Fill::Enum RenderStates::GetFillMode() const
{
	return static_cast<Fill::Enum>(m_nRS[RenderState::FillMode]);
}

inline void RenderStates::SetFillMode(Fill::Enum nValue)
{
	m_nRS[RenderState::FillMode] = nValue;
}

inline Cull::Enum RenderStates::GetCullMode() const
{
	return static_cast<Cull::Enum>(m_nRS[RenderState::CullMode]);
}

inline void RenderStates::SetCullMode(Cull::Enum nValue)
{
	m_nRS[RenderState::CullMode] = nValue;
}

// Z buffer
inline bool RenderStates::GetZEnable() const
{
	return (m_nRS[RenderState::ZEnable] != 0);
}

inline void RenderStates::SetZEnable(bool bValue)
{
	m_nRS[RenderState::ZEnable] = bValue;
}

inline bool RenderStates::GetZWriteEnable() const
{
	return (m_nRS[RenderState::ZWriteEnable] != 0);
}

inline void RenderStates::SetZWriteEnable(bool bValue)
{
	m_nRS[RenderState::ZWriteEnable] = bValue;
}

inline Compare::Enum RenderStates::GetZFunc() const
{
	return static_cast<Compare::Enum>(m_nRS[RenderState::ZFunc]);
}

inline void RenderStates::SetZFunc(Compare::Enum nValue)
{
	m_nRS[RenderState::ZFunc] = nValue;
}

inline float RenderStates::GetZBias() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::ZBias]);
}

inline void RenderStates::SetZBias(float fValue)
{
	m_nRS[RenderState::ZBias] = PLCore::Tools::FloatToUInt32(fValue);
}

inline float RenderStates::GetSlopeScaleDepthBias() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::SlopeScaleDepthBias]);
}

inline void RenderStates::SetSlopeScaleDepthBias(float fValue)
{
	m_nRS[RenderState::SlopeScaleDepthBias] = PLCore::Tools::FloatToUInt32(fValue);
}

inline float RenderStates::GetDepthBias() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::DepthBias]);
}

inline void RenderStates::SetDepthBias(float fValue)
{
	m_nRS[RenderState::DepthBias] = PLCore::Tools::FloatToUInt32(fValue);
}

// Blend
inline bool RenderStates::GetBlendEnable() const
{
	return (m_nRS[RenderState::BlendEnable] != 0);
}

inline void RenderStates::SetBlendEnable(bool bValue)
{
	m_nRS[RenderState::BlendEnable] = bValue;
}

inline BlendFunc::Enum RenderStates::GetSrcBlendFunc() const
{
	return static_cast<BlendFunc::Enum>(m_nRS[RenderState::SrcBlendFunc]);
}

inline void RenderStates::SetSrcBlendFunc(BlendFunc::Enum nValue)
{
	m_nRS[RenderState::SrcBlendFunc] = nValue;
}

inline BlendFunc::Enum RenderStates::GetDstBlendFunc() const
{
	return static_cast<BlendFunc::Enum>(m_nRS[RenderState::DstBlendFunc]);
}

inline void RenderStates::SetDstBlendFunc(BlendFunc::Enum nValue)
{
	m_nRS[RenderState::DstBlendFunc] = nValue;
}

// Stencil
inline bool RenderStates::GetStencilEnable() const
{
	return (m_nRS[RenderState::StencilEnable] != 0);
}

inline void RenderStates::SetStencilEnable(bool bValue)
{
	m_nRS[RenderState::StencilEnable] = bValue;
}

inline Compare::Enum RenderStates::GetStencilFunc() const
{
	return static_cast<Compare::Enum>(m_nRS[RenderState::StencilFunc]);
}

inline void RenderStates::SetStencilFunc(Compare::Enum nValue)
{
	m_nRS[RenderState::StencilFunc] = nValue;
}

inline PLCore::uint32 RenderStates::GetStencilRef() const
{
	return m_nRS[RenderState::StencilRef];
}

inline void RenderStates::SetStencilRef(PLCore::uint32 nValue)
{
	m_nRS[RenderState::StencilRef] = nValue;
}

inline PLCore::uint32 RenderStates::GetStencilMask() const
{
	return m_nRS[RenderState::StencilMask];
}

inline void RenderStates::SetStencilMask(PLCore::uint32 nValue)
{
	m_nRS[RenderState::StencilMask] = nValue;
}

inline StencilOp::Enum RenderStates::GetStencilFail() const
{
	return static_cast<StencilOp::Enum>(m_nRS[RenderState::StencilFail]);
}

inline void RenderStates::SetStencilFail(StencilOp::Enum nValue)
{
	m_nRS[RenderState::StencilFail] = nValue;
}

inline StencilOp::Enum RenderStates::GetStencilZFail() const
{
	return static_cast<StencilOp::Enum>(m_nRS[RenderState::StencilZFail]);
}

inline void RenderStates::SetStencilZFail(StencilOp::Enum nValue)
{
	m_nRS[RenderState::StencilZFail] = nValue;
}

inline StencilOp::Enum RenderStates::GetStencilPass() const
{
	return static_cast<StencilOp::Enum>(m_nRS[RenderState::StencilPass]);
}

inline void RenderStates::SetStencilPass(StencilOp::Enum nValue)
{
	m_nRS[RenderState::StencilPass] = nValue;
}

inline bool RenderStates::GetTwoSidedStencilMode() const
{
	return (m_nRS[RenderState::TwoSidedStencilMode] != 0);
}

inline void RenderStates::SetTwoSidedStencilMode(bool bValue)
{
	m_nRS[RenderState::TwoSidedStencilMode] = bValue;
}

inline Compare::Enum RenderStates::GetCCWStencilFunc() const
{
	return static_cast<Compare::Enum>(m_nRS[RenderState::CCWStencilFunc]);
}

inline void RenderStates::SetCCWStencilFunc(Compare::Enum nValue)
{
	m_nRS[RenderState::CCWStencilFunc] = nValue;
}

inline StencilOp::Enum RenderStates::GetCCWStencilFail() const
{
	return static_cast<StencilOp::Enum>(m_nRS[RenderState::CCWStencilFail]);
}

inline void RenderStates::SetCCWStencilFail(StencilOp::Enum nValue)
{
	m_nRS[RenderState::CCWStencilFail] = nValue;
}

inline StencilOp::Enum RenderStates::GetCCWStencilZFail() const
{
	return static_cast<StencilOp::Enum>(m_nRS[RenderState::CCWStencilZFail]);
}

inline void RenderStates::SetCCWStencilZFail(StencilOp::Enum nValue)
{
	m_nRS[RenderState::CCWStencilZFail] = nValue;
}

inline StencilOp::Enum RenderStates::GetCCWStencilPass() const
{
	return static_cast<StencilOp::Enum>(m_nRS[RenderState::CCWStencilPass]);
}

inline void RenderStates::SetCCWStencilPass(StencilOp::Enum nValue)
{
	m_nRS[RenderState::CCWStencilPass] = nValue;
}

// Point and line
inline float RenderStates::GetPointSize() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::PointSize]);
}

inline void RenderStates::SetPointSize(float fValue)
{
	m_nRS[RenderState::PointSize] = PLCore::Tools::FloatToUInt32(fValue);
}

inline bool RenderStates::GetPointScaleEnable() const
{
	return (m_nRS[RenderState::PointScaleEnable] != 0);
}

inline void RenderStates::SetPointScaleEnable(bool bValue)
{
	m_nRS[RenderState::PointScaleEnable] = bValue;
}

inline float RenderStates::GetPointSizeMin() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::PointSizeMin]);
}

inline void RenderStates::SetPointSizeMin(float fValue)
{
	m_nRS[RenderState::PointSizeMin] = PLCore::Tools::FloatToUInt32(fValue);
}

inline float RenderStates::GetPointSizeMax() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::PointSizeMax]);
}

inline void RenderStates::SetPointSizeMax(float fValue)
{
	m_nRS[RenderState::PointSizeMax] = PLCore::Tools::FloatToUInt32(fValue);
}

inline float RenderStates::GetPointScaleA() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::PointScaleA]);
}

inline void RenderStates::SetPointScaleA(float fValue)
{
	m_nRS[RenderState::PointScaleA] = PLCore::Tools::FloatToUInt32(fValue);
}

inline float RenderStates::GetPointScaleB() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::PointScaleB]);
}

inline void RenderStates::SetPointScaleB(float fValue)
{
	m_nRS[RenderState::PointScaleB] = PLCore::Tools::FloatToUInt32(fValue);
}

inline float RenderStates::GetPointScaleC() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::PointScaleC]);
}

inline void RenderStates::SetPointScaleC(float fValue)
{
	m_nRS[RenderState::PointScaleC] = PLCore::Tools::FloatToUInt32(fValue);
}

inline float RenderStates::GetLineWidth() const
{
	return PLCore::Tools::UInt32ToFloat(m_nRS[RenderState::LineWidth]);
}

inline void RenderStates::SetLineWidth(float fValue)
{
	m_nRS[RenderState::LineWidth] = PLCore::Tools::FloatToUInt32(fValue);
}

// Tessellation
inline PLCore::uint32 RenderStates::GetTessellationFactor() const
{
	return m_nRS[RenderState::TessellationFactor];
}

inline void RenderStates::SetTessellationFactor(PLCore::uint32 nValue)
{
	m_nRS[RenderState::TessellationFactor] = nValue;
}
/*
// [TODO] Linux GCC: "error: changes meaning of ‘TessellationMode’ from ‘class PLRenderer::TessellationMode"
inline TessellationMode::Enum RenderStates::GetTessellationMode() const
{
	return (TessellationMode::Enum)m_nRS[RenderState::TessellationMode];
}

inline void RenderStates::SetTessellationMode(TessellationMode::Enum nValue)
{
	m_nRS[RenderState::TessellationMode] = nValue;
}
*/
// Misc
inline bool RenderStates::GetPointSpriteEnable() const
{
	return (m_nRS[RenderState::PointSpriteEnable] != 0);
}

inline void RenderStates::SetPointSpriteEnable(bool bValue)
{
	m_nRS[RenderState::PointSpriteEnable] = bValue;
}

inline bool RenderStates::GetDitherEnable() const
{
	return (m_nRS[RenderState::DitherEnable] != 0);
}

inline void RenderStates::SetDitherEnable(bool bValue)
{
	m_nRS[RenderState::DitherEnable] = bValue;
}

inline bool RenderStates::GetScissorTestEnable() const
{
	return (m_nRS[RenderState::ScissorTestEnable] != 0);
}

inline void RenderStates::SetScissorTestEnable(bool bValue)
{
	m_nRS[RenderState::ScissorTestEnable] = bValue;
}

inline bool RenderStates::GetMultisampleEnable() const
{
	return (m_nRS[RenderState::MultisampleEnable] != 0);
}

inline void RenderStates::SetMultisampleEnable(bool bValue)
{
	m_nRS[RenderState::MultisampleEnable] = bValue;
}

inline bool RenderStates::GetDepthClamp() const
{
	return (m_nRS[RenderState::DepthClamp] != 0);
}

inline void RenderStates::SetDepthClamp(bool bValue)
{
	m_nRS[RenderState::DepthClamp] = bValue;
}

inline bool RenderStates::GetInvCullMode() const
{
	return (m_nRS[RenderState::InvCullMode] != 0);
}

inline void RenderStates::SetInvCullMode(bool bValue)
{
	m_nRS[RenderState::InvCullMode] = bValue;
}

inline Fill::Enum RenderStates::GetFixedFillMode() const
{
	return static_cast<Fill::Enum>(m_nRS[RenderState::FixedFillMode]);
}

inline void RenderStates::SetFixedFillMode(Fill::Enum nValue)
{
	m_nRS[RenderState::FixedFillMode] = nValue;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Retrieves a render-state value
*/
inline PLCore::uint32 RenderStates::Get(RenderState::Enum nState) const
{
	// Check whether the state is a valid render state member
	return (nState < RenderState::Number) ? m_nRS[nState] : 0;
}

/**
*  @brief
*    Sets a single render-state parameter
*/
inline bool RenderStates::Set(RenderState::Enum nState, PLCore::uint32 nValue)
{
	// Check whether the state is a valid render state member
	if (nState >= RenderState::Number)
		return false; // Error!

	// Set render state value
	m_nRS[nState] = nValue;

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
inline RenderStates &RenderStates::operator =(const RenderStates &cSource)
{
	// Copy states
	for (PLCore::uint32 i=0; i<RenderState::Number; i++)
		m_nRS[i] = cSource.m_nRS[i];

	// Return this
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
