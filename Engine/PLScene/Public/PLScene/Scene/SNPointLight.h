/*********************************************************\
 *  File: SNPointLight.h                                 *
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


#ifndef __PLSCENE_POINTLIGHT_H__
#define __PLSCENE_POINTLIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/PlaneSet.h>
#include "PLScene/Scene/SNLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Omni directional point light scene node were light is emitted from a single point in space
*/
class SNPointLight : public SNLight {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static PLS_API const float MinRange;	/**< Minimum point light range (0.0001) */

		/**
		*  @brief
		*    Scene node debug flags (SceneNode debug flags extension)
		*/
		enum EDebugFlags {
			DebugNoScissorRectangle = 1<<8	/**< Do not draw the (pink) scissor rectangle */
		};
		pl_enum(EDebugFlags)
			pl_enum_base(SNLight::EDebugFlags)
			pl_enum_value(DebugNoScissorRectangle, "Do not draw the (pink) scissor rectangle")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNPointLight, "PLScene", PLScene::SNLight, "Omni directional point light scene node were light is emitted from a single point in space")
		// Attributes
		pl_attribute(Range,			float,						1.0f,	ReadWrite,	GetSet,	"Light range",	"Min='0.0001'")
			// Overwritten SceneNode attributes
		pl_attribute(DebugFlags,	pl_flag_type(EDebugFlags),	0,		ReadWrite,	GetSet,	"Flags",					"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual float GetRange() const;
		PLS_API virtual void SetRange(float fValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNPointLight();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNPointLight();

		/**
		*  @brief
		*    Returns the box plane set
		*
		*  @return
		*    Box plane set
		*/
		PLS_API const PLMath::PlaneSet &GetBoxPlaneSet();


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Flags which hold ínternal light information
		*/
		enum EInternalLightFlags {
			// Recalculate
			RecalculateBoxPlaneSet	= 1<<0	/**< Recalculation of box plane set required */
		};


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::uint8 m_nInternalLightFlags;	/**< Internal light flags */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Makes the box plane set dirty
		*/
		void DirtyBoxPlaneSet();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerPosition;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float			 m_fRange;			/**< Light range */
		PLMath::PlaneSet m_cBoxPlaneSet;	/**< Box plane set */


	//[-------------------------------------------------------]
	//[ Public virtual SNLight functions                      ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool IsRenderLight() const override;
		PLS_API virtual bool IsPointLight() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void UpdateAABoundingBox() override;
		PLS_API virtual void GetBoundingSphere(PLMath::Sphere &cSphere) override;
		PLS_API virtual void GetContainerBoundingSphere(PLMath::Sphere &cSphere) override;


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_POINTLIGHT_H__
