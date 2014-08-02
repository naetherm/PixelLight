/*********************************************************\
 *  File: CompileError.h                                 *
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


#ifndef __PLCORE_TOOLS_COMPILEERROR_H__
#define __PLCORE_TOOLS_COMPILEERROR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/PLCore.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Compile time error
*
*  @remarks
*    Implementation for 'true', takes anything as parameter to the constructor and therefore raises no error
*/
template<bool>
struct CompileTimeError
{
	CompileTimeError(...) {}
};

/**
*  @brief
*    Compile time error
*
*  @remarks
*    Implementation for 'false', raising an error as it has no constructor
*/
template<>
struct CompileTimeError<false>
{
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Raise error
*
*  @param[in] error
*    Error message (must NOT be a string but a valid C++ identifier, such as My_Error_Message!)
*/
#define PLCORE_ERROR(error) \
{ \
	class ERROR_##error {}; \
	ERROR_##error _error; \
	PLCore::CompileTimeError<false> __error(_error); \
}

//	(void)sizeof( PLCore::CompileTimeError<false>(ERROR_##error()) ); \

/**
*  @brief
*    Raise error if expression is false
*
*  @param[in] expr
*	 Expression to check
*  @param[in] error
*    Error message (must NOT be a string but a valid C++ identifier, such as My_Error_Message!)
*/
#define PLCORE_CHECK(expr, error) \
{ \
	class ERROR_##error {}; \
	ERROR_##error _error; \
	PLCore::CompileTimeError<(expr)> __error(_error); \
}


#endif // __PLCORE_TOOLS_COMPILEERROR_H__
