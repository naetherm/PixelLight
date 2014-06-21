/*********************************************************\
 *  File: ForwardDifferences_Cg.h                        *
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
// On-the-fly gradient computation approximating gradient vectors using the forward differences scheme
// as described within the book "Real-Time Volume Graphics", section "5.3.1 Gradient Estimation" (page 109).
// -> Also described within "ACM SIGGRAPH Asia 2008 - Course : Advanced Illumination Techniques for GPU Volume Raycasting" (http://www.voreen.org/files/sa08-coursenotes_1.pdf), section "3.2 Gradient Computation"


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
	//     float GradientInputOffset(vec3 Position, int3 Offset)

	// 4 volume texture fetches for the gradient
	float value  = GradientInput(Position);
	float valueX = GradientInputOffset(Position, int3(1, 0, 0));
	float valueY = GradientInputOffset(Position, int3(0, 1, 0));
	float valueZ = GradientInputOffset(Position, int3(0, 0, 1));

	// Forward differences
	return float3(valueX - value, valueY - value, valueZ - value);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
