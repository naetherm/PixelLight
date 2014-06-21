/*********************************************************\
 *  File: GraphPath.cpp                                  *
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
#include "PLMath/Graph/GraphNode.h"
#include "PLMath/Graph/GraphPath.h"


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
GraphPath::GraphPath(const String &sName, ResourceManager<GraphPath> *pManager) : Resource<GraphPath>(sName, pManager),
	m_pOwnerGraph(nullptr),
	m_bClosed(false)
{
}

/**
*  @brief
*    Constructor
*/
GraphPath::GraphPath(Graph *pOwnerGraph) :
	m_pOwnerGraph(pOwnerGraph),
	m_bClosed(false)
{
}

/**
*  @brief
*    Destructor
*/
GraphPath::~GraphPath()
{
}

/**
*  @brief
*    Returns the owner graph of the path
*/
Graph *GraphPath::GetOwnerGraph() const
{
	return m_pOwnerGraph;
}


//[-------------------------------------------------------]
//[ Nodes                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the number of nodes
*/
uint32 GraphPath::GetNumOfNodes() const
{
	return m_lstNodes.GetNumOfElements();
}

/**
*  @brief
*    Adds a node
*/
bool GraphPath::AddNode(GraphNode &cNode)
{
	return (m_lstNodes.Add(&cNode) != nullptr);
}

/**
*  @brief
*    Removes a node
*/
bool GraphPath::RemoveNode(uint32 nNode)
{
	// Remove node itself?
	if (!m_pOwnerGraph) {
		GraphNode *pNode = m_lstNodes[nNode];
		if (pNode)
			delete pNode;
	}

	// Remove node from list
	return m_lstNodes.RemoveAtIndex(nNode);
}

/**
*  @brief
*    Returns a node
*/
const GraphNode *GraphPath::GetNode(uint32 nID) const
{
	return m_lstNodes[nID];
}

/**
*  @brief
*    Returns a node
*/
GraphNode *GraphPath::GetNode(uint32 nID)
{
	return m_lstNodes[nID];
}


//[-------------------------------------------------------]
//[ Tools                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the length of the path
*/
float GraphPath::GetLength() const
{
	float fLength = 0.0f;

	{ // Get path length
		const GraphNode *pLastNode = nullptr;
		Iterator<GraphNode*> cIterator = m_lstNodes.GetIterator();
		while (cIterator.HasNext()) {
			const GraphNode *pNode = cIterator.Next();
			if (pLastNode)
				fLength += pNode->GetDistance(*pLastNode);
			pLastNode = pNode;
		}
	}

	// If closed add the distance from the first to the last node
	if (m_bClosed)
		fLength += m_lstNodes[0]->GetDistance(*m_lstNodes[m_lstNodes.GetNumOfElements()-1]);

	// Return the path length
	return fLength;
}

/**
*  @brief
*    Returns whether the path is closed or not
*/
bool GraphPath::IsClosed() const
{
	return m_bClosed;
}

/**
*  @brief
*    Sets if the path is closed or not
*/
void GraphPath::SetClosed(bool bClosed)
{
	m_bClosed = bClosed;
}

/**
*  @brief
*    Calculates the position at the given node index
*/
Vector3 GraphPath::GetPosByNodeIndex(float fNodeIndex, bool bLinear) const
{
	// [TODO] Refactor the implementation

	// Check bounding
	if (!m_lstNodes.GetNumOfElements())
		return Vector3::Zero; // Error!
	if (m_lstNodes.GetNumOfElements() == 1)
		return m_lstNodes[0]->GetPos();
	if (!m_bClosed) {
		// Clamp
		if (fNodeIndex <= 0.0f)
			return m_lstNodes[0]->GetPos();
		if (fNodeIndex >= m_lstNodes.GetNumOfElements()-3)
			return m_lstNodes[m_lstNodes.GetNumOfElements()-2]->GetPos();
	}

	// Wrap
	while (fNodeIndex < 0)
		fNodeIndex += m_lstNodes.GetNumOfElements();
	while (fNodeIndex > m_lstNodes.GetNumOfElements())
		fNodeIndex -= m_lstNodes.GetNumOfElements();

	// Set valid nodes
	uint32 nCurrentNode = static_cast<uint32>(fNodeIndex);
	uint32 nNextNode    = nCurrentNode+1;
	if (nNextNode > m_lstNodes.GetNumOfElements()-1)
		nNextNode = m_bClosed ? 0 : nNextNode-1;
	float fProgress = fNodeIndex-nCurrentNode;

	// Interpolate
	if (bLinear) {
		return (m_lstNodes[nCurrentNode])->GetPos()+
			   (m_lstNodes[nNextNode]->GetPos()-m_lstNodes[nCurrentNode]->GetPos())*fProgress;
	} else { // Catmull rom curve
		const Vector3 &vP1 = m_lstNodes[nCurrentNode]->GetPos();
		const Vector3 &vP2 = m_lstNodes[nNextNode]->GetPos();
		nNextNode = nNextNode+1;
		if (nNextNode > m_lstNodes.GetNumOfElements()-1)
			nNextNode = m_bClosed ? 0 : nNextNode-1;
		const Vector3 &vP3 = m_lstNodes[nNextNode]->GetPos();
		nNextNode = nNextNode+1;
		if (nNextNode > m_lstNodes.GetNumOfElements()-1)
			nNextNode = m_bClosed ? 0 : nNextNode-1;
		const Vector3 &vP4 = m_lstNodes[nNextNode]->GetPos();

		float t  = fProgress;
		float t2 = t*t;
		float t3 = t2*t;
		float t1 = (1-t)*(1-t);

		return Vector3((-t*t1*vP1.x + (2-5*t2+3*t3)*vP2.x + t*(1+4*t-3*t2)*vP3.x - t2*(1-t)*vP4.x)/2,
					   (-t*t1*vP1.y + (2-5*t2+3*t3)*vP2.y + t*(1+4*t-3*t2)*vP3.y - t2*(1-t)*vP4.y)/2,
					   (-t*t1*vP1.z + (2-5*t2+3*t3)*vP2.z + t*(1+4*t-3*t2)*vP3.z - t2*(1-t)*vP4.z)/2);
	}
}

/**
*  @brief
*    Calculates the position by using percentage along the path
*/
Vector3 GraphPath::GetPosByPercentageAlongPath(float fPercentageAlongPath, bool bLinear) const
{
	// [TODO] Under construction

	// Check bounding
	if (!m_lstNodes.GetNumOfElements())
		return Vector3::Zero; // Error!
	if (m_lstNodes.GetNumOfElements() == 1)
		return m_lstNodes[0]->GetPos();

	// Linear interpolation?
	if (bLinear) {
		// Clamp to 0-1
		if (fPercentageAlongPath >= 0.0f)
			fPercentageAlongPath = fPercentageAlongPath-static_cast<int>(fPercentageAlongPath);
		else
			fPercentageAlongPath = 1+fPercentageAlongPath-static_cast<int>(fPercentageAlongPath);

		// Get the total path length and current length on path
		float fTotalPathLength = GetLength();
		float fLengthOnPath    = fPercentageAlongPath*fTotalPathLength;

		// We need to find the to nodes we are currently between

		// Get path length
		float fLength = 0.0f;
		float fPreviousLength = 0.0f;
		float fDistanceBetweenNodes = 0.0f;
		const GraphNode *pLastNode = nullptr;
		Iterator<GraphNode*> cIterator = m_lstNodes.GetIterator();
		while (cIterator.HasNext() && fLength < fLengthOnPath) {
			// Backup the current length on path
			fPreviousLength = fLength;

			const GraphNode *pNode = cIterator.Next();
			if (pLastNode) {
				fDistanceBetweenNodes = pNode->GetDistance(*pLastNode);
				fLength += fDistanceBetweenNodes;

				if (fLength >= fLengthOnPath) {
					// Get current and next node
					const GraphNode *pCurrentNode = pLastNode;
					const GraphNode *pNextNode    = pNode;

					float fDistance = fLengthOnPath-fPreviousLength;
					float fPercentageBetweenNodes = fDistanceBetweenNodes ? fDistance/fDistanceBetweenNodes : 0.0f;

					// Interpolate
					return pCurrentNode->GetPos()+(pNextNode->GetPos()-pCurrentNode->GetPos())*fPercentageBetweenNodes;
				}
			}

			// Backup the current node
			pLastNode = pNode;
		}

		// Handle the case of closed path
		if (m_bClosed) {
			const GraphNode *pNode = m_lstNodes[0];
			if (pLastNode) {
				fDistanceBetweenNodes = pNode->GetDistance(*pLastNode);
				fLength += fDistanceBetweenNodes;

				if (fLength >= fLengthOnPath) {
					// Get current and next node
					const GraphNode *pCurrentNode = pLastNode;
					const GraphNode *pNextNode    = pNode;

					float fDistance = fLengthOnPath-fPreviousLength;
					float fPercentageBetweenNodes = (fDistanceBetweenNodes ? fDistance/fDistanceBetweenNodes : 0.0f)-1;

					// Interpolate
					return pCurrentNode->GetPos()+(pNextNode->GetPos()-pCurrentNode->GetPos())*fPercentageBetweenNodes;
				}
			}
		}

	// None linear
	} else {
	}

	// Error, return the position of the first path node
	return m_lstNodes[0]->GetPos();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Resource functions             ]
//[-------------------------------------------------------]
GraphPath &GraphPath::operator =(const GraphPath &cSource)
{
	// Unload the old path
	Unload();

	// Call base implementation
	*static_cast<Resource<GraphPath>*>(this) = cSource;

	// Copy data
	m_pOwnerGraph = cSource.m_pOwnerGraph;
	m_bClosed     = cSource.m_bClosed;
	if (m_pOwnerGraph) {
		// Only copy node links
		m_lstNodes = cSource.m_lstNodes;
	} else {
		// Copy nodes
		Iterator<GraphNode*> cIterator = cSource.m_lstNodes.GetIterator();
		while (cIterator.HasNext())
			AddNode(*(new GraphNode(*cIterator.Next())));
	}

	// Return this
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool GraphPath::Unload()
{
	// Clear nodes
	if (!m_pOwnerGraph) {
		Iterator<GraphNode*> cIterator = m_lstNodes.GetIterator();
		while (cIterator.HasNext())
			delete cIterator.Next();
	}
	m_lstNodes.Clear();

	// Reset data
	m_pOwnerGraph = nullptr;
	m_bClosed     = false;

	// Call base implementation
	return Resource<GraphPath>::Unload();
}

String GraphPath::GetLoadableTypeName() const
{
	static const String sString = "GraphPath";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
