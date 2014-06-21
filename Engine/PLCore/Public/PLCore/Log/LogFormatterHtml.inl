/*********************************************************\
 *  File: LogFormatterHtml.inl                           *
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
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    This text gets printed to the log when the log is opened
*/
inline void LogFormatterHtml::SetHeader(const String &sHeader)
{
	m_sHeader = sHeader;
}

/**
*  @brief
*    Sets the title of the log
*/
inline void LogFormatterHtml::SetTitle(const String &sTitle)
{
	m_sTitle = sTitle;
}

/**
*  @brief
*    Sets the Background for the HTML document either a color or a image
*/
inline void LogFormatterHtml::SetBackground(const String &sBackground)
{
	m_sBackground = sBackground;
}

/**
*  @brief
*    Sets the default text color
*/
inline void LogFormatterHtml::SetDefaultTextColor(const String &sColor)
{
	if (sColor.GetLength() > 0)
		m_sDefaultTextColor = sColor;
}

/**
*  @brief
*    Sets the default text format string
*/
inline void LogFormatterHtml::SetDefaultTextFormat(const String &sFormat)
{
	if (sFormat.GetLength() > 0)
		m_sDefaultTextFormat = sFormat;
}

/**
*  @brief
*    This text gets printed to the log before the log gets closed
*/
inline void LogFormatterHtml::SetFooter(const String &sFooter)
{
	m_sFooter = sFooter;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
