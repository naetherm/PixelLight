/*********************************************************\
 *  File: ProgramUniformCg.cpp                           *
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
#include <PLMath/Vector3i.h>
#include <PLMath/Matrix4x4.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Quaternion.h>
#include <PLGraphics/Color/Color3.h>
#include <PLGraphics/Color/Color4.h>
#include <PLRendererOpenGL/Renderer.h>
#include <PLRendererOpenGL/TextureBuffer1D.h>
#include <PLRendererOpenGL/TextureBuffer2D.h>
#include <PLRendererOpenGL/TextureBuffer2DArray.h>
#include <PLRendererOpenGL/TextureBufferRectangle.h>
#include <PLRendererOpenGL/TextureBuffer3D.h>
#include <PLRendererOpenGL/TextureBufferCube.h>
#include "PLRendererOpenGLCg/ProgramUniformCg.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Public virtual ProgramUniform functions               ]
//[-------------------------------------------------------]
int ProgramUniformCg::Set(GLenum nOpenGLTextureTarget, GLuint nOpenGLTexture)
{
	// Get the texture unit assigned to this uniform
	const int nTextureUnit = GetTextureUnit();

	// Valid texture unit?
	if (nTextureUnit >= 0) {
		// Assign the OpenGL texture to the Cg program parameter
		cgGLSetTextureParameter(m_pCgParameter, nOpenGLTexture);
		cgGLEnableTextureParameter(m_pCgParameter);
	}

	// Done, return the texture unit the uniform is assigned to
	return nTextureUnit;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProgramUniformCg::ProgramUniformCg(CGparameter pCgParameter) :
	m_pCgParameter(pCgParameter)
{
}

/**
*  @brief
*    Destructor
*/
ProgramUniformCg::~ProgramUniformCg()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ProgramUniform functions   ]
//[-------------------------------------------------------]
// 1 component
void ProgramUniformCg::Get(int &nX)
{
	cgGetParameterValueic(m_pCgParameter, 1, &nX);
}

void ProgramUniformCg::Get(float &fX)
{
	cgGetParameterValuefc(m_pCgParameter, 1, &fX);
}

void ProgramUniformCg::Get(double &fX)
{
	cgGetParameterValuedc(m_pCgParameter, 1, &fX);
}

void ProgramUniformCg::Set(int nX)
{
	cgSetParameter1i(m_pCgParameter, nX);
}

void ProgramUniformCg::Set(float fX)
{
	cgSetParameter1f(m_pCgParameter, fX);
}

void ProgramUniformCg::Set(double fX)
{
	cgSetParameter1d(m_pCgParameter, fX);
}

// 2 components
void ProgramUniformCg::Get(int &nX, int &nY)
{
	int nElements[2];
	cgGetParameterValueic(m_pCgParameter, 2, nElements);
	nX = nElements[0];
	nY = nElements[1];
}

void ProgramUniformCg::Get(float &fX, float &fY)
{
	float fElements[2];
	cgGetParameterValuefc(m_pCgParameter, 2, fElements);
	fX = fElements[0];
	fY = fElements[1];
}

void ProgramUniformCg::Get(double &fX, double &fY)
{
	double fElements[2];
	cgGetParameterValuedc(m_pCgParameter, 2, fElements);
	fX = fElements[0];
	fY = fElements[1];
}

void ProgramUniformCg::Get(Vector2i &vVector)
{
	cgGetParameterValueic(m_pCgParameter, 2, vVector.nV);
}

void ProgramUniformCg::Get(Vector2 &vVector)
{
	cgGetParameterValuefc(m_pCgParameter, 2, vVector.fV);
}

void ProgramUniformCg::Get2(int *pnComponents)
{
	cgGetParameterValueic(m_pCgParameter, 2, pnComponents);
}

void ProgramUniformCg::Get2(float *pfComponents)
{
	cgGetParameterValuefc(m_pCgParameter, 2, pfComponents);
}

void ProgramUniformCg::Get2(double *pfComponents)
{
	cgGetParameterValuedc(m_pCgParameter, 2, pfComponents);
}

void ProgramUniformCg::Set(int nX, int nY)
{
	cgSetParameter2i(m_pCgParameter, nX, nY);
}

void ProgramUniformCg::Set(float fX, float fY)
{
	cgSetParameter2f(m_pCgParameter, fX, fY);
}

void ProgramUniformCg::Set(double fX, double fY)
{
	cgSetParameter2d(m_pCgParameter, fX, fY);
}

void ProgramUniformCg::Set(const Vector2i &vVector)
{
	cgSetParameter2iv(m_pCgParameter, vVector);
}

void ProgramUniformCg::Set(const Vector2 &vVector)
{
	cgSetParameter2fv(m_pCgParameter, vVector);
}

void ProgramUniformCg::Set2(const int *pnComponents)
{
	cgSetParameter2iv(m_pCgParameter, pnComponents);
}

void ProgramUniformCg::Set2(const float *pfComponents)
{
	cgSetParameter2fv(m_pCgParameter, pfComponents);
}

void ProgramUniformCg::Set2(const double *pfComponents)
{
	cgSetParameter2dv(m_pCgParameter, pfComponents);
}

// 3 components
void ProgramUniformCg::Get(int &nX, int &nY, int &nZ)
{
	int nElements[3];
	cgGetParameterValueic(m_pCgParameter, 3, nElements);
	nX = nElements[0];
	nY = nElements[1];
	nZ = nElements[2];
}

void ProgramUniformCg::Get(float &fX, float &fY, float &fZ)
{
	float fElements[3];
	cgGetParameterValuefc(m_pCgParameter, 3, fElements);
	fX = fElements[0];
	fY = fElements[1];
	fZ = fElements[2];
}

void ProgramUniformCg::Get(double &fX, double &fY, double &fZ)
{
	double fElements[3];
	cgGetParameterValuedc(m_pCgParameter, 3, fElements);
	fX = fElements[0];
	fY = fElements[1];
	fZ = fElements[2];
}

void ProgramUniformCg::Get(Vector3i &vVector)
{
	cgGetParameterValueic(m_pCgParameter, 3, vVector.nV);
}

void ProgramUniformCg::Get(Vector3 &vVector)
{
	cgGetParameterValuefc(m_pCgParameter, 3, vVector.fV);
}

void ProgramUniformCg::Get(Color3 &cColor)
{
	cgGetParameterValuefc(m_pCgParameter, 3, cColor.fColor);
}

void ProgramUniformCg::Get3(int *pnComponents)
{
	cgGetParameterValueic(m_pCgParameter, 3, pnComponents);
}

void ProgramUniformCg::Get3(float *pfComponents)
{
	cgGetParameterValuefc(m_pCgParameter, 3, pfComponents);
}

void ProgramUniformCg::Get3(double *pfComponents)
{
	cgGetParameterValuedc(m_pCgParameter, 3, pfComponents);
}

void ProgramUniformCg::Set(int nX, int nY, int nZ)
{
	cgSetParameter3i(m_pCgParameter, nX, nY, nZ);
}

void ProgramUniformCg::Set(float fX, float fY, float fZ)
{
	cgSetParameter3f(m_pCgParameter, fX, fY, fZ);
}

void ProgramUniformCg::Set(double fX, double fY, double fZ)
{
	cgSetParameter3d(m_pCgParameter, fX, fY, fZ);
}

void ProgramUniformCg::Set(const Vector3i &vVector)
{
	cgSetParameter3iv(m_pCgParameter, vVector);
}

void ProgramUniformCg::Set(const Vector3 &vVector)
{
	cgSetParameter3fv(m_pCgParameter, vVector);
}

void ProgramUniformCg::Set(const Color3 &cColor)
{
	cgSetParameter3fv(m_pCgParameter, cColor);
}

void ProgramUniformCg::Set3(const int *pnComponents)
{
	cgSetParameter3iv(m_pCgParameter, pnComponents);
}

void ProgramUniformCg::Set3(const float *pfComponents)
{
	cgSetParameter3fv(m_pCgParameter, pfComponents);
}

void ProgramUniformCg::Set3(const double *pfComponents)
{
	cgSetParameter3dv(m_pCgParameter, pfComponents);
}

// 4 components
void ProgramUniformCg::Get(int &nX, int &nY, int &nZ, int &nW)
{
	int nElements[4];
	cgGetParameterValueic(m_pCgParameter, 4, nElements);
	nX = nElements[0];
	nY = nElements[1];
	nZ = nElements[2];
	nW = nElements[3];
}

void ProgramUniformCg::Get(float &fX, float &fY, float &fZ, float &fW)
{
	float fElements[4];
	cgGetParameterValuefc(m_pCgParameter, 4, fElements);
	fX = fElements[0];
	fY = fElements[1];
	fZ = fElements[2];
	fW = fElements[3];
}

void ProgramUniformCg::Get(double &fX, double &fY, double &fZ, double &fW)
{
	double fElements[4];
	cgGetParameterValuedc(m_pCgParameter, 4, fElements);
	fX = fElements[0];
	fY = fElements[1];
	fZ = fElements[2];
	fW = fElements[3];
}

void ProgramUniformCg::Get(Vector4 &vVector)
{
	cgGetParameterValuefc(m_pCgParameter, 4, vVector.fV);
}

void ProgramUniformCg::Get(Color4 &cColor)
{
	cgGetParameterValuefc(m_pCgParameter, 4, cColor.fColor);
}

void ProgramUniformCg::Get(Quaternion &qQuaternion)
{
	cgGetParameterValuefc(m_pCgParameter, 4, qQuaternion.fQ);
}

void ProgramUniformCg::Get4(int *pnComponents)
{
	cgGetParameterValueic(m_pCgParameter, 4, pnComponents);
}

void ProgramUniformCg::Get4(float *pfComponents)
{
	cgGetParameterValuefc(m_pCgParameter, 4, pfComponents);
}

void ProgramUniformCg::Get4(double *pfComponents)
{
	cgGetParameterValuedc(m_pCgParameter, 4, pfComponents);
}

void ProgramUniformCg::Set(int nX, int nY, int nZ, int nW)
{
	cgSetParameter4i(m_pCgParameter, nX, nY, nZ, nW);
}

void ProgramUniformCg::Set(float fX, float fY, float fZ, float fW)
{
	cgSetParameter4f(m_pCgParameter, fX, fY, fZ, fW);
}

void ProgramUniformCg::Set(double fX, double fY, double fZ, double fW)
{
	cgSetParameter4d(m_pCgParameter, fX, fY, fZ, fW);
}

void ProgramUniformCg::Set(const Vector4 &vVector)
{
	cgSetParameter4fv(m_pCgParameter, vVector);
}

void ProgramUniformCg::Set(const Color4 &cColor)
{
	cgSetParameter4fv(m_pCgParameter, cColor);
}

void ProgramUniformCg::Set(const Quaternion &qQuaternion)
{
	cgSetParameter4fv(m_pCgParameter, qQuaternion);
}

void ProgramUniformCg::Set4(const int *pnComponents)
{
	cgSetParameter4iv(m_pCgParameter, pnComponents);
}

void ProgramUniformCg::Set4(const float *pfComponents)
{
	cgSetParameter4fv(m_pCgParameter, pfComponents);
}

void ProgramUniformCg::Set4(const double *pfComponents)
{
	cgSetParameter4dv(m_pCgParameter, pfComponents);
}

// Matrix
void ProgramUniformCg::Get(Matrix3x3 &mMatrix)
{
	cgGetParameterValuefc(m_pCgParameter, 9, mMatrix.fM);
}

void ProgramUniformCg::Get(Matrix4x4 &mMatrix)
{
	cgGetParameterValuefc(m_pCgParameter, 16, mMatrix.fM);
}

void ProgramUniformCg::Set(const Matrix3x3 &mMatrix, bool bTranspose)
{
	if (bTranspose)
		cgSetMatrixParameterfr(m_pCgParameter, mMatrix);
	else
		cgSetMatrixParameterfc(m_pCgParameter, mMatrix);
}

void ProgramUniformCg::Set(const Matrix4x4 &mMatrix, bool bTranspose)
{
	if (bTranspose)
		cgSetMatrixParameterfr(m_pCgParameter, mMatrix);
	else
		cgSetMatrixParameterfc(m_pCgParameter, mMatrix);
}

// Texture
int ProgramUniformCg::GetTextureUnit() const
{
	return (cgGetParameterClass(m_pCgParameter) == CG_PARAMETERCLASS_SAMPLER) ? (cgGLGetTextureEnum(m_pCgParameter) - GL_TEXTURE0_ARB) : -1;
}

int ProgramUniformCg::Set(PLRenderer::TextureBuffer *pTextureBuffer)
{
	// Get the texture unit assigned to this uniform
	const int nTextureUnit = GetTextureUnit();

	// Valid texture unit?
	if (nTextureUnit >= 0) {
		// Get the OpenGL texture
		GLuint nOpenGLTexture = 0;
		if (pTextureBuffer) {
			// Check resource type
			switch (pTextureBuffer->GetType()) {
				case PLRenderer::Resource::TypeTextureBuffer1D:
					nOpenGLTexture = static_cast<PLRendererOpenGL::TextureBuffer1D*>(pTextureBuffer)->GetOpenGLTexture();
					break;

				case PLRenderer::Resource::TypeTextureBuffer2D:
					nOpenGLTexture = static_cast<PLRendererOpenGL::TextureBuffer2D*>(pTextureBuffer)->GetOpenGLTexture();
					break;

				case PLRenderer::Resource::TypeTextureBuffer2DArray:
					nOpenGLTexture = static_cast<PLRendererOpenGL::TextureBuffer2DArray*>(pTextureBuffer)->GetOpenGLTexture();
					break;

				case PLRenderer::Resource::TypeTextureBufferRectangle:
					nOpenGLTexture = static_cast<PLRendererOpenGL::TextureBufferRectangle*>(pTextureBuffer)->GetOpenGLTexture();
					break;

				case PLRenderer::Resource::TypeTextureBuffer3D:
					nOpenGLTexture = static_cast<PLRendererOpenGL::TextureBuffer3D*>(pTextureBuffer)->GetOpenGLTexture();
					break;

				case PLRenderer::Resource::TypeTextureBufferCube:
					nOpenGLTexture = static_cast<PLRendererOpenGL::TextureBufferCube*>(pTextureBuffer)->GetOpenGLTexture();
					break;
			}
		}

		// Assign the OpenGL texture to the Cg program parameter
		cgGLSetTextureParameter(m_pCgParameter, nOpenGLTexture);
		cgGLEnableTextureParameter(m_pCgParameter);

		// Inform the renderer
		if (pTextureBuffer)
			static_cast<PLRendererOpenGL::Renderer&>(pTextureBuffer->GetRenderer()).SetShaderProgramTextureBuffer(nTextureUnit, pTextureBuffer);

		// Done
		return nTextureUnit;
	}

	// Error!
	return -1;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg
