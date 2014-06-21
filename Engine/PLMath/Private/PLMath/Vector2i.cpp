/*********************************************************\
 *  File: Vector2i.cpp                                   *
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
#include "PLMath/Vector2i.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Vector2i Vector2i::Zero		  ( 0,  0);
const Vector2i Vector2i::One		  ( 1,  1);
const Vector2i Vector2i::NegativeOne  (-1, -1);
const Vector2i Vector2i::UnitX		  ( 1,  0);
const Vector2i Vector2i::UnitY		  ( 0,  1);
const Vector2i Vector2i::NegativeUnitX(-1,  0);
const Vector2i Vector2i::NegativeUnitY( 0, -1);


/**
*  @brief
*    To string
*/
String Vector2i::ToString() const
{
	return String::Format("%d %d", x, y);
}

/**
*  @brief
*    From string
*/
bool Vector2i::FromString(const String &sString)
{
	if (sString.GetLength()) {
		// Parse components
		uint32 nParsePos  = 0;
		uint32 nComponent = 0;
		static RegEx cRegEx("\\s*(\\S+)");
		while (nComponent < 2 && cRegEx.Match(sString, nParsePos)) {
			nV[nComponent++] = cRegEx.GetResult(0).GetInt();
			nParsePos = cRegEx.GetPosition();
		}

		// Set unused components to 0
		while (nComponent < 2)
			nV[nComponent++] = 0;

		// Done
		return true;
	} else {
		// Error!
		x = y = 0;
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
