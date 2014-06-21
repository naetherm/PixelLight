/*********************************************************\
 *  File: SRPDeferredSPAAO_GLSL.h                        *
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


// OpenGL 2.1 ("#version 120") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredSPAAO_GLSL_VS = STRINGIFY(
// Attributes
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
varying   vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (1,1)

// Programs
void main()
{
	// Set the clip space vertex position
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);

	// Pass through the scaled vertex texture coordinate
	VertexTexCoordVS = VertexPosition.zw;
}
);	// STRINGIFY


// OpenGL 2.1 ("#version 120") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredSPAAO_GLSL_FS =
// GLSL extensions
"#extension GL_ARB_texture_rectangle : enable\n"
STRINGIFY(
// Attributes
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader, lower/left is (0,0) and upper/right is (1,1)

// Uniforms
uniform float			SampleRadius;		// The sampling radius
uniform float			Intensity;			// The ambient occlusion intensity
uniform float			Scale;				// Scales distance between occludes and occludee
uniform float			Bias;				// Controls the width of the occlusion cone considered by the occludee
uniform vec2			InvFocalLen;		// 1/FocalLen (for example: 0.57771897 x 0.4142136)
uniform int				RandomSize;			// Resolution of the input random data (for example 64)
uniform sampler2D		RandomNormalsMap;	// RG=normal vector, B=linear view space length
uniform ivec2			Resolution;			// Resolution of the input texture data (for example 636 x 456)
uniform sampler2DRect	NormalDepthMap;		// RG=normal vector, B=linear view space length

// Programs
// Decodes a 2 component normal vector to a 3 component normal vector
vec3 decodeNormalVector(vec2 normal)
{
	vec2 fenc = normal*4.0 - 2.0;
	float f = dot(fenc, fenc);
	float g = sqrt(1.0 - f/4.0);
	vec3 n;
	n.xy = fenc*g;
	n.z = 1.0 - f/2.0;
	return n;
}

// Reconstructs view-space position (current surface point) using given parameters
vec3 uv_to_eye(vec2 uv, float eye_z, vec2 invFocalLen)
{
	// Convert from texture space [0,1] range into [-1,1] clip space
	uv = uv*2.0 - 1.0;

	// Reconstructs view-space position
	return vec3(uv * invFocalLen * eye_z, eye_z);
}

// Reconstructs view-space position (current surface point) using a given texel
vec3 fetch_eye_pos(sampler2DRect texture, ivec2 textureResolution, vec2 uv, vec2 invFocalLen)
{
	// Fetch the linear view space depth at the given texel position
	float eye_z = texture2DRect(texture, uv*vec2(textureResolution)).b;

	// Reconstructs view-space position using given parameters
	return uv_to_eye(uv, eye_z, invFocalLen);
}

// AO calculation
float doAmbientOcclusion(vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm)
{
	vec3 diff = fetch_eye_pos(NormalDepthMap, Resolution, tcoord + uv, InvFocalLen) - p;
	vec3 v = normalize(diff);
	float d = length(diff)*Scale;
	return max(0.0, dot(cnorm, v) - Bias)*(1.0/(1.0 + d))*Intensity;
}


const vec2 vec[4] = vec2[4](
	vec2( 1.0,  0.0),
	vec2(-1.0,  0.0),
	vec2( 0.0,  1.0),
	vec2( 0.0, -1.0)
);


// Program entry point
void main()
{
	// Fetch the required texel data
	vec4 sample = texture2DRect(NormalDepthMap, VertexTexCoordVS*vec2(Resolution));

	// Reconstruct view-space position
	vec3 p = uv_to_eye(VertexTexCoordVS, sample.b, InvFocalLen);

	// Reconstruct view-space normal vector
	vec3 n = decodeNormalVector(sample.rg);
	n.z = -n.z; // If this is not done, we get wrong results

	// Locals
	vec2 rand = normalize(texture2D(RandomNormalsMap, vec2(Resolution)*VertexTexCoordVS/float(RandomSize)).xy*2.0 - 1.0);	// Returns a random normal vector
	float ao = 0.0;
	float rad = SampleRadius/p.z;

	// SSAO calculation
	int iterations = 4;
	for (int j=0; j<iterations; j++) {
		vec2 coord1 = reflect(vec[j], rand)*rad;
		vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707, coord1.x*0.707 + coord1.y*0.707);
		ao += doAmbientOcclusion(VertexTexCoordVS, coord1*0.25, p, n);
		ao += doAmbientOcclusion(VertexTexCoordVS, coord2*0.5,  p, n);
		ao += doAmbientOcclusion(VertexTexCoordVS, coord1*0.75, p, n);
		ao += doAmbientOcclusion(VertexTexCoordVS, coord2,      p, n);
	}
	ao /= float(iterations)*4.0;
	ao = 1.0 - clamp(ao, 0.0, 1.0);
	gl_FragColor = vec4(ao, ao, ao, ao);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
