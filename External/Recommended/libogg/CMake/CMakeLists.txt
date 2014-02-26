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

cmake_minimum_required(VERSION 2.8)

project(libogg C)

if(NOT MSVC AND NOT CMAKE_BUILD_TYPE)
    #SET(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "No CMAKE_BUILD_TYPE specified, defaulting to ${CMAKE_BUILD_TYPE}") 
endif()

set(libogg_SRCS src/bitwise.c src/framing.c)
set(libogg_HDRS include/ogg/ogg.h include/ogg/os_types.h)

include_directories(include)

if(MSVC)
    add_definitions(/D_UNICODE /DUNICODE)
    list(APPEND libogg_SRCS win32/ogg.def)
endif()

if(LINUX)
	# libogg expects configure to be called on linux to
	# generate config_types.h
    list(APPEND libogg_HDRS include/ogg/config_types.h)

	add_custom_command(
		OUTPUT include/ogg/config_types.h
		COMMAND ./configure
		DEPENDS include/ogg/ogg.h # Hopefully if the libogg version changes, so does this file
								  # so configure_types.h will be regenerated.
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endif()

add_library(libogg ${libogg_SRCS} ${libogg_HDRS})

install(
	TARGETS libogg
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(FILES ${libogg_HDRS} DESTINATION include/ogg)