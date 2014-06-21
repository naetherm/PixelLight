/*********************************************************\
 *  File: Weight.h                                       *
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


#ifndef __PLMESH_WEIGHT_H__
#define __PLMESH_WEIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Weight class
*/
class Weight {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API Weight();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API ~Weight();

		/**
		*  @brief
		*    Get the weight joint number
		*
		*  @return
		*    Weight joint number, < 0 on error
		*/
		PLMESH_API int GetJoint() const;

		/**
		*  @brief
		*    Sets the weight joint number
		*
		*  @param[in] nJoint
		*    Weight joint number
		*/
		PLMESH_API void SetJoint(int nJoint = -1);

		/**
		*  @brief
		*    Get the weight bias
		*
		*  @return
		*    Weight bias
		*
		*  @remarks
		*    Scale factor for the influence. Usually the weights for all influences for a single
		*    vertex add up to one.
		*/
		PLMESH_API float GetBias() const;

		/**
		*  @brief
		*    Sets the weight bias
		*
		*  @param[in] fBias
		*    Weight bias
		*
		*  @see
		*    - GetBias()
		*/
		PLMESH_API void SetBias(float fBias = 0.0f);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This weight
		*/
		PLMESH_API Weight &operator =(const Weight &cSource);

		/**
		*  @brief
		*    Compares two weights
		*
		*  @param[in] cWeight
		*    Weight to compare with
		*
		*  @return
		*    'true' if both weights are equal, else 'false'
		*/
		PLMESH_API bool operator ==(const Weight &cWeight) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int   m_nJoint;	/**< Joint this weight is for */
		float m_fBias;	/**< Bias factor */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_WEIGHT_H__
