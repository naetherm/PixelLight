/*********************************************************\
 *  File: HDRLightAdaptation_GLSL.h                      *
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


// GLSL vertex shader source code
static const PLCore::String sHDRLightAdaptation_GLSL_VS = "\
// GLSL preprocessor directives\n\
#version 110	// OpenGL 2.0\n\
\n\
// Attributes\n\
attribute vec4 VertexPosition;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Set the clip space vertex position\n\
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);\n\
}";


// GLSL fragment shader source code
static const PLCore::String sHDRLightAdaptation_GLSL_FS = "\
// GLSL preprocessor directives\n\
#version 110	// OpenGL 2.0\n\
\n\
// Uniforms\n\
uniform float		Factor;				// Interpolation factor\n\
uniform sampler2D	PreviousTexture;	// Previous average luminance\n\
uniform sampler2D	CurrentTexture;		// Current average luminance\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Get the previous and current average luminance\n\
	float previouAverageLuminance = texture2D(PreviousTexture, vec2(0.5, 0.5)).r;\n\
	float currentAverageLuminance = texture2D(CurrentTexture,  vec2(0.5, 0.5)).r;\n\
\n\
	// Adapt the luminance using Pattanaik's technique\n\
	float value = mix(previouAverageLuminance, currentAverageLuminance, Factor);\n\
	gl_FragColor = vec4(value, value, value, value);\n\
}";
