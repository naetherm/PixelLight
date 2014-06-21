/*********************************************************\
 *  File: TransferFunctionPreMultiplied_GLSL.h           *
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


// Pre-multiplied (no color bleeding) post-interpolative transfer function
// -> Is using pre-multiplied colors by their corresponding opacity value as described within the book "Real-Time Volume Graphics", section "3.2.3 Compositing" (page 54)


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform sampler1D TransferFunctionTexture;	// Transfer function texture map

/**
*  @brief
*    Scalar classification
*
*  @param[in] Scalar
*    Scalar to perform a classification on
*
*  @return
*    RGBA result of the classification
*/
vec4 Classification(float Scalar)
{
	// Apply 1D transfer function
	vec4 sample = textureLod(TransferFunctionTexture, Scalar, 0.0);

	// Revert pre-multiplication with opacity we did in order to avoid color bleeding during interpolation
	sample.rgb *= 1.0/sample.a;

	// [TODO] Only required for HDR rendering with gamma correction -> Add a trigger
	// [TODO] For better performance, put this directly within the 1D texture
	// Perform sRGB to linear space conversion (gamma correction)
	sample.rgb = pow(sample.rgb, vec3(2.2, 2.2, 2.2));

	// Return the result of the scalar classification
	return sample;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
