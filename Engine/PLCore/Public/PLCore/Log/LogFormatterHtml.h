/*********************************************************\
 *  File: LogFormatterHtml.h                             *
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


#ifndef __PLCORE_LOGFORMATTER_HTML_H__
#define __PLCORE_LOGFORMATTER_HTML_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/HashMap.h"
#include "PLCore/Log/LogFormatter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    HTML log formatter
*
*  @note
*    - Header: This text/HTML codes are written after the <body> tags
*    - Background: The background is set in the <body> tag
*    - Background: You must also give the option name (e.g. bgcolor=<color in HTML notation>)
*    - DefaultTextColor: The color must be in HTML notation. Either the color name(e.g. red) or the RGB values in hexadecimal. (e.g. #FF0000)
*    - Implementation of the strategy design pattern, this class a concrete strategy of the strategy "LogFormatter" of the context "Log"
*/
class LogFormatterHtml : public LogFormatter {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API LogFormatterHtml();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~LogFormatterHtml();

		/**
		*  @brief
		*    Sets the text format string for the specified log level
		*
		*  @param[in] nLogLevel
		*    The log level for which the text format should be set
		*  @param[in] sFormat
		*    The text format string
		*
		*  @note
		*    - You can use every HTML tags for the formating
		*    - The syntax is as follows: <format-tags>|<end format-tags>
		*    - The '|' gets replaced with the log message
		*/
		PLCORE_API void SetTextFormat(uint8 nLogLevel, const String &sFormat);

		/**
		*  @brief
		*    This text gets printed to the log when the log is opened
		*
		*  @param[in] sHeader
		*    Header text
		*
		*  @note
		*    This text/HTML codes are written after the <body> tags
		*/
		inline void SetHeader(const String &sHeader = "");

		/**
		*  @brief
		*    Sets the title of the log
		*
		*  @param[in] sTitle
		*    The log's title
		*/
		inline void SetTitle(const String &sTitle = "PL-Log");

		/**
		*  @brief
		*    Sets the background for the HTML document either a color or a image
		*
		*  @param[in] sBackground
		*    The HTML background
		*
		*  @note
		*    - The background is set in the <body> tag
		*    - You must also give the option name (e.g. bgcolor=<color in HTML notation>)
		*/
		inline void SetBackground(const String &sBackground = "");

		/**
		*  @brief
		*    Sets the default text color
		*
		*  @param[in] sColor
		*    The text color
		*
		*  @remarks
		*    The color must be in HTML notation. Either the color name(e.g. red) or the
		*    RGB values in hexadecimal. (e.g. #FF0000)
		*/
		inline void SetDefaultTextColor(const String &sColor = "black");

		/**
		*  @brief
		*    Sets the default text format string
		*
		*  @param[in] sFormat
		*    The text format string
		*
		*  @note
		*    - See SetTextFormat()
		*/
		inline void SetDefaultTextFormat(const String &sFormat = "|<br>");

		/**
		*  @brief
		*    This text gets printed to the log before the log gets closed
		*
		*  @param[in] sFooter
		*    Footer text
		*/
		inline void SetFooter(const String &sFooter = "");


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		LogFormatterHtml(const LogFormatterHtml &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		LogFormatterHtml &operator =(const LogFormatterHtml &cSource);

		/**
		*  @brief
		*    Returns the HTML formated text the given log level
		*
		*  @param[in] nLogLevel
		*    Log level for which formated text should be returned
		*
		*  @param[in] sText
		*    The log text which should be surrounded with the HTML text format-tags
		*
		*  @return
		*    HTML formated text
		*/
		String GetHtmlFormatedText(uint8 nLogLevel, const String &sText) const;


	//[-------------------------------------------------------]
	//[ Private virtual LogFormatter functions                ]
	//[-------------------------------------------------------]
	private:
		virtual bool Open(const String &sFilename) override;
		virtual bool Close() override;
		virtual bool Output(uint8 nLogLevel, const String &sText) override;
		virtual bool Flush() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String					m_sHeader;				/**< Holds the header text */
		String					m_sTitle;				/**< Holds the title text */
		String					m_sBackground;			/**< Holds the background */
		String					m_sDefaultTextColor;	/**< Holds the default text color */
		String					m_sDefaultTextFormat;	/**< Holds the default text format */
		String					m_sFooter;				/**< Holds the footer text */
		HashMap<uint8, String> *m_mapTextFormats;		/**< Holds the text format for each log level */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Log/LogFormatterHtml.inl"


#endif // __PLCORE_LOGFORMATTER_HTML_H__
