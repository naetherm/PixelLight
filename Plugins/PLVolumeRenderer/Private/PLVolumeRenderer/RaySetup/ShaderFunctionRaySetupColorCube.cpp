/*********************************************************\
 *  File: ShaderFunctionRaySetupColorCube.cpp            *
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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <algorithm>	// [TODO] Test
#include <PLMath/Rectangle.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/VertexShader.h>
#include <PLRenderer/Renderer/ShaderLanguage.h>
#include <PLRenderer/Renderer/FragmentShader.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLRenderer/Renderer/SurfaceTextureBuffer.h>
#include <PLMesh/MeshManager.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLScene/Visibility/SQCull.h>
#include <PLScene/Visibility/VisContainer.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/RaySetup/ShaderFunctionRaySetupColorCube.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionRaySetupColorCube)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionRaySetupColorCube::ShaderFunctionRaySetupColorCube() :
	m_pProgramFront(nullptr),
	m_pColorTargetFront(nullptr),
	m_pCapVertexBuffer(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionRaySetupColorCube::~ShaderFunctionRaySetupColorCube()
{
	// Destroy the color render target for rendering the front side of the color cube
	if (m_pColorTargetFront)
		delete m_pColorTargetFront;

	// Destroy the GPU program for rendering the front side of the color cube
	if (m_pProgramFront) {
		// Get the used vertex shader and fragment shader instance
		VertexShader   *pVertexShader   = m_pProgramFront->GetVertexShader();
		FragmentShader *pFragmentShader = m_pProgramFront->GetFragmentShader();

		// Destroy the GPU program for rendering the front side of the color cube
		delete m_pProgramFront;
		m_pProgramFront = nullptr;

		// Destroy the used vertex shader instance
		if (pVertexShader)
			delete pVertexShader;

		// Destroy the used fragment shader instance
		if (pFragmentShader)
			delete pFragmentShader;
	}

	// Destroy the near cap vertex buffer
	if (m_pCapVertexBuffer)
		delete m_pCapVertexBuffer;
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionRaySetupColorCube::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "ColorCube_GLSL.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case VertexShaderBody:
				return sSourceCode_Vertex;

			case FragmentShaderHeader:
				return sSourceCode_Fragment_Header;

			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case VertexShaderHeader:
			case VertexShaderTemplate:
			case FragmentShaderTemplate:
				// Nothing to do in here
				break;
		}
	} else if (sShaderLanguage == Cg) {
		#include "ColorCube_Cg.h"

		// Return the requested source code
		switch (nSourceCodeType) {
			case VertexShaderBody:
				return sSourceCode_Vertex;

			case FragmentShaderHeader:
				return sSourceCode_Fragment_Header;

			case FragmentShaderBody:
				return sSourceCode_Fragment;

			case VertexShaderHeader:
			case VertexShaderTemplate:
			case FragmentShaderTemplate:
				// Nothing to do in here
				break;
		}
	}

	// Error!
	return "";
}




// Quick'n'Dirty experiment using source code from http://www.asawicki.info/news_1428_finding_polygon_of_plane-aabb_intersection.html
// -> Modified in order to get rid of Lambda-expressions which would limit the number of usable compilers

// OutVD > 0 means ray is back-facing the plane
// returns false if there is no intersection because ray is perpendicular to plane
bool ray_to_plane(const Vector3 &RayOrig, const Vector3 &RayDir, const Plane &Plane, float *OutT, float *OutVD)
{
    *OutVD = Plane.a * RayDir.x + Plane.b * RayDir.y + Plane.c * RayDir.z;
    if (*OutVD == 0.0f)
        return false;
    *OutT = - (Plane.a * RayOrig.x + Plane.b * RayOrig.y + Plane.c * RayOrig.z + Plane.d) / *OutVD;
    return true;
}
// Maximum out_point_count == 6, so out_points must point to 6-element array.
// out_point_count == 0 mean no intersection.
// out_points are not sorted.
void calc_plane_aabb_intersection_points(const Plane &plane,
    const Vector3 &aabb_min, const Vector3 &aabb_max,
    Vector3 *out_points, unsigned &out_point_count)
{
    out_point_count = 0;
    float vd, t;

    // Test edges along X axis, pointing right.
    Vector3 dir = Vector3(aabb_max.x - aabb_min.x, 0.f, 0.f);
    Vector3 orig = aabb_min;
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_min.x, aabb_max.y, aabb_min.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_min.x, aabb_min.y, aabb_max.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_min.x, aabb_max.y, aabb_max.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;

    // Test edges along Y axis, pointing up.
    dir = Vector3(0.f, aabb_max.y - aabb_min.y, 0.f);
    orig = Vector3(aabb_min.x, aabb_min.y, aabb_min.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_max.x, aabb_min.y, aabb_min.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_min.x, aabb_min.y, aabb_max.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_max.x, aabb_min.y, aabb_max.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;

    // Test edges along Z axis, pointing forward.
    dir = Vector3(0.f, 0.f, aabb_max.z - aabb_min.z);
    orig = Vector3(aabb_min.x, aabb_min.y, aabb_min.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_max.x, aabb_min.y, aabb_min.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_min.x, aabb_max.y, aabb_min.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
    orig = Vector3(aabb_max.x, aabb_max.y, aabb_min.z);
    if (ray_to_plane(orig, dir, plane, &t, &vd) && t >= 0.f && t <= 1.f)
        out_points[out_point_count++] = orig + dir * t;
}


//[-------------------------------------------------------]
//[ Local classes                                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Implements a sort compare class for Vector3 objects usable with std::sort
*/
class pl_vector3_compare {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		pl_vector3_compare(const Vector3 plane_normal, const Vector3 origin) :
			m_cPlane_normal(plane_normal),
			m_cOrigin(origin)
		{
		}

		bool operator() (const Vector3 &lhs, const Vector3 &rhs)
		{
			Vector3 v;
			v.CrossProduct((lhs - m_cOrigin), (rhs - m_cOrigin));
			return (v.DotProduct(m_cPlane_normal) < 0);
		}


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		pl_vector3_compare &operator =(const pl_vector3_compare &cSource)
		{
			// No implementation because the copy operator is never used
			return *this;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Vector3 m_cPlane_normal;
		const Vector3 m_cOrigin;


};

void sort_points(Vector3 *points, unsigned point_count, const Plane &plane)
{
    if (point_count == 0) return;

    const Vector3 plane_normal = Vector3(plane.a, plane.b, plane.c);
    const Vector3 origin = points[0];

	pl_vector3_compare cCompareObj(plane_normal, origin);
	std::sort(points, points + point_count, cCompareObj);
}
void ShaderFunctionRaySetupColorCube::UpdateNearCap(const PLMath::Plane &cPlane)
{
	const Vector3 vMin(0.0f, 0.0f, 0.0f);
	const Vector3 vMax(1.0f, 1.0f, 1.0f);
	
	Vector3 vPolygonPoints[6];
	uint32 nNumOfPolygonPoints = 0;

	calc_plane_aabb_intersection_points(cPlane, vMin, vMax, vPolygonPoints, nNumOfPolygonPoints);
	if (nNumOfPolygonPoints) {
		sort_points(vPolygonPoints, nNumOfPolygonPoints, cPlane);
	}
}




void ShaderFunctionRaySetupColorCube::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get used renderer instance
	Renderer &cRenderer = cProgram.GetRenderer();

	// Update the front texture of the color cube
	UpdateFrontTexture(cProgram, cCullQuery, cVisNode, cSRPVolume);

	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return;	// Early escape, there's no sense in continuing

	// Get the volume resource
	PLVolume::Volume *pVolume = pSNVolume->GetVolume();
	if (!pVolume)
		return;	// Early escape, there's no sense in continuing

	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Set object space to clip space matrix
	cProgramWrapper.Set("ObjectSpaceToClipSpaceMatrix", cVisNode.GetWorldViewProjectionMatrix());

	{ // Front texture
		const int nTextureUnit = cProgramWrapper.Set("FrontTexture", m_pColorTargetFront->GetTextureBuffer());
		if (nTextureUnit >= 0) {
			// Setup texture addressing by using clamp
			// -> Clamp: Last valid value is reused for out-of-bound access
			// -> "stretched color" instead of color being set to border color which is black by default
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);

			// No need to perform any texture filtering setup in here
			// -> Within the shader we're performing an unfiltered texture lookup in our sampler by using the texel fetch instruction
		}
	}

	// Set step size
	cProgramWrapper.Set("StepSize", pVolume->GetStepSize(pSNVolume->VolumeTextureLOD.Get() + cSRPVolume.GetGlobalVolumeTextureLOD(), pSNVolume->SampleRateFactor.Get()*cSRPVolume.GetGlobalSampleRateFactor()));
}

/**
*  @brief
*    Updates the front texture of the color cube
*/
void ShaderFunctionRaySetupColorCube::UpdateFrontTexture(const Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, const SRPVolume &cSRPVolume)
{
	// Get our proxy cube mesh (at this point it must already exist)
	Mesh *pMesh = cSRPVolume.m_pMeshHandler->GetMesh();
	if (!pMesh)
		return;	// Early escape, there's no sense in continuing

	// Get the mesh LOD level to use
	const MeshLODLevel *pLODLevel = pMesh->GetLODLevel(0);
	if (!pLODLevel || !pLODLevel->GetIndexBuffer())
		return;	// Early escape, there's no sense in continuing

	// Get the vertex buffer of the mesh handler
	VertexBuffer *pVertexBuffer = cSRPVolume.m_pMeshHandler->GetVertexBuffer();
	if (!pVertexBuffer)
		return;	// Early escape, there's no sense in continuing

	// Get used renderer instance
	Renderer &cRenderer = cProgram.GetRenderer();

	// Get the viewport rectangle size
	const Vector2i vViewportSize(static_cast<int>(cRenderer.GetViewport().GetWidth()), static_cast<int>(cRenderer.GetViewport().GetHeight()));

	// Do we need to recreate the color render target?
	if (m_pColorTargetFront && vViewportSize != m_pColorTargetFront->GetSize()) {
		delete m_pColorTargetFront;
		m_pColorTargetFront = nullptr;
	}

	// Create the front color render target
	// -> Lookout! Must be floating-point (R16G16B16A16F), not e.g. R8G8B8... else we will see that the available precision is not enough to hold the positions...
	if (!m_pColorTargetFront)
		m_pColorTargetFront = cRenderer.CreateSurfaceTextureBuffer2D(vViewportSize, TextureBuffer::R16G16B16A16F, SurfaceTextureBuffer::Depth|SurfaceTextureBuffer::NoMultisampleAntialiasing);

	// Backup the currently set render target
	Surface *pSurfaceBackup = cRenderer.GetRenderTarget();

	// Set the new render target
	if (cRenderer.SetRenderTarget(m_pColorTargetFront)) {
		// Get the GPU program for rendering the front side of the color cube
		// -> This shader function instance may be reused across multiple shader compositions using different shader languages,
		//    in here the shader language doesn't really matter, so we don't care if there's a mix up across multiple shader compositions
		Program *pProgramFront = GetProgramFront(cRenderer, cProgram.GetShaderLanguage());

		// Backup the currently used GPU program
		Program *pProgramBackup = cRenderer.GetProgram();

		// Set the new GPU program
		if (cRenderer.SetProgram(pProgramFront)) {
			// Clear the content of the current render target
			cRenderer.Clear();

			// Get simplified GPU program wrapper interface
			ProgramWrapper &cProgramFrontWrapper = static_cast<ProgramWrapper&>(*pProgramFront);

			// Set object space to clip space matrix
			cProgramFrontWrapper.Set("ObjectSpaceToClipSpaceMatrix", cVisNode.GetWorldViewProjectionMatrix());

			{ // Draw the front side of the color cube
				// Get and use the index buffer of the mesh LOD level
				cRenderer.SetIndexBuffer(pLODLevel->GetIndexBuffer());

				// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
				cProgramFrontWrapper.Set("VertexPosition", pVertexBuffer, VertexBuffer::Position);
				cProgramFrontWrapper.Set("VertexTexCoord", pVertexBuffer, VertexBuffer::TexCoord);

				// Loop through all geometries of the mesh
				cRenderer.SetRenderState(RenderState::CullMode, Cull::CCW);
				const Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
				for (uint32 nGeo=0; nGeo<lstGeometries.GetNumOfElements(); nGeo++) {
					// Is this geometry active?
					const Geometry &cGeometry = lstGeometries[nGeo];
					if (cGeometry.IsActive()) {
						// Draw the geometry
						cRenderer.DrawIndexedPrimitives(
							cGeometry.GetPrimitiveType(),
							0,
							pVertexBuffer->GetNumOfElements()-1,
							cGeometry.GetStartIndex(),
							cGeometry.GetIndexSize()
						);
					}
				}
			}

			// Draw the near-cap (in case one's required)
			if (!(cSRPVolume.GetFlags() & SRPVolume::NoNearCap)) {
				// Get the camera near plane
				const Plane *pNearPlane = cCullQuery.GetViewFrustum()[PlaneSet::VPNear];
				if (pNearPlane) {
					Plane cPlane = (*pNearPlane);																		// Use given near plane
					cPlane.d -= 0.001f;																					// Move it a little bit forward, else our cap polygon will be clipped, again...
					cPlane = (cPlane*cCullQuery.GetVisContainer().GetWorldMatrix())*cVisNode.GetInverseWorldMatrix();	// Transform the camera near plane into volume object space

					const Vector3 vMin(0.0f, 0.0f, 0.0f);
					const Vector3 vMax(1.0f, 1.0f, 1.0f);
					Vector3 vPolygonPoints[6];
					uint32 nNumOfPolygonPoints = 0;
					calc_plane_aabb_intersection_points(cPlane, vMin, vMax, vPolygonPoints, nNumOfPolygonPoints);
					if (nNumOfPolygonPoints) {
						sort_points(vPolygonPoints, nNumOfPolygonPoints, cPlane);


						if (!m_pCapVertexBuffer) {
							m_pCapVertexBuffer = cRenderer.CreateVertexBuffer();
							m_pCapVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
							m_pCapVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float3);
							m_pCapVertexBuffer->Allocate(6, Usage::Dynamic);
						}

						if (m_pCapVertexBuffer->Lock(Lock::WriteOnly)) { // Setup vertices
							for (uint32 i=0; i<6; i++) {
								// Position
								float *pfVertices = static_cast<float*>(m_pCapVertexBuffer->GetData(i, VertexBuffer::Position));
								pfVertices[Vector3::X] = vPolygonPoints[i].x;
								pfVertices[Vector3::Y] = vPolygonPoints[i].y;
								pfVertices[Vector3::Z] = vPolygonPoints[i].z;

								// Texture coordinate
								pfVertices = static_cast<float*>(m_pCapVertexBuffer->GetData(i, VertexBuffer::TexCoord));
								pfVertices[Vector3::X] = vPolygonPoints[i].x;
								pfVertices[Vector3::Y] = vPolygonPoints[i].y;
								pfVertices[Vector3::Z] = vPolygonPoints[i].z;
							}

							// Unlock the vertex buffer
							m_pCapVertexBuffer->Unlock();
						}

						// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
						cProgramFrontWrapper.Set("VertexPosition", m_pCapVertexBuffer, VertexBuffer::Position);
						cProgramFrontWrapper.Set("VertexTexCoord", m_pCapVertexBuffer, VertexBuffer::TexCoord);

						// Draw
						cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
						cRenderer.DrawPrimitives(Primitive::TriangleFan, 0, nNumOfPolygonPoints);
					}

				}
			}

			// Restore the previously set GPU program
			cRenderer.SetProgram(pProgramBackup);
		}

		// Restore the previously set render target
		cRenderer.SetRenderTarget(pSurfaceBackup);
	}
}

/**
*  @brief
*    Returns the GPU program for rendering the front side of the color cube
*/
Program *ShaderFunctionRaySetupColorCube::GetProgramFront(Renderer &cRenderer, const String &sShaderLanguage)
{
	// Create the GPU program for rendering the front side of the color cube if it doesn't exist, yet
	if (!m_pProgramFront) {
		// Get the shader language to use (for example "GLSL" or "Cg")
		ShaderLanguage *pShaderLanguage = cRenderer.GetShaderLanguage(sShaderLanguage);
		if (pShaderLanguage) {
			// Shader source code
			String sVertexShaderSourceCode;
			String sFragmentShaderSourceCode;
			if (sShaderLanguage == ShaderFunction::GLSL) {
				#include "ColorCubeFront_GLSL.h"
				if (cRenderer.GetAPI() == "OpenGL ES 2.0") {
					// Get shader source codes
					sVertexShaderSourceCode   = "#version 100\n" + sSourceCode_Vertex;
					sFragmentShaderSourceCode = "#version 100\n" + sSourceCode_Fragment;
				} else {
					// Remove precision qualifiers so that we're able to use 110 (OpenGL 2.0 shaders) instead of 130 (OpenGL 3.0 shaders,
					// with this version we can keep the precision qualifiers) so that this shader requirements are as low as possible
					sVertexShaderSourceCode   = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sSourceCode_Vertex);
					sFragmentShaderSourceCode = "#version 110\n" + Shader::RemovePrecisionQualifiersFromGLSL(sSourceCode_Fragment);
				}
			} else if (sShaderLanguage == ShaderFunction::Cg) {
				#include "ColorCubeFront_Cg.h"
				sVertexShaderSourceCode   = sSourceCode_Vertex;
				sFragmentShaderSourceCode = sSourceCode_Fragment;
			}

			// Create a vertex shader instance
			VertexShader *pVertexShader = pShaderLanguage->CreateVertexShader(sVertexShaderSourceCode, "arbvp1");
			if (pVertexShader) {
				// Create a fragment shader instance
				FragmentShader *pFragmentShader = pShaderLanguage->CreateFragmentShader(sFragmentShaderSourceCode, "arbfp1");
				if (pFragmentShader) {
					// Create a program instance and assign the created vertex and fragment shaders to it
					m_pProgramFront = pShaderLanguage->CreateProgram(pVertexShader, pFragmentShader);
					if (!m_pProgramFront) {
						// Cleanup on error
						delete pVertexShader;
						delete pFragmentShader;
					}
				} else {
					// Cleanup on error
					delete pVertexShader;
				}
			}
		}
	}

	// Return the GPU program for rendering the front side of the color cube
	return m_pProgramFront;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
