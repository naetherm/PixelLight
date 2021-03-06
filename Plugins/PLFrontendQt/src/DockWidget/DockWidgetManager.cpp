/*********************************************************\
 *  File: DockWidgetManager.cpp                          *
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
#include <QtGui/qdockwidget.h>
#include <PLCore/Base/Class.h>
#include "PLFrontendQt/DockWidget/DockWidget.h"
#include "PLFrontendQt/DockWidget/DockWidgetManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetManager, "PLFrontendQt", PLCore::Object, "PixelLight RTTI dock widget manager")
	// Methods
	pl_method_1_metadata(ShowDockWidget,					pl_ret_type(DockWidget*),	const PLCore::String&,							"Shows a dock widget. Class name of the PixelLight RTTI class encapsulating a Qt dock widget (must be derived from \"PLFrontendQt::DockWidget\") as first parameter.",																"")
	pl_method_1_metadata(GetFirstDockWidget,				pl_ret_type(DockWidget*),	const PLCore::String&,							"Returns the first dock widget instance which is an instance of the given class. Class name of the PixelLight RTTI class encapsulating a Qt dock widget (must be derived from \"PLFrontendQt::DockWidget\") as first parameter.",	"")
	pl_method_0_metadata(HideDockWidgets,					pl_ret_type(void),															"Hides all registered dock widgets.",																																																"")
	pl_method_0_metadata(DestroyDockWidgets,				pl_ret_type(void),															"Destroys all registered dock widgets.",																																															"")
	pl_method_2_metadata(SetDockWidgetsAttribute,			pl_ret_type(void),			const PLCore::String&,	const PLCore::String&,	"Set dock widgets attribute value (dock widget broadcast). Attribute name as first parameter, attribute value as second parameter.",																								"")
	pl_method_1_metadata(SetDockWidgetsAttributeDefault,	pl_ret_type(void),			const PLCore::String&,							"Set dock widgets attribute to it's default value (dock widget broadcast). Attribute name as first parameter.",																														"")
	pl_method_2_metadata(CallDockWidgetsMethod,				pl_ret_type(void),			const PLCore::String&,	const PLCore::String&,	"Call dock widgets method (dock widget broadcast). Method name as first parameter, parameters as string (e.g. \"Param0='x' Param1='y'\") as second parameter.",																		"")
	pl_method_1_metadata(SetDockWidgetsValues,				pl_ret_type(void),			const PLCore::String&,							"Set multiple dock widgets attribute values as a string at once (dock widget broadcast). String containing attributes and values as first parameter (e.g. \"Name='Bob' Position='1 2 3'\").",										"")
	pl_method_0_metadata(SetDockWidgetsDefaultValues,		pl_ret_type(void),															"Set all dock widgets attributes to default (dock widget broadcast).",																																								"")
pl_class_metadata_end(DockWidgetManager)


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String DockWidgetManager::PreBroadcast  = "Pre";	/**< "Pre"-string */
const String DockWidgetManager::PostBroadcast = "Post";	/**< "Post"-string */


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetManager::DockWidgetManager(QMainWindow *pQMainWindow) :
	m_pQMainWindow(pQMainWindow)
{
}

/**
*  @brief
*    Destructor
*/
DockWidgetManager::~DockWidgetManager()
{
	// Inform all registered dock widgets that this dock widget manager is now gone (do not destroy the dock widgets in here!)
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->m_pDockWidgetManager = nullptr;
}

/**
*  @brief
*    Shows a dock widget
*/
DockWidget *DockWidgetManager::ShowDockWidget(const String &sClassName)
{
	// Get the first dock widget instance which is an instance of the given class
	DockWidget *pDockWidget = GetFirstDockWidget(sClassName);
	if (!pDockWidget) {
		// There's no dock widget instance which is an instance of the given class, yet

		// Get the chosen dock widget RTTI class...
		const Class *pClass = ClassManager::GetInstance()->GetClass(sClassName);
		if (pClass && pClass->IsDerivedFrom("PLFrontendQt::DockWidget")) {
			// ... and create an instance of it (the dock widget will register itself within this dock widget manager)
			pDockWidget = reinterpret_cast<DockWidget*>(pClass->Create(Params<Object*, QMainWindow*, DockWidgetManager*>(m_pQMainWindow, this)));
		}
	}
	if (pDockWidget) {
		// Get the encapsulated Qt dock widget
		QDockWidget *pQDockWidget = pDockWidget->GetQDockWidget();
		if (pQDockWidget) {
			pQDockWidget->show();
			pQDockWidget->activateWindow();
			pQDockWidget->raise();
		}
	}

	// Done
	return pDockWidget;
}

/**
*  @brief
*    Returns the first dock widget instance which is an instance of the given class
*/
DockWidget *DockWidgetManager::GetFirstDockWidget(const String &sClassName)
{
	// Look for the first instance of the given dock widget class
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++) {
		// Get the current dock widget instance
		DockWidget *pDockWidget = m_lstDockWidgets[i];

		// Is this an instance of the given class?
		if (pDockWidget->GetClass()->GetClassName() == sClassName)
			return pDockWidget;
	}

	// No instance of the given dock widget class found
	return nullptr;
}

/**
*  @brief
*    Hides all registered dock widgets
*/
void DockWidgetManager::HideDockWidgets()
{
	// Hide all registered dock widgets
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++) {
		// Get the encapsulated Qt dock widget
		QDockWidget *pQDockWidget = m_lstDockWidgets[i]->GetQDockWidget();
		if (pQDockWidget)
			pQDockWidget->hide();
	}
}

/**
*  @brief
*    Destroys all registered dock widgets (dock widget broadcast)
*/
void DockWidgetManager::DestroyDockWidgets()
{
	// Destroy one dock widget after another, each one will unregister it from this dock widget manager
	while (m_lstDockWidgets.GetNumOfElements())
		delete m_lstDockWidgets[0];
}

/**
*  @brief
*    Set dock widgets attribute value by using a given string value (dock widget broadcast)
*/
void DockWidgetManager::SetDockWidgetsAttribute(const String &sName, const String &sValue)
{
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->SetAttribute(sName, sValue);
}

/**
*  @brief
*    Set dock widgets attribute value by using a given dynamic variable reference (dock widget broadcast)
*/
void DockWidgetManager::SetDockWidgetsAttribute(const String &sName, const DynVar &cVar)
{
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->SetAttribute(sName, cVar);
}

/**
*  @brief
*    Set dock widgets attribute value by using a given dynamic variable pointer (dock widget broadcast)
*/
void DockWidgetManager::SetDockWidgetsAttribute(const String &sName, const DynVarPtr pVar)
{
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->SetAttribute(sName, pVar);
}

/**
*  @brief
*    Set dock widgets attribute to it's default value (dock widget broadcast)
*/
void DockWidgetManager::SetDockWidgetsAttributeDefault(const String &sName)
{
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->SetAttributeDefault(sName);
}

/**
*  @brief
*    Call dock widgets method with given dynamic parameters (dock widget broadcast)
*/
void DockWidgetManager::CallDockWidgetsMethod(const String &sName, DynParams &cParams)
{
	{ // Make a pre-broadcast
		const String sPreName = PreBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPreName, cParams);
	}

	// Make the main broadcast
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->CallMethod(sName, cParams);

	{ // Make a post-broadcast
		const String sPostName = PostBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPostName, cParams);
	}
}

/**
*  @brief
*    Call dock widgets method with given constant dynamic parameters (dock widget broadcast)
*/
void DockWidgetManager::CallDockWidgetsMethod(const String &sName, const DynParams &cParams)
{
	{ // Make a pre-broadcast
		const String sPreName = PreBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPreName, cParams);
	}

	// Make the main broadcast
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->CallMethod(sName, cParams);

	{ // Make a post-broadcast
		const String sPostName = PostBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPostName, cParams);
	}
}

/**
*  @brief
*    Call dock widgets method with parameters given as string (dock widget broadcast)
*/
void DockWidgetManager::CallDockWidgetsMethod(const String &sName, const String &sParams)
{
	{ // Make a pre-broadcast
		const String sPreName = PreBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPreName, sParams);
	}

	// Make the main broadcast
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->CallMethod(sName, sParams);

	{ // Make a post-broadcast
		const String sPostName = PostBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPostName, sParams);
	}
}

/**
*  @brief
*    Call dock widgets method with parameters given as XML element (dock widget broadcast)
*/
void DockWidgetManager::CallDockWidgetsMethod(const String &sName, const XmlElement &cElement)
{
	{ // Make a pre-broadcast
		const String sPreName = PreBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPreName, cElement);
	}

	// Make the main broadcast
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->CallMethod(sName, cElement);

	{ // Make a post-broadcast
		const String sPostName = PostBroadcast + sName;
		for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
			m_lstDockWidgets[i]->CallMethod(sPostName, cElement);
	}
}

/**
*  @brief
*    Set multiple dock widgets attribute values as a string at once (dock widget broadcast)
*/
void DockWidgetManager::SetDockWidgetsValues(const String &sVars)
{
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->SetValues(sVars);
}

/**
*  @brief
*    Set dock widgets attribute values from XML (dock widget broadcast)
*/
void DockWidgetManager::SetDockWidgetsValuesXml(const XmlElement &cElement)
{
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->SetValuesXml(cElement);
}

/**
*  @brief
*    Set all dock widgets attributes to default (dock widget broadcast)
*/
void DockWidgetManager::SetDockWidgetsDefaultValues()
{
	for (uint32 i=0; i<m_lstDockWidgets.GetNumOfElements(); i++)
		m_lstDockWidgets[i]->SetDefaultValues();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
