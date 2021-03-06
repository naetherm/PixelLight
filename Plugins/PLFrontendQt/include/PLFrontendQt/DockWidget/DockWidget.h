/*********************************************************\
 *  File: DockWidget.h                                   *
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


#ifndef __PLFRONTENDQT_DOCKWIDGET_H__
#define __PLFRONTENDQT_DOCKWIDGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qnamespace.h>
#include <PLCore/Base/Object.h>
#include "PLFrontendQt/PLFrontendQt.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QWidget;
	class QMainWindow;
	class QDockWidget;
QT_END_NAMESPACE
namespace PLFrontendQt {
	class DockWidgetManager;
	class InternalQDockWidget;
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
*    Abstract PixelLight RTTI class encapsulating a Qt dock widget
*
*  @remarks
*  @verbatim
*    Usage example:
*        "
*        const PLCore::Class *pClass = PLCore::ClassManager::GetInstance()->GetClass("PLFrontendQt::DockWidgetSceneGraph");
*        if (pClass)
*            pClass->Create(PLCore::Params<PLCore::Object*, QWidget*>(pQtMainWindow)));
*        "
*    The destruction of the created RTTI class instance is automatically handled by the Qt main window, see notes below.
*  @endverbatim
*
*    Each RTTI dock widget should have the following properties:
*    - "Title": Human readable title, may be shown e.g. within a menu and is used by default as the encapsulated Qt dock widget title
*
*  @note
*    - When a RTTI dock widget is instanced, it automatically creates the Qt dock widget it's encapsulating, access it by using "GetQDockWidget()"
*    - Derived RTTI dock widgets must implement a decent default behavior to make it possible to use them out-of-the-box
*    - When a RTTI dock widget instance gets destroyed, it automatically destroys the Qt dock widget it's encapsulating
*    - When the encapsulated Qt dock widget gets destroyed, it automatically destroys the encapsulating RTTI dock widget
*    - This behavior results in a Qt similar behavior, when a RTTI dock widget is instanced it's parent takes over the control
*    - By convention, one should only derive new RTTI dock widgets from abstract RTTI dock widgets
*/
class DockWidget : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class DockWidgetManager;
	friend class InternalQDockWidget;


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
		*    Destructor
		*/
		PLFRONTENDQT_API virtual ~DockWidget();

		/**
		*  @brief
		*    Returns the pointer to the dock widget manager this dock widget is registered to
		*
		*  @return
		*    The pointer to the dock widget manager this dock widget is registered to, can be a null pointer
		*/
		inline DockWidgetManager *GetDockWidgetManager() const;

		/**
		*  @brief
		*    Returns a list of dock widgets registered within the same dock widget manager this dock widget is in
		*
		*  @return
		*    A list of dock widgets registered within the same dock widget manager this dock widget is in,
		*    empty list if there's no dock widget manager
		*/
		PLFRONTENDQT_API const PLCore::Array<DockWidget*> &GetFellowDockWidgets() const;

		/**
		*  @brief
		*    Returns the encapsulated Qt dock widget
		*
		*  @return
		*    The encapsulated Qt dock widget, can be a null pointer
		*/
		inline QDockWidget *GetQDockWidget() const;

		/**
		*  @brief
		*    Returns whether or not the encapsulated Qt dock widget is currently visible
		*
		*  @return
		*    'true' if the encapsulated Qt dock widget is currently visible, else 'false'
		*
		*  @note
		*    - Comfort method, you can also use the "QDockWidget"-instance directly
		*/
		PLFRONTENDQT_API bool IsQDockWidgetVisible() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pQWidgetParent
		*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this instance)
		*  @param[in] pDockWidgetManager
		*    Optional pointer to the dock widget manager this dock widget should be registered to, can be a null pointer
		*/
		PLFRONTENDQT_API DockWidget(QWidget *pQWidgetParent, DockWidgetManager *pDockWidgetManager = nullptr);

		//[-------------------------------------------------------]
		//[ Communication - Direct access functions               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set dock widgets attribute value by using a given string value (dock widget broadcast, excludes the emitting dock widget)
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
		*    Set dock widgets attribute value by using a given dynamic variable reference (dock widget broadcast, excludes the emitting dock widget)
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
		*    Set dock widgets attribute value by using a given dynamic variable pointer (dock widget broadcast, excludes the emitting dock widget)
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
		*    Set dock widgets attribute to it's default value (dock widget broadcast, excludes the emitting dock widget)
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
		*    Call dock widgets method with given dynamic parameters (dock widget broadcast, excludes the emitting dock widget)
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
		*    Call dock widgets method with given constant dynamic parameters (dock widget broadcast, excludes the emitting dock widget)
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
		*    Call dock widgets method with parameters given as string (dock widget broadcast, excludes the emitting dock widget)
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
		*    Call dock widgets method with parameters given as XML element (dock widget broadcast, excludes the emitting dock widget)
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
		*    Set multiple dock widgets attribute values as a string at once (dock widget broadcast, excludes the emitting dock widget)
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
		*    Set dock widgets attribute values from XML (dock widget broadcast, excludes the emitting dock widget)
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
		*    Set all dock widgets attributes to default (dock widget broadcast, excludes the emitting dock widget)
		*
		*  @remarks
		*    Similar to the "PLCore::Object::SetDefaultValues()"-method but iterates over all registered
		*    dock widgets and calls the "SetDefaultValues()"-method of each RTTI dock widget class instance.
		*/
		PLFRONTENDQT_API void SetDockWidgetsDefaultValues();

		//[-------------------------------------------------------]
		//[ Helper                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Adds the given Qt dock widget to the given Qt main window and tabifies it
		*
		*  @param[in] cQMainWindow
		*    Qt main window where to add the given Qt dock widget
		*  @param[in] nQtDockWidgetArea
		*    Dock widget area where to add the given Qt dock widget within the given Qt main window
		*  @param[in] cQDockWidget
		*    Qt dock widget to add to the given Qt main window
		*/
		PLFRONTENDQT_API void AddDockWidgetAndTabify(QMainWindow &cQMainWindow, Qt::DockWidgetArea nQtDockWidgetArea, QDockWidget &cQDockWidget) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		InternalQDockWidget	*m_pQDockWidget;		/**< Qt dock widget, can be a null pointer */
		DockWidgetManager   *m_pDockWidgetManager;	/**< Optional pointer to the dock widget manager this dock widget should be registered to, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLFrontendQt/DockWidget/DockWidget.inl"


#endif // __PLFRONTENDQT_DOCKWIDGET_H__
