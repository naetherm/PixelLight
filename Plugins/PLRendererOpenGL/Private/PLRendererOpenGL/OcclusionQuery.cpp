/*********************************************************\
 *  File: OcclusionQuery.cpp                             *
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
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/OcclusionQuery.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
OcclusionQuery::~OcclusionQuery()
{
	// Destroy OpenGL occlusion query
	if (m_nQuery) {
		if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_occlusion_query())
			glDeleteQueriesARB(1, &m_nQuery);
		else
			glDeleteOcclusionQueriesNV(1, &m_nQuery);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
OcclusionQuery::OcclusionQuery(PLRenderer::Renderer &cRenderer) : PLRenderer::OcclusionQuery(cRenderer),
	m_nQuery(0),
	m_nSkipRate(0),
	m_nSkipCounter(0),
	m_nMinFragments(0),
	m_nPixelCount(100000)
{
	// Check whether occlusion query is supported
	if (cRenderer.GetCapabilities().bOcclusionQuery) {
		// Create OpenGL occlusion query
		if (static_cast<Renderer&>(cRenderer).GetContext().GetExtensions().IsGL_ARB_occlusion_query())
			glGenQueriesARB(1, &m_nQuery);
		else
			glGenOcclusionQueriesNV(1, &m_nQuery);
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::OcclusionQuery functions   ]
//[-------------------------------------------------------]
bool OcclusionQuery::BeginOcclusionQuery()
{
	// Check whether occlusion query is supported
	if (GetRenderer().GetCapabilities().bOcclusionQuery) {
		// Update the skip counter
		m_nSkipCounter++;
		if (m_nSkipCounter >= m_nSkipRate)
			m_nSkipCounter = 0;

		// New or none visible objects must always be tested but visible objects can be skipped
		if (!m_nSkipCounter || m_nPixelCount <= m_nMinFragments) {
			if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_occlusion_query())
				glBeginQueryARB(GL_SAMPLES_PASSED_ARB, m_nQuery);
			else
				glBeginOcclusionQueryNV(m_nQuery);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

void OcclusionQuery::EndOcclusionQuery()
{
	// Check whether occlusion query is supported
	if (GetRenderer().GetCapabilities().bOcclusionQuery) {
		// New or none visible objects must always be tested but visible objects can be skipped
		if (!m_nSkipCounter || m_nPixelCount <= m_nMinFragments) {
			if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_occlusion_query())
				glEndQueryARB(GL_SAMPLES_PASSED_ARB);
			else
				glEndOcclusionQueryNV();
		}
	}
}

bool OcclusionQuery::PullOcclusionQuery(uint32 *pnNumOfFragments)
{
	// Check whether occlusion query is supported
	if (GetRenderer().GetCapabilities().bOcclusionQuery) {
		// Check whether the queried data is already available
		if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_occlusion_query()) {
			GLint nAvailable;
			glGetQueryObjectivARB(m_nQuery, GL_QUERY_RESULT_AVAILABLE_ARB, &nAvailable);
			if (!nAvailable)
				return false; // Error!

			// Query
			glGetQueryObjectuivARB(m_nQuery, GL_QUERY_RESULT_ARB, &m_nPixelCount);
			if (pnNumOfFragments)
				*pnNumOfFragments = m_nPixelCount;
		} else {
			GLint nAvailable;
			glGetOcclusionQueryivNV(m_nQuery, GL_PIXEL_COUNT_AVAILABLE_NV, &nAvailable);
			if (!nAvailable)
				return false; // Error!

			// Query
			glGetOcclusionQueryuivNV(m_nQuery, GL_PIXEL_COUNT_NV, &m_nPixelCount);
			if (pnNumOfFragments)
				*pnNumOfFragments = m_nPixelCount;
		}
	} else {
		// Error, occlusion query is not supported!
		m_nPixelCount = 100000;
	}

	// Done
	return true;
}

uint32 OcclusionQuery::GetLastQuerysPixelCount() const
{
	return m_nPixelCount;
}

uint32 OcclusionQuery::GetSkipRate() const
{
	return m_nSkipRate;
}

void OcclusionQuery::SetSkipRate(uint32 nRate)
{
	m_nSkipRate = nRate;
}

uint32 OcclusionQuery::GetMinFragments() const
{
	return m_nMinFragments;
}

void OcclusionQuery::SetMinFragments(uint32 nMinFragments)
{
	m_nMinFragments = nMinFragments;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void OcclusionQuery::BackupDeviceData(uint8 **ppBackup)
{
	// Not used :)
	*ppBackup = nullptr;

	// Destroy OpenGL occlusion query
	if (m_nQuery) {
		if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_occlusion_query())
			glDeleteQueriesARB(1, &m_nQuery);
		else
			glDeleteOcclusionQueriesNV(1, &m_nQuery);
		m_nQuery = 0;
	}
}

void OcclusionQuery::RestoreDeviceData(uint8 **ppBackup)
{
	// Check whether occlusion query is supported
	if (GetRenderer().GetCapabilities().bOcclusionQuery) {
		// Create OpenGL occlusion query
		if (static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions().IsGL_ARB_occlusion_query())
			glGenQueriesARB(1, &m_nQuery);
		else
			glGenOcclusionQueriesNV(1, &m_nQuery);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
