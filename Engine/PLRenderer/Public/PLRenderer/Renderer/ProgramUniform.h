/*********************************************************\
 *  File: ProgramUniform.h                               *
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


#ifndef __PLRENDERER_PROGRAMUNIFORM_H__
#define __PLRENDERER_PROGRAMUNIFORM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/PLRenderer.h"


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
*    Abstract renderer program uniform
*
*  @remarks
*    An uniform is a program parameter which stays the same for all elements processed within the shader pipeline.
*    Whether or not parameter shadowing is used, depends on the internal shader API. GLSL and Cg perform parameter
*    shadowing. Shadowing means that the uniform will keep it's value when another program is set and still has
*    the same value when the program of the uniform is set again as the currently one used for rendering.
*
*    Please note that the internal shader API like GLSL or Cg may handle uniforms differently. When using Cg it's
*    possible to set the uniform value at each time, even when the program the uniform is part of is currently
*    not the used renderer program. When using GLSL, it's only possible to access the value of an uniform when
*    the program, the uniform is part of, is the currently used for rendering. For performance reasons, this
*    abstract program uniform interface can't compensate those differences. It has to be as lightweight as possible
*    in order to keep the interface performance impact as low as possible.
*
*    In order to be as internal shader API independent as possible, it's highly recommended to access program
*    uniforms only when the program, the uniform is part of, is currently used for rendering.
*
*  @note
*    - The value of an uniform is directly coupled to a certain program
*    - Do only access uniform values when the program, the uniform is part of, is currently used for rendering
*/
class ProgramUniform {


	//[-------------------------------------------------------]
	//[ Public virtual ProgramUniform functions               ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ 1 component                                           ]
		//[-------------------------------------------------------]
		virtual void Get(int &nX) = 0;
		virtual void Get(float &fX) = 0;
		virtual void Get(double &fX) = 0;
		virtual void Set(int nX) = 0;
		virtual void Set(float fX) = 0;
		virtual void Set(double fX) = 0;


		//[-------------------------------------------------------]
		//[ 2 components                                          ]
		//[-------------------------------------------------------]
		virtual void Get(int &nX, int &nY) = 0;
		virtual void Get(float &fX, float &fY) = 0;
		virtual void Get(double &fX, double &fY) = 0;
		virtual void Get(PLMath::Vector2i &vVector) = 0;
		virtual void Get(PLMath::Vector2 &vVector) = 0;
		virtual void Get2(int *pnComponents) = 0;
		virtual void Get2(float *pfComponents) = 0;
		virtual void Get2(double *pfComponents) = 0;
		virtual void Set(int nX, int nY) = 0;
		virtual void Set(float fX, float fY) = 0;
		virtual void Set(double fX, double fY) = 0;
		virtual void Set(const PLMath::Vector2i &vVector) = 0;
		virtual void Set(const PLMath::Vector2 &vVector) = 0;
		virtual void Set2(const int *pnComponents) = 0;
		virtual void Set2(const float *pfComponents) = 0;
		virtual void Set2(const double *pfComponents) = 0;


		//[-------------------------------------------------------]
		//[ 3 components                                          ]
		//[-------------------------------------------------------]
		virtual void Get(int &nX, int &nY, int &nZ) = 0;
		virtual void Get(float &fX, float &fY, float &fZ) = 0;
		virtual void Get(double &fX, double &fY, double &fZ) = 0;
		virtual void Get(PLMath::Vector3i &vVector) = 0;
		virtual void Get(PLMath::Vector3 &vVector) = 0;
		virtual void Get(PLGraphics::Color3 &cColor) = 0;	// Floating point
		virtual void Get3(int *pnComponents) = 0;
		virtual void Get3(float *pfComponents) = 0;
		virtual void Get3(double *pfComponents) = 0;
		virtual void Set(int nX, int nY, int nZ) = 0;
		virtual void Set(float fX, float fY, float fZ) = 0;
		virtual void Set(double fX, double fY, double fZ) = 0;
		virtual void Set(const PLMath::Vector3i &vVector) = 0;
		virtual void Set(const PLMath::Vector3 &vVector) = 0;
		virtual void Set(const PLGraphics::Color3 &cColor) = 0;	// Floating point
		virtual void Set3(const int *pnComponents) = 0;
		virtual void Set3(const float *pfComponents) = 0;
		virtual void Set3(const double *pfComponents) = 0;


		//[-------------------------------------------------------]
		//[ 4 components                                          ]
		//[-------------------------------------------------------]
		virtual void Get(int &nX, int &nY, int &nZ, int &nW) = 0;
		virtual void Get(float &fX, float &fY, float &fZ, float &fW) = 0;
		virtual void Get(double &fX, double &fY, double &fZ, double &fW) = 0;
		virtual void Get(PLMath::Vector4 &vVector) = 0;
		virtual void Get(PLGraphics::Color4 &cColor) = 0;	// Floating point
		virtual void Get(PLMath::Quaternion &qQuaternion) = 0;
		virtual void Get4(int *pnComponents) = 0;
		virtual void Get4(float *pfComponents) = 0;
		virtual void Get4(double *pfComponents) = 0;
		virtual void Set(int nX, int nY, int nZ, int nW) = 0;
		virtual void Set(float fX, float fY, float fZ, float fW) = 0;
		virtual void Set(double fX, double fY, double fZ, double fW) = 0;
		virtual void Set(const PLMath::Vector4 &vVector) = 0;
		virtual void Set(const PLGraphics::Color4 &cColor) = 0;	// Floating point
		virtual void Set(const PLMath::Quaternion &qQuaternion) = 0;
		virtual void Set4(const int *pnComponents) = 0;
		virtual void Set4(const float *pfComponents) = 0;
		virtual void Set4(const double *pfComponents) = 0;


		//[-------------------------------------------------------]
		//[ Matrix                                                ]
		//[-------------------------------------------------------]
		virtual void Get(PLMath::Matrix3x3 &mMatrix) = 0;
		virtual void Get(PLMath::Matrix4x4 &mMatrix) = 0;
		virtual void Set(const PLMath::Matrix3x3 &mMatrix, bool bTranspose = false) = 0;
		virtual void Set(const PLMath::Matrix4x4 &mMatrix, bool bTranspose = false) = 0;


		//[-------------------------------------------------------]
		//[ Texture                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    If this uniform is a texture, this method will return the texture unit assigned to this uniform
		*
		*  @return
		*    The texture unit assigned to this uniform, negative on error
		*/
		virtual int GetTextureUnit() const = 0;

		/**
		*  @brief
		*    If this uniform is a texture, this method sets the texture buffer assigned to the uniform
		*
		*  @return
		*    The texture unit assigned to this uniform (same as GetTextureUnit()), negative on error
		*/
		virtual int Set(TextureBuffer *pTextureBuffer) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API ProgramUniform();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~ProgramUniform();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ProgramUniform(const ProgramUniform &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		ProgramUniform &operator =(const ProgramUniform &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_PROGRAMUNIFORM_H__
