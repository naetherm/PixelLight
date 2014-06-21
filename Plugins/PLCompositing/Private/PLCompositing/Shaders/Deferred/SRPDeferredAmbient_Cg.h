/*********************************************************\
 *  File: SRPDeferredAmbient_Cg.h                        *
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
static const PLCore::String sDeferredAmbient_Cg_VS = "\
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
static const PLCore::String sDeferredAmbient_Cg_FS = "\
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
FS_OUTPUT main(VS_OUTPUT   IN					// Interpolated output from the vertex stage\n\
	 , uniform float3      AmbientColor			// Ambient color\n\
	 , uniform samplerRECT AlbedoMap			// Albedo texture\n\
#ifdef FS_SELFILLUMINATION\n\
	 , uniform samplerRECT SelfIlluminationMap	// Self illumination texture\n\
#endif\n\
	 )\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Fetch the required texel data\n\
	float4 sample = texRECT(AlbedoMap, IN.TexCoord);\n\
\n\
	// Apply albedo and ambient color\n\
#ifdef FS_ALBEDO\n\
	OUT.Color0.rgb = sample.rgb*AmbientColor;\n\
#else\n\
	OUT.Color0.rgb = AmbientColor;\n\
#endif\n\
\n\
	// Apply ambient occlusion\n\
#ifdef FS_AMBIENTOCCLUSION\n\
	OUT.Color0.rgb *= sample.a;\n\
#endif\n\
\n\
	// Apply self illumination\n\
#ifdef FS_SELFILLUMINATION\n\
	OUT.Color0.rgb += texRECT(SelfIlluminationMap, IN.TexCoord).rgb;\n\
#endif\n\
\n\
	// Alpha is always 1\n\
	OUT.Color0.a = 1;\n\
\n\
	// Done\n\
	return OUT;\n\
}";
