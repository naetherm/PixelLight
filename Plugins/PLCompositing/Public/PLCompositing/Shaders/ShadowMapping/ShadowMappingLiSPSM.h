/*********************************************************\
 *  File: ShadowMappingLiSPSM.h                          *
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


#ifndef __PLCOMPOSITING_SHADOWMAPPING_LISPSM_H__
#define __PLCOMPOSITING_SHADOWMAPPING_LISPSM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCompositing/Shaders/ShadowMapping/ShadowMapping.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Light Space Perspective Shadow Mapping (LiSPSM) class
*
*  @remarks
*    Light Space Perspective Shadow Mapping (LiSPSM) implementation as described within "Light Space Perspective Shadow Maps"
*    by Michael Wimmer, Daniel Scherzer and Werner Purgathofer (http://www.cg.tuwien.ac.at/research/vr/lispsm/shadows_egsr2004_revised.pdf).
*/
class ShadowMappingLiSPSM : public ShadowMapping {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, ShadowMappingLiSPSM, "PLCompositing", PLCompositing::ShadowMapping, "Light Space Perspective Shadow Mapping (LiSPSM) class")
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
		PLCOM_API ShadowMappingLiSPSM();

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~ShadowMappingLiSPSM();


	//[-------------------------------------------------------]
	//[ Public virtual ShadowMapping functions                ]
	//[-------------------------------------------------------]
	public:
		virtual void CalculateLightMatrices(PLScene::SNLight &cLight, PLMath::Matrix4x4 &mLightView, PLMath::Matrix4x4 &mLightProjection) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_SHADOWMAPPING_LISPSM_H__
