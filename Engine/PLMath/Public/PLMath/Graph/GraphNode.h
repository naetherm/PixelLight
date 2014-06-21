/*********************************************************\
 *  File: GraphNode.h                                    *
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


#ifndef __PLMATH_GRAPH_NODE_H__
#define __PLMATH_GRAPH_NODE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Element.h>
#include "PLMath/Vector3.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Graph node class
*/
class GraphNode : public PLCore::Element<GraphNode> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Main                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] pManager
		*    Element manager using this element, can be a null pointer
		*/
		PLMATH_API GraphNode(const PLCore::String &sName = "", PLCore::ElementManager<GraphNode> *pManager = nullptr);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSource
		*    Node to copy from
		*/
		PLMATH_API GraphNode(GraphNode &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLMATH_API virtual ~GraphNode();

		//[-------------------------------------------------------]
		//[ Neighbours                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the number of neighbours
		*
		*  @return
		*    Number of neighbours
		*/
		PLMATH_API PLCore::uint32 GetNumOfNeighbours() const;

		/**
		*  @brief
		*    Clears all neighbour connectivity
		*/
		PLMATH_API void ClearNeighbours();

		/**
		*  @brief
		*    Returns whether a node is a neighbour of this node or not
		*
		*  @param[in] cNode
		*    Node to check
		*
		*  @return
		*    'true' if the given node is a neighbour of this node, else 'false'
		*/
		PLMATH_API bool IsNeighbour(const GraphNode &cNode) const;

		/**
		*  @brief
		*    Returns a neighbour
		*
		*  @param[in] nNeighbour
		*    Neighbour index
		*
		*  @return
		*    Requested neighbour node, a null pointer on error
		*/
		PLMATH_API const GraphNode *GetNeighbour(PLCore::uint32 nNeighbour) const;

		/**
		*  @brief
		*    Returns a neighbour
		*
		*  @param[in] nNeighbour
		*    Neighbour index
		*
		*  @return
		*    Requested neighbour node, a null pointer on error
		*/
		PLMATH_API GraphNode *GetNeighbour(PLCore::uint32 nNeighbour);

		/**
		*  @brief
		*    Returns the distance to a neighbour
		*
		*  @param[in] nNeighbour
		*    Neighbour index
		*
		*  @return
		*    Requested distance to a neighbour node, -1 on error
		*
		*  @note
		*    - By default the neighbour distance from AddNeighbour() is set, if
		*      SetPos() is called the neighbour distances are updated automatically.
		*/
		PLMATH_API float GetNeighbourDistance(PLCore::uint32 nNeighbour) const;

		/**
		*  @brief
		*    Adds a neighbour connection
		*
		*  @param[in] cNode
		*    Neighbour node to add
		*  @param[in] fDistance
		*    Distance to the neighbour node, if negative the distance is calculated automatically
		*    using the current node positions. (see GetPos())
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe this node is already a neighbour)
		*/
		PLMATH_API bool AddNeighbour(GraphNode &cNode, float fDistance = -1.0f);

		/**
		*  @brief
		*    Removes a neighbour connection
		*
		*  @param[in] cNode
		*    Neighbour node to remove
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMATH_API bool RemoveNeighbour(GraphNode &cNode);

		/**
		*  @brief
		*    Removes a neighbour connection at the given index
		*
		*  @param[in] nNeighbour
		*    Neighbour index
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLMATH_API bool RemoveNeighbourAtIndex(PLCore::uint32 nNeighbour);

		//[-------------------------------------------------------]
		//[ Position                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the node position
		*
		*  @return
		*    Node position
		*/
		PLMATH_API const Vector3 &GetPos() const;

		/**
		*  @brief
		*    Sets the node position
		*
		*  @param[in] vPos
		*    Node position
		*/
		PLMATH_API void SetPos(const Vector3 &vPos);

		/**
		*  @brief
		*    Sets the node position
		*
		*  @param[in] fX
		*    X component of the node position
		*  @param[in] fY
		*    Y component of the node position
		*  @param[in] fZ
		*    Z component of the node position
		*/
		PLMATH_API void SetPos(float fX = 0.0f, float fY = 0.0f, float fZ = 0.0f);

		/**
		*  @brief
		*    Returns the distance between two graph nodes
		*
		*  @param[in] cNode
		*    Other node
		*
		*  @return
		*    The positions (see GetPos()) are used to calculate the distance
		*/
		PLMATH_API float GetDistance(const GraphNode &cNode) const;


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal graph node neighbour structure
		*/
		struct Neighbour {
			GraphNode *pNode;		/**< Neighbour node (always valid!) */
			float	   fDistance;	/**< Distance to neighbour node */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Vector3					 m_vPos;				/**< Node position */
		PLCore::List<Neighbour*> m_lstNeighbours;		/**< Neighbour nodes */
		PLCore::List<GraphNode*> m_lstIsNeighbourFrom;	/**< This node is a neighbour from... */


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Element functions              ]
	//[-------------------------------------------------------]
	public:
		PLMATH_API virtual GraphNode &operator =(const GraphNode &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_GRAPH_NODE_H__
