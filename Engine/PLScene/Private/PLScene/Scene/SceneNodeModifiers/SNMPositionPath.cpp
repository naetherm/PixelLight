/*********************************************************\
 *  File: SNMPositionPath.cpp                            *
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
#include <PLCore/Tools/Timing.h>
#include <PLMath/Matrix4x4.h>
#include <PLMath/Intersect.h>
#include <PLMath/Graph/GraphNode.h>
#include <PLMath/Graph/GraphPath.h>
#include <PLMath/Graph/GraphPathHandler.h>
#include <PLMath/Graph/GraphPathManager.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/SQCull.h"
#include "PLScene/Visibility/VisContainer.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMPositionPath.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPositionPath)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMPositionPath::GetFilename() const
{
	return m_sFilename;
}

void SNMPositionPath::SetFilename(const String &sValue)
{
	if (m_sFilename != sValue) {
		m_sFilename = sValue;

		{ // Load path
			SceneContext *pSceneContext =  GetSceneContext();
			if (pSceneContext)
				m_pPathHandler->SetResource(pSceneContext->GetGraphPathManager().LoadResource(m_sFilename));
		}

		// Is this modifier currently active?
		if (IsActive()) {
			// Set the node at the start position of the path
			const GraphPath *pPath = m_pPathHandler->GetResource();
			if (pPath) {
				if (GetFlags() & NodeIndexProgress)
					GetSceneNode().GetTransform().SetPosition(pPath->GetPosByNodeIndex(Progress, (Interpolation == Linear)));
				else
					GetSceneNode().GetTransform().SetPosition(pPath->GetPosByPercentageAlongPath(Progress, (Interpolation == Linear)));
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPositionPath::SNMPositionPath(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Filename(this),
	Progress(this),
	Speed(this),
	Interpolation(this),
	Flags(this),
	EventHandlerUpdate	 (&SNMPositionPath::OnUpdate,    this),
	EventHandlerDrawDebug(&SNMPositionPath::OnDrawDebug, this),
	m_pPathHandler(new GraphPathHandler())
{
}

/**
*  @brief
*    Destructor
*/
SNMPositionPath::~SNMPositionPath()
{
	// Destroy the path handler
	delete m_pPathHandler;
}

/**
*  @brief
*    Returns the used graph path
*/
GraphPath *SNMPositionPath::GetGraphPath() const
{
	return m_pPathHandler->GetResource();
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMPositionPath::OnActivate(bool bActivate)
{
	// Connect/disconnect event handlers
	SceneNode &cSceneNode = GetSceneNode();
	if (bActivate) {
		// Connect event handlers
		cSceneNode.SignalDrawDebug.Connect(EventHandlerDrawDebug);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);

		// Make a first update to ensure everything is up-to-date when we're going active (synchronization and logic update)
		OnUpdate();
	} else {
		// Disconnect event handlers
		cSceneNode.SignalDrawDebug.Disconnect(EventHandlerDrawDebug);
		SceneContext *pSceneContext = GetSceneContext();
		if (pSceneContext)
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private data                                          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMPositionPath::OnUpdate()
{
	// Move the node on the path
	const GraphPath *pPath = m_pPathHandler->GetResource();
	if (pPath) {
		Progress = Progress + Speed*Timing::GetInstance()->GetTimeDifference();
		if (GetFlags() & NodeIndexProgress)
			GetSceneNode().MoveTo(pPath->GetPosByNodeIndex(Progress, (Interpolation == Linear)));
		else
			GetSceneNode().MoveTo(pPath->GetPosByPercentageAlongPath(Progress, (Interpolation == Linear)));
	}
}

/**
*  @brief
*    Called on scene node debug draw
*/
void SNMPositionPath::OnDrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Get the path
	const GraphPath *pPath = m_pPathHandler->GetResource();
	if (pVisNode && pPath && pPath->GetNumOfNodes()) {
		// We need a view space transform
		const Matrix4x4 &mWorldViewProjection = pVisNode->GetViewProjectionMatrix();

		{ // Visualize the path
			// Draw nodes
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
			for (uint32 i=0; i<pPath->GetNumOfNodes(); i++)
				cRenderer.GetDrawHelpers().DrawPoint(Color4::Green, pPath->GetNode(i)->GetPos(), mWorldViewProjection, 3.0f);

			// Draw path lines
			for (uint32 i=0; i<pPath->GetNumOfNodes(); i++) {
				const GraphNode *pNode = pPath->GetNode(i);

				// Is this the last node?
				const GraphNode *pNodeT;
				if (i == pPath->GetNumOfNodes()-1) {
					if (!pPath->IsClosed())
						break; // No closing line, please
					pNodeT = pPath->GetNode(0);
				} else {
					pNodeT = pPath->GetNode(i+1);
				}

				// Draw line
				cRenderer.GetDrawHelpers().DrawLine(Color4::Blue, pNode->GetPos(), pNodeT->GetPos(), mWorldViewProjection, 1.0f);
			}
		}

		// Show node names
		Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
		if (pFont) {
			// Get cull query (if available) which enables us to cull 3D texts
			const SQCull *pCullQuery = nullptr;
			if (pVisNode) {
				const VisNode *pParentVisNode = pVisNode->GetParent();
				if (pParentVisNode && pParentVisNode->IsContainer()) {
					pCullQuery = static_cast<const VisContainer*>(pParentVisNode)->GetCullQuery();
				}
			}

			// Cull 3D text?
			if (pCullQuery) {
				// Make a visibility check to avoid text where no text should be...
				for (uint32 i=0; i<pPath->GetNumOfNodes(); i++) {
					const GraphNode *pNode = pPath->GetNode(i);
					if (Intersect::PlaneSetPoint(pCullQuery->GetViewFrustum(), pNode->GetPos()))
						cRenderer.GetDrawHelpers().DrawText(*pFont, pNode->GetName(), Color4::White, pNode->GetPos(), mWorldViewProjection, Font::CenterText);
				}
			} else {
				// Loop through all nodes
				for (uint32 i=0; i<pPath->GetNumOfNodes(); i++) {
					const GraphNode *pNode = pPath->GetNode(i);
					cRenderer.GetDrawHelpers().DrawText(*pFont, pNode->GetName(), Color4::White, pNode->GetPos(), mWorldViewProjection, Font::CenterText);
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
