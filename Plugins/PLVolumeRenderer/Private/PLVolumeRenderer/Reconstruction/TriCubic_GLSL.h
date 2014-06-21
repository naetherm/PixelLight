/*********************************************************\
 *  File: TriCubic_GLSL.h                                *
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


// Source: http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter20.html - "Example 20-2. Bicubic B-Spline Filtering" extended to 3D
// Source (optimizations): "CUDA Cubic B-Spline Interpolation (CI)" - http://www.dannyruijters.nl/cubicinterpolation/ - http://www.dannyruijters.nl/cubicinterpolation/CI.zip - "cubicTex3D_kernel.cu"


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


static const PLCore::String sSourceCode_Fragment_Header = STRINGIFY(
// Uniforms
uniform vec3 VolumeTextureSize;	// Volume texture size

/**
*  @brief
*    Reconstructs a scalar by using a given position inside the volume
*
*  @param[in] Position
*    Position inside the volume were to reconstruct the scalar, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    Reconstructed scalar, usually in the interval [0 .. 1]
*/
float Reconstruction(vec3 Position)
{
	// Shift the coordinate from [0, extent] to [-0.5, extent-0.5]
	// -> OpenGL centers texels at 0.5
	vec3 coord_grid = Position*VolumeTextureSize - vec3(0.5, 0.5, 0.5);
	vec3 index = floor(coord_grid);

	// Calculate weights using a cubic B-spline and calculate the periodic g and h functions
	vec3 fraction = coord_grid - index;
	vec3 one_frac = vec3(1.0, 1.0, 1.0) - fraction;
	vec3 squared = fraction*fraction;
	vec3 one_sqd = one_frac*one_frac;
	vec3 w0 = 1.0/6.0*one_sqd*one_frac;
	vec3 w1 = 2.0/3.0 - 0.5*squared*(2.0 - fraction);
	vec3 w2 = 2.0/3.0 - 0.5*one_sqd*(2.0 - one_frac);
	vec3 w3 = 1.0/6.0*squared*fraction;
	vec3 g0 = w0 + w1;
	vec3 g1 = w2 + w3;
	vec3 h0 = ((w1/g0) - 0.5 + index)/VolumeTextureSize;	// h0 = w1/g0 - 1, move from [-0.5, extent-0.5] to [0, extent]
	vec3 h1 = ((w3/g1) + 1.5 + index)/VolumeTextureSize;	// h1 = w3/g1 + 1, move from [-0.5, extent-0.5] to [0, extent]

	// Call the fetch scalar function
	//     float FetchScalar(vec3 Position)

	// Fetch the eight linear interpolations
	// -> Weighting and fetching is interleaved for performance and stability reasons
	float tex000 = FetchScalar(vec3(h0.x, h0.y, h0.z));
	float tex100 = FetchScalar(vec3(h1.x, h0.y, h0.z));
	tex000 = g0.x*tex000 + g1.x*tex100;	// Weight along the x-direction
	float tex010 = FetchScalar(vec3(h0.x, h1.y, h0.z));
	float tex110 = FetchScalar(vec3(h1.x, h1.y, h0.z));
	tex010 = g0.x*tex010 + g1.x*tex110;	// Weight along the x-direction
	tex000 = g0.y*tex000 + g1.y*tex010;	// Weight along the y-direction

	float tex001 = FetchScalar(vec3(h0.x, h0.y, h1.z));
	float tex101 = FetchScalar(vec3(h1.x, h0.y, h1.z));
	tex001 = g0.x*tex001 + g1.x*tex101;	// Weight along the x-direction
	float tex011 = FetchScalar(vec3(h0.x, h1.y, h1.z));
	float tex111 = FetchScalar(vec3(h1.x, h1.y, h1.z));
	tex011 = g0.x*tex011 + g1.x*tex111;	// Weight along the x-direction
	tex001 = g0.y*tex001 + g1.y*tex011;	// Weight along the y-direction

	return (g0.z*tex000 + g1.z*tex001);	// Weight along the z-direction
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
