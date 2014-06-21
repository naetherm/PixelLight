/*********************************************************\
 *  File: DrawHelpersBackendFixedFunctions.h             *
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


#ifndef __PLRENDERERBACKEND_DRAWHELPERSBACKENDFIXEDFUNCTIONS_H__
#define __PLRENDERERBACKEND_DRAWHELPERSBACKENDFIXEDFUNCTIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Matrix4x4.h>
#include "PLRenderer/Renderer/Backend/DrawHelpersBackend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fixed functions based draw helpers interface implementation
*/
class DrawHelpersBackendFixedFunctions : public DrawHelpersBackend {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer instance to use
		*/
		PLRENDERER_API DrawHelpersBackendFixedFunctions(Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~DrawHelpersBackendFixedFunctions();


	//[-------------------------------------------------------]
	//[ Public virtual DrawHelpers functions                  ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual void Begin2DMode(float fX1 = 0.0f, float fY1 = 0.0f, float fX2 = 1.0f, float fY2 = 1.0f) override;
		PLRENDERER_API virtual void End2DMode() override;
		PLRENDERER_API virtual void DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize = PLMath::Vector2::Zero, const PLGraphics::Color4 &cColor = PLGraphics::Color4::White,
											  float fAlphaReference = 1.0f, const PLMath::Vector2 &vTextureCoordinate = PLMath::Vector2::Zero, const PLMath::Vector2 &vTextureCoordinateSize = PLMath::Vector2::One, const PLMath::Matrix4x4 &mTexture = PLMath::Matrix4x4::Identity) override;
		PLRENDERER_API virtual void DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const PLMath::Vector3 &vPos, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, const PLMath::Vector2 &vSize = PLMath::Vector2::Zero, const PLGraphics::Color4 &cColor = PLGraphics::Color4::White,
											  float fAlphaReference = 1.0f, const PLMath::Vector2 &vTextureCoordinate = PLMath::Vector2::Zero, const PLMath::Vector2 &vTextureCoordinateSize = PLMath::Vector2::One, const PLMath::Matrix4x4 &mTexture = PLMath::Matrix4x4::Identity) override;
		PLRENDERER_API virtual void DrawPoint(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vPosition, float fSize = 1.0f) override;
		PLRENDERER_API virtual void DrawPoint(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vPosition, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fSize = 1.0f) override;
		PLRENDERER_API virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vStartPosition, const PLMath::Vector2 &vEndPosition, float fWidth = 1.0f) override;
		PLRENDERER_API virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vStartPosition, const PLMath::Vector3 &vEndPosition, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 1.0f) override;
		PLRENDERER_API virtual void DrawTriangle(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 0.0f) override;
		PLRENDERER_API virtual void DrawQuad(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize, float fWidth = 0.0f) override;
		PLRENDERER_API virtual void DrawQuad(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Vector3 &vV4, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 0.0f) override;
		PLRENDERER_API virtual void DrawGradientQuad(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize) override;
		PLRENDERER_API virtual void DrawGradientQuad(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Vector3 &vV4, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Matrix4x4 m_m2DModeProjBackup;	/**< Projection matrix backup */
		PLMath::Matrix4x4 m_m2DModeViewBackup;	/**< View matrix backup */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERERBACKEND_DRAWHELPERSBACKENDFIXEDFUNCTIONS_H__
