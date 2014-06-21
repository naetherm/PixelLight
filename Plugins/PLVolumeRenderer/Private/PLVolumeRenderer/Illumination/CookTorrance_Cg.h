/*********************************************************\
 *  File: CookTorrance_Cg.h                              *
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


// Cook-Torrance and Torrance-Sparrow are just other names for the same algorithm


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform float3 LightColor;	// Light color


//[-------------------------------------------------------]
//[ Begin: External shader                                ]
//[-------------------------------------------------------]
// From the "Microfacet Based Bidirectional Refectance Distribution Function"-paper
// -> http://jbit.net/~sparky/academic/mm_brdf.pdf
// -> jbit.net/~sparky/academic/illum.h
/**
 *  Copyright (C) 2011 by Morten S. Mikkelsen
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 */


/// IMPORTANT!!!! ///
/// IMPORTANT!!!! ///
// All these are implemented such that n_dot_l is built into the BRDF
// and thus the shader using them should NOT multiply by this factor.
// The diffuse term is also built into the BRDF.

// Another important observation is that most implementations of the Torrance-Sparrow
// or Cook-Torrance model neglect to deal with division by zero issues.
// Most often those that do don't do it well such that the right limit value is determined
// giving a smooth/continuous behavior.

// A clear derivation of the Torrance-Sparrow model is given in section 2.4
// of my paper --> http://jbit.net/~sparky/academic/mm_brdf.pdf
// Note the error in the cook-torrance model pointed out at the end of this section.
// Another important observation is that the bechmann distribution can be replaced
// with a normalized phong distribution (which is cheaper) using: float toNPhong(const float m)


// [CHANGED]
const float M_PI = 3.1415926535897932384626433832795;
// Was
//#ifndef M_PI
//	#define M_PI 3.1415926535897932384626433832795
//#endif

float3 FixNormal(float3 vN, float3 vV);
float toBeckmannParam(const float n);
float toNPhong(const float m);

// Schlick's Fresnel approximation
float fresnelReflectance( float VdotH, float F0 )
{
	float base = 1-VdotH;
	float exponential = pow(base, 5.0);	// replace this by 3 muls for C/C++
	return saturate(exponential + F0 * (1 - exponential));
}

// [CHANGED]
const float FLT_EPSILON = 1.192092896e-07F;        // smallest such that 1.0+FLT_EPSILON != 1.0
const float FLT_MAX     = 3.402823466e+38F;        // max value
const float FLT_MIN     = 1.175494351e-38F;        // min positive value
// Was
//#define FLT_EPSILON     1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0
//#define FLT_MAX         3.402823466e+38F        // max value
//#define FLT_MIN         1.175494351e-38F        // min positive value


// The Torrance-Sparrow visibility factor, G,
// as described by Jim Blinn but divided by VdotN
// Note that this was carefully implemented such that division
// by zero problems and awkward discontinuities are avoided.
float VisibDiv(float LdotN, float VdotN, float VdotH, float HdotN)
{
	// VdotH should never be zero. Only possible if
	// L and V end up in the same plane (unlikely).
	const float denom = max( VdotH, FLT_EPSILON );

	float numL = min(VdotN, LdotN);
	const float numR = 2*HdotN;
	if((numL*numR)<=denom)	// min(x,1) = x
	{
		numL = numL == VdotN ? 1.0 : (LdotN / VdotN);	// VdotN is > 0 if this division is used
		return (numL*numR) / denom;
	}
	else					// min(x,1) = 1				this branch is taken when H and N are "close" (see fig. 3)
		return 1.0 / VdotN;
		// VdotN >= HdotN*VdotN >= HdotN*min(VdotN, LdotN) >= FLT_EPSILON/2
}


// this is a normalized Phong model used in the Torrance-Sparrow model
float3 BRDF_ts_nphong(float3 vN, float3 vL, float3 vV, float3 Cd, float3 Cs, float n=32, float F0=0.2)
{
	// reflect hack when view vector is occluded
	// (doesn't seem to be needed)
	//vN = FixNormal(vN, vV);

	// microfacet normal
	float3 vH = normalize(vV+vL);

	// the various dot products
	const float LdotN = saturate(dot(vL, vN));
	const float VdotN = saturate(dot(vV, vN));
	const float VdotH = saturate(dot(vV, vH));
	const float HdotN = saturate(dot(vH, vN));

	// diffuse
	float fDiff = LdotN;

	// D is a surface distribution function and obeys:
	// D(vH)*HdotN is normalized (over half-spere)
	// Specifically, this is the normalized phong model
	const float D = ((n+2)/(2*M_PI))*pow(HdotN, n);

	// torrance-sparrow visibility term divided by VdotN
	const float fVdivDots = VisibDiv(LdotN, VdotN, VdotH, HdotN);

	// Schlick's approximation
	const float fFres = fresnelReflectance(VdotH, F0);

	// torrance-sparrow:
	// (F * G * D) / (4 * LdotN * VdotN)
	// Division by VdotN is done in VisibDiv()
	// and division by LdotN is removed since 
	// outgoing radiance is determined by:
	// BRDF * LdotN * L()
	float fSpec = (fFres * fVdivDots * D) / 4;

	// sum up: diff + spec
	// technically fDiff should be divided by pi.
	// Instead, we choose here to scale Cs by pi
	// which makes the final result scaled by pi.
	// We do this to keep the output intensity range
	// at a level which is more "familiar".
	float3 res = Cd * fDiff + M_PI * Cs * fSpec;
	return res;
}
//[-------------------------------------------------------]
//[ End: External shader                                  ]
//[-------------------------------------------------------]


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
float3 Illumination(float3 SurfaceColor, float3 SurfaceNormal, float3 ViewingDirection, float3 LightDirection)
{
	// Call the external shader
	return LightColor*BRDF_ts_nphong(SurfaceNormal, LightDirection, ViewingDirection, SurfaceColor, float3(0.2, 0.2, 0.2), 32, 0.2);
}
);	// STRINGIFY


// Buggy: Produces at certain angles ugly lighting artifacts and also NAN values
// -> "Cook-Torrance local illumination" as described within the book "Real-Time Volume Graphics", section "5.4.2 Other Shading Models" (page 121).
// -> Set "DebugCheckAndFixNANValues"-flag within "PLCompositing::SRPEndHDR" in order to check for NAN values (in case something is found, you will be informed)
/*
float3 Illumination(float3 SurfaceColor, float3 SurfaceNormal, float3 ViewingDirection, float3 LightDirection)
{
	// Input from the system
	float3 N = SurfaceNormal;
	float3 V = ViewingDirection;
	float3 L = LightDirection;

	// material properties
	float3 Kd = float3(0.6, 0.6, 0.6); // diffuse
	float3 Ks = float3(0.2, 0.2, 0.2); // specular
	float mean = 0.7; // mean value of microfacet distribution
	float scale = 0.2; // constant factor C
	// light properties
	float3 lightIntensity = float3(1.0, 1.0, 1.0);
	float3 H = normalize(L + V);
	float n_h = dot(N,H);
	float n_v = dot(N,V);
	float v_h = dot(V,H);
	float n_l = dot(N,L);
	float3 diffuse = Kd * max(n_l,0);
	// approximate Fresnel term
	float fresnel = pow(1.0 + v_h,4);
	// approximate microfacet distribution
	float delta = acos(n_h).x;
	float exponent = -pow((delta/mean),2);
	float microfacets = scale * exp(exponent);
	// calculate self-shadowing term
	float term1 = 2 * n_h * n_v/v_h;
	float term2 = 2 * n_h * n_l/v_h;
	float selfshadow = min(1,min(term1,term2));
	// calculate Cook-Torrance model
	float3 specular = Ks *fresnel *microfacets *selfshadow / n_v;
	return LightColor * (diffuse + specular);
}
);	// STRINGIFY
*/


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
