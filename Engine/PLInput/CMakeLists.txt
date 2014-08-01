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
## Project
##################################################
pl_project(PLInput)


##################################################
## Source files
##################################################
pl_collect_source_files()


##################################################
## Dependencies
##################################################
pl_add_dependency(PLCore)
pl_add_dependency(PLMath)


##################################################
## Include directories
##################################################

if(ANDROID)
	# For the "android_native_app_glue.h"-header include
	pl_add_include_paths(${ANDROID_NDK}/sources/android/native_app_glue)
elseif(APPLE)
	# [TODO] Apple input implementation
elseif(LINUX)
	pl_add_include_paths(
		/usr/include/dbus-1.0
		/usr/lib/dbus-1.0/include
		# [TODO] check the dbus include dirs. This are the dirs 'pkg-config --cflags --libs dbus-1' returns on Ubuntu 11.10
		# Added this for Ubuntu includes and hopefully it will be the right location for most of ubuntu installations.
		/usr/include/dbus-1.0 
		/usr/lib/i386-linux-gnu/dbus-1.0/include
		# Ubuntu 11.10 64bit dbus include
		/usr/lib/x86_64-linux-gnu/dbus-1.0/include
	)
endif()

##################################################
## Additional libraries
##################################################

if(WIN32)
	pl_add_link_libs(
		${WIN32_USER32_LIB}
		${WIN32_WINMM_LIB}
		${WIN32_SETUPAPI_LIB}
	)
elseif(ANDROID)
	pl_add_link_libs(android)
elseif(APPLE)
	# [TODO] Apple input implementation
elseif(LINUX)
	pl_add_link_libs(
		${LINUX_X11_LIBS}
		dbus-1
	)
endif()


##################################################
## Preprocessor definitions
##################################################
pl_add_compile_defs(PLINPUT_EXPORTS)


##################################################
## Finalize
##################################################
pl_build_library(PLUGIN)