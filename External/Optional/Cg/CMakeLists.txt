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
define_external_project(Cg)

##################################################
## Look for Cg
##################################################
# Set this value first, it will be used by the script (and that one does not do it properly it seems)
if(CMAKETOOLS_TARGET_BITSIZE MATCHES "64")
	set(NV_SYSTEM_PROCESSOR "x86_64" CACHE INTERNAL "")
else()
	set(NV_SYSTEM_PROCESSOR "x86" CACHE INTERNAL "")
endif()

# We want to look for Cg always on Windows and Mac, for Linux we have binary distribution
if(WIN32 OR NOT CMAKETOOLS_DONT_USE_SYSTEM_LIBS)
	find_package(Cg)
endif()

##################################################
## Configuration
##################################################
if(WIN32)
	# For Windows, there is no binary distribution save the official installer. We don't go through
	# the hassle of re-packing the binaries ourselves (who knows what the licensing implication are)
	# and just softly fail if the package is not installed. Our official SDK installer should offer
	# the Cg Toolkit for installation anyway.	
	if(NOT CG_FOUND)	
		message(STATUS "Cg Toolkit is not installed, download it from http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_Setup.exe")
		
		# We cannot build any of the dependent projects
		unset(PL_PLUGIN_RENDERER_OPENGLCG CACHE)
	else()
		set(CG_INCLUDE_DIRS ${CG_INCLUDE_DIR} CACHE INTERNAL "")
		set(CG_OPENGL_LIBRARIES ${CG_LIBRARY} ${CG_GL_LIBRARY} CACHE INTERNAL "")
		
		# We need to copy over cg.dll and cgGl.dll
		# For now we do it directly here but in the future it would be better to create a custom target
		# and do this in its build phase
		string(REGEX REPLACE "/lib" "/bin" CG_DLL ${CG_LIBRARY})
		string(REGEX REPLACE ".lib" ".dll" CG_DLL ${CG_DLL})
		file(COPY ${CG_DLL} DESTINATION ${PL_RUNTIME_BIN_GITDIR})
		
		string(REGEX REPLACE "/lib" "/bin" CG_GL_DLL ${CG_GL_LIBRARY})
		string(REGEX REPLACE ".lib" ".dll" CG_GL_DLL ${CG_GL_DLL})
		file(COPY ${CG_GL_DLL} DESTINATION ${PL_RUNTIME_BIN_GITDIR})
	endif()
else()
	if(NOT Cg_FOUND)
		# Download and unpack the binary distribution from NVIDIA's site
		if(CMAKETOOLS_TARGET_BITSIZE MATCHES "64")
			set(CG_URL "http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_x86_64.tgz")
			set(CG_LIB_DIR lib64)
		else()
			set(CG_URL "http://developer.download.nvidia.com/cg/Cg_3.1/Cg-3.1_April2012_x86.tgz")
			set(CG_LIB_DIR lib)
		endif()
		
		ExternalProject_Add(${CMAKETOOLS_CURRENT_TARGET}
			DOWNLOAD_DIR ${CMAKETOOLS_CURRENT_DOWNLOAD_DIR}
			SOURCE_DIR ${CMAKETOOLS_CURRENT_SRC_DIR}
			BINARY_DIR ${CMAKETOOLS_CURRENT_BUILD_DIR}
			INSTALL_DIR ${CMAKETOOLS_CURRENT_INSTALL_DIR}
			TMP_DIR ${CMAKETOOLS_CURRENT_TMP_DIR}
			STAMP_DIR ${CMAKETOOLS_CURRENT_STAMP_DIR}
			URL ${CG_URL}
#			URL_MD5 ${ASSIMP_MD5}

			# No configure or build
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ""

			# Install just copies the proper .so files to the install directory
			INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/${CG_LIB_DIR}/libCg.so <INSTALL_DIR>/lib/libCg.so
				COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/${CG_LIB_DIR}/libCgGL.so <INSTALL_DIR>/lib/libCgGL.so
				COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/include/Cg/cg.h <INSTALL_DIR>/include/Cg/cg.h
				COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/include/Cg/cgGL.h <INSTALL_DIR>/include/Cg/cgGL.h				
		)

		# Setup paths
		set(CG_INCLUDE_DIRS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/include CACHE INTERNAL "")
		set(CG_OPENGL_LIBRARIES ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libCg.so ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libCgGL.so CACHE INTERNAL "")

		# Also copy the runtime libraries to our runtime directory
		ExternalProject_Add_Step(${CMAKETOOLS_CURRENT_TARGET} CopyDLL
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libCg.so ${PL_RUNTIME_BIN_GITDIR}
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libCgGL.so ${PL_RUNTIME_BIN_GITDIR}
			DEPENDEES install
		)
	else()
		# Cg found on this system, setup paths
		set(CG_INCLUDE_DIRS ${CG_INCLUDE_DIR} CACHE INTERNAL "")
		set(CG_OPENGL_LIBRARIES ${CG_LIBRARY} ${CG_GL_LIBRARY} CACHE INTERNAL "")

		# Also copy the runtime libs to runtime directory
		# [TODO] Add step? (see Windows path)
		file(COPY ${CG_LIBRARY} DESTINATION ${PL_RUNTIME_BIN_GITDIR})
		file(COPY ${CG_GL_LIBRARY} DESTINATION ${PL_RUNTIME_BIN_GITDIR})
	endif()
endif()
