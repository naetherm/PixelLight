/*********************************************************\
 *  File: TransferFunctionControl.cpp                    *
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
#include <PLMath/Math.h>
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color4.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLVolume/TransferFunctionControl/ImageTools.h"
#include "PLVolume/TransferFunctionControl/TransferFunctionChannel.h"
#include "PLVolume/TransferFunctionControl/TransferFunctionControl.h"


// [TODO] Currently channels are fixed to RGBA, make the implementation more universal/robust


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLVolume {


//[-------------------------------------------------------]
//[ Private static data                                   ]
//[-------------------------------------------------------]
const int TransferFunctionControl::MinIsoValueDistance   = 2;
const int TransferFunctionControl::CircleRadiusDraw      = 4;
const int TransferFunctionControl::CircleRadiusSelection = TransferFunctionControl::CircleRadiusDraw*3;	// Make selection easier


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TransferFunctionControl::TransferFunctionControl() :
	m_bImageDirty(true)
{
}

/**
*  @brief
*    Destructor
*/
TransferFunctionControl::~TransferFunctionControl()
{
	// Remove all transfer function channels
	RemoveAllChannels();
}

/**
*  @brief
*    Adds a transfer function channel
*/
TransferFunctionChannel &TransferFunctionControl::AddChannel()
{
	// Cached image is now considered to be dirty
	m_bImageDirty = true;

	// Create transfer function channel instance
	TransferFunctionChannel *pTransferFunctionChannel = new TransferFunctionChannel(*this);

	// Add it to the internal channel list
	m_lstTransferControlChannels.Add(pTransferFunctionChannel);

	// Return a reference to the created transfer function channel instance
	return *pTransferFunctionChannel;
}

/**
*  @brief
*    Removes all transfer function channels
*/
void TransferFunctionControl::RemoveAllChannels()
{
	// Destroy all channel instances
	for (uint32 i=0; i<m_lstTransferControlChannels.GetNumOfElements(); i++)
		delete m_lstTransferControlChannels[i];

	// Clear the internal channel list
	m_lstTransferControlChannels.Clear();

	// Cached image is now considered to be dirty
	m_bImageDirty = true;
}

/**
*  @brief
*    Sets the default transfer function configuration
*/
void TransferFunctionControl::SetDefaultConfiguration()
{
	// Remove all transfer function channels
	RemoveAllChannels();

	{ // Add red-channel
		TransferFunctionChannel &cTransferFunctionChannel = AddChannel();
		cTransferFunctionChannel.SetName("Red Channel");
		cTransferFunctionChannel.SetVisualizationColor(Color3::Red);
		Array<TransferControlPoint> &cTransferControlPoints = cTransferFunctionChannel.GetTransferControlPoints();
		cTransferControlPoints.Add(TransferControlPoint(0,   0.9f));
		cTransferControlPoints.Add(TransferControlPoint(79,  0.9f));
		cTransferControlPoints.Add(TransferControlPoint(83,  1.0f));
		cTransferControlPoints.Add(TransferControlPoint(256, 1.0f));
	}

	{ // Add green-channel
		TransferFunctionChannel &cTransferFunctionChannel = AddChannel();
		cTransferFunctionChannel.SetName("Green Channel");
		cTransferFunctionChannel.SetVisualizationColor(Color3::Green);
		Array<TransferControlPoint> &cTransferControlPoints = cTransferFunctionChannel.GetTransferControlPoints();
		cTransferControlPoints.Add(TransferControlPoint(0,   0.7f));
		cTransferControlPoints.Add(TransferControlPoint(79,  0.7f));
		cTransferControlPoints.Add(TransferControlPoint(83,  1.0f));
		cTransferControlPoints.Add(TransferControlPoint(256, 1.0f));
	}

	{ // Add blue-channel
		TransferFunctionChannel &cTransferFunctionChannel = AddChannel();
		cTransferFunctionChannel.SetName("Blue Channel");
		cTransferFunctionChannel.SetVisualizationColor(Color3::Blue);
		Array<TransferControlPoint> &cTransferControlPoints = cTransferFunctionChannel.GetTransferControlPoints();
		cTransferControlPoints.Add(TransferControlPoint(0,   0.6f));
		cTransferControlPoints.Add(TransferControlPoint(79,  0.6f));
		cTransferControlPoints.Add(TransferControlPoint(83,  0.88f));
		cTransferControlPoints.Add(TransferControlPoint(256, 0.88f));
	}

	{ // Add alpha channel
		TransferFunctionChannel &cTransferFunctionChannel = AddChannel();
		cTransferFunctionChannel.SetName("Alpha/Opacity Channel");
		cTransferFunctionChannel.SetVisualizationColor(Color3::Purple);
		Array<TransferControlPoint> &cTransferControlPoints = cTransferFunctionChannel.GetTransferControlPoints();
		cTransferControlPoints.Add(TransferControlPoint(0,   0.0f));
		cTransferControlPoints.Add(TransferControlPoint(42,  0.0f));
		cTransferControlPoints.Add(TransferControlPoint(59,  0.2f));
		cTransferControlPoints.Add(TransferControlPoint(64,  0.06f));
		cTransferControlPoints.Add(TransferControlPoint(80,  0.0f));
		cTransferControlPoints.Add(TransferControlPoint(83,  0.9f));
		cTransferControlPoints.Add(TransferControlPoint(256, 1.0f));
	}
}

/**
*  @brief
*    Moves a transfer control point and performs the required security checks while doing so
*/
void TransferFunctionControl::MoveTransferControlPoint(uint32 nChannel, uint32 nControlPoint, const Vector2 &vDelta)
{
	// Valid channel?
	if (nChannel < m_lstTransferControlChannels.GetNumOfElements()) {
		// Get control points of the transfer function channel instance
		Array<TransferControlPoint> &cTransferControlPoints = m_lstTransferControlChannels[nChannel]->GetTransferControlPoints();

		// Valid control point?
		if (nControlPoint < cTransferControlPoints.GetNumOfElements()) {
			// Get control point instance
			TransferControlPoint &cTransferControlPoint = cTransferControlPoints[nControlPoint];

			// Updated required?
			bool bUpdate = false;

			// Update iso-value
			// -> The iso-value of the first control point must always be 0 and therefore is now allowed to be changed
			// -> The iso-value of the last control point must always be <number of bins - 1> and therefore is now allowed to be changed
			if (nControlPoint>0 && nControlPoint<cTransferControlPoints.GetNumOfElements()-1) {
				// Get current value
				int nNewIsoValue = cTransferControlPoint.GetIsoValue();

				// Update value
				nNewIsoValue += static_cast<int>(vDelta.x);

				// Security checks: Minimum
				if (nNewIsoValue < 0)
					nNewIsoValue = 0;
				if (nControlPoint > 0 && nNewIsoValue < static_cast<int>(cTransferControlPoints[nControlPoint-1].GetIsoValue())+MinIsoValueDistance)
					nNewIsoValue = cTransferControlPoints[nControlPoint-1].GetIsoValue() + MinIsoValueDistance;

				// Security checks: Maximum
				if (nNewIsoValue > static_cast<int>(GetNumOfBins()))	// No GetNumOfBins()-1
					nNewIsoValue = static_cast<int>(GetNumOfBins());	// No GetNumOfBins()-1
				if (nControlPoint < cTransferControlPoints.GetNumOfElements()-1 && nNewIsoValue > static_cast<int>(cTransferControlPoints[nControlPoint+1].GetIsoValue())-MinIsoValueDistance)
					nNewIsoValue = cTransferControlPoints[nControlPoint+1].GetIsoValue() - MinIsoValueDistance;

				// Set new value, if required
				if (nNewIsoValue != static_cast<int>(cTransferControlPoint.GetIsoValue())) {
					cTransferControlPoint.SetIsoValue(nNewIsoValue);
					bUpdate = true;
				}
			}

			{ // Update value
				// Get current value
				float fNewValue = cTransferControlPoint.GetValue();

				// Update value
				fNewValue += vDelta.y;

				// Security checks: Minimum
				if (fNewValue < 0.0f)
					fNewValue = 0.0f;

				// Security checks: Maximum
				if (fNewValue > 1.0f)
					fNewValue = 1.0f;

				// Set new value, if required
				if (fNewValue != cTransferControlPoint.GetValue()) {
					cTransferControlPoint.SetValue(fNewValue);
					bUpdate = true;
				}
			}

			// Updated required?
			if (bUpdate) {
				// Cached image is now considered to be dirty
				m_bImageDirty = true;
			}
		}
	}
}

/**
*  @brief
*    Adds the final transfer function values to an image representation, bins along the x axis, values along the y-axis
*/
void TransferFunctionControl::AddToImage(Image &cDestinationImage, uint32 nChannel, const Color3 &cColor)
{
	// Get the destination image buffer, must be RGB byte
	ImageBuffer *pDestinationImageBuffer = cDestinationImage.GetBuffer();
	if (pDestinationImageBuffer && pDestinationImageBuffer->GetDataFormat() == DataByte && pDestinationImageBuffer->GetColorFormat() == ColorRGBA) {
		// Get the transfer function image buffer
		const ImageBuffer *pTransferFunctionImageBuffer = GetConstImage().GetBuffer();
		if (pTransferFunctionImageBuffer) {
			// The width of both images must be identical
			const int nWidth = pDestinationImageBuffer->GetSize().x;
			if (nWidth == pTransferFunctionImageBuffer->GetSize().x) {
				// Valid channel?
				if (nChannel < m_lstTransferControlChannels.GetNumOfElements()) {
					// Draw
					int nXStart = 0;
					int nYStart = 0;
					ImageTools cImageTools(*pDestinationImageBuffer);
					const int nMaxY = pDestinationImageBuffer->GetSize().y - 1;
					const uint8 *pTransferfunctionImageData = pTransferFunctionImageBuffer->GetData();
					const uint32 nTransferFunctionBytesPerPixel = pTransferFunctionImageBuffer->GetBytesPerPixel();
					for (int nX=0; nX<nWidth; nX++, pTransferfunctionImageData+=nTransferFunctionBytesPerPixel) {
						// Get y position of the value
						const int nY = static_cast<int>((255 - pTransferfunctionImageData[nChannel])/255.0f*nMaxY);

						// Draw Bresenham line
						cImageTools.DrawBresenhamLine(cColor, nXStart, nYStart, nX, nY);
						nXStart = nX;
						nYStart = nY;
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Adds the control points to an image representation, bins along the x axis, values along the y-axis
*/
void TransferFunctionControl::AddControlPointsToImage(Image &cDestinationImage, uint32 nChannel, const Color3 &cColor, const Color3 &cLineColor, int nSelectedControlPoint)
{
	// Get the destination image buffer, must be RGB byte
	ImageBuffer *pDestinationImageBuffer = cDestinationImage.GetBuffer();
	if (pDestinationImageBuffer && pDestinationImageBuffer->GetDataFormat() == DataByte && pDestinationImageBuffer->GetColorFormat() == ColorRGBA) {
		// Get the transfer function image buffer
		const ImageBuffer *pTransferFunctionImageBuffer = GetConstImage().GetBuffer();
		if (pTransferFunctionImageBuffer) {
			// The width of both images must be identical
			const int nWidth = pDestinationImageBuffer->GetSize().x;
			if (nWidth == pTransferFunctionImageBuffer->GetSize().x) {
				// Valid channel?
				if (nChannel < m_lstTransferControlChannels.GetNumOfElements()) {
					// Get control points of the transfer function channel instance
					Array<TransferControlPoint> &cTransferControlPoints = m_lstTransferControlChannels[nChannel]->GetTransferControlPoints();

					// Draw
					int nXStart = 0;
					int nYStart = 0;
					ImageTools cImageTools(*pDestinationImageBuffer);
					const int nMaxY = pDestinationImageBuffer->GetSize().y - 1;
					const bool bDrawConnectionLines = (cLineColor != Color3::Null);
					for (uint32 i=0; i<cTransferControlPoints.GetNumOfElements(); i++) {
						const TransferControlPoint &cTransferControlPoint = cTransferControlPoints[i];

						// Get y position of the alpha value
						const int nY = static_cast<int>((1.0f - cTransferControlPoint.GetValue())*nMaxY);

						// Draw Bresenham line
						const int nX = cTransferControlPoint.GetIsoValue();
						if (bDrawConnectionLines) {
							cImageTools.DrawBresenhamLine(cLineColor, nXStart, nYStart, nX, nY);
							nXStart = nX;
							nYStart = nY;
						}

						// Draw Bresenham circle
						if (nSelectedControlPoint == static_cast<int>(i)) {
							cImageTools.DrawBresenhamCircle(Color3::Black, nX, nY, CircleRadiusDraw-2);
							cImageTools.DrawBresenhamCircle(Color3::White, nX, nY, CircleRadiusDraw-1);
							cImageTools.DrawBresenhamCircle(Color3::White, nX, nY, CircleRadiusDraw+1);
							cImageTools.DrawBresenhamCircle(Color3::Black, nX, nY, CircleRadiusDraw+2);
						}
						cImageTools.DrawBresenhamCircle(cColor, nX, nY, CircleRadiusDraw);
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Finds the control point closest to a given coordinate, bins along the x axis, values along the y-axis
*/
int TransferFunctionControl::GetControlPointOnImage(PLCore::uint32 nChannel, uint32 nHeight, int nX, int nY, float *pfDistance) const
{
	int nControlPoint = -1;
	float fClosestDistance = FLT_MAX;

	// Valid channel?
	if (nChannel < m_lstTransferControlChannels.GetNumOfElements()) {
		// Get control points of the transfer function channel instance
		Array<TransferControlPoint> &cTransferControlPoints = m_lstTransferControlChannels[nChannel]->GetTransferControlPoints();

		// Loop through all control points
		const Vector2 vCoordinate(static_cast<float>(nX), static_cast<float>(nY));
		for (uint32 i=0; i<cTransferControlPoints.GetNumOfElements(); i++) {
			const TransferControlPoint &cTransferControlPoint = cTransferControlPoints[i];

			// Get current x position (= iso-value)
			const int nCurrentX = cTransferControlPoint.GetIsoValue();

			// Get current y position of the value
			const int nCurrentY = static_cast<int>((1.0f - cTransferControlPoint.GetValue())*nHeight);

			// Calculate the distance
			const float fDistance = (Vector2(static_cast<float>(nCurrentX), static_cast<float>(nCurrentY)) - vCoordinate).GetLength();

			// Closer?
			if (fDistance <= CircleRadiusSelection && fClosestDistance > fDistance) {
				fClosestDistance = fDistance;
				nControlPoint = i;
			}
		}
	}

	// Done
	if (pfDistance)
		*pfDistance = fClosestDistance;
	return nControlPoint;
}


//[-------------------------------------------------------]
//[ Public virtual TransferFunction functions             ]
//[-------------------------------------------------------]
Image &TransferFunctionControl::GetImage()
{
	// Is the cached image considered to be dirty?
	if (m_bImageDirty) {
		// Are there any transfer function channels?
		if (m_lstTransferControlChannels.GetNumOfElements()) {
			// Call the base implementation in order to get the image instance
			Image &cImage = TransferFunction::GetImage();

			// [TODO] Add optimization: Do only create a new image in case there's no way around it
			cImage.Unload();

			// [TODO] Flexible number of bins
			// Get the number of bins
			const uint32 nNumOfBins = 256;

			// Create image buffer
			ImageBuffer *pImageBuffer = cImage.CreatePart()->CreateMipmap();
			pImageBuffer->CreateImage(DataByte, ColorRGBA, Vector3i(nNumOfBins, 1, 1));

			// Loop through all channels
			Color4 TransferFunc[256];
			for (uint32 nChannel=0; nChannel<m_lstTransferControlChannels.GetNumOfElements(); nChannel++) {
				// Get the transfer function channel instance
				TransferFunctionChannel &cTransferFunctionChannel = *m_lstTransferControlChannels[nChannel];

				// Get control points of the transfer function channel instance
				uint32 numTF = 0;
				const Array<TransferControlPoint> &cTransferControlPoints = cTransferFunctionChannel.GetTransferControlPoints();
				if (cTransferControlPoints.GetNumOfElements()) {
					// Get the list of cubic splines, this call will update the cubic splines if required
					const Array<CubicSpline> &cCubicSplines = cTransferFunctionChannel.GetCubicSplines();

					for (uint32 i=0; i<cTransferControlPoints.GetNumOfElements()-1; i++) {
						int steps = cTransferControlPoints[i + 1].GetIsoValue() - cTransferControlPoints[i].GetIsoValue();
						for (int j=0; j<steps; j++) {
							float k = (float)j / (float)(steps - 1);
							TransferFunc[numTF++].fColor[nChannel] = cCubicSplines[i].GetValueOnSpline(k);
						}
					}
				}
			}

			// Fill
			uint8 *pImageData = pImageBuffer->GetData();
			for (uint32 i=0; i<nNumOfBins; i++) {
				Color4 &color = TransferFunc[i];
				color.Saturate();

				// RGBA entry
				*pImageData = color.GetRInt();
				pImageData++;
				*pImageData = color.GetGInt();
				pImageData++;
				*pImageData = color.GetBInt();
				pImageData++;
				*pImageData = color.GetAInt();
				pImageData++;
			}

			// Cached image is no longer considered to be dirty
			m_bImageDirty = false;

			// Return the image instance
			return cImage;
		} else {
			// Cached image is no longer considered to be dirty
			m_bImageDirty = false;

			// Call the base implementation
			return TransferFunction::GetImage();
		}
	} else {
		// Call the base implementation
		return TransferFunction::GetImage();
	}
}

const Image &TransferFunctionControl::GetConstImage()
{
	// Is the cached image considered to be dirty?
	return m_bImageDirty ? GetImage() : TransferFunction::GetConstImage();
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool TransferFunctionControl::Unload()
{
	// Call base implementation
	return TransferFunction::Unload();
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
