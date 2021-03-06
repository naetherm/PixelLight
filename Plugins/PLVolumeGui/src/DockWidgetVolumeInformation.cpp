/*********************************************************\
 *  File: DockWidgetVolumeInformation.cpp                *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/Renderer/TextureBuffer.h>
#include <PLFrontendQt/Widget/QTableViewWithCopy.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeGui/DockWidgetVolumeInformation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGraphics;
using namespace PLFrontendQt;
namespace PLVolumeGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(DockWidgetVolumeInformation, "PLVolumeGui", PLVolumeGui::DockWidgetVolume, "Volume information Qt dock widget class")
	// Properties
	pl_properties
		pl_property("Title", "Volume Information")
	pl_properties_end
	// Constructors
	pl_constructor_2_metadata(DefaultConstructor,	QMainWindow*,	PLFrontendQt::DockWidgetManager*,	"Constructor with a pointer to the Qt main window as first parameter, pointer to the dock widget manager this dock widget should be registered to as second parameter",	"")
pl_class_metadata_end(DockWidgetVolumeInformation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DockWidgetVolumeInformation::DockWidgetVolumeInformation(QMainWindow *pQMainWindow, DockWidgetManager *pDockWidgetManager) : DockWidgetVolume(pQMainWindow, pDockWidgetManager),
	m_pQTableView(nullptr)
{
	// Get encapsulated Qt dock widget
	QDockWidget *pQDockWidget = GetQDockWidget();
	if (pQDockWidget) {
		// Set window title
		pQDockWidget->setWindowTitle(pQDockWidget->tr(GetClass()->GetProperties().Get("Title")));

		// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
		AddDockWidgetAndTabify(*pQMainWindow, Qt::BottomDockWidgetArea, *pQDockWidget);

		// Update the volume information
		UpdateObject(GetSelectedObject());
	}
}

/**
*  @brief
*    Destructor
*/
DockWidgetVolumeInformation::~DockWidgetVolumeInformation()
{
}


//[-------------------------------------------------------]
//[ Public virtual DockWidgetVolume functions             ]
//[-------------------------------------------------------]
void DockWidgetVolumeInformation::SelectObject(Object *pObject)
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
*    Updates the volume information
*/
void DockWidgetVolumeInformation::UpdateObject(PLCore::Object *pObject)
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
			FillStandardItemModel(*pQStandardItemModel, static_cast<PLVolume::SNVolume&>(*pObject));	// If we're in here, we know the cast is valid

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
void DockWidgetVolumeInformation::FillStandardItemModel(QStandardItemModel &cQStandardItemModel, const PLVolume::SNVolume &cSNVolume) const
{
	// Get the used volume resource
	PLVolume::Volume *pVolume = cSNVolume.GetVolume();
	if (pVolume) {
		int nCurrentRow = 0;

		// Get the image holding the volumetric data
		Image cVolumeImage;
		{
			// In here, we're interested in all data we can get, so, try to use the data used by the GPU for rendering
			// -> Get the renderer texture buffer holding the 3D voxel data
			PLRenderer::TextureBuffer *pTextureBuffer = pVolume->GetVolumeTextureBuffer();
			if (pTextureBuffer) {
				// -> We may get an memory issue in here using huge datasets within a 32 bit build
				// -> In case we're receiving compressed data, the final histogram will not be completely identical to
				//    a histogram of the original uncompressed data (which we don't have when we're in here, so we can't do anything about it)
				pTextureBuffer->CopyDataToImage(cVolumeImage);
			}
			if (!cVolumeImage.GetNumOfParts()) {
				// Get the image holding the volumetric data (internal image data is not copied, just shared, so this is pretty lightweight)
				cVolumeImage = pVolume->GetVolumeImage();
			}
		}

		{ // The size ("voxel size" * "size of the volume image")
			// Get the image buffer
			ImageBuffer *pImageBuffer = cVolumeImage.GetBuffer();
			if (pImageBuffer) {
				const PLMath::Vector3  &vVoxelSize = pVolume->GetVoxelSize();
				const PLMath::Vector3i &vImageSize = pImageBuffer->GetSize();
				cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Size")));
				cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 x %1 x %2 (\"voxel size\" * \"size of the volume image\")").arg(vVoxelSize.x*vImageSize.x).arg(vVoxelSize.y*vImageSize.y).arg(vVoxelSize.z*vImageSize.z)));
				nCurrentRow++;
			}
		}

		{ // The size of one voxel
			const PLMath::Vector3 &vVoxelSize = pVolume->GetVoxelSize();
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Voxel Size")));
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 x %1 x %2 (without metric, but usually one unit is equal to one meter)").arg(vVoxelSize.x).arg(vVoxelSize.y).arg(vVoxelSize.z)));
			nCurrentRow++;
		}

		// Get the image buffer
		ImageBuffer *pImageBuffer = cVolumeImage.GetBuffer();
		if (pImageBuffer) {
			// Fill the given Qt standard item model, image buffer part
			FillStandardItemModelImageBuffer(cQStandardItemModel, *pImageBuffer);
		}

		// Get the first image part (we ignore cube-maps in here because this would be quite unusual for 3D textures)
		ImagePart *pImagePart = cVolumeImage.GetPart(0);
		if (pImagePart) {
			const PLCore::uint32 nNumOfMipmaps = pImagePart->GetNumOfMipmaps();

			// Number of mipmaps
			nCurrentRow = cQStandardItemModel.rowCount();
			cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of Mipmaps")));
			cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 (Including Base-Map)").arg(nNumOfMipmaps)));
			nCurrentRow++;

			{ // Total size of image data in bytes (including mipmaps)
				const PLCore::uint32 nDataSize = pImagePart->GetDataSize();
				cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Total Size of Data")));
				cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes = %1 KiB = %2 MiB (Including Mipmaps)").arg(nDataSize).arg(nDataSize/1024).arg(nDataSize/1024/1024)));
				nCurrentRow++;
			}

			{ // Total size of compressed image data in bytes (including mipmaps)
				const PLCore::uint32 nCompressedDataSize = pImagePart->GetCompressedDataSize();
				cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Total Size of Compressed Data")));
				cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes = %1 KiB = %2 MiB (Including Mipmaps)").arg(nCompressedDataSize).arg(nCompressedDataSize/1024).arg(nCompressedDataSize/1024/1024)));
				nCurrentRow++;
			}
		}
	}
}

/**
*  @brief
*    Fills the given Qt standard item model, image buffer part
*/
void DockWidgetVolumeInformation::FillStandardItemModelImageBuffer(QStandardItemModel &cQStandardItemModel, ImageBuffer &cImageBuffer) const
{
	int nCurrentRow = cQStandardItemModel.rowCount();

	{ // The size of the volume image
		const PLMath::Vector3i &vSize = cImageBuffer.GetSize();
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Number of Voxels")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 x %1 x %2 (%3 voxels)").arg(vSize.x).arg(vSize.y).arg(vSize.z).arg(cImageBuffer.GetNumOfVoxels())));
		nCurrentRow++;
	}

	{ // Data format
		QString sDataFormat;
		switch (cImageBuffer.GetDataFormat()) {
			case DataByte:   sDataFormat = "Byte (8 bit)";			break;
			case DataWord:   sDataFormat = "Word (16 bit)";			break;
			case DataHalf:   sDataFormat = "Half (16 bit float)";	break;
			case DataFloat:  sDataFormat = "IEEE float (32 bit)";	break;
			case DataDouble: sDataFormat = "Double (64 bit)";		break;
		}
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Data Format")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(sDataFormat));
		nCurrentRow++;
	}

	{ // Color format
		QString sColorFormat;
		switch (cImageBuffer.GetColorFormat()) {
			case ColorRGB:        sColorFormat = "RGB";							break;
			case ColorRGBA:       sColorFormat = "RGBA";						break;
			case ColorBGR:        sColorFormat = "BGR";							break;
			case ColorBGRA:       sColorFormat = "BGRA";						break;
			case ColorGrayscale:  sColorFormat = "Gray Scale Image";			break;
			case ColorGrayscaleA: sColorFormat = "Gray Scale Image with Alpha";	break;
			case ColorPalette:    sColorFormat = "Color Palette";				break;
		}
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Color Format")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(sColorFormat));
		nCurrentRow++;
	}

	{ // Number of components per pixel
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Components per Pixel")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0").arg(cImageBuffer.GetComponentsPerPixel())));
		nCurrentRow++;
	}

	{ // Number of bytes per pixel component
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Bytes per Pixel Component")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes").arg(cImageBuffer.GetBytesPerPixelComponent())));
		nCurrentRow++;
	}

	{ // Number of bytes per pixel
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Bytes per Pixel")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes").arg(cImageBuffer.GetBytesPerPixel())));
		nCurrentRow++;
	}

	{ // Size of image data in bytes (base-map without mipmaps)
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Size of Data")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes = %1 KiB = %2 MiB (Base-Map without Mipmaps)").arg(cImageBuffer.GetDataSize()).arg(cImageBuffer.GetDataSize()/1024).arg(cImageBuffer.GetDataSize()/1024/1024)));
		nCurrentRow++;
	}

	{ // Compression
		QString sCompression;
		switch (cImageBuffer.GetCompression()) {
			case CompressionNone:  sCompression = "None";	break;	// No compression
			case CompressionDXT1:  sCompression = "DXT1";	break;	// DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block)
			case CompressionDXT3:  sCompression = "DXT3";	break;	// DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			case CompressionDXT5:  sCompression = "DXT5";	break;	// DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block)
			case CompressionLATC1: sCompression = "LATC1";	break;	// 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block)
			case CompressionLATC2: sCompression = "LATC2";	break;	// 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block)
		}
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Compression")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(sCompression));
		nCurrentRow++;
	}

	{ // Size of compressed image data in bytes (base-map without mipmaps)
		cQStandardItemModel.setItem(nCurrentRow, 0, new QStandardItem(cQStandardItemModel.tr("Size of Compressed Data")));
		cQStandardItemModel.setItem(nCurrentRow, 1, new QStandardItem(QString("%0 bytes = %1 KiB = %2 MiB (Base-Map without Mipmaps)").arg(cImageBuffer.GetCompressedDataSize()).arg(cImageBuffer.GetCompressedDataSize()/1024).arg(cImageBuffer.GetCompressedDataSize()/1024/1024)));
		nCurrentRow++;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeGui
