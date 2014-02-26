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
define_external_project(LuaJIT)

##################################################
## Configuration
##################################################
set(LUAJIT_VERSION "2.0.2")
set(LUAJIT_URL "http://luajit.org/download/LuaJIT-${LUAJIT_VERSION}.tar.gz")
set(LUAJIT_MD5 "112dfb82548b03377fbefbba2e0e3a5b")

##################################################
#[TODO] There are CMake-based git repos for Lua and LuaJIT at LuaDist
# 		We might want to use these for clarity
##################################################

##################################################
## Need to download?
##################################################
# First look if we have the library installed already. This will be the case for the majority of Linux systems
#TODO: what about debug configuration?
message(STATUS "Looking for external dependency 'LuaJIT' with minimum version ${LUAJIT_VERSION}...")
#find_package(LUAJIT ${LUAJIT_VERSION})

# Do we have it?
#if(LUAJIT_FOUND)
#	# We will use the library present on the user system
#	message(STATUS "'LuaJIT' was found on this system")
#	
#	# The find module from CMake itself doesn't add the XXX_LIBRARIES variable and XXX_INCLUDE_DIR variable to the cache, which is the correct behaviour.
#	# These variables should normaly only be visible within the cmake file in which the find module was called.
#	# Otherwise the variables would be visible for all other projects if wanted or not
#	# We want this, because this variables are used in other CMake files, which doesn't include this file, so we add the variables to the cache
#	set(LUAJIT_LIBRARIES ${LUAJIT_LIBRARIES} CACHE INTERNAL "")
#	set(LUAJIT_LIBRARY ${LUAJIT_LIBRARIES} CACHE INTERNAL "")
#	set(LUAJIT_INCLUDE_DIR ${LUAJIT_INCLUDE_DIR} CACHE INTERNAL "")
#else()
	# We'll need to download and build the library ourselves
	message(STATUS "'LuaJIT' was not found, will be downloaded and built from [${LUAJIT_URL}]")
	
	# Define the external project
	if(MSVC)
		ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
			DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
			SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
			INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
			TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
			STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
			URL ${LUAJIT_URL}
			URL_MD5 ${LUAJIT_MD5}
			TIMEOUT 30
			BUILD_IN_SOURCE 1
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ${CMAKE_COMMAND} -E chdir src msvcbuild.bat
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy src/lua51.dll <INSTALL_DIR>/bin/lua51.dll
			COMMAND ${CMAKE_COMMAND} -E copy src/lua51.dll ${PL_RUNTIME_BIN_GITDIR}/lua51.dll
            COMMAND ${CMAKE_COMMAND} -E copy src/lua51.lib <INSTALL_DIR>/lib/lua51.lib
            COMMAND ${CMAKE_COMMAND} -E copy src/lauxlib.h "<INSTALL_DIR>/include/Lua/lauxlib.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/luaconf.h "<INSTALL_DIR>/include/Lua/luaconf.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/lua.h "<INSTALL_DIR>/include/Lua/lua.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/luajit.h "<INSTALL_DIR>/include/Lua/luajit.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/lualib.h "<INSTALL_DIR>/include/Lua/lualib.h"
		)
	elseif(UNIX)
		ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
			DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
			SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
			INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
			TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
			STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
			URL ${LUAJIT_URL}
			URL_MD5 ${LUAJIT_MD5}
			TIMEOUT 30
			BUILD_IN_SOURCE 1
			CONFIGURE_COMMAND ""
			BUILD_COMMAND make
			#[TODO] What is the .so here?
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy src/libluajit.a <INSTALL_DIR>/bin/liblua.a
			COMMAND ${CMAKE_COMMAND} -E copy src/lua51.dll ${PL_RUNTIME_BIN_GITDIR}/lua51.dll
			
            COMMAND ${CMAKE_COMMAND} -E copy src/libluajit.a <INSTALL_DIR>/bin/liblua.a
            COMMAND ${CMAKE_COMMAND} -E copy src/lauxlib.h "<INSTALL_DIR>/include/Lua/lauxlib.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/luaconf.h "<INSTALL_DIR>/include/Lua/luaconf.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/lua.h "<INSTALL_DIR>/include/Lua/lua.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/luajit.h "<INSTALL_DIR>/include/Lua/luajit.h"
            COMMAND ${CMAKE_COMMAND} -E copy src/lualib.h "<INSTALL_DIR>/include/Lua/lualib.h"
		)
	else()
		#[TODO] Revert to classical Lua
		message(FATAL_ERROR "Cannot use LuaJIT with CMake generator ${CMAKE_GENERATOR}")
	endif()
	
	if(WIN32)
		# On Windows, the output .lib file has a different name for debug configuration
		#set(LUAJIT_LIBRARIES optimized ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstatic.lib debug ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstaticd.lib CACHE INTERNAL "")
		set(LUA_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/lua51.lib CACHE INTERNAL "")
	else()
		set(LUA_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/liblua.a CACHE INTERNAL "")
	endif()
	set(LUA_INCLUDE_DIRS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include/Lua CACHE INTERNAL "")
	
	add_dependencies(External-Recommended ${CMAKETOOLS_CURRENT_TARGET})
#endif()
