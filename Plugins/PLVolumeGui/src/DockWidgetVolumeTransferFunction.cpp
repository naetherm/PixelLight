/*********************************************************\
 *  File: DockWidgetVolumeTransferFunction.cpp           *
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
#include <PLCore/PLCore.h>
PL_WARNING_PUSH
	PL_WARNING_DISABLE(4127)	// "warning C4127: conditional expression is constant"
	#include <QtGui/qboxlayout.h>
	#include <QtGui/qdockwidget.h>
	#include <QtGui/QMainWindow>
PL_WARNING_POP
#include <PLCore/Base/Class.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNVolume.h>
#include <PLVolume/TransferFunctionControl/TransferFunctionControl.h>
#include "PLVolumeGui/TransferFunctionWidget.h"
#include "PLVolumeGui/DockWidgetVolumeTransferFunctionQObject.h"
#include "PLVolumeGui/DockWidgetVolumeTransferFunction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLFrontendQt;
namespace PLVolumeGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetVolumeTransferFunction, "PLVolumeGui", PLVolumeGui::DockWidgetVolume, "Volume transfer function Qt dock widget class")
	// Properties
	pl_properties
		pl_property("Title", "Transfer Function")
	pl_properties_end
	// Constructors
	pl_constructor_2_metadata(DefaultConstructor,	QMainWindow*,	PLFrontendQt::DockWidgetManager*,	"Constructor with a pointer to the Qt main window as first parameter, pointer to the dock widget manager this dock widget should be registered to as second parameter",	"")
pl_class_metadata_end(DockWidgetVolumeTransferFunction)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetVolumeTransferFunction::DockWidgetVolumeTransferFunction(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager) : DockWidgetVolume(pQMainWindow, pDockWidgetManager),
	m_pDockWidgetVolumeTransferFunctionQObject(new DockWidgetVolumeTransferFunctionQObject(*this)),
	m_pTransferFunctionWidget(nullptr),
	m_pTransferFunctionResultWidget(nullptr)
{
	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget) {
		// Set window title
		pQDockWidget->setWindowTitle(pQDockWidget->tr(GetClass()->GetProperties().Get("Title")));

		// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
		AddDockWidgetAndTabify(*pQMainWindow, Qt::LeftDockWidgetArea, *pQDockWidget);

		// The Qt object should receive events from the encapsulated Qt dock widget
		pQDockWidget->installEventFilter(m_pDockWidgetVolumeTransferFunctionQObject);

		// Update the volume information
		UpdateObject(GetSelectedObject());
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetVolumeTransferFunction::~DockWidgetVolumeTransferFunction()
{
	// Destroy the QObject instance for Qt's signal/slot mechanisms
	delete m_pDockWidgetVolumeTransferFunctionQObject;

	// Free the internal data
	UpdateObject(nullptr);
}


//[-------------------------------------------------------]
//[ Public virtual DockWidgetVolume functions             ]
//[-------------------------------------------------------]
void DockWidgetVolumeTransferFunction::SelectObject(Object *pObject)
{
	// State change?
	if (GetSelectedObject() != pObject) {
		// Call base implementation
		DockWidgetVolume::SelectObject(pObject);

		// Update the volume information
		UpdateObject(GetSelectedObject());
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Updates the transfer function
*/
void DockWidgetVolumeTransferFunction::UpdateObject(PLCore::Object *pObject)
{
	// Destroy previous transfer function Qt widget
	if (m_pTransferFunctionWidget) {
		delete m_pTransferFunctionWidget;
		m_pTransferFunctionWidget = nullptr;
	}
	if (m_pTransferFunctionResultWidget) {
		delete m_pTransferFunctionResultWidget;
		m_pTransferFunctionResultWidget = nullptr;
	}

	// Is there an selected object?
	if (pObject) {
		// Get encapsulated Qt dock widget
		QDockWidget *pQDockWidget = GetQDockWidget();
		if (pQDockWidget) {
			// Get the used volume resource
			PLVolume::Volume *pVolume = static_cast<PLVolume::SNVolume&>(*pObject).GetVolume();	// If we're in here, we know the cast is valid
			if (pVolume) {
				// Transfer function result widget
				m_pTransferFunctionResultWidget = new QLabel();
				m_pTransferFunctionResultWidget->setAlignment(Qt::AlignTop | Qt::AlignLeft);
				m_pTransferFunctionResultWidget->setToolTip(pQDockWidget->tr("Resulting 1D transfer function"));

				// Create transfer function Qt widget
				m_pTransferFunctionWidget = new TransferFunctionWidget(*this, *pVolume);
				QLayout *pQLayout = new QVBoxLayout();
				pQLayout->setSpacing(0);
				pQLayout->setMargin(0);
				pQLayout->setContentsMargins(0, 0, 0, 0);
				pQLayout->addWidget(m_pTransferFunctionWidget);
				pQLayout->addWidget(m_pTransferFunctionResultWidget);
				QWidget *pQWidget = new QWidget();
				pQWidget->setLayout(pQLayout);
				pQDockWidget->setWidget(pQWidget);
			}
		}
	}
}

/**
*  @brief
*    Updates the transfer function result
*/
void DockWidgetVolumeTransferFunction::UpdateTransferFunctionResult()
{
	Object *pObject = GetSelectedObject();
	if (m_pTransferFunctionResultWidget && pObject) {
		// Get the used volume resource
		PLVolume::Volume *pVolume = static_cast<PLVolume::SNVolume&>(*pObject).GetVolume();	// If we're in here, we know the cast is valid
		if (pVolume) {
			// Create QPixmap
			QPixmap cQPixmap;

			// Get the image buffer
			PLGraphics::ImageBuffer *pImageBuffer = pVolume->GetTransferFunctionControl().GetImage().GetBuffer();
			if (pImageBuffer) {
				const PLCore::uint8 *pnData = pImageBuffer->GetData();
				QImage cQImage(static_cast<const uchar*>(pnData), pImageBuffer->GetSize().width, pImageBuffer->GetSize().height, QImage::Format_ARGB32);
				cQPixmap = QPixmap::fromImage(cQImage.scaled(256, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).rgbSwapped());
			}

			// Set Qt pixmap
			m_pTransferFunctionResultWidget->setPixmap(cQPixmap);
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeGui
