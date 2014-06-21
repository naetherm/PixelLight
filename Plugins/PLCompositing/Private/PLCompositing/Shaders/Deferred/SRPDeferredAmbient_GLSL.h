/*********************************************************\
 *  File: SRPDeferredAmbient_GLSL.h                      *
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
static const PLCore::String sDeferredAmbient_GLSL_VS = "\
// Attributes\n\
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
varying   vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
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
static const PLCore::String sDeferredAmbient_GLSL_FS = "\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Attributes\n\
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
\n\
// Uniforms\n\
uniform vec3		   AmbientColor;	// Ambient color\n\
uniform sampler2DRect AlbedoMap;		// Albedo texture\n\
#ifdef FS_SELFILLUMINATION\n\
	uniform sampler2DRect SelfIlluminationMap;	// Self illumination texture\n\
#endif\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Fetch the required texel data\n\
	vec4 sample = texture2DRect(AlbedoMap, VertexTexCoordVS);\n\
\n\
	// Apply albedo and ambient color\n\
#ifdef FS_ALBEDO\n\
	gl_FragColor.rgb = sample.rgb*AmbientColor;\n\
#else\n\
	gl_FragColor.rgb = AmbientColor;\n\
#endif\n\
\n\
	// Apply ambient occlusion\n\
#ifdef FS_AMBIENTOCCLUSION\n\
	gl_FragColor.rgb *= sample.a;\n\
#endif\n\
\n\
	// Apply self illumination\n\
#ifdef FS_SELFILLUMINATION\n\
	gl_FragColor.rgb += texture2DRect(SelfIlluminationMap, VertexTexCoordVS).rgb;\n\
#endif\n\
\n\
	// Alpha is always 1\n\
	gl_FragColor.a = 1.0;\n\
}";
