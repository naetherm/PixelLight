/*********************************************************\
 *  File: DockWidgetManager.h                            *
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


#ifndef __PLFRONTENDQT_DOCKWIDGETMANAGER_H__
#define __PLFRONTENDQT_DOCKWIDGETMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLFrontendQt/PLFrontendQt.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMainWindow;
QT_END_NAMESPACE
namespace PLFrontendQt {
	class DockWidget;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight RTTI dock widget manager
*
*  @remarks
*    When using the "CallDockWidgetsMethod()"-method to perform a broadcast, the following actions are performed:
*   - Calling "Pre<method name>"
*   - Calling "<method name>"
*   - Calling "Post<method name>" (don't do anything in here which may influence the outside world)
*
*   Just calling directly "<method name>" may not be enough for every broadcast participator due to the near endless
*   possibilities what such a participator can actually do when receiving the broadcast. Let's have a more or less
*   abstract example to make the background clear:
*   For better usability a broadcast participator is calling the Qt method "QTreeView::scrollTo()" to bring something
*   into the visible area. "QTreeView::scrollTo()" is using the widget size, which may change during the main-broadcast
*   when any broadcast participator is for example opening another dock widget with additional information causing the GUI
*   to reduce the hight of our own dock widget. Or changing the size of the main window. Or changing the display resolution.
*   Or doing something totally unexpected crazy. So, while "QTreeView::scrollTo()" was correct at the exact time it was called,
*   the desired final result visible to the user may not be correct when the main-broadcast has been finished.
*   -> This is no problem of the GUI system, a GUI may not even be used at all in certain system constellation
*   -> This is no problem of any application using this system, the using application can't know any technical details of
*      a certain system constellation, nor what broadcast participators will do
*
*   Let's take "DockWidgetSceneGraph::SelectObject() as a concrete example, even if it may not be used in a certain system
*   constellation. In here, the order "issue" from above can't be resolved by simply ensuring that the broadcast receivers
*   are in the order required to make the special case "DockWidgetSceneGraph::SelectObject()" behaves correctly. Reasons:
*   1. Adding any fixed order would require additional information, which is not available to the system or users of the system
*   2. The system itself does not know "DockWidgetSceneGraph::SelectObject()", nor does it know what any broadcast participator do
*   3. Another broadcast participator like "Bob::SelectObject()" may require a totally different broadcast receiver order
*   4. Another broadcast like "Bob::TakesOverTheWorld()" may require a totally different broadcast receiver order
*   5. Any application using this system, can't know what's happening when doing a broadcast, and therefore can't
*      add any special hacks to work around issues possible with certain system configurations, which are also not
*      known due to the dynamic nature of the system. New plugins can add new dock widget required new special behavior.
*   6. Adding any restrictions to this chaotic system would artificially limit it's use or would add dependencies where no
*      dependencies are required
*
*  @note
*    - Loosely couples together RTTI dock widgets, the RTTI dock widgets can still be destroyed individually (e.g. when the Qt dock widget is destroyed)
*/
class DockWidgetManager : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class DockWidget;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		PLFRONTENDQT_API static const PLCore::String PreBroadcast;	/**< "Pre"-string */
		PLFRONTENDQT_API static const PLCore::String PostBroadcast;	/**< "Post"-string */


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLFRONTENDQT_API)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pQMainWindow
		*    Pointer to Qt main window, can be a null pointer
		*
		*  @note
		*    - You're responsible for destroying this PixelLight RTTI dock widget manager instance
		*/
		PLFRONTENDQT_API DockWidgetManager(QMainWindow *pQMainWindow);

		/**
		*  @brief
		*    Destructor
		*/
		PLFRONTENDQT_API virtual ~DockWidgetManager();

		//[-------------------------------------------------------]
		//[ Management                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Shows a dock widget
		*
		*  @param[in] sClassName
		*    Class name of the PixelLight RTTI class encapsulating a Qt dock widget (must be derived from "PLFrontendQt::DockWidget")
		*
		*  @return
		*    The shown dock widget, can be a null pointer (you don't need to care about destroying the dock widget instance)
		*
		*  @note
		*    - If there's already a shown dock widget of the given class name, it will get the focus
		*    - If there's already a dock widget of the given class name, it will be shown
		*    - If there's currently no dock widget of the given class name, it will be instanced and shown
		*/
		PLFRONTENDQT_API DockWidget *ShowDockWidget(const PLCore::String &sClassName);

		/**
		*  @brief
		*    Returns the first dock widget instance which is an instance of the given class
		*
		*  @param[in] sClassName
		*    Class name of the PixelLight RTTI class encapsulating a Qt dock widget (must be derived from "PLFrontendQt::DockWidget")
		*
		*  @return
		*    The first dock widget instance which is an instance of the given class, can be a null pointer
		*/
		PLFRONTENDQT_API DockWidget *GetFirstDockWidget(const PLCore::String &sClassName);

		/**
		*  @brief
		*    Hides all registered dock widgets
		*/
		PLFRONTENDQT_API void HideDockWidgets();

		/**
		*  @brief
		*    Destroys all registered dock widgets
		*/
		PLFRONTENDQT_API void DestroyDockWidgets();

		/**
		*  @brief
		*    Returns the list of registered dock widgets
		*
		*  @return
		*    The list of registered dock widgets
		*/
		inline const PLCore::Array<DockWidget*> &GetDockWidgets() const;

		//[-------------------------------------------------------]
		//[ Communication - Direct access functions               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set dock widgets attribute value by using a given string value (dock widget broadcast)
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] sValue
		*    Attribute value as string
		*
		*  @remarks
		*    Similar to the "PLCore::Object::SetAttribute()"-method but iterates over all registered
		*    dock widgets and calls the "SetAttribute()"-method of each RTTI dock widget class instance.
		*/
		PLFRONTENDQT_API void SetDockWidgetsAttribute(const PLCore::String &sName, const PLCore::String &sValue);

		/**
		*  @brief
		*    Set dock widgets attribute value by using a given dynamic variable reference (dock widget broadcast)
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] cVar
		*    Attribute value as dynamic variable reference
		*
		*  @see
		*    - "void SetDockWidgetsAttribute(const PLCore::String &sName, const PLCore::String &sValue)"
		*/
		PLFRONTENDQT_API void SetDockWidgetsAttribute(const PLCore::String &sName, const PLCore::DynVar &cVar);

		/**
		*  @brief
		*    Set dock widgets attribute value by using a given dynamic variable pointer (dock widget broadcast)
		*
		*  @param[in] sName
		*    Attribute name
		*  @param[in] pVar
		*    Attribute value as dynamic variable pointer, in case of a null pointer, nothing happens at all
		*
		*  @see
		*    - "void SetDockWidgetsAttribute(const PLCore::String &sName, const PLCore::String &sValue)"
		*/
		PLFRONTENDQT_API void SetDockWidgetsAttribute(const PLCore::String &sName, const PLCore::DynVarPtr pVar);

		/**
		*  @brief
		*    Set dock widgets attribute to it's default value (dock widget broadcast)
		*
		*  @param[in] sName
		*    Attribute name
		*
		*  @remarks
		*    Similar to the "PLCore::Object::SetAttributeDefault()"-method but iterates over all registered
		*    dock widgets and calls the "SetAttributeDefault()"-method of each RTTI dock widget class instance.
		*/
		PLFRONTENDQT_API void SetDockWidgetsAttributeDefault(const PLCore::String &sName);

		/**
		*  @brief
		*    Call dock widgets method with given dynamic parameters (dock widget broadcast)
		*
		*  @param[in] sName
		*    Method name
		*  @param[in] cParams
		*    Dynamic parameters
		*
		*  @remarks
		*    Similar to the "PLCore::Object::CallMethod()"-method but iterates over all registered
		*    dock widgets and calls the "CallMethod()"-method of each RTTI dock widget class instance.
		*
		*  @note
		*   - Calling "Pre<method name>"
		*   - Calling "<method name>"
		*   - Calling "Post<method name>" (don't do anything in here which may influence the outside world)
		*   - See "DockWidgetManager" class documentation for more details
		*/
		PLFRONTENDQT_API void CallDockWidgetsMethod(const PLCore::String &sName, PLCore::DynParams &cParams);

		/**
		*  @brief
		*    Call dock widgets method with given constant dynamic parameters (dock widget broadcast)
		*
		*  @param[in] sName
		*    Method name
		*  @param[in] cParams
		*    Constant dynamic parameters
		*
		*  @see
		*    - "void CallDockWidgetsMethod(const PLCore::String &sName, PLCore::DynParams &cParams)"
		*/
		PLFRONTENDQT_API void CallDockWidgetsMethod(const PLCore::String &sName, const PLCore::DynParams &cParams);

		/**
		*  @brief
		*    Call dock widgets method with parameters given as string (dock widget broadcast)
		*
		*  @param[in] sName
		*    Method name
		*  @param[in] sParams
		*    Parameters as string
		*
		*  @see
		*    - "void CallDockWidgetsMethod(const PLCore::String &sName, PLCore::DynParams &cParams)"
		*/
		PLFRONTENDQT_API void CallDockWidgetsMethod(const PLCore::String &sName, const PLCore::String &sParams);

		/**
		*  @brief
		*    Call dock widgets method with parameters given as XML element (dock widget broadcast)
		*
		*  @param[in] sName
		*    Method name
		*  @param[in] cElement
		*    Parameters as XML element
		*
		*  @see
		*    - "void CallDockWidgetsMethod(const PLCore::String &sName, PLCore::DynParams &cParams)"
		*/
		PLFRONTENDQT_API void CallDockWidgetsMethod(const PLCore::String &sName, const PLCore::XmlElement &cElement);

		//[-------------------------------------------------------]
		//[ Communication - Object state functions                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set multiple dock widgets attribute values as a string at once (dock widget broadcast)
		*
		*  @param[in] sString
		*    String containing attributes and values (e.g. \"Name='Bob' Position='1 2 3'\")
		*
		*  @remarks
		*    Similar to the "PLCore::Object::SetValues()"-method but iterates over all registered
		*    dock widgets and calls the "SetValues()"-method of each RTTI dock widget class instance.
		*/
		PLFRONTENDQT_API void SetDockWidgetsValues(const PLCore::String &sVars);

		/**
		*  @brief
		*    Set dock widgets attribute values from XML (dock widget broadcast)
		*
		*  @param[out] cElement
		*    XML element
		*
		*  @see
		*    - "void SetDockWidgetsValues(const PLCore::String &sVars)"
		*/
		PLFRONTENDQT_API void SetDockWidgetsValuesXml(const PLCore::XmlElement &cElement);

		/**
		*  @brief
		*    Set all dock widgets attributes to default (dock widget broadcast)
		*
		*  @remarks
		*    Similar to the "PLCore::Object::SetDefaultValues()"-method but iterates over all registered
		*    dock widgets and calls the "SetDefaultValues()"-method of each RTTI dock widget class instance.
		*/
		PLFRONTENDQT_API void SetDockWidgetsDefaultValues();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Registers the given dock widget within this dock widget manager
		*
		*  @param[in] cDockWidget
		*    Dock widget to register
		*/
		inline void RegisterDockWidget(DockWidget &cDockWidget);

		/**
		*  @brief
		*    Unregisters the given dock widget within this dock widget manager
		*
		*  @param[in] cDockWidget
		*    Dock widget to unregister
		*/
		inline void UnregisterDockWidget(DockWidget &cDockWidget);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		QMainWindow				   *m_pQMainWindow;		/**< Pointer to Qt main window, can be a null pointer */
		PLCore::Array<DockWidget*>  m_lstDockWidgets;	/**< List of registered dock widgets */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLFrontendQt/DockWidget/DockWidgetManager.inl"


#endif // __PLFRONTENDQT_DOCKWIDGETMANAGER_H__
