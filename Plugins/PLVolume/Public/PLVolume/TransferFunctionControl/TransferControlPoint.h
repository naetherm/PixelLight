/*********************************************************\
 *  File: TransferControlPoint.h                         *
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


#ifndef __PLVOLUME_TRANSFERCONTROLPOINT_H__
#define __PLVOLUME_TRANSFERCONTROLPOINT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/PLCore.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Transfer control point
*/
class TransferControlPoint {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline TransferControlPoint();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		inline TransferControlPoint(const TransferControlPoint &cSource);

		/**
		*  @brief
		*    Constructor for iso-value + value
		*
		*  @param[in] nIsoValue
		*    Iso-value (e.g. along x-axis)
		*  @param[in] fValue
		*    Value (e.g. along y-axis)
		*/
		inline TransferControlPoint(PLCore::uint32 nIsoValue, float fValue);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~TransferControlPoint();

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
		inline TransferControlPoint &operator =(const TransferControlPoint &cSource);

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cOther
		*    Other instance to compare with
		*
		*  @return
		*    'true' if equal, else 'false'
		*/
		inline bool operator ==(const TransferControlPoint &cOther) const;

		/**
		*  @brief
		*    Returns the iso-value
		*
		*  @return
		*    The iso-value (e.g. along x-axis)
		*/
		inline PLCore::uint32 GetIsoValue() const;

		/**
		*  @brief
		*    Sets the iso-value
		*
		*  @param[in] nIsoValue
		*    Iso-value (e.g. along x-axis)
		*/
		inline void SetIsoValue(PLCore::uint32 nIsoValue);

		/**
		*  @brief
		*    Returns the value
		*
		*  @return
		*    The value (e.g. along y-axis)
		*/
		inline float GetValue() const;

		/**
		*  @brief
		*    Sets the value
		*
		*  @param[in] fValue
		*    Value to set (e.g. along y-axis)
		*/
		inline void SetValue(float fValue);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32 m_nIsoValue;	/**< Iso-value (e.g. along x-axis) */
		float          m_fValue;	/**< Value (e.g. along y-axis) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLVolume/TransferFunctionControl/TransferControlPoint.inl"


#endif // __PLVOLUME_TRANSFERCONTROLPOINT_H__
