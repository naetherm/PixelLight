/*********************************************************\
 *  File: SPRTTShaders.cpp                               *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/TextureBuffer2D.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include "SPRTTShaders.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SPRTTShaders, "", SPRTT, "Shaders based render to texture surface painter")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLRenderer::Renderer&,	"Parameter constructor",	"")
pl_class_metadata_end(SPRTTShaders)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SPRTTShaders::SPRTTShaders(Renderer &cRenderer) : SPRTT(cRenderer),
	m_pRenderTarget(nullptr),
	m_pColorTarget1(nullptr),
	m_pColorTarget2(nullptr),
	m_pColorTarget3(nullptr),
	m_pSceneVertexShader(nullptr),
	m_pSceneFragmentShader(nullptr),
	m_pSceneProgram(nullptr),
	m_pVertexShader(nullptr),
	m_pFragmentShader(nullptr),
	m_pProgram(nullptr)
{
	// Check/get number of supported color render targets
	uint8 nMaxColorTargets = 4;
	if (nMaxColorTargets > cRenderer.GetCapabilities().nMaxColorRenderTargets)
		nMaxColorTargets = cRenderer.GetCapabilities().nMaxColorRenderTargets;

	{ // Render targets
		// Create the render target. We will create a very low resolution 2D texture buffer to see funny pixels.
		m_pRenderTarget = cRenderer.CreateSurfaceTextureBuffer2D(Vector2i(64, 64), TextureBuffer::R8G8B8, SurfaceTextureBuffer::Depth|SurfaceTextureBuffer::NoMultisampleAntialiasing, nMaxColorTargets);
		if (m_pRenderTarget && nMaxColorTargets > 1) {
			// Set additional color render targets
			if (nMaxColorTargets > 1 && !m_pColorTarget1) {
				Image cImage = Image::CreateImage(DataByte, ColorRGB, Vector3i(64, 64, 1));
				m_pColorTarget1 = cRenderer.CreateTextureBuffer2D(cImage, TextureBuffer::Unknown, TextureBuffer::RenderTarget);
			}
			if (nMaxColorTargets > 2 && !m_pColorTarget2) {
				Image cImage = Image::CreateImage(DataByte, ColorRGB, Vector3i(64, 64, 1));
				m_pColorTarget2 = cRenderer.CreateTextureBuffer2D(cImage, TextureBuffer::Unknown, TextureBuffer::RenderTarget);
			}
			if (nMaxColorTargets > 3 && !m_pColorTarget3) {
				Image cImage = Image::CreateImage(DataByte, ColorRGB, Vector3i(64, 64, 1));
				m_pColorTarget3 = cRenderer.CreateTextureBuffer2D(cImage, TextureBuffer::Unknown, TextureBuffer::RenderTarget);
			}
		}
	}

	// Decide which shader language should be used (for example "GLSL" or "Cg")
	ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(cRenderer.GetDefaultShaderLanguage());
	if (pShaderLanguage) {
		{ // Scene program (with MRT support)
			// Construct the string containing the fragment shader definitions
			String sDefinitions;
			if (m_pColorTarget1)
				sDefinitions += "#define MRT_1\n";
			if (m_pColorTarget2)
				sDefinitions += "#define MRT_2\n";
			if (m_pColorTarget3)
				sDefinitions += "#define MRT_3\n";

			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode;
			if (pShaderLanguage->GetShaderLanguage() == "GLSL") {
				#include "SPRTTShaders_GLSL.h"
				if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
					// Get shader source codes
					sVertexShaderSourceCode   = "#version 100\n" + sSceneVertexShaderSourceCodeGLSL;
					sFragmentShaderSourceCode = "#version 100\n" + sDefinitions + sSceneFragmentShaderSourceCodeGLSL;
				} else {
					// Remove precision qualifiers so that we're able to use 120 (OpenGL 2.1 shaders) instead of 130 (OpenGL 3.0 shaders,
					// with this version we can keep the precision qualifiers) so that this shader requirements are as low as possible
					// -> In here we're using 120 instead of 110 because matrix casts are quite comfortable...
					sVertexShaderSourceCode   = "#version 120\n" + Shader::RemovePrecisionQualifiersFromGLSL(sSceneVertexShaderSourceCodeGLSL);
					sFragmentShaderSourceCode = "#version 120\n" + sDefinitions + Shader::RemovePrecisionQualifiersFromGLSL(sSceneFragmentShaderSourceCodeGLSL);
				}
			} else if (pShaderLanguage->GetShaderLanguage() == "Cg") {
				#include "SPRTTShaders_Cg.h"
				sVertexShaderSourceCode   = sSceneVertexShaderSourceCodeCg;
				sFragmentShaderSourceCode = sSceneFragmentShaderSourceCodeCg;
			}

			// Create a vertex shader instance
			// -> I define a Cg profile because when using an GLSL Cg profile (which is the default), the shader is not working correctly on my AMD/ATI ("AMD Catalyst� 11.3") system while it worked on the tested NVIDIA system...
			m_pSceneVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode, "arbvp1");

			// Create a fragment shader instance
			// -> I define a Cg profile because when using an GLSL Cg profile (which is the default), the shader is not working correctly on my AMD/ATI ("AMD Catalyst� 11.3") system while it worked on the tested NVIDIA system...
			m_pSceneFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode, "arbfp1");

			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pSceneProgram = pShaderLanguage->CreateProgram(m_pSceneVertexShader, m_pSceneFragmentShader);
		}

		{ // Program
			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode;
			if (pShaderLanguage->GetShaderLanguage() == "GLSL") {
				#include "SPRTTShaders_GLSL.h"
				if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
					// Get shader source codes
					sVertexShaderSourceCode   = "#version 100\n" + sVertexShaderSourceCodeGLSL;
					sFragmentShaderSourceCode = "#version 100\n" + sFragmentShaderSourceCodeGLSL;
				} else {
					// Remove precision qualifiers so that we're able to use 110 (OpenGL 2.0 shaders) instead of 130 (OpenGL 3.0 shaders,
					// with this version we can keep the precision qualifiers) so that this shader requirements are as low as possible
					sVertexShaderSourceCode   = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sVertexShaderSourceCodeGLSL);
					sFragmentShaderSourceCode = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sFragmentShaderSourceCodeGLSL);
				}
			} else if (pShaderLanguage->GetShaderLanguage() == "Cg") {
				#include "SPRTTShaders_Cg.h"
				sVertexShaderSourceCode   = sVertexShaderSourceCodeCg;
				sFragmentShaderSourceCode = sFragmentShaderSourceCodeCg;
			}

			// Create a vertex shader instance
			// -> I define a Cg profile because when using an GLSL Cg profile (which is the default), the shader is not working correctly on my AMD/ATI ("AMD Catalyst� 11.3") system while it worked on the tested NVIDIA system...
			m_pVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode, "arbvp1");

			// Create a fragment shader instance
			// -> I define a Cg profile because when using an GLSL Cg profile (which is the default), the shader is not working correctly on my AMD/ATI ("AMD Catalyst� 11.3") system while it worked on the tested NVIDIA system...
			m_pFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode, "arbfp1");

			// Create a program instance and assign the created vertex and fragment shaders to it
			m_pProgram = static_cast<ProgramWrapper*>(pShaderLanguage->CreateProgram(m_pVertexShader, m_pFragmentShader));
		}
	}
}

/**
*  @brief
*    Destructor
*/
SPRTTShaders::~SPRTTShaders()
{
	// Destroy the render targets
	if (m_pRenderTarget)
		delete m_pRenderTarget;
	if (m_pColorTarget1)
		delete m_pColorTarget1;
	if (m_pColorTarget2)
		delete m_pColorTarget2;
	if (m_pColorTarget3)
		delete m_pColorTarget3;

	// Cleanup
	if (m_pSceneProgram)
		delete m_pSceneProgram;
	if (m_pSceneFragmentShader)
		delete m_pSceneFragmentShader;
	if (m_pSceneVertexShader)
		delete m_pSceneVertexShader;
	if (m_pProgram)
		delete m_pProgram;
	if (m_pFragmentShader)
		delete m_pFragmentShader;
	if (m_pVertexShader)
		delete m_pVertexShader;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws the scene
*/
void SPRTTShaders::DrawScene(Renderer &cRenderer)
{
	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Gray);

	// Make our program to the current one
	if (cRenderer.SetProgram(m_pSceneProgram)) {
		// Calculate the world matrix
		Matrix4x4 mWorld;
		{
			// Build a rotation matrix by using a given Euler angle around the y-axis
			mWorld.FromEulerAngleY(static_cast<float>(m_fRotation*Math::DegToRad));
		}

		// Set program uniforms
		ProgramUniform *pProgramUniform = m_pSceneProgram->GetUniform("ObjectSpaceToClipSpaceMatrix");
		if (pProgramUniform) {
			// Calculate the view matrix
			Matrix4x4 mView;
			{
				mView.SetTranslation(0.0f, -0.1f, -0.5f);
			}

			// Calculate the projection matrix
			Matrix4x4 mProjection;
			{
				const float fAspect      = 1.0f;
				const float fAspectRadio = cRenderer.GetViewport().GetWidth()/(cRenderer.GetViewport().GetHeight()*fAspect);
				mProjection.PerspectiveFov(static_cast<float>(45.0f*Math::DegToRad), fAspectRadio, 0.001f, 1000.0f);
			}

			// Calculate the final composed world view projection matrix
			const Matrix4x4 mWorldViewProjection = mProjection*mView*mWorld;

			// Set object space to clip space matrix uniform
			pProgramUniform->Set(mWorldViewProjection);
		}

		// Set object space to world space matrix uniform
		pProgramUniform = m_pSceneProgram->GetUniform("ObjectSpaceToWorldSpaceMatrix");
		if (pProgramUniform)
			pProgramUniform->Set(mWorld);

		// Set world space light direction
		pProgramUniform = m_pSceneProgram->GetUniform("LightDirection");
		if (pProgramUniform)
			pProgramUniform->Set(Vector3::UnitZ);

		// Get the used mesh
		const Mesh *pMesh = m_pMeshHandler->GetMesh();
		if (pMesh) {
			// Get the mesh LOD level to use
			const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
			if (pLODLevel && pLODLevel->GetIndexBuffer()) {
				// Get and use the index buffer of the mesh LOD level
				cRenderer.SetIndexBuffer(pLODLevel->GetIndexBuffer());

				// Get the vertex buffer of the mesh handler
				VertexBuffer *pVertexBuffer = m_pMeshHandler->GetVertexBuffer();
				if (pVertexBuffer) {
					// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
					ProgramAttribute *pProgramAttribute = m_pSceneProgram->GetAttribute("VertexPosition");
					if (pProgramAttribute)
						pProgramAttribute->Set(pVertexBuffer, VertexBuffer::Position);
					pProgramAttribute = m_pSceneProgram->GetAttribute("VertexNormal");
					if (pProgramAttribute)
						pProgramAttribute->Set(pVertexBuffer, VertexBuffer::Normal);

					// Loop through all geometries of the mesh
					const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
					for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
						// Is this geometry active?
						const Geometry &cGeometry = lstGeometries[nGeo];
						if (cGeometry.IsActive()) {
							// Draw the geometry
							cRenderer.DrawIndexedPrimitives(
								cGeometry.GetPrimitiveType(),
								0,
								pVertexBuffer->GetNumOfElements()-1,
								cGeometry.GetStartIndex(),
								cGeometry.GetIndexSize()
							);
						}
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::SurfacePainter functions  ]
//[-------------------------------------------------------]
void SPRTTShaders::OnPaint(Surface &cSurface)
{
	// Get the used renderer
	Renderer &cRenderer = GetRenderer();

	// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
	cRenderer.Clear(Clear::Color | Clear::ZBuffer, Color4::Gray);

	// Backup current render target and set the new one to render in our texture buffer
	Surface *pRenderSurfaceBackup = cRenderer.GetRenderTarget();
	if (cRenderer.SetRenderTarget(m_pRenderTarget)) {
		if (m_pColorTarget1)
			cRenderer.SetColorRenderTarget(static_cast<TextureBuffer*>(m_pColorTarget1), 1);
		if (m_pColorTarget2)
			cRenderer.SetColorRenderTarget(static_cast<TextureBuffer*>(m_pColorTarget2), 2);
		if (m_pColorTarget3)
			cRenderer.SetColorRenderTarget(static_cast<TextureBuffer*>(m_pColorTarget3), 3);

		// Draw the scene
		DrawScene(cRenderer);

		// Reset render target
		cRenderer.SetRenderTarget(pRenderSurfaceBackup);
	}

	// This code is similar to the code of the triangle sample. But instead of a
	// triangle we will draw three rotating quadrangles. Further the used vertex
	// buffer has texture coordinates and we apply the 'teapot' texture buffer on the primitives.
	// Clear the content of the current used render target

	// Make our program to the current one
	if (cRenderer.SetProgram(m_pProgram)) {
		// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
		m_pProgram->Set("VertexPosition",		   m_pPositionVertexBuffer, VertexBuffer::Position);
		m_pProgram->Set("VertexTextureCoordinate", m_pPositionVertexBuffer, VertexBuffer::TexCoord);
		m_pProgram->Set("VertexColor",			   m_pColorVertexBuffer,    VertexBuffer::Color);

		// Set color factor
		m_pProgram->Set("ColorFactor", 0.0f);

		// Calculate the composed view projection matrix - we need it multiple times
		Matrix4x4 mViewProjection;
		{
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

			// Calculate the composed view projection matrix
			mViewProjection = mProjection*mView;
		}

		// No back face culling, please. Else we can only see one 'side' of the quadrangle
		cRenderer.SetRenderState(RenderState::CullMode, Cull::None);

		Matrix4x4 mWorld;
		{ // Draw quadrangle 1: Primary render target
			// Set object space to clip space matrix uniform
			ProgramUniform *pProgramUniform = m_pProgram->GetUniform("ObjectSpaceToClipSpaceMatrix");
			if (pProgramUniform) {
				mWorld.SetTranslationMatrix(2.0f, 1.0f, 0.0f);
				pProgramUniform->Set(mViewProjection*mWorld);
			}

			{ // Set the texture buffer we rendered our teapot in as the current texture buffer
				const int nTextureUnit = m_pProgram->Set("DiffuseMap", m_pRenderTarget->GetTextureBuffer());
				if (nTextureUnit >= 0) {
					// Disable mip mapping - this is required because we created/filled no mipmaps for your texture buffer
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		{ // Draw quadrangle 2: Color render target 1
			// Set object space to clip space matrix uniform
			ProgramUniform *pProgramUniform = m_pProgram->GetUniform("ObjectSpaceToClipSpaceMatrix");
			if (pProgramUniform) {
				mWorld.FromEulerAngleY(static_cast<float>(m_fRotation*Math::DegToRad));
				mWorld.SetTranslation(-2.0f, 1.0f, 0.0f);
				pProgramUniform->Set(mViewProjection*mWorld);
			}

			{ // Set the texture buffer we rendered our teapot in as the current texture buffer
				const int nTextureUnit = m_pProgram->Set("DiffuseMap", m_pColorTarget1 ? static_cast<TextureBuffer*>(m_pColorTarget1) : m_pRenderTarget->GetTextureBuffer());
				if (nTextureUnit >= 0) {
					// Disable mip mapping - this is required because we created/filled no mipmaps for your texture buffer
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		{ // Draw quadrangle 3: Color render target 2
			// Set object space to clip space matrix uniform
			ProgramUniform *pProgramUniform = m_pProgram->GetUniform("ObjectSpaceToClipSpaceMatrix");
			if (pProgramUniform) {
				mWorld.FromEulerAngleZ(static_cast<float>(m_fRotation*Math::DegToRad));
				mWorld.SetTranslation(0.0f, 1.0f, 0.0f);
				pProgramUniform->Set(mViewProjection*mWorld);
			}

			{ // Set the texture buffer we rendered our teapot in as the current texture buffer
				const int nTextureUnit = m_pProgram->Set("DiffuseMap", m_pColorTarget2 ? static_cast<TextureBuffer*>(m_pColorTarget2) : m_pRenderTarget->GetTextureBuffer());
				if (nTextureUnit >= 0) {
					// Disable mip mapping - this is required because we created/filled no mipmaps for your texture buffer
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		{ // Draw quadrangle 4: Color render target 3
			// Set object space to clip space matrix uniform
			ProgramUniform *pProgramUniform = m_pProgram->GetUniform("ObjectSpaceToClipSpaceMatrix");
			if (pProgramUniform) {
				mWorld.FromEulerAngleZ(static_cast<float>(-m_fRotation*Math::DegToRad));
				mWorld.SetTranslation(-2.0f, -1.0f, 0.0f);
				pProgramUniform->Set(mViewProjection*mWorld);
			}

			{ // Set the texture buffer we rendered our teapot in as the current texture buffer
				const int nTextureUnit = m_pProgram->Set("DiffuseMap", m_pColorTarget3 ? static_cast<TextureBuffer*>(m_pColorTarget3) : m_pRenderTarget->GetTextureBuffer());
				if (nTextureUnit >= 0) {
					// Disable mip mapping - this is required because we created/filled no mipmaps for your texture buffer
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		{ // Draw quadrangle 4: Primary render target, but with per vertex color - the primitive will be quite colorful :)
			// Set object space to clip space matrix uniform
			ProgramUniform *pProgramUniform = m_pProgram->GetUniform("ObjectSpaceToClipSpaceMatrix");
			if (pProgramUniform) {
				mWorld.FromEulerAngleZ(static_cast<float>(-m_fRotation*Math::DegToRad));
				mWorld.SetTranslation(2.0f, -1.0f, 0.0f);
				pProgramUniform->Set(mViewProjection*mWorld);
			}

			// Set color factor
			m_pProgram->Set("ColorFactor", 1.0f);

			{ // Set the texture buffer we rendered our teapot in as the current texture buffer
				const int nTextureUnit = m_pProgram->Set("DiffuseMap", m_pRenderTarget->GetTextureBuffer());
				if (nTextureUnit >= 0) {
					// Disable mip mapping - this is required because we created/filled no mipmaps for your texture buffer
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
				}
			}

			// Draw
			cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);
		}

		// Change the backface culling back to the default setting
		cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);
	}

	// Increase the rotation by the current time difference (time past since the last frame)
	// -> Normally, such work should NOT be performed within the rendering step, but we want
	//    to keep the implementation simple in here...
	m_fRotation += Timing::GetInstance()->GetTimeDifference()*50;
}
