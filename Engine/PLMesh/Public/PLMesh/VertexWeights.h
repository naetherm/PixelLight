/*********************************************************\
 *  File: VertexWeights.h                                *
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


#ifndef __PLMESH_VERTEXWEIGHTS_H__
#define __PLMESH_VERTEXWEIGHTS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
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
*    Vertex weights class for managing a list of weights per vertex
*
*  @note
*    - A vertex can have multiple weights
*/
class VertexWeights {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API VertexWeights();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API ~VertexWeights();

		/**
		*  @brief
		*    Get the vertex weight indices array
		*
		*  @return
		*    Vertex weight indices array
		*
		*  @note
		*    - This array holds the vertex weight indices per vertex
		*      (see Mesh::GetVertexWeights())
		*/
		PLMESH_API PLCore::Array<PLCore::uint32> &GetWeights();
		PLMESH_API const PLCore::Array<PLCore::uint32> &GetWeights() const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This vertex weights
		*/
		PLMESH_API VertexWeights &operator =(const VertexWeights &cSource);

		/**
		*  @brief
		*    Compares two vertex weights
		*
		*  @param[in] cVertexWeights
		*    Vertex weights to compare with
		*
		*  @return
		*    'true' if both vertex weights are equal, else 'false'
		*/
		PLMESH_API bool operator ==(const VertexWeights &cVertexWeights) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<PLCore::uint32> m_lstWeights;	/**< Vertex weights */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_VERTEXWEIGHTS_H__
