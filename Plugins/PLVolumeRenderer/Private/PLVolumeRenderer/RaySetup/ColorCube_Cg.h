/*********************************************************\
 *  File: ColorCube_Cg.h                                 *
 *      Fragment shader source code - Cg
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


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
static const PLCore::String sSourceCode_Vertex = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 VertexPosition : POSITION;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float3 VertexTexCoord : TEXCOORD0;	// Vertex texture coordinate
};

// Programs
VS_OUTPUT main(float3   VertexPosition : POSITION,		// Object space vertex position input
			   float3   VertexTexCoord : TEXCOORD0,		// Vertex texture coordinate input
	   uniform float4x4 ObjectSpaceToClipSpaceMatrix)	// Object space to clip space matrix
{
	VS_OUTPUT Out;

	// Calculate the clip space vertex position
	Out.VertexPosition = mul(ObjectSpaceToClipSpaceMatrix, float4(VertexPosition, 1.0));

	// Pass through the vertex texture coordinate
	Out.VertexTexCoord = VertexTexCoord;

	// Done
	return Out;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
static const PLCore::String sSourceCode_Fragment_Header = STRINGIFY(
// Uniforms
uniform sampler2D FrontTexture;	// Front texture map
uniform float     StepSize;		// Step size
);	// STRINGIFY

static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Varying
float2 RaySetup_FragmentCoordinate : WPOS;

// Vertex output
struct VS_OUTPUT {
	float4 VertexPosition : POSITION;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float3 VertexTexCoord : TEXCOORD0;	// Interpolated vertex texture coordinate input from vertex shader, the position (volume object space) the ray leaves the volume
};

// Fragment output
struct FS_OUTPUT {
	float4 Color0 : COLOR0;
};

// Programs
FS_OUTPUT main(VS_OUTPUT In)	// Vertex shader output as fragment shader input
{
	FS_OUTPUT Out;

	// Get the ray end position by using the provided front texture
	float3 rayStartPosition = tex2Dfetch(FrontTexture, int4(int2(RaySetup_FragmentCoordinate.xy), 0, 0)).rgb;

	// The position (volume object space) the ray leaves the volume is provided by the vertex shader
	float3 rayEndPosition = In.VertexTexCoord;

	// Get (not normalized) ray direction in volume object space
	float3 rayDirection = rayEndPosition - rayStartPosition;

	// Calculate the distance our ray travels through the volume and the normalized ray direction
	float maximumTravelLength = length(rayDirection);
	float3 rayDirectionNormalized = rayDirection/maximumTravelLength;

	// Calculate the position advance per step along the ray
	float3 stepPositionDelta = rayDirectionNormalized*StepSize;

	// Call the clip ray function
	//     void ClipRay(inout float3 RayOrigin, float3 RayDirection, inout float MaximumTravelLength)
	ClipRay(rayStartPosition, rayDirectionNormalized, maximumTravelLength);

	// Call the jitter position function to add jittering to the start position of the ray in order to reduce wooden grain effect
	//     float JitterPosition(float3 Position)
	rayStartPosition += stepPositionDelta*JitterPosition(rayStartPosition);

	// Call the ray traversal function
	//     float4 RayTraversal(float3 StartPosition, int NumberOfSteps, float3 StepPositionDelta, float MaximumTravelLength)
	// -> Clamp the result to [0, 1] interval, else the image might flicker ugly on NVIDIA GPU's while working fine on AMD GPU'S (HDR only issue)
	Out.Color0 = (maximumTravelLength > 0.0) ? clamp(RayTraversal(rayStartPosition, int(maximumTravelLength/StepSize), stepPositionDelta, maximumTravelLength), float4(0.0, 0.0, 0.0, 0.0), float4(1.0, 1.0, 1.0, 1.0)) : float4(0.0, 0.0, 0.0, 0.0);

	// Done
	return Out;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
