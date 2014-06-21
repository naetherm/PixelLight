/*********************************************************\
 *  File: SRPDeferredDepthFog_Cg.h                       *
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
static const PLCore::String sDeferredDepthFog_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float4 VertexPosition : POSITION,	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
	   uniform int2	  TextureSize)					// Texture size in texel\n\
{\n\
	VS_OUTPUT OUT;\n\
\n\
	// Pass through the vertex position\n\
	OUT.Position = float4(VertexPosition.xy, 0, 1);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	OUT.TexCoord = VertexPosition.zw*TextureSize;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Cg fragment shader source code
static const PLCore::String sDeferredDepthFog_Cg_FS = "\
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
	 , uniform float	   FarPlane			// Far clipping plane (never 0!)\n\
	 , uniform float4	   FogColor			// Fog color\n\
	#ifdef FS_LINEAR_MODE\n\
	 , uniform float	   FogEnd			// Fog end\n\
	 , uniform float	   FogRange			// Fog end - fog start (never 0!)\n\
	#else\n\
	 , uniform float	   FogDensity		// Fog density\n\
	#endif\n\
	 , uniform samplerRECT NormalDepthMap)	// Normal depth texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Get the depth\n\
	float depth = texRECT(NormalDepthMap, IN.TexCoord).b/FarPlane;\n\
\n\
	// Calculate the fog intensity [0...1] = [full fog...no fog]\n\
	#define E 2.71828 // Natural logarithm\n\
#ifdef FS_LINEAR_MODE\n\
	float fogFactor = (FogEnd - depth)/FogRange;\n\
#elif defined FS_EXPONENTIAL_MODE\n\
	// The pow parameters are safe, so no NAN can be produced in here\n\
	float fogFactor = 1/pow(E, depth*FogDensity);\n\
#elif defined FS_EXPONENTIAL2_MODE\n\
	// The pow parameters are safe, so no NAN can be produced in here\n\
	float fogFactor = 1/pow(E, depth*depth*FogDensity*FogDensity);\n\
#else\n\
	float fogFactor = 0;\n\
#endif\n\
\n\
	// Apply the fog color\n\
	OUT.Color0 = (1 - clamp(fogFactor, 0.0f, 1.0f))*FogColor;\n\
\n\
	// Done\n\
	return OUT;\n\
}";
