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
#include "PLRendererNull/OcclusionQuery.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
OcclusionQuery::~OcclusionQuery()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
OcclusionQuery::OcclusionQuery(PLRenderer::Renderer &cRenderer) : PLRenderer::OcclusionQuery(cRenderer),
	m_nSkipRate(0),
	m_nMinFragments(0),
	m_nPixelCount(0)
{
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::OcclusionQuery functions   ]
//[-------------------------------------------------------]
bool OcclusionQuery::BeginOcclusionQuery()
{
	// Error!
	return false;
}

void OcclusionQuery::EndOcclusionQuery()
{
}

bool OcclusionQuery::PullOcclusionQuery(uint32 *pnNumOfFragments)
{
	m_nPixelCount = 0;
	if (pnNumOfFragments)
		*pnNumOfFragments = m_nPixelCount;

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
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull
