/*********************************************************\
 *  File: SCSound.h                                      *
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


#ifndef __PLSOUND_SCENENODES_SCSOUND_H__
#define __PLSOUND_SCENENODES_SCSOUND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodeHandler.h>
#include "PLSound/PLSound.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSound {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SoundManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sound scene node container
*/
class SCSound : public PLScene::SceneContainer {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLSOUND_RTTI_EXPORT, SCSound, "PLSound", PLScene::SceneContainer, "Sound scene node container")
		// Attributes
	#ifdef ANDROID
		// [TODO] Make PLSoundOpenSLES to the default (because it's open and free) as soon as it's implemented
		pl_attribute(SoundAPI,	PLCore::String,	"PLSoundFMODEx::SoundManager",		ReadWrite,	DirectValue,	"Name of the sound API this scene container is using",						"")
//		pl_attribute(SoundAPI,	PLCore::String,	"PLSoundOpenSLES::SoundManager",	ReadWrite,	DirectValue,	"Name of the sound API this scene container is using",						"")
	#else
		pl_attribute(SoundAPI,	PLCore::String,	"PLSoundOpenAL::SoundManager",		ReadWrite,	DirectValue,	"Name of the sound API this scene container is using",						"")
	#endif
		pl_attribute(Volume,	float,			1.0f,								ReadWrite,	GetSet,			"Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)",			"Min='0.0' Max='1.0'")
		pl_attribute(Pitch,		float,			1.0f,								ReadWrite,	GetSet,			"Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster)",	"Min='0.0'")
		pl_attribute(Listener,	PLCore::String,	"",									ReadWrite,	GetSet,			"Name of the scene node used as listener",									"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Slots
		pl_slot_0(OnUpdate,	"Called when the scene node needs to be updated",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLSOUND_API float GetVolume() const;
		PLSOUND_API void SetVolume(float fValue);
		PLSOUND_API float GetPitch() const;
		PLSOUND_API void SetPitch(float fValue);
		PLSOUND_API PLCore::String GetListener() const;
		PLSOUND_API void SetListener(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLSOUND_API SCSound();

		/**
		*  @brief
		*    Destructor
		*/
		PLSOUND_API virtual ~SCSound();

		/**
		*  @brief
		*    Returns the PL sound manager
		*
		*  @return
		*    The PL sound manager, a null pointer on error (if that's the case, something went totally wrong :)
		*/
		PLSOUND_API SoundManager *GetSoundManager() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float						m_fVolume;				/**< Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume) */
		float						m_fPitch;				/**< Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster) */
		PLCore::String				m_sListener;			/**< Name of the scene node used as listener */
		SoundManager			   *m_pSoundManager;		/**< The PL sound manager, can be a null pointer */
		PLScene::SceneNodeHandler   m_cListenerNodeHandler;	/**< Listener scene node handler */


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNode functions        ]
	//[-------------------------------------------------------]
	protected:
		PLSOUND_API virtual void InitFunction() override;
		PLSOUND_API virtual void OnActivate(bool bActivate) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound


#endif // __PLSOUND_SCENENODES_SCSOUND_H__
