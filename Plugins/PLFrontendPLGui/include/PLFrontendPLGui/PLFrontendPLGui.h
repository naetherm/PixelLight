/*********************************************************\
 *  File: PLFrontendPLGui.h                              *
 *      Main header file of this project
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


#ifndef __PLFRONTENDPLGUI_PLFRONTENDPLGUI_H__
#define __PLFRONTENDPLGUI_PLFRONTENDPLGUI_H__
#pragma once


/**
*  @mainpage PLFrontendPLGui
*
*  @section intro_sec Introduction
*
*  This is the PLFrontendPLGui API reference.
*/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/PLCore.h>


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#ifdef PLFRONTENDPLGUI_EXPORTS
	// To export classes, methods and variables
	#define PLFRONTENDPLGUI_API			PL_GENERIC_API_EXPORT
#else
	// To import classes, methods and variables
	#define PLFRONTENDPLGUI_API			PL_GENERIC_API_IMPORT
#endif


#endif // __PLFRONTENDPLGUI_PLFRONTENDPLGUI_H__
