/*********************************************************\
 *  File: ModuleMain.h                                   *
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


#ifndef __PLCORE_MODULEMAIN_H__
#define __PLCORE_MODULEMAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Core/MemoryManagerOnlyOncePerProject.inl"	// By adding this in here, usually every PixelLight basing project will add automatically the correct memory manager features
#include "PLCore/Base/Rtti.h"


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Defines a module inside an application or library
*
*  @param[in] ModuleName
*    Module name
*
*  @remarks
*    Use this in applications or libraries that are linked
*    directly to the main project (statically or dynamically),
*    but not inside a dynamically loaded plugin library.
*/
#define pl_module(ModuleName) \
	class ModuleInfo { \
		public: \
			ModuleInfo() { \
				ModuleInfo::Register(); \
			} \
		private: \
			static void Register() \
			{ \
				PLCore::String sName        = ModuleName; \
				PLCore::String sVendor      = "Unknown"; \
				PLCore::String sLicense     = "Unknown"; \
				PLCore::String sDescription = "Unknown module"; \

/**
*  @brief
*    Defines a module inside a plugin library
*
*  @param[in] ModuleName
*    Module name
*
*  @remarks
*    Use this in a library that is dynamically loaded as a plugin
*/
#define pl_module_plugin(ModuleName) \
	extern "C" PLCORE_PLUGIN_API bool PLIsPluginDebugBuild() \
	{ \
		return PLCORE_IS_DEBUGMODE; \
	} \
	\
	extern "C" PLCORE_PLUGIN_API int PLGetPluginInfo() \
	{ \
		return PLCore::ModuleID<int>::GetModuleID(); \
	} \
	\
	pl_module(ModuleName) \

/**
*  @brief
*    Sets the module vendor name
*
*  @param[in] Vendor
*    Vendor name
*/
#define pl_module_vendor(Vendor) \
			sVendor = Vendor; \

/**
*  @brief
*    Sets the module license
*
*  @param[in] License
*    License
*/
#define pl_module_license(License) \
			sLicense = License; \

/**
*  @brief
*    Sets the module description
*
*  @param[in] Description
*    Module description
*/
#define pl_module_description(Description) \
			sDescription = Description; \

/**
*  @brief
*    Sets the module version
*
*  @param[in] Version
*    Version string
*/
#define pl_module_version(Version)

/**
*  @brief
*    Sets the module dependencies for Windows 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define pl_module_dependencies_windows_64_release(Dependencies)
#define pl_module_dependencies_windows_32_release(Dependencies)
#define pl_module_dependencies_windows_64_debug(Dependencies)
#define pl_module_dependencies_windows_32_debug(Dependencies)

/**
*  @brief
*    Sets the module dependencies for Linux 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define pl_module_dependencies_linux_64_release(Dependencies)
#define pl_module_dependencies_linux_32_release(Dependencies)
#define pl_module_dependencies_linux_64_debug(Dependencies)
#define pl_module_dependencies_linux_32_debug(Dependencies)

/**
*  @brief
*    Sets the module dependencies for Android 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define pl_module_dependencies_android_64_release(Dependencies)
#define pl_module_dependencies_android_32_release(Dependencies)
#define pl_module_dependencies_android_64_debug(Dependencies)
#define pl_module_dependencies_android_32_debug(Dependencies)

/**
*  @brief
*    Sets the module dependencies for Mac OS X 32/64 bit release/debug mode
*
*  @param[in] Dependencies
*    Module dependencies
*/
#define pl_module_dependencies_macosx_64_release(Dependencies)
#define pl_module_dependencies_macosx_32_release(Dependencies)
#define pl_module_dependencies_macosx_64_debug(Dependencies)
#define pl_module_dependencies_macosx_32_debug(Dependencies)

/**
*  @brief
*    Sets the module's default active state
*
*  @param[in] Active
*    Default active state (1 or 0)
*
*  @note
*    - If not other defined, modules are active by default
*/
#define pl_module_active(Active)

/**
*  @brief
*    Sets the module's default delayed shared library loading state
*
*  @param[in] Delayed
*    Default delayed shared library loading state (1 or 0)
*
*  @note
*    - If not other defined, modules are loading their shared libraries delayed by default
*/
#define pl_module_delayed(Delayed)

/**
*  @brief
*    Ends the module definition
*/
#define pl_module_end \
			PLCore::ModuleID<int>::RegisterModule(sName, sVendor, sLicense, sDescription); \
		} \
	}; \
	\
	ModuleInfo __ModuleInfo;


#endif // __PLCORE_MODULEMAIN_H__
