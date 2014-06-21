/*********************************************************\
 *  File: DrawHelpersBackendShaders_GLSL.cpp             *
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


// GLSL (OpenGL 2.0 ("#version 110") and OpenGL ES 2.0 ("#version 100")) vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sVertexShaderSourceCodeGLSL = "\
// Attributes\n\
attribute mediump vec3 VertexPosition;					// Object space vertex position input\n\
#ifdef VS_TEXCOORD0\n\
	attribute mediump vec2 VertexTextureCoordinate;		// Vertex texture coordinate input\n\
	varying   mediump vec2 VertexTextureCoordinateVS;	// Vertex texture coordinate output\n\
#endif\n\
#ifdef VS_VERTEXCOLOR\n\
	attribute mediump vec4 VertexColor;					// Vertex color input\n\
	varying   mediump vec4 VertexColorVS;				// Vertex color output\n\
#endif\n\
\n\
// Uniforms\n\
uniform mediump mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix\n\
#ifdef VS_TEXCOORD0\n\
	uniform mediump mat4 TextureMatrix;				// Texture matrix\n\
#endif\n\
#ifdef VS_POINTSIZE\n\
	uniform mediump float PointSize;				// Point size\n\
#endif\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	gl_Position = ObjectSpaceToClipSpaceMatrix*vec4(VertexPosition, 1);\n\
\n\
	#ifdef VS_TEXCOORD0\n\
		// Pass through the texture coordinate\n\
		VertexTextureCoordinateVS = (TextureMatrix*vec4(VertexTextureCoordinate, 1, 1)).xy;\n\
	#endif\n\
\n\
	#ifdef VS_VERTEXCOLOR\n\
		// Pass through the color\n\
		VertexColorVS = VertexColor;\n\
	#endif\n\
\n\
	#ifdef VS_POINTSIZE\n\
		// Write the point size\n\
		gl_PointSize = PointSize;\n\
	#endif\n\
}";


// GLSL (OpenGL 2.0 ("#version 110") and OpenGL ES 2.0 ("#version 100")) fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sFragmentShaderSourceCodeGLSL = "\
// Attributes\n\
#ifdef FS_DIFFUSEMAP\n\
	varying mediump vec2 VertexTextureCoordinateVS;	// Vertex texture coordinate input from vertex shader\n\
#endif\n\
#ifdef FS_VERTEXCOLOR\n\
	varying mediump vec4 VertexColorVS;				// Vertex color input from vertex shader\n\
#endif\n\
\n\
// Uniforms\n\
#ifndef FS_VERTEXCOLOR\n\
	uniform lowp vec4 Color;					// Color\n\
#endif\n\
#ifdef FS_DIFFUSEMAP\n\
	#ifdef FS_DIFFUSEMAP2D\n\
		uniform lowp sampler2D DiffuseMap;		// 2D diffuse map\n\
	#else\n\
		uniform lowp sampler2DRect DiffuseMap;	// Rectangle diffuse map\n\
	#endif\n\
	#ifdef FS_ALPHATEST\n\
		uniform mediump float AlphaReference;	// Alpha reference\n\
	#endif\n\
#endif\n\
\n\
// Programs\n\
void main()\n\
{\n\
#ifdef FS_DIFFUSEMAP\n\
	#ifdef FS_DIFFUSEMAP2D\n\
		lowp vec4 diffuseColor = texture2D(DiffuseMap, VertexTextureCoordinateVS);\n\
	#else\n\
		lowp vec4 diffuseColor = texture2DRect(DiffuseMap, VertexTextureCoordinateVS);\n\
	#endif\n\
	#ifdef FS_ALPHATEST\n\
		// Perform an alpha test to discard fragments\n\
		if (diffuseColor.a < AlphaReference)\n\
			discard; // Throw the fragment away and don't draw it!\n\
	#endif\n\
\n\
	// Still here? Then apply the given color...\n\
	#ifdef FS_VERTEXCOLOR\n\
		gl_FragColor = diffuseColor*VertexColorVS;\n\
	#else\n\
		gl_FragColor = diffuseColor*Color;\n\
	#endif\n\
#else\n\
	#ifdef FS_VERTEXCOLOR\n\
		gl_FragColor = VertexColorVS;\n\
	#else\n\
		gl_FragColor = Color;\n\
	#endif\n\
#endif\n\
}";
