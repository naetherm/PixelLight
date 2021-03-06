/*********************************************************\
 *  File: SoundManager.h                                 *
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


#ifndef __PLSOUNDFMODEX_SOUNDMANAGER_H__
#define __PLSOUNDFMODEX_SOUNDMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLSound/SoundManager.h>
#include "PLSoundFMODEx/PLSoundFMODEx.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundFMODEx {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    FMOD Ex sound backend
*/
class SoundManager : public PLSound::SoundManager {


	//[-------------------------------------------------------]
	//[ Public data types                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Output
		*/
		enum EOutput {
			AUTODETECT,
			UNKNOWN,
			NOSOUND,
			WAVWRITER,
			NOSOUND_NRT,
			WAVWRITER_NRT,
			DSOUND,
			WINMM,
			ASIO,
			OSS,
			ALSA,
			ESD,
			SOUNDMANAGER,
			COREAUDIO,
			XBOX,
			PS2,
			PS3,
			GC,
			XBOX360,
			PSP,
			WII,
			MAX
		};
		pl_enum(EOutput)
			pl_enum_value(AUTODETECT,		"")
			pl_enum_value(UNKNOWN,			"")
			pl_enum_value(NOSOUND,			"")
			pl_enum_value(WAVWRITER,		"")
			pl_enum_value(NOSOUND_NRT,		"")
			pl_enum_value(WAVWRITER_NRT,	"")
			pl_enum_value(DSOUND,			"")
			pl_enum_value(WINMM,			"")
			pl_enum_value(ASIO,				"")
			pl_enum_value(OSS,				"")
			pl_enum_value(ALSA,				"")
			pl_enum_value(ESD,				"")
			pl_enum_value(SOUNDMANAGER,		"")
			pl_enum_value(COREAUDIO,		"")
			pl_enum_value(XBOX,				"")
			pl_enum_value(PS2,				"")
			pl_enum_value(PS3,				"")
			pl_enum_value(GC,				"")
			pl_enum_value(XBOX360,			"")
			pl_enum_value(PSP,				"")
			pl_enum_value(WII,				"")
			pl_enum_value(MAX,				"")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Attributes
		pl_attribute_directvalue(Driver,		PLCore::int32,	-1,			ReadWrite)
		pl_attribute_directvalue(Output,		EOutput,		AUTODETECT,	ReadWrite)
		pl_attribute_directvalue(Channels,		PLCore::uint32,	32,			ReadWrite)
		pl_attribute_directvalue(BufferLength,	PLCore::uint32,	1024,		ReadWrite)
		pl_attribute_directvalue(NumOfBuffers,	PLCore::uint32,	4,			ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Checks for an FMOD Ex error
		*
		*  @param[in] nResult
		*    FMOD Ex error code to check
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		static bool ErrorCheck(FMOD_RESULT nResult);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SoundManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SoundManager();

		/**
		*  @brief
		*    Adds an active sound source to the sound manager
		*
		*  @param[in] cSource
		*    Sound source to add
		*
		*  @return
		*    'true' if all went fine and the sound source was added to the sound manager,
		*    else 'false'
		*
		*  @note
		*    - The sound source itself is only added to the sound manager's list of sources!
		*/
		bool AddActiveSource(PLSound::Source &cSource);

		/**
		*  @brief
		*    Removes an active sound source from the sound manager
		*
		*  @param[in] cSource
		*    Sound source to remove
		*
		*  @return
		*    'true' if all went fine and the sound source was removed from the
		*    sound manager, else 'false' (maybe the source isn't in the sound manager)
		*
		*  @note
		*    - The sound source itself isn't destroyed, it is just removed
		*      from the sound manager's list of sources! 
		*/
		bool RemoveActiveSource(PLSound::Source &cSource);

		/**
		*  @brief
		*    Returns the FMOD Ex system instance
		*
		*  @return
		*    The FMOD Ex system instance, a null pointer on error
		*/
		FMOD::System *GetSystem() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FMOD::System				    *m_pSystem;								/**< FMOD Ex system instance, can be a null pointer */
		float							 m_fVolume;								/**< Master volume */
		float							 m_fPitch;								/**< Master pitch */
		float							 m_fDopplerFactor;						/**< Doppler factor */
		PLMath::Vector3					 m_vListenerAttributes[ListenerNumber];	/**< Listener attributes */
		PLCore::Array<PLSound::Source*>  m_lstActiveSources;					/**< Current active sound sources of this sound manager */


	//[-------------------------------------------------------]
	//[ Public virtual PLSound::SoundManager functions        ]
	//[-------------------------------------------------------]
	public:
		virtual PLCore::String GetDescription() const override;
		virtual bool GetFormatList(PLCore::List<Format> &lstList) const override;
		virtual float GetVolume() const override;
		virtual void SetVolume(float fVolume = 1.0f) override;
		virtual float GetPitch() const override;
		virtual void SetPitch(float fPitch = 1.0f) override;
		virtual float GetDopplerFactor() const override;
		virtual void SetDopplerFactor(float fFactor = 1.0f) override;

		//[-------------------------------------------------------]
		//[ Create sound buffer/source                            ]
		//[-------------------------------------------------------]
		virtual PLSound::Buffer *CreateSoundBuffer(const PLCore::String &sFilename = "", bool bStream = false) override;
		virtual PLSound::Source *CreateSoundSource(PLSound::Buffer *pSoundBuffer = nullptr) override;

		//[-------------------------------------------------------]
		//[ Listener                                              ]
		//[-------------------------------------------------------]
		virtual PLMath::Vector3 GetListenerAttribute(EListener nAttribute) const override;
		virtual bool SetListenerAttribute(EListener nAttribute, const PLMath::Vector3 &vV) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::ResourceManager functions      ]
	//[-------------------------------------------------------]
	public:
		virtual bool Init() override;
		virtual bool DeInit() override;
		virtual bool Update() override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ResourceManager functions     ]
	//[-------------------------------------------------------]
	private:
		virtual PLSound::Buffer *CreateResource(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundFMODEx


#endif // __PLSOUNDFMODEX_SOUNDMANAGER_H__
