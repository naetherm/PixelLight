/*********************************************************\
 *  File: SRPDeferredSSAO_GLSL.h                         *
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


// GLSL vertex shader source code
static const PLCore::String sDeferredSSAO_GLSL_VS =
// GLSL preprocessor directives
"#version 110	// OpenGL 2.0\n"
STRINGIFY(
// Attributes
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
varying   vec4 VertexTexCoordVS;	// xy = Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)
									// zw = Vertex input texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)

// Uniforms
uniform ivec2 TextureSize;		// Texture size in texel
uniform ivec2 InputTextureSize;	// Input size in texel

// Programs
void main()
{
	// Set the clip space vertex position
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);

	// Pass through the scaled vertex texture coordinate
	VertexTexCoordVS.xy = VertexPosition.zw*vec2(TextureSize);
	VertexTexCoordVS.zw = VertexPosition.zw*vec2(InputTextureSize);
}
);	// STRINGIFY


// GLSL vertex shader source code
static const PLCore::String sDeferredSSAO_GLSL_FS =
// GLSL preprocessor directives
"#version 110	// OpenGL 2.0\n\
\n\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n"
STRINGIFY(
// Attributes
varying vec4 VertexTexCoordVS;	// xy = Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)
								// zw = Vertex input texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)

// Uniforms
uniform float			BlurRadius;			// Blur radius
uniform float			BlurFalloff;		// Blur falloff
uniform float			Sharpness;			// Sharpness
uniform vec2			UVScale;			// UV scale
uniform sampler2DRect	InputTexture;		// Input texture
uniform sampler2DRect	NormalDepthTexture;	// RG=normal vector, B=linear view space length

// Programs
float FetchEyeZ(sampler2DRect normalDepthTexture, vec2 uv)
{
	return texture2DRect(normalDepthTexture, uv).b;
}

float BlurFunction(sampler2DRect inputTexture, sampler2DRect normalDepthTexture, float blurFalloff, float sharpness, vec2 uv, vec2 uvInput, float r, float center_c, float center_d, inout float w_total)
{
	float c = texture2DRect(inputTexture, uvInput).r;
	float d = FetchEyeZ(normalDepthTexture, uv);

	float ddiff = d - center_d;
	float w = exp(-r*r*blurFalloff - ddiff*ddiff*sharpness);
	w_total += w;

	return w*c;
}

// Program entry point
void main()
{
	float b = 0.0;
	float w_total = 0.0;
	float center_c = texture2DRect(InputTexture, VertexTexCoordVS.zw).r;
	float center_d = FetchEyeZ(NormalDepthTexture, VertexTexCoordVS.xy);
	for (float r=-BlurRadius; r<=BlurRadius; r++)
		b += BlurFunction(InputTexture, NormalDepthTexture, BlurFalloff, Sharpness, VertexTexCoordVS.xy + r*UVScale, VertexTexCoordVS.zw + r*UVScale, r, center_c, center_d, w_total);
	float value = b/w_total;
	gl_FragColor = vec4(value, value, value, value);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
