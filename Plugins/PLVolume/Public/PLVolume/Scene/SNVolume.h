/*********************************************************\
 *  File: SNVolume.h                                     *
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


#ifndef __PLVOLUME_SNVOLUME_H__
#define __PLVOLUME_SNVOLUME_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNode.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Volume;
class VolumeHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node representing a volume
*
*  @note
*    - Connects a volume with the scene graph
*/
class SNVolume : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    2.0 - Ray Traversal
		*/
		enum ERayTraversal {
			RayTraversalNull              = 0,	/**< No ray traversal */
			RayTraversalDebugRayStart     = 1,	/**< Debug ray traversal: Show start position inside the volume */
			RayTraversalDebugRayEnd       = 2,	/**< Debug ray traversal: Show end position inside the volume */
			RayTraversalDebugRayDirection = 3,	/**< Debug ray traversal: Show ray direction inside the volume (the sign of negative values is flipped) */
			RayTraversalDebugRayLength    = 4,	/**< Debug ray traversal: Show maximum travel length inside the volume */
			RayTraversalIsosurface        = 5,	/**< Isosurface (also known as "first-hit ray casting") ray traversal */
			RayTraversalMIP               = 6,	/**< "Maximum Intensity Projection" (MIP) ray traversal */
			RayTraversalGMIP              = 7,	/**< "Gradient Maximum Intensity Projection" (GMIP) ray traversal as described within the paper "Instant Volumetric Understanding with Order-Independent Volume Rendering" from B. Mora & D. S. Ebert */
			RayTraversalDVRBackToFront    = 8,	/**< Back-to-front compositing (DVR) ray traversal (no early ray termination) */
			RayTraversalDVRFrontToBack    = 9,	/**< Front-to-back compositing (DVR) ray traversal (early ray termination) */
			RayTraversalMIDA              = 10	/**< "Maximum intensity differences accumulation" (MIDA) ray traversal (hybrid combining the advantages of MIP and DVR) */
		};
		pl_enum(ERayTraversal)
			pl_enum_value(RayTraversalNull,					"No ray traversal")
			pl_enum_value(RayTraversalDebugRayStart,		"Debug ray traversal: Show start position inside the volume")
			pl_enum_value(RayTraversalDebugRayEnd,			"Debug ray traversal: Show end position inside the volume")
			pl_enum_value(RayTraversalDebugRayDirection,	"Debug ray traversal: Show ray direction inside the volume (the sign of negative values is flipped)")
			pl_enum_value(RayTraversalDebugRayLength,		"Debug ray traversal: Show maximum travel length inside the volume")
			pl_enum_value(RayTraversalIsosurface,			"Isosurface (also known as \"first-hit ray casting\") ray traversal")
			pl_enum_value(RayTraversalMIP,					"\"Maximum Intensity Projection\" (MIP) ray traversal")
			pl_enum_value(RayTraversalGMIP,					"\"Gradient Maximum Intensity Projection\" (GMIP) ray traversal as described within the paper \"Instant Volumetric Understanding with Order-Independent Volume Rendering\" from B. Mora & D. S. Ebert")
			pl_enum_value(RayTraversalDVRBackToFront,		"Back-to-front compositing (DVR) ray traversal (no early ray termination)")
			pl_enum_value(RayTraversalDVRFrontToBack,		"Front-to-back compositing (DVR) ray traversal (early ray termination)")
			pl_enum_value(RayTraversalMIDA,					"\"Maximum intensity differences accumulation\" (MIDA) ray traversal (hybrid combining the advantages of MIP and DVR)")
		pl_enum_end

		/**
		*  @brief
		*    Reconstruction (2.2 - Reconstruction)(2.2 - Fetch Scalar)
		*/
		enum EReconstruction {
			NearestNeighbourFiltering = 0,	/**< Nearest neighbour filtering (1 volume texture fetch for the reconstruction) */
			TrilinearFiltering        = 1,	/**< Trilinear filtering (1 volume texture fetch for the reconstruction) */
			TriCubicFiltering         = 2,	/**< Tri-cubic B-spline filtering (8 volume texture fetches for the reconstruction, higher-order texture filtering) */
			TriCubicKernelFiltering   = 3	/**< Tri-cubic kernel filtering (8 volume texture fetches + 3 kernel texture fetches for the reconstruction, higher-order texture filtering) */
		};
		pl_enum(EReconstruction)
			pl_enum_value(NearestNeighbourFiltering,	"Nearest neighbour filtering (1 volume texture fetch for the reconstruction)")
			pl_enum_value(TrilinearFiltering,			"Trilinear filtering (1 volume texture fetch for the reconstruction)")
			pl_enum_value(TriCubicFiltering,			"Tri-cubic B-spline filtering (8 volume texture fetches for the reconstruction, higher-order texture filtering)")
			pl_enum_value(TriCubicKernelFiltering,		"Tri-cubic kernel filtering (8 volume texture fetches + 3 kernel texture fetches for the reconstruction, higher-order texture filtering)")
		pl_enum_end

		/**
		*  @brief
		*    2.3 - Shading
		*/
		enum EShading {
			ShadingNull                  = 0,	/**< No shading is performed, the classification result of given scalar will be returned instead */
			ShadingDebugScalar           = 1,	/**< Debug shading: Show scalar */
			ShadingDebugPosition         = 2,	/**< Debug shading: Show current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)] */
			ShadingDebugClassification   = 3,	/**< Debug shading: Show classification result */
			ShadingDebugGradient         = 4,	/**< Debug shading: Show gradient, only normalized if length is greater than one */
			ShadingDebugNormal           = 5,	/**< Debug shading: Show normalized normal as used for the illumination, similar to gradient, but in case of an invalid normal the previous valid normal will be reused */
			ShadingDebugViewingDirection = 6,	/**< Debug shading: Show normalized viewing direction */
			ShadingDebugLightDirection   = 7,	/**< Debug shading: Show normalized light direction */
			ShadingLighting              = 8	/**< Shading with lighting (gradient-based shading) */
		};
		pl_enum(EShading)
			pl_enum_value(ShadingNull,					"No shading is performed, the classification result of given scalar will be returned instead")
			pl_enum_value(ShadingDebugScalar,			"Debug shading: Show scalar")
			pl_enum_value(ShadingDebugPosition,			"Debug shading: Show current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]")
			pl_enum_value(ShadingDebugClassification,	"Debug shading: Show classification result")
			pl_enum_value(ShadingDebugGradient,			"Debug shading: Show gradient, only normalized if length is greater than one")
			pl_enum_value(ShadingDebugNormal,			"Debug shading: Show normalized normal as used for the illumination, similar to gradient, but in case of an invalid normal the previous valid normal will be reused")
			pl_enum_value(ShadingDebugViewingDirection,	"Debug shading: Show normalized viewing direction")
			pl_enum_value(ShadingDebugLightDirection,	"Debug shading: Show normalized light direction")
			pl_enum_value(ShadingLighting,				"Shading with lighting (gradient-based shading)")
		pl_enum_end

		/**
		*  @brief
		*    2.4 - Classification
		*/
		enum EClassification {
			ClassificationNull                          = 0,	/**< No classification is performed, the given scalar will be returned instead */
			ClassificationThreshold                     = 1,	/**< For given scalar values below the threshold zero will be returned, else the given scalar will be returned instead */
			ClassificationTransferFunction              = 2,	/**< The given scalar value is used as index for a standard post-interpolative transfer function */
			ClassificationTransferFunctionPreMultiplied = 3,	/**< The given scalar value is used as index for a pre-multiplied (no color bleeding) post-interpolative transfer function */
			ClassificationPreIntegration                = 4		/**< The given scalar value is used as index for a pre-integration transfer function */
		};
		pl_enum(EClassification)
			pl_enum_value(ClassificationNull,							"No classification is performed, the given scalar will be returned instead")
			pl_enum_value(ClassificationThreshold,						"For given scalar values below the threshold zero will be returned, else the given scalar will be returned instead")
			pl_enum_value(ClassificationTransferFunction,				"The given scalar value is used as index for a standard post-interpolative transfer function")
			pl_enum_value(ClassificationTransferFunctionPreMultiplied,	"The given scalar value is used as index for a pre-multiplied (no color bleeding) post-interpolative transfer function")
			pl_enum_value(ClassificationPreIntegration,					"The given scalar value is used as index for a pre-integration transfer function")
		pl_enum_end

		/**
		*  @brief
		*    2.5 - Gradient
		*/
		enum EGradient {
			GradientNull                = 0,	/**< No gradient computation, returns always (0, 1, 0) instead (... at least it's normalized...) */
			GradientForwardDifferences  = 1,	/**< On-the-fly gradient computation approximating gradient vectors using the forward differences (finite differences) scheme (4 volume texture fetches for the gradient) */
			GradientBackwardDifferences = 2,	/**< On-the-fly gradient computation approximating gradient vectors using the backward differences (finite differences) scheme (4 volume texture fetches for the gradient) */
			GradientCentralDifferences  = 3,	/**< On-the-fly gradient computation approximating gradient vectors using the central differences (finite differences) scheme (6 volume texture fetches for the gradient) */
			GradientCentralDifferences9 = 4		/**< On-the-fly gradient computation approximating gradient vectors using the central differences (finite differences) scheme (gradients of neighbour voxels weighted, 6*9=54 volume texture fetches for the gradient) */
		};
		pl_enum(EGradient)
			pl_enum_value(GradientNull,					"No gradient computation, returns always (0, 1, 0) instead (... at least it's normalized...)")
			pl_enum_value(GradientForwardDifferences,	"On-the-fly gradient computation approximating gradient vectors using the forward differences (finite differences) scheme (4 volume texture fetches for the gradient)")
			pl_enum_value(GradientBackwardDifferences,	"On-the-fly gradient computation approximating gradient vectors using the backward differences (finite differences) scheme (4 volume texture fetches for the gradient)")
			pl_enum_value(GradientCentralDifferences,	"On-the-fly gradient computation approximating gradient vectors using the central differences (finite differences) scheme (6 volume texture fetches for the gradient)")
			pl_enum_value(GradientCentralDifferences9,	"On-the-fly gradient computation approximating gradient vectors using the central differences (finite differences) scheme (gradients of neighbour voxels weighted, 6*9=54 volume texture fetches for the gradient)")
		pl_enum_end

		/**
		*  @brief
		*    2.6 - Illumination
		*/
		enum EIllumination {
			IlluminationNull                 = 0,	/**< No illumination, returns always (0, 0, 0) instead */
			IlluminationLambertianReflection = 1,	/**< Illumination Lambertian reflection */
			IlluminationBlinnPhong           = 2,	/**< Blinn-Phong illumination */
			IlluminationCookTorrance         = 3	/**< Cook-Torrance (aka Torrance-Sparrow) illumination (expensive) */
		};
		pl_enum(EIllumination)
			pl_enum_value(IlluminationNull,					"No illumination, returns always (0, 0, 0) instead")
			pl_enum_value(IlluminationLambertianReflection,	"Illumination Lambertian reflection")
			pl_enum_value(IlluminationBlinnPhong,			"Blinn-Phong illumination")
			pl_enum_value(IlluminationCookTorrance,			"Cook-Torrance (aka Torrance-Sparrow) illumination (expensive)")
		pl_enum_end

		/**
		*  @brief
		*    Scene node flags (PLScene::SceneNode flags extension)
		*/
		enum EFlags {
			NoTextureCompression = 1<<10,	/**< Disable the usage of texture compression? Has only an effect when loading new volume data. Lookout! When using compression, it may take a moment to compress if no compressed data is provided. */
			NoTextureMipmapping  = 1<<11,	/**< Disable the usage of mipmaps? Has only an effect when loading new volume data. Lookout! When using mipmaps, it may take a moment to generate mipmaps if no mipmap data is provided. */
			NoVolumeScale        = 1<<12	/**< When loading, do not use volume scale information to modify the scale of this scene node */
		};
		pl_enum(EFlags)
			pl_enum_base(PLScene::SceneNode::EFlags)
			pl_enum_value(NoTextureCompression,	"Disable the usage of texture compression? Has only an effect when loading new volume data. Lookout! When using compression, it may take a moment to compress if no compressed data is provided.")
			pl_enum_value(NoTextureMipmapping,	"Disable the usage of mipmaps? Has only an effect when loading new volume data. Lookout! When using mipmaps, it may take a moment to generate mipmaps if no mipmap data is provided.")
			pl_enum_value(NoVolumeScale,		"When loading, do not use volume scale information to modify the scale of this scene node")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, SNVolume, "PLVolume", PLScene::SceneNode, "Scene node representing a volume")
		// Attributes
		pl_attribute(VolumeFilenameSave,			PLCore::String,						"",									ReadWrite,	DirectValue,	"In case this filename is not empty, the loaded volume will be saved directly after loading by using this filename (can e.g. be used for data conversion)",				"Type='Volume'")
		pl_attribute(LoaderParameters,				PLCore::String,						"",									ReadWrite,	DirectValue,	"Optional parameters for the used file loader",																															"")
		pl_attribute(VolumeFilename,				PLCore::String,						"",									ReadWrite,	GetSet,			"Filename of the volume data to use",																																	"Type='Volume'")
		// 1.0 - Ray Setup
		pl_attribute(SampleRateFactor,				float,								1.0f,								ReadWrite,	DirectValue,	"Sample rate factor to use (1.0 = 100% for correct result, 0.0 = 50% = take only half of the samples) (1.0 - Ray Setup)",												"Max='10'")
		// 2.0 - Ray Traversal
		pl_attribute(RayTraversal,					pl_enum_type(ERayTraversal),		RayTraversalDVRFrontToBack,			ReadWrite,	DirectValue,	"Technique to use for the ray traversal (2.0 - Ray Traversal)",																											"")
		pl_attribute(Opacity,						float,								1.0f,								ReadWrite,	DirectValue,	"Opacity, usually within the interval [~0 .. 1] = [transparent .. solid] (2.0 - Ray Traversal)",																		"")
		pl_attribute(IsosurfaceValue,				float,								0.2f,								ReadWrite,	DirectValue,	"Isosurface value, usually within the interval [0 .. 1] (2.0 - Ray Traversal)",																							"")
		// 2.2 - Reconstruction
		// 2.2 - Fetch Scalar
		pl_attribute(Reconstruction,				pl_enum_type(EReconstruction),		TrilinearFiltering,					ReadWrite,	DirectValue,	"Continues volume reconstruction (2.2 - Fetch Scalar)",																													"")
		pl_attribute(VolumeTextureLOD,				float,								0.0,								ReadWrite,	DirectValue,	"Volume texture level of detail (0...<number of mipmaps>-1), usually the value 3 shows a well notable effect (2.2 - Reconstruction)(2.2 - Fetch Scalar)",				"Min='0'")
		// 2.3 - Shading
		pl_attribute(Shading,						pl_enum_type(EShading),				ShadingLighting,					ReadWrite,	DirectValue,	"Technique to use for the shading (2.3 - Shading)",																														"")
		// 2.4 - Classification
		pl_attribute(Classification,				pl_enum_type(EClassification),		ClassificationTransferFunction,		ReadWrite,	DirectValue,	"Technique to use for the classification (2.4 - Classification)",																										"")
		pl_attribute(ScalarClassificationThreshold,	float,								0.06f,								ReadWrite,	DirectValue,	"Scalar classification threshold (2.4 - Classification)",																												"")
		// 2.5 - Gradient
		pl_attribute(Gradient,						pl_enum_type(EGradient),			GradientCentralDifferences,			ReadWrite,	DirectValue,	"Technique to use for the gradient (2.5 - Gradient)",																													"")
		pl_attribute(PostClassificationGradient,	bool,								false,								ReadWrite,	DirectValue,	"If 'true', the gradient is calculated using the alpha channel of the classified scalar, else the unclassified scalar is used (better performance)(2.5 - Gradient)",	"")
		// 2.6 - Illumination
		pl_attribute(Illumination,					pl_enum_type(EIllumination),		IlluminationBlinnPhong,				ReadWrite,	DirectValue,	"Technique to use for the illumination (2.6 - Illumination)",																											"")
			// Overwritten PLScene::SceneNode attributes
		pl_attribute(Flags,							pl_flag_type(EFlags),				0,									ReadWrite,	GetSet,			"Flags",																																								"")
		pl_attribute(AABBMin,						PLMath::Vector3,					PLMath::Vector3(0.0f, 0.0f, 0.0f),	ReadWrite,	GetSet,			"Minimum position of the 'scene node space' axis aligned bounding box",																									"")
		pl_attribute(AABBMax,						PLMath::Vector3,					PLMath::Vector3(1.0f, 1.0f, 1.0f),	ReadWrite,	GetSet,			"Maximum position of the 'scene node space' axis aligned bounding box",																									"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLVOLUME_API PLCore::String GetVolumeFilename() const;
		PLVOLUME_API void SetVolumeFilename(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUME_API SNVolume();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~SNVolume();

		/**
		*  @brief
		*    Returns the used volume resource
		*
		*  @return
		*    The used volume resource, can be a null pointer
		*/
		PLVOLUME_API Volume *GetVolume() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String  m_sVolumeFilename;	/**< Filename of the volume data to use */
		VolumeHandler  *m_pVolumeHandler;	/**< Volume resource handler, always valid */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loads/reloads the volume
		*/
		void Load();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_SNVOLUME_H__
