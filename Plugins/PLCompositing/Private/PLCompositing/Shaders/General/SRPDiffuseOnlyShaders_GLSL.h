/*********************************************************\
 *  File: SRPDiffuseOnlyShaders_GLSL.h                   *
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
static const PLCore::String sDiffuseOnly_GLSL_VS = "\
// Attributes\n\
attribute mediump vec4 VertexPosition;			// Object space vertex position input\n\
#ifdef VS_TEXCOORD0\n\
	attribute mediump vec2 VertexTexCoord0;		// Vertex texture coordinate 0 input\n\
	varying   mediump vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output\n\
#endif\n\
\n\
// Uniforms\n\
uniform mediump mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	gl_Position = ObjectSpaceToClipSpaceMatrix*VertexPosition;\n\
\n\
#ifdef VS_TEXCOORD0\n\
	// Pass through the vertex texture coordinate 0\n\
	VertexTexCoordVS = VertexTexCoord0;\n\
#endif\n\
}";


// GLSL (OpenGL 2.0 ("#version 110") and OpenGL ES 2.0 ("#version 100")) fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDiffuseOnly_GLSL_FS = "\
// Attributes\n\
#ifdef FS_DIFFUSEMAP\n\
	varying mediump vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader\n\
#endif\n\
\n\
// Uniforms\n\
uniform lowp vec4 DiffuseColor;	// Alpha stores the opacity\n\
#ifdef FS_DIFFUSEMAP\n\
	uniform lowp sampler2D DiffuseMap;\n\
	#ifdef FS_ALPHATEST\n\
		uniform lowp float AlphaReference;\n\
	#endif\n\
#endif\n\
\n\
// Programs\n\
void main()\n\
{\n\
#ifdef FS_DIFFUSEMAP\n\
	gl_FragColor = texture2D(DiffuseMap, VertexTexCoordVS);\n\
	#ifdef FS_ALPHATEST\n\
		// Perform an alpha test to discard fragments\n\
		if (gl_FragColor.a < AlphaReference)\n\
			discard; // Throw the fragment away and don't draw it!\n\
	#endif\n\
	#ifdef FS_GAMMACORRECTION\n\
		gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(2.2, 2.2, 2.2));	// Perform sRGB to linear space conversion (gamma correction)\n\
	#endif\n\
	gl_FragColor *= DiffuseColor;\n\
#else\n\
	gl_FragColor = DiffuseColor;\n\
#endif\n\
}";
