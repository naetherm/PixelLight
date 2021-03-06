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
	m_pFragmentShader(nullptr),
	m_pProgram(nullptr)
{
	// Decide which shader language should be used (for example "GLSL" or "Cg")
	ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(cRenderer.GetDefaultShaderLanguage());
	if (pShaderLanguage) {
		// Shader source code
		String sVertexShaderSourceCode;
		String sGeometryShaderSourceCode;
		String sFragmentShaderSourceCode;
		if (pShaderLanguage->GetShaderLanguage() == "GLSL") {
			#include "SPTriangleShaders_GLSL.h"
			if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
				// Get shader source codes
				// -> OpenGL ES 2.0 has no support for geometric primitive instancing
				sVertexShaderSourceCode   = "#version 100\n" + sVertexShaderSourceCodeGLSL;
				sFragmentShaderSourceCode = "#version 100\n" + sFragmentShaderSourceCodeGLSL;
			} else {
				// Remove precision qualifiers so that we're able to use 110 (OpenGL 2.0 shaders) instead of 130 (OpenGL 3.0 shaders,
				// with this version we can keep the precision qualifiers) so that this shader requirements are as low as possible

				// Is geometric primitive instancing supported?
				if (cRenderer.GetCapabilities().bInstancing)
					sVertexShaderSourceCode = "#version 110\n#extension GL_EXT_gpu_shader4 : enable\n#extension GL_ARB_draw_instanced : enable\n" + Shader::RemovePrecisionQualifiersFromGLSL(sVertexShaderSourceCodeGLSL_Instanced);
				else
					sVertexShaderSourceCode = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sVertexShaderSourceCodeGLSL);

				sFragmentShaderSourceCode = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sFragmentShaderSourceCodeGLSL);
			}
		} else if (pShaderLanguage->GetShaderLanguage() == "Cg") {
			#include "SPTriangleShaders_Cg.h"

			// Is geometric primitive instancing supported?
			if (cRenderer.GetCapabilities().bInstancing)
				sVertexShaderSourceCode = sVertexShaderSourceCodeCg_Instanced;
			else
				sVertexShaderSourceCode = sVertexShaderSourceCodeCg;

			sFragmentShaderSourceCode = sFragmentShaderSourceCodeCg;
		}

		// Create a vertex shader instance
		m_pVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode);

		// Create a fragment shader instance
		m_pFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode);

		// Create a program instance and assign the created vertex, geometry and fragment shaders to it
		m_pProgram = static_cast<ProgramWrapper*>(pShaderLanguage->CreateProgram(m_pVertexShader, m_pFragmentShader));
	}
}

/**
*  @brief
*    Destructor
*/
SPTriangleShaders::~SPTriangleShaders()
{
	// Cleanup
	if (m_pProgram)
		delete m_pProgram;
	if (m_pFragmentShader)
		delete m_pFragmentShader;
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

	// Object space to clip space matrix
	Matrix4x4 mObjectSpaceToClipSpaceMatrix;

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
		mObjectSpaceToClipSpaceMatrix = mProjection*mView*mWorld;
	}

	// Make our program to the current one
	if (cRenderer.SetProgram(m_pProgram)) {
		// Set the program uniforms
		m_pProgram->Set("ObjectSpaceToClipSpaceMatrix", mObjectSpaceToClipSpaceMatrix);

		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		m_pProgram->Set("VertexPosition", m_pVertexBuffer, VertexBuffer::Position);
		m_pProgram->Set("VertexColor", m_pVertexBuffer, VertexBuffer::Color);

		// No back face culling, please. Else we can only see one 'side' of the triangle
		cRenderer.SetRenderState(RenderState::CullMode, Cull::None);

		// Now draw the primitives of our cool triangle

		// Is geometric primitive instancing supported?
		if (cRenderer.GetCapabilities().bInstancing) {
			// The primitive type is 'triangles', we start at vertex 0, draw '3' vertices and 10 instances of the triangle
			cRenderer.DrawPrimitivesInstanced(Primitive::TriangleList, 0, 3, 10);
		} else {
			// Draw 10 instances of the triangle
			for (int i=0; i<10; i++) {
				// Set the program uniform containing the integer instance ID of the primitive within the current batch
				m_pProgram->Set("InstanceID", i);

				// Draw: The primitive type is 'triangles', we start at vertex 0 and draw '3' vertices
				cRenderer.DrawPrimitives(Primitive::TriangleList, 0, 3);
			}
		}
	}
}
