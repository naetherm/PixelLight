/*********************************************************\
 *  File: ShaderLanguage.h                               *
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


#ifndef __PLRENDERER_SHADERLANGUAGE_H__
#define __PLRENDERER_SHADERLANGUAGE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLRenderer/Renderer/GeometryShader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Program;
class VertexShader;
class UniformBuffer;
class FragmentShader;
class TessellationControlShader;
class TessellationEvaluationShader;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract shader language class
*/
class ShaderLanguage : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, ShaderLanguage, "PLRenderer", PLCore::Object, "Abstract shader language class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Creates a vertex shader and sets the shader source code
		*
		*  @param[in] sSourceCode
		*    Shader source code, usually blank ASCII code
		*  @param[in] sProfile
		*    Shader profile to use, if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*  @param[in] sArguments
		*    Optional shader compiler arguments, e.g. "version=150" when using Cg and a "glslv" profile
		*  @param[in] sEntry
		*    Entry point, if empty string, "main" is used as default
		*
		*  @return
		*    The created vertex shader, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateVertexShader()"-method
		*    - "Shader::SetSourceCode()"
		*/
		PLRENDERER_API VertexShader *CreateVertexShader(const PLCore::String &sSourceCode, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "");

		/**
		*  @brief
		*    Creates a tessellation control shader (named "hull shader" in DirectX) and sets the shader source code
		*
		*  @param[in] sSourceCode
		*    Shader source code, usually blank ASCII code
		*  @param[in] sProfile
		*    Shader profile to use, if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*  @param[in] sArguments
		*    Optional shader compiler arguments ("February 2012 version of Cg 3.1": No GLSL tessellation control shader support, at least one that's not using special NVIDIA-only extensions)
		*  @param[in] sEntry
		*    Entry point, if empty string, "main" is used as default
		*
		*  @return
		*    The created tessellation control shader, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateTessellationControlShader()"-method
		*    - "Shader::SetSourceCode()"
		*/
		PLRENDERER_API TessellationControlShader *CreateTessellationControlShader(const PLCore::String &sSourceCode, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "");

		/**
		*  @brief
		*    Creates a tessellation evaluation shader (named "domain shader" in DirectX) and sets the shader source code
		*
		*  @param[in] sSourceCode
		*    Shader source code, usually blank ASCII code
		*  @param[in] sProfile
		*    Shader profile to use, if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*  @param[in] sArguments
		*    Optional shader compiler arguments ("February 2012 version of Cg 3.1": No GLSL tessellation evaluation shader support, at least one that's not using special NVIDIA-only extensions)
		*  @param[in] sEntry
		*    Entry point, if empty string, "main" is used as default
		*
		*  @return
		*    The created tessellation evaluation shader, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateTessellationEvaluationShader()"-method
		*    - "Shader::SetSourceCode()"
		*/
		PLRENDERER_API TessellationEvaluationShader *CreateTessellationEvaluationShader(const PLCore::String &sSourceCode, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "");

		/**
		*  @brief
		*    Creates a geometry shader and sets the shader source code
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
		*    The created geometry shader, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateGeometryShader()"-method
		*    - "GeometryShader::SetSourceCode()"
		*/
		PLRENDERER_API GeometryShader *CreateGeometryShader(const PLCore::String &sSourceCode, GeometryShader::EInputPrimitiveType nInputPrimitiveType, GeometryShader::EOutputPrimitiveType nOutputPrimitiveType, PLCore::uint32 nNumOfOutputVertices, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "");

		/**
		*  @brief
		*    Creates a fragment shader and sets the shader source code
		*
		*  @param[in] sSourceCode
		*    Shader source code, usually blank ASCII code
		*  @param[in] sProfile
		*    Shader profile to use, if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*  @param[in] sArguments
		*    Optional shader compiler arguments, e.g. "version=150" when using Cg and a "glslf" profile
		*  @param[in] sEntry
		*    Entry point, if empty string, "main" is used as default
		*
		*  @return
		*    The created fragment shader, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateFragmentShader()"-method
		*    - "Shader::SetSourceCode()"
		*/
		PLRENDERER_API FragmentShader *CreateFragmentShader(const PLCore::String &sSourceCode, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "");

		/**
		*  @brief
		*    Creates a program and assigns a vertex and fragment shader to it
		*
		*  @param[in] pVertexShader
		*    Vertex shader the program is using, can be a null pointer, vertex shader and program language must match!
		*  @param[in] pFragmentShader
		*    Fragment shader the program is using, can be a null pointer, fragment shader and program language must match!
		*
		*  @return
		*    The created program, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateProgram()"-method
		*    - "Program::SetVertexShader()"
		*    - "Program::SetFragmentShader()"
		*/
		PLRENDERER_API Program *CreateProgram(VertexShader *pVertexShader, FragmentShader *pFragmentShader);

		/**
		*  @brief
		*    Creates a program and assigns a vertex, tessellation control, tessellation evaluation and fragment shader to it
		*
		*  @param[in] pVertexShader
		*    Vertex shader the program is using, can be a null pointer, vertex shader and program language must match!
		*  @param[in] pTessellationControlShader
		*    Tessellation control shader the program is using, can be a null pointer, tessellation control shader and program language must match!
		*  @param[in] pTessellationEvaluationShader
		*    Tessellation evaluation shader the program is using, can be a null pointer, tessellation evaluation shader and program language must match!
		*  @param[in] pFragmentShader
		*    Fragment shader the program is using, can be a null pointer, fragment shader and program language must match!
		*
		*  @return
		*    The created program, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateProgram()"-method
		*    - "Program::SetVertexShader()"
		*    - "Program::SetTessellationControlShader()"
		*    - "Program::SetTessellationEvaluationShader()"
		*    - "Program::SetFragmentShader()"
		*/
		PLRENDERER_API Program *CreateProgram(VertexShader *pVertexShader, TessellationControlShader *pTessellationControlShader, TessellationEvaluationShader *pTessellationEvaluationShader, FragmentShader *pFragmentShader);

		/**
		*  @brief
		*    Creates a program and assigns a vertex, geometry and fragment shader to it
		*
		*  @param[in] pVertexShader
		*    Vertex shader the program is using, can be a null pointer, vertex shader and program language must match!
		*  @param[in] pGeometryShader
		*    Geometry shader the program is using, can be a null pointer, geometry shader and program language must match!
		*  @param[in] pFragmentShader
		*    Fragment shader the program is using, can be a null pointer, fragment shader and program language must match!
		*
		*  @return
		*    The created program, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateProgram()"-method
		*    - "Program::SetVertexShader()"
		*    - "Program::SetGeometryShader()"
		*    - "Program::SetFragmentShader()"
		*/
		PLRENDERER_API Program *CreateProgram(VertexShader *pVertexShader, GeometryShader *pGeometryShader, FragmentShader *pFragmentShader);

		/**
		*  @brief
		*    Creates a program and assigns a vertex, tessellation control, tessellation evaluation, geometry and fragment shader to it
		*
		*  @param[in] pVertexShader
		*    Vertex shader the program is using, can be a null pointer, vertex shader and program language must match!
		*  @param[in] pTessellationControlShader
		*    Tessellation control shader the program is using, can be a null pointer, tessellation control shader and program language must match!
		*  @param[in] pTessellationEvaluationShader
		*    Tessellation evaluation shader the program is using, can be a null pointer, tessellation evaluation shader and program language must match!
		*  @param[in] pGeometryShader
		*    Geometry shader the program is using, can be a null pointer, geometry shader and program language must match!
		*  @param[in] pFragmentShader
		*    Fragment shader the program is using, can be a null pointer, fragment shader and program language must match!
		*
		*  @return
		*    The created program, a null pointer on error
		*
		*  @see
		*    - Virtual "ShaderLanguage::CreateProgram()"-method
		*    - "Program::SetVertexShader()"
		*    - "Program::SetTessellationControlShader()"
		*    - "Program::SetTessellationEvaluationShader()"
		*    - "Program::SetGeometryShader()"
		*    - "Program::SetFragmentShader()"
		*/
		PLRENDERER_API Program *CreateProgram(VertexShader *pVertexShader, TessellationControlShader *pTessellationControlShader, TessellationEvaluationShader *pTessellationEvaluationShader, GeometryShader *pGeometryShader, FragmentShader *pFragmentShader);


	//[-------------------------------------------------------]
	//[ Public virtual ShaderLanguage functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the name of the shader language
		*
		*  @return
		*    The name of the shader language (for example "GLSL" or "Cg")
		*/
		virtual PLCore::String GetShaderLanguage() const = 0;

		/**
		*  @brief
		*    Creates a vertex shader
		*
		*  @return
		*    The created vertex shader, a null pointer on error
		*/
		virtual VertexShader *CreateVertexShader() = 0;

		/**
		*  @brief
		*    Creates a tessellation control shader (named "hull shader" in DirectX)
		*
		*  @return
		*    The created tessellation control shader, a null pointer on error
		*/
		virtual TessellationControlShader *CreateTessellationControlShader() = 0;

		/**
		*  @brief
		*    Creates a tessellation evaluation shader (named "domain shader" in DirectX)
		*
		*  @return
		*    The created tessellation evaluation shader, a null pointer on error
		*/
		virtual TessellationEvaluationShader *CreateTessellationEvaluationShader() = 0;

		/**
		*  @brief
		*    Creates a geometry shader
		*
		*  @return
		*    The created geometry shader, a null pointer on error
		*/
		virtual GeometryShader *CreateGeometryShader() = 0;

		/**
		*  @brief
		*    Creates a fragment shader
		*
		*  @return
		*    The created fragment shader, a null pointer on error
		*/
		virtual FragmentShader *CreateFragmentShader() = 0;

		/**
		*  @brief
		*    Creates a program
		*
		*  @return
		*    The created program, a null pointer on error
		*/
		virtual Program *CreateProgram() = 0;

		/**
		*  @brief
		*    Creates an uniform buffer
		*
		*  @return
		*    The created uniform buffer, a null pointer on error
		*/
		virtual UniformBuffer *CreateUniformBuffer() = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API ShaderLanguage();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~ShaderLanguage();


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
		ShaderLanguage(const ShaderLanguage &cSource);

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
		ShaderLanguage &operator =(const ShaderLanguage &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_SHADERLANGUAGE_H__
