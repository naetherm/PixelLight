/*********************************************************\
 *  File: Graph.h                                        *
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


#ifndef __PLMATH_GRAPH_GRAPH_H__
#define __PLMATH_GRAPH_GRAPH_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Bitset.h>
#include <PLCore/Container/Resource.h>
#include <PLCore/Container/BinaryHeap.h>
#include <PLCore/Container/ElementManager.h>
#include "PLMath/Graph/GraphNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GraphPath;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Graph class (directed)
*/
class Graph : public PLCore::Resource<Graph>, public PLCore::ElementManager<GraphNode> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Resource name to set
		*  @param[in] pManager
		*    Resource manager using this resource, can be a null pointer
		*/
		PLMATH_API Graph(const PLCore::String &sName = "", PLCore::ResourceManager<Graph> *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLMATH_API virtual ~Graph();

		/**
		*  @brief
		*    Returns the shortest path from a node to another
		*
		*  @param[in] nStartNode
		*    Start node
		*  @param[in] nEndNode
		*    End node
		*
		*  @return
		*    The shortest path from the start node to the end node,
		*    a null pointer if no such path exist. (you have do delete this path by yourself!)
		*
		*  @note
		*    - Dijkstra's single source shortest path algorithm is used
		*/
		PLMATH_API GraphPath *FindShortestPath(PLCore::uint32 nStartNode, PLCore::uint32 nEndNode);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Temp data used during path finding (so we do not reallocate it each time...)
		PLCore::Array<float>				  m_lstNodeDistance;	/**< Shortest distance of each node to start node */
		PLCore::Array<GraphNode*>			  m_lstPreviousNode;	/**< Previous node, if a null pointer, not processed yet */
		PLCore::Bitset						  m_lstTouched;			/**< Holds whether nodes are already touched or not */
		PLCore::Bitset						  m_lstProcessed;		/**< Holds whether nodes are already processed or not */
		PLCore::Array<GraphNode*>			  m_lstNodes;			/**< Temp nodes array */
		PLCore::BinaryHeap<float, GraphNode*> m_mapToProcess;		/**< Current processed nodes */


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual GraphNode *CreateElement(const PLCore::String &sName) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Resource functions             ]
	//[-------------------------------------------------------]
	public:
		PLMATH_API virtual Graph &operator =(const Graph &cSource);


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLMATH_API virtual bool Unload() override;
		PLMATH_API virtual PLCore::String GetLoadableTypeName() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_GRAPH_GRAPH_H__
