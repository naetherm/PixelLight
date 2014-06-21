/*********************************************************\
 *  File: Graph.cpp                                      *
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
#include "PLMath/Graph/GraphPath.h"
#include "PLMath/Graph/Graph.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Graph::Graph(const String &sName, ResourceManager<Graph> *pManager) :
	Resource<Graph>(sName, pManager)
{
}

/**
*  @brief
*    Destructor
*/
Graph::~Graph()
{
}

/**
*  @brief
*    Returns the shortest path from a node to another
*/
GraphPath *Graph::FindShortestPath(uint32 nStartNode, uint32 nEndNode)
{
	// Get the start node
	GraphNode *pStartNode = GetByIndex(nStartNode);
	if (pStartNode) {
		// Get the end node
		GraphNode *pEndNode = GetByIndex(nEndNode);
		if (pEndNode) {
			// Total number of graph nodes and the priority queue
			uint32 nNumOfNodes = m_lstElements.GetNumOfElements();

			// Initialize temp data
			m_lstNodeDistance.Resize(nNumOfNodes);
			m_lstPreviousNode.Resize(nNumOfNodes);
			m_lstProcessed.Resize(nNumOfNodes);
			m_lstTouched.ClearAll();
			m_lstProcessed.ClearAll();

			// Reset add the start node to the priority queue
			m_mapToProcess.Reset();
			m_mapToProcess.Add(0.0, pStartNode);
			m_lstNodeDistance[nStartNode] = 0.0f;
			m_lstPreviousNode[nStartNode] = nullptr;
			m_lstTouched.Set(nStartNode);

			// Loop until the priority queue is empty
			while (!m_mapToProcess.IsEmpty()) {
				// Get the node with the shortest distance and mark it as processed
				float fDistance;
				GraphNode *pNode;
				m_mapToProcess.ExtractTop(&pNode, &fDistance);
				m_lstProcessed.Set(pNode->GetID());

				// Loop through the neighbours of this node
				for (uint32 nNeighbour=0; nNeighbour<pNode->GetNumOfNeighbours(); nNeighbour++) {
					GraphNode *pNeighbour   = pNode->GetNeighbour(nNeighbour);
					uint32     nNeighbourID = pNeighbour->GetID();

					// Is this node already processed?
					if (!m_lstProcessed.IsSet(nNeighbourID)) {
						// Calculate the new distance
						float fNewDistance = pNode->GetNeighbourDistance(nNeighbourID)+fDistance;

						// Was this node already touched?
						if (m_lstTouched.IsSet(nNeighbourID)) {
							// Maybe we are in luck and the old distance is shorter as the current...
							if (fNewDistance < m_lstNodeDistance[nNeighbourID]) {
								// D'OH!, we have to update this node information :(
								m_lstNodeDistance[nNeighbourID] = fNewDistance;
								m_lstPreviousNode[nNeighbourID] = pNode;

								// [TODO] Change key
								// Update the priority queue
								m_mapToProcess.Add(fNewDistance, pNeighbour);
							}
						} else {
							// Nope, this is the first touch :)
							m_lstNodeDistance[nNeighbourID] = fNewDistance;
							m_lstPreviousNode[nNeighbourID] = pNode;
							m_mapToProcess.Add(fNewDistance, pNeighbour);
							m_lstTouched.Set(nNeighbourID);
						}
					}
				}
			}

			// Now build the path with the collected information
			GraphNode *pNode = m_lstPreviousNode[nEndNode];
			if (pNode) {
				m_lstNodes.Resize(nNumOfNodes);
				GraphPath *pPath = new GraphPath();
				uint32 nNumOfPathNodes = 0;
				while (pNode) {
					m_lstNodes[nNumOfPathNodes] = pNode;
					pNode = m_lstPreviousNode[pNode->GetID()];
					nNumOfPathNodes++;
				}

				// And add the nodes to the path in correct order
				for (int i=nNumOfPathNodes-1; i>=0; i--)
					pPath->AddNode(*m_lstNodes[i]);

				// Return the created path
				return pPath;
			}
		}
	}

	// Error, there's no path from 'start' to 'end'!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
GraphNode *Graph::CreateElement(const String &sName)
{
	return new GraphNode(sName, this);
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Resource functions             ]
//[-------------------------------------------------------]
Graph &Graph::operator =(const Graph &cSource)
{
	// Unload the old graph
	Unload();

	// Call base function
	*static_cast<Resource<Graph>*>(this)           = cSource;
	*static_cast<ElementManager<GraphNode>*>(this) = cSource;

	// Done
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool Graph::Unload()
{
	// Clear the graph
	Clear();

	// [TODO]
	// Clear temp data
	// m_lstPreviousNode.Clear();

	// Call base implementation
	return Resource<Graph>::Unload();
}

String Graph::GetLoadableTypeName() const
{
	static const String sString = "Graph";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
