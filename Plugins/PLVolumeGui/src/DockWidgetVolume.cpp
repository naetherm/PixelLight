/*********************************************************\
 *  File: DockWidgetVolume.cpp                           *
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
#include "PLVolumeGui/DockWidgetVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLVolumeGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetVolume, "PLVolumeGui", PLFrontendQt::DockWidget, "Abstract volume Qt dock widget base class")
	// Properties
	pl_properties
		pl_property("Title", "Volume")
	pl_properties_end
	// Methods
	pl_method_0_metadata(GetSelectedObject,	pl_ret_type(PLCore::Object*),						"Returns the currently selected object, can be a null pointer.",	"")
	pl_method_1_metadata(SelectObject,		pl_ret_type(void),				PLCore::Object*,	"Selects the given object. Object to select as first parameter.",	"")
	// Slots
	pl_slot_0_metadata(OnDestroyed,	"Called when the object assigned with this dock widget was destroyed",	"")
pl_class_metadata_end(DockWidgetVolume)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the currently selected object
*/
Object *DockWidgetVolume::GetSelectedObject() const
{
	return m_pObject;
}

/**
*  @brief
*    Selects the given object
*/
void DockWidgetVolume::SelectObject(Object *pObject)
{
	// State change?
	if (m_pObject != pObject) {
		// Disconnect event handler
		if (m_pObject) {
			m_pObject->SignalDestroyed.Disconnect(SlotOnDestroyed);
			m_pObject = nullptr;
		}

		// We're only interested in volume scene nodes
		if (pObject && pObject->IsInstanceOf("PLVolume::SNVolume")) {
			// Backup the given object pointer
			m_pObject = pObject;

			// Connect event handler
			if (m_pObject)
				m_pObject->SignalDestroyed.Connect(SlotOnDestroyed);
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetVolume::DockWidgetVolume(QWidget *pQWidgetParent, PLFrontendQt::DockWidgetManager *pDockWidgetManager) : DockWidget(pQWidgetParent, pDockWidgetManager),
	SlotOnDestroyed(this),
	m_pObject(nullptr)
{
	// Ask the RTTI dock widget fellows whether or not someone knows which is the currently selected object

	// Get a list of dock widgets registered within the same dock widget manager this dock widget is in
	const Array<DockWidget*> &lstDockWidgets = GetFellowDockWidgets();
	Object *pObject = nullptr;
	for (uint32 i=0; i<lstDockWidgets.GetNumOfElements() && !pObject; i++) {
		// Get the dock widget, and ignore our own ego
		DockWidget *pDockWidget = lstDockWidgets[i];
		if (pDockWidget != this) {
			// Get the typed dynamic parameters
			Params<Object*> cParams;

			// Call the RTTI method
			pDockWidget->CallMethod("GetSelectedObject", cParams);

			// Get the result, we're interested in any object
			pObject = cParams.Return;
			if (pObject)
				SelectObject(pObject);
		}
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetVolume::~DockWidgetVolume()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the object assigned with this dock widget was destroyed
*/
void DockWidgetVolume::OnDestroyed()
{
	// Argh! Mayday! We lost our object!
	// -> Now no object is currently selected
	SelectObject(nullptr);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeGui
