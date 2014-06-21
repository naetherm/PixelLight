/*********************************************************\
 *  File: Template_GLSL.h                                *
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


// Shader template: We want to be able to apply multiple clip ray functions at one and the same time
// -> From the outside, only one "ClipRay()"-function should be called to keep the overall structure manageable
// -> C++ configures this shader and adds multiple "ClipRay()"-function calls in here


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


// Template version
static const PLCore::String sSourceCode_Fragment_Template = STRINGIFY(
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
void ClipRay(inout vec3 RayOrigin, vec3 RayDirection, inout float MaximumTravelLength)
{
	_x_	// This will be replaced via C++ by real function calls
}
);	// STRINGIFY

// None-template version
static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// None-template version
void ClipRay(inout vec3 RayOrigin, vec3 RayDirection, inout float MaximumTravelLength)
{
	// Nothing to do in here
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
