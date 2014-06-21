/*********************************************************\
 *  File: FrontendMain.h                                 *
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


#ifndef __PLCORE_FRONTENDMAIN_H__
#define __PLCORE_FRONTENDMAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Main.h"
#include "PLCore/Runtime.h"
#include "PLCore/ModuleMain.h"
#include "PLCore/Frontend/Frontend.h"
#include "PLCore/Frontend/FrontendContext.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Defines a default frontend for a module inside a plugin library
*
*  @param[in] ModuleName
*    Module name
*  @param[in] ApplicationClass
*    Name of the application RTTI class to use (must be derived from "PLCore::FrontendApplication")
*
*  @remarks
*    Use this when you don't want to care about the program entry point.
*/
#define pl_module_application(ModuleName, ApplicationClass) \
	int PLMain(const PLCore::String &sExecutableFilename, const PLCore::Array<PLCore::String> &lstArguments) \
	{ \
		/* Scan PL-runtime directory for compatible plugins and load them in as well as scan for compatible data and register it */ \
		if (PLCore::Runtime::ScanDirectoryPluginsAndData()) { \
			/* Setup the frontend context */ \
			PLCore::FrontendContext cFrontendContext; \
			cFrontendContext.SetExecutableFilename(sExecutableFilename); \
			cFrontendContext.SetArguments(lstArguments); \
			cFrontendContext.SetName(ModuleName); \
			cFrontendContext.SetFrontendParameters(PLCore::String("ApplicationClass=\"") + ApplicationClass + '\"'); \
	\
			/* Run the frontend */ \
			return PLCore::Frontend::Run(cFrontendContext); \
		} else { \
			/* Error! */ \
			return -1; \
		} \
	} \
	\
	pl_module_plugin(ModuleName)

/**
*  @brief
*    Defines a frontend for a module inside a plugin library
*
*  @param[in] ModuleName
*    Module name
*  @param[in] ApplicationClass
*    Name of the application RTTI class to use (must be derived from "PLCore::FrontendApplication")
*  @param[in] FrontendClass
*    Name of the frontend implementation RTTI class to use (e.g. "PLFrontendOS::Frontend")
*
*  @remarks
*    Use this when you don't want to care about the program entry point.
*/
#define pl_module_application_frontend(ModuleName, ApplicationClass, FrontendClass) \
	int PLMain(const PLCore::String &sExecutableFilename, const PLCore::Array<PLCore::String> &lstArguments) \
	{ \
		/* Scan PL-runtime directory for compatible plugins and load them in as well as scan for compatible data and register it */ \
		if (PLCore::Runtime::ScanDirectoryPluginsAndData()) { \
			/* Setup the frontend context */ \
			PLCore::FrontendContext cFrontendContext; \
			cFrontendContext.SetExecutableFilename(sExecutableFilename); \
			cFrontendContext.SetArguments(lstArguments); \
			cFrontendContext.SetName(ModuleName); \
			cFrontendContext.SetFrontendParameters(PLCore::String("ApplicationClass=\"") + ApplicationClass + '\"'); \
			cFrontendContext.SetFrontendImplementation(FrontendClass); \
	\
			/* Run the frontend */ \
			return PLCore::Frontend::Run(cFrontendContext); \
		} else { \
			/* Error! */ \
			return -1; \
		} \
	} \
	\
	pl_module_plugin(ModuleName)


#endif // __PLCORE_FRONTENDMAIN_H__
