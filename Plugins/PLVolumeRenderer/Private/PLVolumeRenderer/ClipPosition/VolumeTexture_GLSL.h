/*********************************************************\
 *  File: VolumeTexture_GLSL.h                           *
 *      Fragment shader source code - GLSL (OpenGL 3.3 ("#version 330")
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


// Template version
static const PLCore::String sSourceCode_Fragment_Template = STRINGIFY(
// Uniforms
uniform mat4      VolumeSpaceToClipVolumeTextureSpace_x_;	// Volume space to clip volume texture space matrix
uniform bool      InvertClipping_x_;						// Invert clipping?
uniform sampler3D ClipVolumeTexture_x_;						// Clip volume texture map
uniform float     ClipThreshold_x_;							// Clip threshold, everything above will be clipped

/**
*  @brief
*    Performs an clipping operation on the given position
*
*  @param[in] Position
*    Position inside the volume to perform clipping on, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    'true' if the given position was clipped, else 'false' if the given position survived the clipping operation
*/
bool ClipPosition_x_(vec3 Position)
{
	// Transform the given volume space position into clip volume texture space
	vec3 position = (VolumeSpaceToClipVolumeTextureSpace_x_*vec4(Position, 1.0)).xyz + vec3(0.5, 0.5, 0.5);

	// In clip volume texture space, the interval is [(0, 0, 0) .. (1, 1, 1)]

	// Does the given position survive the clipping operation or is it clipped away?
	bool clipPosition = (position.x > 0.0 && position.x < 1.0 && position.y > 0.0 && position.y < 1.0 && position.z > 0.0 && position.z < 1.0);
	if (clipPosition)
		clipPosition = (textureLod(ClipVolumeTexture_x_, position, 0.0).r > ClipThreshold_x_);
	return InvertClipping_x_ ? !clipPosition : clipPosition;
}
);	// STRINGIFY

// None-template version
static const PLCore::String sSourceCode_Fragment = sSourceCode_Fragment_Template + '\n' + STRINGIFY(
bool ClipPosition(vec3 Position)
{
	// Call the function version also used by the template
	return ClipPosition_x_(Position);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
