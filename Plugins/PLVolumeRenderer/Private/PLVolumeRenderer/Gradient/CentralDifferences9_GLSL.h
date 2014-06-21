/*********************************************************\
 *  File: CentralDifferences9_GLSL.h                     *
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


// Finite differences
// On-the-fly gradient computation approximating gradient vectors using the central differences scheme
// as described within the book "Real-Time Volume Graphics", section "5.3.1 Gradient Estimation" (page 109) and section "5.6 On-the-Fly Gradients" (page 127).
// -> Also described within the book "GPU Gems", section "39.4.1 Data Representation and Processing" (http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html)


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform vec3 GradientInvVolumeTextureSize;	// 1/<Volume texture size>

/**
*  @brief
*    Gradient via central difference
*
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    Gradient (not normalized)
*/
vec3 CentralDifference(vec3 Position)
{
	// Call the gradient input function by using a texel offset
	//     float GradientInputOffset(vec3 Position, ivec3 Offset)

	// 6 volume texture fetches for the gradient
	vec3 sample1;
	sample1.x = GradientInputOffset(Position, ivec3(-1,  0,  0));
	sample1.y = GradientInputOffset(Position, ivec3( 0, -1,  0));
	sample1.z = GradientInputOffset(Position, ivec3( 0,  0, -1));
	vec3 sample2;
	sample2.x = GradientInputOffset(Position, ivec3( 1,  0,  0));
	sample2.y = GradientInputOffset(Position, ivec3( 0,  1,  0));
	sample2.z = GradientInputOffset(Position, ivec3( 0,  0,  1));

	// Central difference
	return sample2 - sample1;
}

/**
*  @brief
*    Gradient
*
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    Gradient (not normalized)
*/
vec3 Gradient(vec3 Position)
{
	vec3 g0 = CentralDifference(Position);
	vec3 g1 = CentralDifference(Position + vec3(-GradientInvVolumeTextureSize.x, -GradientInvVolumeTextureSize.y, -GradientInvVolumeTextureSize.z));
	vec3 g2 = CentralDifference(Position + vec3( GradientInvVolumeTextureSize.x,  GradientInvVolumeTextureSize.y,  GradientInvVolumeTextureSize.z));
	vec3 g3 = CentralDifference(Position + vec3(-GradientInvVolumeTextureSize.x,  GradientInvVolumeTextureSize.y, -GradientInvVolumeTextureSize.z));
	vec3 g4 = CentralDifference(Position + vec3( GradientInvVolumeTextureSize.x, -GradientInvVolumeTextureSize.y,  GradientInvVolumeTextureSize.z));
	vec3 g5 = CentralDifference(Position + vec3(-GradientInvVolumeTextureSize.x, -GradientInvVolumeTextureSize.y,  GradientInvVolumeTextureSize.z));
	vec3 g6 = CentralDifference(Position + vec3( GradientInvVolumeTextureSize.x,  GradientInvVolumeTextureSize.y, -GradientInvVolumeTextureSize.z));
	vec3 g7 = CentralDifference(Position + vec3(-GradientInvVolumeTextureSize.x,  GradientInvVolumeTextureSize.y,  GradientInvVolumeTextureSize.z));
	vec3 g8 = CentralDifference(Position + vec3( GradientInvVolumeTextureSize.x, -GradientInvVolumeTextureSize.y, -GradientInvVolumeTextureSize.z));
	vec3 mix0 = mix(mix(g1, g2, 0.5), mix(g3, g4, 0.5), 0.5);
	vec3 mix1 = mix(mix(g5, g6, 0.5), mix(g7, g8, 0.5), 0.5);
	return mix(g0, mix(mix0, mix1, 0.5), 0.75);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
