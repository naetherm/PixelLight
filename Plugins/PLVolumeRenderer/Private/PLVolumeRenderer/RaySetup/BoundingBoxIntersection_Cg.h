/*********************************************************\
 *  File: BoundingBoxIntersection_Cg.h                   *
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
};

// Programs
VS_OUTPUT main(float3   VertexPosition : POSITION,		// Object space vertex position input
	   uniform float4x4 ObjectSpaceToClipSpaceMatrix)	// Object space to clip space matrix
{
	VS_OUTPUT Out;

	// Calculate the clip space vertex position
	Out.VertexPosition = mul(ObjectSpaceToClipSpaceMatrix, float4(VertexPosition, 1.0));

	// Done
	return Out;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
static const PLCore::String sSourceCode_Fragment_Header = STRINGIFY(
// Uniforms
uniform int2     WindowSize;				// Window size
uniform float4x4 ClipSpaceToObjectSpace;	// Clip space to object space matrix
uniform float3   RayOrigin;					// Ray origin in volume object space
uniform float    StepSize;					// Step size
);	// STRINGIFY

static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Varying
float2 RaySetup_FragmentCoordinate : WPOS;

// Vertex output
struct VS_OUTPUT {
	float4 VertexPosition : POSITION;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
};

// Fragment output
struct FS_OUTPUT {
	float4 Color0 : COLOR0;
};

// Programs
FS_OUTPUT main(VS_OUTPUT In)	// Vertex shader output as fragment shader input
{
	FS_OUTPUT Out;

	// Get normalized ray direction in volume object space
	float3 rayDirection;
	{
		// Compute the homogeneous view-space position
		float4 hviewpos;
		hviewpos.xy = RaySetup_FragmentCoordinate/WindowSize;	// Window position is in [0,1]^2
		hviewpos.z  = 0.0;										// Depth in [0,1]
		hviewpos.w  = 1.0;

		// We need this to be in [-1,1]^3 clip space
		hviewpos = hviewpos*2.0 - 1.0;

		// Back-project to homogeneous volume space
		float4 hvolpos = mul(ClipSpaceToObjectSpace, hviewpos);

		// Get normalized volume-space position
		float4 nearPosition = hvolpos/hvolpos.w;

		// Compute the homogeneous view-space position
		hviewpos.xy = RaySetup_FragmentCoordinate/WindowSize;	// Window position is in [0,1]^2
		hviewpos.z  = 1.0;										// Depth in [0,1]
		hviewpos.w  = 1.0;

		// We need this to be in [-1,1]^3 clip space
		hviewpos = hviewpos*2.0 - 1.0;

		// Back-project to homogeneous volume space
		hvolpos = mul(ClipSpaceToObjectSpace, hviewpos);

		// Get normalized volume-space position
		float4 farPosition = hvolpos/hvolpos.w;

		// Get normalized ray direction in volume object space
		rayDirection = normalize(farPosition.xyz - nearPosition.xyz);
	}

	// The position (volume object space) the ray enters the volume
	float3 rayStartPosition;

	// Calculate the distance our ray travels through the volume
	float maximumTravelLength;
	{ // Intersect ray with a cube (http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm)
		// Compute intersection of ray with all six bbox planes
		// (due overhead we don't test for division by zero)
		float3 tbot = (float3(0.0, 0.0, 0.0) - RayOrigin) / rayDirection;
		float3 ttop = (float3(1.0, 1.0, 1.0) - RayOrigin) / rayDirection;

		// Re-order intersections to find smallest and largest on each axis
		float3 tmin = min(ttop, tbot);
		float3 tmax = max(ttop, tbot);

		// Find the largest tmin and the smallest tmax
		float near = max(max(tmin.x, tmin.y), max(tmin.x, tmin.z));
		float far  = min(min(tmax.x, tmax.y), min(tmax.x, tmax.z));

		// Clamp near
		if (near < 0.0)
			near = 0.0;

		// Calculate the position (volume object space) the ray enters the volume
		rayStartPosition = RayOrigin + near*rayDirection;

		// [DEBUG] Fragment color = [0, 1] -> 1 (white) if there was an intersection, else 0 (black) -> result should always be white because we're drawing a cube in order to generate the rays
		// Out.Color0 = (far > near) ? float4(1.0, 1.0, 1.0, 1.0) : float4(0.0, 0.0, 0.0, 1.0); return Out;

		// Calculate the distance our ray travels through the volume
		maximumTravelLength = far - near;
	}

	// Calculate the position advance per step along the ray
	float3 stepPositionDelta = rayDirection*StepSize;

	// Call the clip ray function
	//     void ClipRay(inout float3 RayOrigin, float3 RayDirection, inout float MaximumTravelLength)
	ClipRay(rayStartPosition, rayDirection, maximumTravelLength);

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
