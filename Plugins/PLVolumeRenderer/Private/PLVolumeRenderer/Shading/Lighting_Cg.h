/*********************************************************\
 *  File: Lighting_Cg.h                                  *
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


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform float3 ViewingDirection;	// Viewing direction in scene node space
uniform float3 LightDirection;		// Normalized light direction in scene node space
uniform float3 AmbientColor;		// Ambient color

// Varying
float3 previousNormal = float3(0.0, 0.0, 0.0);	// Normal from the previous step

/**
*  @brief
*    Shading
*
*  @param[in] Scalar
*    Current voxel scalar
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] StepPositionDelta
*    Position advance per step along the ray, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    RGBA result of the shading
*/
float4 Shading(float Scalar, float3 Position, float3 StepPositionDelta)
{
	// Call the classification function
	//     float4 Classification(float Scalar)
	float4 color = Classification(Scalar);

	// Call the gradient function
	//     float3 Gradient(float3 Position)
	float3 gradient = Gradient(Position);

	// Calculate the normalized normal, avoid nasty division by zero
	float gradientLength = length(gradient);
	float3 normal = (gradientLength > 0.0) ? gradient/gradientLength : previousNormal;

	// Backup the currently used normal vector
	previousNormal = normal;

	// Call the illumination function
	//     float3 Illumination(float3 SurfaceColor, float3 SurfaceNormal, float3 ViewingDirection, float3 LightDirection)
	color.rgb = AmbientColor + Illumination(color.rgb, normal, ViewingDirection, LightDirection);

	// Return the RGBA result of the shading
	return color;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
