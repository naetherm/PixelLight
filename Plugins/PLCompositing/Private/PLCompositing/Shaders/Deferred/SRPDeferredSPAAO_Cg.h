/*********************************************************\
 *  File: SRPDeferredSPAAO_Cg.h                          *
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
static const PLCore::String sDeferredSPAAO_Cg_VS = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
};

// Programs
VS_OUTPUT main(float4 VertexPosition : POSITION)	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
{
	VS_OUTPUT OUT;

	// Pass through the vertex position
	OUT.Position = float4(VertexPosition.xy, 0, 1);

	// Pass through the scaled vertex texture coordinate
	OUT.TexCoord = VertexPosition.zw;

	// Done
	return OUT;
}
);	// STRINGIFY


// Cg fragment shader source code
static const PLCore::String sDeferredSPAAO_Cg_FS = "\
// Definitions\n\
#define M_PI 3.14159265f\n\
\n\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
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
// AO calculation\n\
float doAmbientOcclusion(float2 tcoord, float2 uv, float3 p, float3 cnorm, float intensity, float scale, float bias, float2 invFocalLen, int2 resolution, samplerRECT normalDepthMap)\n\
{\n\
	float3 diff = fetch_eye_pos(normalDepthMap, resolution, tcoord + uv, invFocalLen) - p;\n\
	const float3 v = normalize(diff);\n\
	const float d = length(diff)*scale;\n\
	return max(0.0f, dot(cnorm, v) - bias)*(1.0f/(1.0f + d))*intensity;\n\
}\n\
\n\
\n\
static const float2 vec[4] =\n\
{\n\
	{ 1.0,  0.0},\n\
	{-1.0,  0.0},\n\
	{ 0.0,  1.0},\n\
	{ 0.0, -1.0}\n\
};\n\
\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT	IN					// Interpolated output from the vertex stage\n\
	 , uniform float		SampleRadius		// The sampling radius\n\
	 , uniform float		Intensity			// The ambient occlusion intensity\n\
	 , uniform float		Scale				// Scales distance between occludes and occludee\n\
	 , uniform float		Bias				// Controls the width of the occlusion cone considered by the occludee\n\
	 , uniform float2		InvFocalLen			// 1/FocalLen (for example: 0.57771897 x 0.4142136)\n\
	 , uniform int			RandomSize			// Resolution of the input random data (for example 64)\n\
	 , uniform sampler2D	RandomNormalsMap	// RG=normal vector, B=linear view space length\n\
	 , uniform int2			Resolution			// Resolution of the input texture data (for example 636 x 456)\n\
	 , uniform samplerRECT	NormalDepthMap)		// RG=normal vector, B=linear view space length\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Fetch the required texel data\n\
	float4 sample = texRECT(NormalDepthMap, IN.TexCoord*Resolution);\n\
\n\
	// Reconstruct view-space position\n\
	float3 p = uv_to_eye(IN.TexCoord, sample.b, InvFocalLen);\n\
\n\
	// Reconstruct view-space normal vector\n\
	float3 n = decodeNormalVector(sample.rg);\n\
	n.z = -n.z; // If this is not done, we get wrong results\n\
\n\
	// Locals\n\
	float2 rand = normalize(tex2D(RandomNormalsMap, Resolution*IN.TexCoord/RandomSize).xy*2.0f - 1.0f);	// Returns a random normal vector\n\
	float ao = 0.0f;\n\
	float rad = SampleRadius/p.z;\n\
\n\
	// SSAO calculation\n\
	int iterations = 4;\n\
	for (int j=0; j<iterations; j++) {\n\
		float2 coord1 = reflect(vec[j], rand)*rad;\n\
		float2 coord2 = float2(coord1.x*0.707f - coord1.y*0.707f, coord1.x*0.707f + coord1.y*0.707f);\n\
		ao += doAmbientOcclusion(IN.TexCoord, coord1*0.25f, p, n, Intensity, Scale, Bias, InvFocalLen, Resolution, NormalDepthMap);\n\
		ao += doAmbientOcclusion(IN.TexCoord, coord2*0.5f,  p, n, Intensity, Scale, Bias, InvFocalLen, Resolution, NormalDepthMap);\n\
		ao += doAmbientOcclusion(IN.TexCoord, coord1*0.75f, p, n, Intensity, Scale, Bias, InvFocalLen, Resolution, NormalDepthMap);\n\
		ao += doAmbientOcclusion(IN.TexCoord, coord2,       p, n, Intensity, Scale, Bias, InvFocalLen, Resolution, NormalDepthMap);\n\
	}\n\
	ao /= iterations*4.0;\n\
	OUT.Color0 = float4(1 - clamp(ao, 0.0f, 1.0f));\n\
\n\
	// Done\n\
	return OUT;\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
