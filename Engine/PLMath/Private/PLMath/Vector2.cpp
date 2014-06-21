/*********************************************************\
 *  File: Vector2.cpp                                    *
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
#include <PLCore/String/RegEx.h>
#include "PLMath/Matrix3x3.h"
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"
#include "PLMath/Vector2.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Vector2 Vector2::Zero			( 0.0f,  0.0f);
const Vector2 Vector2::One			( 1.0f,  1.0f);
const Vector2 Vector2::NegativeOne	(-1.0f, -1.0f);
const Vector2 Vector2::UnitX		( 1.0f,  0.0f);
const Vector2 Vector2::UnitY		( 0.0f,  1.0f);
const Vector2 Vector2::NegativeUnitX(-1.0f,  0.0f);
const Vector2 Vector2::NegativeUnitY( 0.0f, -1.0f);


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Constructors                                          ]
//[-------------------------------------------------------]
Vector2::Vector2(const Vector3 &vV) :
	x(vV.x),
	y(vV.y)
{
}

Vector2::Vector2(const Vector4 &vV) :
	x(vV.x),
	y(vV.y)
{
}


//[-------------------------------------------------------]
//[ Assignment operators                                  ]
//[-------------------------------------------------------]
Vector2 &Vector2::operator =(const Vector3 &vV)
{
	x = vV.x;
	y = vV.y;
	return *this;
}

Vector2 &Vector2::operator =(const Vector4 &vV)
{
	x = vV.x;
	y = vV.y;
	return *this;
}

/**
*  @brief
*    Rotates the vector
*/
Vector2 &Vector2::operator *=(const Matrix3x3 &mRot)
{
	float xt = x, yt = y;
	x = mRot.xx*xt + mRot.xy*yt;
	y = mRot.yx*xt + mRot.yy*yt;
	return *this;
}

/**
*  @brief
*    Transforms the vector
*/
Vector2 &Vector2::operator *=(const Matrix3x4 &mRot)
{
	float xt = x, yt = y;
	x = mRot.xx*xt + mRot.xy*yt + mRot.xw;
	y = mRot.yx*xt + mRot.yy*yt + mRot.yw;
	return *this;
}

/**
*  @brief
*    Transforms the vector
*/
Vector2 &Vector2::operator *=(const Matrix4x4 &mTrans)
{
	float xt = x, yt = y;
	float fInvW = 1.0f/(mTrans.wx*xt + mTrans.wy*yt + mTrans.ww);
	x = (mTrans.xx*xt + mTrans.xy*yt + mTrans.xw)*fInvW;
	y = (mTrans.yx*xt + mTrans.yy*yt + mTrans.yw)*fInvW;
	return *this;
}

/**
*  @brief
*    To string
*/
String Vector2::ToString() const
{
	return String::Format("%g %g", x, y);
}

/**
*  @brief
*    From string
*/
bool Vector2::FromString(const String &sString)
{
	if (sString.GetLength()) {
		// Parse components
		uint32 nParsePos  = 0;
		uint32 nComponent = 0;
		static RegEx cRegEx("\\s*(\\S+)");
		while (nComponent < 2 && cRegEx.Match(sString, nParsePos)) {
			fV[nComponent++] = cRegEx.GetResult(0).GetFloat();
			nParsePos = cRegEx.GetPosition();
		}

		// Set unused components to 0
		while (nComponent < 2)
			fV[nComponent++] = 0.0f;

		// Done
		return true;
	} else {
		// Error!
		x = y = 0.0f;
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
