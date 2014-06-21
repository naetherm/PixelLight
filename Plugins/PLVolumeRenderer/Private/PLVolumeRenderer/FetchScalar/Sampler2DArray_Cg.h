/*********************************************************\
 *  File: Sampler2DArray_Cg.h                            *
 *      Fragment shader source code - Cg
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


static const PLCore::String sSourceCode_Fragment_Header = STRINGIFY(
// Uniforms
uniform sampler2DARRAY VolumeTexture;		// Volume texture map
uniform float          VolumeTextureDepth;	// Depth of the volume texture (z-component)
uniform float          VolumeTextureLOD;	// Volume texture level of detail (0...<number of mipmaps>-1)

/**
*  @brief
*    Fetches a scalar by using a given position inside the volume
*
*  @param[in] Position
*    Position inside the volume to fetch the scalar from, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    Scalar, usually in the interval [0 .. 1]
*/
float FetchScalar(float3 Position)
{
	// Fetch and return the requested scalar
	// -> There's no automatic filtering between the array layers
	return tex2DARRAYlod(VolumeTexture, float4(Position.xy, Position.z*VolumeTextureDepth, VolumeTextureLOD)).r;	// z-component = array layer index, must be an integer between [0...depth-1]
}

/**
*  @brief
*    Fetches a scalar by using a given position inside the volume by using a texel offset
*
*  @param[in] Position
*    Position inside the volume to fetch the scalar from, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] Offset
*    Texel offset
*
*  @return
*    Scalar, usually in the interval [0 .. 1]
*/
float FetchScalarOffset(float3 Position, int3 Offset)
{
	// Fetch and return the requested scalar
	return tex2DARRAYlod(VolumeTexture, float4(Position.xy, Position.z*VolumeTextureDepth, VolumeTextureLOD), Offset).r;	// z-component = array layer index, must be an integer between [0...depth-1]
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
