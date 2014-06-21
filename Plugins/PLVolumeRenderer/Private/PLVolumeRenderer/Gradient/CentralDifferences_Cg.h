/*********************************************************\
 *  File: CentralDifferences_Cg.h                        *
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


// Finite differences
// On-the-fly gradient computation approximating gradient vectors using the central differences scheme
// as described within the book "Real-Time Volume Graphics", section "5.3.1 Gradient Estimation" (page 109) and section "5.6 On-the-Fly Gradients" (page 127).
// -> Also described within the book "GPU Gems", section "39.4.1 Data Representation and Processing" (http://http.developer.nvidia.com/GPUGems/gpugems_ch39.html)


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
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
float3 Gradient(float3 Position)
{
	// Call the gradient input function by using a texel offset
	//     float GradientInputOffset(float3 Position, int3 Offset)

	// 6 volume texture fetches for the gradient
	float3 sample1;
	sample1.x = GradientInputOffset(Position, int3(-1,  0,  0));
	sample1.y = GradientInputOffset(Position, int3( 0, -1,  0));
	sample1.z = GradientInputOffset(Position, int3( 0,  0, -1));
	float3 sample2;
	sample2.x = GradientInputOffset(Position, int3( 1,  0,  0));
	sample2.y = GradientInputOffset(Position, int3( 0,  1,  0));
	sample2.z = GradientInputOffset(Position, int3( 0,  0,  1));

	// Central difference
	return sample2 - sample1;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
