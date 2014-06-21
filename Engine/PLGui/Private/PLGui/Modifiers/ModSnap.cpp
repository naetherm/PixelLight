/*********************************************************\
 *  File: ModSnap.cpp                                    *
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
#include <PLCore/Base/Event/Event.h>
#include <PLMath/Math.h>
#include "PLGui/Gui/Screen.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Modifiers/ModSnap.h"


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
#define SNAP		12		// Number of pixels below which snapping is performed
#define UNSNAP		15		// Number of pixels that are used to unsnap a widget
#define UNSNAP_GAP	 2		// Number of pixels necessary for unsnapping a widget

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(ModSnap)


//[-------------------------------------------------------]
//[ Global helper functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if two lines overlap each other (one dimensional, so you can use X or Y here)
*/
bool IsInside(int nX1, int nX2, int nLeft, int nRight)
{
	return  (nX1 >= nLeft && nX1 < nRight) ||
			(nX2 >= nLeft && nX2 < nRight) ||
			(nLeft  >= nX1 && nLeft  < nX2) ||
			(nRight >= nX1 && nRight < nX2);
}

/**
*  @brief
*    Check if a widget can snap on to another widget (only allowed, if it is not already snapped to another widget on that side)
*/
bool IsAllowed(Widget *pOldSnap, Widget *pSnapWidget)
{
//	return (pOldSnap == nullptr);
	return (pOldSnap == nullptr || pOldSnap == pSnapWidget);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ModSnap::ModSnap() : Modifier(),
	m_pSnappedLeft(nullptr),
	m_pSnappedRight(nullptr),
	m_pSnappedTop(nullptr),
	m_pSnappedBottom(nullptr),
	m_bPerformedSnap(false)
{
}

/**
*  @brief
*    Destructor
*/
ModSnap::~ModSnap()
{
}


//[-------------------------------------------------------]
//[ Protected virtual Modifier functions                  ]
//[-------------------------------------------------------]
void ModSnap::OnAttach(Widget &cWidget)
{
	// Reset state
	m_pSnappedLeft	 = nullptr;
	m_pSnappedRight  = nullptr;
	m_pSnappedTop	 = nullptr;
	m_pSnappedBottom = nullptr;
	m_bPerformedSnap = false;

	// Save current position
	m_vPos = cWidget.GetPos();
}

void ModSnap::OnDetach(Widget &cWidget)
{
	// Reset state
	m_pSnappedLeft	 = nullptr;
	m_pSnappedRight  = nullptr;
	m_pSnappedTop	 = nullptr;
	m_pSnappedBottom = nullptr;
	m_bPerformedSnap = false;
}


//[-------------------------------------------------------]
//[ Protected virtual WidgetFunctions functions           ]
//[-------------------------------------------------------]
void ModSnap::OnMove(const Vector2i &vPos)
{
	// Has a snap occurred in the last cycle?
	if (m_bPerformedSnap) {
		// Yes. Update to new position and don't do anything else this single time
		m_vPos = vPos;
		m_bPerformedSnap = false;
	} else {
		// Unsnap
		TryUnsnap(vPos);

		// Get parent widget
		// [TODO] GetDesktopWidget()
		Widget *pParent = m_pWidget->GetParent();
//		if (!pParent) pParent = m_pWidget->GetScreen()->GetDesktopWidget();
		if (pParent) {
			// Snap to parent
			TrySnap(m_vPos, pParent, true);

			// Snap to siblings
			for (uint32 i=0; i<pParent->GetChildren().GetNumOfElements(); i++) {
				TrySnap(m_vPos, pParent->GetChildren().Get(i), false);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Try to unsnap the widget from it's current snap-targets
*/
void ModSnap::TryUnsnap(const PLMath::Vector2i &vPos)
{
	// Try unsnap
	Vector2i vNewPos(vPos);

	// Get relative movement
	Vector2i vRel = vPos - m_vPos;

	// Snapped on the left side?
	if (m_pSnappedLeft) {
		// Try to unsnap
		if (vRel.x > UNSNAP_GAP) {
			// Unsnap
			vNewPos.x = vPos.x + static_cast<int>(Math::Sign(static_cast<float>(vRel.x))) * UNSNAP;
			m_pSnappedLeft = nullptr;
		} else {
			// Stay snapped
			vNewPos.x = m_vPos.x;
		}
	}

	// Snapped on the right side?
	if (m_pSnappedRight) {
		// Try to unsnap
		if (vRel.x < -UNSNAP_GAP) {
			// Unsnap
			vNewPos.x = vPos.x + static_cast<int>(Math::Sign(static_cast<float>(vRel.x))) * UNSNAP;
			m_pSnappedRight = nullptr;
		} else {
			// Stay snapped
			vNewPos.x = m_vPos.x;
		}
	}

	// Snapped on the top side?
	if (m_pSnappedTop) {
		// Try to unsnap
		if (vRel.y > UNSNAP_GAP) {
			// Unsnap
			vNewPos.y = vPos.y + static_cast<int>(Math::Sign(static_cast<float>(vRel.y))) * UNSNAP;
			m_pSnappedTop = nullptr;
		} else {
			// Stay snapped
			vNewPos.y = m_vPos.y;
		}
	}

	// Snapped on the bottom side?
	if (m_pSnappedBottom) {
		// Try to unsnap
		if (vRel.y < -UNSNAP_GAP) {
			// Unsnap
			vNewPos.y = vPos.y + static_cast<int>(Math::Sign(static_cast<float>(vRel.y)) * UNSNAP);
			m_pSnappedBottom = nullptr;
		} else {
			// Stay snapped
			vNewPos.y = m_vPos.y;
		}
	}

	// Set new position
	if (vNewPos.x != vPos.x || vNewPos.y != vPos.y) {
		// Update widget position
		m_pWidget->SetPos(vNewPos);
	}

	// Save position
	m_vPos = vNewPos;
}

/**
*  @brief
*    Try to snap to a specific widget
*/
void ModSnap::TrySnap(const Vector2i &vPos, Widget *pSnapWidget, bool bParent)
{
	// Snap state for this widget only
	bool bSnappedLeft	= false;
	bool bSnappedRight	= false;
	bool bSnappedTop	= false;
	bool bSnappedBottom	= false;

	// Get widget position and size
	int nX1 = vPos.x;
	int nY1 = vPos.y;
	int nWidth  = m_pWidget->GetSize().x;
	int nHeight = m_pWidget->GetSize().y;
	int nX2 = vPos.x + nWidth;
	int nY2 = vPos.y + nHeight;

	// Try snap
	Vector2i vNewPos(vPos);
	if (bParent) {
		// Snap to parent

		// Get snap positions
		int nLeft	= 0;
		int nRight	= Math::Max(pSnapWidget->GetSize().x - 1, 0);
		int nTop	= 0;
		int nBottom	= Math::Max(pSnapWidget->GetSize().y - 1, 0);

		// Snap
			 if (Math::Abs(nLeft   - nX1) < SNAP && IsInside(nY1, nY2, nTop, nBottom) && IsAllowed(m_pSnappedLeft,   pSnapWidget)) { bSnappedLeft   = true; vNewPos.x = nLeft; }
		else if (Math::Abs(nRight  - nX2) < SNAP && IsInside(nY1, nY2, nTop, nBottom) && IsAllowed(m_pSnappedRight,  pSnapWidget)) { bSnappedRight  = true; vNewPos.x = nRight - nWidth + 1; }
			 if (Math::Abs(nTop    - nY1) < SNAP && IsInside(nX1, nX2, nLeft, nRight) && IsAllowed(m_pSnappedTop,    pSnapWidget)) { bSnappedTop    = true; vNewPos.y = nTop; }
		else if (Math::Abs(nBottom - nY2) < SNAP && IsInside(nX1, nX2, nLeft, nRight) && IsAllowed(m_pSnappedBottom, pSnapWidget)) { bSnappedBottom = true; vNewPos.y = nBottom - nHeight + 1; }
	} else {
		// Snap to sibling widget

		// Get snap positions
		int nLeft	= pSnapWidget->GetPos().x;
		int nRight	= pSnapWidget->GetPos().x + pSnapWidget->GetSize().x - 1;
		int nTop	= pSnapWidget->GetPos().y;
		int nBottom	= pSnapWidget->GetPos().y + pSnapWidget->GetSize().y - 1;

		// Snap
			 if (Math::Abs(nLeft   - nX2) < SNAP && IsInside(nY1, nY2, nTop, nBottom) && IsAllowed(m_pSnappedRight,  pSnapWidget)) { bSnappedRight  = true; vNewPos.x = nLeft - nWidth + 1; }
		else if (Math::Abs(nRight  - nX1) < SNAP && IsInside(nY1, nY2, nTop, nBottom) && IsAllowed(m_pSnappedLeft,   pSnapWidget)) { bSnappedLeft   = true; vNewPos.x = nRight; }
			 if (Math::Abs(nTop    - nY2) < SNAP && IsInside(nX1, nX2, nLeft, nRight) && IsAllowed(m_pSnappedBottom, pSnapWidget)) { bSnappedBottom = true; vNewPos.y = nTop - nHeight + 1; }
		else if (Math::Abs(nBottom - nY1) < SNAP && IsInside(nX1, nX2, nLeft, nRight) && IsAllowed(m_pSnappedTop,    pSnapWidget)) { bSnappedTop    = true; vNewPos.y = nBottom; }

		// If we are already snapped on to a side of a widget, allow snapping to it's other borders, too
		if (bSnappedTop || bSnappedBottom) {
				 if (Math::Abs(nLeft   - nX1) < SNAP && IsInside(nY1, nY2, nTop, nBottom) && IsAllowed(m_pSnappedLeft,  pSnapWidget)) { bSnappedLeft  = true; vNewPos.x = nLeft; }
			else if (Math::Abs(nRight  - nX2) < SNAP && IsInside(nY1, nY2, nTop, nBottom) && IsAllowed(m_pSnappedRight, pSnapWidget)) { bSnappedRight = true; vNewPos.x = nRight - nWidth + 1; }
		}
		if (bSnappedLeft || bSnappedRight) {
				 if (Math::Abs(nTop    - nY1) < SNAP && IsInside(nX1, nX2, nLeft, nRight) && IsAllowed(m_pSnappedTop,    pSnapWidget)) { bSnappedTop    = true; vNewPos.y = nTop; }
			else if (Math::Abs(nBottom - nY2) < SNAP && IsInside(nX1, nX2, nLeft, nRight) && IsAllowed(m_pSnappedBottom, pSnapWidget)) { bSnappedBottom = true; vNewPos.y = nBottom - nHeight + 1; }
		}
	}

	// Set new position
	if (vNewPos.x != vPos.x || vNewPos.y != vPos.y) {
		// Update snap state
		m_bPerformedSnap = bSnappedLeft | bSnappedRight | bSnappedTop | bSnappedBottom;
		if (bSnappedLeft)	m_pSnappedLeft	 = pSnapWidget;
		if (bSnappedRight)	m_pSnappedRight	 = pSnapWidget;
		if (bSnappedTop)	m_pSnappedTop	 = pSnapWidget;
		if (bSnappedBottom)	m_pSnappedBottom = pSnapWidget;

		// Update widget position
		m_pWidget->SetPos(vNewPos);
	}

	// Check for unnoticed unsnaps
	if (m_pSnappedLeft	 == pSnapWidget && !bSnappedLeft)	m_pSnappedLeft	 = nullptr;
	if (m_pSnappedRight  == pSnapWidget && !bSnappedRight)	m_pSnappedRight  = nullptr;
	if (m_pSnappedTop	 == pSnapWidget && !bSnappedTop)	m_pSnappedTop	 = nullptr;
	if (m_pSnappedBottom == pSnapWidget && !bSnappedBottom)	m_pSnappedBottom = nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
