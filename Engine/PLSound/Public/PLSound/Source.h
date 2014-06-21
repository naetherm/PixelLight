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


#ifndef __PLSOUND_SOURCE_H__
#define __PLSOUND_SOURCE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLSound/Resource.h"
#include "PLSound/BufferHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSound {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Buffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract sound source resource
*
*  @note
*    - For sources with 3D spatialization, do only use one channel buffers because not each sound backend may be capable to use 3D spatialization for buffers with multiple channels
*/
class Source : public Resource {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sound source flags
		*/
		enum EFlags {
			NoMasterPitch = 1<<0	/**< The master pitch has no influence on this source */
		};

		/**
		*  @brief
		*    Source attributes
		*/
		enum EAttributes {
			Position        = 0,	/**< Position (float3, x/y/z) */
			Velocity        = 1,	/**< Velocity in meters per second (float3, x/y/z) */
			NumOfAttributes = 2		/**< Number of source attributes */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLSOUND_API virtual ~Source();

		/**
		*  @brief
		*    Returns the buffer the source is using
		*
		*  @return
		*    The used sound buffer, a null pointer on error
		*/
		PLSOUND_API Buffer *GetBuffer() const;

		/**
		*  @brief
		*    Returns the source flags
		*
		*  @return
		*    Source flags (see EFlags)
		*/
		PLSOUND_API PLCore::uint32 GetFlags() const;

		/**
		*  @brief
		*    Sets the source flags
		*
		*  @param[in] nFlags
		*    Source flags (see EFlags)
		*/
		PLSOUND_API void SetFlags(PLCore::uint32 nFlags = 0);


	//[-------------------------------------------------------]
	//[ Public virtual Source functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Loads the sound source with the given sound buffer
		*
		*  @param[in] pBuffer
		*    Sound buffer the source should load, can be a null pointer (in this case just the same as Unload())
		*
		*  @return
		*   'true' if all went fine, else 'false'
		*/
		PLSOUND_API virtual bool Load(Buffer *pBuffer = nullptr);

		/**
		*  @brief
		*    Unload the source
		*/
		PLSOUND_API virtual void Unload();

		/**
		*  @brief
		*    Plays the source
		*
		*  @param[in] bRestart
		*    Restart source if it is already playing?
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no buffer is loaded)
		*/
		virtual bool Play(bool bRestart = false) = 0;

		/**
		*  @brief
		*    Checks whether the source is currently playing or not
		*
		*  @return
		*    'true' if the source is playing at the moment, else 'false'
		*
		*  @note
		*    - If the source is paused this function will return 'true' because the
		*      source is played at the moment - but paused :)
		*/
		virtual bool IsPlaying() const = 0;

		/**
		*  @brief
		*    Pauses the source playback
		*
		*  @note
		*    - Use Play() to continue the playback
		*/
		virtual void Pause() = 0;

		/**
		*  @brief
		*    Checks whether the source is currently paused or not
		*
		*  @return
		*    'true' if the source is paused at the moment, else 'false'
		*/
		virtual bool IsPaused() const = 0;

		/**
		*  @brief
		*    Stops the source playback
		*/
		virtual void Stop() = 0;

		/**
		*  @brief
		*    Returns the volume
		*
		*  @return
		*    Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		*/
		virtual float GetVolume() const = 0;

		/**
		*  @brief
		*    Sets the volume
		*
		*  @param[in] fVolume
		*    Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		*/
		virtual void SetVolume(float fVolume = 1.0f) = 0;

		/**
		*  @brief
		*    Returns whether the source is 2D or not
		*
		*  @return
		*    'true' if source is 2D, else 'false'
		*/
		virtual bool Is2D() const = 0;

		/**
		*  @brief
		*    Sets whether the source is 2D or not
		*
		*  @param[in] b2D
		*    If 'true' the source is 2D, else 'false'
		*/
		virtual void Set2D(bool b2D = false) = 0;

		/**
		*  @brief
		*    Returns whether the source is looping or not
		*
		*  @return
		*    'true' if source is looping, else 'false'
		*/
		virtual bool IsLooping() const = 0;

		/**
		*  @brief
		*    Sets whether the source is looping or not
		*
		*  @param[in] bLooping
		*    If 'true' the source is looping, else 'false'
		*/
		virtual void SetLooping(bool bLooping = false) = 0;

		/**
		*  @brief
		*    Returns the pitch multiplier
		*
		*  @return
		*    Pitch multiplier
		*
		*  @remarks
		*    pitch < 1.0 = slower\n
		*    pitch = 1.0 = normal\n
		*    pitch > 1.0 = faster
		*/
		virtual float GetPitch() const = 0;

		/**
		*  @brief
		*    Sets the pitch multiplier
		*
		*  @param[in] fPitch
		*    Pitch multiplier
		*
		*  @see
		*    - GetPitch()
		*/
		virtual void SetPitch(float fPitch = 1.0f) = 0;

		/**
		*  @brief
		*    Returns the reference distance
		*
		*  @return
		*    Reference distance
		*
		*  @remarks
		*    Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively.
		*/
		virtual float GetReferenceDistance() const = 0;

		/**
		*  @brief
		*    Sets the reference distance
		*
		*  @param[in] fReferenceDistance
		*    Reference distance
		*
		*  @note
		*    - See GetReferenceDistance()
		*/
		virtual void SetReferenceDistance(float fReferenceDistance = 1.0f) = 0;

		/**
		*  @brief
		*    Returns the maximum distance 
		*
		*  @return
		*    Maximum distance
		*
		*  @remarks
		*    Defines a distance beyond which the source will not be further attenuated by distance.
		*/
		virtual float GetMaxDistance() const = 0;

		/**
		*  @brief
		*    Sets the maximum distance
		*
		*  @param[in] fMaxDistance
		*    Maximum distance
		*
		*  @see
		*    - GetMaxDistance()
		*/
		virtual void SetMaxDistance(float fMaxDistance = 10000.0f) = 0;

		/**
		*  @brief
		*    Returns the roll off factor
		*
		*  @return
		*    Roll off factor
		*
		*  @remarks
		*    This will scale the distance attenuation over the applicable range.
		*/
		virtual float GetRolloffFactor() const = 0;

		/**
		*  @brief
		*    Sets the roll off factor
		*
		*  @param[in] fRolloffFactor
		*    Roll off factor
		*
		*  @see
		*    - GetRolloffFactor()
		*/
		virtual void SetRolloffFactor(float fRolloffFactor = 1.0f) = 0;

		/**
		*  @brief
		*    Gets a source attribute value
		*
		*  @param[in] nAttribute
		*    Source attribute to return
		*
		*  @return
		*    Requested source attribute value
		*/
		virtual PLMath::Vector3 GetAttribute(EAttributes nAttribute) const = 0;

		/**
		*  @brief
		*    Sets a source attribute value
		*
		*  @param[in] nAttribute
		*    Source attribute to set
		*  @param[in] vV
		*    New source attribute value
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool SetAttribute(EAttributes nAttribute, const PLMath::Vector3 &vV) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSoundManager
		*    Owner sound manager
		*/
		PLSOUND_API Source(SoundManager &cSoundManager);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Source(const Source &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Source &operator =(const Source &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BufferHandler  m_cBufferHandler;	/**< Sound buffer the source is using */
		PLCore::uint32 m_nFlags;			/**< Flags */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound


#endif // __PLSOUND_SOURCE_H__
