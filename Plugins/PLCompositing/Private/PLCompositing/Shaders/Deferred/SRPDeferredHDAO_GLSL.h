/*********************************************************\
 *  File: SRPDeferredHDAO_GLSL.h                         *
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


// OpenGL 3.0 ("#version 130") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredHDAO_GLSL_VS = STRINGIFY(
// Attributes
in  vec4 VertexPosition;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
							// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
out vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)

// Uniforms
uniform ivec2 TextureSize;	// Texture size in texel

// Programs
void main()
{
	// Set the clip space vertex position
	gl_Position = vec4(VertexPosition.xy, 0.0f, 1.0f);

	// Pass through the scaled vertex texture coordinate
	VertexTexCoordVS = VertexPosition.zw*TextureSize;
}
);	// STRINGIFY


// OpenGL 3.0 ("#version 130") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredHDAO_GLSL_FS = "\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Definitions\n\
#define M_PI 3.14159265f\n\
\n\
// Attributes\n\
in vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
\n\
// Uniforms\n\
uniform float			AORejectRadius;		// AO reject radius\n\
uniform float			Contrast;			// Contrast\n\
uniform float			AOAcceptRadius;		// AO accept radius\n\
#ifdef FS_NORMAL\n\
	uniform float		NormalScale;		// Normal scale\n\
#endif\n\
uniform float			AcceptAngle;		// Accept angle\n\
uniform int				NumRingGathers;\n\
uniform int				NumRings;\n\
uniform ivec2			Resolution;			// Resolution of the input texture data (for example 636 x 456)\n\
uniform sampler2DRect	NormalDepthMap;		// RG=normal vector, B=linear view space length\n\
\n\
// Programs\n\
// Decodes a 2 component normal vector to a 3 component normal vector\n\
vec3 decodeNormalVector(vec2 normal)\n\
{\n\
	vec2 fenc = normal*4 - 2;\n\
	float f = dot(fenc, fenc);\n\
	float g = sqrt(1 - f/4);\n\
	vec3 n;\n\
	n.xy = fenc*g;\n\
	n.z = 1 - f/2;\n\
	return n;\n\
}\n\
\n\
// Reconstructs view-space position (current surface point) using given parameters\n\
vec3 uv_to_eye(vec2 uv, float eye_z, vec2 invFocalLen)\n\
{\n\
	// Convert from texture space [0,1] range into [-1,1] clip space\n\
	uv = uv*2 - 1;\n\
\n\
	// Reconstructs view-space position\n\
	return vec3(uv * invFocalLen * eye_z, eye_z);\n\
}\n\
\n\
// Reconstructs view-space position (current surface point) using a given texel\n\
vec3 fetch_eye_pos(sampler2DRect texture, vec2 textureResolution, vec2 uv, vec2 invFocalLen)\n\
{\n\
	// Fetch the linear view space depth at the given texel position\n\
	float eye_z = texture2DRect(texture, uv*textureResolution).b;\n\
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
const vec2 g_f2HDAORingPattern[NUM_RING_4_GATHERS] = vec2[NUM_RING_4_GATHERS](\n\
	// Ring 1\n\
	vec2( 1, -1 ),\n\
	vec2( 0,  1 ),\n\
\n\
	// Ring 2\n\
	vec2( 0,  3 ),\n\
	vec2( 2,  1 ),\n\
	vec2( 3, -1 ),\n\
	vec2( 1, -3 ),\n\
\n\
	// Ring 3\n\
	vec2( 1, -5 ),\n\
	vec2( 3, -3 ),\n\
	vec2( 5, -1 ),\n\
	vec2( 4,  1 ),\n\
	vec2( 2,  3 ),\n\
	vec2( 0,  5 ),\n\
\n\
	// Ring 4\n\
	vec2( 0,  7 ),\n\
	vec2( 2,  5 ),\n\
	vec2( 4,  3 ),\n\
	vec2( 6,  1 ),\n\
	vec2( 7, -1 ),\n\
	vec2( 5, -3 ),\n\
	vec2( 3, -5 ),\n\
	vec2( 1, -7 )\n\
);\n\
\n\
// Ring weights\n\
const vec4 g_f4HDAORingWeight[NUM_RING_4_GATHERS] = vec4[NUM_RING_4_GATHERS](\n\
	// Ring 1 (Sum = 5.30864)\n\
	vec4( 1.00000, 0.50000, 0.44721, 0.70711 ),\n\
	vec4( 0.50000, 0.44721, 0.70711, 1.00000 ),\n\
\n\
	// Ring 2 (Sum = 6.08746)\n\
	vec4( 0.30000, 0.29104, 0.37947, 0.40000 ),\n\
	vec4( 0.42426, 0.33282, 0.37947, 0.53666 ),\n\
	vec4( 0.40000, 0.30000, 0.29104, 0.37947 ),\n\
	vec4( 0.53666, 0.42426, 0.33282, 0.37947 ),\n\
\n\
	// Ring 3 (Sum = 6.53067)\n\
	vec4( 0.31530, 0.29069, 0.24140, 0.25495 ),\n\
	vec4( 0.36056, 0.29069, 0.26000, 0.30641 ),\n\
	vec4( 0.26000, 0.21667, 0.21372, 0.25495 ),\n\
	vec4( 0.29069, 0.24140, 0.25495, 0.31530 ),\n\
	vec4( 0.29069, 0.26000, 0.30641, 0.36056 ),\n\
	vec4( 0.21667, 0.21372, 0.25495, 0.26000 ),\n\
\n\
	// Ring 4 (Sum = 7.00962)\n\
	vec4( 0.17500, 0.17365, 0.19799, 0.20000 ),\n\
	vec4( 0.22136, 0.20870, 0.24010, 0.25997 ),\n\
	vec4( 0.24749, 0.21864, 0.24010, 0.28000 ),\n\
	vec4( 0.22136, 0.19230, 0.19799, 0.23016 ),\n\
	vec4( 0.20000, 0.17500, 0.17365, 0.19799 ),\n\
	vec4( 0.25997, 0.22136, 0.20870, 0.24010 ),\n\
	vec4( 0.28000, 0.24749, 0.21864, 0.24010 ),\n\
	vec4( 0.23016, 0.22136, 0.19230, 0.19799 )\n\
);\n\
\n\
const float g_fRingWeightsTotal[RING_4] = float[RING_4](\n\
	5.30864,\n\
	11.39610,\n\
	17.92677,\n\
	24.93639\n\
);\n\
\n\
#define NUM_NORMAL_LOADS (4)\n\
const ivec2 g_i2NormalLoadPattern[NUM_NORMAL_LOADS] = ivec2[NUM_NORMAL_LOADS](\n\
	ivec2( 1,  8 ),\n\
	ivec2( 8, -1 ),\n\
	ivec2( 5,  4 ),\n\
	ivec2( 4, -4 )\n\
);\n\
\n\
\n\
// Helper function to gather samples\n\
vec4 GatherZSamples(sampler2DRect Tex, vec2 f2TexCoord)\n\
{\n\
	vec4 f4Ret;\n\
	f4Ret.x = texture2DRect(Tex, f2TexCoord + vec2(0, 1)).b;\n\
	f4Ret.y = texture2DRect(Tex, f2TexCoord + vec2(1, 1)).b;\n\
	f4Ret.z = texture2DRect(Tex, f2TexCoord + vec2(1, 0)).b;\n\
	f4Ret.w = texture2DRect(Tex, f2TexCoord + vec2(0, 0)).b;\n\
	return f4Ret;\n\
}\n\
vec4 GatherNZSamples(sampler2DRect Tex, vec2 f2TexCoord)\n\
{\n\
	vec4 f4Ret;\n\
	f4Ret.x = decodeNormalVector(texture2DRect(Tex, f2TexCoord + vec2(0, 1)).rg).z;\n\
	f4Ret.y = decodeNormalVector(texture2DRect(Tex, f2TexCoord + vec2(1, 1)).rg).z;\n\
	f4Ret.z = decodeNormalVector(texture2DRect(Tex, f2TexCoord + vec2(1, 0)).rg).z;\n\
	f4Ret.w = decodeNormalVector(texture2DRect(Tex, f2TexCoord + vec2(0, 0)).rg).z;\n\
	return f4Ret;\n\
}\n\
\n\
\n\
// Used as an early rejection test - based on normals\n\
float NormalRejectionTest(sampler2DRect normalDepthMap, ivec2 i2ScreenCoord, ivec2 resolution, float acceptAngle)\n\
{\n\
	vec3 f3N1;\n\
	vec3 f3N2;\n\
	float fSummedDot = 0;\n\
\n\
	for (int iNormal=0; iNormal<NUM_NORMAL_LOADS; iNormal++) {\n\
		ivec2 i2MirrorPattern		    = (g_i2NormalLoadPattern[iNormal] + ivec2(1, 1))*ivec2(-1, -1);\n\
		ivec2 i2OffsetScreenCoord	    = i2ScreenCoord + g_i2NormalLoadPattern[iNormal];\n\
		ivec2 i2MirrorOffsetScreenCoord = i2ScreenCoord + i2MirrorPattern;\n\
\n\
		// Clamp our test to screen coordinates\n\
		i2OffsetScreenCoord.x	    = clamp(i2OffsetScreenCoord.x,	     0, resolution.x - 1);\n\
		i2OffsetScreenCoord.y	    = clamp(i2OffsetScreenCoord.y,	     0, resolution.y - 1);\n\
		i2MirrorOffsetScreenCoord.x = clamp(i2MirrorOffsetScreenCoord.x, 0, resolution.x - 1);\n\
		i2MirrorOffsetScreenCoord.y = clamp(i2MirrorOffsetScreenCoord.y, 0, resolution.y - 1);\n\
\n\
		f3N1.zxy = decodeNormalVector(texture2DRect(normalDepthMap, i2OffsetScreenCoord).rg);\n\
		f3N2.zxy = decodeNormalVector(texture2DRect(normalDepthMap, i2MirrorOffsetScreenCoord).rg);\n\
\n\
		float fDot = dot(f3N1, f3N2);\n\
\n\
		fSummedDot += (fDot > acceptAngle) ? 0.0f : (1.0f - (abs(fDot)*0.25f));\n\
	}\n\
\n\
	return (0.5f + fSummedDot*0.25f);\n\
}\n\
\n\
\n\
// Program entry point\n\
// HDAO : Performs valley detection in Camera Z space, and optionally offsets by the Z\n\
// component of the camera space normal\n\
void main()\n\
{\n\
	// Locals\n\
	vec4 f4Occlusion = vec4(0);\n\
	vec4 f4SampledZ[2];\n\
	vec4 f4OffsetSampledZ[2];\n\
	vec4 f4SampledNormalZ[2];\n\
	vec4 f4Diff;\n\
	vec4 f4Compare[2];\n\
	vec2 f2KernelScale = vec2(Resolution.x/1024.0f, Resolution.y/768.0f);\n\
\n\
	// Test the normals to see if we should apply occlusion\n\
	float fDot = NormalRejectionTest(NormalDepthMap, ivec2(VertexTexCoordVS), Resolution, AcceptAngle);\n\
	if (fDot > 0.5f) {\n\
		// Sample the center pixel for camera Z\n\
		vec4 sample = texture2DRect(NormalDepthMap, VertexTexCoordVS);\n\
		float fCenterZ = sample.b;\n\
		#ifdef FS_NORMAL\n\
			float fOffsetCenterZ = fCenterZ + decodeNormalVector(sample.rg).z*NormalScale;\n\
		#endif\n\
\n\
		// Loop through each gather location, and compare with its mirrored location\n\
		for (int iGather=0; iGather<NumRingGathers; iGather++) {\n\
			vec2 f2TexCoord = VertexTexCoordVS + f2KernelScale*g_f2HDAORingPattern[iGather];\n\
			vec2 f2MirrorScreenCoord = ((f2KernelScale*g_f2HDAORingPattern[iGather]) + vec2(1, 1))*vec2(-1, -1);\n\
			vec2 f2MirrorTexCoord = VertexTexCoordVS + f2MirrorScreenCoord;\n\
\n\
			// Sample\n\
			f4SampledZ[0] = GatherZSamples(NormalDepthMap, f2TexCoord);\n\
			f4SampledZ[1] = GatherZSamples(NormalDepthMap, f2MirrorTexCoord);\n\
\n\
			// Detect valleys\n\
			f4Diff = fCenterZ.xxxx - f4SampledZ[0];\n\
			f4Compare[0].x  = (f4Diff.x < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[0].x *= (f4Diff.x > AOAcceptRadius) ? 1.0f : 0.0f;\n\
			f4Compare[0].y  = (f4Diff.y < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[0].y *= (f4Diff.y > AOAcceptRadius) ? 1.0f : 0.0f;\n\
			f4Compare[0].z  = (f4Diff.z < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[0].z *= (f4Diff.z > AOAcceptRadius) ? 1.0f : 0.0f;\n\
			f4Compare[0].w  = (f4Diff.w < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[0].w *= (f4Diff.w > AOAcceptRadius) ? 1.0f : 0.0f;\n\
			f4Diff = fCenterZ.xxxx - f4SampledZ[1];\n\
			f4Compare[1].x  = (f4Diff.x < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[1].x *= (f4Diff.x > AOAcceptRadius) ? 1.0f : 0.0f;\n\
			f4Compare[1].y  = (f4Diff.y < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[1].y *= (f4Diff.y > AOAcceptRadius) ? 1.0f : 0.0f;\n\
			f4Compare[1].z  = (f4Diff.z < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[1].z *= (f4Diff.z > AOAcceptRadius) ? 1.0f : 0.0f;\n\
			f4Compare[1].w  = (f4Diff.w < AORejectRadius) ? 1.0f : 0.0f;\n\
			f4Compare[1].w *= (f4Diff.w > AOAcceptRadius) ? 1.0f : 0.0f;\n\
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
				f4Compare[0].x  = (f4Diff.x < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[0].x *= (f4Diff.x > AOAcceptRadius) ? 1.0f : 0.0f;\n\
				f4Compare[0].y  = (f4Diff.y < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[0].y *= (f4Diff.y > AOAcceptRadius) ? 1.0f : 0.0f;\n\
				f4Compare[0].z  = (f4Diff.z < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[0].z *= (f4Diff.z > AOAcceptRadius) ? 1.0f : 0.0f;\n\
				f4Compare[0].w  = (f4Diff.w < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[0].w *= (f4Diff.w > AOAcceptRadius) ? 1.0f : 0.0f;\n\
\n\
				f4Diff = fOffsetCenterZ.xxxx - f4OffsetSampledZ[1];\n\
				f4Compare[1].x  = (f4Diff.x < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[1].x *= (f4Diff.x > AOAcceptRadius) ? 1.0f : 0.0f;\n\
				f4Compare[1].y  = (f4Diff.y < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[1].y *= (f4Diff.y > AOAcceptRadius) ? 1.0f : 0.0f;\n\
				f4Compare[1].z  = (f4Diff.z < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[1].z *= (f4Diff.z > AOAcceptRadius) ? 1.0f : 0.0f;\n\
				f4Compare[1].w  = (f4Diff.w < AORejectRadius) ? 1.0f : 0.0f;\n\
				f4Compare[1].w *= (f4Diff.w > AOAcceptRadius) ? 1.0f : 0.0f;\n\
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
	gl_FragColor = vec4(1 - clamp(fOcclusion*Contrast, 0.0f, 1.0f));\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
