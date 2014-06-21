/*********************************************************\
 *  File: SRPVolumetricFog_Cg.h                          *
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
static const PLCore::String sVolumetricFog_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
#ifdef VS_TEXCOORD0\n\
	float2 TexCoord : TEXCOORD0;	// Vertex texture coordinate 0\n\
#endif\n\
	float3 PositionVS : TEXCOORD1;	// World space vertex position\n\
	float3 FogVS      : TEXCOORD2;	// World space fog position\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float4 VertexPosition : POSITION			// Object space vertex position input\n\
	#ifdef VS_TEXCOORD0\n\
		, float2 VertexTexCoord0 : TEXCOORD0			// Vertex texture coordinate 0 input\n\
	#endif\n\
	, uniform float4x4 ObjectSpaceToWorldSpaceMatrix	// Object space to world space matrix\n\
	, uniform float4x4 ObjectSpaceToClipSpaceMatrix		// Object space to clip space matrix\n\
	, uniform float3   FogPosition						// World space fog position\n\
	, uniform float	   FogInvRadius						// World space fog inverse radius\n\
	, uniform float3   EyePos							// World space space eye position\n\
)\n\
{\n\
	VS_OUTPUT Out;\n\
\n\
	// Calculate the clip space vertex position\n\
	Out.Position = mul(ObjectSpaceToClipSpaceMatrix, VertexPosition);\n\
\n\
#ifdef VS_TEXCOORD0\n\
	// Pass through the vertex texture coordinate 0\n\
	Out.TexCoord = VertexTexCoord0;\n\
#endif\n\
\n\
	// Transform vertex position to world space\n\
	float4 worldSpacePosition = mul(ObjectSpaceToWorldSpaceMatrix, VertexPosition);\n\
	Out.PositionVS = (EyePos - worldSpacePosition.xyz)*FogInvRadius;\n\
\n\
	// Calculate world space fog data\n\
	Out.FogVS = (FogPosition - worldSpacePosition.xyz)*FogInvRadius;\n\
\n\
	// Done\n\
	return Out;\n\
}";


// Cg fragment shader source code
static const PLCore::String sVolumetricFog_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
#ifdef FS_DIFFUSEMAP\n\
	float2 TexCoord : TEXCOORD0;	// Vertex coordinate\n\
#endif\n\
	float3 PositionVS : TEXCOORD1;	// World space vertex position\n\
	float3 FogVS      : TEXCOORD2;	// World space fog position\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Programs\n\
FS_OUTPUT main(VS_OUTPUT In			// Vertex shader output as fragment shader input\n\
	#ifdef FS_DIFFUSEMAP\n\
		, uniform sampler2D DiffuseMap\n\
		#ifdef FS_ALPHATEST\n\
			, uniform float AlphaReference\n\
		#endif\n\
	#endif\n\
	, uniform float3 FogColor0\n\
	, uniform float3 FogColor1\n\
)\n\
{\n\
	FS_OUTPUT Out;\n\
\n\
#ifdef FS_DIFFUSEMAP\n\
	Out.Color0 = tex2D(DiffuseMap, In.TexCoord);\n\
	#ifdef FS_ALPHATEST\n\
		// Perform an alpha test to discard fragments\n\
		if (Out.Color0.a < AlphaReference)\n\
			discard; // Throw the fragment away and don't draw it!\n\
	#endif\n\
#endif\n\
\n\
	// Calculate the fog factor\n\
	float  k  = clamp(dot(In.FogVS, In.PositionVS)/dot(In.PositionVS, In.PositionVS), 0, 1);\n\
	float4 pl = float4(k*In.PositionVS - In.FogVS, 1.0);\n\
	Out.Color0.rgb = clamp(FogColor0/dot(pl, pl) - FogColor1, 0, 1);\n\
	Out.Color0.a = 1;\n\
\n\
	// Done\n\
	return Out;\n\
}";
