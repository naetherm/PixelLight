/*********************************************************\
 *  File: SRPDeferredLighting_Cg.h                       *
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


// Cg vertex shader source code
static const PLCore::String sDeferredLighting_Cg_VS = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 Position			  : POSITION;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float2 TexCoord			  : TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)
	float2 TexCoordNormalized : TEXCOORD1;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
};

// Main function
VS_OUTPUT main(float4 VertexPosition  : POSITION,	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
			   float2 VertexTexCoord0 : TEXCOORD0,	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
	   uniform int2   TextureSize)					// Texture size
{
	VS_OUTPUT OUT;

	// Pass through the vertex position
	OUT.Position = VertexPosition;

	// Pass through the scaled vertex texture coordinate
	OUT.TexCoord = VertexTexCoord0*TextureSize;

	// Pass through the vertex texture coordinate
	OUT.TexCoordNormalized = VertexTexCoord0;

	// Done
	return OUT;
}
);	// STRINGIFY


// Cg fragment shader source code
static const PLCore::String sDeferredLighting_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position			  : POSITION;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord			  : TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
	float2 TexCoordNormalized : TEXCOORD1;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Performs the Blinn-Phong lighting calculation\n\
float3 BlinnPhong(float3 lightVector, float3 lightColor, float3 viewVector, float3 normalVector, float3 diffuseColor, float3 specularColor, float specularExponent)\n\
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
	float3 diffuseLighting = clamp(dot(lightVector, normalVector), 0.0f, 1.0f)*diffuseColor*lightColor;\n\
\n\
	// Specular term\n\
	#ifdef FS_NO_SPECULAR\n\
		#define specularLighting 0\n\
	#else\n\
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
		float3 specularLighting = pow(clamp(dot(halfVector, normalVector), 0.0f, 1.0f), specularExponent)*specularColor*lightColor;\n\
	#endif\n\
\n\
	// Final color\n\
	return diffuseLighting + specularLighting;\n\
}\n\
\n\
// Fake PCF (percentage-closer filtering)\n\
float4 texPCF(sampler2D map, float4 location, float2 offset)\n\
{\n\
	return tex2Dproj(map, float4(location.xy + location.w*offset, location.z, location.w));\n\
}\n\
float texPCF(samplerCUBE map, float3 location)\n\
{\n\
	// Get shadow map data\n\
	float4 shadowMap = texCUBE(map, location);\n\
\n\
	// Unpack\n\
	return shadowMap.r/1 + shadowMap.g/256 + shadowMap.b/65536 + shadowMap.a/16777216;\n\
}\n\
float texPCF(samplerCUBE map, float3 location, float3 offset)\n\
{\n\
	// Get shadow map data\n\
	float4 shadowMap = texCUBE(map, location + offset);\n\
\n\
	// Unpack\n\
	return shadowMap.r/1 + shadowMap.g/256 + shadowMap.b/65536 + shadowMap.a/16777216;\n\
}\n\
\n\
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
// Reconstructs view-space position using given parameters\n\
float3 uv_to_eye(float2 uv, float eye_z, float2 invFocalLen)\n\
{\n\
	// Convert from texture space [0,1] range into [-1,1] clip space\n\
	uv = uv*2 - 1;\n\
\n\
	// Reconstructs view-space position\n\
	// [TODO] Why do I need to invert in here, but not within 'SRPDeferredHBAO_Cg'?\n\
	return float3(uv * invFocalLen * eye_z, -eye_z);\n\
}\n\
\n\
// Main function\n\
FS_OUTPUT main(VS_OUTPUT IN										// Interpolated output from the vertex stage\n\
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
			#ifdef FS_PROJECTIVE_SPOT\n\
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
	, uniform float3	  LightColor							// Light color\n\
	, uniform float2	  InvFocalLen							// 1/FocalLen (for example: 0.57771897 x 0.4142136)\n\
	, uniform samplerRECT RenderTargetTexture0					// Render target texture 0\n\
	, uniform samplerRECT RenderTargetTexture1					// Render target texture 1\n\
	, uniform samplerRECT RenderTargetTexture2)					// Render target texture 2\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Reconstruct view-space position (current surface point)\n\
	float4 sampleRT1 = texRECT(RenderTargetTexture1, IN.TexCoord);\n\
	float3 position  = uv_to_eye(IN.TexCoordNormalized, sampleRT1.b, InvFocalLen);\n\
\n\
	// Perform the lighting calculation in view space\n\
#ifdef FS_DIRECTIONAL\n\
	float3 normal    = decodeNormalVector(sampleRT1.rg);\n\
	float4 sampleRT0 = texRECT(RenderTargetTexture0, IN.TexCoord);\n\
	float4 sampleRT2 = texRECT(RenderTargetTexture2, IN.TexCoord);\n\
	#ifdef FS_NO_ALBEDO\n\
		sampleRT0.rgb = 1;	// Set to default\n\
	#endif\n\
	#ifdef FS_NO_SPECULARCOLOR\n\
		sampleRT2.rgb = 1;	// Set to default\n\
	#endif\n\
	#ifdef FS_NO_SPECULAREXPONENT\n\
		sampleRT2.a = 45;	// Set to default\n\
	#endif\n\
	OUT.Color0.rgb = BlinnPhong(LightDirection, LightColor, -normalize(position), normal, sampleRT0.rgb, sampleRT2.rgb, sampleRT2.a);\n\
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
			OUT.Color0 = 0;\n\
			return OUT;\n\
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
				shadow = clamp(shadow, 0.0f, 1.0f);\n\
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
				OUT.Color0 = 0;\n\
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
			#ifdef FS_PROJECTIVE_SPOT\n\
				// Calculate the projective spot map texture coordinate\n\
				float4 projectiveSpotMapUV = mul(ViewSpaceToSpotMapSpace, -float4(position, 1));\n\
\n\
				// No back projection, please!\n\
				if (projectiveSpotMapUV.z < 0) {\n\
					// Early escape: Not influenced by the light\n\
					#ifdef FS_DISCARD\n\
						discard;\n\
					#else\n\
						OUT.Color0 = 0;\n\
						return OUT;\n\
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
							OUT.Color0 = 0;\n\
							return OUT;\n\
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
							OUT.Color0 = 0;\n\
							return OUT;\n\
						#endif\n\
					}\n\
				#endif\n\
			#endif\n\
		#endif\n\
	#endif\n\
\n\
	// Perform the lighting calculation in view space\n\
	float3 normal    = decodeNormalVector(sampleRT1.rg);\n\
	float4 sampleRT0 = texRECT(RenderTargetTexture0, IN.TexCoord);\n\
	float4 sampleRT2 = texRECT(RenderTargetTexture2, IN.TexCoord);\n\
	#ifdef FS_NO_ALBEDO\n\
		sampleRT0.rgb = 1;	// Set to default\n\
	#endif\n\
	#ifdef FS_NO_SPECULARCOLOR\n\
		sampleRT2.rgb = 1;	// Set to default\n\
	#endif\n\
	#ifdef FS_NO_SPECULAREXPONENT\n\
		sampleRT2.a = 45;	// Set to default\n\
	#endif\n\
	OUT.Color0.rgb = BlinnPhong(normalize(lightVector), lightColor, -normalize(position), normal, sampleRT0.rgb, sampleRT2.rgb, sampleRT2.a);\n\
\n\
	// Apply attenuation\n\
	OUT.Color0.rgb *= clamp(1 - distance/LightRadius, 0.0f, 1.0f);\n\
#endif\n\
\n\
	// Apply ambient occlusion or the calculated realtime shadow\n\
#ifdef FS_NO_AMBIENTOCCLUSION\n\
	// Just modulate the calculated lighting color with the calculated shadowing\n\
	OUT.Color0.rgb *= shadow;\n\
#else\n\
	// Modulate the calculated lighting color with the calculated shadowing or the ambient occlusion value, do not multiply both shadow values\n\
	OUT.Color0.rgb *= min(sampleRT0.a, shadow);\n\
#endif\n\
\n\
	// Still here? Write any alpha value so all color components were written.\n\
	OUT.Color0.a = 1;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
