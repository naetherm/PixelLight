/*********************************************************\
 *  File: SRPDeferredEdgeAA_Cg.h                         *
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
static const PLCore::String sDeferredEdgeAA_Cg_VS = "\
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
static const PLCore::String sDeferredEdgeAA_Cg_FS = "\
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
// Neighbor offset table\n\
const static float2 Offsets[9] = {\n\
	float2( 0,  0), // Center       0\n\
	float2(-1, -1), // Top left     1\n\
	float2( 0, -1), // Top          2\n\
	float2( 1, -1), // Top right    3\n\
	float2( 1,  0), // Right        4\n\
	float2( 1,  1), // Bottom right 5\n\
	float2( 0,  1), // Bottom       6\n\
	float2(-1,  1), // Bottom left  7\n\
	float2(-1,  0)  // Left         8\n\
};\n\
\n\
// Programs\n\
// Decodes a 2 component normal vector to a 3 component normal vector\n\
float3 decodeNormalVector(float2 normal)\n\
{\n\
	float2 fenc = normal*4 - 2;\n\
	float f = dot(fenc, fenc);\n\
	float g = sqrt(1 - f/4);\n\
	float3 n;\n\
	n.xy = fenc*g;\n\
	n.z = 1 - f/2;\n\
	return n;\n\
}\n\
\n\
// Program entry point\n\
FS_OUTPUT main(VS_OUTPUT   IN				// Interpolated output from the vertex stage\n\
	 , uniform float	   MinGradient		// Minimum gradient\n\
	 , uniform float	   WeightScale		// Weight scale\n\
	#ifdef FS_SHOW_EDGES\n\
	 , uniform float3	   EdgeColor		// Edge color\n\
	#endif\n\
	 , uniform samplerRECT FrontMap			// Front texture\n\
	 , uniform samplerRECT NormalDepthMap)	// Normal depth texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Retrieve normal and depth data for all neighbors\n\
	float  depth[9];\n\
	float3 normal[9];\n\
	for (int i=0; i<9; i++) {\n\
		// Retrieve sample at new location\n\
		float3 sample = texRECT(NormalDepthMap, IN.TexCoord + Offsets[i]).xyz;\n\
\n\
		// Get depth and normal\n\
		depth[i]  = sample.b;\n\
		normal[i] = decodeNormalVector(sample.rg);\n\
	}\n\
\n\
	// Compute deltas in depth\n\
	float4 deltas1;\n\
	float4 deltas2;\n\
	deltas1.x = depth[1];\n\
	deltas1.y = depth[2];\n\
	deltas1.z = depth[3];\n\
	deltas1.w = depth[4];\n\
	deltas2.x = depth[5];\n\
	deltas2.y = depth[6];\n\
	deltas2.z = depth[7];\n\
	deltas2.w = depth[8];\n\
\n\
	// Compute absolute gradients from center\n\
	deltas1 = abs(deltas1 - depth[0]);\n\
	deltas2 = abs(depth[0] - deltas2);\n\
\n\
	// Find min and max gradient, ensuring min != 0\n\
	float4 maxDeltas = max(deltas1, deltas2);\n\
	float4 minDeltas = max(min(deltas1, deltas2), MinGradient);\n\
\n\
	// Compare change in gradients, flagging ones that change significantly.\n\
	// How severe the change must be to get flagged is a function of the\n\
	// minimum gradient. It is not resolution dependent. The constant\n\
	// number here would change based on how the depth values are stored\n\
	// and how sensitive the edge detection should be.\n\
	float4 depthResults = step(minDeltas*25.0f, maxDeltas);\n\
\n\
	// Compute change in the cosine of the angle between normals\n\
	deltas1.x = dot(normal[1], normal[0]);\n\
	deltas1.y = dot(normal[2], normal[0]);\n\
	deltas1.z = dot(normal[3], normal[0]);\n\
	deltas1.w = dot(normal[4], normal[0]);\n\
	deltas2.x = dot(normal[5], normal[0]);\n\
	deltas2.y = dot(normal[6], normal[0]);\n\
	deltas2.z = dot(normal[7], normal[0]);\n\
	deltas2.w = dot(normal[8], normal[0]);\n\
	deltas1 = abs(deltas1 - deltas2);\n\
\n\
	// Compare change in the cosine of the angles, flagging changes\n\
	// above some constant threshold. The cosine of the angle is not a\n\
	// linear function of the angle, so to have the flagging be\n\
	// independent of the angles involved, an arccos function would be\n\
	// required.\n\
	float4 normalResults = step(0.4f, deltas1);\n\
	normalResults = max(normalResults, depthResults);\n\
	float w = (normalResults.x + normalResults.y + normalResults.z + normalResults.w)*0.25f; // 0=no aa, 1=full aa\n\
	if (w <= 0) {\n\
		// Early escape: No anti-aliasing required\n\
		#ifdef FS_SHOW_EDGESONLY\n\
			OUT.Color0 = 0;\n\
		#else\n\
			OUT.Color0 = texRECT(FrontMap, IN.TexCoord);\n\
		#endif\n\
		return OUT;\n\
	}\n\
\n\
	// Scale the weight\n\
	w *= WeightScale;\n\
\n\
	// Perform anti-aliasing\n\
	#ifdef FS_SHOW_EDGES\n\
		OUT.Color0 = float4(EdgeColor, 1);\n\
	#else\n\
		// Smoothed color\n\
		#ifdef FS_MORE_SAMPLES\n\
			OUT.Color0 = 0;\n\
			for (int i=0; i<9; i++)\n\
				OUT.Color0 += texRECT(FrontMap, IN.TexCoord + Offsets[i]*w);\n\
			OUT.Color0 /= 9;\n\
		#else\n\
			float4 s0 = texRECT(FrontMap, IN.TexCoord + Offsets[2]*w);\n\
			float4 s1 = texRECT(FrontMap, IN.TexCoord + Offsets[4]*w);\n\
			float4 s2 = texRECT(FrontMap, IN.TexCoord + Offsets[6]*w);\n\
			float4 s3 = texRECT(FrontMap, IN.TexCoord + Offsets[8]*w);\n\
			OUT.Color0 = (s0 + s1 + s2 + s3)/4.0f;\n\
		#endif\n\
	#endif\n\
\n\
	// Done\n\
	return OUT;\n\
}";
