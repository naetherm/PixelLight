/*********************************************************\
 *  File: ParseTools.cpp                                 *
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
#include "PLCore/String/Tokenizer.h"
#include "PLCore/String/ParseTools.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parse string containing a hex value to an integer (for example "FF" => "255")
*/
uint32 ParseTools::ParseHexValue(const String &sString)
{
	uint32 nValue = 0;
	for (uint32 i=0; i<sString.GetLength(); i++) {
		const String sChar = sString.GetSubstring(i, 1).ToLower();
		uint8 nHex = 0;
		if (sChar == "0")
			nHex =  0;
		else if (sChar == "1")
			nHex =  1;
		else if (sChar == "2")
			nHex =  2;
		else if (sChar == "3")
			nHex =  3;
		else if (sChar == "4")
			nHex =  4;
		else if (sChar == "5")
			nHex =  5;
		else if (sChar == "6")
			nHex =  6;
		else if (sChar == "7")
			nHex =  7;
		else if (sChar == "8")
			nHex =  8;
		else if (sChar == "9")
			nHex =  9;
		else if (sChar == "a")
			nHex = 10;
		else if (sChar == "b")
			nHex = 11;
		else if (sChar == "c")
			nHex = 12;
		else if (sChar == "d")
			nHex = 13;
		else if (sChar == "e")
			nHex = 14;
		else if (sChar == "f")
			nHex = 15;
		nValue = nValue * 16 + nHex;
	}
	return nValue;
}

/**
*  @brief
*    Parse an array of integer values
*/
int ParseTools::ParseIntegerArray(const String &sString, int *pnValues, uint32 nMaxNumOfElements)
{
	// Check parameters
	if (!pnValues || !sString.GetLength() || !nMaxNumOfElements)
		return 0;

	// Parse string
	Tokenizer cTokenizer;
	cTokenizer.Start(sString);
	uint32 nPos = 0;
	while (cTokenizer.GetNextToken().GetLength() > 0 && nPos < nMaxNumOfElements) {
		// Parse integer value
		pnValues[nPos] = cTokenizer.GetToken().GetInt();
		nPos++;
	}
	cTokenizer.Stop();

	// Return the number of elements read
	return nPos;
}

/**
*  @brief
*    Parse an array of float values
*/
int ParseTools::ParseFloatArray(const String &sString, float *pfValues, uint32 nMaxNumOfElements)
{
	// Check parameters
	if (!pfValues || !sString.GetLength() || !nMaxNumOfElements)
		return 0;

	// Parse string
	Tokenizer cTokenizer;
	cTokenizer.Start(sString);
	uint32 nPos = 0;
	while (cTokenizer.GetNextToken().GetLength() > 0 && nPos < nMaxNumOfElements) {
		// Parse integer value
		pfValues[nPos] = cTokenizer.GetToken().GetFloat();
		nPos++;
	}
	cTokenizer.Stop();

	// Return the number of elements read
	return nPos;
}

/**
*  @brief
*    Parse an array of double values
*/
int ParseTools::ParseDoubleArray(const String &sString, double *pdValues, uint32 nMaxNumOfElements)
{
	// Check parameters
	if (!pdValues || !sString.GetLength() || !nMaxNumOfElements)
		return 0;

	// Parse string
	Tokenizer cTokenizer;
	cTokenizer.Start(sString);
	uint32 nPos = 0;
	while (cTokenizer.GetNextToken().GetLength() > 0 && nPos < nMaxNumOfElements) {
		// Parse integer value
		pdValues[nPos] = cTokenizer.GetToken().GetDouble();
		nPos++;
	}
	cTokenizer.Stop();

	// Return the number of elements read
	return nPos;
}

/**
*  @brief
*    Returns the flags from a string
*/
uint32 ParseTools::GetFlagsFromString(const String &sFlags)
{
	// Compose flag variable from a string
	uint32 nFlags = 0;
	if (sFlags.GetLength()) {
		Tokenizer cTokenizer;
		cTokenizer.Start(sFlags);
		cTokenizer.SetDelimiters(" \t\r\n&,|");
		while (cTokenizer.GetNextToken().GetLength())
			nFlags |= cTokenizer.GetToken().GetInt();
		cTokenizer.Stop();
	}
	return nFlags;
}

/**
*  @brief
*    Creates a string from flags
*/
String ParseTools::GetStringFromFlags(uint32 nFlags)
{
	// Init string
	String sFlags = (nFlags == 0) ? "0" : "";

	// Check all flags bitwise
	uint32 nFlag = nFlags;
	int nPos = 1;
	while (nFlag) {
		// Check current flag
		if (nFlag & 1) {
			// Add to string
			if (nPos > 1 && sFlags.GetLength())
				sFlags += "|";
			sFlags += nPos;
		}

		// Go to next flag
		nFlag = nFlag >> 1;
		nPos  = nPos  << 1;
	}

	// Return string
	return sFlags;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
