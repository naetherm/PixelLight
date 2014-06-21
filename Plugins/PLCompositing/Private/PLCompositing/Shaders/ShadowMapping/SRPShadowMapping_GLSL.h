/*********************************************************\
 *  File: SRPShadowMapping_GLSL.h                        *
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


// OpenGL 2.0 ("#version 110") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sSRPShadowMapping_GLSL_VS = "\
// Attributes\n\
attribute highp vec4 VertexPosition;		// Object space vertex position input\n\
varying   highp vec3 LightVecVS;\n\
#ifdef VS_TEXCOORD0\n\
	varying highp vec2 VertexTexCoord0;		// Vertex texture coordinate 0 input\n\
	varying highp vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output\n\
#endif\n\
\n\
// Uniforms\n\
uniform highp mat4  WorldVP;	// World view projection matrix\n\
uniform highp mat4  WorldV;		// World view matrix\n\
uniform highp float InvRadius;	// World space inverse light radius\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Transform vertex to clip space, lower/left is (-1,-1) and upper/right is (1,1)\n\
	gl_Position = WorldVP*VertexPosition;\n\
\n\
	// Calculate the world space light vector (the light position is the origin)\n\
	LightVecVS = -InvRadius*(WorldV*VertexPosition).xyz;\n\
\n\
#ifdef VS_DIFFUSEMAP\n\
	// Pass through the vertex texture coordinate 0\n\
	VertexTexCoordVS = VertexTexCoord0;\n\
#endif\n\
}";


// OpenGL 2.0 ("#version 110") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sSRPShadowMapping_GLSL_FS = "\
// Attributes\n\
varying highp vec3 LightVecVS;\n\
#ifdef FS_ALPHATEST\n\
	varying highp vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader\n\
#endif\n\
\n\
// Uniforms\n\
#ifdef FS_ALPHATEST\n\
	uniform highp sampler2D DiffuseMap;\n\
	uniform highp float    AlphaReference;\n\
#endif\n\
\n\
// Programs\n\
void main()\n\
{\n\
	#ifdef FS_ALPHATEST\n\
		// Fetch the required texel data\n\
		highp vec4 sample = texture2D(DiffuseMap, VertexTexCoordVS);\n\
\n\
		// Perform an alpha test to discard fragments\n\
		if (sample.a < AlphaReference)\n\
			discard; // Throw the fragment away and don't draw it!\n\
	#endif\n\
\n\
	// + '0.005' => Set polygon offset to avoid nasty shadow artefacts\n\
	highp vec4 pack = vec4(1.0, 256.0, 65536.0, 16777216.0);\n\
	gl_FragColor = pack * (length(LightVecVS) + 0.005);\n\
}";
