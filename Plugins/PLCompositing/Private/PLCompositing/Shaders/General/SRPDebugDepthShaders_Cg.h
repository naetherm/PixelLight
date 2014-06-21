/*********************************************************\
 *  File: SRPDebugDepthShaders_Cg.h                      *
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
static const PLCore::String sDebugDepth_Cg_VS = STRINGIFY(
// Vertex output
struct VS_OUTPUT {
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
	float2 TexCoord : TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)
};

// Programs
VS_OUTPUT main(float4 VertexPosition : POSITION	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
												// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
  , uniform int2	  TextureSize				// Texture size in texel
	)
{
	VS_OUTPUT OUT;

	// Pass through the vertex position
	OUT.Position = float4(VertexPosition.xy, 0, 1);

	// Pass through the scaled vertex texture coordinate
	OUT.TexCoord = VertexPosition.zw*TextureSize;

	// Done
	return OUT;
}
);	// STRINGIFY


// Cg fragment shader source code
static const PLCore::String sDebugDepth_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord : TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Programs\n\
FS_OUTPUT main(VS_OUTPUT   IN		// Interpolated output from the vertex stage\n\
	 , uniform samplerRECT Texture	// Texture\n\
#ifdef FS_LINEARIZE_DEPTH\n\
	 , uniform float2      ZNearFar	// x=Z near plane, y=Z far plane\n\
#endif\n\
	)\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Fetch the required texel data\n\
	float sample = texRECT(Texture, IN.TexCoord).x;\n\
\n\
	// Linearize the depth\n\
	#ifdef FS_LINEARIZE_DEPTH\n\
		sample = (2.0*ZNearFar.x)/(ZNearFar.y + ZNearFar.x - sample*(ZNearFar.y - ZNearFar.x));\n\
	#endif\n\
\n\
	// Set the resulting color\n\
	OUT.Color0 = float4(sample, sample, sample, 1.0);\n\
\n\
	// Done\n\
	return OUT;\n\
}";


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
