/*********************************************************\
 *  File: Readme.txt                                     *
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


The volume rendering shader system is split up into separate components.
- Inspired by the book "Real-Time Volume Graphics", section "1.6 Volume-Rendering Pipeline and Basic Approaches" (page 25)
- The following function signatures are using GLSL syntax (for Cg, just replace e.g. "vec4" by "float4")
- "2.2 - Reconstruction" and "2.2 - Fetch Scalar" are both pipeline step 2.2 by intent because "reconstruction" can be seen as a high-order texture filtering
- "2.5 - Gradient" and "2.5 - Gradient Input" are both pipeline step 2.5 by intent, just decoupled to avoid an combinatorial explosion of hand-written shader building blocks


//[-------------------------------------------------------]
//[ 1.0 - Ray Setup                                       ]
//[-------------------------------------------------------]
- Main component responsible for
	- The ray setup
	- Calling the clip ray function (see "1.1 - Clip Ray")
	- Calling the jitter position function (see "1.2 - Jitter Position") for adding jittering to the start position of the ray in order to reduce wooden grain effect
	- Calling the ray traversal function (see "2.0 - Ray Traversal")
	- Writing out the final fragment data


//[-------------------------------------------------------]
//[ 1.1 - Clip Ray                                        ]
//[-------------------------------------------------------]
- Signature of the clip ray function:
	/**
	*  @brief
	*    Performs an clipping operation on the given ray
	*
	*  @param[in, out] RayOrigin
	*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*  @param[in]      RayDirection
	*    Normalized ray direction
	*  @param[in, out] MaximumTravelLength
	*    Maximum travel length along the ray, within the interval [0 .. 1]
	*/
	void ClipRay(inout vec3 RayOrigin, vec3 RayDirection, inout float MaximumTravelLength)


//[-------------------------------------------------------]
//[ 1.2 - Jitter Position                                 ]
//[-------------------------------------------------------]
- Signature of the jitter position function:
	/**
	*  @brief
	*    Jitter position to the start position of the ray in order to reduce wooden grain effect
	*
	*  @param[in] Position
	*    Position inside the volume to perform jitter on, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*
	*  @return
	*    Jitter factor, usually in the interval [0 .. 1]
	*/
	float JitterPosition(vec3 Position)


//[-------------------------------------------------------]
//[ 2.0 - Ray Traversal                                   ]
//[-------------------------------------------------------]
- Signature of the ray traversal function:
	/**
	*  @brief
	*    Integrates over the volume
	*
	*  @param[in] StartPosition
	*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*  @param[in] NumberOfSteps
	*    Number of steps along the ray (= number of samples to take), must be positive
	*  @param[in] StepPositionDelta
	*    Position advance per step along the ray, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*  @param[in] MaximumTravelLength
	*    Maximum travel length along the ray, within the interval [0 .. 1]
	*
	*  @return
	*    RGBA result of the ray traversal
	*/
	vec4 RayTraversal(vec3 StartPosition, int NumberOfSteps, vec3 StepPositionDelta, float MaximumTravelLength)
- Calling the clip position function (see "2.1 - Clip Position")
- Calling the reconstruction function (see "2.2 - Reconstruction")
- Calling the shading function (see "2.3 - Shading")


//[-------------------------------------------------------]
//[ 2.1 - Clip Position                                   ]
//[-------------------------------------------------------]
- Signature of the clip position function:
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
	bool ClipPosition(vec3 Position)


//[-------------------------------------------------------]
//[ 2.2 - Reconstruction                                  ]
//[-------------------------------------------------------]
- Signature of the reconstruction function:
	/**
	*  @brief
	*    Reconstructs a scalar by using a given position inside the volume
	*
	*  @param[in] Position
	*    Position inside the volume were to reconstruct the scalar, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*
	*  @return
	*    Reconstructed scalar, usually in the interval [0 .. 1]
	*/
	float Reconstruction(vec3 Position)
- Calling the fetch scalar function (see "2.2 - Fetch Scalar")


//[-------------------------------------------------------]
//[ 2.2 - Fetch Scalar                                    ]
//[-------------------------------------------------------]
- Signature of the fetch scalar function:
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
	float FetchScalar(vec3 Position)
- Signature of the fetch scalar function by using a texel offset:
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
	float FetchScalarOffset(vec3 Position, ivec3 Offset)


//[-------------------------------------------------------]
//[ 2.3 - Shading                                         ]
//[-------------------------------------------------------]
- Signature of the shading function:
	/**
	*  @brief
	*    Shading
	*
	*  @param[in] Scalar
	*    Current voxel scalar
	*  @param[in] Position
	*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*  @param[in] StepPositionDelta
	*    Position advance per step along the ray, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*
	*  @return
	*    RGBA result of the shading
	*/
	vec4 Shading(float Scalar, vec3 Position, vec3 StepPositionDelta)
- Calling the classification function (see "2.4 - Classification")
- Calling the gradient function (see "2.5 - Gradient")
- Calling the illumination function (see "2.6 - Illumination")


//[-------------------------------------------------------]
//[ 2.4 - Classification                                  ]
//[-------------------------------------------------------]
- Signature of the classification function:
	/**
	*  @brief
	*    Scalar classification
	*
	*  @param[in] Scalar
	*    Scalar to perform a classification on
	*
	*  @return
	*    RGBA result of the classification
	*/
	vec4 Classification(float Scalar)
- Calling the fetch scalar function (see "2.2 - Fetch Scalar")


//[-------------------------------------------------------]
//[ 2.5 - Gradient                                        ]
//[-------------------------------------------------------]
- Signature of the gradient function:
	/**
	*  @brief
	*    Gradient
	*
	*  @param[in] Position
	*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*
	*  @return
	*    Gradient (not normalized)
	*/
	vec3 Gradient(vec3 Position)
- Calling the gradient input function (see "2.5 - Gradient Input")


//[-------------------------------------------------------]
//[ 2.5 - Gradient Input                                  ]
//[-------------------------------------------------------]
- Signature of the gradient function:
	/**
	*  @brief
	*    Fetches the gradient input by using a given position inside the volume
	*
	*  @param[in] Position
	*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*
	*  @return
	*    Gradient input
	*/
	float GradientInput(vec3 Position)
- Signature of the gradient input function by using a texel offset:
	/**
	*  @brief
	*    Fetches the gradient input by using a given position inside the volume by using a texel offset
	*
	*  @param[in] Position
	*    Current position along the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
	*  @param[in] Offset
	*    Texel offset
	*
	*  @return
	*    Gradient input
	*/
	float GradientInputOffset(vec3 Position, ivec3 Offset)


//[-------------------------------------------------------]
//[ 2.6 - Illumination                                    ]
//[-------------------------------------------------------]
- Signature of the illumination function:
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
