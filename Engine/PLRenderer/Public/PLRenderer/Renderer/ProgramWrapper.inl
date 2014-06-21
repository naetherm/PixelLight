/*********************************************************\
 *  File: ProgramWrapper.inl                             *
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
#include "PLRenderer/Renderer/ProgramUniform.h"
#include "PLRenderer/Renderer/ProgramAttribute.h"
#include "PLRenderer/Renderer/ProgramUniformBlock.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Program uniform ("ProgramUniform"-class)              ]
//[-------------------------------------------------------]
inline void ProgramWrapper::Get(const PLCore::String &sUniformName, int &nX)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(nX);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, float &fX)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(fX);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, double &fX)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(fX);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, int nX)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(nX);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, float fX)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, double fX)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, int &nX, int &nY)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(nX, nY);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, float &fX, float &fY)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(fX, fY);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, double &fX, double &fY)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(fX, fY);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Vector2i &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(vVector);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Vector2 &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(vVector);
}

inline void ProgramWrapper::Get2(const PLCore::String &sUniformName, int *pnComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get2(pnComponents);
}

inline void ProgramWrapper::Get2(const PLCore::String &sUniformName, float *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get2(pfComponents);
}

inline void ProgramWrapper::Get2(const PLCore::String &sUniformName, double *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get2(pfComponents);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, int nX, int nY)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(nX, nY);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, float fX, float fY)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX, fY);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, double fX, double fY)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX, fY);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Vector2i &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(vVector);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Vector2 &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(vVector);
}

inline void ProgramWrapper::Set2(const PLCore::String &sUniformName, const int *pnComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set2(pnComponents);
}

inline void ProgramWrapper::Set2(const PLCore::String &sUniformName, const float *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set2(pfComponents);
}

inline void ProgramWrapper::Set2(const PLCore::String &sUniformName, const double *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set2(pfComponents);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, int &nX, int &nY, int &nZ)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(nX, nY, nZ);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, float &fX, float &fY, float &fZ)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(fX, fY, fZ);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, double &fX, double &fY, double &fZ)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(fX, fY, fZ);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Vector3i &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(vVector);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Vector3 &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(vVector);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLGraphics::Color3 &cColor)	// Floating point
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(cColor);
}

inline void ProgramWrapper::Get3(const PLCore::String &sUniformName, int *pnComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get3(pnComponents);
}

inline void ProgramWrapper::Get3(const PLCore::String &sUniformName, float *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get3(pfComponents);
}

inline void ProgramWrapper::Get3(const PLCore::String &sUniformName, double *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get3(pfComponents);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, int nX, int nY, int nZ)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(nX, nY, nZ);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, float fX, float fY, float fZ)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX, fY, fZ);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, double fX, double fY, double fZ)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX, fY, fZ);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Vector3i &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(vVector);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Vector3 &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(vVector);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLGraphics::Color3 &cColor)	// Floating point
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(cColor);
}

inline void ProgramWrapper::Set3(const PLCore::String &sUniformName, const int *pnComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set3(pnComponents);
}

inline void ProgramWrapper::Set3(const PLCore::String &sUniformName, const float *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set3(pfComponents);
}

inline void ProgramWrapper::Set3(const PLCore::String &sUniformName, const double *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set3(pfComponents);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, int &nX, int &nY, int &nZ, int &nW)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(nX, nY, nZ, nW);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, float &fX, float &fY, float &fZ, float &fW)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(fX, fY, fZ, fW);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, double &fX, double &fY, double &fZ, double &fW)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX, fY, fZ, fW);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Vector4 &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(vVector);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLGraphics::Color4 &cColor)	// Floating point
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(cColor);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Quaternion &qQuaternion)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(qQuaternion);
}

inline void ProgramWrapper::Get4(const PLCore::String &sUniformName, int *pnComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get4(pnComponents);
}

inline void ProgramWrapper::Get4(const PLCore::String &sUniformName, float *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get4(pfComponents);
}

inline void ProgramWrapper::Get4(const PLCore::String &sUniformName, double *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get4(pfComponents);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, int nX, int nY, int nZ, int nW)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(nX, nY, nZ, nW);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, float fX, float fY, float fZ, float fW)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX, fY, fZ, fW);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, double fX, double fY, double fZ, double fW)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(fX, fY, fZ, fW);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Vector4 &vVector)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(vVector);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLGraphics::Color4 &cColor)	// Floating point
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(cColor);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Quaternion &qQuaternion)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(qQuaternion);
}

inline void ProgramWrapper::Set4(const PLCore::String &sUniformName, const int *pnComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set4(pnComponents);
}

inline void ProgramWrapper::Set4(const PLCore::String &sUniformName, const float *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set4(pfComponents);
}

inline void ProgramWrapper::Set4(const PLCore::String &sUniformName, const double *pfComponents)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set4(pfComponents);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Matrix3x3 &mMatrix)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(mMatrix);
}

inline void ProgramWrapper::Get(const PLCore::String &sUniformName, PLMath::Matrix4x4 &mMatrix)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Get(mMatrix);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Matrix3x3 &mMatrix, bool bTranspose)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(mMatrix, bTranspose);
}

inline void ProgramWrapper::Set(const PLCore::String &sUniformName, const PLMath::Matrix4x4 &mMatrix, bool bTranspose)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	if (pProgramUniform)
		pProgramUniform->Set(mMatrix, bTranspose);
}

inline int ProgramWrapper::GetTextureUnit(const PLCore::String &sUniformName)
{
	const ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	return pProgramUniform ? pProgramUniform->GetTextureUnit() : -1;
}

inline int ProgramWrapper::Set(const PLCore::String &sUniformName, TextureBuffer *pTextureBuffer)
{
	ProgramUniform *pProgramUniform = GetUniform(sUniformName);
	return pProgramUniform ? pProgramUniform->Set(pTextureBuffer) : -1;
}


//[-------------------------------------------------------]
//[ Program uniform buffer ("ProgramUniformBlock"-class)  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the uniform buffer feeding the specified uniform block with data
*/
inline bool ProgramWrapper::Set(const PLCore::String &sUniformBlockName, UniformBuffer *pUniformBuffer, PLCore::uint32 nBindingPoint)
{
	ProgramUniformBlock *pProgramUniformBlock = GetUniformBlock(sUniformBlockName);
	return pProgramUniformBlock ? pProgramUniformBlock->SetUniformBuffer(pUniformBuffer, nBindingPoint) : false;
}


//[-------------------------------------------------------]
//[ Program attribute ("ProgramAttribute"-class)          ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a attribute by using an index to reference the vertex buffer attribute to use
*/
inline bool ProgramWrapper::Set(const PLCore::String &sAttributeName, VertexBuffer *pVertexBuffer, PLCore::uint32 nIndex)
{
	ProgramAttribute *pProgramAttribute = GetAttribute(sAttributeName);
	return pProgramAttribute ? pProgramAttribute->Set(pVertexBuffer, nIndex) : false;
}

/**
*  @brief
*    Sets a attribute by using a semantic to reference the vertex buffer attribute to use
*/
inline bool ProgramWrapper::Set(const PLCore::String &sAttributeName, VertexBuffer *pVertexBuffer, VertexBuffer::ESemantic nSemantic, PLCore::uint32 nChannel)
{
	ProgramAttribute *pProgramAttribute = GetAttribute(sAttributeName);
	return pProgramAttribute ? pProgramAttribute->Set(pVertexBuffer, nSemantic, nChannel) : false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline ProgramWrapper::ProgramWrapper(Renderer &cRenderer) : Program(cRenderer)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
