/*********************************************************\
 *  File: BlinnPhong_GLSL.h                              *
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


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform vec3 LightColor;	// Light color

/**
*  @brief
*    Illumination
*
*  @param[in] SurfaceColor
*    Surface color
*  @param[in] SurfaceNormal
*    Normalized surface normal
*  @param[in] ViewingDirection
*    Normalized viewing direction
*  @param[in] LightDirection
*    Normalized light direction
*
*  @return
*    Illumination result
*/
vec3 Illumination(vec3 SurfaceColor, vec3 SurfaceNormal, vec3 ViewingDirection, vec3 LightDirection)
{
	// [TODO] Cleanup (from the book "Real-Time Volume Graphics")

	// material properties
	vec3 Kd = SurfaceColor; // diffuse
	vec3 Ks = vec3(0.2, 0.2, 0.2); // specular
	float shininess = 100.0; // shininess

	// Calculate halfway vector
	vec3 H = normalize(LightDirection + ViewingDirection);

	// Compute the diffuse term
	float diffuseLight = max(dot(LightDirection, SurfaceNormal), 0);
	vec3 diffuse = Kd * LightColor * diffuseLight;

	// Compute the specular term
	float specularLight = pow(max(dot(H, SurfaceNormal), 0), shininess);
	if (diffuseLight <= 0) specularLight = 0;
	vec3 specular = Ks * LightColor * specularLight;
	return diffuse + specular;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
