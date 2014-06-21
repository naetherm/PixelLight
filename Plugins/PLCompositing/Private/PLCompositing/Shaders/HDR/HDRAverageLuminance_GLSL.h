/*********************************************************\
 *  File: HDRAverageLuminance_GLSL.h                     *
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


// GLSL vertex shader source code
static const PLCore::String sHDRAverageLuminance_GLSL_VS = "\
// GLSL preprocessor directives\n\
#version 120	// OpenGL 2.1\n\
\n\
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


// Downsample GLSL vertex shader source code
static const PLCore::String sHDRAverageLuminance_GLSL_VS_Downsample = "\
// GLSL preprocessor directives\n\
#version 120	// OpenGL 2.1\n\
\n\
// Attributes\n\
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
varying   vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
\n\
// Uniforms\n\
uniform ivec2 TextureSize;	// Texture size in texel\n\
uniform vec2  Size;			// Size [0..1]\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Convert the incoming clip space vertex position from [-1..1] to [0..1]\n\
	vec2 position = (VertexPosition.xy + vec2(1.0, 1.0))*vec2(0.5, 0.5);\n\
\n\
	// Apply size\n\
	position *= Size;\n\
\n\
	// Write out the vertex position in clip space\n\
	gl_Position = vec4(position*vec2(2.0, 2.0) - vec2(1.0, 1.0), 0.0, 1.0);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	VertexTexCoordVS = VertexPosition.zw*vec2(TextureSize);\n\
}";


// Common GLSL fragment shader source code
static const PLCore::String sHDRAverageLuminance_GLSL_FS_Common = "\
// GLSL preprocessor directives\n\
#version 120	// OpenGL 2.1\n\
\n\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Attributes\n\
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
\n\
// Uniforms\n\
uniform sampler2DRect Texture;	// HDR texture\n\
\n\
// Neighbor offset table\n\
const vec2 Offsets[4] = vec2[4](\n\
	vec2(0.0, 0.0), // Center       0\n\
	vec2(1.0, 0.0), // Right        1\n\
	vec2(1.0, 1.0), // Bottom right 2\n\
	vec2(0.0, 1.0)  // Bottom       3\n\
);\n";


// Downsample log GLSL fragment shader source code
static const PLCore::String sHDRAverageLuminance_GLSL_FS_DownsampleLog = "\
// Uniforms\n\
uniform vec3	LuminanceConvert;	// Convert RGB to luminance\n\
uniform float	Epsilon;			// A small value to avoid undefined log(0)\n\
\n\
// Downsample log main function\n\
void main()\n\
{\n\
	// 'Photographic Tone Reproduction for Digital Images': Formula 1\n\
	float luminance = 0.0;\n\
	for (int i=0; i<4; i++) {\n\
		// If, for any reason, there's an invalid value it would mess up everything... so, security is really a must have in here!\n\
		float value = log(dot(texture2DRect(Texture, VertexTexCoordVS + Offsets[i]).rgb, LuminanceConvert) + Epsilon);\n\
		if (value == value)	// IEEE standard: NaN != NaN - I don't use isnan so I can use lower shader versions\n\
			luminance += value;\n\
	}\n\
\n\
	// Write down the result\n\
	float value = luminance*0.25;\n\
	gl_FragColor = vec4(value, value, value, value);\n\
}";


// Downsample GLSL fragment shader source code
static const PLCore::String sHDRAverageLuminance_GLSL_FS_Downsample = "\
// Downsample main function\n\
void main()\n\
{\n\
	float luminance = 0.0;\n\
	for (int i=0; i<4; i++)\n\
		luminance += texture2DRect(Texture, VertexTexCoordVS + Offsets[i]).r;\n\
	float value = luminance*0.25;\n\
	gl_FragColor = vec4(value, value, value, value);\n\
}";


// Downsample exp GLSL fragment shader source code
static const PLCore::String sHDRAverageLuminance_GLSL_FS_DownsampleExp = "\
// Downsample exp main function\n\
void main()\n\
{\n\
	float luminance = 0.0;\n\
	for (int i=0; i<4; i++)\n\
		luminance += texture2DRect(Texture, VertexTexCoordVS + Offsets[i]).r;\n\
	float value = exp(luminance*0.25);\n\
	gl_FragColor = vec4(value, value, value, value);\n\
}";
