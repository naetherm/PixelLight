/*********************************************************\
 *  File: SRPDeferredGodRays_Cg.h                        *
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


// Cg vertex shader source code
static const PLCore::String sDeferredGodRays_Cg_VS = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)
};

// Programs
VS_OUTPUT main(float4 VertexPosition : POSITION,	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
	   uniform int2	  TextureSize)					// Texture size in texel
{
	VS_OUTPUT OUT;

	// Pass through the vertex position
	OUT.Position = float4(VertexPosition.xy, 0, 1);

	// Pass through the scaled vertex texture coordinate
	OUT.TexCoord = VertexPosition.zw*TextureSize;

	// Done
	return OUT;
}
);	// STRINGIFY


// Cg fragment shader source code
static const PLCore::String sDeferredGodRays_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Programs\n\
FS_OUTPUT main(VS_OUTPUT   IN				// Interpolated output from the vertex stage\n\
	 , uniform float       NumberOfSamples	// Number of samples, must be >0 (should be of integer type, but that's causing troubles on some GPU drivers, see PLCompositing diary entry 02.06.2011 for more details)\n\
	 , uniform float       Density		    // Density, must be >0\n\
	 , uniform float       Weight		    // Weight\n\
	 , uniform float       Decay		    // Decay\n\
	 , uniform float2      LightPosition	// Screen space light position, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
	 , uniform float3      Color			// Color\n\
	 , uniform samplerRECT Map)				// Texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Calculate vector from pixel to light source in screen space\n\
	float2 texUV = IN.TexCoord;\n\
	float2 deltaTexUV = texUV - LightPosition;\n\
\n\
	// Divide by number of samples and scale by control factor\n\
	deltaTexUV *= 1.0f/NumberOfSamples*Density;\n\
\n\
	// Store initial sample\n\
	float3 color = texRECT(Map, IN.TexCoord).rgb;\n\
\n\
	// Set up illumination decay factor\n\
	float illuminationDecay = 1;\n\
\n\
	// Evaluate summation\n\
	for (int i=0; i<int(NumberOfSamples); i++) {\n\
		// Step sample location along ray\n\
		texUV -= deltaTexUV;\n\
\n\
		// Retrieve sample at new location\n\
		float3 sample = texRECT(Map, texUV).rgb;\n\
\n\
		// Apply sample attenuation scale/decay factors\n\
		sample *= illuminationDecay*Weight;\n\
\n\
		// Accumulate combined color\n\
		color += sample;\n\
\n\
		// Update exponential decay factor\n\
		illuminationDecay *= Decay;\n\
	}\n\
\n\
	// Output final color with a further scale control factor\n\
	float3 resultingColor = color*Color;\n\
\n\
	// Use discard?\n\
#ifdef FS_DISCARD\n\
	if (resultingColor.r == 0 && resultingColor.g == 0 && resultingColor.b == 0)\n\
		discard;\n\
#endif\n\
\n\
	// Set fragment color\n\
	OUT.Color0 = float4(resultingColor, 1);\n\
\n\
	// Done\n\
	return OUT;\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
