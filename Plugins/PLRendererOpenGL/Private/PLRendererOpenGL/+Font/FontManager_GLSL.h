/*********************************************************\
 *  File: FontManager_GLSL.h                             *
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


//  OpenGL 2.0 ("#version 110") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sVertexShaderSourceCodeGLSL = "\
// Attributes\n\
attribute vec3 VertexPosition;		// Object space vertex position input, lower/left is (0,0) and upper/right is (1,1)\n\
varying   vec2 VertexTexCoordVS;	// Normalized vertex texture coordinate output\n\
\n\
// Uniforms\n\
uniform vec4 GlyphSizePenPosition;			// Object space glyph size (xy) and object space pen position (zw) => scale & bias\n\
uniform vec4 TextureCoordinateMinMax;		// The normalized minimum (xy) and maximum (zw) glyph texture coordinate inside the glyph texture atlas\n\
uniform mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Calculate the object space vertex position\n\
	vec4 position = vec4(VertexPosition.xy*GlyphSizePenPosition.xy + GlyphSizePenPosition.zw, 0, 1);\n\
\n\
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	gl_Position = ObjectSpaceToClipSpaceMatrix*position;\n\
\n\
	// Set the normalized vertex texture coordinate\n\
	if (int(VertexPosition.z) == 0)			// Vertex 0 - lower/left corner\n\
		VertexTexCoordVS = vec2(TextureCoordinateMinMax.x, TextureCoordinateMinMax.w);\n\
	else if (int(VertexPosition.z) == 1)	// Vertex 1 - lower/right corner\n\
		VertexTexCoordVS = TextureCoordinateMinMax.zw;\n\
	else if (int(VertexPosition.z) == 2)	// Vertex 2 - upper/left corner\n\
		VertexTexCoordVS = TextureCoordinateMinMax.xy;\n\
	else if (int(VertexPosition.z) == 3)	// Vertex 3 - upper/right corner\n\
		VertexTexCoordVS = vec2(TextureCoordinateMinMax.z, TextureCoordinateMinMax.y);\n\
}";


// OpenGL 2.0 ("#version 110") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sFragmentShaderSourceCodeGLSL = "\
// Attributes\n\
varying vec2 VertexTexCoordVS;	// Interpolated vertex texture coordinate from vertex shader\n\
\n\
// Uniforms\n\
uniform sampler2D GlyphMap;	// Glyph atlas texture map\n\
uniform vec4	  Color;	// Text color\n\
\n\
// Programs\n\
void main()\n\
{\n\
	gl_FragColor = vec4(Color.r, Color.g, Color.b, texture2D(GlyphMap, VertexTexCoordVS).a*Color.a);\n\
}";
