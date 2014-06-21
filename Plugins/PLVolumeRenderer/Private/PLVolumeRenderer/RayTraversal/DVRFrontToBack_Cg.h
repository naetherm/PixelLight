/*********************************************************\
 *  File: DVRFrontToBack_Cg.h                            *
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


// Front-to-back compositing as described within the book "Real-Time Volume Graphics", section "7.1 Basic Structure of Ray Casting" (page 165)
// -> Early ray termination
// -> Requires opacity correction
//   -> Described in the book "Real-Time Volume Graphics", section "9.1.3 Opacity Correction" (page 219)
//   -> Also described in "GPU Gems", section "39.4.3 Rendering" ( http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html )


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform float OpacityCorrectionFactor;	// Opacity correction factor
uniform float Opacity;					// Opacity, usually within the interval [~0 .. 1] = [transparent .. solid]

/**
*  @brief
*    Integrates over the volume
*
*  @param[in] StartPosition
*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] NumberOfSteps
*    Number of steps along the ray (= number of samples to take), must be positive
*  @param[in] StepPositionDelta
*    Position advance per step along the ray, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] MaximumTravelLength
*    Maximum travel length along the ray, within the interval [0 .. 1]
*
*  @return
*    RGBA result of the ray traversal
*/
float4 RayTraversal(float3 StartPosition, int NumberOfSteps, float3 StepPositionDelta, float MaximumTravelLength)
{
	// Current position inside the volume
	float3 currentPosition = StartPosition;

	// Color accumulation variable
	float4 destinationColor = float4(0.0, 0.0, 0.0, 0.0);

	// Integrate front-to-back over the volume
	// -> Front-to-back traversal
	for (int step=0; step<NumberOfSteps; step++) {
		// Call the clip position function
		//     bool ClipPosition(float3 Position)
		if (!ClipPosition(currentPosition)) {
			// Call the reconstruction function
			//     float Reconstruction(float3 Position)
			float scalar = Reconstruction(currentPosition);

			// Call the shading function
			//     vec4 Shading(float Scalar, float3 Position, float3 StepPositionDelta)
			float4 sourceColor = Shading(scalar, currentPosition, StepPositionDelta);

			// Apply opacity correction
			sourceColor.a = (1.0 - pow(1.0 - sourceColor.a, OpacityCorrectionFactor))*Opacity;

			// Front-to-back compositing
			sourceColor.rgb *= sourceColor.a;
			destinationColor = destinationColor + (1.0 - destinationColor.a)*sourceColor;

			// Early ray termination: Break from the loop when alpha gets high enough
			if (destinationColor.a >= 0.95)
				break;
		}

		// Update the current position inside the volume
		currentPosition += StepPositionDelta;
	}

	// Return the calculated fragment color
	return destinationColor;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
