/*********************************************************\
 *  File: Half.cpp                                       *
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
#include "PLMath/Half.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const uint16 Half::Zero							= 0x0000;
const uint16 Half::One							= 0x3C00;
const float	 Half::SmallestPositive				= 5.96046448e-08f;
const float	 Half::SmallestPositiveNormalized	= 6.10351562e-05f;
const float	 Half::LargestPositive				= 65504.0f;
const float	 Half::Epsilon						= 0.00097656f;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Converts a given half value into a float value
*/
float Half::ToFloat(uint16 nHalf)
{
	// Bring the input into a form the following algorithm is able to process, "uif" is used for output
	union uif {
		unsigned int i;
		float		 f;
	};
	const unsigned short y = nHalf;

	// Implementation from OpenEXR IlmBase 1.0.2 (Modified BSD License)
	///////////////////////////////////////////////////////////////////////////
	//
	// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
	// Digital Ltd. LLC
	// 
	// All rights reserved.
	// 
	// Redistribution and use in source and binary forms, with or without
	// modification, are permitted provided that the following conditions are
	// met:
	// *       Redistributions of source code must retain the above copyright
	// notice, this list of conditions and the following disclaimer.
	// *       Redistributions in binary form must reproduce the above
	// copyright notice, this list of conditions and the following disclaimer
	// in the documentation and/or other materials provided with the
	// distribution.
	// *       Neither the name of Industrial Light & Magic nor the names of
	// its contributors may be used to endorse or promote products derived
	// from this software without specific prior written permission. 
	// 
	// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	//
	///////////////////////////////////////////////////////////////////////////

	const int s = (y >> 15) & 0x00000001;
		  int e = (y >> 10) & 0x0000001f;
		  int m =  y        & 0x000003ff;

	if (e == 0) {
		if (m == 0) {
			// Plus or minus zero
			uif sValue;
			sValue.i = s << 31;
			return sValue.f;
		} else {
			// Denormalized number -- renormalize it
			while (!(m & 0x00000400)) {
				m <<= 1;
				e -=  1;
			}

			e += 1;
			m &= ~0x00000400;
		}
	} else if (e == 31) {
		if (m == 0) {
			// Positive or negative infinity
			uif sValue;
			sValue.i = (s << 31) | 0x7f800000;
			return sValue.f;
		} else {
			// Nan -- preserve sign and significant bits
			uif sValue;
			sValue.i = (s << 31) | 0x7f800000 | (m << 13);
			return sValue.f;
		}
	}

	// Normalized number
	e = e + (127 - 15);
	m = m << 13;

	// Assemble s, e and m.
	uif sValue;
	sValue.i = (s << 31) | (e << 23) | m;
	return sValue.f;
}

/**
*  @brief
*    Converts a given float value into a half value
*/
uint16 Half::FromFloat(float fFloat)
{
	// Bring the input into a form the following algorithm is able to process
	union uif {
		unsigned int i;
		float		 f;
	};
	uif sValue;
	sValue.f = fFloat;
	const int i = sValue.i;

	// Implementation from OpenEXR IlmBase 1.0.2 (Modified BSD License)
	///////////////////////////////////////////////////////////////////////////
	//
	// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
	// Digital Ltd. LLC
	// 
	// All rights reserved.
	// 
	// Redistribution and use in source and binary forms, with or without
	// modification, are permitted provided that the following conditions are
	// met:
	// *       Redistributions of source code must retain the above copyright
	// notice, this list of conditions and the following disclaimer.
	// *       Redistributions in binary form must reproduce the above
	// copyright notice, this list of conditions and the following disclaimer
	// in the documentation and/or other materials provided with the
	// distribution.
	// *       Neither the name of Industrial Light & Magic nor the names of
	// its contributors may be used to endorse or promote products derived
	// from this software without specific prior written permission. 
	// 
	// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	//
	///////////////////////////////////////////////////////////////////////////

	// Our floating point number, f, is represented by the bit
	// pattern in integer i.  Disassemble that bit pattern into
	// the sign, s, the exponent, e, and the significant, m.
	// Shift s into the position where it will go in in the
	// resulting half number.
	// Adjust e, accounting for the different exponent bias
	// of float and half (127 versus 15).
	register int s =  (i >> 16) & 0x00008000;
	register int e = ((i >> 23) & 0x000000ff) - (127 - 15);
	register int m =   i        & 0x007fffff;

	// Now reassemble s, e and m into a half:
	if (e <= 0) {
		if (e < -10) {
			// E is less than -10.  The absolute value of f is
			// less than HALF_MIN (f may be a small normalized
			// float, a denormalized float or a zero).

			// We convert f to a half zero with the same sign as f.
			return static_cast<uint16>(s);
		}

		// E is between -10 and 0.  F is a normalized float
		// whose magnitude is less than HALF_NRM_MIN.

		// We convert f to a denormalized half.

		// Add an explicit leading 1 to the significant.
		m = m | 0x00800000;

		// Round to m to the nearest (10+e)-bit value (with e between
		// -10 and 0); in case of a tie, round to the nearest even value.

		// Rounding may cause the significant to overflow and make
		// our number normalized.  Because of the way a half's bits
		// are laid out, we don't have to treat this case separately;
		// the code below will handle it correctly.

		const int t = 14 - e;
		const int a = (1 << (t - 1)) - 1;
		const int b = (m >> t) & 1;

		m = (m + a + b) >> t;

		// Assemble the half from s, e (zero) and m.
		return static_cast<uint16>(s | m);
	} else if (e == 0xff - (127 - 15)) {
		if (m == 0) {
			// F is an infinity; convert f to a half
			// infinity with the same sign as f.
			return static_cast<uint16>(s | 0x7c00);
		} else {
			// F is a NAN; we produce a half NAN that preserves
			// the sign bit and the 10 leftmost bits of the
			// significant of f, with one exception: If the 10
			// leftmost bits are all zero, the NAN would turn 
			// into an infinity, so we have to set at least one
			// bit in the significant.
			m >>= 13;
			return static_cast<uint16>(s | 0x7c00 | m | (m == 0));
		}
	} else {
		// E is greater than zero.  F is a normalized float.
		// We try to convert f to a normalized half.

		// Round to m to the nearest 10-bit value.  In case of
		// a tie, round to the nearest even value.
		m = m + 0x00000fff + ((m >> 13) & 1);
		if (m & 0x00800000) {
			m =  0;		// overflow in significant,
			e += 1;		// adjust exponent
		}

		// Handle exponent overflow
		if (e > 30) {
			{ // Cause a hardware floating point overflow
				volatile float f = 1e10;
				for (int n=0; n<10; n++)
					f *= f;	// This will overflow before the for­loop terminates
			}
			return static_cast<uint16>(s | 0x7c00);	// If this returns, the half becomes an
		}											// infinity with the same sign as f.

		// Assemble the half from s, e and m.
		return static_cast<uint16>(s | (e << 10) | (m >> 13));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
