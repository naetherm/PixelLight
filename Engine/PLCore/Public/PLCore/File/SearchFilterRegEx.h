/*********************************************************\
 *  File: SearchFilterRegEx.h                            *
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


#ifndef __PLCORE_FILE_SEARCHFILTERREGEX_H__
#define __PLCORE_FILE_SEARCHFILTERREGEX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/RegEx.h"
#include "PLCore/File/SearchFilter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Search filter working with a regular expression (PCRE syntax)
*
*  @see
*    - 'RegEx'-class
*/
class SearchFilterRegEx : public SearchFilter {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sExpression
		*    Regular expression
		*  @param[in] nMode
		*    Processing mode (combination of EMatch values)
		*/
		PLCORE_API SearchFilterRegEx(const String &sExpression, uint32 nMode = RegEx::MatchCaseSensitive | RegEx::MatchGreedy);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~SearchFilterRegEx();

		/**
		*  @brief
		*    Get expression
		*
		*  @return
		*    Regular expression
		*/
		PLCORE_API String GetExpression() const;


	//[-------------------------------------------------------]
	//[ Public virtual SearchFilter functions                 ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual bool CheckFile(const String &sFilename) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RegEx m_cRegEx;		/**< Regular expression */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FILE_SEARCHFILTERREGEX_H__
