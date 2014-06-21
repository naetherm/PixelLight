/*********************************************************\
 *  File: Isosurface_Cg.h                                *
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
uniform float IsosurfaceValue;	// Isosurface value
uniform float Opacity;			// Opacity, usually within the interval [~0 .. 1] = [transparent .. solid]

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

	// Integrate over the volume
	for (int step=0; step<NumberOfSteps; step++) {
		// Call the clip position function
		//     bool ClipPosition(float3 Position)
		if (!ClipPosition(currentPosition)) {
			// Call the reconstruction function
			//     float Reconstruction(float3 Position)
			float scalar = Reconstruction(currentPosition);

			// Check for isosurface value match
			if (scalar > IsosurfaceValue) {
				// Intersection refinement as mentioned within the book "Real-Time Volume Graphics", section "14.5.1 Deferred Shading Pipeline" (page 374)
				// -> Improve image quality by sacrificing a bit of the performance
				StepPositionDelta *= -0.5;
				for (int i=0; i<4; i++) {
					// Update the current position inside the volume
					currentPosition += StepPositionDelta;

					// Call the reconstruction function
					//     float Reconstruction(float3 Position)
					scalar = Reconstruction(currentPosition);

					// Update the refine advance
					StepPositionDelta *= (scalar > IsosurfaceValue) ?  0.5 : -0.5;
				}

				// Call the shading function
				//     float4 Shading(float Scalar, float3 Position, float3 StepPositionDelta)
				float4 color = Shading(scalar, currentPosition, StepPositionDelta);

				// Ignore calculated opacity, this is a solid surface
				return float4(color.rgb, Opacity);
			}
		}

		// Update the current position inside the volume
		currentPosition += StepPositionDelta;
	}

	// No isosurface hit
	return float4(0.0, 0.0, 0.0, 0.0);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
