#*********************************************************#
#*  File: CMakeLists.txt                                 *
#*
#*  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
#*
#*  This file is part of PixelLight.
#*
#*  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
#*  and associated documentation files (the "Software"), to deal in the Software without
#*  restriction, including without limitation the rights to use, copy, modify, merge, publish,
#*  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
#*  Software is furnished to do so, subject to the following conditions:
#*
#*  The above copyright notice and this permission notice shall be included in all copies or
#*  substantial portions of the Software.
#*
#*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
#*  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#*  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
#*  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#*********************************************************#


##################################################
## Architecture
##################################################

# Check the architecture of the host system
# There is currently a bug in CMake that makes it report 32 bit architecture even on 64 bit system. This is why we don't use the
# CMAKE_SYSTEM_PROCESSOR value. We prefer the native bitsize for the system, however, for MS Visual Studio, we always pick the
# bitsize specified by the generator (as it won't compile otherwise!)
if(CMAKE_GENERATOR MATCHES "Visual Studio .. Win64")
	set(X86_64 1)
elseif(CMAKE_GENERATOR MATCHES "Visual Studio*")
	set(X86 1)
elseif(CMAKE_SIZEOF_VOID_P MATCHES 8)
	set(X86_64 1)
else()
	set(X86 1)
endif()

# Target architecture (x86, arm...)
if(PL_TARGET_ARCH)
	set(PL_TARGET_ARCH ${PL_TARGET_ARCH} CACHE ADVANCED "Target architecture")
else()
	set(PL_TARGET_ARCH "x86" CACHE ADVANCED "Target architecture")
endif()

# Set default bit size depending on the host system architecture (currently set 64bit only for x86_64 hosts)
if(X86_64 AND PL_TARGET_ARCH MATCHES "x86")
	# Target bitsize (32/64)
	set(PL_TARGET_BITSIZE "64" CACHE ADVANCED "Target bitsize")

	# Target architecture & bitsize (x86, x64 - yes, there are many names for the 64 bit version, but e.g. "x64" is just more handy as the technically "x86_64")
	set(PL_TARGET_ARCHBITSIZE "x64" CACHE ADVANCED "Target architecture & bitsize")
else()
	# Target bitsize (32/64)
	set(PL_TARGET_BITSIZE "32" CACHE ADVANCED "Target bitsize")

	# Target architecture & bitsize (x86, x64 - yes, there are many names for the 64 bit version, but e.g. "x64" is just more handy as the technically "x86_32")
	set(PL_TARGET_ARCHBITSIZE ${CMAKETOOLS_TARGET_ARCH} CACHE ADVANCED "Target architecture & bitsize")
endif()


##################################################
## Platform-specific folders
##################################################

# This list will initially contain all the supported platform. Each platform-specific
# include can remove from this to enable the source files relevant for the platform
set(PL_UNSUPPORTED_PLATFORMS Windows Linux MaxOSX Android)

# This is the exact opposite of the above variable
set(PL_SUPPORTED_PLATFORMS "")

##################################################
## Platform-specific includes
##################################################

if(WIN32)
	include(CMake/Platforms/WinMSVC.cmake)
elseif(LINUX)

	# Common settings
	include(CMake/Platforms/LinuxCommon.cmake)

	# Android NDK ("CMAKE_CXX_COMPILER_ID" matches also "GNU", so this test must come first)
	if(ANDROID)
		include(CMake/Platforms/LinuxNDK.cmake)

	# NaCL ("CMAKE_CXX_COMPILER_ID" matches also "GNU", so this test must come first)
	elseif(NACL)
		include(CMake/Platforms/LinuxNaCL.cmake)

	# GCC
	elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
		include(CMake/Platforms/LinuxGCC.cmake)

	# Clang
	elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		include(CMake/Platforms/LinuxClang.cmake)

	# ?
	else()
		message(FATAL_ERROR "Unsupported compiler, use GNU, Clang or NDK")
	endif()
endif()