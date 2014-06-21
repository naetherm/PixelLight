/*********************************************************\
 *  File: SRPDeferredEdgeAA_GLSL.h                       *
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


// OpenGL 2.1 ("#version 120") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredEdgeAA_GLSL_VS = "\
// Attributes\n\
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
varying   vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output\n\
\n\
// Uniforms\n\
uniform ivec2 TextureSize;	// Texture size in texel\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Set the clip space vertex position\n\
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	VertexTexCoordVS = VertexPosition.zw*vec2(TextureSize);\n\
}";


// OpenGL 2.1 ("#version 120") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredEdgeAA_GLSL_FS = "\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Attributes\n\
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader\n\
\n\
// Uniforms\n\
uniform float	MinGradient;			// Minimum gradient\n\
uniform float	WeightScale;			// Weight scale\n\
#ifdef FS_SHOW_EDGES\n\
	uniform vec3 EdgeColor;				// Edge color\n\
#endif\n\
uniform sampler2DRect FrontMap;			// Front texture\n\
uniform sampler2DRect NormalDepthMap;	// Normal depth texture\n\
\n\
// Neighbor offset table\n\
const vec2 Offsets[9] = vec2[9](\n\
	vec2( 0.0,  0.0), // Center       0\n\
	vec2(-1.0, -1.0), // Top left     1\n\
	vec2( 0.0, -1.0), // Top          2\n\
	vec2( 1.0, -1.0), // Top right    3\n\
	vec2( 1.0,  0.0), // Right        4\n\
	vec2( 1.0,  1.0), // Bottom right 5\n\
	vec2( 0.0,  1.0), // Bottom       6\n\
	vec2(-1.0,  1.0), // Bottom left  7\n\
	vec2(-1.0,  0.0)  // Left         8\n\
);\n\
\n\
// Programs\n\
// Decodes a 2 component normal vector to a 3 component normal vector\n\
vec3 decodeNormalVector(vec2 normal)\n\
{\n\
	vec2 fenc = normal*4.0 - 2.0;\n\
	float f = dot(fenc, fenc);\n\
	float g = sqrt(1.0 - f/4.0);\n\
	vec3 n;\n\
	n.xy = fenc*g;\n\
	n.z = 1.0 - f/2.0;\n\
	return n;\n\
}\n\
\n\
// Program entry point\n\
void main()\n\
{\n\
	// Retrieve normal and depth data for all neighbors\n\
	float depth[9];\n\
	vec3  normal[9];\n\
	for (int i=0; i<9; i++) {\n\
		// Retrieve sample at new location\n\
		vec3 sample = texture2DRect(NormalDepthMap, VertexTexCoordVS + Offsets[i]).xyz;\n\
\n\
		// Get depth and normal\n\
		depth[i]  = sample.b;\n\
		normal[i] = decodeNormalVector(sample.rg);\n\
	}\n\
\n\
	// Compute deltas in depth\n\
	vec4 deltas1;\n\
	vec4 deltas2;\n\
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
	vec4 maxDeltas = max(deltas1, deltas2);\n\
	vec4 minDeltas = max(min(deltas1, deltas2), MinGradient);\n\
\n\
	// Compare change in gradients, flagging ones that change significantly.\n\
	// How severe the change must be to get flagged is a function of the\n\
	// minimum gradient. It is not resolution dependent. The constant\n\
	// number here would change based on how the depth values are stored\n\
	// and how sensitive the edge detection should be.\n\
	vec4 depthResults = step(minDeltas*25.0, maxDeltas);\n\
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
	vec4 normalResults = step(0.4, deltas1);\n\
	normalResults = max(normalResults, depthResults);\n\
	float w = (normalResults.x + normalResults.y + normalResults.z + normalResults.w)*0.25; // 0=no aa, 1=full aa\n\
	if (w <= 0.0) {\n\
		// Early escape: No anti-aliasing required\n\
		#ifdef FS_SHOW_EDGESONLY\n\
			gl_FragColor = 0.0;\n\
		#else\n\
			gl_FragColor = texture2DRect(FrontMap, VertexTexCoordVS);\n\
		#endif\n\
		return;\n\
	}\n\
\n\
	// Scale the weight\n\
	w *= WeightScale;\n\
\n\
	// Perform anti-aliasing\n\
	#ifdef FS_SHOW_EDGES\n\
		gl_FragColor = vec4(EdgeColor, 1.0);\n\
	#else\n\
		// Smoothed color\n\
		#ifdef FS_MORE_SAMPLES\n\
			gl_FragColor = 0.0;\n\
			for (int i=0; i<9; i++)\n\
				gl_FragColor += texture2DRect(FrontMap, VertexTexCoordVS + Offsets[i]*w);\n\
			gl_FragColor /= 9.0;\n\
		#else\n\
			vec4 s0 = texture2DRect(FrontMap, VertexTexCoordVS + Offsets[2]*w);\n\
			vec4 s1 = texture2DRect(FrontMap, VertexTexCoordVS + Offsets[4]*w);\n\
			vec4 s2 = texture2DRect(FrontMap, VertexTexCoordVS + Offsets[6]*w);\n\
			vec4 s3 = texture2DRect(FrontMap, VertexTexCoordVS + Offsets[8]*w);\n\
			gl_FragColor = (s0 + s1 + s2 + s3)/4.0;\n\
		#endif\n\
	#endif\n\
}";
