/*********************************************************\
 *  File: SRPEndHDR_Cg.h                                 *
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


// Cg vertex shader source code
static const PLCore::String sEndHDR_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;			// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	#ifdef VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
		float3 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>) + average luminance within the z component\n\
	#else\n\
		float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
	#endif\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float4 VertexPosition : POSITION	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
												// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
  , uniform int2	  TextureSize				// Texture size in texel\n\
	#ifdef VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
  , uniform sampler2D AverageLuminanceTexture	// Automatic average luminance texture\n\
	#endif\n\
	)\n\
{\n\
	VS_OUTPUT OUT;\n\
\n\
	// Pass through the vertex position\n\
	OUT.Position = float4(VertexPosition.xy, 0, 1);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	OUT.TexCoord.xy = VertexPosition.zw*TextureSize;\n\
\n\
	// Get the average luminance by using vertex texture fetch so we have just 4 instead of xxxx average luminance texture accesses when doing this within the fragment shader\n\
	#ifdef VS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
		OUT.TexCoord.z = tex2D(AverageLuminanceTexture, float2(0.5f, 0.5f)).r;\n\
	#endif\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Cg fragment shader source code
static const PLCore::String sEndHDR_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;			// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
		float3 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>) + average luminance within the z component\n\
	#else\n\
		float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
	#endif\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Programs\n\
FS_OUTPUT main(VS_OUTPUT   IN						// Interpolated output from the vertex stage\n\
#ifdef FS_TONE_MAPPING\n\
	 , uniform float3      LuminanceConvert			// Luminance convert\n\
	 , uniform float       Key						// Key, must be >=0\n\
	 , uniform float       WhiteLevel				// White level, must be >=0\n\
	#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE\n\
		#ifndef FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
			, uniform sampler2D AverageLuminanceTexture	// Automatic average luminance texture\n\
		#endif\n\
	#else\n\
	 , uniform float       AverageLuminance			// User set average luminance\n\
	#endif\n\
#endif\n\
#ifdef FS_BLOOM\n\
	 , uniform float       BloomFactor				// Bloom factor\n\
	 , uniform float       BloomDownscale			// Bloom downscale\n\
	 , uniform samplerRECT BloomTexture				// Bloom texture\n\
#endif\n\
#ifdef FS_GAMMA_CORRECTION\n\
	 , uniform float       InvGamma					// Inversed gamma correction value, must be >0\n\
#endif\n\
	 , uniform samplerRECT HDRTexture)				// HDR texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Fetch the required texel data\n\
	float4 sample = texRECT(HDRTexture, IN.TexCoord.xy);\n\
\n\
	// Perform tone mapping\n\
#ifdef FS_TONE_MAPPING\n\
	// Convert RGB to luminance\n\
	float pixelLuminance = dot(sample.rgb, LuminanceConvert);\n\
\n\
	// Get the average luminance\n\
	#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE\n\
		#ifdef FS_AUTOMATIC_AVERAGE_LUMINANCE_VTF\n\
			#define averageLuminance IN.TexCoord.z\n\
		#else\n\
			float averageLuminance = tex2D(AverageLuminanceTexture, float2(0.5f, 0.5f)).r;\n\
		#endif\n\
	#else\n\
		#define averageLuminance AverageLuminance\n\
	#endif\n\
\n\
	// Use the Reinhard global tone map operator to compute the scaled luminance\n\
	float keyOverLuminance = (averageLuminance > 0.0f) ? Key/averageLuminance : 0.0f;\n\
	// 'Photographic Tone Reproduction for Digital Images': Formula 2\n\
	float scaledLuminance = keyOverLuminance*pixelLuminance;\n\
	// 'Photographic Tone Reproduction for Digital Images': Formula 4\n\
	scaledLuminance = (scaledLuminance*(1 + (scaledLuminance/pow(keyOverLuminance*WhiteLevel, 2)))) / (1 + scaledLuminance);\n\
\n\
	// Set LDR color\n\
	OUT.Color0 = float4(clamp(sample.rgb*scaledLuminance, 0.0f, 1.0f), sample.a);\n\
#else\n\
	// Just copy over the color\n\
	OUT.Color0 = sample;\n\
#endif\n\
\n\
	// Add bloom\n\
#ifdef FS_BLOOM\n\
	OUT.Color0 = clamp(OUT.Color0 + texRECT(BloomTexture, IN.TexCoord.xy/BloomDownscale)*BloomFactor, 0.0f, 1.0f);\n\
#endif\n\
\n\
	// Perform gamma correction (linear space -> sRGB space)\n\
#ifdef FS_GAMMA_CORRECTION\n\
	OUT.Color0.rgb = pow(OUT.Color0.rgb, InvGamma);\n\
#endif\n\
\n\
	// Done\n\
	return OUT;\n\
}";
