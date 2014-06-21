/*********************************************************\
 *  File: Shader.h                                       *
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


#ifndef __PLRENDERER_SHADER_H__
#define __PLRENDERER_SHADER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLRenderer/Renderer/Resource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer shader resource
*/
class Shader : public Resource {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Removes precision qualifiers from the given GLSL shader source code
		*
		*  @param[in] sSourceCode
		*    Shader source code, usually blank ASCII code
		*
		*  @return
		*    The modified GLSL shader source code
		*
		*  @remarks
		*    From the "The OpenGL® Shading Language" specification "Language Version: 3.30, Document Revision: 6, 11-Mar-2010"
		*        "Precision qualifiers are added for code portability with OpenGL ES, not for functionality. They have the
		*         same syntax as in OpenGL ES, as described below, but they have no semantic meaning, which includes no
		*         effect on the precision used to store or operate on variables."
		*    Although the precision qualifiers "should" have no effect when not using OpenGL ES, we noticed that some NVIDIA GPU
		*    drivers produced compiler errors when using precision qualifiers. Due this buggy behavior, it's recommended to
		*    remove the precision qualifiers before passing on the shader source code to OpenGL.
		*/
		static PLRENDERER_API PLCore::String RemovePrecisionQualifiersFromGLSL(const PLCore::String &sSourceCode);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Shader();


	//[-------------------------------------------------------]
	//[ Public virtual Shader functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the name of the shader language the shader is using
		*
		*  @return
		*    The name of the shader language the shader is using (for example "GLSL" or "Cg")
		*/
		virtual PLCore::String GetShaderLanguage() const = 0;

		/**
		*  @brief
		*    Returns the shader source code
		*
		*  @return
		*    The shader source code
		*/
		virtual PLCore::String GetSourceCode() const = 0;

		/**
		*  @brief
		*    Returns the name of the shader profile the shader is using
		*
		*  @return
		*    The name of the shader profile the shader is using (for example "arbvp1")
		*/
		virtual PLCore::String GetProfile() const = 0;

		/**
		*  @brief
		*    Returns the optional shader compiler arguments the shader is using
		*
		*  @return
		*    The optional shader compiler arguments the shader is using (for example "version=150" when using Cg and a "glslv"/"glslg"/"glslf" profile)
		*/
		virtual PLCore::String GetArguments() const = 0;

		/**
		*  @brief
		*    Returns the name of the shader entry point the shader is using
		*
		*  @return
		*    The name of the shader entry point the shader is using (for example "main")
		*/
		virtual PLCore::String GetEntry() const = 0;

		/**
		*  @brief
		*    Sets the shader source code
		*
		*  @param[in] sSourceCode
		*    Shader source code, usually blank ASCII code
		*  @param[in] sProfile
		*    Shader profile to use, if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*  @param[in] sArguments
		*    Optional shader compiler arguments, e.g. "version=150" when using Cg and a "glslv"/"glslg"/"glslf" profile
		*  @param[in] sEntry
		*    Entry point, if empty string, "main" is used as default
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    'sProfile' is not supported by each shader-API and is in general shader-API dependent. GLSL doesn't have such
		*    profiles, just something named "version" - one has to directly write into the shader. But even when this information
		*    is not used for compiling the GLSL shader, we highly recommend to provide GLSL version information in the form of e.g.
		*    "130" for OpenGL 3.0 shaders ("#version 130"). Within Cg, a basic vertex profile may be "arbvp1" and a basic
		*    fragment profile "arbfp1". Cg also provides GLSL profiles: "glslv" for vertex shader, "glslg" for geometry shader and
		*    "glslf" for fragment shader.
		*    Please note that the profile is just a hint, if necessary, the implementation is free to choose another profile.
		*    In general, be carefully when explicitly setting a profile - when using Cg, and one of the shaders, a GPU program
		*    is composed of, is a GLSL profile like "glslv", all other shaders must use GLSL profiles as well!
		*
		*   'sEntry' is not supported by each shader-API. GLSL doesn't have such an user defined entry point and the main
		*   function must always be "main". Cg supports entry points with names other than "main".
		*
		*   Look out! When working with shaders you have to be prepared that a shader may work on one system, but fails to even
		*   compile on another one. Sadly, even if there are e.g. official GLSL specifications, you CAN'T be sure that every
		*   GPU driver is implementing them in detail. Here are some pitfalls which already produced some headaches...
		*
		*   When using GLSL, don't forget to provide the #version directive! Quote from
		*     "The OpenGL® Shading Language - Language Version: 3.30 - Document Revision: 6 - 11-Mar-2010" Page 14
		*       "Version 1.10 of the language does not require shaders to include this directive, and shaders that do not include
		*        a #version directive will be treated as targeting version 1.10."
		*   It looks like that AMD/ATI drivers ("AMD Catalyst™ 11.3") are NOT using just version 1.10 if there's no #version directive, but a higher
		*   version... so don't trust your GPU driver when your GLSL code, using modern language features, also works for you without
		*   #version directive, because it may not on other systems! OpenGL version and GLSL version can be a bit confusing, so
		*   here's a version table:
		*     GLSL #version    OpenGL version    Some comments
		*     110              2.0
		*     120              2.1
		*     130              3.0               Precision qualifiers added
		*                                        "attribute" deprecated; linkage between a vertex shader and OpenGL for per-vertex data -> use "in" instead
		*                                        "varying"/"centroid varying" deprecated; linkage between a vertex shader and a fragment shader for interpolated data -> use "in"/"out" instead
		*     140              3.1
		*     150              3.2               Almost feature-identical to Direct3D Shader Model 4.0 (DirectX version 10), geometry shader added
		*     330              3.3               Equivalent to Direct3D Shader Model 4.0 (DirectX version 10)
		*     400              4.0               Tessellation control ("Hull"-Shader in DirectX 11) and evaluation ("Domain"-Shader in DirectX 11) shaders added
		*     410              4.1
		*     420              4.2               Equivalent to Direct3D Shader Model 5.0 (DirectX version 11)
		*  #version must occur before any other statement in the program as stated within:
		*    "The OpenGL® Shading Language - Language Version: 3.30 - Document Revision: 6 - 11-Mar-2010" Page 15
		*      "The #version directive must occur in a shader before anything else, except for comments and white space."
		*  ... sadly, this time NVIDIA (driver: "266.58 WHQL") is not implementing the specification in detail and while on AMD/ATI drivers ("AMD Catalyst™ 11.3")
		*  you get the error message "error(#105) #version must occur before any other statement in the program" when breaking specification,
		*  NVIDIA just accepts it without any error.
		*
		*  Enough on GLSL - now to Cg. Sadly, in general Cg is on AMD/ATI GPU's just poor due to the lack of modern profiles. When using Cg on none NVIDIA
		*  GPU's you have virtually no other change then using the GLSL profiles in order to write shaders using modern features. While the concept of Cg is
		*  fantastic, this lack of modern none NVIDIA profiles destroys many of Cg's advantages...
		*/
		virtual bool SetSourceCode(const PLCore::String &sSourceCode, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "") = 0;


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
		*  @param[in] nType
		*    Resource type
		*/
		PLRENDERER_API Shader(Renderer &cRenderer, EType nType);


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
		Shader(const Shader &cSource);

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
		Shader &operator =(const Shader &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_SHADER_H__
