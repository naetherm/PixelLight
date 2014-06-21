/*********************************************************\
 *  File: Source.h                                       *
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


#ifndef __PLSOUNDOPENAL_SOURCE_H__
#define __PLSOUNDOPENAL_SOURCE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLSound/Source.h>
#include "PLSoundOpenAL/PLSoundOpenAL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Stream;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenAL sound source resource
*/
class Source : public PLSound::Source {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SoundManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Source();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSoundManager
		*    Owner sound manager
		*/
		Source(PLSound::SoundManager &cSoundManager);

		/**
		*  @brief
		*    Updates the source
		*
		*  @note
		*    - This must only be done for currently active sources
		*/
		void Update();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ALuint			 m_nSource;				/**< Sound source */
		Stream			*m_pStream;				/**< Sound source stream if streamed buffer is used, else a null pointer */
		bool			 m_b2D;					/**< Is this a 2D sound? */
		bool			 m_bLooping;			/**< Is the source looping? */
		float			 m_fPitch;				/**< Pitch */
		float			 m_fReferenceDistance;	/**< Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively */
		float			 m_fMaxDistance;		/**< Defines a distance beyond which the source will not be further attenuated by distance */
		float			 m_fRolloffFactor;		/**< This will scale the distance attenuation over the applicable range */
		PLMath::Vector3  m_vPosition;			/**< Sound position */
		PLMath::Vector3  m_vVelocity;			/**< Sound velocity */


	//[-------------------------------------------------------]
	//[ Public virtual PLSound::Source functions              ]
	//[-------------------------------------------------------]
	public:
		virtual bool Load(PLSound::Buffer *pBuffer = nullptr) override;
		virtual void Unload() override;
		virtual bool Play(bool bRestart = false) override;
		virtual bool IsPlaying() const override;
		virtual void Pause() override;
		virtual bool IsPaused() const override;
		virtual void Stop() override;
		virtual float GetVolume() const override;
		virtual void SetVolume(float fVolume = 1.0f) override;
		virtual bool Is2D() const override;
		virtual void Set2D(bool b2D = false) override;
		virtual bool IsLooping() const override;
		virtual void SetLooping(bool bLooping = false) override;
		virtual float GetPitch() const override;
		virtual void SetPitch(float fPitch = 1.0f) override;
		virtual float GetReferenceDistance() const override;
		virtual void SetReferenceDistance(float fReferenceDistance = 1.0f) override;
		virtual float GetMaxDistance() const override;
		virtual void SetMaxDistance(float fMaxDistance = 10000.0f) override;
		virtual float GetRolloffFactor() const override;
		virtual void SetRolloffFactor(float fRolloffFactor = 1.0f) override;
		virtual PLMath::Vector3 GetAttribute(EAttributes nAttribute) const override;
		virtual bool SetAttribute(EAttributes nAttribute, const PLMath::Vector3 &vV) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL


#endif // __PLSOUNDOPENAL_SOURCE_H__
