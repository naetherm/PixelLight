/*********************************************************\
 *  File: SRPDeferredDepthFog_GLSL.h                     *
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


// OpenGL 2.0 ("#version 110") vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredDepthFog_GLSL_VS = "\
// Attributes\n\
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
varying   vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output\n\
\n\
// Uniforms\n\
uniform ivec2 TextureSize;	// Texture size in texel\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Set the clip space vertex position\n\
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	VertexTexCoordVS = VertexPosition.zw*vec2(TextureSize);\n\
}";


// OpenGL 2.0 ("#version 110") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredDepthFog_GLSL_FS = "\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Attributes\n\
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader\n\
\n\
// Uniforms\n\
uniform float FarPlane;					// Far clipping plane (never 0!)\n\
uniform vec4  FogColor;					// Fog color\n\
#ifdef FS_LINEAR_MODE\n\
	uniform float FogEnd;				// Fog end\n\
	uniform float FogRange;				// Fog end - fog start (never 0!)\n\
#else\n\
	uniform float FogDensity;			// Fog density\n\
#endif\n\
uniform sampler2DRect NormalDepthMap;	// Normal depth texture\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Get the depth\n\
	float depth = texture2DRect(NormalDepthMap, VertexTexCoordVS).b/FarPlane;\n\
\n\
	// Calculate the fog intensity [0...1] = [full fog...no fog]\n\
	#define E 2.71828 // Natural logarithm\n\
#ifdef FS_LINEAR_MODE\n\
	float fogFactor = (FogEnd - depth)/FogRange;\n\
#elif defined FS_EXPONENTIAL_MODE\n\
	// The pow parameters are safe, so no NAN can be produced in here\n\
	float fogFactor = 1.0/pow(E, depth*FogDensity);\n\
#elif defined FS_EXPONENTIAL2_MODE\n\
	// The pow parameters are safe, so no NAN can be produced in here\n\
	float fogFactor = 1.0/pow(E, depth*depth*FogDensity*FogDensity);\n\
#else\n\
	float fogFactor = 0.0;\n\
#endif\n\
\n\
	// Apply the fog color\n\
	gl_FragColor = (1.0 - clamp(fogFactor, 0.0, 1.0))*FogColor;\n\
}";
