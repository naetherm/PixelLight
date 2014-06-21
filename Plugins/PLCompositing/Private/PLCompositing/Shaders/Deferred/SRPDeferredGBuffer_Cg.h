/*********************************************************\
 *  File: SRPDeferredGBuffer_Cg.h                        *
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


// Cg vertex shader source code
static const PLCore::String sDeferredGBuffer_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position    : POSITION;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
#ifdef VS_SECONDTEXTURECOORDINATE\n\
	float4 TexCoord	   : TEXCOORD0;	// Vertex texture coordinate, zw for ambient occlusion map and/or light map texture coordinate\n\
#else\n\
	float2 TexCoord	   : TEXCOORD0;	// Vertex texture coordinate\n\
#endif\n\
	float4 NormalDepth : TEXCOORD1; // View space vertex normal and view space linear depth [0...far plane]\n\
#ifdef VS_TANGENT_BINORMAL\n\
	float3 Tangent     : TEXCOORD2;	// View space vertex tangent\n\
	float3 Binormal    : TEXCOORD3;	// View space vertex tangent\n\
#endif\n\
	float3 EyeVec      : TEXCOORD4;	// Tangent space vector pointing from the pixel to the eye point\n\
#ifdef VS_VIEWSPACEPOSITION\n\
	float3 PositionVS  : TEXCOORD5;	// View space vertex position\n\
#endif\n\
};\n\
\n\
// Main function\n\
VS_OUTPUT main(float4 VertexPosition  : POSITION	// Object space vertex position\n\
			 , float2 VertexTexCoord0 : TEXCOORD0	// Vertex texture coordinate\n\
			#ifdef VS_SECONDTEXTURECOORDINATE\n\
			 , float2 VertexTexCoord1 : TEXCOORD1	// Vertex ambient occlusion map and/or light map texture coordinate\n\
			#endif\n\
			 , float3 VertexNormal    : NORMAL		// Object space vertex normal\n\
			#ifdef VS_TANGENT_BINORMAL\n\
			 , float3 VertexTangent   : TANGENT		// Object space vertex tangent\n\
			 , float3 VertexBinormal  : BINORMAL	// Object space vertex tangent\n\
			#endif\n\
#ifdef VS_TWOSIDED\n\
	 , uniform float     NormalScale				// Normal scale (negative to flip normals)\n\
#endif\n\
	 , uniform float3    EyePos						// Object space eye position\n\
	 , uniform float4x4  WorldVP					// World view projection matrix\n\
	 , uniform float4x4  WorldV						// World view matrix\n\
#ifdef VS_DISPLACEMENTMAP\n\
	 , uniform sampler2D DisplacementMap			// Displacement map\n\
	 , uniform float2    DisplacementScaleBias		// Displacement scale and bias\n\
#endif\n\
	)\n\
{\n\
	VS_OUTPUT OUT;\n\
\n\
#ifdef VS_DISPLACEMENTMAP\n\
	// Sample displacement map\n\
	float displacement = tex2Dlod(DisplacementMap, float4(VertexTexCoord0, 0, 0)).r;\n\
	displacement = (displacement*DisplacementScaleBias.x) + DisplacementScaleBias.y;\n\
	float4 vertexPosition = float4(VertexPosition.xyz + displacement*VertexNormal, 1);\n\
#else\n\
	#define vertexPosition VertexPosition\n\
#endif\n\
\n\
	// Transform vertex position to clip space\n\
	OUT.Position = mul(WorldVP, vertexPosition);\n\
\n\
	// Pass through the vertex texture coordinate\n\
	OUT.TexCoord.xy = VertexTexCoord0;\n\
\n\
	// Pass through the vertex ambient occlusion texture map and/or light map coordinate\n\
#ifdef VS_SECONDTEXTURECOORDINATE\n\
	OUT.TexCoord.zw = VertexTexCoord1;\n\
#endif\n\
\n\
	// Transform vertex normal to view space\n\
	OUT.NormalDepth.xyz = mul((float3x3)WorldV, VertexNormal);\n\
#ifdef VS_TWOSIDED\n\
	// Allow vertex normal flip if required for proper two sided lighting\n\
	OUT.NormalDepth.xyz = OUT.NormalDepth.xyz*NormalScale;\n\
#endif\n\
\n\
	// Transform vertex tangent and binormal to view space\n\
#ifdef VS_TANGENT_BINORMAL\n\
	OUT.Tangent  = mul((float3x3)WorldV, VertexTangent);\n\
	OUT.Binormal = mul((float3x3)WorldV, VertexBinormal);\n\
\n\
	// Calculate the tangent space vector pointing from the pixel to the eye point for parallax mapping	\n\
	OUT.EyeVec = mul(float3x3(VertexTangent, VertexBinormal, VertexNormal), EyePos - VertexPosition.xyz);\n\
#endif\n\
\n\
	// Transform vertex position to view space\n\
	float4 viewSpacePosition = mul(WorldV, VertexPosition);\n\
#ifdef VS_VIEWSPACEPOSITION\n\
	OUT.PositionVS = viewSpacePosition.xyz;\n\
#endif\n\
\n\
	// Use the z coordinate of the view space vertex position as linear depth\n\
	// Because we're using a right-handed coordinate system, we need to negate the depth!\n\
	OUT.NormalDepth.w = -viewSpacePosition.z;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Cg fragment shader source code
static const PLCore::String sDeferredGBuffer_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position    : POSITION;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
#if defined(FS_AMBIENTOCCLUSIONMAP) || defined(FS_LIGHTMAP)\n\
	float4 TexCoord	   : TEXCOORD0;	// Vertex texture coordinate, zw for ambient occlusion map and/or light map texture coordinate\n\
#else\n\
	float2 TexCoord	   : TEXCOORD0;	// Vertex texture coordinate\n\
#endif\n\
	float4 NormalDepth : TEXCOORD1; // View space vertex normal (normalize it to avoid interpolation artifacts!) and view space linear depth [0...far plane]\n\
#ifdef FS_NORMALMAP\n\
	float3 Tangent     : TEXCOORD2;	// View space vertex tangent (normalize it to avoid interpolation artifacts!)\n\
	float3 Binormal    : TEXCOORD3;	// View space vertex tangent (normalize it to avoid interpolation artifacts!)\n\
#endif\n\
	float3 EyeVec      : TEXCOORD4;	// Tangent space vector pointing from the pixel to the eye point\n\
#ifdef FS_REFLECTION\n\
	float3 PositionVS  : TEXCOORD5;	// View space vertex position\n\
#endif\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
	float4 Color1 : COLOR1;\n\
	float4 Color2 : COLOR2;\n\
	float4 Color3 : COLOR3;\n\
};\n\
\n\
// Encodes a 3 component normal vector to a 2 component normal vector\n\
float2 encodeNormalVector(float3 normal)\n\
{\n\
	float p = sqrt(normal.z*8 + 8);\n\
	return float2(normal.xy/p + 0.5f);\n\
}\n\
\n\
float fresnel(float3 light, float3 normal, float2 constants)\n\
{\n\
	// Light and normal are assumed to be normalized\n\
	// constants.x = R0 [0..1]\n\
	// constants.y = Power, always >0\n\
	float cosAngle = clamp(1 - dot(light, normal), 0.0f, 1.0f); // We REALLY need to clamp in here or pow may hurt us when using negative numbers!\n\
	return constants.x + (1 - constants.x) * pow(cosAngle, constants.y);\n\
}\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT IN				// Interpolated output from the vertex stage\n\
	 , uniform float3    DiffuseColor\n\
#ifdef FS_DIFFUSEMAP\n\
	 , uniform sampler2D DiffuseMap\n\
	  #ifdef FS_ALPHATEST\n\
		 , uniform float AlphaReference\n\
	  #endif\n\
#endif\n\
#ifdef FS_SPECULAR\n\
	 , uniform float3    SpecularColor\n\
	 , uniform float     SpecularExponent\n\
	#ifdef FS_SPECULARMAP\n\
	 , uniform sampler2D SpecularMap\n\
	#endif\n\
#endif\n\
#ifdef FS_NORMALMAP\n\
	 , uniform sampler2D NormalMap\n\
	 , uniform float     NormalMapBumpiness\n\
	#ifdef FS_DETAILNORMALMAP\n\
	 , uniform sampler2D DetailNormalMap\n\
	 , uniform float     DetailNormalMapBumpiness\n\
	 , uniform float2    DetailNormalMapUVScale\n\
	#endif\n\
#endif\n\
#ifdef FS_PARALLAXMAPPING\n\
	 , uniform sampler2D HeightMap\n\
	 , uniform float2    ParallaxScaleBias\n\
#endif\n\
#ifdef FS_AMBIENTOCCLUSIONMAP\n\
	 , uniform sampler2D AmbientOcclusionMap\n\
	 , uniform float     AmbientOcclusionFactor\n\
#endif\n\
#ifdef FS_LIGHTMAP\n\
	 , uniform sampler2D LightMap\n\
	 , uniform float3    LightMapColor\n\
#endif\n\
#ifdef FS_EMISSIVEMAP\n\
	 , uniform sampler2D EmissiveMap\n\
	 , uniform float3    EmissiveMapColor\n\
#endif\n\
#ifdef FS_GLOW\n\
	 , uniform float GlowFactor\n\
	#ifdef FS_GLOWMAP\n\
		, uniform sampler2D GlowMap\n\
	#endif\n\
#endif\n\
#ifdef FS_REFLECTION\n\
	 , uniform float3 ReflectionColor\n\
	 , uniform float  Reflectivity\n\
	#ifdef FS_REFLECTIVITYMAP\n\
		, uniform sampler2D ReflectivityMap\n\
	#endif\n\
	#ifdef FS_FRESNELREFLECTION\n\
		, uniform float2 FresnelConstants // x = R0 [0..1] and y = Power, always >0\n\
	#endif\n\
	#ifdef FS_2DREFLECTIONMAP\n\
		, uniform sampler2D ReflectionMap\n\
		, uniform float3x3  ViewSpaceToWorldSpace	// View space to world space transform matrix\n\
	#elif defined(FS_CUBEREFLECTIONMAP)\n\
		, uniform samplerCUBE ReflectionMap\n\
		, uniform float3x3    ViewSpaceToWorldSpace	// View space to world space transform matrix\n\
	#endif\n\
#endif\n\
	 )\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Get the texel coordinate (textureCoordinate) to use\n\
#ifdef FS_PARALLAXMAPPING\n\
	// Plain parallax mapping works by offsetting the texture coordinate by approximating\n\
	// where the ray would hit the surface. The height map indicates how far to offset.\n\
	// The approximation assumes that all heights in the height-map is equal, which of\n\
	// course won't be the case, but if the bump map is fairly smooth it works well enough.\n\
	float3 eyeVec = normalize(IN.EyeVec);\n\
	float  scale  = ParallaxScaleBias.x;\n\
	float  bias   = ParallaxScaleBias.y;\n\
	float  height = tex2D(HeightMap, IN.TexCoord.xy).r;\n\
\n\
	// First offset pass with offset limiting (no division through eyeVec.z)\n\
	float offset = height*scale + bias;\n\
	float2 textureCoordinate = IN.TexCoord.xy + offset*eyeVec.xy;\n\
\n\
	// For better quality: Refine the parallax by making another lookup at where we ended\n\
	// up in the first parallax computation, then averaging the results.\n\
	float height2 = (height + tex2D(HeightMap, textureCoordinate).r)*0.5f;\n\
	offset = height2*scale + bias;\n\
	textureCoordinate = IN.TexCoord.xy + offset*eyeVec.xy;\n\
#else\n\
	#define textureCoordinate IN.TexCoord.xy\n\
#endif\n\
\n\
	// RT0: Albedo RGB + alpha test value and/or ambient occlusion\n\
#ifdef FS_DIFFUSEMAP\n\
	OUT.Color0 = tex2D(DiffuseMap, textureCoordinate);\n\
	#ifdef FS_ALPHATEST\n\
		// Perform an alpha test to discard fragments\n\
		if (OUT.Color0.a < AlphaReference)\n\
			discard; // Throw the fragment away and don't draw it!\n\
	#endif\n\
	// Perform sRGB to linear space conversion (gamma correction)\n\
	#ifdef FS_GAMMACORRECTION\n\
		OUT.Color0.rgb = pow(OUT.Color0.rgb, 2.2f);\n\
	#endif\n\
	// Apply diffuse color\n\
	OUT.Color0.rgb *= DiffuseColor;\n\
\n\
	// Use ambient occlusion map\n\
	#ifdef FS_AMBIENTOCCLUSIONMAP\n\
		OUT.Color0.a = tex2D(AmbientOcclusionMap, IN.TexCoord.zw).r*AmbientOcclusionFactor;\n\
	#endif\n\
#else\n\
	OUT.Color0.rgb = DiffuseColor;\n\
\n\
	// Use ambient occlusion map\n\
	#ifdef FS_AMBIENTOCCLUSIONMAP\n\
		OUT.Color0.a = tex2D(AmbientOcclusionMap, IN.TexCoord.zw).r*AmbientOcclusionFactor;\n\
	#else\n\
		OUT.Color0.a = 1;\n\
	#endif\n\
#endif\n\
\n\
	// RT1: RG encoded normal vector\n\
#ifdef FS_NORMALMAP\n\
	// Fetch normal texel data\n\
	#if defined(FS_NORMALMAP_DXT5_XGXR) || defined(FS_NORMALMAP_LATC2)\n\
		// Fetch the xy-components of the normal and reconstruct the z-component\n\
		float3 normal;\n\
		#ifdef FS_NORMALMAP_DXT5_XGXR\n\
			normal.xy = tex2D(NormalMap, textureCoordinate).ag*2 - 1;\n\
		#else\n\
			normal.xy = tex2D(NormalMap, textureCoordinate).ra*2 - 1;\n\
		#endif\n\
		normal.z = sqrt(clamp(1 - normal.x*normal.x - normal.y*normal.y, 0.0f, 1.0f));\n\
	#else\n\
		float3 normal = tex2D(NormalMap, textureCoordinate).xyz*2 - 1;\n\
	#endif\n\
	normal.xy *= NormalMapBumpiness;\n\
\n\
	// Fetch and apply detail normal texel data\n\
	#ifdef FS_DETAILNORMALMAP\n\
		#if defined(FS_DETAILNORMALMAP_DXT5_XGXR) || defined(FS_DETAILNORMALMAP_LATC2)\n\
			// Fetch the xy-components of the normal and reconstruct the z-component\n\
			float3 detailNormal;\n\
			#ifdef FS_DETAILNORMALMAP_DXT5_XGXR\n\
				detailNormal.xy = tex2D(DetailNormalMap, textureCoordinate*DetailNormalMapUVScale).ag*2 - 1;\n\
			#else\n\
				detailNormal.xy = tex2D(DetailNormalMap, textureCoordinate*DetailNormalMapUVScale).ra*2 - 1;\n\
			#endif\n\
			detailNormal.z = sqrt(clamp(1 - detailNormal.x*detailNormal.x - detailNormal.y*detailNormal.y, 0.0f, 1.0f));\n\
		#else\n\
			float3 detailNormal = tex2D(DetailNormalMap, textureCoordinate*DetailNormalMapUVScale).xyz*2 - 1;\n\
		#endif\n\
\n\
		// Just add the detail normal to the standard normal\n\
		normal = normal + detailNormal*DetailNormalMapBumpiness;\n\
	#endif\n\
\n\
	// Transform normal to view space\n\
	normal = normalize(normal.x*IN.Tangent + normal.y*IN.Binormal + normal.z*IN.NormalDepth.rgb);\n\
#else\n\
	float3 normal = normalize(IN.NormalDepth.rgb);\n\
#endif\n\
	// [TODO] There seem to be invalid normal vectors here (NAN)\n\
	if (isnan(normal.x) || isnan(normal.y) || isnan(normal.z))\n\
		normal = float3(0, 0, 1); // I had situations with invalid normal vectors...\n\
	// RG encoded normal vector\n\
	OUT.Color1.rg = encodeNormalVector(normal);\n\
\n\
	// RT1: Write the linear view space depth into the blue channel of RT1\n\
	OUT.Color1.b = IN.NormalDepth.w;\n\
#ifdef FS_PARALLAXMAPPING\n\
	// Manipulate the depth of the 'Virtual Position' so shadow mapping and projected textures are influenced by parallax mapping as well\n\
	// [TODO] This already looks quite nice for projected textures, but currently doesn't work correctly with shadow mapping because during\n\
	// the creation of the shadow map, this 'Virtual Position' must be taken into account, too. Check it later again when refactoring the shadow\n\
	// mapping system!\n\
//	OUT.Color1.b -= normal.z*(height*ParallaxScaleBias.x + ParallaxScaleBias.y);\n\
#endif\n\
	OUT.Color1.a = 1; // Currently unused\n\
\n\
	// RT2: Specular color RGB + specular exponent\n\
#ifdef FS_SPECULAR\n\
	#ifdef FS_SPECULARMAP\n\
		OUT.Color2      = tex2D(SpecularMap, textureCoordinate);\n\
		OUT.Color2.rgb *= SpecularColor;\n\
		OUT.Color2.a   *= SpecularExponent;\n\
	#else\n\
		OUT.Color2.rgb = SpecularColor;\n\
		OUT.Color2.a   = SpecularExponent;\n\
	#endif\n\
#else\n\
	OUT.Color2 = 0;\n\
#endif\n\
\n\
	// RT3: Light accumulation RGB\n\
	OUT.Color3 = 0;\n\
	// Light map color RGB\n\
	#ifdef FS_LIGHTMAP\n\
		// Get light map texel data\n\
		float3 lightMapTexel = tex2D(LightMap, IN.TexCoord.zw).rgb;\n\
		// Perform sRGB to linear space conversion (gamma correction)\n\
		#ifdef FS_GAMMACORRECTION\n\
			lightMapTexel = pow(lightMapTexel, 2.2f);\n\
		#endif\n\
		// Add color\n\
		OUT.Color3.rgb += lightMapTexel*LightMapColor;\n\
	#endif\n\
	// Emissive map color RGB\n\
	#ifdef FS_EMISSIVEMAP\n\
		// Get emissive map texel data\n\
		float3 emissiveMapTexel = tex2D(EmissiveMap, textureCoordinate).rgb;\n\
		// Perform sRGB to linear space conversion (gamma correction)\n\
		#ifdef FS_GAMMACORRECTION\n\
			emissiveMapTexel = pow(emissiveMapTexel, 2.2f);\n\
		#endif\n\
		// Add color\n\
		OUT.Color3.rgb += emissiveMapTexel*EmissiveMapColor;\n\
	#endif\n\
	// Glow map color a\n\
	#ifdef FS_GLOW\n\
		#ifdef FS_GLOWMAP\n\
			OUT.Color3.a = tex2D(GlowMap, textureCoordinate).r*GlowFactor;\n\
		#else\n\
			OUT.Color3.a = GlowFactor;\n\
		#endif\n\
	#endif\n\
\n\
	// Reflection\n\
	#ifdef FS_REFLECTION\n\
		// Reflectivity = 0.0...1.0=no reflection...full reflection\n\
		float reflectivity = Reflectivity;\n\
		// Fresnel reflection\n\
		#ifdef FS_FRESNELREFLECTION\n\
			reflectivity *= fresnel(normalize(-IN.PositionVS), normal, FresnelConstants);\n\
		#endif\n\
		// Reflectivity map\n\
		#ifdef FS_REFLECTIVITYMAP\n\
			reflectivity *= tex2D(ReflectivityMap, textureCoordinate).r;\n\
		#endif\n\
\n\
		// Reflection color\n\
		float3 reflectionColor = 1;\n\
		#ifdef FS_2DREFLECTIONMAP\n\
			// Spherical environment mapping\n\
			float3 r = mul(ViewSpaceToWorldSpace, normalize(reflect(IN.PositionVS, normal)));\n\
			float  m = 2*sqrt(r.x*r.x + r.y*r.y + (r.z + 1)*(r.z + 1));\n\
			#define FLT_MIN 1.175494351e-38F // Minimum positive value\n\
			if (m < FLT_MIN)\n\
				m = FLT_MIN;\n\
			#undef FLT_MIN\n\
			reflectionColor = tex2D(ReflectionMap, float2(r.x/m + 0.5f, 1 - (r.y/m + 0.5f))).rgb;\n\
		#elif defined(FS_CUBEREFLECTIONMAP)\n\
			// Cubic environment mapping\n\
			// There's no need to normalize the reflection vector when using cube maps\n\
			reflectionColor = texCUBE(ReflectionMap, mul(ViewSpaceToWorldSpace, reflect(IN.PositionVS, normal))).rgb;\n\
		#endif\n\
		// Perform sRGB to linear space conversion (gamma correction)\n\
		#ifdef FS_GAMMACORRECTION\n\
			reflectionColor = pow(reflectionColor, 2.2f);\n\
		#endif\n\
		// Apply reflection color\n\
		reflectionColor *= ReflectionColor;\n\
\n\
		// Calculate the final albedo color after reflection was applied\n\
		OUT.Color0.rgb = lerp(OUT.Color0.rgb, reflectionColor, reflectivity);\n\
	#endif\n\
\n\
	// Done\n\
	return OUT;\n\
}";
