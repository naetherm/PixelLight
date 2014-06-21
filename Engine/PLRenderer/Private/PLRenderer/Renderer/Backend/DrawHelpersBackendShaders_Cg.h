/*********************************************************\
 *  File: DrawHelpersBackendShaders_Cg.cpp               *
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
static const PLCore::String sVertexShaderSourceCodeCg = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;			// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	#ifdef VS_TEXCOORD0\n\
		float2 TexCoord : TEXCOORD0;	// Vertex texture coordinate\n\
	#endif\n\
	#ifdef VS_VERTEXCOLOR\n\
		float4 Color : COLOR0;			// Vertex color\n\
	#endif\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float3 VertexPosition : POSITION			// Object space vertex position input\n\
	#ifdef VS_TEXCOORD0\n\
		, float2 VertexTextureCoordinate : TEXCOORD0	// Vertex texture coordinate input\n\
		, uniform float4x4 TextureMatrix				// Texture matrix\n\
	#endif\n\
	#ifdef VS_VERTEXCOLOR\n\
		, float4 VertexColor : COLOR0					// Vertex color input\n\
	#endif\n\
	, uniform float4x4 ObjectSpaceToClipSpaceMatrix		// Object space to clip space matrix\n\
)\n\
{\n\
	VS_OUTPUT Out;\n\
\n\
	// Calculate the clip space vertex position\n\
	Out.Position = mul(ObjectSpaceToClipSpaceMatrix, float4(VertexPosition, 1));\n\
\n\
#ifdef VS_TEXCOORD0\n\
	// Pass through the texture coordinate\n\
	Out.TexCoord = mul(TextureMatrix, float4(VertexTextureCoordinate, 1, 1)).xy;\n\
#endif\n\
\n\
#ifdef VS_VERTEXCOLOR\n\
	// Pass through the color\n\
	Out.Color = VertexColor;\n\
#endif\n\
\n\
	// Done\n\
	return Out;\n\
}";


// Cg fragment shader source code
static const PLCore::String sFragmentShaderSourceCodeCg = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;			// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	#ifdef FS_DIFFUSEMAP\n\
		float2 TexCoord : TEXCOORD0;	// Vertex texture coordinate\n\
	#endif\n\
	#ifdef FS_VERTEXCOLOR\n\
		float4 Color : COLOR0;			// Vertex color\n\
	#endif\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Programs\n\
FS_OUTPUT main(VS_OUTPUT In						// Vertex shader or geometry shader output as fragment shader input\n\
	#ifndef FS_VERTEXCOLOR\n\
		, uniform float4 Color					// Color\n\
	#endif\n\
	#ifdef FS_DIFFUSEMAP\n\
		#ifdef FS_DIFFUSEMAP2D\n\
			, uniform sampler2D DiffuseMap		// 2D diffuse map\n\
		#else\n\
			, uniform samplerRECT DiffuseMap	// Rectangle diffuse map\n\
		#endif\n\
		#ifdef FS_ALPHATEST\n\
			, uniform float AlphaReference		// Alpha reference\n\
		#endif\n\
	#endif\n\
)\n\
{\n\
	FS_OUTPUT Out;\n\
\n\
#ifdef FS_DIFFUSEMAP\n\
	#ifdef FS_DIFFUSEMAP2D\n\
		Out.Color0 = tex2D(DiffuseMap, In.TexCoord);\n\
	#else\n\
		Out.Color0 = texRECT(DiffuseMap, In.TexCoord);\n\
	#endif\n\
	#ifdef FS_ALPHATEST\n\
		// Perform an alpha test to discard fragments\n\
		if (Out.Color0.a < AlphaReference)\n\
			discard; // Throw the fragment away and don't draw it!\n\
	#endif\n\
\n\
	// Still here? Then apply the given color...\n\
	#ifdef FS_VERTEXCOLOR\n\
		Out.Color0 *= In.Color;\n\
	#else\n\
		Out.Color0 *= Color;\n\
	#endif\n\
#else\n\
	#ifdef FS_VERTEXCOLOR\n\
		Out.Color0 = In.Color;\n\
	#else\n\
		Out.Color0 = Color;\n\
	#endif\n\
#endif\n\
\n\
	// Done\n\
	return Out;\n\
}";
