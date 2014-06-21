/*********************************************************\
 *  File: Classified_GLSL.h                              *
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


static const PLCore::String sSourceCode_Fragment_Part = STRINGIFY(
/**
*  @brief
*    Fetches the gradient input by using a given position inside the volume
*
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    Gradient input
*/
float GradientInput(vec3 Position)
{
	// Call the fetch scalar function
	//     float FetchScalar(vec3 Position)
	// Call the classification function
	//     vec4 Classification(float Scalar)
	return Classification(FetchScalar(Position)).a;
}
);	// STRINGIFY

/**
*  @brief
*    Fetches the gradient input by using a given position inside the volume by using a texel offset
*
*  @param[in] Position
*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] Offset
*    Texel offset
*
*  @return
*    Gradient input
*/
// Call the fetch scalar function by using a texel offset
//     float FetchScalarOffset(vec3 Position, ivec3 Offset)
// Call the classification function
//     vec4 Classification(float Scalar)
// When realizing this as function, I received the following GLSL compiler error:
//   "error(#30) Built-in function parameter type check fail: offset must be a constant/literal in texture functions Offset should be a constant expression"
// -> According to the GLSL specification ("The OpenGL® Shading Language - Language Version: 3.30 - Document Revision: 6 - 11-Mar-2010") this behaviour correct in general
// -> When resolving functions, the GLSL compiler was unable to detect that the given parameter was actually constant/literal
// -> So, this one in here is realized by using a macro instead of a function
// -> Used system: "ATI Mobility Radeon HD 4850" (512 MiB) using Catalyst 12.1 and Windows 7 64 bit
static const PLCore::String sSourceCode_Fragment = sSourceCode_Fragment_Part + "\n#define GradientInputOffset(Position, Offset) Classification(FetchScalarOffset(Position, Offset)).a\n";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
