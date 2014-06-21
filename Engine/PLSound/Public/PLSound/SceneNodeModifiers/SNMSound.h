/*********************************************************\
 *  File: SNMSound.h                                     *
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


#ifndef __PLSOUND_SCENENODEMODIFIERS_SNMSOUND_H__
#define __PLSOUND_SCENENODEMODIFIERS_SNMSOUND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNodeModifier.h>
#include "PLSound/PLSound.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSound {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Source;
class SCSound;
class ResourceHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node modifier which is able to playback a sound
*
*  @note
*    - For sources with 3D spatialization, do only use one channel buffers because not each sound backend may be capable to use 3D spatialization for buffers with multiple channels
*/
class SNMSound : public PLScene::SceneNodeModifier {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node modifier flags (SceneNodeModifier flags extension)
		*/
		enum EFlags {
			Stream          = 1<<2,	/**< Stream the file (recommended for large files!) */
			No3D            = 1<<3,	/**< No 3D sound */
			NoLoop          = 1<<4,	/**< Do not loop the sound playback */
			NoStartPlayback = 1<<5	/**< Do not start sound playback after scene node initialization */
		};
		pl_enum(EFlags)
			pl_enum_base(PLScene::SceneNodeModifier::EFlags)
			pl_enum_value(Stream,			"Stream the file (recommended for large files!)")
			pl_enum_value(No3D,				"No 3D sound")
			pl_enum_value(NoLoop,			"Do not loop the sound playback")
			pl_enum_value(NoStartPlayback,	"Do not start sound playback after scene node initialization")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLSOUND_RTTI_EXPORT, SNMSound, "PLSound", PLScene::SceneNodeModifier, "Scene node modifier which is able to playback a sound")
		// Attributes
		pl_attribute(Sound,				PLCore::String,			"",			ReadWrite,	GetSet,	"Name of the sound which should be played (full path, supported file formats are API dependent). For 3D spatialization, do only use one channel buffers for best compatibility.",	"Ext='mp3 ogg wav mid midi it mod s3m xm'")
		pl_attribute(Volume,			float,					1.0f,		ReadWrite,	GetSet,	"Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)",																													"Min='0.0' Max='1.0'")
		pl_attribute(Pitch,				float,					1.0f,		ReadWrite,	GetSet,	"Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster)",																											"Min='0.0'")
		pl_attribute(ReferenceDistance,	float,					1.0f,		ReadWrite,	GetSet,	"Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively",																		"Min='0.0'")
		pl_attribute(MaxDistance,		float,					10000.0f,	ReadWrite,	GetSet,	"Defines a distance beyond which the source will not be further attenuated by distance",																							"Min='0.0'")
		pl_attribute(RolloffFactor,		float,					1.0f,		ReadWrite,	GetSet,	"This will scale the distance attenuation over the applicable range",																												"Min='0.0'")
			// Overwritten PLScene::SceneNodeModifier attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,			ReadWrite,	GetSet,	"Flags",																																											"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_0(OnPosition,	"Called when the scene node position changed",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLSOUND_API PLCore::String GetSound() const;
		PLSOUND_API void SetSound(const PLCore::String &sValue);
		PLSOUND_API float GetVolume() const;
		PLSOUND_API void SetVolume(float fValue);
		PLSOUND_API float GetPitch() const;
		PLSOUND_API void SetPitch(float fValue);
		PLSOUND_API float GetReferenceDistance() const;
		PLSOUND_API void SetReferenceDistance(float fValue);
		PLSOUND_API float GetMaxDistance() const;
		PLSOUND_API void SetMaxDistance(float fValue);
		PLSOUND_API float GetRolloffFactor() const;
		PLSOUND_API void SetRolloffFactor(float fValue);
		PLSOUND_API virtual void SetFlags(PLCore::uint32 nValue) override;	// From PLScene::SceneNodeModifier


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLSOUND_API SNMSound(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLSOUND_API virtual ~SNMSound();

		/**
		*  @brief
		*    Returns the sound source
		*
		*  @return
		*    The sound source, can be a null pointer
		*/
		PLSOUND_API Source *GetSoundSource() const;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNodeModifier functions ]
	//[-------------------------------------------------------]
	protected:
		PLSOUND_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String m_sSound;				/**< Name of the sound which should be played (full path, supported file formats are API dependent) */
		float		   m_fVolume;				/**< Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume) */
		float		   m_fPitch;				/**< Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster) */
		float		   m_fReferenceDistance;	/**< Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively */
		float		   m_fMaxDistance;			/**< Defines a distance beyond which the source will not be further attenuated by distance */
		float		   m_fRolloffFactor;		/**< This will scale the distance attenuation over the applicable range */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Returns the sound container this sound is in
		*
		*  @return
		*    The sound container this sound is in, a null pointer on error
		*/
		SCSound *GetSoundContainer() const;

		/**
		*  @brief
		*    Called when the scene node position changed
		*/
		void OnPosition();

		/**
		*  @brief
		*    Loads/reloads the sound
		*/
		void Load();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ResourceHandler *m_pSoundSourceHandler;	/**< Sound source handler (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound


#endif // __PLSOUND_SCENENODEMODIFIERS_SNMSOUND_H__
