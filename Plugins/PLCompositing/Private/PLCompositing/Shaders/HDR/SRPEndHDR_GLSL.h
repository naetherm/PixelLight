/*********************************************************\
 *  File: SRPEndHDR_GLSL.h                               *
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
static const PLCore::String sEndHDR_GLSL_VS = "\
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


// OpenGL 2.0 ("#version 110") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sEndHDR_GLSL_FS = "\
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
	uniform vec3			LuminanceConvert;				// Luminance convert\n\
	uniform float			Key;							// Key, must be >=0\n\
	uniform float			WhiteLevel;						// White level, must be >=0\n\
	#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE\n\
		#ifndef FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
			uniform sampler2D	AverageLuminanceTexture;	// Automatic average luminance texture\n\
		#endif\n\
	#else\n\
		uniform float		AverageLuminance;				// User set average luminance\n\
	#endif\n\
#endif\n\
#ifdef FS_BLOOM\n\
	uniform float			BloomFactor;					// Bloom factor\n\
	uniform float			BloomDownscale;					// Bloom downscale\n\
	uniform sampler2DRect	BloomTexture;					// Bloom texture\n\
#endif\n\
#ifdef FS_GAMMA_CORRECTION\n\
	uniform float			InvGamma;						// Inversed gamma correction value, must be >0\n\
#endif\n\
uniform sampler2DRect		HDRTexture;						// HDR texture\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Fetch the required texel data\n\
	vec4 sample = texture2DRect(HDRTexture, VertexTexCoordVS.xy);\n\
\n\
	// Perform tone mapping\n\
#ifdef FS_TONE_MAPPING\n\
	// Convert RGB to luminance\n\
	float pixelLuminance = dot(sample.rgb, LuminanceConvert);\n\
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
	gl_FragColor = vec4(clamp(sample.rgb*scaledLuminance, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0)), sample.a);\n\
#else\n\
	// Just copy over the color\n\
	gl_FragColor = sample;\n\
#endif\n\
\n\
	// Add bloom\n\
#ifdef FS_BLOOM\n\
	gl_FragColor = clamp(gl_FragColor + texture2DRect(BloomTexture, VertexTexCoordVS.xy/BloomDownscale)*BloomFactor, 0.0, 1.0);\n\
#endif\n\
\n\
	// Perform gamma correction (linear space -> sRGB space)\n\
#ifdef FS_GAMMA_CORRECTION\n\
	gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(InvGamma, InvGamma, InvGamma));\n\
#endif\n\
}";
