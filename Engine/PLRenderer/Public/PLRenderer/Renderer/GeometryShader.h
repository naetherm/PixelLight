/*********************************************************\
 *  File: GeometryShader.h                               *
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


#ifndef __PLRENDERER_GEOMETRYSHADER_H__
#define __PLRENDERER_GEOMETRYSHADER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Shader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer geometry shader resource
*/
class GeometryShader : public Shader {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Input primitive type
		*/
		enum EInputPrimitiveType {
			InputPoints             =  0,	/**< List of point primitives */
			InputLines              =  1,	/**< List of line or line strip primitives */
			InputLinesAdjacency     =  2,	/**< List of line with adjacency or line strip with adjacency primitives */
			InputTriangles          =  3,	/**< List of triangle or triangle strip primitives */
			InputTrianglesAdjacency =  4	/**< List of triangle with adjacency or triangle strip with adjacency primitives */
		};

		/**
		*  @brief
		*    Output primitive type
		*/
		enum EOutputPrimitiveType {
			OutputPoints    =  0,	/**< A list of of point primitives */
			OutputLines     =  1,	/**< A list of line primitives */
			OutputTriangles =  2	/**< A list of triangle primitives */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~GeometryShader();


	//[-------------------------------------------------------]
	//[ Public virtual GeometryShader functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the input primitive type
		*
		*  @return
		*    The input primitive type
		*/
		virtual EInputPrimitiveType GetInputPrimitiveType() const = 0;

		/**
		*  @brief
		*    Returns the output primitive type
		*
		*  @return
		*    The output primitive type
		*/
		virtual EOutputPrimitiveType GetOutputPrimitiveType() const = 0;

		/**
		*  @brief
		*    Returns the number of output vertices
		*
		*  @return
		*    The number of output vertices, 0 if the maximum possible number of output vertices should be used
		*/
		virtual PLCore::uint32 GetNumOfOutputVertices() const = 0;

		/**
		*  @brief
		*    Sets the geometry shader source code
		*
		*  @param[in] sSourceCode
		*    Geometry shader source code, usually blank ASCII code
		*  @param[in] nInputPrimitiveType
		*    Input primitive type, for "Shader::SetSourceCode()" "InputTriangles" is used as default
		*  @param[in] nOutputPrimitiveType
		*    Output primitive type, for "Shader::SetSourceCode()" "OutputTriangles" is used as default
		*  @param[in] nNumOfOutputVertices
		*    Number of output vertices, 0 if the maximum possible number of output vertices should be used, for "Shader::SetSourceCode()" "0" is used as default
		*  @param[in] sProfile
		*    Geometry shader profile to use, if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*  @param[in] sArguments
		*    Optional shader compiler arguments, e.g. "version=150" when using Cg and a "glslg" profile
		*  @param[in] sEntry
		*    Entry point, if empty string, "main" is used as default
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    Extended version of "Shader::SetSourceCode()" for geometry shaders allowing also to specify
		*    the input/output primitives and the number of generated vertices. Please note that not each
		*    internal implementation may actually need this information, but it's highly recommended to
		*    provide this information anyway to be able to switch the internal implementation (e.g. using
		*    OpenGL instead of DirectX and/or Cg instead of HLSL/GLSL).
		*
		*  @see
		*   - "Shader::SetSourceCode()" for additional information
		*/
		virtual bool SetSourceCode(const PLCore::String &sSourceCode, EInputPrimitiveType nInputPrimitiveType, EOutputPrimitiveType nOutputPrimitiveType, PLCore::uint32 nNumOfOutputVertices, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "") = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*/
		PLRENDERER_API GeometryShader(Renderer &cRenderer);


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
		GeometryShader(const GeometryShader &cSource);

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
		GeometryShader &operator =(const GeometryShader &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_GEOMETRYSHADER_H__
