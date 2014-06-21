/*********************************************************\
 *  File: TransferFunctionChannel.inl                    *
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
#include "PLVolume/TransferFunctionControl/TransferFunctionControl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
inline TransferFunctionChannel::TransferFunctionChannel(TransferFunctionControl &cTransferFunctionControl) :
	m_pTransferFunctionControl(&cTransferFunctionControl)
{
}

/**
*  @brief
*    Copy constructor
*/
inline TransferFunctionChannel::TransferFunctionChannel(const TransferFunctionChannel &cSource) :
	m_pTransferFunctionControl(cSource.m_pTransferFunctionControl),
	m_lstTransferControlPoints(cSource.m_lstTransferControlPoints),
	m_sName(cSource.m_sName),
	m_cVisualizationColor(cSource.m_cVisualizationColor),
	m_lstCubicSplines(cSource.m_lstCubicSplines)
{
}

/**
*  @brief
*    Destructor
*/
inline TransferFunctionChannel::~TransferFunctionChannel()
{
}

/**
*  @brief
*    Copy operator
*/
inline TransferFunctionChannel &TransferFunctionChannel::operator =(const TransferFunctionChannel &cSource)
{
	// Image is now considered to be dirty (previous owner)
	m_pTransferFunctionControl->m_bImageDirty = true;

	// Copy data
	m_pTransferFunctionControl = cSource.m_pTransferFunctionControl;
	m_lstTransferControlPoints = cSource.m_lstTransferControlPoints;
	m_sName					   = cSource.m_sName;
	m_cVisualizationColor	   = cSource.m_cVisualizationColor;
	m_lstCubicSplines		   = cSource.m_lstCubicSplines;

	// Image is now considered to be dirty
	m_pTransferFunctionControl->m_bImageDirty = true;

	// Return a reference to this instance
	return *this;
}

/**
*  @brief
*    Returns the owner transfer function control instance
*/
inline TransferFunctionControl &TransferFunctionChannel::GetTransferFunctionChannel() const
{
	return *m_pTransferFunctionControl;
}

/**
*  @brief
*    Returns the list of transfer control points
*/
inline PLCore::Array<TransferControlPoint> &TransferFunctionChannel::GetTransferControlPoints()
{
	// Cached cubic splines and image are now considered to be dirty
	m_lstCubicSplines.Clear();
	m_pTransferFunctionControl->m_bImageDirty = true;

	// Return the list of transfer control points
	return m_lstTransferControlPoints;
}

/**
*  @brief
*    Returns the list of transfer control points
*/
inline const PLCore::Array<TransferControlPoint> &TransferFunctionChannel::GetTransferControlPoints() const
{
	// Return the list of transfer control points
	return m_lstTransferControlPoints;
}

/**
*  @brief
*    Returns the color to use for channel visualization
*/
inline const PLGraphics::Color3 &TransferFunctionChannel::GetVisualizationColor() const
{
	return m_cVisualizationColor;
}

/**
*  @brief
*    Returns the human readable channel name
*/
inline PLCore::String TransferFunctionChannel::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Sets the human readable channel name
*/
inline void TransferFunctionChannel::SetName(const PLCore::String &sName)
{
	m_sName = sName;
}

/**
*  @brief
*    Sets the color to use for channel visualization
*/
inline void TransferFunctionChannel::SetVisualizationColor(const PLGraphics::Color3 &cColor)
{
	// Set visualization color
	m_cVisualizationColor = cColor;

	// Image is now considered to be dirty
	m_pTransferFunctionControl->m_bImageDirty = true;
}

/**
*  @brief
*    Returns the list of cubic splines
*/
inline const PLCore::Array<CubicSpline> &TransferFunctionChannel::GetCubicSplines()
{
	// Create cached cubic splines right now?
	if (!m_lstCubicSplines.GetNumOfElements() && m_lstTransferControlPoints.GetNumOfElements())
		CubicSpline::CalculateCubicSpline(m_lstTransferControlPoints.GetNumOfElements() - 1, m_lstTransferControlPoints, m_lstCubicSplines);

	// Return the list of cached cubic splines
	return m_lstCubicSplines;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
