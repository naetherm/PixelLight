/*********************************************************\
 *  File: SNMPositionKeyframeAnimation.h                 *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_POSITIONKEYFRAMEANIMATION_H__
#define __PLSCENE_SCENENODEMODIFIER_POSITIONKEYFRAMEANIMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include "PLScene/Scene/SceneNodeModifiers/SNMTransform.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Chunk;
}
namespace PLRenderer {
	class Animation;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Keyframe position animation scene node modifier class
*
*  @remarks
*    Animates the position of the scene nodes over time using keyframes.
*
*  @note
*    - The "CoordinateSystem" attribute can be quite useful if the scene contains portals
*/
class SNMPositionKeyframeAnimation : public SNMTransform {


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node modifier flags (SceneNodeModifier flags extension)
		*/
		enum EFlags {
			PlaybackNoLoop	 = 1<<2,	/**< No loop animation */
			PlaybackPingPong = 1<<3		/**< Ping pong animation */
		};
		pl_flag(EFlags)
			pl_enum_base(SNMTransform::EFlags)
			pl_enum_value(PlaybackNoLoop,	"No loop animation")
			pl_enum_value(PlaybackPingPong,	"Ping pong animation")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLS_API)
		// Attributes
		pl_attribute_directvalue(								Speed,				float,			1.0f,	ReadWrite)
		pl_attribute_directvalue(								FramesPerSecond,	PLCore::uint32,	24,		ReadWrite)
		pl_attribute_getset		(SNMPositionKeyframeAnimation,	Keys,				PLCore::String,	"",		ReadWrite)
		pl_attribute_directvalue(								CoordinateSystem,	PLCore::String,	"",		ReadWrite)
			// Overwritten SceneNodeModifier attributes
		pl_attribute_getset		(SNMPositionKeyframeAnimation,	Flags,				PLCore::uint32,	0,		ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetKeys() const;
		PLS_API void SetKeys(const PLCore::String &sValue);


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
		PLS_API SNMPositionKeyframeAnimation(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMPositionKeyframeAnimation();

		/**
		*  @brief
		*    Returns the chunk holding the keyframe data
		*
		*  @return
		*    The chunk holding the keyframe data
		*/
		PLS_API PLCore::Chunk &GetChunk();

		/**
		*  @brief
		*    Returns the animation controller
		*
		*  @return
		*    The animation controller
		*/
		PLS_API PLRenderer::Animation &GetAnimation();


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNodeModifier functions         ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node modifier needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String		   m_sKeys;			/**< Position keys (x, y and z) chunk filename */
		PLCore::Chunk		  *m_pChunk;		/**< Chunk holding the keyframe data, always valid! */
		PLRenderer::Animation *m_pAnimation;	/**< Animation */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_POSITIONKEYFRAMEANIMATION_H__
