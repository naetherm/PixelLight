/*********************************************************\
 *  File: DockWidget.cpp                                 *
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
#include <QtGui/qmainwindow.h>
#include "PLFrontendQt/DockWidget/DockWidgetManager.h"
#include "PLFrontendQt/DockWidget/DockWidget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidget, "PLFrontendQt", PLCore::Object, "Abstract PixelLight RTTI class encapsulating a Qt dock widget")
	// Properties
	pl_properties
		pl_property("Title", "")
	pl_properties_end
pl_class_metadata_end(DockWidget)


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Simple helper class to inform the RTTI dock widget when the Qt dock widget gets destroyed
*/
class InternalQDockWidget : public QDockWidget {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class DockWidget;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cDockWidget
		*    RTTI dock widget we need to inform when we get destroyed
		*  @param[in] pQWidgetParent
		*    Pointer to parent Qt widget as parameter, can be a null pointer
		*/
		InternalQDockWidget(DockWidget &cDockWidget, QWidget *pQWidgetParent) : QDockWidget(pQWidgetParent),
			m_pDockWidget(&cDockWidget)
		{
			// Enable getting input focus when the user clicks on the header part of the QDockWidget.
			// Normally the QDockWidget itself or at least its titlebar doesn't get input focus.
			// In conjunction with not setting the render window directly as the central window of the QMainWindow, this
			// solves the "problem" that the "render window" processes mouse move input events while the user drags an QDockWidget.
			setFocusPolicy(Qt::ClickFocus);

			// By default, when someone is closing the dock widget Qt hides instead of deletes it
			// -> We could write
			//        setAttribute(Qt::WA_DeleteOnClose);	// Delete the dock widget instead of hiding it when someone is closing it
			//    to change this behavior, but when "reopening" the previously made changes to the widget layout would be gone and
			//    we don't want this. So, long story short conclusion, don't change the default behavior in here.
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~InternalQDockWidget()
		{
			// Inform the RTTI dock widget politely that we're now dead, then shoot at it...
			if (m_pDockWidget) {
				m_pDockWidget->m_pQDockWidget = nullptr;
				delete m_pDockWidget;
			}
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DockWidget *m_pDockWidget;	/**< RTTI dock widget we need to inform when we get destroyed, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DockWidget::~DockWidget()
{
	// If there's a dock widget manager provided, unregister this dock widget
	if (m_pDockWidgetManager)
		m_pDockWidgetManager->UnregisterDockWidget(*this);

	// Destroy the encapsulated Qt dock widget, if it still exists
	if (m_pQDockWidget) {
		m_pQDockWidget->m_pDockWidget = nullptr;
		delete m_pQDockWidget;
	}
}

/**
*  @brief
*    Returns a list of dock widgets registered within the same dock widget manager this dock widget is in
*/
const Array<DockWidget*> &DockWidget::GetFellowDockWidgets() const
{
	// If there's a dock widget manager provided, return it's dock widget list
	if (m_pDockWidgetManager) {
		return m_pDockWidgetManager->GetDockWidgets();
	} else {
		// We need to return anything decent (we don't want to return a pointer, because the user would have to check for a null pointer which is annoying)
		static const Array<DockWidget*> lstEmpty;
		return lstEmpty;
	}
}

/**
*  @brief
*    Returns whether or not the encapsulated Qt dock widget is currently visible
*/
bool DockWidget::IsQDockWidgetVisible() const
{
	return m_pQDockWidget ? m_pQDockWidget->isVisible() : false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidget::DockWidget(QWidget *pQWidgetParent, DockWidgetManager *pDockWidgetManager) :
	m_pQDockWidget(pQWidgetParent ? new InternalQDockWidget(*this, pQWidgetParent) : nullptr),
	m_pDockWidgetManager(pDockWidgetManager)
{
	// If there's a dock widget manager provided, register this dock widget
	if (m_pDockWidgetManager)
		m_pDockWidgetManager->RegisterDockWidget(*this);
}

/**
*  @brief
*    Set dock widgets attribute value by using a given string value (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttribute(const String &sName, const String &sValue)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttribute(sName, sValue);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute value by using a given dynamic variable reference (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttribute(const String &sName, const DynVar &cVar)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttribute(sName, cVar);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute value by using a given dynamic variable pointer (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttribute(const String &sName, const DynVarPtr pVar)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttribute(sName, pVar);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute to it's default value (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsAttributeDefault(const String &sName)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetAttributeDefault(sName);
		}
	}
}

/**
*  @brief
*    Call dock widgets method with given dynamic parameters (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, DynParams &cParams)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		{ // Make a pre-broadcast
			const String sPreName = DockWidgetManager::PreBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPreName, cParams);
			}
		}

		// Perform main broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, cParams);
		}

		{ // Make a post-broadcast
			const String sPostName = DockWidgetManager::PostBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPostName, cParams);
			}
		}
	}
}

/**
*  @brief
*    Call dock widgets method with given constant dynamic parameters (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, const DynParams &cParams)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		{ // Make a pre-broadcast
			const String sPreName = DockWidgetManager::PreBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPreName, cParams);
			}
		}

		// Perform main broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, cParams);
		}

		{ // Make a post-broadcast
			const String sPostName = DockWidgetManager::PostBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPostName, cParams);
			}
		}
	}
}

/**
*  @brief
*    Call dock widgets method with parameters given as string (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, const String &sParams)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		{ // Make a pre-broadcast
			const String sPreName = DockWidgetManager::PreBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPreName, sParams);
			}
		}

		// Perform main broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, sParams);
		}

		{ // Make a post-broadcast
			const String sPostName = DockWidgetManager::PostBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPostName, sParams);
			}
		}
	}
}

/**
*  @brief
*    Call dock widgets method with parameters given as XML element (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::CallDockWidgetsMethod(const String &sName, const XmlElement &cElement)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		{ // Make a pre-broadcast
			const String sPreName = DockWidgetManager::PreBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPreName, cElement);
			}
		}

		// Perform main broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->CallMethod(sName, cElement);
		}

		{ // Make a post-broadcast
			const String sPostName = DockWidgetManager::PostBroadcast + sName;
			for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
				// Get the current dock widget, do the exclude test and broadcast
				DockWidget *pDockWidget = lstDockWidgets[i];
				if (pDockWidget != this)
					pDockWidget->CallMethod(sPostName, cElement);
			}
		}
	}
}

/**
*  @brief
*    Set multiple dock widgets attribute values as a string at once (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsValues(const String &sVars)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetValues(sVars);
		}
	}
}

/**
*  @brief
*    Set dock widgets attribute values from XML (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsValuesXml(const XmlElement &cElement)
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetValuesXml(cElement);
		}
	}
}

/**
*  @brief
*    Set all dock widgets attributes to default (dock widget broadcast, excludes the emitting dock widget)
*/
void DockWidget::SetDockWidgetsDefaultValues()
{
	// If there's a dock widget manager provided, perform a dock widget manager broadcast
	if (m_pDockWidgetManager) {
		// Get list of registered dock widgets
		const Array<DockWidget*> &lstDockWidgets = m_pDockWidgetManager->m_lstDockWidgets;

		// Perform broadcast, but exclude this emitting dock widget
		for (uint32 i=0; i<lstDockWidgets.GetNumOfElements(); i++) {
			// Get the current dock widget, do the exclude test and broadcast
			DockWidget *pDockWidget = lstDockWidgets[i];
			if (pDockWidget != this)
				pDockWidget->SetDefaultValues();
		}
	}
}

/**
*  @brief
*    Adds the given Qt dock widget to the given Qt main window and tabifies it
*/
void DockWidget::AddDockWidgetAndTabify(QMainWindow &cQMainWindow, Qt::DockWidgetArea nQtDockWidgetArea, QDockWidget &cQDockWidget) const
{
	// Add the given Qt dock widget to the given Qt main window
	cQMainWindow.addDockWidget(nQtDockWidgetArea, &cQDockWidget);

	// Tabify
	const QList<QDockWidget*> lstQDockWidgets = cQMainWindow.findChildren<QDockWidget*>();
	for (int i=0; i<lstQDockWidgets.count(); i++) {
		// Get the current dock widget
		QDockWidget *pQDockWidget = lstQDockWidgets.at(i);

		// Is this current dock widget in the same dock widget area?
		if (pQDockWidget != &cQDockWidget && cQMainWindow.dockWidgetArea(pQDockWidget) == nQtDockWidgetArea) {
			// Tabify dock widget
			cQMainWindow.tabifyDockWidget(pQDockWidget, &cQDockWidget);

			// Get us out of the loop
			i = lstQDockWidgets.count();
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
