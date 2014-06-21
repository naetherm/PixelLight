/*********************************************************\
 *  File: SNPortal.cpp                                   *
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
#include <PLCore/String/Tokenizer.h>
#include <PLCore/Base/Func/Functor.h>
#include <PLMath/Intersect.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/SQCull.h"
#include "PLScene/Visibility/VisContainer.h"
#include "PLScene/Scene/SNPortal.h"


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
pl_implement_class(SNPortal)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNPortal::GetVertices() const
{
	return m_sVertices;
}

void SNPortal::SetVertices(const String &sValue)
{
	if (m_sVertices != sValue) {
		m_sVertices = sValue;
		Array<Vector3> &lstVertices = m_cPolygon.GetVertexList();

		// Cleanup
		lstVertices.Clear();

		// Check if vertices should be added to the polygon
		if (m_sVertices.GetLength()) {
			Tokenizer cTokenizer;
			Vector3 vPos;

			cTokenizer.Start(m_sVertices);
			while (cTokenizer.GetNextToken().GetLength()) {
				for (int i=0; i<3; i++) {
					vPos[i] = cTokenizer.GetToken().GetFloat();
					if (i < 2 && !cTokenizer.GetNextToken().GetLength())
						PL_LOG(Error, GetName() + String::Format(": Portal vertex %d is incomplete!", i+1))
				}
				lstVertices.Add(vPos);
			}
			cTokenizer.Stop();
		}

		// Update the polygon
		UpdatePolygon();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the portal polygon
*/
Polygon &SNPortal::GetPolygon()
{
	return m_cPolygon;
}

/**
*  @brief
*    Call this function after you manipulated the portal polygon
*/
void SNPortal::UpdatePolygon()
{
	const Array<Vector3> &lstVertices = m_cPolygon.GetVertexList();

	// Update the axis aligned bounding box of the scene node
	if (lstVertices.GetNumOfElements()) {
		// First vertex
		AABoundingBox cAABB(lstVertices[0]);

		// Loop through all polygon vertices
		for (uint32 i=1; i<lstVertices.GetNumOfElements(); i++) {
			const Vector3 &cVertex = lstVertices[i];

			// Min
			if (cAABB.vMin.x > cVertex.x)
				cAABB.vMin.x = cVertex.x;
			if (cAABB.vMin.y > cVertex.y)
				cAABB.vMin.y = cVertex.y;
			if (cAABB.vMin.z > cVertex.z)
				cAABB.vMin.z = cVertex.z;

			// Max
			if (cAABB.vMax.x < cVertex.x)
				cAABB.vMax.x = cVertex.x;
			if (cAABB.vMax.y < cVertex.y)
				cAABB.vMax.y = cVertex.y;
			if (cAABB.vMax.z < cVertex.z)
				cAABB.vMax.z = cVertex.z;
		}

		// Avoid zero dimensions... this 'may' cause problems in certain situations
		if (!cAABB.GetWidth()) {
			cAABB.vMin.x -= 0.001f;
			cAABB.vMax.x += 0.001f;
		}
		if (!cAABB.GetHeight()) {
			cAABB.vMin.y -= 0.001f;
			cAABB.vMax.y += 0.001f;
		}
		if (!cAABB.GetDepth()) {
			cAABB.vMin.z -= 0.001f;
			cAABB.vMax.z += 0.001f;
		}

		// Set axis aligned bounding box
		SetAABoundingBox(cAABB);
	} else {
		SetAABoundingBox(AABoundingBox());
	}

	// Calculate the plane
	m_cPolygon.ComputePlane();

	// We need to recalculate the container space portal polygon
	m_nInternalPortalFlags |= RecalculateContainerPolygon;
}

/**
*  @brief
*    Returns the portal polygon in container space
*/
const Polygon &SNPortal::GetContainerPolygon()
{
	// Calculate container polygon if required
	if (m_nInternalPortalFlags & RecalculateContainerPolygon) {
		Array<Vector3> &lstContainerVertices = m_cContainerPolygon.GetVertexList();
		const Array<Vector3> &lstVertices = m_cPolygon.GetVertexList();
		uint32 nNumOfVertices = lstVertices.GetNumOfElements();
		const Matrix3x4 &mTrans = GetTransform().GetMatrix();
		lstContainerVertices.Resize(lstVertices.GetNumOfElements());
		for (uint32 i=0; i<nNumOfVertices; i++) {
			lstContainerVertices[i]  = lstVertices[i];
			lstContainerVertices[i] *= mTrans;
		}

		// Recalculation done
		m_nInternalPortalFlags &= ~RecalculateContainerPolygon;
	}

	// Return the container polygon
	return m_cContainerPolygon;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNPortal::SNPortal() :
	Vertices(this),
	Flags(this),
	DebugFlags(this),
	m_nInternalPortalFlags(RecalculateContainerPolygon),
	EventHandlerContainerPositionRotationScale(&SNPortal::OnContainerPositionRotationScale, this)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|CastShadow|ReceiveShadow);

	// Set the internal flag
	m_nInternalFlags |= ClassPortal;

	// We initialized 'Vertices'
	SetVertices("-1.0 -1.0 0.0 -1.0 1.0 0.0 1.0 1.0 0.0 1.0 -1.0 0.0");

	// Connect event handlers
	SignalContainer.Connect(EventHandlerContainerPositionRotationScale);
	GetTransform().EventPosition.Connect(EventHandlerContainerPositionRotationScale);
	GetTransform().EventRotation.Connect(EventHandlerContainerPositionRotationScale);
	GetTransform().EventScale.   Connect(EventHandlerContainerPositionRotationScale);
}

/**
*  @brief
*    Destructor
*/
SNPortal::~SNPortal()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node container, position, rotation or scale changed
*/
void SNPortal::OnContainerPositionRotationScale()
{
	// We need to recalculate the container space portal polygon
	m_nInternalPortalFlags |= RecalculateContainerPolygon;
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNPortal::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawDebug(cRenderer, pVisNode);

	// Draw anything?
	if (pVisNode && (!(GetDebugFlags() & DebugNoPortalPolygon) || (GetDebugFlags() & DebugPortalVertices))) {
		Array<Vector3> &lstVertices = m_cPolygon.GetVertexList();
		if (lstVertices.GetNumOfElements() > 2) {
			// Get cull query (if available) which enables us to cull 3D debug texts
			const SQCull *pCullQuery = nullptr;
			if (pVisNode) {
				const VisNode *pParentVisNode = pVisNode->GetParent();
				if (pParentVisNode && pParentVisNode->IsContainer())
					pCullQuery = static_cast<const VisContainer*>(pParentVisNode)->GetCullQuery();
			}

			// Set render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
			cRenderer.SetRenderState(RenderState::BlendEnable,  true);

			// Draw polygon?
			if (!(GetDebugFlags() & DebugNoPortalPolygon)) {
				const Color4 cColor(1.0f, 0.3f, 0.3f, 0.5f);
				for (uint32 i=0; i<lstVertices.GetNumOfElements()-1; i+=2) {
					uint32 nID = i+2;
					if (nID >= lstVertices.GetNumOfElements())
						nID = 0;
					cRenderer.GetDrawHelpers().DrawTriangle(cColor, lstVertices[i], lstVertices[i+1], lstVertices[nID], pVisNode->GetWorldViewProjectionMatrix(), 0.0f);
				}
			}

			// Draw vertices?
			if (GetDebugFlags() & DebugPortalVertices) {
				Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
				if (pFont) {
					for (uint32 i=0; i<lstVertices.GetNumOfElements(); i++) {
						if (!pCullQuery || Intersect::PlaneSetPoint(pCullQuery->GetViewFrustum(), GetTransform().GetPosition()+lstVertices[i]))
							cRenderer.GetDrawHelpers().DrawText(*pFont, String::Format("%d", i), Color4::White, lstVertices[i], pVisNode->GetWorldViewProjectionMatrix(), Font::CenterText);
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
