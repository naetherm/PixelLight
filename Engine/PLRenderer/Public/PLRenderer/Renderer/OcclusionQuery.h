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


#ifndef __PLRENDERER_OCCLUSION_QUERY_H__
#define __PLRENDERER_OCCLUSION_QUERY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Resource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer occlusion query
*
*  @verbatim
*    Usage example:
*    Create one or more OcclusionQuery object one per outstanding
*    query or one per tested object.
*
*    OcclusionQuery *pOcclusionQuery = cRenderer.CreateOcclusionQuery();
*    // In the rendering loop:
*    // Draw all occludes
*    pOcclusionQuery->BeginOcclusionQuery();
*    // Draw the polygons to be tested
*    pOcclusionQuery->EndOcclusionQuery();
*    // Do something useful to give the GPU some time to process the stuff
*    // Results must be pulled using:
*    PLCore::uint32 nNumberOfVisibleFragments;
*    if (!pOcclusionQuery->PullOcclusionQuery(&nNumberOfVisibleFragments)) {
*      if (nNumberOfVisibleFragments >= pOcclusionQuery->GetMinFragments()) {
*        // Our object is visible (or visible enough ;-)
*      }
*    } else {
*       // The result isn't available yet! We still have to wait for it...
*    }
*  @endverbatim
*/
class OcclusionQuery : public Resource {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~OcclusionQuery();


	//[-------------------------------------------------------]
	//[ Public virtual OcclusionQuery functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Starts the hardware occlusion query
		*
		*  @return
		*    'true' if all went fine and the occlusion query was started, else
		*    'false' (maybe occlusion query is not supported or skip rate is active)
		*/
		virtual bool BeginOcclusionQuery() = 0;

		/**
		*  @brief
		*    Ends the hardware occlusion query
		*/
		virtual void EndOcclusionQuery() = 0;

		/**
		*  @brief
		*    Pulls the hardware occlusion query too see if there is a result
		*
		*  @param[out] pnNumOfFragments
		*    If not a null pointer it will receive the number of visible fragments
		*
		*  @return
		*    'true' if all went fine, else 'false' (the queried data isn't available yet)
		*
		*  @note
		*    - Hardware occlusion is an asynchronous process the result may take a frame or so.
		*      one idea is to test pass 1 and if not visible skip pass 2. Also note that objects which are
		*      not visible must be tested every frame. Visible objects don't need testing every frame.
		*      Testing non visible objects can be don unlit, no texture with low LOD object - this
		*      is also the case when using for instance only a bounding box for a complex mesh.
		*    - After BeginOcclusionQuery()/EndOcclusionQuery() you should do some other stuff before
		*      asking for the result using this function to give the GPU some time to process the
		*      the stuff. If this function returns 'true' then you still have to wait for the result!
		*
		*  @see
		*    - GetMinFragments()
		*/
		virtual bool PullOcclusionQuery(PLCore::uint32 *pnNumOfFragments = nullptr) = 0;

		/**
		*  @brief
		*    Returns the number of the last occlusion query visible fragments
		*
		*  @return
		*    The fragment count from the last test
		*/
		virtual PLCore::uint32 GetLastQuerysPixelCount() const = 0;

		/**
		*  @brief
		*    Returns the skip rate
		*
		*  @return
		*    Current skip rate
		*
		*  @remarks
		*    This function allows you to set how often the hardware occlusion really are sent to the driver
		*    if you set it to 0 every hw occlusion test is actually made. If you set it to 1 only the half
		*    of your queries are sent for all visible objects. 2 will result in 25% of all queries to actually be sent
		*    which will result in a better performance. New and none visible objects will be tested all the time.
		*    This class can keep track on visible and none visible objects for you.
		*/
		virtual PLCore::uint32 GetSkipRate() const = 0;

		/**
		*  @brief
		*    Sets the skip rate
		*
		*  @param[in] nRate
		*    New skip rate
		*
		*  @see
		*    - GetSkipRate()
		*/
		virtual void SetSkipRate(PLCore::uint32 nRate = 0) = 0;

		/**
		*  @brief
		*    Gets the minimum number of visible fragments
		*
		*  @return
		*    Minimum number of visible fragments
		*
		*  @remarks
		*    This setting indicates when something can be seen as invisible.
		*    For instance often you can see something as invisible if less then
		*    e.g. 10 fragments are visible without a visual quality impact.
		*/
		virtual PLCore::uint32 GetMinFragments() const = 0;

		/**
		*  @brief
		*    Sets the minimum number of visible fragments
		*
		*  @param[in] nMinFragments
		*    Minimum number of visible fragments
		*
		*  @see
		*    - GetMinFragments()
		*/
		virtual void SetMinFragments(PLCore::uint32 nMinFragments = 0) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    The owner renderer
		*/
		PLRENDERER_API OcclusionQuery(Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		OcclusionQuery(const OcclusionQuery &cSource);

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
		OcclusionQuery &operator =(const OcclusionQuery &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_OCCLUSION_QUERY_H__
