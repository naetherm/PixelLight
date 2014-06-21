/*********************************************************\
 *  File: SRPVolume.h                                    *
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


#ifndef __PLVOLUMERENDERER_SRPVOLUME_H__
#define __PLVOLUMERENDERER_SRPVOLUME_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color3.h>
#include <PLScene/Scene/SceneNodeHandler.h>
#include <PLVolume/Scene/SRPVolume.h>
#include "PLVolumeRenderer/PLVolumeRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMesh {
	class MeshHandler;
}
namespace PLVolumeRenderer {
	class ShaderCompositor;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene renderer pass for volume scene nodes targeting PC
*/
class SRPVolume : public PLVolume::SRPVolume {


	// [TODO] Remove those
	friend class ShaderComposition;
	friend class ShaderFunctionRaySetupColorCube;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    1.0 - Ray Setup
		*/
		enum ERaySetup {
			RaySetupNull                    = 0,	/**< No ray setup */
			RaySetupColorCube               = 1,	/**< Ray setup using a color cube rasterization */
			RaySetupBoundingBoxIntersection = 2,	/**< Ray setup using bounding box intersection */
			RaySetupHybrid                  = 3		/**< Ray setup using a combination of color cube and bounding box intersection */
		};
		pl_enum(ERaySetup)
			pl_enum_value(RaySetupNull,						"No ray setup")
			pl_enum_value(RaySetupColorCube,				"Ray setup using a color cube rasterization")
			pl_enum_value(RaySetupBoundingBoxIntersection,	"Ray setup using bounding box intersection")
			pl_enum_value(RaySetupHybrid,					"Ray setup using a combination of color cube and bounding box intersection")
		pl_enum_end

		/**
		*  @brief
		*    1.2 - Jitter Position
		*
		*  @note
		*    - Jitter the start position of the ray in order to reduce wooden grain effect
		*/
		enum EJitterPosition {
			JitterPositionNull          = 0,	/**< No jitter position */
			JitterPositionStochastic    = 1,	/**< Jitter position by using a random texture */
			JitterPositionTrigonometric = 2,	/**< Jitter position by using trigonometric functions */
		};
		pl_enum(EJitterPosition)
			pl_enum_value(JitterPositionNull,			"No jitter position")
			pl_enum_value(JitterPositionStochastic,		"Jitter position by using a random texture")
			pl_enum_value(JitterPositionTrigonometric,	"Jitter position by using trigonometric functions")
		pl_enum_end

		/**
		*  @brief
		*    Scene renderer pass flags (PLScene::SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoBlend                       = 1<<2,	/**< Do not perform blend */
			NoNearCap                     = 1<<3,	/**< Do not use near-cap (required for camera within volume) */
			NoDepthClamp                  = 1<<4,	/**< Do not use depth clamp (required for proper camera within volume) */
			NoDepthTexture                = 1<<5,	/**< Do not use depth buffer information */
			NoIlluminationThreshold       = 1<<6,	/**< Do not use illumination threshold */
			// Suppress shader function
			NoRaySetup                    = 1<<7,	/**< Suppress shader function: 1.0 - Ray Setup */
			NoGlobalSampleRateFactor      = 1<<8,	/**< Suppress global sampling rate factor within shader function: 1.0 - Ray Setup */
			NoClipRay                     = 1<<9,	/**< Suppress shader function: 1.1 - Clip Ray */
			NoJitterPosition              = 1<<10,	/**< Suppress shader function: 1.2 - Jitter Position */
			NoRayTraversal                = 1<<11,	/**< Suppress shader function: 2.0 - Ray Traversal */
			NoClipPosition                = 1<<12,	/**< Suppress shader function: 2.1 - Clip Position */
			NoReconstruction              = 1<<13,	/**< Suppress shader function: 2.2 - Reconstruction (higher-order texture filtering) */
			NoFetchScalar                 = 1<<14,	/**< Suppress shader function: 2.2 - Fetch Scalar */
			NoGlobalVolumeTextureLOD      = 1<<15,	/**< Suppress global volume texture LOD within shader function: 2.2 - Fetch Scalar */
			NoShading                     = 1<<16,	/**< Suppress shader function: 2.3 - Shading */
			NoClassification              = 1<<17,	/**< Suppress shader function: 2.4 - Classification */
			NoGradient                    = 1<<18,	/**< Suppress shader function: 2.5 - Gradient */
			NoGradientInput               = 1<<19,	/**< Suppress shader function: 2.5 - Gradient Input */
			NoIllumination                = 1<<20	/**< Suppress shader function: 2.6 - Illumination */
		};
		pl_enum(EFlags)
			pl_enum_base(PLVolume::SRPVolume::EFlags)
			pl_enum_value(NoBlend,							"Do not perform blend")
			pl_enum_value(NoNearCap,						"Do not use near-cap (required for camera within volume)")
			pl_enum_value(NoDepthClamp,						"Do not use depth clamp (required for proper camera within volume)")
			pl_enum_value(NoDepthTexture,					"Do not use depth buffer information")
			pl_enum_value(NoIlluminationThreshold,			"Do not use illumination threshold")
			// Suppress shader function
			pl_enum_value(NoRaySetup,						"Suppress shader function: 1.0 - Ray Setup")
			pl_enum_value(NoGlobalSampleRateFactor,			"Suppress global sampling rate factor within shader function: 1.0 - Ray Setup")
			pl_enum_value(NoClipRay,						"Suppress shader function: 1.1 - Clip Ray")
			pl_enum_value(NoJitterPosition,					"Suppress shader function: 1.2 - Jitter Position")
			pl_enum_value(NoRayTraversal,					"Suppress shader function: 2.0 - Ray Traversal")
			pl_enum_value(NoClipPosition,					"Suppress shader function: 2.1 - Clip Position")
			pl_enum_value(NoReconstruction,					"Suppress shader function: 2.2 - Reconstruction (higher-order texture filtering)")
			pl_enum_value(NoFetchScalar,					"Suppress shader function: 2.2 - Fetch Scalar")
			pl_enum_value(NoGlobalVolumeTextureLOD,			"Suppress global volume texture LOD within shader function: 2.2 - Fetch Scalar")
			pl_enum_value(NoShading,						"Suppress shader function: 2.3 - Shading")
			pl_enum_value(NoClassification,					"Suppress shader function: 2.4 - Classification")
			pl_enum_value(NoGradient,						"Suppress shader function: 2.5 - Gradient")
			pl_enum_value(NoGradientInput,					"Suppress shader function: 2.5 - Gradient Input")
			pl_enum_value(NoIllumination,					"Suppress shader function: 2.6 - Illumination")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, SRPVolume, "PLVolumeRenderer", PLVolume::SRPVolume, "Scene renderer pass for volume scene nodes targeting PC")
		// Attributes
		pl_attribute(ShaderLanguage,				PLCore::String,						"",										ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",																"")
		// 1.0 - Ray Setup
		pl_attribute(RaySetup,						pl_enum_type(ERaySetup),			RaySetupHybrid,							ReadWrite,	DirectValue,	"Technique to use for the ray setup (1.0 - Ray Setup)",																																				"")
		pl_attribute(GlobalSampleRateFactor,		float,								1.0f,									ReadWrite,	DirectValue,	"Global sample rate factor to use (1.0 = 100% for correct result, 0.0 = 50% = take only half of the samples) (1.0 - Ray Setup), simply multiplied with the per scene node sampling rate factor",	"Max='10'")
		// 1.2 - Jitter Position
		pl_attribute(JitterPosition,				pl_enum_type(EJitterPosition),		JitterPositionTrigonometric,			ReadWrite,	DirectValue,	"Technique to use for jitter ray start position (1.2 - Jitter Position, jitter the start position of the ray in order to reduce wooden grain effect)",												"")
		pl_attribute(DitherRay,						float,								1.0f,									ReadWrite,	DirectValue,	"Scale factor for dithering the ray's start position in order to avoid wooden grain effects (usually value between 0...1) (1.2 - Jitter Position)",													"")
		// 2.2 - Fetch Scalar
		pl_attribute(GlobalVolumeTextureLOD,		float,								0.0,									ReadWrite,	DirectValue,	"Global volume texture level of detail (0...<number of mipmaps>-1), usually the value 3 shows a well notable effect (2.2 - Fetch Scalar), simply added to the per scene node volume texture LOD",	"Min='0'")
		// 2.3 - Shading
		pl_attribute(AmbientColor,					PLGraphics::Color3,					PLGraphics::Color3(0.2f, 0.2f, 0.2f),	ReadWrite,	DirectValue,	"Ambient color (2.3 - Shading)",																																									"")
		pl_attribute(IlluminationThreshold,			float,								0.01f,									ReadWrite,	DirectValue,	"Performance: Illumination opacity threshold, usually within the interval [0 .. 1] (2.3 - Shading), illumination only if opacity >= this value",													"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,							pl_flag_type(EFlags),				0,										ReadWrite,	GetSet,			"Flags",																																															"")
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
		PLVOLUMERENDERER_API SRPVolume();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~SRPVolume();

		/**
		*  @brief
		*    Returns the global sampling rate factor
		*
		*  @return
		*    Global sample rate factor to use (1.0 = 100% for correct result, 0.0 = 50% = take only half of the samples) (1.0 - Ray Setup)
		*
		*  @note
		*    - Respects the "NoGlobalSampleRateFactor"-flag
		*/
		PLVOLUMERENDERER_API float GetGlobalSampleRateFactor() const;

		/**
		*  @brief
		*    Returns the global volume texture level of detail
		*
		*  @return
		*    Global volume texture level of detail (0...<number of mipmaps>-1), usually the value 3 shows a well notable effect (2.2 - Fetch Scalar)
		*
		*  @note
		*    - Respects the "NoGlobalVolumeTextureLOD"-flag
		*/
		PLVOLUMERENDERER_API float GetGlobalVolumeTextureLOD() const;


	//[-------------------------------------------------------]
	//[ Protected virtual PLVolume::SRPVolume functions       ]
	//[-------------------------------------------------------]
	protected:
		virtual void DrawVolumeSceneNode(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShaderCompositor	*m_pShaderCompositor;	/**< Used shader compositor instance, can be a null pointer */
		PLMesh::MeshHandler	*m_pMeshHandler;		/**< Mesh handler holding your proxy cube mesh (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SRPVOLUME_H__
