/*********************************************************\
 *  File: OcclusionQuery.h                               *
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


#ifndef __PLRENDERERNULL_OCCLUSION_QUERY_H__
#define __PLRENDERERNULL_OCCLUSION_QUERY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/OcclusionQuery.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererNull {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null occlusion query
*/
class OcclusionQuery : public PLRenderer::OcclusionQuery {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Renderer;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OcclusionQuery();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*/
		OcclusionQuery(PLRenderer::Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32 m_nSkipRate;		/**< Skip rate */
		PLCore::uint32 m_nMinFragments;	/**< Minimum number of visible fragments */
		PLCore::uint32 m_nPixelCount;	/**< Last pixel count */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::OcclusionQuery functions   ]
	//[-------------------------------------------------------]
	public:
		virtual bool BeginOcclusionQuery() override;
		virtual void EndOcclusionQuery() override;
		virtual bool PullOcclusionQuery(PLCore::uint32 *pnNumOfFragments = nullptr) override;
		virtual PLCore::uint32 GetLastQuerysPixelCount() const override;
		virtual PLCore::uint32 GetSkipRate() const override;
		virtual void SetSkipRate(PLCore::uint32 nRate = 0) override;
		virtual PLCore::uint32 GetMinFragments() const override;
		virtual void SetMinFragments(PLCore::uint32 nMinFragments = 0) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererNull


#endif // __PLRENDERERNULL_OCCLUSION_QUERY_H__
