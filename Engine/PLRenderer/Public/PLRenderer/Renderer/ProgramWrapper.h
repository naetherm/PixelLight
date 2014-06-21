/*********************************************************\
 *  File: ProgramWrapper.h                               *
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


#ifndef __PLRENDERER_PROGRAMWRAPPER_H__
#define __PLRENDERER_PROGRAMWRAPPER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Program.h"
#include "PLRenderer/Renderer/VertexBuffer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2;
	class Vector2i;
	class Vector3;
	class Vector3i;
	class Vector4;
	class Matrix3x3;
	class Matrix3x4;
	class Matrix4x4;
	class Quaternion;
}
namespace PLGraphics {
	class Color3;
	class Color4;
}
namespace PLRenderer {
	class TextureBuffer;
	class UniformBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Comfort renderer program wrapper
*
*  @remarks
*    The "Program.h"-header is slim and the program interface is designed to be universal and efficient.
*    While this is fine in general, it may feel to complicated in simple daily use. For example, in order
*    to set a program attribute one has to write for instance
*        ProgramAttribute *pProgramAttribute = pProgram->GetAttribute("VertexPosition");
*        if (pProgramAttribute)
*            pProgramAttribute->Set(pVertexBuffer, VertexBuffer::Position);
*
*    The purpose of this "ProgramWrapper"-interface is to make simple usage as shown in the example above
*    more compact and direct by allowing to write
*        m_pProgram->Set("VertexPosition", m_pVertexBuffer, VertexBuffer::Position)
*    instead.
*
*    Please note that "ProgramWrapper" is just an interface and no real class which can be instanced. All
*    methods are inlined and simple. In order to use this interface write e.g.
*        ProgramWrapper *pProgram = static_cast<ProgramWrapper*>(pShaderLanguage->CreateProgram(pVertexShader, pFragmentShader));
*    This means that it is actually a hack because one has to cast an class instance to a certain class, although the class
*    instance is in fact not an instance of it. Due to the simplicity of "ProgramWrapper" and the fact that it doesn't introduce
*    e.g. new variables, this is working.
*
*  @note
*    - In case you need the best possible performance when dealing with GPU programs, you may not want to use this program wrapper (see "Program"-class)
*/
class ProgramWrapper : public Program {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Program uniform ("ProgramUniform"-class)              ] -> See "ProgramUniform"-class for more information
		//[-------------------------------------------------------]
		inline void Get(const PLCore::String &sUniformName, int &nX);
		inline void Get(const PLCore::String &sUniformName, float &fX);
		inline void Get(const PLCore::String &sUniformName, double &fX);
		inline void Set(const PLCore::String &sUniformName, int nX);
		inline void Set(const PLCore::String &sUniformName, float fX);
		inline void Set(const PLCore::String &sUniformName, double fX);
		inline void Get(const PLCore::String &sUniformName, int &nX, int &nY);
		inline void Get(const PLCore::String &sUniformName, float &fX, float &fY);
		inline void Get(const PLCore::String &sUniformName, double &fX, double &fY);
		inline void Get(const PLCore::String &sUniformName, PLMath::Vector2i &vVector);
		inline void Get(const PLCore::String &sUniformName, PLMath::Vector2 &vVector);
		inline void Get2(const PLCore::String &sUniformName, int *pnComponents);
		inline void Get2(const PLCore::String &sUniformName, float *pfComponents);
		inline void Get2(const PLCore::String &sUniformName, double *pfComponents);
		inline void Set(const PLCore::String &sUniformName, int nX, int nY);
		inline void Set(const PLCore::String &sUniformName, float fX, float fY);
		inline void Set(const PLCore::String &sUniformName, double fX, double fY);
		inline void Set(const PLCore::String &sUniformName, const PLMath::Vector2i &vVector);
		inline void Set(const PLCore::String &sUniformName, const PLMath::Vector2 &vVector);
		inline void Set2(const PLCore::String &sUniformName, const int *pnComponents);
		inline void Set2(const PLCore::String &sUniformName, const float *pfComponents);
		inline void Set2(const PLCore::String &sUniformName, const double *pfComponents);
		inline void Get(const PLCore::String &sUniformName, int &nX, int &nY, int &nZ);
		inline void Get(const PLCore::String &sUniformName, float &fX, float &fY, float &fZ);
		inline void Get(const PLCore::String &sUniformName, double &fX, double &fY, double &fZ);
		inline void Get(const PLCore::String &sUniformName, PLMath::Vector3i &vVector);
		inline void Get(const PLCore::String &sUniformName, PLMath::Vector3 &vVector);
		inline void Get(const PLCore::String &sUniformName, PLGraphics::Color3 &cColor);	// Floating point
		inline void Get3(const PLCore::String &sUniformName, int *pnComponents);
		inline void Get3(const PLCore::String &sUniformName, float *pfComponents);
		inline void Get3(const PLCore::String &sUniformName, double *pfComponents);
		inline void Set(const PLCore::String &sUniformName, int nX, int nY, int nZ);
		inline void Set(const PLCore::String &sUniformName, float fX, float fY, float fZ);
		inline void Set(const PLCore::String &sUniformName, double fX, double fY, double fZ);
		inline void Set(const PLCore::String &sUniformName, const PLMath::Vector3i &vVector);
		inline void Set(const PLCore::String &sUniformName, const PLMath::Vector3 &vVector);
		inline void Set(const PLCore::String &sUniformName, const PLGraphics::Color3 &cColor);	// Floating point
		inline void Set3(const PLCore::String &sUniformName, const int *pnComponents);
		inline void Set3(const PLCore::String &sUniformName, const float *pfComponents);
		inline void Set3(const PLCore::String &sUniformName, const double *pfComponents);
		inline void Get(const PLCore::String &sUniformName, int &nX, int &nY, int &nZ, int &nW);
		inline void Get(const PLCore::String &sUniformName, float &fX, float &fY, float &fZ, float &fW);
		inline void Get(const PLCore::String &sUniformName, double &fX, double &fY, double &fZ, double &fW);
		inline void Get(const PLCore::String &sUniformName, PLMath::Vector4 &vVector);
		inline void Get(const PLCore::String &sUniformName, PLGraphics::Color4 &cColor);	// Floating point
		inline void Get(const PLCore::String &sUniformName, PLMath::Quaternion &qQuaternion);
		inline void Get4(const PLCore::String &sUniformName, int *pnComponents);
		inline void Get4(const PLCore::String &sUniformName, float *pfComponents);
		inline void Get4(const PLCore::String &sUniformName, double *pfComponents);
		inline void Set(const PLCore::String &sUniformName, int nX, int nY, int nZ, int nW);
		inline void Set(const PLCore::String &sUniformName, float fX, float fY, float fZ, float fW);
		inline void Set(const PLCore::String &sUniformName, double fX, double fY, double fZ, double fW);
		inline void Set(const PLCore::String &sUniformName, const PLMath::Vector4 &vVector);
		inline void Set(const PLCore::String &sUniformName, const PLGraphics::Color4 &cColor);	// Floating point
		inline void Set(const PLCore::String &sUniformName, const PLMath::Quaternion &qQuaternion);
		inline void Set4(const PLCore::String &sUniformName, const int *pnComponents);
		inline void Set4(const PLCore::String &sUniformName, const float *pfComponents);
		inline void Set4(const PLCore::String &sUniformName, const double *pfComponents);
		inline void Get(const PLCore::String &sUniformName, PLMath::Matrix3x3 &mMatrix);
		inline void Get(const PLCore::String &sUniformName, PLMath::Matrix4x4 &mMatrix);
		inline void Set(const PLCore::String &sUniformName, const PLMath::Matrix3x3 &mMatrix, bool bTranspose = false);
		inline void Set(const PLCore::String &sUniformName, const PLMath::Matrix4x4 &mMatrix, bool bTranspose = false);
		inline int GetTextureUnit(const PLCore::String &sUniformName);
		inline int Set(const PLCore::String &sUniformName, TextureBuffer *pTextureBuffer);

		//[-------------------------------------------------------]
		//[ Program uniform buffer ("ProgramUniformBlock"-class)  ] -> See "ProgramUniformBlock"-class for more information
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets the uniform buffer feeding the specified uniform block with data
		*
		*  @param[in] sUniformBlockName
		*    Uniform block name
		*  @param[in] pUniformBuffer
		*    The uniform buffer feeding this uniform block with data, can be a null pointer
		*  @param[in] nBindingPoint
		*    Binding point to use
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the number of bytes within the given uniform buffer
		*    is less than the number of bytes within this uniform block or there's a shader language mismatch)
		*
		*  @remarks
		*    The binding point is somewhat similar a texture unit. The total number of binding points
		*    which can be used at the same time during rendering are limited to e.g. 45 (just an example!)
		*    binding points.
		*/
		inline bool Set(const PLCore::String &sUniformBlockName, UniformBuffer *pUniformBuffer, PLCore::uint32 nBindingPoint);

		//[-------------------------------------------------------]
		//[ Program attribute ("ProgramAttribute"-class)          ] -> See "ProgramAttribute"-class for more information
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets a attribute by using an index to reference the vertex buffer attribute to use
		*
		*  @param[in] sAttributeName
		*    Attribute name
		*  @param[in] pVertexBuffer
		*    Vertex buffer to use, can be a null pointer
		*  @param[in] nIndex
		*    Index of the vertex buffer attribute to connect with the vertex shader attribute
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no attribute with the given name?
		*
		*  @note
		*    - Comfort method
		*/
		inline bool Set(const PLCore::String &sAttributeName, VertexBuffer *pVertexBuffer, PLCore::uint32 nIndex);

		/**
		*  @brief
		*    Sets a attribute by using a semantic to reference the vertex buffer attribute to use
		*
		*  @param[in] sAttributeName
		*    Attribute name
		*  @param[in] pVertexBuffer
		*    Vertex buffer to use, can be a null pointer
		*  @param[in] nSemantic
		*    Semantic of the vertex buffer attribute to connect with the vertex shader attribute
		*  @param[in] nChannel
		*    Pipeline channel (see ESemantic, maximum see MaxPipelineChannels)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no attribute with the given name?
		*
		*  @note
		*    - Comfort method
		*/
		inline bool Set(const PLCore::String &sAttributeName, VertexBuffer *pVertexBuffer, VertexBuffer::ESemantic nSemantic, PLCore::uint32 nChannel = 0);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*/
		inline ProgramWrapper(Renderer &cRenderer);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/ProgramWrapper.inl"


#endif // __PLRENDERER_PROGRAMWRAPPER_H__
