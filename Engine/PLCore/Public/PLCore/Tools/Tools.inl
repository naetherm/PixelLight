/*********************************************************\
 *  File: Tools.inl                                      *
 *      Tools inline implementation
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    float to uint32
*/
inline uint32 Tools::FloatToUInt32(float f)
{
	return *(reinterpret_cast<uint32*>(&f));
}

/**
*  @brief
*    uint32 to float
*/
inline float Tools::UInt32ToFloat(uint32 n)
{
	return *(reinterpret_cast<float*>(&n));
}

/**
*  @brief
*    Base64 encoder
*/
inline String Tools::GetBase64(const String &sString)
{
	String sBase64;
	uint32 nLength = sString.GetLength();
	const char *pszString = sString.GetASCII();
	for (uint32 i=0; i<sString.GetLength(); i+=3) {
		uint8 nByte0 = pszString[i];
		uint8 nByte1 = (i+1 < nLength) ? pszString[i+1] : 0;
		uint8 nByte2 = (i+2 < nLength) ? pszString[i+2] : 0;
		for (int j=0; j<4; j++) {
			uint8 nBlock = 0;
			switch (j) {
				case 0: nBlock = ((nByte0 & 252) >> 2); break;
				case 1: nBlock = ((nByte0 &   3) << 4) | ((nByte1 & 240) >> 4); break;
				case 2: nBlock = ((nByte1 &  15) << 2) | ((nByte2 & 128+64) >> 6); break;
				case 3: nBlock = ((nByte2 &  63)); break;
			}
			char cChar = 0;
			if (nBlock <= 25)						cChar = static_cast<char>('A' + nBlock);
			else if (nBlock >= 26 && nBlock <= 51)	cChar = static_cast<char>('a' + (nBlock - 26));
			else if (nBlock >= 52 && nBlock <= 61)	cChar = static_cast<char>('0' + (nBlock - 52));
			else if (nBlock == 62)					cChar = '+';
			else if (nBlock == 63)					cChar = '/';
			if ((j >= 2 && i+1 >= nLength) || (j == 3 && i+2 >= nLength))
				cChar = '=';
			sBase64 += cChar;
		}
	}
	return sBase64;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
