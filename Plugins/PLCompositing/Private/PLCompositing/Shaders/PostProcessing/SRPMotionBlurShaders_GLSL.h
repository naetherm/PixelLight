/*********************************************************\
 *  File: SRPMotionBlurShaders_GLSL.h                     *
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


// OpenGL 2.0 ("#version 110") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sMotionBlur_GLSL_VS = STRINGIFY(
// Attributes
attribute vec4 VertexPosition;	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
								// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (1,1)

// Programs
void main()
{
	// Set the clip space vertex position
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);

	// Pass through the vertex texture coordinate
	VertexTexCoordVS = VertexPosition.zw;
}
);	// STRINGIFY


// OpenGL 2.0 ("#version 110") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sMotionBlur_GLSL_FS = "\
// GLSL extensions\n\
#extension GL_ARB_texture_rectangle : enable\n\
\n\
// Attributes\n\
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 input from vertex shader, lower/left is (0,0) and upper/right is (1,1)\n\
\n\
// Uniforms\n\
uniform ivec2		  TextureSize;						// Texture size in texel\n\
uniform sampler2DRect ColorTexture;						// Color texture\n\
uniform sampler2DRect DepthTexture;						// Depth texture\n\
uniform mat4		  ClipSpaceToWorldSpace;			// Clip space to world space transform matrix\n\
uniform mat4		  PreviousWorldSpaceToClipSpace;	// Previous world space to clip space transform matrix\n\
uniform float		  NumberOfSamples;					// Number of samples, must be >0 (should be of integer type, but that's causing troubles on some GPU drivers, see PLCompositing diary entry 02.06.2011 for more details)\n\
uniform float		  VelocityFactor;					// Velocity factor\n\
\n\
// Programs\n\
void main()\n\
{\n\
	// Get per-pixel world space position\n\
		// Get the depth buffer value at this pixel\n\
		float zOverW = texture2DRect(DepthTexture, VertexTexCoordVS*vec2(TextureSize)).x;\n\
\n\
		// H is the viewport position at this pixel in the range -1 to 1\n\
		vec4 H = vec4(VertexTexCoordVS.x*2.0 - 1.0, VertexTexCoordVS.y*2.0 - 1.0, zOverW*2.0 - 1.0, 1.0);\n\
\n\
		// Transform by the view-projection inverse\n\
		vec4 D = ClipSpaceToWorldSpace*H;\n\
\n\
		// Divide by w to get the world position\n\
		vec4 worldPos = D/D.w;\n\
\n\
	// Calculate the per-pixel velocity vectors that determine the direction to blur the image\n\
		// Current viewport position\n\
		vec4 currentPos = H;\n\
\n\
		// Use the world position, and transform by the previous view-projection matrix\n\
		vec4 previousPos = PreviousWorldSpaceToClipSpace*worldPos;\n\
\n\
		// Convert to nonhomogeneous points [-1,1] by dividing by w\n\
		previousPos /= previousPos.w;\n\
\n\
		// Use this frame's position and last frame's to compute the pixel velocity\n\
		// -> /2.0 is build into VelocityFactor\n\
		vec2 velocity = (currentPos.xy - previousPos.xy)*VelocityFactor;\n\
\n\
	// Show only velocity, everything else black? (for debugging)\n\
	#ifdef FS_SHOW_ONLY_VELOCITY\n\
		gl_FragColor = vec4(velocity, 0.0, 1.0);\n\
	#else\n\
		// Use the velocity vector at the current pixel to sample the color buffer multiple times to achieve the motion blur effect\n\
		vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n\
		for (int i=0; i<int(NumberOfSamples); i++) {\n\
			// Get offset in range [-0.5, 0.5]\n\
			float scale = (float(i)/NumberOfSamples - 0.5);\n\
			vec2 offset = scale*velocity;\n\
\n\
			// Sample and add to result\n\
			color += texture2DRect(ColorTexture, (VertexTexCoordVS + offset)*vec2(TextureSize));\n\
		}\n\
\n\
		// Show velocity? (for debugging)\n\
		#ifdef FS_SHOW_VELOCITY\n\
			// Average all of the samples to get the final blur color\n\
			gl_FragColor = color/NumberOfSamples*vec4(1.0 - velocity, 1.0, 1.0);\n\
		#else\n\
			// Average all of the samples to get the final blur color\n\
			gl_FragColor = color/NumberOfSamples;\n\
		#endif\n\
	#endif\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
