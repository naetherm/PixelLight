/*********************************************************\
 *  File: SRPLightEffectsFixedFunctions.h                *
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


#ifndef __PLCOMPOSITING_FIXEDFUNCTIONS_LIGHTEFFECTS_H__
#define __PLCOMPOSITING_FIXEDFUNCTIONS_LIGHTEFFECTS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Pool.h>
#include "PLCompositing/SRPLightEffects.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
	class OcclusionQuery;
}
namespace PLScene {
	class VisNode;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fixed functions based scene renderer pass drawing light effects like lens flares
*/
class SRPLightEffectsFixedFunctions : public SRPLightEffects {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPLightEffectsFixedFunctions, "PLCompositing", PLCompositing::SRPLightEffects, "Fixed functions based scene renderer pass drawing light effects like lens flares")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPLightEffectsFixedFunctions();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SRPLightEffectsFixedFunctions();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Prepares light effects recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*/
		void PrepareRec(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery);

		/**
		*  @brief
		*    Frees all light effects
		*/
		void FreeLightEffects();

		/**
		*  @brief
		*    Draws a light effect flare
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] fP
		*    Position. 1.0f is where the light is, -1.0f is the opposite side
		*  @param[in] fS
		*    Size. 1.0f will fill the screen when looking right at the light
		*  @param[in] fR
		*    Red color component of the flare
		*  @param[in] fG
		*    Green color component of the flare
		*  @param[in] fB
		*    Blue color component of the flare
		*  @param[in] fA
		*    Alpha color component of the flare
		*  @param[in] v2DPos
		*    2D screen position of the light
		*/
		void DrawFlare(PLRenderer::Renderer &cRenderer, float fP, float fS, float fR, float fG,
					   float fB, float fA, const PLMath::Vector3 &v2DPos) const;

		/**
		*  @brief
		*    Draws a simple billboard
		*
		*  param[in] vPos
		*    Billboard position
		*  param[in] fSize
		*    Billboard size
		*
		*  @remarks
		*    A billboard will always 'look into the camera'. The position is normally in world space.
		*    The world matrix of the renderer should be an identity matrix.
		*/
		void DrawBillboard(const PLMath::Vector3 &vPos, float fSize);

		/**
		*  @brief
		*    Draws a simple billboard
		*
		*  param[in] vPos
		*    Billboard position
		*  param[in] fSize
		*    Billboard size
		*  param[in] cColor
		*    Billboard color
		*
		*  @see
		*    - DrawBillboard() above
		*/
		void DrawBillboard(const PLMath::Vector3 &vPos, float fSize, const PLGraphics::Color4 &cColor);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal prepared light effect
		*/
		struct LightEffect {
			const PLScene::VisNode	   *pVisNode;			/**< ALWAYS valid! */
			PLRenderer::OcclusionQuery *pOcclusionQueryAll;	/**< Can be a null pointer */
			PLRenderer::OcclusionQuery *pOcclusionQuery;	/**< Can be a null pointer */
			float						fFactor;			/**< Factor */
			float						fFlareBlendFactor;	/**< Factor for flares & blend */
			PLMath::Vector3				v2DPos;				/**< 2D screen position (-1 to 1) */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::VertexBuffer   *m_pBillboardVB;			/**< Billboard vertex buffer, can be a null pointer */
		PLRenderer::VertexBuffer   *m_pBillboardColorVB;	/**< Billboard vertex buffer with color support, can be a null pointer */
		PLCore::Pool<LightEffect*>  m_lstLightEffects;		/**< List of used light effects */
		PLCore::Pool<LightEffect*>  m_lstFreeLightEffects;	/**< List of free light effects */


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_FIXEDFUNCTIONS_LIGHTEFFECTS_H__
