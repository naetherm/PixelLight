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
pl_project(PLCore)


##################################################
## Source files
##################################################

# Include Zip support if not disabled
set(add_features)
if(NOT PL_NO_ZIP_SUPPORT)
	set(add_features Zip)
endif()

pl_collect_source_files(${add_features})


##################################################
## Dependencies
##################################################

pl_add_dependency(pcre)

if(NOT PL_NO_ZIP_SUPPORT)
	pl_add_dependency(zlib)
endif()


##################################################
## Additional libraries
##################################################

if(WIN32)
	pl_add_link_libs(${WIN32_STANDARD_LIBS} ${WIN32_WSOCK32_LIB} ${WIN32_USER32_LIB})
elseif(LINUX)
	pl_add_link_libs(dl)

	# There's no native curses library on Android, and compiling one
	# just for this usually never used function would be overkill
	# (used within "PLCore::ConsoleLinux::GetCursorPosition()" & "PLCore::ConsoleLinux::SetCursorPosition()")
	# No need to link "pthread", it's already within Androids c library
	if(ANDROID)
		pl_add_link_libs(log android)
	else()
		pl_add_link_libs(pthread ncurses)
	endif()
endif()


##################################################
## Preprocessor definitions
##################################################
pl_add_compile_defs(PLCORE_EXPORTS)

if(PL_NO_ZIP_SUPPORT)
	# Altough not recommended, it's possible to live without ZIP support for minimal builds
	pl_add_compile_defs(DISABLE_ZIP_SUPPORT)
endif()

if(WIN32)
	pl_add_compile_defs(PCRE_STATIC)
endif()


##################################################
## Finalize
##################################################
pl_build_library(SHARED)


##################################################
## Build static version of PLCore
##################################################

# Change the definition setup
#if(NOT LINUX)
#	pl_remove_compile_defs(PLCORE_EXPORTS)
#endif()
#pl_add_compile_defs(PLCORE_STATIC)

# Build it
#pl_project(PLCoreStat NORESET)
#pl_build_library(STATIC NODOC)

# Install
#install(
#	TARGETS ${PL_CURRENT_TARGET_NAME}
#	RUNTIME DESTINATION ${PL_BIN_DIR} COMPONENT Runtime
#	LIBRARY DESTINATION ${PL_BIN_DIR} COMPONENT Runtime
#	ARCHIVE DESTINATION ${PL_LIB_DIR} COMPONENT SDK
#)