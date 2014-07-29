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
pl_project(PLScriptLua)


##################################################
## Source files
##################################################
pl_collect_source_files()


##################################################
## Dependencies
##################################################
pl_add_dependency(PLCore)
pl_add_dependency(LuaJIT)


##################################################
## Additional libraries
##################################################
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	pl_add_link_libs(dl)
endif()


##################################################
## Preprocessor definitions
##################################################
pl_add_compile_defs(PLSCRIPTLUA_EXPORTS)


##################################################
## Finalize
##################################################
pl_build_library(PLUGIN)


##################################################
## Custom install
##################################################

# External dependencies, but only when not using system-libs
# [TODO] Uncomment this as soon as there's support for system-libs
#if(NOT CMAKETOOLS_USE_SYSTEM_LIBS)
#	if(WIN32)
#		install(FILES ${PL_RUNTIME_BIN_DIR}/lua51.dll
#			DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
#		)
#	endif()
#endif()
