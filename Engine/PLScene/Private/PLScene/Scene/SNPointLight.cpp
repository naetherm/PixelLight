/*********************************************************\
 *  File: SNPointLight.cpp                               *
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
#include <PLMath/Matrix4x4.h>
#include <PLMath/Rectangle.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SNPointLight.h"


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
pl_implement_class(SNPointLight)


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
const float SNPointLight::MinRange = 0.0001f;


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
float SNPointLight::GetRange() const
{
	return m_fRange;
}

void SNPointLight::SetRange(float fValue)
{
	// Clamp minimum range
	if (fValue < MinRange)
		fValue = MinRange;

	// Same value?
	if (m_fRange != fValue) {
		// Set the new range
		m_fRange = fValue;

		// We have to recalculate the box plane set
		m_nInternalLightFlags |= RecalculateBoxPlaneSet;

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNPointLight::SNPointLight() :
	Range(this),
	DebugFlags(this),
	m_nInternalLightFlags(RecalculateBoxPlaneSet),
	EventHandlerPosition(&SNPointLight::DirtyBoxPlaneSet, this),
	m_fRange(1.0f)
{
	// We have to recalculate the current axis align bounding box in 'scene node space'
	DirtyAABoundingBox();

	// Connect event handler
	GetTransform().EventPosition.Connect(EventHandlerPosition);
}

/**
*  @brief
*    Destructor
*/
SNPointLight::~SNPointLight()
{
}

/**
*  @brief
*    Returns the box plane set
*/
const PlaneSet &SNPointLight::GetBoxPlaneSet()
{
	// Calculate the box plane set if required
	if (m_nInternalLightFlags & RecalculateBoxPlaneSet) {
		// Update box planes
		AABoundingBox cBox(-m_fRange, -m_fRange, -m_fRange,
							m_fRange,  m_fRange,  m_fRange);
		cBox.vMin += GetTransform().GetPosition();
		cBox.vMax += GetTransform().GetPosition();
		m_cBoxPlaneSet.CreateBox(cBox.vMin, cBox.vMax);

		// Recalculation done
		m_nInternalLightFlags &= ~RecalculateBoxPlaneSet;
	}

	// Return the box plane set
	return m_cBoxPlaneSet;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Makes the box plane set dirty
*/
void SNPointLight::DirtyBoxPlaneSet()
{
	// We have to recalculate the box plane set
	m_nInternalLightFlags |= RecalculateBoxPlaneSet;
}


//[-------------------------------------------------------]
//[ Public virtual SNLight functions                      ]
//[-------------------------------------------------------]
bool SNPointLight::IsRenderLight() const
{
	return true;
}

bool SNPointLight::IsPointLight() const
{
	return true;
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNPointLight::UpdateAABoundingBox()
{
	// Set the final axis aligned bounding box
	SetAABoundingBox(AABoundingBox(-m_fRange, -m_fRange, -m_fRange,
									m_fRange,  m_fRange,  m_fRange));
}

void SNPointLight::GetBoundingSphere(Sphere &cSphere)
{
	// The sphere has always the 'range' as radius
	cSphere.SetRadius(m_fRange);

	// The sphere is always within the scene node origin
	cSphere.SetPos(GetTransform().GetPosition());
}

void SNPointLight::GetContainerBoundingSphere(Sphere &cSphere)
{
	// The sphere has always the 'range' as radius
	cSphere.SetRadius(m_fRange);

	// The sphere is always within the scene node origin
	cSphere.SetPos(GetTransform().GetPosition());
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNPointLight::DrawDebug(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Call base implementation
	SceneNode::DrawDebug(cRenderer, pVisNode);

	// Draw anything?
	if (!(GetDebugFlags() & DebugNoScissorRectangle) && pVisNode) {
		// Get the vertices of the bounding box
		Array<Vector3> vVertex(8);
		GetAABoundingBox().GetVertices(vVertex.GetData());

		// Get viewport parameters
		const uint32 nX      = static_cast<uint32>(cRenderer.GetViewport().GetX());
		const uint32 nY      = static_cast<uint32>(cRenderer.GetViewport().GetY());
		const uint32 nWidth  = static_cast<uint32>(cRenderer.GetViewport().GetWidth());
		const uint32 nHeight = static_cast<uint32>(cRenderer.GetViewport().GetHeight());

		// Calculate the scissor rectangle
		Rectangle cRectangle;
		cRectangle.ScreenRectangle(vVertex, true, pVisNode->GetWorldViewProjectionMatrix(), nX, nY, nWidth, nHeight);

		// Is the rectangle visible?
		if (cRectangle.GetWidth() || cRectangle.GetHeight()) {
			// Setup render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Begin 2D mode
			static const Color4 cColor(1.0f, 0.3f, 0.3f, 1.0f);
			DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
			cDrawHelpers.Begin2DMode(static_cast<float>(nX), static_cast<float>(nY), static_cast<float>(nWidth), static_cast<float>(nHeight));

				// Draw the rectangle
				cDrawHelpers.DrawLine(cColor, Vector2(cRectangle.vMin.x, cRectangle.vMin.y), Vector2(cRectangle.vMax.x, cRectangle.vMin.y), 2.0f);
				cDrawHelpers.DrawLine(cColor, Vector2(cRectangle.vMin.x, cRectangle.vMax.y), Vector2(cRectangle.vMax.x, cRectangle.vMax.y), 2.0f);
				cDrawHelpers.DrawLine(cColor, Vector2(cRectangle.vMin.x, cRectangle.vMin.y), Vector2(cRectangle.vMin.x, cRectangle.vMax.y), 2.0f);
				cDrawHelpers.DrawLine(cColor, Vector2(cRectangle.vMax.x, cRectangle.vMin.y), Vector2(cRectangle.vMax.x, cRectangle.vMax.y), 2.0f);

			// End 2D mode
			cDrawHelpers.End2DMode();
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
