/*********************************************************\
 *  File: SRPDeferredDOF_Cg.h                            *
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
static const PLCore::String sDeferredDOF_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float4 VertexPosition : POSITION,	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
	   uniform int2	  TextureSize)					// Texture size in texel\n\
{\n\
	VS_OUTPUT OUT;\n\
\n\
	// Pass through the vertex position\n\
	OUT.Position = float4(VertexPosition.xy, 0, 1);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	OUT.TexCoord = VertexPosition.zw*TextureSize;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Depth blur Cg fragment shader source code
static const PLCore::String sDeferredDOF_Cg_FS_DepthBlur = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR;\n\
};\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT   IN					// Interpolated output from the vertex stage\n\
	 , uniform float4      DOFParams			// DOFParams coefficients\n\
												// x = near blur depth (!= y); y = focal plane depth; z = far blur depth (!= y))\n\
												// w = blurriness cutoff constant for objects behind the focal plane\n\
	 , uniform samplerRECT RGBTexture			// RGB texture\n\
	 , uniform samplerRECT NormalDepthTexture)	// Normal depth texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Fetch the required texel RGB data and pass it through\n\
	OUT.Color0.rgb = texRECT(RGBTexture, IN.TexCoord).rgb;\n\
\n\
	// Fetch the required texel depth data\n\
	float depth = texRECT(NormalDepthTexture, IN.TexCoord).b;\n\
\n\
	// Compute depth blur\n\
	float f;\n\
	if (depth < DOFParams.y) {\n\
		// Scale depth value between near blur distance and focal distance to [-1, 0] range\n\
		f = (depth - DOFParams.y)/(DOFParams.y - DOFParams.x);\n\
	} else {\n\
		// Scale depth value between focal distance and far blur distance to [0, 1] range\n\
		f = (depth - DOFParams.y)/(DOFParams.z - DOFParams.y);\n\
\n\
		// Clamp the far blur to a maximum blurriness\n\
		f = clamp(f, 0, DOFParams.w);\n\
	}\n\
\n\
	// Scale and bias into [0, 1] range\n\
	OUT.Color0.a = clamp(f*0.5f + 0.5f, 0.0f, 1.0f);\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Downscale Cg fragment shader source code
static const PLCore::String sDeferredDOF_Cg_FS_Downscale = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR;\n\
};\n\
\n\
// Neighbor offset table\n\
const static float2 Offsets[16] = {\n\
	float2( 1.5f, -1.5f), // 0\n\
	float2( 1.5f, -0.5f), // 1\n\
	float2( 1.5f,  0.5f), // 2\n\
	float2( 1.5f,  1.5f), // 3\n\
	float2( 0.5f, -1.5f), // 4\n\
	float2( 0.5f, -0.5f), // 5\n\
	float2( 0.5f,  0.5f), // 6\n\
	float2( 0.5f,  1.5f), // 7\n\
	float2(-0.5f, -1.5f), // 8\n\
	float2(-0.5f, -0.5f), // 9\n\
	float2(-0.5f,  0.5f), // 10\n\
	float2(-0.5f,  1.5f), // 11\n\
	float2(-1.5f, -1.5f), // 12\n\
	float2(-1.5f, -0.5f), // 13\n\
	float2(-1.5f,  0.5f), // 14\n\
	float2(-1.5f,  1.5f)  // 15\n\
};\n\
\n\
// Main main function\n\
FS_OUTPUT main(VS_OUTPUT  IN		// Interpolated output from the vertex stage\n\
	, uniform samplerRECT Texture)	// Emissive/glow texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Downscale\n\
	float4 color = 0;\n\
	for (int i=0; i<16; i++)\n\
		color += texRECT(Texture, IN.TexCoord + Offsets[i]);\n\
	OUT.Color0 = color*(1.0f/16.0f);\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Blur Cg fragment shader source code
static const PLCore::String sDeferredDOF_Cg_FS_Blur = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR;\n\
};\n\
\n\
// Neighbor offset table\n\
const static float2 Offsets[13] = {\n\
	float2( 6,  6), // 0\n\
	float2( 5,  5), // 1\n\
	float2( 4,  4), // 2\n\
	float2( 3,  3), // 3\n\
	float2( 2,  2), // 4\n\
	float2( 1,  1), // 5\n\
	float2( 0,  0), // 6\n\
	float2(-1, -1), // 7\n\
	float2(-2, -2), // 8\n\
	float2(-3, -3), // 9\n\
	float2(-4, -4), // 10\n\
	float2(-5, -5), // 11\n\
	float2(-6, -6) // 12\n\
};\n\
const static float Weights[13] = {\n\
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
};\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT   IN		// Interpolated output from the vertex stage\n\
	 , uniform float2	   UVScale	// UV scale\n\
	 , uniform samplerRECT Texture)	// texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	OUT.Color0 = 0;\n\
	for (int i=0; i<13; i++)\n\
		OUT.Color0 += texRECT(Texture, IN.TexCoord + Offsets[i]*UVScale)*Weights[i];\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Result Cg fragment shader source code
static const PLCore::String sDeferredDOF_Cg_FS_Result = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR;\n\
};\n\
\n\
#define NUM_TAPS 8				   // Number of taps the shader will use\n\
const float2 poisson[NUM_TAPS] = { // Constant poisson-distributed positions on the unit circle\n\
	{ 0.7f, -0.2f},\n\
	{ 0.1f, -0.9f},\n\
	{ 0.3f,  0.8f},\n\
	{ 0.8f,  0.3f},\n\
	{-0.7f,  0.4f},\n\
	{-0.4f,  0.6f},\n\
	{-0.8f, -0.5f},\n\
	{-0.4f,  0.6f}\n\
};\n\
\n\
const float2 maxCoC = float2(5, 10);	// Maximum circle of confusion (CoC) radius\n\
										// and diameter in pixels\n\
const float radiusScale = 0.4f;  // Scale factor for minimum CoC size on low res image\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT   IN				// Interpolated output from the vertex stage\n\
	 , uniform float       EffectWeight		// Effect weight\n\
	 , uniform float       BlurDownscale	// Blur downscale (never 0!)\n\
	 , uniform samplerRECT BlurTexture		// Blur texture\n\
	 , uniform samplerRECT Texture)			// Texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Fetch center tap\n\
	float4 texelColor = texRECT(Texture, IN.TexCoord);\n\
	OUT.Color0 = texelColor;\n\
\n\
	// Save its depth\n\
	float centerDepth = OUT.Color0.a;\n\
\n\
	// Convert depth into blur radius in pixels\n\
	float discRadius = abs(OUT.Color0.a*maxCoC.y - maxCoC.x);\n\
\n\
	// Compute radius on low-res image\n\
	float discRadiusLow = discRadius*radiusScale*(1/BlurDownscale);\n\
\n\
	// Reusing output to accumulate samples\n\
	OUT.Color0 = 0;\n\
	for (int i=0; i<NUM_TAPS; i++) {\n\
		// Compute tap texture coordinates\n\
		float2 coordLow  = IN.TexCoord/BlurDownscale + (poisson[i]*discRadiusLow);\n\
		float2 coordHigh = IN.TexCoord + (poisson[i]*discRadius);\n\
\n\
		// Fetch low- and high-res tap\n\
		float4 tapLow  = texRECT(BlurTexture, coordLow);\n\
		float4 tapHigh = texRECT(Texture,     coordHigh);\n\
\n\
		// Mix low- and high-res taps based on tap blurriness\n\
		float  tapBlur = abs(tapHigh.a*2 - 1); // Put blurriness into [0, 1]\n\
		float4 tap     = lerp(tapHigh, tapLow, tapBlur);\n\
\n\
		// 'smart' blur ignores taps that are closer than the center tap and in focus\n\
		tap.a = (tap.a >= centerDepth) ? 1 : abs(tap.a*2 - 1);\n\
\n\
		// Accumulate\n\
		OUT.Color0.rgb += tap.rgb*tap.a;\n\
		OUT.Color0.a   += tap.a;\n\
	}\n\
	#define FLT_MIN 1.175494351e-38F // Minimum positive value\n\
	if (OUT.Color0.a > FLT_MIN)\n\
		OUT.Color0 = OUT.Color0/OUT.Color0.a;\n\
	#undef FLT_MIN\n\
\n\
	// Blend between without and with effect\n\
	OUT.Color0 = lerp(texelColor, OUT.Color0, EffectWeight);\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Debug Cg fragment shader source code
static const PLCore::String sDeferredDOF_Cg_FS_Debug = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR;\n\
};\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT   IN		// Interpolated output from the vertex stage\n\
	 , uniform samplerRECT Texture)	// Texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
#ifdef FS_DEPTH_BLUR\n\
	OUT.Color0 = texRECT(Texture, IN.TexCoord).a;\n\
#elif defined FS_BLUR\n\
	OUT.Color0 = float4(texRECT(Texture, IN.TexCoord).rgb, 1);\n\
#endif\n\
\n\
	// Done\n\
	return OUT;\n\
}";
