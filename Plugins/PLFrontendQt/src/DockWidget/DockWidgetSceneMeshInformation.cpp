/*********************************************************\
 *  File: DockWidgetSceneMeshInformation.cpp             *
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
	#include <QtGui/qdockwidget.h>
	#include <QtGui/qheaderview.h>
	#include <QtGui/qstandarditemmodel.h>
	#include <QtGui/QMainWindow>
PL_WARNING_POP
#include <PLCore/Base/Class.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include <PLMesh/Mesh.h>
#include <PLMesh/MeshHandler.h>
#include <PLMesh/MeshLODLevel.h>
#include <PLMesh/MeshMorphTarget.h>
#include <PLScene/Scene/SceneNode.h>
#include "PLFrontendQt/QtStringAdapter.h"
#include "PLFrontendQt/Widget/QTableViewWithCopy.h"
#include "PLFrontendQt/DockWidget/DockWidgetSceneMeshInformation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
using namespace PLMesh;
using namespace PLScene;
namespace PLFrontendQt {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetSceneMeshInformation, "PLFrontendQt", PLFrontendQt::DockWidgetScene, "Mesh information Qt dock widget class")
	// Properties
	pl_properties
		pl_property("Title", "Mesh Information")
	pl_properties_end
	// Methods
	pl_method_0_metadata(GetSelectedObject,	pl_ret_type(PLCore::Object*),						"Returns the currently selected object, can be a null pointer.",	"")
	pl_method_1_metadata(SelectObject,		pl_ret_type(void),				PLCore::Object*,	"Selects the given object. Object to select as first parameter.",	"")
	// Constructors
	pl_constructor_2_metadata(DefaultConstructor,	QMainWindow*,	PLFrontendQt::DockWidgetManager*,	"Constructor with a pointer to the Qt main window as first parameter, pointer to the dock widget manager this dock widget should be registered to as second parameter",	"")
	// Slots
	pl_slot_0_metadata(OnDestroyed,	"Called when the object assigned with this dock widget was destroyed",	"")
pl_class_metadata_end(DockWidgetSceneMeshInformation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetSceneMeshInformation::DockWidgetSceneMeshInformation(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager) : DockWidgetScene(pQMainWindow, pDockWidgetManager),
	SlotOnDestroyed(this),
	m_pObject(nullptr),
	m_pQTableView(nullptr)
{
	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget) {
		// Set window title
		pQDockWidget->setWindowTitle(pQDockWidget->tr(GetClass()->GetProperties().Get("Title")));

		// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
		AddDockWidgetAndTabify(*pQMainWindow, Qt::BottomDockWidgetArea, *pQDockWidget);

		{ // Ask the RTTI dock widget fellows whether or not someone knows which is the currently selected object
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
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetSceneMeshInformation::~DockWidgetSceneMeshInformation()
{
}

/**
*  @brief
*    Returns the currently selected object
*/
Object *DockWidgetSceneMeshInformation::GetSelectedObject() const
{
	return m_pObject;
}

/**
*  @brief
*    Selects the given object
*/
void DockWidgetSceneMeshInformation::SelectObject(Object *pObject)
{
	// State change?
	if (m_pObject != pObject) {
		// Disconnect event handler
		if (m_pObject) {
			m_pObject->SignalDestroyed.Disconnect(SlotOnDestroyed);
			m_pObject = nullptr;
		}

		// We're only interested in scene nodes
		if (pObject && pObject->IsInstanceOf("PLScene::SceneNode")) {
			// Backup the given object pointer
			m_pObject = pObject;

			// Connect event handler
			if (m_pObject)
				m_pObject->SignalDestroyed.Connect(SlotOnDestroyed);

			// Update the mesh information
			UpdateObject(GetSelectedObject());
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the object assigned with this dock widget was destroyed
*/
void DockWidgetSceneMeshInformation::OnDestroyed()
{
	// Argh! Mayday! We lost our object!
	// -> Now no object is currently selected
	SelectObject(nullptr);
}

/**
*  @brief
*    Updates the mesh information
*/
void DockWidgetSceneMeshInformation::UpdateObject(Object *pObject)
{
	// Create Qt tree view instance right now?
	if (!m_pQTableView) {
		// Get encapsulated Qt dock widget
		QDockWidget *pQDockWidget = GetQDockWidget();
		if (pQDockWidget) {
			// Create table view
			m_pQTableView = new PLFrontendQt::QTableViewWithCopy();
			m_pQTableView->setEditTriggers(QTableView::NoEditTriggers);	// Not ediable
			m_pQTableView->verticalHeader()->hide();	// Hide column with row numbers
			pQDockWidget->setWidget(m_pQTableView);
		}
	}

	// Update Qt tree view instance
	if (m_pQTableView) {
		// Create standard item model
		QStandardItemModel *pQStandardItemModel = new QStandardItemModel();
		pQStandardItemModel->setHorizontalHeaderItem(0, new QStandardItem(m_pQTableView->tr("Name")));
		pQStandardItemModel->setHorizontalHeaderItem(1, new QStandardItem(m_pQTableView->tr("Value")));

		// Fill the standard item model
		if (pObject)
			FillStandardItemModel(*pQStandardItemModel, static_cast<SceneNode&>(*pObject));	// If we're in here, we know the cast is valid

		// Set model
		m_pQTableView->setModel(pQStandardItemModel);

		// Usability: Resize the columns to the size of its contents
		// [HACK](Qt 4.8.0) In we're not making the Qt table view invisible before calling "resizeColumnsToContents()" the size is not the expected one
		m_pQTableView->setVisible(false);
		m_pQTableView->resizeColumnsToContents();
		m_pQTableView->setVisible(true);
	}
}

/**
*  @brief
*    Fills the given Qt standard item model
*/
void DockWidgetSceneMeshInformation::FillStandardItemModel(QStandardItemModel &cQStandardItemModel, SceneNode &cSceneNode) const
{
	// Get mesh handler
	MeshHandler *pMeshHandler = cSceneNode.GetMeshHandler();
	if (pMeshHandler) {
		// Get the used mesh rsource
		Mesh *pMesh = pMeshHandler->GetResource();
		if (pMesh) {
			// Fill the given Qt standard item model, mesh part
			FillStandardItemModelMesh(cQStandardItemModel, *pMesh);
		}
	}
}

/**
*  @brief
*    Fills the given Qt standard item model, mesh part
*/
void DockWidgetSceneMeshInformation::FillStandardItemModelMesh(QStandardItemModel &cQStandardItemModel, Mesh &cMesh) const
{
	int nCurrentRow = cQStandardItemModel.rowCount();

	{ // Mesh filename
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Filename")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QtStringAdapter::PLToQt(cMesh.GetFilename())));
		nCurrentRow++;
	}

	{ // Checks all LODs and returns the maximum number of vertices
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of Vertices")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cMesh.GetMaxNumOfVertices())));
		nCurrentRow++;
	}

	{ // Checks all LODs and returns the maximum number of geometries
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of Geometries")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cMesh.GetMaxNumOfGeometries())));
		nCurrentRow++;
	}

	{ // Checks all LODs and returns the maximum number of triangles
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of Triangles")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cMesh.GetMaxNumOfTriangles())));
		nCurrentRow++;
	}

	{ // The number of morph targets
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of Morph Targets")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cMesh.GetNumOfMorphTargets())));
		nCurrentRow++;
	}

	{ // The number of LOD levels
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of LOD Levels")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cMesh.GetNumOfLODLevels())));
		nCurrentRow++;
	}

	{ // The number of materials
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of Materials")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cMesh.GetNumOfMaterials())));
		nCurrentRow++;
	}

	// Get the base morph target
	MeshMorphTarget *pMeshMorphTarget = cMesh.GetMorphTarget(0);
	if (pMeshMorphTarget) {
		// Fill the given Qt standard item model, mesh morph target part
		FillStandardItemModelMeshMorphTarget(cQStandardItemModel, *pMeshMorphTarget);
	}

	// Get the base LOD level
	MeshLODLevel *pMeshLODLevel = cMesh.GetLODLevel(0);
	if (pMeshLODLevel) {
		// Fill the given Qt standard item model, mesh LOD level part
		FillStandardItemModelMeshLODLevel(cQStandardItemModel, *pMeshLODLevel);
	}
}

/**
*  @brief
*    Fills the given Qt standard item model, mesh morph target part
*/
void DockWidgetSceneMeshInformation::FillStandardItemModelMeshMorphTarget(QStandardItemModel &cQStandardItemModel, MeshMorphTarget &cMeshMorphTarget) const
{
	// Get the vertex buffer
	VertexBuffer *pVertexBuffer = cMeshMorphTarget.GetVertexBuffer();
	if (pVertexBuffer) {
		// Fill the given Qt standard item model, vertex buffer part
		FillStandardItemModelVertexBuffer(cQStandardItemModel, *pVertexBuffer);
	}
}

/**
*  @brief
*    Fills the given Qt standard item model, vertex buffer part
*/
void DockWidgetSceneMeshInformation::FillStandardItemModelVertexBuffer(QStandardItemModel &cQStandardItemModel, VertexBuffer &cVertexBuffer) const
{
	int nCurrentRow = cQStandardItemModel.rowCount();

	{ // The number of buffer elements
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("VBO: Number of Buffer Elements")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cVertexBuffer.GetNumOfElements())));
		nCurrentRow++;
	}

	{ // The buffer size (in bytes)
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("VBO: Buffer Size")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes = %1 KiB = %2 MiB").arg(cVertexBuffer.GetSize()).arg(cVertexBuffer.GetSize()/1024).arg(cVertexBuffer.GetSize()/1024/1024)));
		nCurrentRow++;
	}

	{ // The vertex size (in bytes)
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("VBO: Vertex Size")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes").arg(cVertexBuffer.GetVertexSize())));
		nCurrentRow++;
	}

	{ // The number of vertex attributes
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("VBO: Number of Vertex Attributes")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cVertexBuffer.GetNumOfVertexAttributes())));
		nCurrentRow++;
	}

	// Loop though all vertex attributes
	for (uint32 i=0; i<cVertexBuffer.GetNumOfVertexAttributes(); i++) {
		// Get the current vertex attribute
		const VertexBuffer::Attribute &sAttribute = *cVertexBuffer.GetVertexAttribute(i);

		{ // Semantic
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(QString("VBO Attribute %0 Semantic").arg(i)));
			QString sQStringType;
			switch (sAttribute.nSemantic) {
				// Position data (1 channel)
				case VertexBuffer::Position:
					sQStringType = "Position";
					break;

				// Blend weight data (1 channel)
				case VertexBuffer::BlendWeight:
					sQStringType = "Blend Weight";
					break;

				// Normal data (1 channel, only Float3/Half3 allowed due to restrictions of legacy APIs!)
				case VertexBuffer::Normal:
					sQStringType = "Normal";
					break;

				// Color data (2 channels, only RGBA allowed, second channel requires Capabilities::bVertexBufferSecondaryColor)
				case VertexBuffer::Color:
					sQStringType = "Color";
					break;

				// Fog coordinate data (1 channel, only Float1 allowed, requires FixedFunctions::Capabilities::bVertexBufferFogCoord)
				case VertexBuffer::FogCoord:
					sQStringType = "Fog Coordinate";
					break;

				// Point sprite size data (1 channel, only Float1 allowed! Known as "PSize", but this name may conflict with OS definitions)
				case VertexBuffer::PointSize:
					sQStringType = "Point Size";
					break;

				// Blend indices data (1 channel)
				case VertexBuffer::BlendIndices:
					sQStringType = "Blend Indices";
					break;

				// Texture coordinate data (n channels)
				case VertexBuffer::TexCoord:
					sQStringType = "Texture Coordinate";
					break;

				// Tangent data (1 channel, only Float3/Half3 allowed due to restrictions of legacy APIs!)
				case VertexBuffer::Tangent:
					sQStringType = "Tangent";
					break;

				// Binormal (also referred to as bitangent) data (1 channel, only Float3/Half3 allowed due to restrictions of legacy APIs!)
				case VertexBuffer::Binormal:
					sQStringType = "Binormal";
					break;
			}
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(sQStringType));
			nCurrentRow++;
		}

		{ // Pipeline channel (see ESemantic)
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(QString("VBO Attribute %0 Channel").arg(i)));
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(sAttribute.nChannel)));
			nCurrentRow++;
		}

		{ // Type: Any member of the EType enumeration type
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(QString("VBO Attribute %0 Type").arg(i)));
			QString sQStringType;
			switch (sAttribute.nType) {
				// Color (legacy API dependent storage which is no longer required when using modern shader based API's, do always use GetColor() and SetColor()!)
				case VertexBuffer::RGBA:
					sQStringType = "RGBA";
					break;

				// Float 1 (one component per element, 32 bit floating point per component)
				case VertexBuffer::Float1:
					sQStringType = "Float1";
					break;

				// Float 2 (two components per element, 32 bit floating point per component)
				case VertexBuffer::Float2:
					sQStringType = "Float2";
					break;

				// Float 3 (three components per element, 32 bit floating point per component)
				case VertexBuffer::Float3:
					sQStringType = "Float3";
					break;

				// Float 4 (four components per element, 32 bit floating point per component)
				case VertexBuffer::Float4:
					sQStringType = "Float4";
					break;

				// Short 2 (two components per element, 16 bit integer per component)
				case VertexBuffer::Short2:
					sQStringType = "Short2";
					break;

				// Short 4 (four components per element, 16 bit integer per component)
				case VertexBuffer::Short4:
					sQStringType = "Short4";
					break;

				// Half 1 (one component per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it)
				case VertexBuffer::Half1:
					sQStringType = "Half1";
					break;

				// Half 2 (two components per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it)
				case VertexBuffer::Half2:
					sQStringType = "Half2";
					break;

				// Half 3 (three components per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it)
				case VertexBuffer::Half3:
					sQStringType = "Half3";
					break;

				// Half 4 (four components per element, 16 bit floating point per component, may not be supported by each API, be careful with this data type because not every GPU driver is optimized for it)
				case VertexBuffer::Half4:
					sQStringType = "Half4";
					break;

			}
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(sQStringType));
			nCurrentRow++;
		}

		{ // Offset (in bytes) of the vertex attribute
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(QString("VBO Attribute %0 Offset").arg(i)));
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes").arg(sAttribute.nOffset)));
			nCurrentRow++;
		}

		{ // API dependent: Size (in bytes) of the vertex attribute
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(QString("VBO Attribute %0 Size").arg(i)));
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes").arg(sAttribute.nSizeAPI)));
			nCurrentRow++;
		}

		{ // API dependent: Number of vertex type components
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(QString("VBO Attribute %0 Compoents").arg(i)));
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(sAttribute.nComponentsAPI)));
			nCurrentRow++;
		}
	}
}

/**
*  @brief
*    Fills the given Qt standard item model, mesh LOD level part
*/
void DockWidgetSceneMeshInformation::FillStandardItemModelMeshLODLevel(QStandardItemModel &cQStandardItemModel, MeshLODLevel &cMeshLODLevel) const
{
	// Get the index buffer
	IndexBuffer *pIndexBuffer = cMeshLODLevel.GetIndexBuffer();
	if (pIndexBuffer) {
		// Fill the given Qt standard item model, index buffer part
		FillStandardItemModelIndexBuffer(cQStandardItemModel, *pIndexBuffer);
	}
}

/**
*  @brief
*    Fills the given Qt standard item model, index buffer part
*/
void DockWidgetSceneMeshInformation::FillStandardItemModelIndexBuffer(QStandardItemModel &cQStandardItemModel, IndexBuffer &cIndexBuffer) const
{
	int nCurrentRow = cQStandardItemModel.rowCount();

	{ // The number of buffer elements
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("IBO: Number of Buffer Elements")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cIndexBuffer.GetNumOfElements())));
		nCurrentRow++;
	}

	{ // The buffer size (in bytes)
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("IBO: Buffer Size")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes = %1 KiB = %2 MiB").arg(cIndexBuffer.GetSize()).arg(cIndexBuffer.GetSize()/1024).arg(cIndexBuffer.GetSize()/1024/1024)));
		nCurrentRow++;
	}

	{ // The type of the buffer elements
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("IBO: Type of Buffer Elements")));
		QString sQStringType;
		switch (cIndexBuffer.GetElementType()) {
			// Unsigned int (may not be supported by each API)
			case IndexBuffer::UInt:
				sQStringType = "UInt (32 bit)";
				break;

			// Unsigned short (default)
			case IndexBuffer::UShort:
				sQStringType = "UShort (16 bit)";
				break;

			// Unsigned byte (may not be supported by each API)
			case IndexBuffer::UByte:
				sQStringType = "UByte (8 bit)";
				break;
		}
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(sQStringType));
		nCurrentRow++;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendQt
