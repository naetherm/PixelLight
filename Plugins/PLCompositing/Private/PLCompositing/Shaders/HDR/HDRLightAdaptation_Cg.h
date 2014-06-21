/*********************************************************\
 *  File: HDRLightAdaptation_Cg.h                        *
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


// Cg vertex shader source code
static const PLCore::String sHDRLightAdaptation_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float2 VertexPosition : POSITION)	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
{\n\
	VS_OUTPUT OUT;\n\
\n\
	// Pass through the vertex position\n\
	OUT.Position = float4(VertexPosition.xy, 0, 1);\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Cg fragment shader source code
static const PLCore::String sHDRLightAdaptation_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT		 IN					// Interpolated output from the vertex stage\n\
			 , uniform float	 Factor				// Interpolation factor\n\
			 , uniform sampler2D PreviousTexture	// Previous average luminance\n\
			 , uniform sampler2D CurrentTexture)	// Current average luminance\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Get the previous and current average luminance\n\
	float previouAverageLuminance = tex2D(PreviousTexture, float2(0.5f, 0.5f)).r;\n\
	float currentAverageLuminance = tex2D(CurrentTexture,  float2(0.5f, 0.5f)).r;\n\
\n\
	// Adapt the luminance using Pattanaik's technique\n\
	OUT.Color0 = lerp(previouAverageLuminance, currentAverageLuminance, Factor);\n\
\n\
	// Done\n\
	return OUT;\n\
}";
