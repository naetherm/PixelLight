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


#ifndef __PLSOUNDNULL_SOUNDMANAGER_H__
#define __PLSOUNDNULL_SOUNDMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLSound/SoundManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundNull {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null sound backend
*/
class SoundManager : public PLSound::SoundManager {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SoundManager, "PLSoundNull", PLSound::SoundManager, "Null sound backend")
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
		SoundManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SoundManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float			m_fVolume;							   /**< Master volume */
		float			m_fPitch;							   /**< Master pitch */
		float			m_fDopplerFactor;					   /**< Doppler factor */
		PLMath::Vector3 m_vListenerAttributes[ListenerNumber]; /**< Listener attributes */


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
} // PLSoundNull


#endif // __PLSOUNDNULL_SOUNDMANAGER_H__
