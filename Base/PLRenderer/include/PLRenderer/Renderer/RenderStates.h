/*********************************************************\
 *  File: RenderStates.h                                 *
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


#ifndef __PLRENDERER_RENDERSTATES_H__
#define __PLRENDERER_RENDERSTATES_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLRenderer/PLRenderer.h"
#include "PLRenderer/Renderer/Types.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Render states RTTI wrapper class
*/
class RenderStates : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLRENDERER_API)
		// Attributes
			// Modes
		pl_attribute_getset(RenderStates,	FillMode,				Fill::Enum,			Fill::Solid,				ReadWrite)
		pl_attribute_getset(RenderStates,	CullMode,				Cull::Enum,			Cull::CCW,					ReadWrite)
			// Z buffer
		pl_attribute_getset(RenderStates,	ZEnable,				bool,				true,						ReadWrite)
		pl_attribute_getset(RenderStates,	ZWriteEnable,			bool,				true,						ReadWrite)
		pl_attribute_getset(RenderStates,	ZFunc,					Compare::Enum,		Compare::LessEqual,			ReadWrite)
		pl_attribute_getset(RenderStates,	ZBias,					float,				0.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	SlopeScaleDepthBias,	float,				0.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	DepthBias,				float,				0.0f,						ReadWrite)
			// Blend
		pl_attribute_getset(RenderStates,	BlendEnable,			bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	SrcBlendFunc,			BlendFunc::Enum,	BlendFunc::SrcAlpha,		ReadWrite)
		pl_attribute_getset(RenderStates,	DstBlendFunc,			BlendFunc::Enum,	BlendFunc::InvSrcAlpha,		ReadWrite)
			// Stencil
		pl_attribute_getset(RenderStates,	StencilEnable,			bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	StencilFunc,			Compare::Enum,		Compare::Always,			ReadWrite)
		pl_attribute_getset(RenderStates,	StencilRef,				PLCore::uint32,		0,							ReadWrite)
		pl_attribute_getset(RenderStates,	StencilMask,			PLCore::uint32,		0xFFFFFFFF,					ReadWrite)
		pl_attribute_getset(RenderStates,	StencilFail,			StencilOp::Enum,	StencilOp::Keep,			ReadWrite)
		pl_attribute_getset(RenderStates,	StencilZFail,			StencilOp::Enum,	StencilOp::Keep,			ReadWrite)
		pl_attribute_getset(RenderStates,	StencilPass,			StencilOp::Enum,	StencilOp::Keep,			ReadWrite)
		pl_attribute_getset(RenderStates,	TwoSidedStencilMode,	bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	CCWStencilFunc,			Compare::Enum,		Compare::Always,			ReadWrite)
		pl_attribute_getset(RenderStates,	CCWStencilFail,			StencilOp::Enum,	StencilOp::Keep,			ReadWrite)
		pl_attribute_getset(RenderStates,	CCWStencilZFail,		StencilOp::Enum,	StencilOp::Keep,			ReadWrite)
		pl_attribute_getset(RenderStates,	CCWStencilPass,			StencilOp::Enum,	StencilOp::Keep,			ReadWrite)
			// Point and line
		pl_attribute_getset(RenderStates,	PointSize,				float,				1.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	PointScaleEnable,		bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	PointSizeMin,			float,				1.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	PointSizeMax,			float,				64.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	PointScaleA,			float,				1.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	PointScaleB,			float,				0.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	PointScaleC,			float,				0.0f,						ReadWrite)
		pl_attribute_getset(RenderStates,	LineWidth,				float,				1.0f,						ReadWrite)
			// Tessellation
		pl_attribute_getset(RenderStates,	TessellationFactor,		PLCore::uint32,		1,							ReadWrite)
// [TODO] Linux GCC: "error: changes meaning of ‘TessellationMode’ from ‘class PLRenderer::TessellationMode"
//		pl_attribute_getset(RenderStates,	TessellationMode,		TessellationMode::Enum,	TessellationMode::Discrete,	ReadWrite)
			// Misc
		pl_attribute_getset(RenderStates,	PointSpriteEnable,		bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	DitherEnable,			bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	ScissorTestEnable,		bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	MultisampleEnable,		bool,				true,						ReadWrite)
		pl_attribute_getset(RenderStates,	DepthClamp,				bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	InvCullMode,			bool,				false,						ReadWrite)
		pl_attribute_getset(RenderStates,	FixedFillMode,			Fill::Enum,			Fill::Unknown,				ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		// Modes
		inline Fill::Enum GetFillMode() const;
		inline void SetFillMode(Fill::Enum nValue);
		inline Cull::Enum GetCullMode() const;
		inline void SetCullMode(Cull::Enum nValue);
		// Z buffer
		inline bool GetZEnable() const;
		inline void SetZEnable(bool bValue);
		inline bool GetZWriteEnable() const;
		inline void SetZWriteEnable(bool bValue);
		inline Compare::Enum GetZFunc() const;
		inline void SetZFunc(Compare::Enum nValue);
		inline float GetZBias() const;
		inline void SetZBias(float fValue);
		inline float GetSlopeScaleDepthBias() const;
		inline void SetSlopeScaleDepthBias(float fValue);
		inline float GetDepthBias() const;
		inline void SetDepthBias(float fValue);
		// Blend
		inline bool GetBlendEnable() const;
		inline void SetBlendEnable(bool bValue);
		inline BlendFunc::Enum GetSrcBlendFunc() const;
		inline void SetSrcBlendFunc(BlendFunc::Enum nValue);
		inline BlendFunc::Enum GetDstBlendFunc() const;
		inline void SetDstBlendFunc(BlendFunc::Enum nValue);
		// Stencil
		inline bool GetStencilEnable() const;
		inline void SetStencilEnable(bool bValue);
		inline Compare::Enum GetStencilFunc() const;
		inline void SetStencilFunc(Compare::Enum nValue);
		inline PLCore::uint32 GetStencilRef() const;
		inline void SetStencilRef(PLCore::uint32 nValue);
		inline PLCore::uint32 GetStencilMask() const;
		inline void SetStencilMask(PLCore::uint32 nValue);
		inline StencilOp::Enum GetStencilFail() const;
		inline void SetStencilFail(StencilOp::Enum nValue);
		inline StencilOp::Enum GetStencilZFail() const;
		inline void SetStencilZFail(StencilOp::Enum nValue);
		inline StencilOp::Enum GetStencilPass() const;
		inline void SetStencilPass(StencilOp::Enum nValue);
		inline bool GetTwoSidedStencilMode() const;
		inline void SetTwoSidedStencilMode(bool bValue);
		inline Compare::Enum GetCCWStencilFunc() const;
		inline void SetCCWStencilFunc(Compare::Enum nValue);
		inline StencilOp::Enum GetCCWStencilFail() const;
		inline void SetCCWStencilFail(StencilOp::Enum nValue);
		inline StencilOp::Enum GetCCWStencilZFail() const;
		inline void SetCCWStencilZFail(StencilOp::Enum nValue);
		inline StencilOp::Enum GetCCWStencilPass() const;
		inline void SetCCWStencilPass(StencilOp::Enum nValue);
		// Point and line
		inline float GetPointSize() const;
		inline void SetPointSize(float fValue);
		inline bool GetPointScaleEnable() const;
		inline void SetPointScaleEnable(bool bValue);
		inline float GetPointSizeMin() const;
		inline void SetPointSizeMin(float fValue);
		inline float GetPointSizeMax() const;
		inline void SetPointSizeMax(float fValue);
		inline float GetPointScaleA() const;
		inline void SetPointScaleA(float fValue);
		inline float GetPointScaleB() const;
		inline void SetPointScaleB(float fValue);
		inline float GetPointScaleC() const;
		inline void SetPointScaleC(float fValue);
		inline float GetLineWidth() const;
		inline void SetLineWidth(float fValue);
		// Tessellation
		inline PLCore::uint32 GetTessellationFactor() const;
		inline void SetTessellationFactor(PLCore::uint32 nValue);
		/*
		// [TODO] Linux GCC: "error: changes meaning of ‘TessellationMode’ from ‘class PLRenderer::TessellationMode"
		inline TessellationMode::Enum GetTessellationMode() const;
		inline void SetTessellationMode(TessellationMode::Enum nValue);
		*/
		// Misc
		inline bool GetPointSpriteEnable() const;
		inline void SetPointSpriteEnable(bool bValue);
		inline bool GetDitherEnable() const;
		inline void SetDitherEnable(bool bValue);
		inline bool GetScissorTestEnable() const;
		inline void SetScissorTestEnable(bool bValue);
		inline bool GetMultisampleEnable() const;
		inline void SetMultisampleEnable(bool bValue);
		inline bool GetDepthClamp() const;
		inline void SetDepthClamp(bool bValue);
		inline bool GetInvCullMode() const;
		inline void SetInvCullMode(bool bValue);
		inline Fill::Enum GetFixedFillMode() const;
		inline void SetFixedFillMode(Fill::Enum nValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API RenderStates();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLRENDERER_API RenderStates(const RenderStates &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~RenderStates();

		/**
		*  @brief
		*    Retrieves a render-state value
		*
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the render state enumerated type.
		*
		*  @return
		*    The value of the queried render state variable
		*
		*  @note
		*    - Some settings like stencil, fog and scissor test aren't set
		*      by the material through the default setting
		*/
		inline PLCore::uint32 Get(RenderState::Enum nState) const;

		/**
		*  @brief
		*    Sets a single render-state parameter
		*
		*  @param[in] nState
		*    State variable that is being modified. This parameter can
		*    be any member of the render state enumerated type.
		*  @param[in] nValue
		*    New value for the render state to be set. The meaning of
		*    this parameter is dependent on the value specified for nState.
		*    For example, if nState is Shade, the second parameter
		*    must be one member of the Shade enumerated type. (e.g. Shade::Flat)
		*    You can also set the value to unknown - then this state is ignored.
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		inline bool Set(RenderState::Enum nState, PLCore::uint32 nValue);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This class
		*/
		inline RenderStates &operator =(const RenderStates &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Exported variables
		PLCore::uint32 m_nRS[RenderState::Number];	/**< List of render states (see RenderState) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/RenderStates.inl"


#endif // __PLRENDERER_RENDERSTATES_H__
