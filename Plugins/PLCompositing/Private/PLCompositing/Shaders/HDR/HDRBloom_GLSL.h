/*********************************************************\
 *  File: HDRBloom_GLSL.h                                *
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


// Downsample and bloom OpenGL 2.1 ("#version 120") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator" or by hand
static const PLCore::String sHDRBloom_GLSL_VS = "\
// Attributes\n\
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
#ifdef VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
	varying vec3 VertexTexCoordVS;	// Vertex texture coordinate 0 + average luminance within the z component output, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
#else\n\
	varying vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
#endif\n\
\n\
// Uniforms\n\
uniform ivec2 TextureSize;						// Texture size in texel\n\
#ifdef VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
	uniform sampler2D AverageLuminanceTexture;	// Automatic average luminance texture\n\
#endif\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Set the clip space vertex position\n\
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	VertexTexCoordVS.xy = VertexPosition.zw*vec2(TextureSize);\n\
\n\
	// Get the average luminance by using vertex texture fetch so we have just 4 instead of xxxx average luminance texture accesses when doing this within the fragment shader\n\
	#ifdef VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
		VertexTexCoordVS.z = texture2D(AverageLuminanceTexture, vec2(0.5, 0.5)).r;\n\
	#endif\n\
}";


// Downsample OpenGL 2.1 ("#version 120") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sHDRBloom_GLSL_FS_Downscale = "\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Attributes\n\
#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
	varying vec3 VertexTexCoordVS;	// Vertex texture coordinate 0 + average luminance within the z component input from vertex shader, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
#else\n\
	varying vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 input from vertex shader, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
#endif\n\
\n\
// Uniforms\n\
#ifdef FS_TONE_MAPPING\n\
	uniform vec3			LuminanceConvert;	// Luminance convert\n\
	uniform float			Key;				// Key, must be >=0\n\
	uniform float			WhiteLevel;			// White level, must be >=0\n\
	#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE\n\
		#ifndef FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
			uniform sampler2D	AverageLuminanceTexture;	// Automatic average luminance texture\n\
		#endif\n\
	#else\n\
		uniform float		AverageLuminance;	// User set average luminance\n\
	#endif\n\
#endif\n\
uniform float			BrightThreshold;		// Bright threshold\n\
uniform sampler2DRect	HDRTexture;				// HDR texture\n\
\n\
// Neighbor offset table\n\
const vec2 Offsets[16] = vec2[16](\n\
	vec2( 1.5, -1.5), // 0\n\
	vec2( 1.5, -0.5), // 1\n\
	vec2( 1.5,  0.5), // 2\n\
	vec2( 1.5,  1.5), // 3\n\
	vec2( 0.5, -1.5), // 4\n\
	vec2( 0.5, -0.5), // 5\n\
	vec2( 0.5,  0.5), // 6\n\
	vec2( 0.5,  1.5), // 7\n\
	vec2(-0.5, -1.5), // 8\n\
	vec2(-0.5, -0.5), // 9\n\
	vec2(-0.5,  0.5), // 10\n\
	vec2(-0.5,  1.5), // 11\n\
	vec2(-1.5, -1.5), // 12\n\
	vec2(-1.5, -0.5), // 13\n\
	vec2(-1.5,  0.5), // 14\n\
	vec2(-1.5,  1.5)  // 15\n\
);\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Downsample\n\
	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n\
	for (int i=0; i<16; i++)\n\
		color += texture2DRect(HDRTexture, VertexTexCoordVS.xy + Offsets[i]);\n\
	gl_FragColor = color*(1.0/16.0);\n\
\n\
    // The rest is for tone mapping - nearly same code as in 'SRPEndHDR_GLSL'\n\
\n\
	// Perform tone mapping\n\
#ifdef FS_TONE_MAPPING\n\
	// Convert RGB to luminance\n\
	float pixelLuminance = dot(gl_FragColor.rgb, LuminanceConvert);\n\
\n\
	// Get the average luminance\n\
	#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE\n\
		#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
			#define averageLuminance VertexTexCoordVS.z\n\
		#else\n\
			float averageLuminance = texture2D(AverageLuminanceTexture, vec2(0.5, 0.5)).r;\n\
		#endif\n\
	#else\n\
		#define averageLuminance AverageLuminance\n\
	#endif\n\
\n\
	// Use the Reinhard global tone map operator to compute the scaled luminance\n\
	float keyOverLuminance = (averageLuminance > 0.0) ? Key/averageLuminance : 0.0;\n\
	// 'Photographic Tone Reproduction for Digital Images': Formula 2\n\
	float scaledLuminance = keyOverLuminance*pixelLuminance;\n\
	// 'Photographic Tone Reproduction for Digital Images': Formula 4\n\
	scaledLuminance = (scaledLuminance*(1.0 + (scaledLuminance/pow(keyOverLuminance*WhiteLevel, 2.0)))) / (1.0 + scaledLuminance);\n\
\n\
	// Set LDR color\n\
	gl_FragColor.rgb = clamp(gl_FragColor.rgb*scaledLuminance, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));\n\
#endif\n\
\n\
	// Bright pass - do this 'after' tone mapping, just looks better\n\
	gl_FragColor = max(vec4(0.0, 0.0, 0.0, 0.0), gl_FragColor - vec4(BrightThreshold, BrightThreshold, BrightThreshold, BrightThreshold));\n\
}";


// Bloom GLSL vertex shader source code
static const PLCore::String sHDRBloom_GLSL_FS = "\
// GLSL preprocessor directives\n\
#version 120	// OpenGL 2.1\n\
\n\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Attributes\n\
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 input from vertex shader\n\
\n\
// Uniforms\n\
uniform vec2			UVScale;	// UV scale\n\
uniform sampler2DRect	HDRTexture;	// HDR texture\n\
\n\
// Neighbor offset table\n\
const vec2 Offsets[13] = vec2[13](\n\
	vec2( 6.0,  6.0), // 0\n\
	vec2( 5.0,  5.0), // 1\n\
	vec2( 4.0,  4.0), // 2\n\
	vec2( 3.0,  3.0), // 3\n\
	vec2( 2.0,  2.0), // 4\n\
	vec2( 1.0,  1.0), // 5\n\
	vec2( 0.0,  0.0), // 6\n\
	vec2(-1.0, -1.0), // 7\n\
	vec2(-2.0, -2.0), // 8\n\
	vec2(-3.0, -3.0), // 9\n\
	vec2(-4.0, -4.0), // 10\n\
	vec2(-5.0, -5.0), // 11\n\
	vec2(-6.0, -6.0)  // 12\n\
);\n\
const float Weights[13] = float[13](\n\
	0.002216, // 0\n\
	0.008764, // 1\n\
	0.026995, // 2\n\
	0.064759, // 3\n\
	0.120985, // 4\n\
	0.176033, // 5\n\
	0.199471, // 6\n\
	0.176033, // 7\n\
	0.120985, // 8\n\
	0.064759, // 9\n\
	0.026995, // 10\n\
	0.008764, // 11\n\
	0.002216  // 12\n\
);\n\
\n\
// Programs\n\
void main()\n\
{\n\
	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);\n\
	for (int i=0; i<13; i++)\n\
		gl_FragColor += texture2DRect(HDRTexture, VertexTexCoordVS + Offsets[i]*UVScale)*Weights[i];\n\
}";
