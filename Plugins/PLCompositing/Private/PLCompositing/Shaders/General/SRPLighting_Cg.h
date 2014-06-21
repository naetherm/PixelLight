/*********************************************************\
 *  File: SRPLighting_Cg.h                               *
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
static const PLCore::String sLighting_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;			// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
#ifdef VS_TEXCOORD0\n\
	float2 TexCoord : TEXCOORD0;		// Vertex texture coordinate 0\n\
#endif\n\
	float3 PositionVS : TEXCOORD1;		// View space vertex position\n\
#ifdef VS_NORMAL\n\
	float3 Normal : TEXCOORD2;			// View space vertex normal\n\
	#ifdef VS_TANGENT_BINORMAL\n\
		float3 Tangent  : TEXCOORD3;	// View space vertex tangent\n\
		float3 Binormal : TEXCOORD4;	// View space vertex binormal\n\
		#ifdef VS_PARALLAXMAPPING\n\
			float3 EyeVec : TEXCOORD5;	// Tangent space vector pointing from the pixel to the eye point\n\
		#endif\n\
	#endif\n\
#endif\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float4 VertexPosition : POSITION			// Object space vertex position input\n\
	#ifdef VS_TEXCOORD0\n\
		, float2 VertexTexCoord0 : TEXCOORD0			// Vertex texture coordinate 0 input\n\
	#endif\n\
	#ifdef VS_NORMAL\n\
		, float3 VertexNormal : NORMAL					// Object space vertex normal input\n\
		#ifdef VS_TWOSIDEDLIGHTING\n\
			, uniform float NormalScale					// Normal scale (negative to flip normals)\n\
		#endif\n\
		#ifdef VS_TANGENT_BINORMAL\n\
			, float3 VertexTangent  : TEXCOORD3			// Object space vertex tangent input\n\
			, float3 VertexBinormal : TEXCOORD4			// Object space vertex binormal input\n\
			#ifdef VS_PARALLAXMAPPING\n\
				, uniform float3 EyePos					// Object space eye position\n\
			#endif\n\
		#endif\n\
	#endif\n\
	, uniform float4x4 ObjectSpaceToViewSpaceMatrix		// Object space to view space matrix\n\
	, uniform float4x4 ObjectSpaceToClipSpaceMatrix)	// Object space to clip space matrix\n\
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
	// Transform vertex position to view space\n\
	float4 viewSpacePosition = mul(ObjectSpaceToViewSpaceMatrix, VertexPosition);\n\
	Out.PositionVS = viewSpacePosition.xyz;\n\
\n\
#ifdef VS_NORMAL\n\
	// Transform vertex normal to view space\n\
	Out.Normal = mul((float3x3)ObjectSpaceToViewSpaceMatrix, VertexNormal);\n\
\n\
	#ifdef VS_TWOSIDEDLIGHTING\n\
		// Allow vertex normal flip if required for proper two sided lighting\n\
		Out.Normal *= NormalScale;\n\
	#endif\n\
\n\
	#ifdef VS_TANGENT_BINORMAL\n\
		// Transform vertex tangent and binormal to view space\n\
		Out.Tangent  = mul((float3x3)ObjectSpaceToViewSpaceMatrix, VertexTangent);\n\
		Out.Binormal = mul((float3x3)ObjectSpaceToViewSpaceMatrix, VertexBinormal);\n\
\n\
		#ifdef VS_PARALLAXMAPPING\n\
			// Calculate the tangent space vector pointing from the pixel to the eye point for parallax mapping	\n\
			Out.EyeVec = mul(float3x3(VertexTangent, VertexBinormal, VertexNormal), EyePos - VertexPosition.xyz);\n\
		#endif\n\
	#endif\n\
#endif\n\
\n\
	// Done\n\
	return Out;\n\
}";


// Cg fragment shader source code - divided into two parts because I got the following VC compiler error: "error C2026: string too big, trailing characters truncated"
static const PLCore::String sLighting_Cg_FS_Part1 = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;			// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
#ifdef FS_TEXCOORD0\n\
	float2 TexCoord : TEXCOORD0;		// Vertex texture coordinate 0\n\
#endif\n\
	float3 PositionVS  : TEXCOORD1;		// View space vertex position\n\
#ifdef FS_NORMAL\n\
	float3 Normal : TEXCOORD2;			// View space vertex normal\n\
	#ifdef FS_TANGENT_BINORMAL\n\
		float3 Tangent  : TEXCOORD3;	// View space vertex tangent\n\
		float3 Binormal : TEXCOORD4;	// View space vertex binormal\n\
		#ifdef FS_PARALLAXMAPPING\n\
			float3 EyeVec : TEXCOORD5;	// Tangent space vector pointing from the pixel to the eye point\n\
		#endif\n\
	#endif\n\
#endif\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Ramp maps\n\
#if defined(FS_DIFFUSERAMPMAP) || defined(FS_SPECULARRAMPMAP) || defined(FS_EDGERAMPMAP)\n\
	#define RAMPMAPS\n\
	struct FS_RAMPMAPS {\n\
		#ifdef FS_DIFFUSERAMPMAP\n\
			 sampler1D DiffuseRampMap;\n\
		#endif\n\
		#ifdef FS_SPECULARRAMPMAP\n\
			sampler1D SpecularRampMap;\n\
		#endif\n\
		#ifdef FS_EDGERAMPMAP\n\
			sampler1D EdgeRampMap;\n\
		#endif\n\
	};\n\
#endif\n\
\n\
// Programs\n\
#ifdef FS_FRESNELREFLECTION\n\
	float fresnel(float3 light, float3 normal, float2 constants)\n\
	{\n\
		// Light and normal are assumed to be normalized\n\
		// constants.x = R0 [0..1]\n\
		// constants.y = Power, always >0\n\
		float cosAngle = clamp(1 - dot(light, normal), 0, 1); // We REALLY need to clamp in here or pow may hurt us when using negative numbers!\n\
		return constants.x + (1 - constants.x) * pow(cosAngle, constants.y);\n\
	}\n\
#endif\n\
\n\
// Performs the Blinn-Phong lighting calculation\n\
float3 BlinnPhong(float3 lightVector, float3 lightColor, float3 viewVector, float3 normalVector, float3 diffuseColor, float3 specularColor, float specularExponent\n\
	#ifdef RAMPMAPS\n\
		, FS_RAMPMAPS rampMaps\n\
	#endif\n\
)\n\
{\n\
	// [TODO] There seem to be invalid normal vectors here (NAN)\n\
	if (isnan(normalVector.x) || isnan(normalVector.y) || isnan(normalVector.z))\n\
		normalVector = float3(0, 0, 1);\n\
	if (isnan(lightVector.x) || isnan(lightVector.y) || isnan(lightVector.z))\n\
		lightVector = float3(0, 0, 1);\n\
	if (isnan(viewVector.x) || isnan(viewVector.y) || isnan(viewVector.z))\n\
		viewVector = float3(0, 0, 1);\n\
\n\
	// Diffuse term\n\
	float diffuse = clamp(dot(lightVector, normalVector), 0, 1);\n\
	#ifdef FS_DIFFUSERAMPMAP\n\
		diffuse *= tex1D(rampMaps.DiffuseRampMap, diffuse).x;\n\
	#endif\n\
	float3 diffuseLighting = diffuse*diffuseColor*lightColor;\n\
\n\
	// Specular term\n\
	#ifdef FS_SPECULAR\n\
		// Calculate the half vector between the light vector and the view vector. This is cheaper then calculating the actual reflective vector.\n\
		float3 halfVector = normalize(lightVector + viewVector);\n\
\n\
		// Ensure that the specular exponent is never ever <=0, else NANs may be produced by pow!\n\
		#define FLT_MIN 1.175494351e-38F // Minimum positive value\n\
		if (specularExponent < FLT_MIN)\n\
			specularExponent = FLT_MIN;\n\
		#undef FLT_MIN\n\
\n\
		// Specular term\n\
		float specular = pow(clamp(dot(halfVector, normalVector), 0, 1), specularExponent);\n\
		float3 specularLighting = specular*specularColor*lightColor;\n\
		#ifdef FS_SPECULARRAMPMAP\n\
			specularLighting *= tex1D(rampMaps.SpecularRampMap, specular).x;\n\
		#endif\n\
	#else\n\
		#define specularLighting 0\n\
	#endif\n\
\n\
	// Edge detection\n\
	#ifdef FS_EDGERAMPMAP\n\
		float edge = tex1D(rampMaps.EdgeRampMap, max(0, dot(viewVector, normalVector))).x;\n\
	#else\n\
		#define edge 1\n\
	#endif\n\
\n\
	// Final color\n\
	return (diffuseLighting + specularLighting)*edge;\n\
}\n\
\n\
#ifdef FS_SOFTSHADOWMAPPING\n\
	// Fake PCF (percentage-closer filtering)\n\
	float4 texPCF(sampler2D map, float4 location, float2 offset)\n\
	{\n\
		return tex2Dproj(map, float4(location.xy + location.w*offset, location.z, location.w));\n\
	}\n\
\n\
	float texPCF(samplerCUBE map, float3 location, float3 offset)\n\
	{\n\
		// Get shadow map data\n\
		float4 shadowMap = texCUBE(map, location + offset);\n\
\n\
		// Unpack\n\
		return shadowMap.r/1 + shadowMap.g/256 + shadowMap.b/65536 + shadowMap.a/16777216;\n\
	}\n\
#endif\n\
\n\
#ifdef FS_SHADOWMAPPING\n\
	float texPCF(samplerCUBE map, float3 location)\n\
	{\n\
		// Get shadow map data\n\
		float4 shadowMap = texCUBE(map, location);\n\
\n\
		// Unpack\n\
		return shadowMap.r/1 + shadowMap.g/256 + shadowMap.b/65536 + shadowMap.a/16777216;\n\
	}\n\
#endif\n\
\n\
FS_OUTPUT main(VS_OUTPUT In			// Vertex shader output as fragment shader input\n\
	, uniform float3 DiffuseColor\n\
	#ifdef FS_DIFFUSEMAP\n\
		, uniform sampler2D DiffuseMap\n\
		#ifdef FS_ALPHATEST\n\
			, uniform float AlphaReference\n\
		#endif\n\
	#endif\n\
	#ifdef FS_DIFFUSERAMPMAP\n\
		, uniform sampler1D DiffuseRampMap\n\
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
	#ifdef FS_DIRECTIONAL\n\
		, uniform float3 LightDirection								// View space normalized light direction\n\
	#else\n\
		, uniform float3 LightPosition								// View space light position\n\
		, uniform float	 LightRadius								// View space light radius\n\
		#ifdef FS_PROJECTIVE_POINT\n\
			, uniform samplerCUBE ProjectivePointCubeMap			// Cube map texture for projective point light\n\
			, uniform float3x3    ViewSpaceToCubeMapSpace			// View space to cube map space transform matrix\n\
		#else\n\
			#ifdef FS_SPOT\n\
				, uniform float3	   LightDirection				// View space normalized light direction\n\
				#ifdef FS_SPOT_PROJECTIVE\n\
					, uniform sampler2D ProjectiveSpotMap			// Texture for projective spot light\n\
					, uniform float4x4  ViewSpaceToSpotMapSpace		// View space to spot map space transform matrix\n\
				#endif\n\
				#ifdef FS_SPOT_CONE\n\
					#ifdef FS_SPOT_SMOOTHCONE\n\
						, uniform float2 SpotConeCos				// Cosinus of the outer and inner cone angle in view space\n\
					#else\n\
						, uniform float  SpotConeCos				// Cosinus of the cone angle in view space\n\
					#endif\n\
				#endif\n\
			#endif\n\
		#endif\n\
		#ifdef FS_SHADOWMAPPING\n\
			#ifdef FS_SPOT\n\
				, uniform sampler2D ShadowMap						// 2D shadow map\n\
				, uniform float4x4  ViewSpaceToShadowMapSpace		// View space to shadow map space transform matrix\n\
			#else\n\
				, uniform samplerCUBE ShadowMap						// Cube shadow map\n\
				, uniform float3x3    ViewSpaceToShadowCubeMapSpace	// View space to shadow cube map space transform matrix\n\
				, uniform float		  InvLightRadius				// 1/LightRadius\n\
			#endif\n\
			#ifdef FS_SOFTSHADOWMAPPING\n\
				, uniform float TexelSize							// Shadow map texel size\n\
			#endif\n\
		#endif\n\
	#endif\n\
	, uniform float3 LightColor	// Light color\n\
	#ifdef FS_SPECULAR\n\
		, uniform float3 SpecularColor\n\
		, uniform float  SpecularExponent\n\
		#ifdef FS_SPECULARMAP\n\
			, uniform sampler2D SpecularMap\n\
		#endif\n\
		#ifdef FS_SPECULARRAMPMAP\n\
			, uniform sampler1D SpecularRampMap\n\
		#endif\n\
	#endif\n\
	#ifdef FS_EDGERAMPMAP\n\
		, uniform sampler1D EdgeRampMap\n\
	#endif\n\
)";
static const PLCore::String sLighting_Cg_FS_Part2 = "\
{\n\
	FS_OUTPUT Out;\n\
\n\
	// Define were the texture coordinates come from\n\
	#ifdef FS_TEXCOORD0\n\
		#define TexCoord0 In.TexCoord\n\
	#endif\n\
\n\
	#ifdef FS_PARALLAXMAPPING\n\
		// Plain parallax mapping works by offsetting the texture coordinate by approximating\n\
		// where the ray would hit the surface. The height map indicates how far to offset.\n\
		// The approximation assumes that all heights in the height-map is equal, which of\n\
		// course won't be the case, but if the bump map is fairly smooth it works well enough.\n\
		float3 eyeVec = normalize(In.EyeVec);\n\
		float  scale  = ParallaxScaleBias.x;\n\
		float  bias   = ParallaxScaleBias.y;\n\
		float  height = tex2D(HeightMap, TexCoord0).r;\n\
\n\
		// First offset pass with offset limiting (no division through eyeVec.z)\n\
		float offset = height*scale + bias;\n\
		float2 textureCoordinate = TexCoord0 + offset*eyeVec.xy;\n\
\n\
		// For better quality: Refine the parallax by making another lookup at where we ended\n\
		// up in the first parallax computation, then averaging the results.\n\
		float height2 = (height + tex2D(HeightMap, textureCoordinate).r)*0.5f;\n\
		offset = height2*scale + bias;\n\
		TexCoord0 = TexCoord0 + offset*eyeVec.xy;\n\
	#endif\n\
\n\
	// Get the diffuse color\n\
	#ifdef FS_DIFFUSEMAP\n\
		float4 diffuseMapTexel = tex2D(DiffuseMap, TexCoord0);\n\
		#ifdef FS_ALPHATEST\n\
			// Perform an alpha test to discard fragments\n\
			if (diffuseMapTexel.a < AlphaReference)\n\
				discard; // Throw the fragment away and don't draw it!\n\
		#endif\n\
		#ifdef FS_GAMMACORRECTION\n\
			diffuseMapTexel.rgb = pow(diffuseMapTexel.rgb, 2.2f);	// Perform sRGB to linear space conversion (gamma correction)\n\
		#endif\n\
		float3 diffuseColor = diffuseMapTexel.rgb*DiffuseColor;\n\
	#else\n\
		float3 diffuseColor = DiffuseColor;\n\
	#endif\n\
\n\
	// Get the view space normal vector\n\
	#ifdef FS_NORMAL\n\
		#ifdef FS_NORMALMAP\n\
			// Fetch normal texel data\n\
			#if defined(FS_NORMALMAP_DXT5_XGXR) || defined(FS_NORMALMAP_LATC2)\n\
				// Fetch the xy-components of the normal and reconstruct the z-component\n\
				float3 normal;\n\
				#ifdef FS_NORMALMAP_DXT5_XGXR\n\
					normal.xy = tex2D(NormalMap, TexCoord0).ag*2 - 1;\n\
				#else\n\
					normal.xy = tex2D(NormalMap, TexCoord0).ra*2 - 1;\n\
				#endif\n\
				normal.z = sqrt(clamp(1 - normal.x*normal.x - normal.y*normal.y, 0.0f, 1.0f));\n\
			#else\n\
				float3 normal = tex2D(NormalMap, TexCoord0).xyz*2 - 1;\n\
			#endif\n\
			normal.xy *= NormalMapBumpiness;\n\
\n\
			// Fetch and apply detail normal texel data\n\
			#ifdef FS_DETAILNORMALMAP\n\
				#if defined(FS_DETAILNORMALMAP_DXT5_XGXR) || defined(FS_DETAILNORMALMAP_LATC2)\n\
					// Fetch the xy-components of the normal and reconstruct the z-component\n\
					float3 detailNormal;\n\
					#ifdef FS_DETAILNORMALMAP_DXT5_XGXR\n\
						detailNormal.xy = tex2D(DetailNormalMap, TexCoord0*DetailNormalMapUVScale).ag*2 - 1;\n\
					#else\n\
						detailNormal.xy = tex2D(DetailNormalMap, TexCoord0*DetailNormalMapUVScale).ra*2 - 1;\n\
					#endif\n\
					detailNormal.z = sqrt(clamp(1 - detailNormal.x*detailNormal.x - detailNormal.y*detailNormal.y, 0.0f, 1.0f));\n\
				#else\n\
					float3 detailNormal = tex2D(DetailNormalMap, TexCoord0*DetailNormalMapUVScale).xyz*2 - 1;\n\
				#endif\n\
\n\
				// Just add the detail normal to the standard normal\n\
				normal = normal + detailNormal*DetailNormalMapBumpiness;\n\
			#endif\n\
\n\
			// Transform normal to view space\n\
			normal = normalize(normal.x*In.Tangent + normal.y*In.Binormal + normal.z*In.Normal);\n\
		#else\n\
			float3 normal = normalize(In.Normal);\n\
		#endif\n\
		// [TODO] There seem to be invalid normal vectors here (NAN)\n\
		if (isnan(normal.x) || isnan(normal.y) || isnan(normal.z))\n\
			normal = float3(0, 0, 1); // I had situations with invalid normal vectors...\n\
	#else\n\
		// Define a dummy normal so, when using reflections, we get at least some 'kind of reflection' effect\n\
		#define normal float3(0, 0, 1)\n\
	#endif\n\
\n\
	// Apply reflection - modifies the diffuse color\n\
	#ifdef FS_REFLECTION\n\
		// Reflectivity = 0.0...1.0=no reflection...full reflection\n\
		float reflectivity = Reflectivity;\n\
		// Fresnel reflection\n\
		#ifdef FS_FRESNELREFLECTION\n\
			reflectivity *= fresnel(normalize(-In.PositionVS), normal, FresnelConstants);\n\
		#endif\n\
		// Reflectivity map\n\
		#ifdef FS_REFLECTIVITYMAP\n\
			reflectivity *= tex2D(ReflectivityMap, TexCoord0).r;\n\
		#endif\n\
\n\
		// Reflection color\n\
		float3 reflectionColor = 1;\n\
		#ifdef FS_2DREFLECTIONMAP\n\
			// Spherical environment mapping\n\
			float3 r = mul(ViewSpaceToWorldSpace, normalize(reflect(In.PositionVS, normal)));\n\
			float  m = 2*sqrt(r.x*r.x + r.y*r.y + (r.z + 1)*(r.z + 1));\n\
			#define FLT_MIN 1.175494351e-38F // Minimum positive value\n\
			if (m < FLT_MIN)\n\
				m = FLT_MIN;\n\
			#undef FLT_MIN\n\
			reflectionColor = tex2D(ReflectionMap, float2(r.x/m + 0.5f, 1 - (r.y/m + 0.5f))).rgb;\n\
		#elif defined(FS_CUBEREFLECTIONMAP)\n\
			// Cubic environment mapping\n\
			// There's no need to normalize the reflection vector when using cube maps\n\
			reflectionColor = texCUBE(ReflectionMap, mul(ViewSpaceToWorldSpace, reflect(In.PositionVS, normal))).rgb;\n\
		#endif\n\
		// Perform sRGB to linear space conversion (gamma correction)\n\
		#ifdef FS_GAMMACORRECTION\n\
			reflectionColor = pow(reflectionColor, 2.2f);\n\
		#endif\n\
		// Apply reflection color\n\
		reflectionColor *= ReflectionColor;\n\
\n\
		// Calculate the final diffuse color after reflection was applied\n\
		diffuseColor = lerp(diffuseColor, reflectionColor, reflectivity);\n\
	#endif\n\
\n\
	// Specular color and exponent\n\
	#ifdef FS_SPECULAR\n\
		#ifdef FS_SPECULARMAP\n\
			float4 specularMapTexel = tex2D(SpecularMap, TexCoord0);\n\
			float3 specularColor    = specularMapTexel.rgb*SpecularColor;\n\
			float  specularExponent = specularMapTexel.a*SpecularExponent;\n\
		#else\n\
			float3 specularColor = SpecularColor;\n\
			#define specularExponent SpecularExponent\n\
		#endif\n\
	#else\n\
		#define specularColor 0\n\
		#define specularExponent 0\n\
	#endif\n\
\n\
	#ifdef RAMPMAPS\n\
		FS_RAMPMAPS rampMaps;\n\
		#ifdef FS_DIFFUSERAMPMAP\n\
			 rampMaps.DiffuseRampMap = DiffuseRampMap;\n\
		#endif\n\
		#ifdef FS_SPECULARRAMPMAP\n\
			rampMaps.SpecularRampMap = SpecularRampMap;\n\
		#endif\n\
		#ifdef FS_EDGERAMPMAP\n\
			rampMaps.EdgeRampMap = EdgeRampMap;\n\
		#endif\n\
	#endif\n\
\n\
	// Perform the lighting calculation in view space\n\
	float3 position = In.PositionVS;\n\
	#ifdef FS_DIRECTIONAL\n\
		// Perform the Blinn-Phong lighting calculation\n\
		#ifdef RAMPMAPS\n\
			float3 lightingColor = BlinnPhong(LightDirection, LightColor, normalize(-position), normal, diffuseColor, specularColor, specularExponent, rampMaps);\n\
		#else\n\
			float3 lightingColor = BlinnPhong(LightDirection, LightColor, normalize(-position), normal, diffuseColor, specularColor, specularExponent);\n\
		#endif\n\
\n\
		// Not shadowed by default\n\
		#define shadow 1\n\
	#else\n\
		// Calculate the view space light vector pointing from the position to the light position\n\
		float3 lightVector = LightPosition - position;\n\
\n\
		// Get the distance between the light and position\n\
		float distance = length(lightVector);\n\
\n\
		// If the position is outside of area of effect, discard position\n\
		if (distance > LightRadius) {\n\
			// Early escape: Not influenced by the light\n\
			#ifdef FS_DISCARD\n\
				discard;\n\
			#else\n\
				Out.Color0 = 0;\n\
				return Out;\n\
			#endif\n\
		}\n\
\n\
		// Perform shadow mapping\n\
		#ifdef FS_SHADOWMAPPING\n\
			#ifdef FS_SPOT\n\
				// Calculate the shadow vector\n\
				float4 shadowVector = mul(ViewSpaceToShadowMapSpace, float4(position, 1));\n\
\n\
				// Shadow mapping\n\
				#ifdef FS_SOFTSHADOWMAPPING\n\
					// Fake PCF (percentage-closer filtering)\n\
					float shadow = (texPCF(ShadowMap, shadowVector, float2(-TexelSize,  TexelSize)).x +\n\
									texPCF(ShadowMap, shadowVector, float2( TexelSize,  TexelSize)).x +\n\
									texPCF(ShadowMap, shadowVector, float2(-TexelSize, -TexelSize)).x +\n\
									texPCF(ShadowMap, shadowVector, float2( TexelSize, -TexelSize)).x) * 0.25f;\n\
				#else\n\
					float shadow = tex2Dproj(ShadowMap, shadowVector).x;\n\
				#endif\n\
			#else\n\
				// Shadow mapping\n\
				float3 shadowVector = mul(ViewSpaceToShadowCubeMapSpace, -lightVector*InvLightRadius);\n\
				float shadowVecLength = length(shadowVector);\n\
				#ifdef FS_SOFTSHADOWMAPPING\n\
					// Shadowed?\n\
					float shadow = (shadowVecLength < texPCF(ShadowMap, shadowVector, float3(TexelSize, TexelSize, TexelSize))) ? 0.16666667 : 0;\n\
					shadow += (shadowVecLength < texPCF(ShadowMap, shadowVector, float3(-TexelSize, -TexelSize, -TexelSize))) ? 0.16666667 : 0;\n\
					shadow += (shadowVecLength < texPCF(ShadowMap, shadowVector, float3( TexelSize, -TexelSize, -TexelSize))) ? 0.16666667 : 0;\n\
					shadow += (shadowVecLength < texPCF(ShadowMap, shadowVector, float3(-TexelSize,  TexelSize, -TexelSize))) ? 0.16666667 : 0;\n\
					shadow += (shadowVecLength < texPCF(ShadowMap, shadowVector, float3( TexelSize, -TexelSize,  TexelSize))) ? 0.16666667 : 0;\n\
					shadow += (shadowVecLength < texPCF(ShadowMap, shadowVector, float3(-TexelSize,  TexelSize,  TexelSize))) ? 0.16666667 : 0;\n\
					shadow = clamp(shadow, 0, 1);\n\
				#else\n\
					// Unpack\n\
					float depthValue = texPCF(ShadowMap, shadowVector);\n\
\n\
					// Shadowed?\n\
					float shadow = (shadowVecLength < depthValue) ? 1 : 0;\n\
				#endif\n\
			#endif\n\
\n\
			// Is the position completely shadowed?\n\
			if (shadow <= 0) {\n\
				// Early escape: Not influenced by the light\n\
				#ifdef FS_DISCARD\n\
					discard;\n\
				#else\n\
					Out.Color0 = 0;\n\
					return OUT;\n\
				#endif\n\
			}\n\
		#else\n\
			// Not shadowed by default\n\
			#define shadow 1\n\
		#endif\n\
\n\
		// Get the light color (lightColor)\n\
		#ifdef FS_PROJECTIVE_POINT\n\
			// Calculate the cube map space vector ('-' -> The light is the source of the ray, not the surface!)\n\
			float3 cubeMapVector = mul(ViewSpaceToCubeMapSpace, -lightVector);\n\
\n\
			// Get cube map texture for projective point light\n\
			float3 lightColor = texCUBE(ProjectivePointCubeMap, cubeMapVector).rgb;\n\
			// Perform sRGB to linear space conversion (gamma correction)\n\
			#ifdef FS_GAMMACORRECTION\n\
				lightColor = pow(lightColor, 2.2f);\n\
			#endif\n\
			// Apply light color\n\
			lightColor *= LightColor;\n\
		#else\n\
			// Start with the original light color\n\
			float3 lightColor = LightColor;\n\
\n\
			// Add spot light\n\
			#ifdef FS_SPOT\n\
				// Projective spot map\n\
				#ifdef FS_SPOT_PROJECTIVE\n\
					// Calculate the projective spot map texture coordinate\n\
					float4 projectiveSpotMapUV = mul(ViewSpaceToSpotMapSpace, -float4(position, 1));\n\
\n\
					// No back projection, please!\n\
					if (projectiveSpotMapUV.z < 0) {\n\
						// Early escape: Not influenced by the light\n\
						#ifdef FS_DISCARD\n\
							discard;\n\
						#else\n\
							Out.Color0 = 0;\n\
							return Out;\n\
						#endif\n\
					}\n\
\n\
					// Get the projective spot map texel data\n\
					float3 projectiveSpotMapTexel = tex2Dproj(ProjectiveSpotMap, projectiveSpotMapUV).rgb;\n\
					// Perform sRGB to linear space conversion (gamma correction)\n\
					#ifdef FS_GAMMACORRECTION\n\
						projectiveSpotMapTexel = pow(projectiveSpotMapTexel, 2.2f);\n\
					#endif\n\
					// Modulate the color of the light using the projective spot map texel data\n\
					lightColor *= projectiveSpotMapTexel;\n\
				#endif\n\
\n\
				// Spot light with cone\n\
				#ifdef FS_SPOT_CONE\n\
					// Calculate the angle between the current position and the spot light\n\
					float currentSpotConeCos = dot(normalize(position - LightPosition), LightDirection);\n\
\n\
					// Spot light with smooth cone\n\
					#ifdef FS_SPOT_SMOOTHCONE\n\
						// Get the current smooth spot cone attenuation\n\
						float currentSpotConeCosAttenuation = smoothstep(SpotConeCos.x, SpotConeCos.y, currentSpotConeCos);\n\
\n\
						// Is the position completely outside the spot cone?\n\
						if (currentSpotConeCosAttenuation <= 0) {\n\
							// Early escape: The position is outside the light cone and therefore not influenced by the light\n\
							#ifdef FS_DISCARD\n\
								discard;\n\
							#else\n\
								Out.Color0 = 0;\n\
								return Out;\n\
							#endif\n\
						}\n\
\n\
						// Modulate the color of the light using the current smooth spot cone attenuation\n\
						lightColor *= currentSpotConeCosAttenuation;\n\
					#else\n\
						// A position is inside or outside the spot cone, there is no 'in between'\n\
						if (SpotConeCos > currentSpotConeCos) {\n\
							// Early escape: The position is outside the light cone and therefore not influenced by the light\n\
							#ifdef FS_DISCARD\n\
								discard;\n\
							#else\n\
								Out.Color0 = 0;\n\
								return Out;\n\
							#endif\n\
						}\n\
					#endif\n\
				#endif\n\
			#endif\n\
		#endif\n\
\n\
		// Perform the lighting calculation in view space\n\
		#ifdef RAMPMAPS\n\
			float3 lightingColor = BlinnPhong(normalize(lightVector), lightColor, -normalize(position), normal, diffuseColor, specularColor, specularExponent, rampMaps);\n\
		#else\n\
			float3 lightingColor = BlinnPhong(normalize(lightVector), lightColor, -normalize(position), normal, diffuseColor, specularColor, specularExponent);\n\
		#endif\n\
\n\
		// Apply attenuation\n\
		lightingColor *= clamp(1 - distance/LightRadius, 0, 1);\n\
	#endif\n\
\n\
	// Calculate the fragment color\n\
	Out.Color0 = float4(lightingColor*shadow, 1.0f);\n\
\n\
	// Done\n\
	return Out;\n\
}";
static const PLCore::String sLighting_Cg_FS = sLighting_Cg_FS_Part1 + sLighting_Cg_FS_Part2;
