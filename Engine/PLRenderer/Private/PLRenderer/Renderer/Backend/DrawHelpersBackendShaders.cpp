/*********************************************************\
 *  File: DrawHelpersBackendShaders.cpp                  *
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
#include <PLCore/Tools/Tools.h>
#include <PLMath/Rectangle.h>
#include "PLRenderer/Renderer/Program.h"
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/VertexBuffer.h"
#include "PLRenderer/Renderer/VertexShader.h"
#include "PLRenderer/Renderer/SamplerStates.h"
#include "PLRenderer/Renderer/FragmentShader.h"
#include "PLRenderer/Renderer/ProgramUniform.h"
#include "PLRenderer/Renderer/FixedFunctions.h"
#include "PLRenderer/Renderer/ProgramAttribute.h"
#include "PLRenderer/Renderer/TextureBuffer2D.h"
#include "PLRenderer/Renderer/TextureBufferRectangle.h"
#include "PLRenderer/Renderer/Backend/DrawHelpersBackendShaders.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DrawHelpersBackendShaders::DrawHelpersBackendShaders(Renderer &cRenderer) : DrawHelpersBackend(cRenderer),
	m_pProgramGenerator(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
DrawHelpersBackendShaders::~DrawHelpersBackendShaders()
{
	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Public virtual DrawHelpers functions                  ]
//[-------------------------------------------------------]
void DrawHelpersBackendShaders::Begin2DMode(float fX1, float fY1, float fX2, float fY2)
{
	// Is the 2D mode already set?
	if (m_b2DMode)
		End2DMode();

	// Set to viewport values?
	if (!fX1 && !fY1 && !fX2 && !fY2) {
		const Rectangle &cViewportRect = m_pRenderer->GetViewport();
		fX1 = cViewportRect.vMin.x;
		fY1 = cViewportRect.vMin.y;
		fX2 = cViewportRect.vMax.x;
		fY2 = cViewportRect.vMax.y;
	}

	// Backup settings
	m_fVirtualScreen[0] = fX1;
	m_fVirtualScreen[1] = fY1;
	m_fVirtualScreen[2] = fX2;
	m_fVirtualScreen[3] = fY2;

	// Set orthogonal projection matrix
	m_mObjectSpaceToClipSpace.OrthoOffCenter(fX1, fX2, fY1, fY2, -1.0f, 1.0f);

	// Fixed functions - just so fixed function stuff using Begin2DMode() & End2DMode() to setup the projection matrix still works
	FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
	if (pFixedFunctions) {
		// Backup projection and view matrix
		m_m2DModeProjBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
		m_m2DModeViewBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);

		// Set projection and view matrix
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, m_mObjectSpaceToClipSpace);
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, Matrix4x4::Identity);
	}

	// Set 2D mode status
	m_b2DMode = true;
}

void DrawHelpersBackendShaders::End2DMode()
{
	// Is the 2D mode set?
	if (m_b2DMode) {
		// Fixed functions - just so fixed function stuff using Begin2DMode() & End2DMode() to setup the projection matrix still works
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Reset projection and view matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, m_m2DModeProjBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,		  m_m2DModeViewBackup);
		}

		// Set 2D mode status
		m_b2DMode = false;
	}
}

void DrawHelpersBackendShaders::DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const Vector2 &vPos, const Vector2 &vSize, const Color4 &cColor,
										  float fAlphaReference, const Vector2 &vTextureCoordinate, const Vector2 &vTextureCoordinateSize, const Matrix4x4 &mTexture)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Get the image size
		Vector2 vImageSize = vSize;
		if (vImageSize.IsNull()) {
			switch (cTextureBuffer.GetType()) {
				case Resource::TypeTextureBuffer2D:
					vImageSize.x = static_cast<float>(static_cast<TextureBuffer2D&>(cTextureBuffer).GetSize().x);
					vImageSize.y = static_cast<float>(static_cast<TextureBuffer2D&>(cTextureBuffer).GetSize().y);
					break;

				case Resource::TypeTextureBufferRectangle:
					vImageSize.x = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().x);
					vImageSize.y = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().y);
					break;

				case Resource::TypeIndexBuffer:
				case Resource::TypeVertexBuffer:
				case Resource::TypeUniformBuffer:
				case Resource::TypeTextureBuffer1D:
				case Resource::TypeTextureBuffer2DArray:
				case Resource::TypeTextureBuffer3D:
				case Resource::TypeTextureBufferCube:
				case Resource::TypeOcclusionQuery:
				case Resource::TypeVertexShader:
				case Resource::TypeTessellationControlShader:
				case Resource::TypeTessellationEvaluationShader:
				case Resource::TypeGeometryShader:
				case Resource::TypeFragmentShader:
				case Resource::TypeProgram:
				case Resource::TypeFont:
				default:
					return;	// Error, must be 2D or rectangle!
			}
		}

		// Get texture scale - rectangle textures use non-normalized texture coordinates
		float fTextureCoordinateScaleX = 1.0f;
		float fTextureCoordinateScaleY = 1.0f;
		if (cTextureBuffer.GetType() == Resource::TypeTextureBufferRectangle) {
			fTextureCoordinateScaleX = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().x);
			fTextureCoordinateScaleY = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().y);
		}

		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y + vImageSize.y;
			pfVertex[2] = m_fZValue2D;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::TexCoord));
			pfVertex[0] = vTextureCoordinate.x*fTextureCoordinateScaleX;
			pfVertex[1] = (vTextureCoordinate.y + vTextureCoordinateSize.y)*fTextureCoordinateScaleY;

			// Vertex 1
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[0] = vPos.x + vImageSize.x;
			pfVertex[1] = vPos.y + vImageSize.y;
			pfVertex[2] = m_fZValue2D;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::TexCoord));
			pfVertex[0] = (vTextureCoordinate.x + vTextureCoordinateSize.x)*fTextureCoordinateScaleX;
			pfVertex[1] = (vTextureCoordinate.y + vTextureCoordinateSize.y)*fTextureCoordinateScaleY;

			// Vertex 2
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::Position));
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = m_fZValue2D;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::TexCoord));
			pfVertex[0] = vTextureCoordinate.x*fTextureCoordinateScaleX;
			pfVertex[1] = vTextureCoordinate.y*fTextureCoordinateScaleY;

			// Vertex 3
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::Position));
			pfVertex[0] = vPos.x + vImageSize.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = m_fZValue2D;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::TexCoord));
			pfVertex[0] = (vTextureCoordinate.x + vTextureCoordinateSize.x)*fTextureCoordinateScaleX;
			pfVertex[1] = vTextureCoordinate.y*fTextureCoordinateScaleY;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the image GPU program
		if (UseImageProgram(*m_pTempVertexBuffer, cColor, m_mObjectSpaceToClipSpace, cTextureBuffer, cSamplerStates, fAlphaReference, mTexture)) {
			// Draw image
			m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}

void DrawHelpersBackendShaders::DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const Vector3 &vPos, const Matrix4x4 &mObjectSpaceToClipSpace, const Vector2 &vSize,
										  const Color4 &cColor, float fAlphaReference, const Vector2 &vTextureCoordinate, const Vector2 &vTextureCoordinateSize, const Matrix4x4 &mTexture)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Get the image size
		Vector2 vImageSize = vSize;
		if (vImageSize.IsNull()) {
			switch (cTextureBuffer.GetType()) {
				case Resource::TypeTextureBuffer2D:
					vImageSize.x = static_cast<float>(static_cast<TextureBuffer2D&>(cTextureBuffer).GetSize().x);
					vImageSize.y = static_cast<float>(static_cast<TextureBuffer2D&>(cTextureBuffer).GetSize().y);
					break;

				case Resource::TypeTextureBufferRectangle:
					vImageSize.x = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().x);
					vImageSize.y = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().y);
					break;

				case Resource::TypeIndexBuffer:
				case Resource::TypeVertexBuffer:
				case Resource::TypeUniformBuffer:
				case Resource::TypeTextureBuffer1D:
				case Resource::TypeTextureBuffer2DArray:
				case Resource::TypeTextureBuffer3D:
				case Resource::TypeTextureBufferCube:
				case Resource::TypeOcclusionQuery:
				case Resource::TypeVertexShader:
				case Resource::TypeTessellationControlShader:
				case Resource::TypeTessellationEvaluationShader:
				case Resource::TypeGeometryShader:
				case Resource::TypeFragmentShader:
				case Resource::TypeProgram:
				case Resource::TypeFont:
				default:
					return;	// Error, must be 2D or rectangle!
			}
		}

		// Get texture scale - rectangle textures use non-normalized texture coordinates
		float fTextureCoordinateScaleX = 1.0f;
		float fTextureCoordinateScaleY = 1.0f;
		if (cTextureBuffer.GetType() == Resource::TypeTextureBufferRectangle) {
			fTextureCoordinateScaleX = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().x);
			fTextureCoordinateScaleY = static_cast<float>(static_cast<TextureBufferRectangle&>(cTextureBuffer).GetSize().y);
		}

		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vPos.x + vImageSize.x;
			pfVertex[1] = vPos.y + vImageSize.y;
			pfVertex[2] = vPos.z;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::TexCoord));
			pfVertex[0] = (vTextureCoordinate.x + vTextureCoordinateSize.x)*fTextureCoordinateScaleX;
			pfVertex[1] = vTextureCoordinate.y*fTextureCoordinateScaleY;

			// Vertex 1
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y + vImageSize.y;
			pfVertex[2] = vPos.z;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::TexCoord));
			pfVertex[0] = vTextureCoordinate.x*fTextureCoordinateScaleX;
			pfVertex[1] = vTextureCoordinate.y*fTextureCoordinateScaleY;

			// Vertex 2
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::Position));
			pfVertex[0] = vPos.x + vImageSize.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = vPos.z;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::TexCoord));
			pfVertex[0] = (vTextureCoordinate.x + vTextureCoordinateSize.x)*fTextureCoordinateScaleX;
			pfVertex[1] = (vTextureCoordinate.y + vTextureCoordinateSize.y)*fTextureCoordinateScaleY;

			// Vertex 3
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::Position));
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = vPos.z;
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::TexCoord));
			pfVertex[0] = vTextureCoordinate.x*fTextureCoordinateScaleX;
			pfVertex[1] = (vTextureCoordinate.y + vTextureCoordinateSize.y)*fTextureCoordinateScaleY;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the image GPU program
		if (UseImageProgram(*m_pTempVertexBuffer, cColor, mObjectSpaceToClipSpace, cTextureBuffer, cSamplerStates, fAlphaReference, mTexture)) {
			// Draw image
			m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}

void DrawHelpersBackendShaders::DrawPoint(const Color4 &cColor, const Vector2 &vPosition, float fSize)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vPosition.x;
			pfVertex[1] = vPosition.y;
			pfVertex[2] = m_fZValue2D;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the primitive GPU program
		if (UsePrimitiveProgram(*m_pTempVertexBuffer, cColor, m_mObjectSpaceToClipSpace, fSize)) {
			// Set the point size - used for Cg and OpenGL GLSL, for OpenGL ES 2.0, gl_PointSize is used within the vertex shader
			m_pRenderer->SetRenderState(RenderState::PointSize, Tools::FloatToUInt32(fSize));

			// Draw point
			m_pRenderer->DrawPrimitives(Primitive::PointList, 0, 1);
		}
	}
}

void DrawHelpersBackendShaders::DrawPoint(const Color4 &cColor, const Vector3 &vPosition, const Matrix4x4 &mObjectSpaceToClipSpace, float fSize)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vPosition.x;
			pfVertex[1] = vPosition.y;
			pfVertex[2] = vPosition.z;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the primitive GPU program
		if (UsePrimitiveProgram(*m_pTempVertexBuffer, cColor, mObjectSpaceToClipSpace, fSize)) {
			// Set the point size - used for Cg and OpenGL GLSL, for OpenGL ES 2.0, gl_PointSize is used within the vertex shader
			m_pRenderer->SetRenderState(RenderState::PointSize, Tools::FloatToUInt32(fSize));

			// Draw point
			m_pRenderer->DrawPrimitives(Primitive::PointList, 0, 1);
		}
	}
}

void DrawHelpersBackendShaders::DrawLine(const Color4 &cColor, const Vector2 &vStartPosition, const Vector2 &vEndPosition, float fWidth)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vStartPosition.x;
			pfVertex[1] = vStartPosition.y;
			pfVertex[2] = m_fZValue2D;

			// Vertex 1
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[0] = vEndPosition.x;
			pfVertex[1] = vEndPosition.y;
			pfVertex[2] = m_fZValue2D;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the primitive GPU program
		if (UsePrimitiveProgram(*m_pTempVertexBuffer, cColor, m_mObjectSpaceToClipSpace)) {
			// Set the line width
			m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

			// Draw line
			m_pRenderer->DrawPrimitives(Primitive::LineList, 0, 2);
		}
	}
}

void DrawHelpersBackendShaders::DrawLine(const Color4 &cColor, const Vector3 &vStartPosition, const Vector3 &vEndPosition, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vStartPosition.x;
			pfVertex[1] = vStartPosition.y;
			pfVertex[2] = vStartPosition.z;

			// Vertex 1
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[0] = vEndPosition.x;
			pfVertex[1] = vEndPosition.y;
			pfVertex[2] = vEndPosition.z;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the primitive GPU program
		if (UsePrimitiveProgram(*m_pTempVertexBuffer, cColor, mObjectSpaceToClipSpace)) {
			// Set the line width
			m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

			// Draw line
			m_pRenderer->DrawPrimitives(Primitive::LineList, 0, 2);
		}
	}
}

void DrawHelpersBackendShaders::DrawTriangle(const Color4 &cColor, const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vV1.x;
			pfVertex[1] = vV1.y;
			pfVertex[2] = vV1.z;

			// Vertex 1
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[0] = vV2.x;
			pfVertex[1] = vV2.y;
			pfVertex[2] = vV2.z;

			// Vertex 2
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::Position));
			pfVertex[0] = vV3.x;
			pfVertex[1] = vV3.y;
			pfVertex[2] = vV3.z;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the primitive GPU program
		if (UsePrimitiveProgram(*m_pTempVertexBuffer, cColor, mObjectSpaceToClipSpace)) {
			// Set the line width
			if (fWidth)
				m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

			// Draw triangle
			m_pRenderer->DrawPrimitives(fWidth ? Primitive::LineList : Primitive::TriangleList, 0, 3);
		}
	}
}

void DrawHelpersBackendShaders::DrawQuad(const Color4 &cColor, const Vector2 &vPos, const Vector2 &vSize, float fWidth)
{
	// Draw lines?
	if (fWidth) {
		// Set the line width
		m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

		// Draw single lines
		const Vector3 &vV1 = vPos;
		const Vector3 vV2(vPos.x + vSize.x, vPos.y);
		const Vector3 vV3(vPos.x + vSize.x, vPos.y + vSize.y);
		const Vector3 vV4(vPos.x, vPos.y + vSize.y);
		DrawLine(cColor, vV1, vV2, fWidth);
		DrawLine(cColor, vV2, vV3, fWidth);
		DrawLine(cColor, vV3, vV4, fWidth);
		DrawLine(cColor, vV4, vV1, fWidth);
	} else {
		// Create vertex buffer
		if (CreateTempBuffes()) {
			// Setup the vertex buffer
			if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
				// Vertex 0
				float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
				pfVertex[0] = vPos.x;
				pfVertex[1] = vPos.y + vSize.y;
				pfVertex[2] = m_fZValue2D;

				// Vertex 1
				pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
				pfVertex[0] = vPos.x + vSize.x;
				pfVertex[1] = vPos.y + vSize.y;
				pfVertex[2] = m_fZValue2D;

				// Vertex 2
				pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::Position));
				pfVertex[0] = vPos.x;
				pfVertex[1] = vPos.y;
				pfVertex[2] = m_fZValue2D;

				// Vertex 3
				pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::Position));
				pfVertex[0] = vPos.x + vSize.x;
				pfVertex[1] = vPos.y;
				pfVertex[2] = m_fZValue2D;

				// Unlock the vertex buffer
				m_pTempVertexBuffer->Unlock();
			}

			// Use the primitive GPU program
			if (UsePrimitiveProgram(*m_pTempVertexBuffer, cColor, m_mObjectSpaceToClipSpace)) {
				// Draw quad
				m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);
			}
		}
	}
}

void DrawHelpersBackendShaders::DrawQuad(const Color4 &cColor, const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3, const Vector3 &vV4, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	// Draw lines?
	if (fWidth) {
		// Set the line width
		m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

		// Draw single lines
		DrawLine(cColor, vV1, vV2, fWidth);
		DrawLine(cColor, vV2, vV3, fWidth);
		DrawLine(cColor, vV3, vV4, fWidth);
		DrawLine(cColor, vV4, vV1, fWidth);
	} else {
		// Create vertex buffer
		if (CreateTempBuffes()) {
			// Setup the vertex buffer
			if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
				// Vertex 0
				float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
				pfVertex[0] = vV1.x;
				pfVertex[1] = vV1.y;
				pfVertex[2] = vV1.z;

				// Vertex 1
				pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
				pfVertex[0] = vV2.x;
				pfVertex[1] = vV2.y;
				pfVertex[2] = vV2.z;

				// Vertex 2
				pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::Position));
				pfVertex[0] = vV3.x;
				pfVertex[1] = vV3.y;
				pfVertex[2] = vV3.z;

				// Vertex 3
				pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::Position));
				pfVertex[0] = vV4.x;
				pfVertex[1] = vV4.y;
				pfVertex[2] = vV4.z;

				// Unlock the vertex buffer
				m_pTempVertexBuffer->Unlock();
			}

			// Use the primitive GPU program
			if (UsePrimitiveProgram(*m_pTempVertexBuffer, cColor, mObjectSpaceToClipSpace)) {
				// Draw quad
				m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);
			}
		}
	}
}

void DrawHelpersBackendShaders::DrawGradientQuad(const Color4 &cColor1, const Color4 &cColor2, float fAngle, const Vector2 &vPos, const Vector2 &vSize)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// For color calculation
			Color4 cColor;
			const float fSin     = Math::Sin(fAngle);
			const float fCos     = Math::Cos(fAngle);
			const float fScale   = 1.0f/(Math::Abs(fSin)+Math::Abs(fCos));

			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y + vSize.y;
			pfVertex[2] = m_fZValue2D;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor1* fCos;
				if (fCos < 0.0f)
					cColor += cColor2*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor2* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor1*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(0, cColor);

			// Vertex 1
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[0] = vPos.x + vSize.x;
			pfVertex[1] = vPos.y + vSize.y;
			pfVertex[2] = m_fZValue2D;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor2* fCos;
				if (fCos < 0.0f)
					cColor += cColor1*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor2* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor1*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(1, cColor);

			// Vertex 2
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::Position));
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = m_fZValue2D;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor1* fCos;
				if (fCos < 0.0f)
					cColor += cColor2*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor1* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor2*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(2, cColor);

			// Vertex 3
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::Position));
			pfVertex[0] = vPos.x + vSize.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = m_fZValue2D;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor2* fCos;
				if (fCos < 0.0f)
					cColor += cColor1*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor1* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor2*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(3, cColor);

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the primitive GPU program
		if (UsePrimitiveProgram(*m_pTempVertexBuffer, Color4::Null, m_mObjectSpaceToClipSpace)) {
			// Draw quad
			m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}

void DrawHelpersBackendShaders::DrawGradientQuad(const Color4 &cColor1, const Color4 &cColor2, float fAngle, const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3, const Vector3 &vV4, const Matrix4x4 &mObjectSpaceToClipSpace)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// For color calculation
			Color4 cColor;
			const float fSin     = Math::Sin(fAngle);
			const float fCos     = Math::Cos(fAngle);
			const float fScale   = 1.0f/(Math::Abs(fSin)+Math::Abs(fCos));

			// Vertex 0
			float *pfVertex = static_cast<float*>(m_pTempVertexBuffer->GetData(0, VertexBuffer::Position));
			pfVertex[0] = vV1.x;
			pfVertex[1] = vV1.y;
			pfVertex[2] = vV1.z;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor1* fCos;
				if (fCos < 0.0f)
					cColor += cColor2*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor2* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor1*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(0, cColor);

			// Vertex 1
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(1, VertexBuffer::Position));
			pfVertex[0] = vV2.x;
			pfVertex[1] = vV2.y;
			pfVertex[2] = vV2.z;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor2* fCos;
				if (fCos < 0.0f)
					cColor += cColor1*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor2* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor1*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(1, cColor);

			// Vertex 2
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(2, VertexBuffer::Position));
			pfVertex[0] = vV3.x;
			pfVertex[1] = vV3.y;
			pfVertex[2] = vV3.z;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor1* fCos;
				if (fCos < 0.0f)
					cColor += cColor2*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor1* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor2*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(2, cColor);

			// Vertex 3
			pfVertex	= static_cast<float*>(m_pTempVertexBuffer->GetData(3, VertexBuffer::Position));
			pfVertex[0] = vV4.x;
			pfVertex[1] = vV4.y;
			pfVertex[2] = vV4.z;
			{ // Calculate color
				cColor = 0.0f;
				// Horizontal color influence
				if (fCos > 0.0f)
					cColor += cColor2* fCos;
				if (fCos < 0.0f)
					cColor += cColor1*-fCos;
				// Vertical color influence
				if (fSin > 0.0f)
					cColor = (cColor1* fSin + cColor)*fScale;
				if (fSin < 0.0f)
					cColor = (cColor2*-fSin + cColor)*fScale;
				cColor.Saturate();
			}
			m_pTempVertexBuffer->SetColor(3, cColor);

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Use the primitive GPU program
		if (UsePrimitiveProgram(*m_pTempVertexBuffer, Color4::Null, mObjectSpaceToClipSpace)) {
			// Draw quad
			m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the program generator
*/
ProgramGenerator *DrawHelpersBackendShaders::GetProgramGenerator()
{
	// Get the shader language to use
	const String sShaderLanguage = m_pRenderer->GetDefaultShaderLanguage();

	// Create the program generator if there's currently no instance of it
	if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
		// If there's an previous instance of the program generator, destroy it first
		if (m_pProgramGenerator) {
			delete m_pProgramGenerator;
			m_pProgramGenerator = nullptr;
		}

		// Choose the shader source codes depending on the requested shader language
		if (sShaderLanguage == "GLSL") {
			#include "DrawHelpersBackendShaders_GLSL.h"
			if (m_pRenderer->GetAPI() == "OpenGL ES 2.0") {
				// Create the program generator
				m_pProgramGenerator = new ProgramGenerator(*m_pRenderer, sShaderLanguage, sVertexShaderSourceCodeGLSL, "100", sFragmentShaderSourceCodeGLSL, "100");
			} else {
				// Add GLSL extension definition
				const String sFragmentShaderSourceCodeGLSLModified = "#extension GL_ARB_texture_rectangle : enable\n" + sFragmentShaderSourceCodeGLSL;

				// Remove precision qualifiers so that we're able to use 110 (OpenGL 2.0 shaders) instead of 130 (OpenGL 3.0 shaders,
				// with this version we can keep the precision qualifiers) so that this shader requirements are as low as possible
				m_pProgramGenerator = new ProgramGenerator(*m_pRenderer, sShaderLanguage, Shader::RemovePrecisionQualifiersFromGLSL(sVertexShaderSourceCodeGLSL), "110", Shader::RemovePrecisionQualifiersFromGLSL(sFragmentShaderSourceCodeGLSLModified), "110");
			}
		} else if (sShaderLanguage == "Cg") {
			#include "DrawHelpersBackendShaders_Cg.h"
			m_pProgramGenerator = new ProgramGenerator(*m_pRenderer, sShaderLanguage, sVertexShaderSourceCodeCg, "arbvp1", sFragmentShaderSourceCodeCg, "arbfp1");
		}
	}

	// Return the program generator
	return m_pProgramGenerator;
}

/**
*  @brief
*    Returns the generated program using the current flags
*/
ProgramGenerator::GeneratedProgram *DrawHelpersBackendShaders::GetAndSetGeneratedProgram()
{
	// Get the program generator
	ProgramGenerator *pProgramGenerator = GetProgramGenerator();
	if (pProgramGenerator) {
		// Get a program instance from the program generator using the given program flags
		ProgramGenerator::GeneratedProgram *pGeneratedProgram = pProgramGenerator->GetProgram(m_cProgramFlags);

		// Make our program to the current one
		if (pGeneratedProgram && m_pRenderer->SetProgram(pGeneratedProgram->pProgram)) {
			// Set pointers to uniforms & attributes of a generated program if they are not set yet
			GeneratedProgramUserData *pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);
			if (!pGeneratedProgramUserData) {
				pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
				Program *pProgram = pGeneratedProgram->pProgram;

				// Vertex shader attributes
				static const String sVertexPosition = "VertexPosition";
				pGeneratedProgramUserData->pVertexPosition					= pProgram->GetAttribute(sVertexPosition);
				static const String sVertexTextureCoordinate = "VertexTextureCoordinate";
				pGeneratedProgramUserData->pVertexTextureCoordinate			= pProgram->GetAttribute(sVertexTextureCoordinate);
				static const String sVertexColor = "VertexColor";
				pGeneratedProgramUserData->pVertexColor						= pProgram->GetAttribute(sVertexColor);
				// Vertex shader uniforms
				static const String sObjectSpaceToClipSpaceMatrix = "ObjectSpaceToClipSpaceMatrix";
				pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix	= pProgram->GetUniform(sObjectSpaceToClipSpaceMatrix);
				static const String sTextureMatrix = "TextureMatrix";
				pGeneratedProgramUserData->pTextureMatrix					= pProgram->GetUniform(sTextureMatrix);
				static const String sPointSize = "PointSize";
				pGeneratedProgramUserData->pPointSize						= pProgram->GetUniform(sPointSize);
				// Fragment shader uniforms
				static const String sColor = "Color";
				pGeneratedProgramUserData->pColor							= pProgram->GetUniform(sColor);
				static const String sDiffuseMap = "DiffuseMap";
				pGeneratedProgramUserData->pDiffuseMap						= pProgram->GetUniform(sDiffuseMap);
				static const String sAlphaReference = "AlphaReference";
				pGeneratedProgramUserData->pAlphaReference					= pProgram->GetUniform(sAlphaReference);
			}

			// Return the generated program
			return pGeneratedProgram;
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Uses the image GPU program
*/
bool DrawHelpersBackendShaders::UseImageProgram(VertexBuffer &cVertexBuffer, const Color4 &cColor, const Matrix4x4 &mObjectSpaceToClipSpace, TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, float fAlphaReference, const Matrix4x4 &mTexture)
{
	// Set the program flags
	m_cProgramFlags.Reset();
	PL_ADD_VS_FLAG(m_cProgramFlags, VS_TEXCOORD0)
	PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIFFUSEMAP)
	if (cTextureBuffer.GetType() == Resource::TypeTextureBuffer2D)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_DIFFUSEMAP2D)
	if (fAlphaReference < 1.0f)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALPHATEST)

	// Get the generated program using the current flags
	ProgramGenerator::GeneratedProgram *pGeneratedProgram = GetAndSetGeneratedProgram();
	if (pGeneratedProgram && pGeneratedProgram->pUserData) {
		GeneratedProgramUserData *pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);

		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (pGeneratedProgramUserData->pVertexPosition)
			pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);
		if (pGeneratedProgramUserData->pVertexTextureCoordinate)
			pGeneratedProgramUserData->pVertexTextureCoordinate->Set(&cVertexBuffer, PLRenderer::VertexBuffer::TexCoord);

		// Set program uniforms
		if (pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix)
			pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix->Set(mObjectSpaceToClipSpace);
		if (pGeneratedProgramUserData->pTextureMatrix)
			pGeneratedProgramUserData->pTextureMatrix->Set(mTexture);
		if (pGeneratedProgramUserData->pColor)
			pGeneratedProgramUserData->pColor->Set(cColor);
		if (pGeneratedProgramUserData->pDiffuseMap) {
			const int nTextureUnit = pGeneratedProgramUserData->pDiffuseMap->Set(&cTextureBuffer);
			if (nTextureUnit >= 0) {
				// Set sampler states
				for (uint32 nState=0; nState<Sampler::Number; nState++)
					m_pRenderer->SetSamplerState(nTextureUnit, static_cast<Sampler::Enum>(nState), cSamplerStates.Get(static_cast<Sampler::Enum>(nState)));
			}
			if (pGeneratedProgramUserData->pAlphaReference)
				pGeneratedProgramUserData->pAlphaReference->Set(fAlphaReference);
		}

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Uses the primitive GPU program
*/
bool DrawHelpersBackendShaders::UsePrimitiveProgram(VertexBuffer &cVertexBuffer, const Color4 &cColor, const Matrix4x4 &mObjectSpaceToClipSpace, float fPointSize)
{
	// Set the program flags
	m_cProgramFlags.Reset();
	if (cColor == Color4::Null) {
		// Use vertex color
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_VERTEXCOLOR)
		PL_ADD_FS_FLAG(m_cProgramFlags, FS_VERTEXCOLOR)
	}
	if (fPointSize)
		PL_ADD_VS_FLAG(m_cProgramFlags, VS_POINTSIZE)

	// Get the generated program using the current flags
	ProgramGenerator::GeneratedProgram *pGeneratedProgram = GetAndSetGeneratedProgram();
	if (pGeneratedProgram && pGeneratedProgram->pUserData) {
		GeneratedProgramUserData *pGeneratedProgramUserData = static_cast<GeneratedProgramUserData*>(pGeneratedProgram->pUserData);

		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		if (pGeneratedProgramUserData->pVertexPosition)
			pGeneratedProgramUserData->pVertexPosition->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Position);
		if (pGeneratedProgramUserData->pVertexColor)
			pGeneratedProgramUserData->pVertexColor->Set(&cVertexBuffer, PLRenderer::VertexBuffer::Color);

		// Set program uniforms
		if (pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix)
			pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix->Set(mObjectSpaceToClipSpace);
		if (pGeneratedProgramUserData->pColor)
			pGeneratedProgramUserData->pColor->Set(cColor);
		if (pGeneratedProgramUserData->pPointSize)
			pGeneratedProgramUserData->pPointSize->Set(fPointSize);

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
