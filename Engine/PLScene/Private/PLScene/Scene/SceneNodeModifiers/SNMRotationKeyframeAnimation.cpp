/*********************************************************\
 *  File: SNMRotationKeyframeAnimation.cpp               *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Log/Log.h>
#include <PLCore/Tools/Chunk.h>
#include <PLCore/Tools/Timing.h>
#include <PLRenderer/Animation/Animation.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMRotationKeyframeAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMRotationKeyframeAnimation)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMRotationKeyframeAnimation::GetKeys() const
{
	return m_sKeys;
}

void SNMRotationKeyframeAnimation::SetKeys(const String &sValue)
{
	if (m_sKeys != sValue) {
		m_sKeys = sValue;

		// Load the chunk
		if (m_pChunk->LoadByFilename(m_sKeys)) {
			bool bValid = false;

			// Check semantic
			if (m_pChunk->GetSemantic() == Chunk::Rotation) {
				// Check components per element
				if (m_pChunk->GetNumOfComponentsPerElement() == 4) {
					bValid = true;
				} else {
					PL_LOG(Error, m_sKeys + ": 4 components per element required!")
				}
			} else {
				PL_LOG(Error, m_sKeys + ": Rotation chunk required!")
			}

			// Unload the chunk if it's invalid
			if (!bValid)
				m_pChunk->Unload();
		}

		// Get animation playback flags
		uint32 nAnimationPlaybackFlags = 0;
		if (!(GetFlags() & PlaybackNoLoop))
			nAnimationPlaybackFlags |= Animation::Loop;
		if (GetFlags() & PlaybackPingPong)
			nAnimationPlaybackFlags |= Animation::PingPong;

		// Start the animation
		m_pAnimation->Start(0, m_pChunk->GetNumOfElements()-1, static_cast<float>(FramesPerSecond), nAnimationPlaybackFlags);
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMRotationKeyframeAnimation::SNMRotationKeyframeAnimation(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Speed(this),
	FramesPerSecond(this),
	Keys(this),
	Flags(this),
	EventHandlerUpdate(&SNMRotationKeyframeAnimation::OnUpdate, this),
	m_pChunk(new Chunk()),
	m_pAnimation(new Animation())
{
	// Set chunk semantic
	m_pChunk->SetSemantic(Chunk::Rotation);
}

/**
*  @brief
*    Destructor
*/
SNMRotationKeyframeAnimation::~SNMRotationKeyframeAnimation()
{
	delete m_pChunk;
	delete m_pAnimation;
}

/**
*  @brief
*    Returns the chunk holding the keyframe data
*/
Chunk &SNMRotationKeyframeAnimation::GetChunk()
{
	return *m_pChunk;
}

/**
*  @brief
*    Returns the animation controller
*/
Animation &SNMRotationKeyframeAnimation::GetAnimation()
{
	return *m_pAnimation;
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMRotationKeyframeAnimation::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate) {
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);

			// Make a first update to ensure everything is up-to-date when we're going active (synchronization and logic update)
			OnUpdate();
		} else {
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMRotationKeyframeAnimation::OnUpdate()
{
	// Are there any keys?
	const float *pfData = reinterpret_cast<const float*>(m_pChunk->GetData());
	if (pfData) {
		// Update the frame
		m_pAnimation->Update(Speed*Timing::GetInstance()->GetTimeDifference());

		// Get the current frame
		const uint32 nCurrentFrame = m_pAnimation->GetCurrentFrame();
		if (nCurrentFrame < m_pChunk->GetNumOfElements()) {
			// Get the first component of the current frame
			const uint32 nCurrentFrameFirstComponent = nCurrentFrame*m_pChunk->GetNumOfComponentsPerElement();
			if (nCurrentFrameFirstComponent+m_pChunk->GetNumOfComponentsPerElement() <= m_pChunk->GetTotalNumOfComponents()) {
				// Get the current rotation
				const Quaternion qCurrentRotation = &pfData[nCurrentFrameFirstComponent];

				// Get the next frame
				const uint32 nNextFrame = m_pAnimation->GetNextFrame();
				if (nNextFrame < m_pChunk->GetNumOfElements()) {
					// Get the first component of the next frame
					const uint32 nNextFrameFirstComponent = nNextFrame*m_pChunk->GetNumOfComponentsPerElement();
					if (nNextFrameFirstComponent+m_pChunk->GetNumOfComponentsPerElement() <= m_pChunk->GetTotalNumOfComponents()) {
						// Get the next rotation
						const Quaternion qNextRotation = &pfData[nNextFrameFirstComponent];

						// Get the interpolated rotation
						Quaternion qResultRotation;
						qResultRotation.Slerp(qCurrentRotation, qNextRotation, m_pAnimation->GetProgress());

						// Set scene node rotation
						GetSceneNode().GetTransform().SetRotation(qResultRotation);
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
