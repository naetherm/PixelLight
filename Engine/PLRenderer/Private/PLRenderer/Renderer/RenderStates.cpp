/*********************************************************\
 *  File: RendersStates.cpp                              *
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
#include "PLRenderer/Renderer/RenderStates.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(RenderStates)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
RenderStates::RenderStates() :
	FillMode(this),
	CullMode(this),
	ZEnable(this),
	ZWriteEnable(this),
	ZFunc(this),
	ZBias(this),
	SlopeScaleDepthBias(this),
	DepthBias(this),
	BlendEnable(this),
	SrcBlendFunc(this),
	DstBlendFunc(this),
	StencilEnable(this),
	StencilFunc(this),
	StencilRef(this),
	StencilMask(this),
	StencilFail(this),
	StencilZFail(this),
	StencilPass(this),
	TwoSidedStencilMode(this),
	CCWStencilFunc(this),
	CCWStencilFail(this),
	CCWStencilZFail(this),
	CCWStencilPass(this),
	PointSize(this),
	PointScaleEnable(this),
	PointSizeMin(this),
	PointSizeMax(this),
	PointScaleA(this),
	PointScaleB(this),
	PointScaleC(this),
	LineWidth(this),
	TessellationFactor(this),
// [TODO] Linux GCC: "error: changes meaning of ‘TessellationMode’ from ‘class PLRenderer::TessellationMode"
//	TessellationMode(this),
	PointSpriteEnable(this),
	DitherEnable(this),
	ScissorTestEnable(this),
	MultisampleEnable(this),
	DepthClamp(this),
	InvCullMode(this),
	FixedFillMode(this)
{
	// Set default renderer states
	m_nRS[RenderState::FillMode] = Fill::Solid;
	m_nRS[RenderState::CullMode] = Cull::CCW;
	// Z buffer
	m_nRS[RenderState::ZEnable]             = true;
	m_nRS[RenderState::ZWriteEnable]        = true;
	m_nRS[RenderState::ZFunc]               = Compare::LessEqual;
	m_nRS[RenderState::ZBias]               = Tools::FloatToUInt32(0.0f);
	m_nRS[RenderState::SlopeScaleDepthBias] = Tools::FloatToUInt32(0.0f);
	m_nRS[RenderState::DepthBias]           = Tools::FloatToUInt32(0.0f);
	// Blend
	m_nRS[RenderState::BlendEnable]  = false;
	m_nRS[RenderState::SrcBlendFunc] = BlendFunc::SrcAlpha;
	m_nRS[RenderState::DstBlendFunc] = BlendFunc::InvSrcAlpha;
	// Stencil
	m_nRS[RenderState::StencilEnable]       = false;
	m_nRS[RenderState::StencilFunc]         = Compare::Always;
	m_nRS[RenderState::StencilRef]          = 0;
	m_nRS[RenderState::StencilMask]         = 0xFFFFFFFF;
	m_nRS[RenderState::StencilFail]         = StencilOp::Keep;
	m_nRS[RenderState::StencilZFail]        = StencilOp::Keep;
	m_nRS[RenderState::StencilPass]         = StencilOp::Keep;
	m_nRS[RenderState::TwoSidedStencilMode] = false;
	m_nRS[RenderState::CCWStencilFunc]      = Compare::Always;
	m_nRS[RenderState::CCWStencilFail]      = StencilOp::Keep;
	m_nRS[RenderState::CCWStencilZFail]     = StencilOp::Keep;
	m_nRS[RenderState::CCWStencilPass]      = StencilOp::Keep;
	// Point and line
	m_nRS[RenderState::PointSize]        = Tools::FloatToUInt32(1.0f);
	m_nRS[RenderState::PointScaleEnable] = false;
	m_nRS[RenderState::PointSizeMin]     = Tools::FloatToUInt32(1.0f);
	m_nRS[RenderState::PointSizeMax]     = Tools::FloatToUInt32(64.0f);
	m_nRS[RenderState::PointScaleA]      = Tools::FloatToUInt32(1.0f);
	m_nRS[RenderState::PointScaleB]      = Tools::FloatToUInt32(0.0f);
	m_nRS[RenderState::PointScaleC]      = Tools::FloatToUInt32(0.0f);
	m_nRS[RenderState::LineWidth]        = Tools::FloatToUInt32(1.0f);
	// Tessellation
	m_nRS[RenderState::TessellationFactor] = 1;
// [TODO] Linux GCC: "error: changes meaning of ‘TessellationMode’ from ‘class PLRenderer::TessellationMode"
//	m_nRS[RenderState::TessellationMode]   = TessellationMode::Discrete;
	// Misc
	m_nRS[RenderState::PointSpriteEnable] = false;
	m_nRS[RenderState::DitherEnable]      = false;
	m_nRS[RenderState::ScissorTestEnable] = false;
	m_nRS[RenderState::MultisampleEnable] = false;
	m_nRS[RenderState::DepthClamp]        = false;
	m_nRS[RenderState::InvCullMode]       = false;
	m_nRS[RenderState::FixedFillMode]     = Fill::Unknown;
}

/**
*  @brief
*    Copy constructor
*/
RenderStates::RenderStates(const RenderStates &cSource) :
	FillMode(this),
	CullMode(this),
	ZEnable(this),
	ZWriteEnable(this),
	ZFunc(this),
	ZBias(this),
	SlopeScaleDepthBias(this),
	DepthBias(this),
	BlendEnable(this),
	SrcBlendFunc(this),
	DstBlendFunc(this),
	StencilEnable(this),
	StencilFunc(this),
	StencilRef(this),
	StencilMask(this),
	StencilFail(this),
	StencilZFail(this),
	StencilPass(this),
	TwoSidedStencilMode(this),
	CCWStencilFunc(this),
	CCWStencilFail(this),
	CCWStencilZFail(this),
	CCWStencilPass(this),
	PointSize(this),
	PointScaleEnable(this),
	PointSizeMin(this),
	PointSizeMax(this),
	PointScaleA(this),
	PointScaleB(this),
	PointScaleC(this),
	LineWidth(this),
	TessellationFactor(this),
// [TODO] Linux GCC: "error: changes meaning of ‘TessellationMode’ from ‘class PLRenderer::TessellationMode"
//	TessellationMode(this),
	PointSpriteEnable(this),
	DitherEnable(this),
	ScissorTestEnable(this),
	MultisampleEnable(this),
	DepthClamp(this),
	InvCullMode(this),
	FixedFillMode(this)
{
	// Copy states
	for (uint32 i=0; i<RenderState::Number; i++)
		m_nRS[i] = cSource.m_nRS[i];
}

/**
*  @brief
*    Destructor
*/
RenderStates::~RenderStates()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
