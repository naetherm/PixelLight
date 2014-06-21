/*********************************************************\
 *  File: SNMPositionKeyframeRecord.h                    *
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


#ifndef __PLENGINE_TOOLS_SNMPOSITIONKEYFRAMERECORD_H__
#define __PLENGINE_TOOLS_SNMPOSITIONKEYFRAMERECORD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNodeModifiers/SNMTransform.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Chunk;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Keyframe position record scene node modifier class
*
*  @remarks
*    If this modifier is active, it records the position of the scene nodes over time using keyframes.
*    When the modifier get's inactive or destroyed, the recorded keyframes are written into a chunk file.
*
*  @note
*    - The "CoordinateSystem" attribute can be quite useful if the scene contains portals
*/
class SNMPositionKeyframeRecord : public PLScene::SNMTransform {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNMPositionKeyframeRecord, "PLEngine", PLScene::SNMTransform, "Keyframe position animation scene node modifier class")
		// Attributes
		pl_attribute(FramesPerSecond,	PLCore::uint32,	24,	ReadWrite,	DirectValue,	"Frames per second",																											"")
		pl_attribute(Keys,				PLCore::String,	"",	ReadWrite,	DirectValue,	"Position keys (x, y and z) chunk filename",																					"")
		pl_attribute(CoordinateSystem,	PLCore::String,	"",	ReadWrite,	DirectValue,	"Name of the scene container the position keys are in, empty string means scene container of the modifiers owner scene node",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_0(OnUpdate,	"Called when the scene node needs to be updated",	"")
	pl_class_end


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
		PL_API SNMPositionKeyframeRecord(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNMPositionKeyframeRecord();

		/**
		*  @brief
		*    Returns the chunk holding the keyframe data
		*
		*  @return
		*    The chunk holding the keyframe data (only with valid/current data after the record was stopped)
		*/
		PL_API PLCore::Chunk &GetChunk();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();

		/**
		*  @brief
		*    Starts the record
		*
		*  @note
		*    - Called when the modifier gets active
		*    - The previous chunk data will be destroyed
		*/
		void StartRecord();

		/**
		*  @brief
		*    Stops the record
		*
		*  @note
		*    - Called when the modifier get's inactive or destroyed
		*    - If "Keys" is not empty, the chunk is saved
		*/
		void StopRecord();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						   m_bRecording;	/**< Is recording currently enabled? */
		PLCore::List<PLMath::Vector3>  m_lstData;		/**< Recorded data */
		float						   m_fFrame;		/**< Current frame as floating point */
		int							   m_nFrame;		/**< Current frame as integer */
		PLCore::Chunk				  *m_pChunk;		/**< Chunk holding the keyframe data, always valid! */


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNodeModifier functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void OnActivate(bool bActivate) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_TOOLS_SNMPOSITIONKEYFRAMERECORD_H__
