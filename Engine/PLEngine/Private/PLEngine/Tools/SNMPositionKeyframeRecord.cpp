/*********************************************************\
 *  File: SNMPositionKeyframeRecord.cpp                  *
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
#include <PLCore/Tools/Chunk.h>
#include <PLCore/Tools/Timing.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include "PLEngine/Tools/SNMPositionKeyframeRecord.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPositionKeyframeRecord)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPositionKeyframeRecord::SNMPositionKeyframeRecord(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	FramesPerSecond(this),
	Keys(this),
	CoordinateSystem(this),
	SlotOnUpdate(this),
	m_bRecording(false),
	m_fFrame(0.0f),
	m_nFrame(0),
	m_pChunk(new Chunk())
{
	// Set chunk semantic
	m_pChunk->SetSemantic(Chunk::Position);
}

/**
*  @brief
*    Destructor
*/
SNMPositionKeyframeRecord::~SNMPositionKeyframeRecord()
{
	// Stop the record
	StopRecord();

	// Destroy the chunk
	delete m_pChunk;
}

/**
*  @brief
*    Returns the chunk holding the keyframe data
*/
Chunk &SNMPositionKeyframeRecord::GetChunk()
{
	return *m_pChunk;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNMPositionKeyframeRecord::OnUpdate()
{
	// Update the current frame
	m_fFrame += float(FramesPerSecond)*Timing::GetInstance()->GetTimeDifference();

	// Is it time to record the next key?
	if (m_nFrame != int(m_fFrame)) {
		// Get the current position of the owner scene node
		Vector3 vPosition = GetSceneNode().GetTransform().GetPosition();

		// Transform the position into the requested scene container space?
		if (CoordinateSystem.Get().GetLength()) {
			// Get the scene container the position keys should be in
			SceneNode *pTargetSceneNode = GetSceneNode().GetContainer()->GetByName(CoordinateSystem);
			if (pTargetSceneNode && pTargetSceneNode->IsContainer()) {
				// Get the transform matrix that transform from "this scene container" into "the other scene container"
				Matrix3x4 mTransform;
				GetSceneNode().GetContainer()->GetTransformMatrixTo((SceneContainer&)*pTargetSceneNode, mTransform);

				// Transform the position into the requested coordinate system
				vPosition *= mTransform;
			}
		}

		// Record the current position of the owner scene node
		m_lstData.Add(vPosition);

		// Backup the current frame as integer
		m_nFrame = int(m_fFrame);
	}
}

/**
*  @brief
*    Starts the record
*/
void SNMPositionKeyframeRecord::StartRecord()
{
	// Stop the previous recording
	StopRecord();

	// Clear the previous chunk data
	m_pChunk->Clear();

	// Recording is now enabled
	m_bRecording = true;

	// Set initial frame
	m_fFrame = 0.0f;
	m_nFrame = -1;	// -1 so force to generate a new key at once
}

/**
*  @brief
*    Stops the record
*/
void SNMPositionKeyframeRecord::StopRecord()
{
	// Is recording currently enabled?
	if (m_bRecording) {
		// Recording is no longer enabled
		m_bRecording = false;

		// Fill the chunk with the recorded data
		m_pChunk->Allocate(PLCore::Chunk::Float, 3, m_lstData.GetNumOfElements());
		if (m_lstData.GetNumOfElements()) {
			// Get a direct pointer to the chunk data
			float *pfChunkData = (float*)m_pChunk->GetData();

			// Loop through the recorded data
			Iterator<Vector3> cIterator = m_lstData.GetIterator();
			while (cIterator.HasNext()) {
				// Get the recorded position
				const Vector3 &vPosition = cIterator.Next();

				// x
				*pfChunkData = vPosition.x;
				pfChunkData++;

				// y
				*pfChunkData = vPosition.y;
				pfChunkData++;

				// z
				*pfChunkData = vPosition.z;
				pfChunkData++;
			}
		}

		// Clear the recorded data (it's not within the chunk!)
		m_lstData.Clear();

		// If a chunk filename is given, save the chunk right now
		if (Keys.Get().GetLength())
			m_pChunk->SaveByFilename(Keys);
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNodeModifier functions  ]
//[-------------------------------------------------------]
void SNMPositionKeyframeRecord::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler and start/stop the record
	if (bActivate) {
		// Start the record
		StartRecord();

		// Connect event handler
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Connect(SlotOnUpdate);
	} else {
		// Stop the record
		StopRecord();

		// Disconnect event handler
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Disconnect(SlotOnUpdate);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
