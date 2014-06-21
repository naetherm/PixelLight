/*********************************************************\
 *  File: PerlinNoiseTileable.cpp                        *
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
#include "PLMath/PerlinNoiseTileable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
float PerlinNoiseTileable::TileableNoise1(float fX, float fW)
{
	return static_cast<float>((Noise1(fX)*(fW-fX) + Noise1(fX-fW)*fX)/fW);
}

float PerlinNoiseTileable::TileableNoise2(float fX, float fY, float fW, float fH)
{
	return (Noise2(fX,    fY)   *(fW-fX)*(fH-fY) +
			Noise2(fX-fW, fY)   *    fX *(fH-fY) +
			Noise2(fX,    fY-fH)*(fW-fX)*    fY  +
			Noise2(fX-fW, fY-fH)*    fX *    fY)/(fW*fH);
}

float PerlinNoiseTileable::TileableNoise3(float fX, float fY, float fZ, float fW, float fH, float fD)
{
	return (Noise3(fX,    fY,    fZ)   *(fW-fX)*(fH-fY)*(fD-fZ) +
			Noise3(fX-fW, fY,    fZ)   *    fX *(fH-fY)*(fD-fZ) +
			Noise3(fX,    fY-fH, fZ)   *(fW-fX)*    fY *(fD-fZ) +
			Noise3(fX-fW, fY-fH, fZ)   *    fX *    fY *(fD-fZ) + 
			Noise3(fX,    fY,    fZ-fD)*(fW-fX)*(fH-fY)*    fZ  +
			Noise3(fX-fW, fY,    fZ-fD)*    fX *(fH-fY)*    fZ  +
			Noise3(fX,    fY-fH, fZ-fD)*(fW-fX)*    fY *    fZ  +
			Noise3(fX-fW, fY-fH, fZ-fD)*    fX *    fY *    fZ)/(fW*fH*fD);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
