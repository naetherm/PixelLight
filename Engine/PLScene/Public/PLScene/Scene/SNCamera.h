/*********************************************************\
 *  File: SNCamera.h                                     *
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


#ifndef __PLSCENE_CAMERA_H__
#define __PLSCENE_CAMERA_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Frustum.h>
#include <PLMath/Matrix4x4.h>
#include "PLScene/Scene/SceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SceneRenderer;
class SceneRendererHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight standard camera scene node
*
*  @remarks
*    PixelLight is using a right-handed coordinate system like OpenGL does, therefore the camera usually looks
*    along the negative z-axis. While this fact may be ok in 'low level' functionality, it makes cameras less
*    comfortable within the scene graph because this 'look along negative z' doesn't go along with universal
*    features like 'look at this point'. So, we decided to 'break' the right-handed coordinate system convention
*    within the camera scene node when it's coming to 'look along negative z'. Instead we 'look along positive z'
*    and we profit from an universal behavior and don't need to care about flipping axis - although it's still
*    possible to flip axis.
*
*  @note
*    - Scene node scale is ignored
*/
class SNCamera : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			FlipY        = 1<<10,	/**< Flip y axis */
			InvCullMode  = 1<<11,	/**< Invert cull mode */
			NoZFar       = 1<<12	/**< Do not use the far clipping plane */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(FlipY,		"Flip y axis")
			pl_enum_value(InvCullMode,	"Invert cull mode")
			pl_enum_value(NoZFar,		"Do not use the far clipping plane")
		pl_enum_end

		/**
		*  @brief
		*    Scene node debug flags (SceneNode debug flags extension)
		*/
		enum EDebugFlags {
			DebugNoFrustum       = 1<<8,	/**< Do not draw the (green) frustum */
			DebugFrustumVertices = 1<<9		/**< Draw the (green) frustum vertices */
		};
		pl_enum(EDebugFlags)
			pl_enum_base(SceneNode::EDebugFlags)
			pl_enum_value(DebugNoFrustum,		"Do not draw the (green) frustum")
			pl_enum_value(DebugFrustumVertices,	"Draw the (green) frustum vertices")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNCamera, "PLScene", PLScene::SceneNode, "PixelLight standard camera scene node")
		// Properties
		pl_properties
			pl_property("Icon",	"Data/Textures/IconCamera.dds")
		pl_properties_end
		// Attributes
		pl_attribute(FOV,					float,						45.0f,		ReadWrite,	GetSet,	"Field of view in degree",																									"Min='10.0' Max='170.0'")
		pl_attribute(Aspect,				float,						1.0f,		ReadWrite,	GetSet,	"Aspect factor",																											"")
		pl_attribute(ZNear,					float,						0.1f,		ReadWrite,	GetSet,	"Near clipping plane",																										"")
		pl_attribute(ZFar,					float,						1000.0f,	ReadWrite,	GetSet,	"Far clipping plane",																										"")
		pl_attribute(SceneRendererFilename,	PLCore::String,				"",			ReadWrite,	GetSet,	"Filename of the scene renderer to use, if empty the default scene renderer of the scene graph surface painter is used",	"Type='SceneRenderer'")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,					pl_flag_type(EFlags),		0,			ReadWrite,	GetSet,	"Flags",																													"")
		pl_attribute(DebugFlags,			pl_flag_type(EDebugFlags),	0,			ReadWrite,	GetSet,	"Debug flags",																												"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API float GetFOV() const;
		PLS_API void SetFOV(float fValue);
		PLS_API float GetAspect() const;
		PLS_API void SetAspect(float fValue);
		PLS_API float GetZNear() const;
		PLS_API void SetZNear(float fValue);
		PLS_API float GetZFar() const;
		PLS_API void SetZFar(float fValue);
		PLS_API PLCore::String GetSceneRendererFilename() const;
		PLS_API void SetSceneRendererFilename(const PLCore::String &sValue);
		PLS_API virtual void SetFlags(PLCore::uint32 nValue);


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the current set camera
		*
		*  @return
		*    The current set camera, a null pointer if no camera is currently set
		*
		*  @note
		*    - Depreciated, will be removed in the future
		*/
		static PLS_API SNCamera *GetCamera();

		/**
		*  @brief
		*    Sets the current set camera
		*
		*  @param[in] pCamera
		*    The current set camera, a null pointer if no camera is currently set
		*  @param[in] pRenderer
		*    Renderer to use, if a null pointer nothing happens
		*
		*  @note
		*    - Depreciated, will be removed in the future
		*/
		static PLS_API void SetCamera(SNCamera *pCamera, PLRenderer::Renderer *pRenderer);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNCamera();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNCamera();

		/**
		*  @brief
		*    Returns the used scene renderer
		*
		*  @return
		*    The used scene renderer, can be a null pointer
		*/
		PLS_API SceneRenderer *GetSceneRenderer() const;

		/**
		*  @brief
		*    Sets the camera
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] bSetProjection
		*    Set projection matrix?
		*  @param[in] bSetView
		*    Set view matrix?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - The projection and view matrix will be set using the camera settings
		*/
		PLS_API bool SetCamera(PLRenderer::Renderer &cRenderer, bool bSetProjection = true, bool bSetView = true);

		/**
		*  @brief
		*    Returns whether the camera projection and view matrix and the frustum are updated automatically
		*
		*  @return
		*    'true' if the camera projection and view matrix and the frustum are updated automatically,
		*    else 'false'
		*
		*  @note
		*    - If this is 'true', the mentioned things will be updated within 'SetCamera()'
		*/
		PLS_API bool GetAutoUpdate() const;

		/**
		*  @brief
		*    Sets whether the camera projection and view matrix and the frustum are updated automatically
		*
		*  @param[in] bAutoUpdate
		*    'true' if the camera projection and view matrix and the frustum are updated automatically,
		*    else 'false'
		*
		*  @see
		*    - GetAutoUpdate()
		*/
		PLS_API void SetAutoUpdate(bool bAutoUpdate = true);

		/**
		*  @brief
		*    Returns the projection matrix
		*
		*  @param[in] cViewport
		*    Viewport to use
		*
		*  @return
		*    Projection matrix
		*
		*  @see
		*    - GetAutoUpdate()
		*/
		PLS_API PLMath::Matrix4x4 &GetProjectionMatrix(const PLMath::Rectangle &cViewport);

		/**
		*  @brief
		*    Returns the view rotation offset
		*
		*  @return
		*    View rotation offset
		*
		*  @remarks
		*    By using the view rotation offset, a camera can be rotated towards one direction while viewing towards another one.
		*    This view rotation offset can for example be controlled by a head tracker system.
		*/
		PLS_API const PLMath::Quaternion &GetViewRotationOffset() const;

		/**
		*  @brief
		*    Sets the view rotation offset
		*
		*  @param[in] qViewRotationOffset
		*    New view rotation offset to set
		*
		*  @see
		*    - GetViewRotationOffset()
		*/
		PLS_API void SetViewRotationOffset(const PLMath::Quaternion &qViewRotationOffset);

		/**
		*  @brief
		*    Returns the view matrix
		*
		*  @return
		*    View matrix
		*
		*  @remarks
		*    PixelLight is using a right-handed coordinate system like OpenGL does, therefore the view matrix 'looks'
		*    along the negative z-axis. (unlike the camera scene node itself, have a look into the class documentation)
		*
		*  @see
		*    - GetAutoUpdate()
		*/
		PLS_API PLMath::Matrix3x4 &GetViewMatrix();

		/**
		*  @brief
		*    Returns the camera frustum
		*
		*  @param[in] cViewport
		*    Viewport to use
		*
		*  @return
		*    Camera frustum in 'scene container space'
		*
		*  @see
		*    - GetAutoUpdate()
		*/
		PLS_API PLMath::Frustum &GetFrustum(const PLMath::Rectangle &cViewport);

		/**
		*  @brief
		*    Returns the 8 camera frustum vertices
		*
		*  @param[in] cViewport
		*    Viewport to use
		*
		*  @return
		*    8 camera vertices in 'scene container space'
		*
		*  @note
		*    - This vertices can for instance be used to check the camera frustum visibility
		*/
		PLS_API const PLCore::Array<PLMath::Vector3> &GetFrustumVertices(const PLMath::Rectangle &cViewport);

		/**
		*  @brief
		*    Gets the current camera viewport corners
		*
		*  @param[out] vUpperRight
		*    Will receive the upper/right corner
		*  @param[out] vLowerRight
		*    Will receive the lower/right corner
		*  @param[out] vLowerLeft
		*    Will receive the lower/left corner
		*  @param[out] vUpperLeft
		*    Will receive the upper/left corner
		*  @param[in]  bContainerSpace
		*    Transform corners into container space? (else they are within local scene node space)
		*  @param[in]  fDistance
		*    Distance from 0, use for example GetNearPlane() to get the viewport corners on the near plane
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no camera is set?)
		*/
		PLS_API bool GetViewportCorners(PLMath::Vector3 &vUpperRight, PLMath::Vector3 &vLowerRight,
									   PLMath::Vector3 &vLowerLeft, PLMath::Vector3 &vUpperLeft,
									   bool bContainerSpace, float fDistance);


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Flags which hold internal camera information
		*/
		enum EInternalCameraFlags {
			RecalculateProjectionMatrix	= 1<<0,	/**< Recalculation of projection matrix required */
			RecalculateViewMatrix		= 1<<1,	/**< Recalculation of view matrix required */
			RecalculateFrustum			= 1<<2,	/**< Recalculation of frustum required */
			RecalculateFrustumVertices	= 1<<3	/**< Recalculation of frustum vertices required */
		};


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		// [TODO] Remove this
		static SNCamera *m_pCamera;	/**< Current set camera, can be a null pointer */


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
		float							m_fFOV;						/**< Field of view in degree */
		float							m_fAspect;					/**< Aspect factor */
		float							m_fZNear;					/**< Near clipping plane */
		float							m_fZFar;					/**< Far clipping plane */
		PLCore::String				    m_sSceneRendererFilename;	/**< Filename of the scene renderer to use, if empty the default scene renderer of the scene graph surface painter is used */
		PLCore::uint8				    m_nInternalCameraFlags;		/**< Internal camera flags */
		bool							m_bAutoUpdate;				/**< Update projection & view matrix and frustum automatically? */
		PLMath::Matrix4x4				m_mProj;					/**< Current projection matrix */
		PLMath::Quaternion				m_qViewRotationOffset;		/**< View rotation offset */
		PLMath::Matrix3x4				m_mView;					/**< Current view matrix */
		PLMath::Frustum					m_cFrustum;					/**< Current frustum */
		PLCore::Array<PLMath::Vector3>  m_cFrustumVertices;			/**< Current frustum vertices */
		PLCore::uint32				    m_nViewportWidth;			/**< Current viewport width */
		PLCore::uint32				    m_nViewportHeight;			/**< Current viewport height */
		SceneRendererHandler		   *m_pSceneRendererHandler;	/**< Scene renderer handler, always valid! */


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


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_CAMERA_H__
