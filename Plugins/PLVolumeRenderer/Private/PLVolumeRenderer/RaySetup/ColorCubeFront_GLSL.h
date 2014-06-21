/*********************************************************\
 *  File: ColorCubeFront_GLSL.h                          *
 *      Vertex and fragment shader source code - GLSL
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


//[-------------------------------------------------------]
//[ Vertex shader source code                             ]
//[-------------------------------------------------------]
// GLSL (OpenGL 2.0 ("#version 110") and OpenGL ES 2.0 ("#version 100")) vertex shader source code, "#version" is added by hand
static const PLCore::String sSourceCode_Vertex = STRINGIFY(
// Attributes
attribute highp   vec3 VertexPosition;		// Object space vertex position input
attribute mediump vec3 VertexTexCoord;		// Vertex texture coordinate input
varying   mediump vec3 VertexTexCoordVS;	// Vertex texture coordinate output

// Uniforms
uniform highp mat4 ObjectSpaceToClipSpaceMatrix;	// Object space to clip space matrix

// Programs
void main()
{
	// Calculate the clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	gl_Position = ObjectSpaceToClipSpaceMatrix*vec4(VertexPosition, 1.0);

	// Pass through the vertex texture coordinate
	VertexTexCoordVS = VertexTexCoord;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Fragment shader source code                           ]
//[-------------------------------------------------------]
// GLSL (OpenGL 2.0 ("#version 110") and OpenGL ES 2.0 ("#version 100")) fragment shader source code, "#version" is added by hand
static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Attributes
varying mediump vec3 VertexTexCoordVS;	// Interpolated vertex texture coordinate input from vertex shader, the position (volume object space) the ray enters the volume

// Programs
void main()
{
	// The position (volume object space) the ray enters the volume is provided by the vertex shader
	mediump vec3 rayPosition = VertexTexCoordVS;

	// Fragment color = The position the ray enters the volume = Color cube
	gl_FragColor = vec4(rayPosition.x, rayPosition.y, rayPosition.z, 1.0);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY