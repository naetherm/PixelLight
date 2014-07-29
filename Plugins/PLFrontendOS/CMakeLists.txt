#*********************************************************#
#*  File: CMakeLists.txt                                 *
#*
#*  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
#*
#*  This file is part of PixelLight.
#*
#*  PixelLight is free software: you can redistribute it and/or modify
#*  it under the terms of the GNU Lesser General Public License as published by
#*  the Free Software Foundation, either version 3 of the License, or
#*  (at your option) any later version.
#*
#*  PixelLight is distributed in the hope that it will be useful,
#*  but WITHOUT ANY WARRANTY; without even the implied warranty of
#*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#*  GNU Lesser General Public License for more details.
#*
#*  You should have received a copy of the GNU Lesser General Public License
#*  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
#*********************************************************#


##################################################
## Project
##################################################
pl_project(PLFrontendOS)


##################################################
## Source files
##################################################
pl_collect_source_files()

# [TODO] Currently, this will collect Frontend.cpp and OSWindow.cpp files for all platforms,
# but they should be actually excluded on Android. Also, Android should include 
# file '${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c'


##################################################
## Dependencies
##################################################
pl_add_dependency(PLCore)

# [TODO] Add include directory "${ANDROID_NDK}/sources/android/native_app_glue" for Android


##################################################
## Additional libraries
##################################################
if(ANDROID)
	pl_add_link_libs(log android)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	pl_add_link_libs(${LINUX_X11_LIBS})
endif()


##################################################
## Preprocessor definitions
##################################################
pl_add_compile_defs(PLFRONTENDOS_EXPORTS)

# [TODO] Add linker flag '-framework Cocoa' on Mac


##################################################
## Finalize
##################################################
pl_build_library(PLUGIN)