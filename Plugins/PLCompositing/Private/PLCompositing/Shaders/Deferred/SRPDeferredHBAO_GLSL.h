/*********************************************************\
 *  File: SRPDeferredHBAO_GLSL.h                         *
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


// OpenGL 3.0 ("#version 130") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredHBAO_GLSL_VS = STRINGIFY(
// Attributes
in  vec4 VertexPosition;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
							// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
out vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (1,1)

// Programs
void main()
{
	// Set the clip space vertex position
	gl_Position = vec4(VertexPosition.xy, 0.0f, 1.0f);

	// Pass through the scaled vertex texture coordinate
	VertexTexCoordVS = VertexPosition.zw;
}
);	// STRINGIFY


// OpenGL 3.0 ("#version 130") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredHBAO_GLSL_FS = "\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Definitions\n\
#define M_PI 3.14159265f\n\
\n\
// Attributes\n\
in vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader, lower/left is (0,0) and upper/right is (1,1)\n\
\n\
// Uniforms\n\
uniform int				NumSteps;			// Number of steps (for example 8)\n\
uniform float			NumDir;				// Number of directions (for example 16) (should be of integer type, but that's causing troubles on some GPU drivers, see PLCompositing diary entry 02.06.2011 for more details)\n\
uniform float			Radius;				// Radius of influence (for example 1)\n\
uniform float			InvR;				// 1/Radius (for example 1)\n\
uniform float			SqrR;				// Radius*Radius (for example 1)\n\
uniform float			AngleBias;			// Angle bias (for example 0.523599)\n\
uniform float			TanAngleBias;		// Tangent of angle bias (for example 0.57735026)\n\
uniform float			Contrast;			// Contrast (for example 2.5)\n\
uniform float			Attenuation;		// Attenuation (for example 1)\n\
uniform vec2			FocalLen;			// Focal length (for example 1.7309455 x 2.4142134)\n\
uniform vec2			InvFocalLen;		// 1/FocalLen (for example: 0.57771897 x 0.4142136)\n\
uniform vec2			Resolution;			// Resolution of the input texture data (for example 636 x 456)\n\
uniform vec2			InvResolution;		// 1/Resolution (for example 0.001572327f x 0.0021929825f)\n\
uniform vec2			RandomUVScale;		// UV scale for the random normals texture\n\
uniform sampler2D		RandomNormalsMap;	// RGB texture with random normals\n\
uniform sampler2DRect	NormalDepthMap;		// RG=normal vector, B=linear view space length\n\
\n\
// Programs\n\
// Decodes a 2 component normal vector to a 3 component normal vector\n\
vec3 decodeNormalVector(vec2 normal)\n\
{\n\
	vec2 fenc = normal*4 - 2;\n\
	float f = dot(fenc, fenc);\n\
	float g = sqrt(1 - f/4);\n\
	vec3 n;\n\
	n.xy = fenc*g;\n\
	n.z = 1 - f/2;\n\
	return n;\n\
}\n\
\n\
// Reconstructs view-space position (current surface point) using given parameters\n\
vec3 uv_to_eye(vec2 uv, float eye_z, vec2 invFocalLen)\n\
{\n\
	// Convert from texture space [0,1] range into [-1,1] clip space\n\
	uv = uv*2 - 1;\n\
\n\
	// Reconstructs view-space position\n\
	return vec3(uv * invFocalLen * eye_z, eye_z);\n\
}\n\
\n\
// Reconstructs view-space position (current surface point) using a given texel\n\
vec3 fetch_eye_pos(sampler2DRect texture, vec2 textureResolution, vec2 uv, vec2 invFocalLen)\n\
{\n\
	// Fetch the linear view space depth at the given texel position\n\
	float eye_z = texture2DRect(texture, uv*textureResolution).b;\n\
\n\
	// Reconstructs view-space position using given parameters\n\
	return uv_to_eye(uv, eye_z, invFocalLen);\n\
}\n\
\n\
vec3 tangent_eye_pos(sampler2DRect texture, vec2 textureResolution, vec2 uv, vec2 invFocalLen, vec4 tangentPlane)\n\
{\n\
	// View vector going through the surface point at uv\n\
	vec3 V = fetch_eye_pos(texture, textureResolution, uv, invFocalLen);\n\
	float NdotV = dot(tangentPlane.xyz, V);\n\
\n\
	// Intersect with tangent plane except for silhouette edges\n\
	if (NdotV < 0.0)\n\
		V *= (tangentPlane.w / NdotV);\n\
\n\
	return V;\n\
}\n\
\n\
float length2(vec3 v)\n\
{\n\
	return dot(v, v);\n\
}\n\
\n\
vec3 min_diff(vec3 P, vec3 Pr, vec3 Pl)\n\
{\n\
	vec3 V1 = Pr - P;\n\
	vec3 V2 = P - Pl;\n\
	return (length2(V1) < length2(V2)) ? V1 : V2;\n\
}\n\
\n\
vec2 rotate_direction(vec2 dir, vec2 cosSin)\n\
{\n\
	return vec2(dir.x*cosSin.x - dir.y*cosSin.y,\n\
			    dir.x*cosSin.y + dir.y*cosSin.x);\n\
}\n\
\n\
vec2 snap_uv_offset(vec2 uv, vec2 resolution, vec2 invResolution)\n\
{\n\
	return round(uv * resolution) * invResolution;\n\
}\n\
\n\
float tangent(vec3 T)\n\
{\n\
	return -T.z / length(T.xy);\n\
}\n\
\n\
float tangent(vec3 P, vec3 S)\n\
{\n\
	return (P.z - S.z) / length(S.xy - P.xy);\n\
}\n\
\n\
float biased_tangent(vec3 T, float angleBias)\n\
{\n\
	float phi = atan(tangent(T)) + angleBias;\n\
	return tan(min(phi, M_PI*0.5f));\n\
}\n\
\n\
float falloff(float r, float attenuation)\n\
{\n\
	return 1 - attenuation*r*r;\n\
}\n\
\n\
// Program entry point\n\
void main()\n\
{\n\
	// Fetch the required texel data\n\
	vec4 sample = texture2DRect(NormalDepthMap, VertexTexCoordVS*Resolution);\n\
\n\
	// Reconstruct view-space position\n\
	vec3 P = uv_to_eye(VertexTexCoordVS, sample.b, InvFocalLen);\n\
\n\
	// Project the radius of influence Radius from view space to texture space.\n\
	// The scaling by 0.5f is to go from [-1,1] to [0,1].\n\
	vec2 step_size = 0.5f * Radius * FocalLen / P.z;\n\
\n\
	// Early out if the projected radius is smaller than 1 pixel.\n\
	float numSteps = min(NumSteps, min(step_size.x * Resolution.x, step_size.y * Resolution.y));\n\
	if (numSteps < 1) {\n\
		gl_FragColor = vec4(1);\n\
		return;\n\
	}\n\
	step_size = step_size / (numSteps + 1);\n\
\n\
	// Nearest neighbor pixels on the tangent plane\n\
	#ifdef FS_NORMAL\n\
		// Reconstruct view-space normal vector\n\
		vec3 N = decodeNormalVector(sample.rg);\n\
		N.z = -N.z; // If this is not done, we get wrong results\n\
\n\
		vec4 tangentPlane = vec4(N, dot(P, N));\n\
		vec3 Pr = tangent_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2( InvResolution.x,                0), InvFocalLen, tangentPlane);\n\
		vec3 Pl = tangent_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2(-InvResolution.x,                0), InvFocalLen, tangentPlane);\n\
		vec3 Pt = tangent_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2(               0,  InvResolution.y), InvFocalLen, tangentPlane);\n\
		vec3 Pb = tangent_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2(               0, -InvResolution.y), InvFocalLen, tangentPlane);\n\
	#else\n\
		vec3 Pr = fetch_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2( InvResolution.x,                0), InvFocalLen);\n\
		vec3 Pl = fetch_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2(-InvResolution.x,                0), InvFocalLen);\n\
		vec3 Pt = fetch_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2(               0,  InvResolution.y), InvFocalLen);\n\
		vec3 Pb = fetch_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS + vec2(               0, -InvResolution.y), InvFocalLen);\n\
	#endif\n\
\n\
	// Screen-aligned basis for the tangent plane\n\
	vec3 dPdu = min_diff(P, Pr, Pl);\n\
	vec3 dPdv = min_diff(P, Pt, Pb) * (Resolution.y * InvResolution.x);\n\
\n\
	// (cos(alpha), sin(alpha), jitter)\n\
	vec3 rand = texture2D(RandomNormalsMap, VertexTexCoordVS*RandomUVScale).xyz;\n\
\n\
	float aoFinal = 0;\n\
	float alpha = 2 * M_PI / NumDir;\n\
\n\
	// Calculate the AO\n\
	for (int d=0; d<int(NumDir); d++) {\n\
		float angle = alpha * d;\n\
		vec2 dir = vec2(cos(angle), sin(angle));\n\
		vec2 deltaUV = rotate_direction(dir, rand.xy) * step_size.xy;\n\
\n\
		{ // Accumulated horizon occlusion - Normal quality\n\
			// Randomize starting point within the first sample distance\n\
			vec2 uv = VertexTexCoordVS + snap_uv_offset(rand.z * deltaUV, Resolution, InvResolution);\n\
\n\
			// Snap increments to pixels to avoid disparities between xy\n\
			// and z sample locations and sample along a line\n\
			deltaUV = snap_uv_offset(deltaUV, Resolution, InvResolution);\n\
\n\
			// Compute tangent vector using the tangent plane\n\
			vec3 T = deltaUV.x * dPdu + deltaUV.y * dPdv;\n\
			float tanH = biased_tangent(T, AngleBias);\n\
			float sinH = tanH / sqrt(1 + tanH*tanH);\n\
\n\
			float ao = 0;\n\
			for (float j=1; j<=numSteps; ++j) {\n\
				uv += deltaUV;\n\
				vec3 S = fetch_eye_pos(NormalDepthMap, Resolution, uv, InvFocalLen);\n\
\n\
				// Ignore any samples outside the radius of influence\n\
				float d2  = length2(S - P);\n\
				if (d2 < SqrR) {\n\
					float tanS = tangent(P, S);\n\
\n\
					if(tanS > tanH) {\n\
						// Accumulate AO between the horizon and the sample\n\
						float sinS = tanS / sqrt(1 + tanS*tanS);\n\
						float r = sqrt(d2) * InvR;\n\
						ao += falloff(r, Attenuation) * (sinS - sinH);\n\
\n\
						// Update the current horizon angle\n\
						tanH = tanS;\n\
						sinH = sinS;\n\
					}\n\
				}\n\
			}\n\
			aoFinal += ao;\n\
		}\n\
	}\n\
	gl_FragColor = vec4(1 - aoFinal/NumDir*Contrast);\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
