/*********************************************************\
 *  File: PLFrontendQt.h                                 *
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


/**
*  @mainpage PLFrontendQt
*
*  @section intro_sec Introduction
*
*  This is the PLFrontendQt reference. The PLFrontendQt adapter project fulfils multiple purposes.
*
*  There are generic helpers like the static adapter class for mapping Qt strings to PixelLight
*  strings and vice versa. By using those helpers as well as provided data models and so on it's
*  possible to add PixelLight functionality into Qt applications. In other words: Qt is the master,
*  PixelLight is the slave.
*
*  On the other hand this project also supports the other way around were PixelLight is the master
*  and Qt the slave. This is accomplished by adding an "PLCore::FrontendImpl"-implementation using
*  Qt. Internally Qt has the main loop, but due to the frontend approach this is nothing the user
*  has to care about. One is just using PixelLight. When using "PLFrontend::Frontend" as PixelLight
*  frontend, it's possible to access Qt functionality easily to add a GUI. Additionally, there are
*  advanced features like the RTTI dock widgets making it possible to add new dock widgets to the
*  main window in an universal way, even by implementing them in separate plugins.
*/


#ifndef __PLFRONTENDQT_H__
#define __PLFRONTENDQT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qglobal.h>
#include <PLCore/PLCore.h>


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#ifdef PLFRONTENDQT_EXPORTS
	// To export classes, methods and variables
	#define PLFRONTENDQT_API			PL_GENERIC_API_EXPORT
#else
	// To import classes, methods and variables
	#define PLFRONTENDQT_API			PL_GENERIC_API_IMPORT
#endif


#endif // __PLFRONTENDQT_H__
