/*********************************************************\
 *  File: Plane_GLSL.h                                   *
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


// Template version
static const PLCore::String sSourceCode_Fragment_Template = STRINGIFY(
// Uniforms
uniform vec4 ClipPlane_x_;	// Clip plane in Hesse normal form in scene node space (xyz=N0, w=p)

/**
*  @brief
*    Performs an clipping operation on the given ray
*
*  @param[in, out] RayOrigin
*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in]      RayDirection
*    Normalized ray direction
*  @param[in, out] MaximumTravelLength
*    Maximum travel length along the ray, within the interval [0 .. 1]
*/
void ClipRay_x_(inout vec3 RayOrigin, vec3 RayDirection, inout float MaximumTravelLength)
{
	// Calculate the ray end position
	vec3 rayEnd = RayOrigin + RayDirection*MaximumTravelLength;

	// Calculate the distance from the ray start and end position to the plane
	float rayOriginDistanceFromPlane = dot(ClipPlane_x_.xyz, RayOrigin) + ClipPlane_x_.w;
	float rayEndDistanceFromPlane = dot(ClipPlane_x_.xyz, rayEnd) + ClipPlane_x_.w;

	// Are both positions on the same side of the plane?
	if (rayOriginDistanceFromPlane*rayEndDistanceFromPlane > 0.0) {
		// Both positions are on the same side of the plane, are we completely clipped?
		if (rayOriginDistanceFromPlane < 0.0)
			MaximumTravelLength = -1.0;
	} else {
		// We intersect the plane, do we need to update the ray origin or the ray end?
		if (rayOriginDistanceFromPlane < 0.0) {
			// Update the ray origin
			RayOrigin -= RayDirection*(rayOriginDistanceFromPlane/dot(ClipPlane_x_.xyz, RayDirection));
		} else {
			// Update the ray end
			rayEnd -= RayDirection*(rayEndDistanceFromPlane/dot(ClipPlane_x_.xyz, RayDirection));
		}

		// Update maximum travel length along the ray
		float newLength = length(rayEnd - RayOrigin);

		// Security check: When something goes totally wrong and the maximum travel length along the ray
		// is increased instead of decreased we can overload the GPU which in worst case may result in
		// a total operation system stop, this has to be avoided, so no risk in here
		if (MaximumTravelLength > newLength)
			MaximumTravelLength = newLength;
	}
}
);	// STRINGIFY

// None-template version
static const PLCore::String sSourceCode_Fragment = sSourceCode_Fragment_Template + '\n' + STRINGIFY(
// None-template version
void ClipRay(inout vec3 RayOrigin, vec3 RayDirection, inout float MaximumTravelLength)
{
	// Call the function version also used by the template
	ClipRay_x_(RayOrigin, RayDirection, MaximumTravelLength);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
