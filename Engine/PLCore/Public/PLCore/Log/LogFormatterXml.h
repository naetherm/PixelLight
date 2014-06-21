/*********************************************************\
 *  File: LogFormatterXml.h                              *
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


#ifndef __PLCORE_LOGFORMATTER_XML_H__
#define __PLCORE_LOGFORMATTER_XML_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    XML log formatter
*
*  @note
*    - Implementation of the strategy design pattern, this class a concrete strategy of the strategy "LogFormatter" of the context "Log"
*/
class LogFormatterXml : public LogFormatter {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API LogFormatterXml();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~LogFormatterXml();


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
		LogFormatterXml(const LogFormatterXml &cSource);

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
		LogFormatterXml &operator =(const LogFormatterXml &cSource);

		/**
		*  @brief
		*    Returns the XML formated text the given log level
		*
		*  @param[in] nLogLevel
		*    Log level for which formated text should be returned
		*
		*  @param[in] sText
		*    The log text which should be surrounded with the XML format-tags
		*
		*  @return
		*    XML formated text
		*/
		String GetXmlFormatedText(uint8 nLogLevel, const String &sText) const;


	//[-------------------------------------------------------]
	//[ Private virtual LogFormatter functions                ]
	//[-------------------------------------------------------]
	private:
		virtual bool Open(const String &sFilename) override;
		virtual bool Close() override;
		virtual bool Output(uint8 nLogLevel, const String &sText) override;
		virtual bool Flush() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_LOGFORMATTER_XML_H__
