/*********************************************************\
 *  File: SPTriangleShaders.cpp                          *
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
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/UniformBuffer.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include "SPTriangleShaders.h"


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
pl_class_metadata(SPTriangleShaders, "", SPTriangle, "Shaders based triangle surface painter")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLRenderer::Renderer&,	"Parameter constructor",	"")
pl_class_metadata_end(SPTriangleShaders)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPTriangleShaders::SPTriangleShaders(Renderer &cRenderer) : SPTriangle(cRenderer),
	m_pVertexShader(nullptr),
	m_pGeometryShader(nullptr),
	m_pFragmentShader(nullptr),
	m_pProgram(nullptr),
	m_pUniformBuffer(nullptr)
{
	// Decide which shader language should be used (for example "GLSL" or "Cg")
	ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(cRenderer.GetDefaultShaderLanguage());
	if (pShaderLanguage) {
		// Create the uniform buffer instance, null pointer if for instance not supported
		m_pUniformBuffer = pShaderLanguage->CreateUniformBuffer();

		// Create a geometry shader instance
		m_pGeometryShader = pShaderLanguage->CreateGeometryShader();

		// Shader source code
		String sVertexShaderSourceCode;
		String sGeometryShaderSourceCode;
		String sFragmentShaderSourceCode;
		if (pShaderLanguage->GetShaderLanguage() == "GLSL") {
			#include "SPTriangleShaders_GLSL.h"
			if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
				// Get shader source codes
				sVertexShaderSourceCode   = "#version 100\n" + sVertexShaderSourceCodeGLSL;
				// No geometry shaders possible with OpenGL ES 2.0
				sFragmentShaderSourceCode = "#version 100\n" + sFragmentShaderSourceCodeGLSL;
			} else {
				// Remove precision qualifiers so that we're able to use 110 (OpenGL 2.0 shaders) instead of 130 (OpenGL 3.0 shaders,
				// with this version we can keep the precision qualifiers) so that this shader requirements are as low as possible
				sVertexShaderSourceCode   = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(m_pUniformBuffer ? sVertexShaderSourceCodeGLSL_UniformBuffer : sVertexShaderSourceCodeGLSL);
				sGeometryShaderSourceCode = Shader::RemovePrecisionQualifiersFromGLSL(sGeometryShaderSourceCodeGLSL);
				sFragmentShaderSourceCode = ("#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(m_pGeometryShader ? sFragmentShaderSourceCodeGLSL_GS : sFragmentShaderSourceCodeGLSL));
			}
		} else if (pShaderLanguage->GetShaderLanguage() == "Cg") {
			#include "SPTriangleShaders_Cg.h"
			sVertexShaderSourceCode   = m_pUniformBuffer ? sVertexShaderSourceCodeCg_UniformBuffer : sVertexShaderSourceCodeCg;
			sGeometryShaderSourceCode = sGeometryShaderSourceCodeCg;
			sFragmentShaderSourceCode = sFragmentShaderSourceCodeCg;
		}

		// Create a vertex shader instance
		m_pVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode);

		// Set geometry shader source code
		if (m_pGeometryShader)
			m_pGeometryShader->SetSourceCode(sGeometryShaderSourceCode, GeometryShader::InputTriangles, GeometryShader::OutputTriangles, 6);

		// Create a fragment shader instance
		m_pFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

		// Create a program instance and assign the created vertex, geometry and fragment shaders to it
		m_pProgram = static_cast<ProgramWrapper*>(pShaderLanguage->CreateProgram(m_pVertexShader, m_pGeometryShader, m_pFragmentShader));
		if (m_pProgram) {
			// Setup the uniform buffer (if there's one)
			if (m_pUniformBuffer) {
				ProgramUniformBlock *pProgramUniformBlock = m_pProgram->GetUniformBlock("UniformBlock");
				if (pProgramUniformBlock) {
					// Allocate the uniform buffer
					m_pUniformBuffer->Allocate(pProgramUniformBlock->GetSize(), Usage::WriteOnly);
				}
			}
		}
	}
}

/**
*  @brief
*    Destructor
*/
SPTriangleShaders::~SPTriangleShaders()
{
	// Cleanup
	if (m_pUniformBuffer)
		delete m_pUniformBuffer;
	if (m_pProgram)
		delete m_pProgram;
	if (m_pFragmentShader)
		delete m_pFragmentShader;
	if (m_pGeometryShader)
		delete m_pGeometryShader;
	if (m_pVertexShader)
		delete m_pVertexShader;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::SurfacePainter functions  ]
//[-------------------------------------------------------]
void SPTriangleShaders::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Gray);

	// CPU side of the uniform buffer
	struct {
		Matrix4x4 mObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix
		Color4    cColor;							// Object color
	} cUniformBuffer;

	{ // Calculate the object space to clip space matrix
		// Calculate the world matrix
		Matrix4x4 mWorld;
		{
			// Build a rotation matrix by using a given Euler angle around the y-axis
			mWorld.FromEulerAngleY(static_cast<float>(m_fRotation*Math::DegToRad));

			// Increase the rotation by the current time difference (time past since the last frame)
			// -> Normally, such work should NOT be performed within the rendering step, but we want
			//    to keep the implementation simple in here...
			m_fRotation += Timing::GetInstance()->GetTimeDifference()*50;
		}

		// Calculate the view matrix
		Matrix4x4 mView;
		{
			mView.SetTranslation(0.0f, 0.0f, -5.0f);
		}

		// Calculate the projection matrix
		Matrix4x4 mProjection;
		{
			const float fAspect      = 1.0f;
			const float fAspectRadio = cRenderer.GetViewport().GetWidth()/(cRenderer.GetViewport().GetHeight()*fAspect);
			mProjection.PerspectiveFov(static_cast<float>(45.0f*Math::DegToRad), fAspectRadio, 0.001f, 1000.0f);
		}

		// Calculate the final composed world view projection matrix
		cUniformBuffer.mObjectSpaceToClipSpaceMatrix = mProjection*mView*mWorld;
	}

	// Set the object color
	cUniformBuffer.cColor.SetRGBA(0.2f, 1.0f, 1.0f, 1.0f);

	// Use uniform buffer to deliver the uniform block package at once to the GPU?
	// (please note that the data remains on the GPU side, so there's no need to
	//  upload this data to the GPU each time when there's no change within the data)
	if (m_pUniformBuffer) {
		// Copy over all the uniform data at once into the uniform buffer
		m_pUniformBuffer->CopyDataFrom(&cUniformBuffer);

		// Create a connection between "Uniform Block" and "Uniform Buffer" at binding point 0
		// -> When using Cg, this must be done while the program is not active, when using GLSL it doesn't matter
		cRenderer.SetProgram(nullptr);
		m_pProgram->Set("UniformBlock", m_pUniformBuffer, 0);
	}

	// Make our program to the current one
	if (cRenderer.SetProgram(m_pProgram)) {
		// Use uniform buffer?
		if (!m_pUniformBuffer) {
			// Nope, so set the program uniforms by using the good old way - one after another!
			// (resulting in more internal API calls and is therefore somewhat slower)
			m_pProgram->Set("ObjectSpaceToClipSpaceMatrix", cUniformBuffer.mObjectSpaceToClipSpaceMatrix);
			m_pProgram->Set("Color", cUniformBuffer.cColor);
		}

		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		m_pProgram->Set("VertexPosition", m_pVertexBuffer, VertexBuffer::Position);
		m_pProgram->Set("VertexColor", m_pVertexBuffer, VertexBuffer::Color);

		// No back face culling, please. Else we can only see one 'side' of the triangle
		cRenderer.SetRenderState(RenderState::CullMode, Cull::None);

		// Now draw the primitives of our cool triangle.
		// The primitive type is 'triangles', we start at vertex 0 and draw '3' vertices.
		cRenderer.DrawPrimitives(Primitive::TriangleList, 0, 3);
	}
}
