/*********************************************************\
 *  File: ColorCube_GLSL.h                               *
 *      Fragment shader source code - GLSL (OpenGL 3.3 ("#version 330")
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
// Attributes
in  vec3 VertexPosition;	// Object space vertex position input
in  vec3 VertexTexCoord;	// Vertex texture coordinate input
out vec3 VertexTexCoordVS;	// Vertex texture coordinate output

// Uniforms
uniform mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix

// Programs
void main()
{
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix*vec4(VertexPosition, 1.0);

	// Pass through the vertex texture coordinate
	VertexTexCoordVS = VertexTexCoord;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
static const PLCore::String sSourceCode_Fragment_Header = STRINGIFY(
// Attributes
					 in  vec3 VertexTexCoordVS;	// Interpolated vertex texture coordinate input from vertex shader, the position (volume object space) the ray leaves the volume
layout(location = 0) out vec4 OutFragmentColor;	// Resulting fragment color

// Uniforms
uniform sampler2D FrontTexture;	// Front texture map
uniform float     StepSize;		// Step size
);	// STRINGIFY

static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Programs
void main()
{
	// Get the ray start position by using the provided front texture
	vec3 rayStartPosition = texelFetch(FrontTexture, ivec2(gl_FragCoord.xy), 0).rgb;

	// The position (volume object space) the ray leaves the volume is provided by the vertex shader
	vec3 rayEndPosition = VertexTexCoordVS;

	// Get (not normalized) ray direction in volume object space
	vec3 rayDirection = rayEndPosition - rayStartPosition;

	// Calculate the distance our ray travels through the volume and the normalized ray direction
	float maximumTravelLength = length(rayDirection);
	vec3 rayDirectionNormalized = rayDirection/maximumTravelLength;

	// Calculate the position advance per step along the ray
	vec3 stepPositionDelta = rayDirectionNormalized*StepSize;

	// Call the clip ray function
	//     void ClipRay(inout vec3 RayOrigin, vec3 RayDirection, inout float MaximumTravelLength)
	ClipRay(rayStartPosition, rayDirectionNormalized, maximumTravelLength);

	// Call the jitter position function to add jittering to the start position of the ray in order to reduce wooden grain effect
	//     float JitterPosition(vec3 Position)
	rayStartPosition += stepPositionDelta*JitterPosition(rayStartPosition);

	// Call the ray traversal function
	//     vec4 RayTraversal(vec3 StartPosition, int NumberOfSteps, vec3 StepPositionDelta, float MaximumTravelLength)
	// -> Clamp the result to [0, 1] interval, else the image might flicker ugly on NVIDIA GPU's while working fine on AMD GPU'S (HDR only issue)
	OutFragmentColor = (maximumTravelLength > 0.0) ? clamp(RayTraversal(rayStartPosition, int(maximumTravelLength/StepSize), stepPositionDelta, maximumTravelLength), vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 1.0, 1.0, 1.0)) : vec4(0.0, 0.0, 0.0, 0.0);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
