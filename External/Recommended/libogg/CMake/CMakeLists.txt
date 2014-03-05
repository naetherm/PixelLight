## The CMake script used to build libogg during externals build

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