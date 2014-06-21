/*********************************************************\
 *  File: SRPMotionBlurShaders_Cg.h                      *
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
static const PLCore::String sMotionBlur_Cg_VS = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float2 TexCoord : TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
};

// Programs
VS_OUTPUT main(float4 VertexPosition : POSITION)	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
{
	VS_OUTPUT OUT;

	// Pass through the vertex position
	OUT.Position = float4(VertexPosition.xy, 0, 1);

	// Pass through the vertex texture coordinate
	OUT.TexCoord = VertexPosition.zw;

	// Done
	return OUT;
}
);	// STRINGIFY


// Cg fragment shader source code
static const PLCore::String sMotionBlur_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord : TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Programs\n\
FS_OUTPUT main(VS_OUTPUT   IN								// Interpolated output from the vertex stage\n\
	 , uniform int2		   TextureSize						// Texture size in texel\n\
	 , uniform samplerRECT ColorTexture						// Color texture\n\
	 , uniform samplerRECT DepthTexture						// Depth texture\n\
	 , uniform float4x4	   ClipSpaceToWorldSpace			// Clip space to world space transform matrix\n\
	 , uniform float4x4	   PreviousWorldSpaceToClipSpace	// Previous world space to clip space transform matrix\n\
	 , uniform float	   NumberOfSamples					// Number of samples, must be >0 (should be of integer type, but that's causing troubles on some GPU drivers, see PLCompositing diary entry 02.06.2011 for more details)\n\
	 , uniform float	   VelocityFactor					// Velocity factor\n\
	)\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Get per-pixel world space position\n\
		// Get the depth buffer value at this pixel\n\
		float zOverW = texRECT(DepthTexture, IN.TexCoord*TextureSize).x;\n\
\n\
		// H is the viewport position at this pixel in the range -1 to 1\n\
		float4 H = float4(IN.TexCoord.x*2.0 - 1.0, IN.TexCoord.y*2.0 - 1.0, zOverW*2.0 - 1.0, 1.0);\n\
\n\
		// Transform by the view-projection inverse\n\
		float4 D = mul(ClipSpaceToWorldSpace, H);\n\
\n\
		// Divide by w to get the world position\n\
		float4 worldPos = D/D.w;\n\
\n\
	// Calculate the per-pixel velocity vectors that determine the direction to blur the image\n\
		// Current viewport position\n\
		float4 currentPos = H;\n\
\n\
		// Use the world position, and transform by the previous view-projection matrix\n\
		float4 previousPos = mul(PreviousWorldSpaceToClipSpace, worldPos);\n\
\n\
		// Convert to nonhomogeneous points [-1,1] by dividing by w\n\
		previousPos /= previousPos.w;\n\
\n\
		// Use this frame's position and last frame's to compute the pixel velocity\n\
		// -> /2.0 is build into VelocityFactor\n\
		float2 velocity = (currentPos.xy - previousPos.xy)*VelocityFactor;\n\
\n\
	// Use the velocity vector at the current pixel to sample the color buffer multiple times to achieve the motion blur effect\n\
		float4 color = float4(0.0, 0.0, 0.0, 0.0);\n\
		for (int i=0; i<int(NumberOfSamples); i++) {\n\
			// Get offset in range [-0.5, 0.5]\n\
			float scale = (float(i)/NumberOfSamples - 0.5);\n\
			float2 offset = scale*velocity;\n\
\n\
			// Sample and add to result\n\
			color += texRECT(ColorTexture, (IN.TexCoord + offset)*TextureSize);\n\
		}\n\
\n\
		// Average all of the samples to get the final blur color\n\
		float4 finalColor = color/NumberOfSamples;\n\
\n\
	// Set the resulting color\n\
	OUT.Color0 = finalColor;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
