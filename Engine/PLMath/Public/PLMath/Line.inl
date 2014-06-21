/*********************************************************\
 *  File: Line.inl                                       *
 *      Line inline implementation
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
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting all start- and end-position components to 0
*/
inline Line::Line()
{
}

/**
*  @brief
*    Copy constructor
*/
inline Line::Line(const Line &cSource) :
	vStart(cSource.vStart),
	vEnd(cSource.vEnd)
{
}

/**
*  @brief
*    Copy constructor
*/
inline Line::Line(const Vector3 &vStartPoint, const Vector3 &vEndPoint) :
	vStart(vStartPoint),
	vEnd(vEndPoint)
{
}

/**
*  @brief
*    Destructor
*/
inline Line::~Line()
{
}

/**
*  @brief
*    Copy operator
*/
inline Line &Line::operator =(const Line &cSource)
{
	// Copy data
	vStart = cSource.vStart;
	vEnd   = cSource.vEnd;

	// Return this
	return *this;
}

/**
*  @brief
*    Sets the start and end position of the line
*/
inline void Line::Set(const Vector3 &vStart, const Vector3 &vEnd)
{
	this->vStart = vStart;
	this->vEnd   = vEnd;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
