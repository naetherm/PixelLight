/*********************************************************\
 *  File: PerlinNoiseTurbulence.cpp                      *
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
#include "PLMath/PerlinNoiseTurbulence.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
float PerlinNoiseTurbulence::Turbulence2(float fX, float fY, float fFreq)
{
	float t = 0.0f;

	do {
		t += Noise2(fFreq*fX, fFreq*fY)/fFreq;
		fFreq *= 0.5f;
	} while (fFreq >= 1.0f);
	
	return t;
}

float PerlinNoiseTurbulence::Turbulence3(float fX, float fY, float fZ, float fFreq)
{
	float t = 0.0f;

	do {
		t += Noise3(fFreq*fX, fFreq*fY, fFreq*fZ)/fFreq;
		fFreq *= 0.5f;
	} while (fFreq >= 1.0f);
	
	return t;
}

float PerlinNoiseTurbulence::TileableTurbulence2(float fX, float fY, float fW, float fH, float fFreq)
{
	float t = 0.0f;

	do {
		t += TileableNoise2(fFreq*fX, fFreq*fY, fW*fFreq, fH*fFreq)/fFreq;
		fFreq *= 0.5f;
	} while (fFreq >= 1.0f);
	
	return t;
}

float PerlinNoiseTurbulence::TileableTurbulence3(float fX, float fY, float fZ, float fW, float fH, float fD, float fFreq)
{
	float t = 0.0f;

	do {
		t += TileableNoise3(fFreq*fX, fFreq*fY, fFreq*fZ, fW*fFreq, fH*fFreq, fD*fFreq)/fFreq;
		fFreq *= 0.5f;
	} while (fFreq >= 1.0f);
	
	return t;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
