/*********************************************************\
 *  File: TransferFunctionChannel.h                      *
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


#ifndef __PLVOLUME_TRANSFERFUNCTIONCHANNEL_H__
#define __PLVOLUME_TRANSFERFUNCTIONCHANNEL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color3.h>
#include "PLVolume/TransferFunctionControl/CubicSpline.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class TransferFunctionControl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Transfer function channel
*/
class TransferFunctionChannel {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cTransferFunctionControl
		*    Owner transfer function control instance
		*/
		inline TransferFunctionChannel(TransferFunctionControl &cTransferFunctionControl);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline TransferFunctionChannel(const TransferFunctionChannel &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~TransferFunctionChannel();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		inline TransferFunctionChannel &operator =(const TransferFunctionChannel &cSource);

		/**
		*  @brief
		*    Returns the owner transfer function control instance
		*
		*  @return
		*    The owner transfer function control instance
		*/
		inline TransferFunctionControl &GetTransferFunctionChannel() const;

		/**
		*  @brief
		*    Returns the list of transfer control points
		*
		*  @return
		*    The list of transfer control points
		*
		*  @note
		*    - This method marks derived data to be dirty, use the constant method version of this method whenever possible to avoid unnecessary recalculations
		*/
		inline PLCore::Array<TransferControlPoint> &GetTransferControlPoints();

		/**
		*  @brief
		*    Returns the list of transfer control points
		*
		*  @return
		*    The list of transfer control points
		*/
		inline const PLCore::Array<TransferControlPoint> &GetTransferControlPoints() const;

		/**
		*  @brief
		*    Returns the human readable channel name
		*
		*  @return
		*    The human readable channel name
		*/
		inline PLCore::String GetName() const;

		/**
		*  @brief
		*    Sets the human readable channel name
		*
		*  @param[in] sName
		*    The human readable channel name
		*/
		inline void SetName(const PLCore::String &sName);

		/**
		*  @brief
		*    Returns the color to use for channel visualization
		*
		*  @return
		*    The color to use for channel visualization
		*/
		inline const PLGraphics::Color3 &GetVisualizationColor() const;

		/**
		*  @brief
		*    Sets the color to use for channel visualization
		*
		*  @param[in] cColor
		*    The color to use for channel visualization
		*/
		inline void SetVisualizationColor(const PLGraphics::Color3 &cColor);

		/**
		*  @brief
		*    Returns the list of cubic splines
		*
		*  @return
		*    The list of cubic splines
		*
		*  @note
		*    - The data is automatically derived from the transfer control points
		*/
		inline const PLCore::Array<CubicSpline> &GetCubicSplines();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		TransferFunctionControl				*m_pTransferFunctionControl;	/**< Owner transfer function control instance, always valid! */
		PLCore::Array<TransferControlPoint>  m_lstTransferControlPoints;	/**< Transfer control points */
		PLCore::String						 m_sName;						/**< Human readable channel name */
		PLGraphics::Color3					 m_cVisualizationColor;			/**< Color to use for channel visualization */
		// Cached cubic splines and image
		PLCore::Array<CubicSpline>			 m_lstCubicSplines;				/**< Cubic splines (from "m_lstTransferControlPoints" derived data, access it via "GetCubicSplines()") */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLVolume/TransferFunctionControl/TransferFunctionChannel.inl"


#endif // __PLVOLUME_TRANSFERFUNCTIONCHANNEL_H__
