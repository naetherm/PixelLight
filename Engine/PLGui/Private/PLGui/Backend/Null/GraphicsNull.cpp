/*********************************************************\
 *  File: GraphicsNull.cpp                               *
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
#include "PLGui/Backend/Null/GraphicsNull.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GraphicsNull::GraphicsNull(Graphics &cGraphics) : GraphicsImpl(cGraphics)
{
}

/**
*  @brief
*    Destructor
*/
GraphicsNull::~GraphicsNull()
{
}


//[-------------------------------------------------------]
//[ Public virtual GraphicsImpl functions                 ]
//[-------------------------------------------------------]
void GraphicsNull::DrawLine(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nWidth)
{
	// Not implemented
}

void GraphicsNull::DrawRect(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY, uint32 nWidth)
{
	// Not implemented
}

void GraphicsNull::DrawBox(const Color4 &cColor, const Vector2i &vPos1, const Vector2i &vPos2, uint32 nRoundX, uint32 nRoundY)
{
	// Not implemented
}

void GraphicsNull::DrawGradientBox(const Color4 &cColor1, const Color4 &cColor2, float fAngle, const Vector2i &vPos1, const Vector2i &vPos2)
{
	// Not implemented
}

void GraphicsNull::DrawImage(const Image &cImage, const Vector2i &vPos, const Vector2i &vSize)
{
	// Not implemented
}

void GraphicsNull::DrawText(const Font &cFont, const Color4 &cTextColor, const Color4 &cBkColor, const Vector2i &vPos, const String &sText)
{
	// Not implemented
}

uint32 GraphicsNull::GetTextWidth(const Font &cFont, const String &sText)
{
	// Not implemented
	return 0;
}

uint32 GraphicsNull::GetTextHeight(const Font &cFont, const String &sText)
{
	// Not implemented
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
