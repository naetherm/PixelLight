/*********************************************************\
 *  File: SRPDeferredGBuffer_GLSL.h                      *
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
*********************************************************/


// OpenGL 2.1 ("#version 120") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredGBuffer_GLSL_VS = "\
// In attributes\n\
attribute vec4 VertexPosition;		// Object space vertex position\n\
attribute vec2 VertexTexCoord0;		// Vertex texture coordinate\n\
#ifdef VS_SECONDTEXTURECOORDINATE\n\
	attribute vec2 VertexTexCoord1;	// Vertex ambient occlusion map and/or light map texture coordinate\n\
#endif\n\
attribute vec3 VertexNormal;		// Object space vertex normal\n\
#ifdef VS_TANGENT_BINORMAL\n\
	attribute vec3 VertexTangent;	// Object space vertex tangent\n\
	attribute vec3 VertexBinormal;	// Object space vertex tangent\n\
#endif\n\
\n\
// Out attributes\n\
#ifdef VS_SECONDTEXTURECOORDINATE\n\
	varying vec4 TexCoordVS;		// Vertex texture coordinate, zw for ambient occlusion map and/or light map texture coordinate output\n\
#else\n\
	varying vec2 TexCoordVS;		// Vertex texture coordinate output\n\
#endif\n\
varying vec4 NormalDepthVS;			// View space vertex normal and view space linear depth [0...far plane] output\n\
varying vec3 TangentVS;				// View space vertex tangent output\n\
varying vec3 BinormalVS;			// View space vertex tangent output\n\
varying vec3 EyeVecVS;				// Tangent space vector pointing from the pixel to the eye point output\n\
#ifdef VS_VIEWSPACEPOSITION\n\
	varying vec3 PositionVS;		// View space vertex position output\n\
#endif\n\
\n\
// Uniforms\n\
#ifdef VS_TWOSIDED\n\
	uniform float NormalScale;					// Normal scale (negative to flip normals)\n\
#endif\n\
uniform vec3 EyePos;							// Object space eye position\n\
uniform mat4 WorldVP;							// World view projection matrix\n\
uniform mat4 WorldV;							// World view matrix\n\
#ifdef VS_DISPLACEMENTMAP\n\
	uniform sampler2D DisplacementMap;			// Displacement map\n\
	uniform vec2      DisplacementScaleBias;	// Displacement scale and bias\n\
#endif\n\
\n\
// Programs\n\
void main()\n\
{\n\
#ifdef VS_DISPLACEMENTMAP\n\
	// Sample displacement map\n\
	float displacement = texture(DisplacementMap, vec4(VertexTexCoord0, 0.0, 0.0)).r;\n\
	displacement = (displacement*DisplacementScaleBias.x) + DisplacementScaleBias.y;\n\
	vec4 vertexPosition = vec4(VertexPosition.xyz + displacement*VertexNormal, 1.0);\n\
#else\n\
	#define vertexPosition VertexPosition\n\
#endif\n\
\n\
	// Transform vertex position to clip space, lower/left is (-1,-1) and upper/right is (1,1)\n\
	gl_Position = WorldVP*vertexPosition;\n\
\n\
	// Pass through the vertex texture coordinate\n\
	TexCoordVS.xy = VertexTexCoord0;\n\
\n\
	// Pass through the vertex ambient occlusion texture map and/or light map coordinate\n\
#ifdef VS_SECONDTEXTURECOORDINATE\n\
	TexCoordVS.zw = VertexTexCoord1;\n\
#endif\n\
\n\
	// Transform vertex normal to view space\n\
	NormalDepthVS.xyz = mat3(WorldV)*VertexNormal;\n\
#ifdef VS_TWOSIDED\n\
	// Allow vertex normal flip if required for proper two sided lighting\n\
	NormalDepthVS.xyz = NormalDepthVS.xyz*NormalScale;\n\
#endif\n\
\n\
	// Transform vertex tangent and binormal to view space\n\
#ifdef VS_TANGENT_BINORMAL\n\
	TangentVS  = mat3(WorldV)*VertexTangent;\n\
	BinormalVS = mat3(WorldV)*VertexBinormal;\n\
\n\
	// Calculate the tangent space vector pointing from the pixel to the eye point for parallax mapping	\n\
	EyeVecVS = mat3(VertexTangent, VertexBinormal, VertexNormal)*(EyePos - VertexPosition.xyz);\n\
#endif\n\
\n\
	// Transform vertex position to view space\n\
	vec4 viewSpacePosition = WorldV*VertexPosition;\n\
#ifdef VS_VIEWSPACEPOSITION\n\
	PositionVS = viewSpacePosition.xyz;\n\
#endif\n\
\n\
	// Use the z coordinate of the view space vertex position as linear depth\n\
	// Because we're using a right-handed coordinate system, we need to negate the depth!\n\
	NormalDepthVS.w = -viewSpacePosition.z;\n\
}";


// OpenGL 2.1 ("#version 120") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredGBuffer_GLSL_FS = "\
// Attributes\n\
#if defined(FS_AMBIENTOCCLUSIONMAP) || defined(FS_LIGHTMAP)\n\
	varying vec4 TexCoordVS;	// Vertex texture coordinate, zw for ambient occlusion map and/or light map texture coordinate from vertex shader\n\
#else\n\
	varying vec2 TexCoordVS;	// Vertex texture coordinate from vertex shader\n\
#endif\n\
varying vec4 NormalDepthVS;		// View space vertex normal (normalize it to avoid interpolation artifacts!) and view space linear depth [0...far plane] from vertex shader\n\
#ifdef FS_NORMALMAP\n\
	varying vec3 TangentVS;		// View space vertex tangent from vertex shader (normalize it to avoid interpolation artifacts!)\n\
	varying vec3 BinormalVS;	// View space vertex tangent from vertex shader (normalize it to avoid interpolation artifacts!)\n\
#endif\n\
varying vec3 EyeVecVS;			// Tangent space vector pointing from the pixel to the eye point from vertex shader\n\
#ifdef FS_REFLECTION\n\
	varying vec3 PositionVS;	// View space vertex position from vertex shader\n\
#endif\n\
\n\
// Uniforms\n\
uniform vec3 DiffuseColor;\n\
#ifdef FS_DIFFUSEMAP\n\
	uniform sampler2D DiffuseMap;\n\
	#ifdef FS_ALPHATEST\n\
		uniform float AlphaReference;\n\
	#endif\n\
#endif\n\
#ifdef FS_SPECULAR\n\
	uniform vec3  SpecularColor;\n\
	uniform float SpecularExponent;\n\
	#ifdef FS_SPECULARMAP\n\
		uniform sampler2D SpecularMap;\n\
	#endif\n\
#endif\n\
#ifdef FS_NORMALMAP\n\
	uniform sampler2D NormalMap;\n\
	uniform float     NormalMapBumpiness;\n\
	#ifdef FS_DETAILNORMALMAP\n\
		uniform sampler2D DetailNormalMap;\n\
		uniform float     DetailNormalMapBumpiness;\n\
		uniform vec2      DetailNormalMapUVScale;\n\
	#endif\n\
#endif\n\
#ifdef FS_PARALLAXMAPPING\n\
	uniform sampler2D HeightMap;\n\
	uniform vec2      ParallaxScaleBias;\n\
#endif\n\
#ifdef FS_AMBIENTOCCLUSIONMAP\n\
	uniform sampler2D AmbientOcclusionMap;\n\
	uniform float     AmbientOcclusionFactor;\n\
#endif\n\
#ifdef FS_LIGHTMAP\n\
	uniform sampler2D LightMap;\n\
	uniform vec3      LightMapColor;\n\
#endif\n\
#ifdef FS_EMISSIVEMAP\n\
	uniform sampler2D EmissiveMap;\n\
	uniform vec3      EmissiveMapColor;\n\
#endif\n\
#ifdef FS_GLOW\n\
	uniform float GlowFactor;\n\
	#ifdef FS_GLOWMAP\n\
		uniform sampler2D GlowMap;\n\
	#endif\n\
#endif\n\
#ifdef FS_REFLECTION\n\
	uniform vec3  ReflectionColor;\n\
	uniform float Reflectivity;\n\
	#ifdef FS_REFLECTIVITYMAP\n\
		uniform sampler2D ReflectivityMap;\n\
	#endif\n\
	#ifdef FS_FRESNELREFLECTION\n\
		uniform vec2 FresnelConstants; // x = R0 [0..1] and y = Power, always >0\n\
	#endif\n\
	#ifdef FS_2DREFLECTIONMAP\n\
		uniform sampler2D ReflectionMap;\n\
		uniform mat3      ViewSpaceToWorldSpace;	// View space to world space transform matrix\n\
	#elif defined(FS_CUBEREFLECTIONMAP)\n\
		uniform samplerCube ReflectionMap;\n\
		uniform mat3        ViewSpaceToWorldSpace;	// View space to world space transform matrix\n\
	#endif\n\
#endif\n\
\n\
// Programs\n\
// Encodes a 3 component normal vector to a 2 component normal vector\n\
vec2 encodeNormalVector(vec3 normal)\n\
{\n\
	float p = sqrt(normal.z*8.0 + 8.0);\n\
	return vec2(normal.xy/p + vec2(0.5, 0.5));\n\
}\n\
\n\
float fresnel(vec3 light, vec3 normal, vec2 constants)\n\
{\n\
	// Light and normal are assumed to be normalized\n\
	// constants.x = R0 [0..1]\n\
	// constants.y = Power, always >0\n\
	float cosAngle = clamp(1.0 - dot(light, normal), 0.0, 1.0); // We REALLY need to clamp in here or pow may hurt us when using negative numbers!\n\
	return constants.x + (1.0 - constants.x) * pow(cosAngle, constants.y);\n\
}\n\
\n\
// Program entry point\n\
void main()\n\
{\n\
	// Get the texel coordinate (textureCoordinate) to use\n\
#ifdef FS_PARALLAXMAPPING\n\
	// Plain parallax mapping works by offsetting the texture coordinate by approximating\n\
	// where the ray would hit the surface. The height map indicates how far to offset.\n\
	// The approximation assumes that all heights in the height-map is equal, which of\n\
	// course won't be the case, but if the bump map is fairly smooth it works well enough.\n\
	vec3  eyeVec = normalize(EyeVecVS);\n\
	float scale  = ParallaxScaleBias.x;\n\
	float bias   = ParallaxScaleBias.y;\n\
	float height = texture2D(HeightMap, TexCoordVS.xy).r;\n\
\n\
	// First offset pass with offset limiting (no division through eyeVec.z)\n\
	float offset = height*scale + bias;\n\
	vec2 textureCoordinate = TexCoordVS.xy + offset*eyeVec.xy;\n\
\n\
	// For better quality: Refine the parallax by making another lookup at where we ended\n\
	// up in the first parallax computation, then averaging the results.\n\
	float height2 = (height + texture2D(HeightMap, textureCoordinate).r)*0.5;\n\
	offset = height2*scale + bias;\n\
	textureCoordinate = TexCoordVS.xy + offset*eyeVec.xy;\n\
#else\n\
	#define textureCoordinate TexCoordVS.xy\n\
#endif\n\
\n\
	// RT0: Albedo RGB + alpha test value and/or ambient occlusion\n\
#ifdef FS_DIFFUSEMAP\n\
	gl_FragData[0] = texture2D(DiffuseMap, textureCoordinate);\n\
	#ifdef FS_ALPHATEST\n\
		// Perform an alpha test to discard fragments\n\
		if (gl_FragData[0].a < AlphaReference)\n\
			discard; // Throw the fragment away and don't draw it!\n\
	#endif\n\
	// Perform sRGB to linear space conversion (gamma correction)\n\
	#ifdef FS_GAMMACORRECTION\n\
		gl_FragData[0].rgb = pow(gl_FragData[0].rgb, vec3(2.2, 2.2, 2.2));\n\
	#endif\n\
	// Apply diffuse color\n\
	gl_FragData[0].rgb *= DiffuseColor;\n\
\n\
	// Use ambient occlusion map\n\
	#ifdef FS_AMBIENTOCCLUSIONMAP\n\
		gl_FragData[0].a = texture2D(AmbientOcclusionMap, TexCoordVS.zw).r*AmbientOcclusionFactor;\n\
	#endif\n\
#else\n\
	gl_FragData[0].rgb = DiffuseColor;\n\
\n\
	// Use ambient occlusion map\n\
	#ifdef FS_AMBIENTOCCLUSIONMAP\n\
		gl_FragData[0].a = texture2D(AmbientOcclusionMap, TexCoordVS.zw).r*AmbientOcclusionFactor;\n\
	#else\n\
		gl_FragData[0].a = 1.0;\n\
	#endif\n\
#endif\n\
\n\
	// RT1: RG encoded normal vector\n\
#ifdef FS_NORMALMAP\n\
	// Fetch normal texel data\n\
	#if defined(FS_NORMALMAP_DXT5_XGXR) || defined(FS_NORMALMAP_LATC2)\n\
		// Fetch the xy-components of the normal and reconstruct the z-component\n\
		vec3 normal;\n\
		#ifdef FS_NORMALMAP_DXT5_XGXR\n\
			normal.xy = texture2D(NormalMap, textureCoordinate).ag*2.0 - 1.0;\n\
		#else\n\
			normal.xy = texture2D(NormalMap, textureCoordinate).ra*2.0 - 1.0;\n\
		#endif\n\
		normal.z = sqrt(clamp(1.0 - normal.x*normal.x - normal.y*normal.y, 0.0, 1.0));\n\
	#else\n\
		vec3 normal = texture2D(NormalMap, textureCoordinate).xyz*2.0 - 1.0;\n\
	#endif\n\
	normal.xy *= NormalMapBumpiness;\n\
\n\
	// Fetch and apply detail normal texel data\n\
	#ifdef FS_DETAILNORMALMAP\n\
		#if defined(FS_DETAILNORMALMAP_DXT5_XGXR) || defined(FS_DETAILNORMALMAP_LATC2)\n\
			// Fetch the xy-components of the normal and reconstruct the z-component\n\
			vec3 detailNormal;\n\
			#ifdef FS_DETAILNORMALMAP_DXT5_XGXR\n\
				detailNormal.xy = texture2D(DetailNormalMap, textureCoordinate*DetailNormalMapUVScale).ag*2.0 - 1.0;\n\
			#else\n\
				detailNormal.xy = texture2D(DetailNormalMap, textureCoordinate*DetailNormalMapUVScale).ra*2.0 - 1.0;\n\
			#endif\n\
			detailNormal.z = sqrt(clamp(1.0 - detailNormal.x*detailNormal.x - detailNormal.y*detailNormal.y, 0.0, 1.0));\n\
		#else\n\
			vec3 detailNormal = texture2D(DetailNormalMap, textureCoordinate*DetailNormalMapUVScale).xyz*2.0 - 1.0;\n\
		#endif\n\
\n\
		// Just add the detail normal to the standard normal\n\
		normal = normal + detailNormal*DetailNormalMapBumpiness;\n\
	#endif\n\
\n\
	// Transform normal to view space\n\
	normal = normalize(normal.x*TangentVS + normal.y*BinormalVS + normal.z*NormalDepthVS.rgb);\n\
#else\n\
	vec3 normal = normalize(NormalDepthVS.rgb);\n\
#endif\n\
	// [TODO] There seem to be invalid normal vectors here (NAN) - IEEE standard: NaN != NaN - I don't use isnan so I can use lower shader versions\n\
	if (normal.x != normal.x || normal.y != normal.y || normal.z != normal.z)\n\
		normal = vec3(0.0, 0.0, 1.0); // I had situations with invalid normal vectors...\n\
	// RG encoded normal vector\n\
	gl_FragData[1].rg = encodeNormalVector(normal);\n\
\n\
	// RT1: Write the linear view space depth into the blue channel of RT1\n\
	gl_FragData[1].b = NormalDepthVS.w;\n\
#ifdef FS_PARALLAXMAPPING\n\
	// Manipulate the depth of the 'Virtual Position' so shadow mapping and projected textures are influenced by parallax mapping as well\n\
	// [TODO] This already looks quite nice for projected textures, but currently doesn't work correctly with shadow mapping because during\n\
	// the creation of the shadow map, this 'Virtual Position' must be taken into account, too. Check it later again when refactoring the shadow\n\
	// mapping system!\n\
//	gl_FragData[1].b -= normal.z*(height*ParallaxScaleBias.x + ParallaxScaleBias.y);\n\
#endif\n\
	gl_FragData[1].a = 1.0; // Currently unused\n\
\n\
	// RT2: Specular color RGB + specular exponent\n\
#ifdef FS_SPECULAR\n\
	#ifdef FS_SPECULARMAP\n\
		gl_FragData[2]      = texture2D(SpecularMap, textureCoordinate);\n\
		gl_FragData[2].rgb *= SpecularColor;\n\
		gl_FragData[2].a   *= SpecularExponent;\n\
	#else\n\
		gl_FragData[2].rgb = SpecularColor;\n\
		gl_FragData[2].a   = SpecularExponent;\n\
	#endif\n\
#else\n\
	gl_FragData[2] = vec4(0.0, 0.0, 0.0, 0.0);\n\
#endif\n\
\n\
	// RT3: Light accumulation RGB\n\
	gl_FragData[3] = vec4(0.0, 0.0, 0.0, 0.0);\n\
	// Light map color RGB\n\
	#ifdef FS_LIGHTMAP\n\
		// Get light map texel data\n\
		vec3 lightMapTexel = texture2D(LightMap, TexCoordVS.zw).rgb;\n\
		// Perform sRGB to linear space conversion (gamma correction)\n\
		#ifdef FS_GAMMACORRECTION\n\
			lightMapTexel = pow(lightMapTexel, vec3(2.2, 2.2, 2.2));\n\
		#endif\n\
		// Add color\n\
		gl_FragData[3].rgb += lightMapTexel*LightMapColor;\n\
	#endif\n\
	// Emissive map color RGB\n\
	#ifdef FS_EMISSIVEMAP\n\
		// Get emissive map texel data\n\
		vec3 emissiveMapTexel = texture2D(EmissiveMap, textureCoordinate).rgb;\n\
		// Perform sRGB to linear space conversion (gamma correction)\n\
		#ifdef FS_GAMMACORRECTION\n\
			emissiveMapTexel = pow(emissiveMapTexel, vec3(2.2, 2.2, 2.2));\n\
		#endif\n\
		// Add color\n\
		gl_FragData[3].rgb += emissiveMapTexel*EmissiveMapColor;\n\
	#endif\n\
	// Glow map color a\n\
	#ifdef FS_GLOW\n\
		#ifdef FS_GLOWMAP\n\
			gl_FragData[3].a = texture2D(GlowMap, textureCoordinate).r*GlowFactor;\n\
		#else\n\
			gl_FragData[3].a = GlowFactor;\n\
		#endif\n\
	#endif\n\
\n\
	// Reflection\n\
	#ifdef FS_REFLECTION\n\
		// Reflectivity = 0.0...1.0=no reflection...full reflection\n\
		float reflectivity = Reflectivity;\n\
		// Fresnel reflection\n\
		#ifdef FS_FRESNELREFLECTION\n\
			reflectivity *= fresnel(normalize(-PositionVS), normal, FresnelConstants);\n\
		#endif\n\
		// Reflectivity map\n\
		#ifdef FS_REFLECTIVITYMAP\n\
			reflectivity *= texture2D(ReflectivityMap, textureCoordinate).r;\n\
		#endif\n\
\n\
		// Reflection color\n\
		vec3 reflectionColor = vec3(1.0, 1.0, 1.0);\n\
		#ifdef FS_2DREFLECTIONMAP\n\
			// Spherical environment mapping\n\
			vec3  r = ViewSpaceToWorldSpace*normalize(reflect(PositionVS, normal));\n\
			float m = 2.0*sqrt(r.x*r.x + r.y*r.y + (r.z + 1.0)*(r.z + 1.0));\n\
			#define FLT_MIN 1.175494351e-38 // Minimum positive value\n\
			if (m < FLT_MIN)\n\
				m = FLT_MIN;\n\
			#undef FLT_MIN\n\
			reflectionColor = texture2D(ReflectionMap, vec2(r.x/m + 0.5, 1.0 - (r.y/m + 0.5))).rgb;\n\
		#elif defined(FS_CUBEREFLECTIONMAP)\n\
			// Cubic environment mapping\n\
			// There's no need to normalize the reflection vector when using cube maps\n\
			reflectionColor = textureCube(ReflectionMap, ViewSpaceToWorldSpace*reflect(PositionVS, normal)).rgb;\n\
		#endif\n\
		// Perform sRGB to linear space conversion (gamma correction)\n\
		#ifdef FS_GAMMACORRECTION\n\
			reflectionColor = pow(reflectionColor, vec3(2.2, 2.2, 2.2));\n\
		#endif\n\
		// Apply reflection color\n\
		reflectionColor *= ReflectionColor;\n\
\n\
		// Calculate the final albedo color after reflection was applied\n\
		gl_FragData[0].rgb = mix(gl_FragData[0].rgb, reflectionColor, reflectivity);\n\
	#endif\n\
}";
