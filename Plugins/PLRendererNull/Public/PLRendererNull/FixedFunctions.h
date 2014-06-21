/*********************************************************\
 *  File: FixedFunctions.h                               *
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


#ifndef __PLRENDERERNULL_FIXEDFUNCTIONS_H__
#define __PLRENDERERNULL_FIXEDFUNCTIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Matrix4x4.h>
#include <PLRenderer/Renderer/FixedFunctions.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Renderer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null fixed functions renderer interface implementation
*/
class FixedFunctions : public PLRenderer::FixedFunctions {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRendererNull
		*    Null renderer instance
		*/
		FixedFunctions(Renderer &cRendererNull);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FixedFunctions();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Renderer		   *m_pRendererNull;									/**< Null renderer instance, always valid! */
		Capabilities	    m_sCapabilities;									/**< The renderer fixed functions capabilities */
		// Render states
		PLCore::uint32   	m_nDefaultRenderState[RenderState::Number];			/**< Default renderer states */
		PLCore::uint32   	m_nRenderState[RenderState::Number];				/**< List of render states (see RenderState) */
		PLGraphics::Color4 	m_cColor;											/**< Current color */
		// Transform states
		PLMath::Matrix4x4 	m_mProj;											/**< Projection transformation matrix */
		PLMath::Matrix4x4 	m_mView;											/**< View transformation matrix */
		PLMath::Matrix4x4 	m_mWorld;											/**< World transformation matrix */
		PLMath::Matrix4x4 	m_mTextureMatrix[8];								/**< Texture transformation matrix */
		// Texture stage states
		PLCore::uint32   	m_nDefaultTextureStageState[TextureStage::Number];	/**< Default texture stage states */
		PLCore::uint32 	  **m_ppnTextureStageState;								/**< List of texture stage states for each stage (see TextureStage) */
		PLCore::uint32 	  **m_ppnInternalTextureStageState;						/**< List of internal texture stage states for each stage (see TextureStage) */
		// Material states
		PLCore::uint32   	m_nDefaultMaterialState[MaterialState::Number];		/**< Default material states */
		PLCore::uint32   	m_nMaterialState[MaterialState::Number];			/**< List of material states */
		// Vertex buffer states
		PLRenderer::VertexBuffer **m_ppCurrentVertexBuffer;						/**< Current vertex buffer */
		PLCore::uint32			  *m_nVertexBufferOffsets;						/**< Current vertex buffer offsets */
		PLCore::uint32			   m_nVertexOffset;								/**< Current vertex offset */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::FixedFunctions functions   ]
	//[-------------------------------------------------------]
	public:
		virtual const Capabilities &GetCapabilities() const override;
		virtual void Reset() override;
		// Render states
		virtual PLCore::uint32 GetDefaultRenderState(RenderState::Enum nState) const override;
		virtual void ResetRenderStates() override;
		virtual int GetRenderState(RenderState::Enum nState) const override;
		virtual bool SetRenderState(RenderState::Enum nState, PLCore::uint32 nValue) override;
		virtual PLGraphics::Color4 GetColor() const override;
		virtual void SetColor(const PLGraphics::Color4 &cColor = PLGraphics::Color4::White) override;
		// Transform states
		virtual const PLMath::Matrix4x4 &GetTransformState(Transform::Enum nState, bool bOriginal = true) const override;
		virtual void ResetTransformStates() override;
		virtual bool SetTransformState(Transform::Enum nState, const PLMath::Matrix3x4 &mTrans) override;
		virtual bool SetTransformState(Transform::Enum nState, const PLMath::Matrix4x4 &mTrans) override;
		// Texture stage states
		virtual PLCore::uint32 GetDefaultTextureStageState(TextureStage::Enum nState) const override;
		virtual void ResetTextureStageStates() override;
		virtual int GetTextureStageState(PLCore::uint32 nStage, TextureStage::Enum nState) const override;
		virtual bool SetTextureStageState(PLCore::uint32 nStage, TextureStage::Enum nState, PLCore::uint32 nValue) override;
		// Material states
		virtual PLCore::uint32 GetDefaultMaterialState(MaterialState::Enum nState) const override;
		virtual void ResetMaterialStates() override;
		virtual int GetMaterialState(MaterialState::Enum nState) const override;
		virtual bool SetMaterialState(MaterialState::Enum nState, PLCore::uint32 nValue) override;
		// Light states
		virtual void GetDefaultLightSettings(Light &sLight) const override;
		virtual void ResetLights() override;
		virtual bool IsLightEnabled(PLCore::uint32 nLightID) const override;
		virtual bool SetLightEnabled(PLCore::uint32 nLightID, bool bEnabled) override;
		virtual bool GetLight(PLCore::uint32 nLightID, Light &sLight) const override;
		virtual bool SetLight(PLCore::uint32 nLightID, const Light &sLight) override;
		// Clip plane states
		virtual bool IsClipPlaneEnabled(PLCore::uint8 nIndex) const override;
		virtual bool SetClipPlaneEnabled(char nIndex = -1, bool bEnable = false) override;
		virtual bool GetClipPlane(PLCore::uint8 nIndex, float &fA, float &fB, float &fC, float &fD) const override;
		virtual bool SetClipPlane(char nIndex = -1, float fA = 0.0f, float fB = 0.0f, float fC = 0.0f, float fD = 0.0f) override;
		// Vertex buffer states
		virtual PLRenderer::VertexBuffer *GetVertexBuffer(PLCore::uint32 nStreamNumber = 0, PLCore::uint32 *pnOffset = nullptr) const override;
		virtual bool SetVertexBuffer(PLRenderer::VertexBuffer *pVertexBuffer = nullptr, PLCore::uint32 nOffset = 0, PLCore::uint32 nStreamNumber = 0) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull


#endif // __PLRENDERERNULL_FIXEDFUNCTIONS_H__
