/*********************************************************\
 *  File: SNCellPortal.cpp                               *
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
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SNCellPortal.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNCellPortal)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNCellPortal::GetTargetCell() const
{
	return m_sTargetCell;
}

void SNCellPortal::SetTargetCell(const String &sValue)
{
	if (m_sTargetCell != sValue) {
		// Remove the current target cell
		SceneNode *pCell = m_pTargetCell->GetElement();
		if (pCell) {
			// Disconnect event handlers
			pCell->SignalContainer.Disconnect(EventHandlerCellContainerPositionRotationScale);
			pCell->GetTransform().EventPosition.Disconnect(EventHandlerCellContainerPositionRotationScale);
			pCell->GetTransform().EventRotation.Disconnect(EventHandlerCellContainerPositionRotationScale);
			pCell->GetTransform().EventScale.   Disconnect(EventHandlerCellContainerPositionRotationScale);

			// Clear target cell handler
			m_pTargetCell->SetElement();
		}

		// Set the new target cell name
		m_sTargetCell = sValue;

		// We need to recalculate the warp matrix
		m_nInternalPortalFlags |= RecalculateWarpMatrix;
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNCellPortal::SNCellPortal() :
	TargetCell(this),
	Flags(this),
	EventHandlerCellContainerPositionRotationScale(&SNCellPortal::OnCellContainerPositionRotationScale, this),
	m_pTargetCell(new SceneNodeHandler())
{
	// Set the internal portal flag
	m_nInternalPortalFlags |= RecalculateWarpMatrix;
}

/**
*  @brief
*    Destructor
*/
SNCellPortal::~SNCellPortal()
{
	delete m_pTargetCell;
}

/**
*  @brief
*    Returns the cell the cell-portal links to
*/
SCCell *SNCellPortal::GetTargetCellInstance()
{
	if (!m_pTargetCell->GetElement() && m_sTargetCell.GetLength()) {
		SceneNode *pCell = GetContainer()->GetByName(m_sTargetCell);
		if (pCell && pCell->IsCell()) {
			m_pTargetCell->SetElement(pCell);

			// Connect event handlers
			pCell->SignalContainer.Connect(EventHandlerCellContainerPositionRotationScale);
			pCell->GetTransform().EventPosition. Connect(EventHandlerCellContainerPositionRotationScale);
			pCell->GetTransform().EventRotation. Connect(EventHandlerCellContainerPositionRotationScale);
			pCell->GetTransform().EventScale.    Connect(EventHandlerCellContainerPositionRotationScale);
		}
	}

	// Return the target cell
	return reinterpret_cast<SCCell*>(m_pTargetCell->GetElement());
}

/**
*  @brief
*    Returns the warp matrix
*/
const Matrix3x4 &SNCellPortal::GetWarpMatrix()
{
	// Calculate warp matrix if required
	if (m_nInternalPortalFlags & RecalculateWarpMatrix) {
		// Get the scene container this portal is in
		SceneContainer *pContainer = GetContainer();
		if (pContainer) {
			// Get the target cell
			SceneNode *pTargetNode = pContainer->GetByName(m_sTargetCell);
			if (pTargetNode && pTargetNode->IsCell()) {
				// Get transform matrix
				pContainer->GetTransformMatrixTo(static_cast<SceneContainer&>(*pTargetNode), m_mWarp);
			}
		}

		// Recalculation done
		m_nInternalPortalFlags &= ~RecalculateWarpMatrix;
	}

	// Return the warp matrix
	return m_mWarp;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the target cell container, position, rotation or scale changed
*/
void SNCellPortal::OnCellContainerPositionRotationScale()
{
	// We need to recalculate the warp matrix
	m_nInternalPortalFlags |= RecalculateWarpMatrix;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
