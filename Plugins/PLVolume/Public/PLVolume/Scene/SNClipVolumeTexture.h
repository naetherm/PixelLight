/*********************************************************\
 *  File: SNClipVolumeTexture.h                          *
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


#ifndef __PLVOLUME_SNCLIP_VOLUMETEXTURE_H__
#define __PLVOLUME_SNCLIP_VOLUMETEXTURE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolume/Scene/SNClip.h"


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
*    Clip volume texture scene node
*/
class SNClipVolumeTexture : public SNClip {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (PLScene::SceneNode flags extension)
		*/
		enum EFlags {
			NoTextureCompression = 1<<11,	/**< Disable the usage of texture compression? Has only an effect when loading new volume data. Lookout! When using compression, it may take a moment to compress if no compressed data is provided. */
			NoTextureMipmapping  = 1<<12,	/**< Disable the usage of mipmaps? Has only an effect when loading new volume data. Lookout! When using mipmaps, it may take a moment to generate mipmaps if no mipmap data is provided. */
			NoVolumeScale        = 1<<13	/**< When loading, do not use volume scale information to modify the scale of this scene node */
		};
		pl_enum(EFlags)
			pl_enum_base(PLVolume::SNClip::EFlags)
			pl_enum_value(NoTextureCompression,	"Disable the usage of texture compression? Has only an effect when loading new volume data. Lookout! When using compression, it may take a moment to compress if no compressed data is provided.")
			pl_enum_value(NoTextureMipmapping,	"Disable the usage of mipmaps? Has only an effect when loading new volume data. Lookout! When using mipmaps, it may take a moment to generate mipmaps if no mipmap data is provided.")
			pl_enum_value(NoVolumeScale,		"When loading, do not use volume scale information to modify the scale of this scene node")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, SNClipVolumeTexture, "PLVolume", PLVolume::SNClip, "Clip volume texture scene node")
		// Attributes
		pl_attribute(LoaderParameters,	PLCore::String,			"",										ReadWrite,	DirectValue,	"Optional parameters for the used file loader",							"")
		pl_attribute(VolumeFilename,	PLCore::String,			"",										ReadWrite,	GetSet,			"Filename of the volume data to use",									"Type='Volume'")
		pl_attribute(ClipThreshold,		float,					0.5f,									ReadWrite,	DirectValue,	"Clip threshold, everything above will be clipped",						"")
			// Overwritten PLScene::SceneNode attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,										ReadWrite,	GetSet,			"Flags",																"")
		pl_attribute(AABBMin,			PLMath::Vector3,		PLMath::Vector3(-0.5f, -0.5f, -0.5f),	ReadWrite,	GetSet,			"Minimum position of the 'scene node space' axis aligned bounding box",	"")
		pl_attribute(AABBMax,			PLMath::Vector3,		PLMath::Vector3( 0.5f,  0.5f,  0.5f),	ReadWrite,	GetSet,			"Maximum position of the 'scene node space' axis aligned bounding box",	"")
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
		PLVOLUME_API SNClipVolumeTexture();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~SNClipVolumeTexture();

		/**
		*  @brief
		*    Returns the used volume resource
		*
		*  @return
		*    The used volume resource, can be a null pointer
		*/
		PLVOLUME_API Volume *GetVolume() const;


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PLVOLUME_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loads/reloads the volume
		*/
		void Load();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String  m_sVolumeFilename;	/**< Filename of the volume data to use */
		VolumeHandler  *m_pVolumeHandler;	/**< Volume resource handler, always valid */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_SNCLIP_VOLUMETEXTURE_H__
