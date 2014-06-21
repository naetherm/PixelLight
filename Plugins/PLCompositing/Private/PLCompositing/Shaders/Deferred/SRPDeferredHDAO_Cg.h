/*********************************************************\
 *  File: SRPDeferredHDAO_Cg.h                           *
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


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


// Cg vertex shader source code
static const PLCore::String sDeferredHDAO_Cg_VS = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)
};

// Programs
VS_OUTPUT main(float4 VertexPosition : POSITION,	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
	   uniform int2	  TextureSize)					// Texture size in texel
{
	VS_OUTPUT OUT;

	// Pass through the vertex position
	OUT.Position = float4(VertexPosition.xy, 0, 1);

	// Pass through the scaled vertex texture coordinate
	OUT.TexCoord = VertexPosition.zw*TextureSize;

	// Done
	return OUT;
}
);	// STRINGIFY


// Cg fragment shader source code
static const PLCore::String sDeferredHDAO_Cg_FS = "\
// Definitions\n\
#define M_PI 3.14159265f\n\
\n\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Decodes a 2 component normal vector to a 3 component normal vector\n\
float3 decodeNormalVector(float2 normal)\n\
{\n\
	float2 fenc = normal*4 - 2;\n\
	float f = dot(fenc, fenc);\n\
	float g = sqrt(1 - f/4);\n\
	float3 n;\n\
	n.xy = fenc*g;\n\
	n.z = 1 - f/2;\n\
	return n;\n\
}\n\
\n\
// Reconstructs view-space position (current surface point) using given parameters\n\
float3 uv_to_eye(float2 uv, float eye_z, float2 invFocalLen)\n\
{\n\
	// Convert from texture space [0,1] range into [-1,1] clip space\n\
	uv = uv*2 - 1;\n\
\n\
	// Reconstructs view-space position\n\
	return float3(uv * invFocalLen * eye_z, eye_z);\n\
}\n\
\n\
// Reconstructs view-space position (current surface point) using a given texel\n\
float3 fetch_eye_pos(samplerRECT texture, float2 textureResolution, float2 uv, float2 invFocalLen)\n\
{\n\
	// Fetch the linear view space depth at the given texel position\n\
	float eye_z = texRECT(texture, uv*textureResolution).b;\n\
\n\
	// Reconstructs view-space position using given parameters\n\
	return uv_to_eye(uv, eye_z, invFocalLen);\n\
}\n\
\n\
\n\
//--------------------------------------------------------------------------------------\n\
// Gather pattern\n\
//--------------------------------------------------------------------------------------\n\
// Gather defines\n\
#define RING_1    (1)\n\
#define RING_2    (2)\n\
#define RING_3    (3)\n\
#define RING_4    (4)\n\
#define NUM_RING_1_GATHERS    (2)\n\
#define NUM_RING_2_GATHERS    (6)\n\
#define NUM_RING_3_GATHERS    (12)\n\
#define NUM_RING_4_GATHERS    (20)\n\
\n\
// Ring sample pattern\n\
static const float2 g_f2HDAORingPattern[NUM_RING_4_GATHERS] =\n\
{\n\
	// Ring 1\n\
	{ 1, -1 },\n\
	{ 0, 1 },\n\
\n\
	// Ring 2\n\
	{ 0, 3 },\n\
	{ 2, 1 },\n\
	{ 3, -1 },\n\
	{ 1, -3 },\n\
\n\
	// Ring 3\n\
	{ 1, -5 },\n\
	{ 3, -3 },\n\
	{ 5, -1 },\n\
	{ 4, 1 },\n\
	{ 2, 3 },\n\
	{ 0, 5 },\n\
\n\
	// Ring 4\n\
	{ 0, 7 },\n\
	{ 2, 5 },\n\
	{ 4, 3 },\n\
	{ 6, 1 },\n\
	{ 7, -1 },\n\
	{ 5, -3 },\n\
	{ 3, -5 },\n\
	{ 1, -7 }\n\
};\n\
\n\
// Ring weights\n\
static const float4 g_f4HDAORingWeight[NUM_RING_4_GATHERS] = \n\
{\n\
	// Ring 1 (Sum = 5.30864)\n\
	{ 1.00000, 0.50000, 0.44721, 0.70711 },\n\
	{ 0.50000, 0.44721, 0.70711, 1.00000 },\n\
\n\
	// Ring 2 (Sum = 6.08746)\n\
	{ 0.30000, 0.29104, 0.37947, 0.40000 },\n\
	{ 0.42426, 0.33282, 0.37947, 0.53666 },\n\
	{ 0.40000, 0.30000, 0.29104, 0.37947 },\n\
	{ 0.53666, 0.42426, 0.33282, 0.37947 },\n\
\n\
	// Ring 3 (Sum = 6.53067)\n\
	{ 0.31530, 0.29069, 0.24140, 0.25495 },\n\
	{ 0.36056, 0.29069, 0.26000, 0.30641 },\n\
	{ 0.26000, 0.21667, 0.21372, 0.25495 },\n\
	{ 0.29069, 0.24140, 0.25495, 0.31530 },\n\
	{ 0.29069, 0.26000, 0.30641, 0.36056 },\n\
	{ 0.21667, 0.21372, 0.25495, 0.26000 },\n\
\n\
	// Ring 4 (Sum = 7.00962)\n\
	{ 0.17500, 0.17365, 0.19799, 0.20000 },\n\
	{ 0.22136, 0.20870, 0.24010, 0.25997 },\n\
	{ 0.24749, 0.21864, 0.24010, 0.28000 },\n\
	{ 0.22136, 0.19230, 0.19799, 0.23016 },\n\
	{ 0.20000, 0.17500, 0.17365, 0.19799 },\n\
	{ 0.25997, 0.22136, 0.20870, 0.24010 },\n\
	{ 0.28000, 0.24749, 0.21864, 0.24010 },\n\
	{ 0.23016, 0.22136, 0.19230, 0.19799 }\n\
};\n\
\n\
static const float g_fRingWeightsTotal[RING_4] =\n\
{\n\
	5.30864,\n\
	11.39610,\n\
	17.92677,\n\
	24.93639\n\
};\n\
\n\
#define NUM_NORMAL_LOADS (4)\n\
static const int2 g_i2NormalLoadPattern[NUM_NORMAL_LOADS] =\n\
{\n\
	{ 1, 8 },\n\
	{ 8, -1 },\n\
	{ 5, 4 },\n\
	{ 4, -4 }\n\
};\n\
\n\
\n\
// Helper function to gather samples\n\
float4 GatherZSamples(samplerRECT Tex, float2 f2TexCoord)\n\
{\n\
	float4 f4Ret;\n\
	f4Ret.x = texRECT(Tex, f2TexCoord + float2(0, 1)).b;\n\
	f4Ret.y = texRECT(Tex, f2TexCoord + float2(1, 1)).b;\n\
	f4Ret.z = texRECT(Tex, f2TexCoord + float2(1, 0)).b;\n\
	f4Ret.w = texRECT(Tex, f2TexCoord + float2(0, 0)).b;\n\
	return f4Ret;\n\
}\n\
float4 GatherNZSamples(samplerRECT Tex, float2 f2TexCoord)\n\
{\n\
	float4 f4Ret;\n\
	f4Ret.x = decodeNormalVector(texRECT(Tex, f2TexCoord + float2(0, 1)).rg).z;\n\
	f4Ret.y = decodeNormalVector(texRECT(Tex, f2TexCoord + float2(1, 1)).rg).z;\n\
	f4Ret.z = decodeNormalVector(texRECT(Tex, f2TexCoord + float2(1, 0)).rg).z;\n\
	f4Ret.w = decodeNormalVector(texRECT(Tex, f2TexCoord + float2(0, 0)).rg).z;\n\
	return f4Ret;\n\
}\n\
\n\
\n\
// Used as an early rejection test - based on normals\n\
float NormalRejectionTest(samplerRECT normalDepthMap, int2 i2ScreenCoord, int2 resolution, float acceptAngle)\n\
{\n\
	float3 f3N1;\n\
	float3 f3N2;\n\
	float fSummedDot = 0;\n\
\n\
	for (int iNormal=0; iNormal<NUM_NORMAL_LOADS; iNormal++) {\n\
		int2 i2MirrorPattern		   = (g_i2NormalLoadPattern[iNormal] + int2(1, 1))*int2(-1, -1);\n\
		int2 i2OffsetScreenCoord	   = i2ScreenCoord + g_i2NormalLoadPattern[iNormal];\n\
		int2 i2MirrorOffsetScreenCoord = i2ScreenCoord + i2MirrorPattern;\n\
\n\
		// Clamp our test to screen coordinates\n\
		i2OffsetScreenCoord.x	    = clamp((float)i2OffsetScreenCoord.x,	    0.0f, resolution.x - 1.0f);\n\
		i2OffsetScreenCoord.y	    = clamp((float)i2OffsetScreenCoord.y,	    0.0f, resolution.y - 1.0f);\n\
		i2MirrorOffsetScreenCoord.x = clamp((float)i2MirrorOffsetScreenCoord.x, 0.0f, resolution.x - 1.0f);\n\
		i2MirrorOffsetScreenCoord.y = clamp((float)i2MirrorOffsetScreenCoord.y, 0.0f, resolution.y - 1.0f);\n\
\n\
		f3N1.zxy = decodeNormalVector(texRECT(normalDepthMap, i2OffsetScreenCoord).rg);\n\
		f3N2.zxy = decodeNormalVector(texRECT(normalDepthMap, i2MirrorOffsetScreenCoord).rg);\n\
\n\
		float fDot = dot(f3N1, f3N2);\n\
\n\
		fSummedDot += (fDot > acceptAngle) ? 0 : (1 - (abs(fDot)*0.25f));\n\
	}\n\
\n\
	return (0.5f + fSummedDot*0.25f);\n\
}\n\
\n\
\n\
// HDAO : Performs valley detection in Camera Z space, and optionally offsets by the Z\n\
// component of the camera space normal\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT   IN					// Interpolated output from the vertex stage\n\
	 , uniform float	   AORejectRadius		// AO reject radius\n\
	 , uniform float	   Contrast				// Contrast\n\
	 , uniform float	   AOAcceptRadius		// AO accept radius\n\
#ifdef FS_NORMAL\n\
	 , uniform float	   NormalScale			// Normal scale\n\
#endif\n\
	 , uniform float	   AcceptAngle			// Accept angle\n\
	 , uniform int		   NumRingGathers\n\
	 , uniform int		   NumRings\n\
	 , uniform int2		   Resolution			// Resolution of the input texture data (for example 636 x 456)\n\
	 , uniform samplerRECT NormalDepthMap)		// RG=normal vector, B=linear view space length\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Locals\n\
	float4 f4Occlusion = 0;\n\
	float4 f4SampledZ[2];\n\
	float4 f4OffsetSampledZ[2];\n\
	float4 f4SampledNormalZ[2];\n\
	float4 f4Diff;\n\
	float4 f4Compare[2];\n\
	float2 f2KernelScale = float2(Resolution.x/1024.0f, Resolution.y/768.0f);\n\
\n\
	// Test the normals to see if we should apply occlusion\n\
	float fDot = NormalRejectionTest(NormalDepthMap, IN.TexCoord, Resolution, AcceptAngle);\n\
	if (fDot > 0.5f) {\n\
		// Sample the center pixel for camera Z\n\
		float4 sample = texRECT(NormalDepthMap, IN.TexCoord);\n\
		float fCenterZ = sample.b;\n\
		#ifdef FS_NORMAL\n\
			float fOffsetCenterZ = fCenterZ + decodeNormalVector(sample.rg).z*NormalScale;\n\
		#endif\n\
\n\
		// Loop through each gather location, and compare with its mirrored location\n\
		for (int iGather=0; iGather<NumRingGathers; iGather++) {\n\
			float2 f2TexCoord = IN.TexCoord + f2KernelScale*g_f2HDAORingPattern[iGather];\n\
			float2 f2MirrorScreenCoord = ((f2KernelScale*g_f2HDAORingPattern[iGather]) + float2(1, 1))*float2(-1, -1);\n\
			float2 f2MirrorTexCoord = IN.TexCoord + f2MirrorScreenCoord;\n\
\n\
			// Sample\n\
			f4SampledZ[0] = GatherZSamples(NormalDepthMap, f2TexCoord);\n\
			f4SampledZ[1] = GatherZSamples(NormalDepthMap, f2MirrorTexCoord);\n\
\n\
			// Detect valleys\n\
			f4Diff = fCenterZ.xxxx - f4SampledZ[0];\n\
			f4Compare[0]  = (f4Diff < AORejectRadius.xxxx) ? 1 : 0;\n\
			f4Compare[0] *= (f4Diff > AOAcceptRadius.xxxx) ? 1 : 0;\n\
\n\
			f4Diff = fCenterZ.xxxx - f4SampledZ[1];\n\
			f4Compare[1]  = (f4Diff < AORejectRadius.xxxx) ? 1 : 0;\n\
			f4Compare[1] *= (f4Diff > AOAcceptRadius.xxxx) ? 1 : 0;\n\
\n\
			f4Occlusion.xyzw += (g_f4HDAORingWeight[iGather].xyzw*(f4Compare[0].xyzw*f4Compare[1].zwxy)*fDot);\n\
\n\
			#ifdef FS_NORMAL\n\
				// Sample normals\n\
				f4SampledNormalZ[0] = GatherNZSamples(NormalDepthMap, f2TexCoord);\n\
				f4SampledNormalZ[1] = GatherNZSamples(NormalDepthMap, f2MirrorTexCoord);\n\
\n\
				// Scale normals\n\
				f4OffsetSampledZ[0] = f4SampledZ[0] + f4SampledNormalZ[0]*NormalScale;\n\
				f4OffsetSampledZ[1] = f4SampledZ[1] + f4SampledNormalZ[1]*NormalScale;\n\
\n\
				// Detect valleys\n\
				f4Diff = fOffsetCenterZ.xxxx - f4OffsetSampledZ[0];\n\
				f4Compare[0]  = (f4Diff < AORejectRadius.xxxx) ? 1 : 0;\n\
				f4Compare[0] *= (f4Diff > AOAcceptRadius.xxxx) ? 1 : 0;\n\
\n\
				f4Diff = fOffsetCenterZ.xxxx - f4OffsetSampledZ[1];\n\
				f4Compare[1]  = (f4Diff < AORejectRadius.xxxx) ? 1 : 0;\n\
				f4Compare[1] *= (f4Diff > AOAcceptRadius.xxxx) ? 1 : 0;\n\
\n\
				f4Occlusion.xyzw += (g_f4HDAORingWeight[iGather].xyzw*(f4Compare[0].xyzw*f4Compare[1].zwxy)*fDot);\n\
			#endif\n\
		}\n\
	}\n\
\n\
	// Finally calculate the HDAO occlusion value\n\
	#ifdef FS_NORMAL\n\
		float fOcclusion = ((f4Occlusion.x + f4Occlusion.y + f4Occlusion.z + f4Occlusion.w)/(3*g_fRingWeightsTotal[NumRings- 1]));\n\
	#else\n\
		float fOcclusion = ((f4Occlusion.x + f4Occlusion.y + f4Occlusion.z + f4Occlusion.w)/(2*g_fRingWeightsTotal[NumRings - 1]));\n\
	#endif\n\
	OUT.Color0 = 1 - clamp(fOcclusion*Contrast, 0.0f, 1.0f);\n\
\n\
	// Done\n\
	return OUT;\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
