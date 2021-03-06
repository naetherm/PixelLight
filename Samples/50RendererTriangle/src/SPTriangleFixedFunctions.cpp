/*********************************************************\
 *  File: SPTriangleFixedFunctions.cpp                   *
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
#include <PLMath/Rectangle.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include "SPTriangleFixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SPTriangleFixedFunctions, "", SPTriangle, "Fixed functions triangle surface painter")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLRenderer::Renderer&,	"Parameter constructor",	"")
pl_class_metadata_end(SPTriangleFixedFunctions)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPTriangleFixedFunctions::SPTriangleFixedFunctions(Renderer &cRenderer) : SPTriangle(cRenderer)
{
}

/**
*  @brief
*    Destructor
*/
SPTriangleFixedFunctions::~SPTriangleFixedFunctions()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::SurfacePainter functions  ]
//[-------------------------------------------------------]
void SPTriangleFixedFunctions::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Gray);

	// Fixed functions support required
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		{ // Set the world matrix
			// Build a rotation matrix by using a given Euler angle around the y-axis
			Matrix4x4 mWorld;
			mWorld.FromEulerAngleY(static_cast<float>(m_fRotation*Math::DegToRad));
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorld);

			// Increase the rotation by the current time difference (time past since the last frame)
			// -> Normally, such work should NOT be performed within the rendering step, but we want
			//    to keep the implementation simple in here...
			m_fRotation += Timing::GetInstance()->GetTimeDifference()*50;
		}

		{ // Set the view matrix
			Matrix4x4 mView;
			mView.SetTranslation(0.0f, 0.0f, -5.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, mView);
		}

		{ // Set the projection matrix
			const float fAspect      = 1.0f;
			const float fAspectRadio = cRenderer.GetViewport().GetWidth()/(cRenderer.GetViewport().GetHeight()*fAspect);
			Matrix4x4 mProj;
			mProj.PerspectiveFov(static_cast<float>(45.0f*Math::DegToRad), fAspectRadio, 0.001f, 1000.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProj);
		}

		// Normally this vertex buffer we created is NEVER a null pointer, but in this sample
		// we want to go for sure
		if (m_pVertexBuffer) {
			// Bind our vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pVertexBuffer);

			// No back face culling, please. Else we can only see one 'side' of the triangle
			cRenderer.SetRenderState(RenderState::CullMode, Cull::None);

			// Now draw the primitives of our cool triangle.
			// The primitive type is 'triangles', we start at vertex 0 and draw '3' vertices.
			cRenderer.DrawPrimitives(Primitive::TriangleList, 0, 3);
		}
	}
}
