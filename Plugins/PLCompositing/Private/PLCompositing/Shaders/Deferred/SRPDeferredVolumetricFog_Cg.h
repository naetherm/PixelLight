/*********************************************************\
 *  File: SRPDeferredVolumetricFog_Cg.h                  *
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
static const PLCore::String sDeferredVolumetricFog_Cg_VS = STRINGIFY(
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

	// Pass through the vertex texture coordinate
	OUT.TexCoord = VertexPosition.zw;

	// Done
	return OUT;
}
);	// STRINGIFY


// Cg fragment shader source code
static const PLCore::String sDeferredVolumetricFog_Cg_FS = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
};

// Fragment output
struct FS_OUTPUT {
	float4 Color0 : COLOR0;
};

// Decodes a 2 component normal vector to a 3 component normal vector
float3 decodeNormalVector(float2 normal)
{
	float2 fenc = normal*4 - 2;
	float f = dot(fenc, fenc);
	float g = sqrt(1 - f/4);
	float3 n;
	n.xy = fenc*g;
	n.z = 1 - f/2;
	return n;
}

// Reconstructs view-space position (current surface point) using given parameters
float3 uv_to_eye(float2 uv, float eye_z, float2 invFocalLen)
{
	// Convert from texture space [0,1] range into [-1,1] clip space
	uv = uv*2 - 1;

	// Reconstructs view-space position
	return float3(uv * invFocalLen * eye_z, eye_z);
}

// Reconstructs view-space position (current surface point) using a given texel
float3 fetch_eye_pos(samplerRECT texture, float2 textureResolution, float2 uv, float2 invFocalLen)
{
	// Fetch the linear view space depth at the given texel position
	float eye_z = texRECT(texture, uv*textureResolution).b;

	// Reconstructs view-space position using given parameters
	return uv_to_eye(uv, eye_z, invFocalLen);
}

// Programs
FS_OUTPUT main(VS_OUTPUT   IN				// Interpolated output from the vertex stage
	 , uniform float3      FogPosition		// View space fog position
	 , uniform float       FogInvRadius		// View space fog inverse radius
	 , uniform float3      FogColor0
	 , uniform float3      FogColor1
	 , uniform float2      InvFocalLen		// 1/FocalLen (for example: 0.57771897 x 0.4142136)
	 , uniform int2        Resolution		// Resolution of the input texture data (for example 636 x 456)
	 , uniform samplerRECT NormalDepthMap	// Normal depth texture
	 )
{
	FS_OUTPUT OUT;

	// Get the current view space position
	float3 viewSpacePos = fetch_eye_pos(NormalDepthMap, Resolution, IN.TexCoord, InvFocalLen);
	viewSpacePos.z = -viewSpacePos.z;

	// Calculate the fog factor
	float3 fogPosition = (FogPosition - viewSpacePos)*FogInvRadius;
	viewSpacePos = -viewSpacePos*FogInvRadius;
	float value = dot(viewSpacePos, viewSpacePos);
	if (value == 0.0f)
		value = 0.000001f;	// Do never ever divide through zero, this may lead to ugly blocks!
	float  k  = clamp(dot(fogPosition, viewSpacePos)/value, 0.0f, 1.0f);
	float4 pl = float4(k*viewSpacePos - fogPosition, 1.0f);
	value = dot(pl, pl);
	if (value == 0.0f)
		value = 0.000001f;	// Do never ever divide through zero, this may lead to ugly blocks!
	OUT.Color0.rgb = clamp(FogColor0/value - FogColor1, 0.0f, 1.0f);
	OUT.Color0.a = 1;

	// Done
	return OUT;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
