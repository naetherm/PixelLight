/*********************************************************\
 *  File: Transform3.h                                   *
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


#ifndef __PLMATH_TRANSFORM3_H__
#define __PLMATH_TRANSFORM3_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include <PLMath/Matrix3x4.h>
#include <PLMath/Quaternion.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    3D transform class
*
*  @remarks
*    The design goal of this class was not to be as memory compact as possible - for such a use case, one can use
*    for example directly Matrix3x4. The design goal was to provide a class that is able to convert between different
*    representations of a 3D transform in an efficient way. To achieve this, the "Lazy Evaluation"-scheme is used. This
*    means that, for example, the inverse transform is only recalculated if it's really required. To allow a practical
*    usage of this transform class within more advanced systems, events are provided to inform when transform data has
*    been changed.
*/
class Transform3 {


	//[-------------------------------------------------------]
	//[ Events                                                ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<>	EventPosition;	/**< Position change event */
		PLCore::Event<>	EventRotation;	/**< Rotation change event */
		PLCore::Event<>	EventScale;		/**< Scale change event */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor (set's an identity transform matrix)
		*/
		PLMATH_API Transform3();

		/**
		*  @brief
		*    Copy constructor (event data is not copied)
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLMATH_API Transform3(const Transform3 &cSource);

		/**
		*  @brief
		*    Copy operator (event data is not copied)
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*
		*  @note
		*    - EventPosition, EventRotation and EventScale events are emitted
		*/
		PLMATH_API Transform3 &operator =(const Transform3 &cSource);

		/**
		*  @brief
		*    Gets the position
		*
		*  @return
		*    Position
		*/
		inline const Vector3 &GetPosition() const;

		/**
		*  @brief
		*    Sets the position
		*
		*  @param[in] vPosition
		*    New position
		*/
		PLMATH_API void SetPosition(const Vector3 &vPosition);

		/**
		*  @brief
		*    Gets the rotation
		*
		*  @return
		*    Rotation
		*/
		inline const Quaternion &GetRotation() const;

		/**
		*  @brief
		*    Sets the rotation
		*
		*  @param[in] qRotation
		*    New rotation
		*/
		PLMATH_API void SetRotation(const Quaternion &qRotation);

		/**
		*  @brief
		*    Gets the scale
		*
		*  @return
		*    Scale
		*/
		inline const Vector3 &GetScale() const;

		/**
		*  @brief
		*    Sets the scale
		*
		*  @param[in] vScale
		*    New scale
		*/
		PLMATH_API void SetScale(const Vector3 &vScale);

		/**
		*  @brief
		*    Returns the current final transform matrix
		*
		*  @return
		*    The current final transform matrix
		*
		*  @note
		*    - If position, rotation or scale was changed, the current transform matrix
		*      is recalculated internally before it is returned
		*/
		PLMATH_API const Matrix3x4 &GetMatrix();

		/**
		*  @brief
		*    Sets the current final transform matrix
		*
		*  @param[in] mTrans
		*    The current final transform matrix
		*
		*  @note
		*    - The scale can't be extracted correctly from the given transform matrix if one or
		*      two scale components are negative while another is/are not (we can't figure out
		*      WHICH axis are negative!)
		*/
		PLMATH_API void SetMatrix(const Matrix3x4 &mTrans);

		/**
		*  @brief
		*    Returns the current final inverse transform matrix
		*
		*  @return
		*    The current final inverse transform matrix
		*
		*  @note
		*    - If position, rotation or scale was changed, the current inverse transform matrix
		*      is recalculated internally before it is returned
		*/
		PLMATH_API const Matrix3x4 &GetInverseMatrix();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Flags which hold ínternal information
		*/
		enum EInternalFlags {
			RecalculateTransformMatrix		   = 1<<0,	/**< Recalculation of transform matrix required */
			RecalculateInverseTransformMatrix  = 1<<1	/**< Recalculation of inverse transform matrix required */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint8 m_nInternalFlags;	/**< Internal flags */
		Vector3		  m_vPosition;		/**< Position as vector */
		Quaternion	  m_qRotation;		/**< Rotation as quaternion */
		Vector3		  m_vScale;			/**< Scale as vector */
		Matrix3x4	  m_mTrans;			/**< Current final composed transform matrix */
		Matrix3x4	  m_mInvTrans;		/**< Current final composed inverse transform matrix */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLMath/Transform3.inl"


#endif // __PLMATH_TRANSFORM3_H__
