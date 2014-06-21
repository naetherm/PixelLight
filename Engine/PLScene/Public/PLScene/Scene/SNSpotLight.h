/*********************************************************\
 *  File: SNSpotLight.h                                  *
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


#ifndef __PLSCENE_SPOTLIGHT_H__
#define __PLSCENE_SPOTLIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Frustum.h>
#include <PLMath/Matrix4x4.h>
#include "PLScene/Scene/SNPointLight.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Spot light scene node
*
*  @remarks
*    PixelLight is using a right-handed coordinate system like OpenGL does, therefore the spot light usually looks
*    along the negative z-axis. While this fact may be ok in 'low level' functionality, it makes spot lights less
*    comfortable within the scene graph because this 'look along negative z' doesn't go along with universal
*    features like 'look at this point'. So, we decided to 'break' the right-handed coordinate system convention
*    within the spot light scene node when it's coming to 'look along negative z'. Instead we 'look along positive z'
*    and we profit from an universal behavior and don't need to care about flipping axis.
*
*  @note
*    - By default, the 'NoCone' flag is not set meaning that the given outer and inner angles are
*      used for lighting
*/
class SNSpotLight : public SNPointLight {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoCone = 1<<13	/**< Do not use the given outer and inner angles for lighting */
		};
		pl_enum(EFlags)
			pl_enum_base(SNPointLight::EFlags)
			pl_enum_value(NoCone, "Do not use the given outer and inner angles for lighting")
		pl_enum_end

		/**
		*  @brief
		*    Scene node debug flags (SceneNode debug flags extension)
		*/
		enum EDebugFlags {
			DebugDepthTest       = 1<<9,	/**< Perform a depth test when render the debug stuff like lines */
			DebugNoFrustum       = 1<<10,	/**< Do not draw the (green) frustum */
			DebugFrustumVertices = 1<<11	/**< Draw the (green) frustum vertices */
		};
		pl_enum(EDebugFlags)
			pl_enum_base(SNPointLight::EDebugFlags)
			pl_enum_value(DebugDepthTest,		"Perform a depth test when render the debug stuff like lines")
			pl_enum_value(DebugNoFrustum,		"Do not draw the (green) frustum")
			pl_enum_value(DebugFrustumVertices,	"Draw the (green) frustum vertices")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNSpotLight, "PLScene", PLScene::SNPointLight, "Spot light scene node")
		// Attributes
		pl_attribute(OuterAngle,	float,						45.0f,	ReadWrite,	GetSet,	"Outer cone angle in degree",									"")
		pl_attribute(InnerAngle,	float,						35.0f,	ReadWrite,	GetSet,	"Inner cone angle in degree (smaller than the outer angle)",	"")
		pl_attribute(ZNear,			float,						0.1f,	ReadWrite,	GetSet,	"Near clipping plane",											"")
		pl_attribute(Aspect,		float,						1.0f,	ReadWrite,	GetSet,	"Aspect factor (only used if 'NoCone'-flag is set!)",			"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),		0,		ReadWrite,	GetSet,	"Flags",														"")
		pl_attribute(DebugFlags,	pl_flag_type(EDebugFlags),	0,		ReadWrite,	GetSet,	"Debug flags",													"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void SetRange(float fValue) override;	// From SNPointLight
		PLS_API float GetOuterAngle() const;
		PLS_API void SetOuterAngle(float fValue);
		PLS_API float GetInnerAngle() const;
		PLS_API void SetInnerAngle(float fValue);
		PLS_API float GetZNear() const;
		PLS_API void SetZNear(float fValue);
		PLS_API float GetAspect() const;
		PLS_API void SetAspect(float fValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNSpotLight();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNSpotLight();

		/**
		*  @brief
		*    Returns the projection matrix
		*
		*  @return
		*    Projection matrix
		*/
		PLS_API const PLMath::Matrix4x4 &GetProjectionMatrix();

		/**
		*  @brief
		*    Returns the view matrix
		*
		*  @remarks
		*    PixelLight is using a right-handed coordinate system like OpenGL does, therefore the view matrix 'looks'
		*    along the negative z-axis. (unlike the spot light scene node itself, have a look into the class documentation)
		*
		*  @return
		*    View matrix
		*/
		PLS_API const PLMath::Matrix3x4 &GetViewMatrix();

		/**
		*  @brief
		*    Returns the spot light frustum
		*
		*  @return
		*    Spot light frustum in 'scene container space'
		*/
		PLS_API const PLMath::Frustum &GetFrustum();

		/**
		*  @brief
		*    Returns the 8 spot light frustum vertices
		*
		*  @return
		*    8 spot light frustum vertices in 'scene container space'
		*
		*  @note
		*    - This vertices can for instance be used to check the spot frustum visibility
		*/
		PLS_API const PLCore::Array<PLMath::Vector3> &GetFrustumVertices();


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Flags which hold ínternal light information ('SNPointLight::EInternalLightFlags' extension)
		*/
		enum EInternalLightFlags {
			// Recalculate
			RecalculateProjectionMatrix	= 1<<1,	/**< Recalculation of projection matrix required */
			RecalculateViewMatrix		= 1<<2,	/**< Recalculation of view matrix required */
			RecalculateFrustum			= 1<<3,	/**< Recalculation of frustum required */
			RecalculateFrustumVertices	= 1<<4	/**< Recalculation of frustum vertices required */
		};


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node position or rotation changed
		*/
		void OnPositionRotation();

		/**
		*  @brief
		*    Calculates and returns the current view rotation
		*
		*  @return
		*    The current view rotation
		*/
		PLMath::Quaternion CalculateViewRotation() const;


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerPositionRotation;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float						   m_fOuterAngle;		/**< Outer cone angle in degree */
		float						   m_fInnerAngle;		/**< Inner cone angle in degree (smaller than the outer angle) */
		float						   m_fZNear;			/**< Near clipping plane */
		float						   m_fAspect;			/**< Aspect factor (only used if 'NoCone'-flag is set!) */
		PLMath::Matrix4x4			   m_mProj;				/**< Current projection matrix */
		PLMath::Matrix3x4			   m_mView;				/**< Current view matrix */
		PLMath::Frustum				   m_cFrustum;			/**< Current frustum */
		PLCore::Array<PLMath::Vector3> m_cFrustumVertices;	/**< Current frustum vertices */


	//[-------------------------------------------------------]
	//[ Public virtual SNLight functions                      ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool IsSpotLight() const override;


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void UpdateAABoundingBox() override;
		PLS_API virtual void GetBoundingSphere(PLMath::Sphere &cSphere) override;
		PLS_API virtual void GetContainerBoundingSphere(PLMath::Sphere &cSphere) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SPOTLIGHT_H__
