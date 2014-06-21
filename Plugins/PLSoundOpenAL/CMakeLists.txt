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
pl_project(PLSoundOpenAL)


##################################################
## Source files
##################################################
pl_collect_source_files()


##################################################
## Dependencies
##################################################
pl_add_dependency(PLCore)
pl_add_dependency(PLMath)
pl_add_dependency(PLSound)
pl_add_dependency(OpenAL)
pl_add_dependency(ogg)
pl_add_dependency(vorbis)


##################################################
## Preprocessor definitions
##################################################
pl_add_compile_defs(PLSOUNDOPENAL_EXPORTS)


##################################################
## Finalize
##################################################
pl_build_library(PLUGIN)


##################################################
## Custom install
##################################################

# External dependencies, but only when not using system-libs
#if(NOT CMAKETOOLS_USE_SYSTEM_LIBS)
#	if(WIN32)
#		add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
#			COMMAND ${CMAKE_COMMAND} -E copy ${PL_RUNTIME_BIN_GITDIR}/OpenAL32.dll ${PL_RUNTIME_BIN_DIR}
#		)
#	elseif(LINUX)
#		# Set COPY_SOURCE to files which should be copied (this variable is used by the "copy.cmake"-script)
#		set(COPY_SOURCE 
#			${PL_RUNTIME_BIN_GITDIR}/libopenal.so
#			${PL_RUNTIME_BIN_GITDIR}/libopenal.so.1
#			${PL_RUNTIME_BIN_GITDIR}/libopenal.so.1.15.1
#			${PL_RUNTIME_BIN_GITDIR}/libogg.so
#			${PL_RUNTIME_BIN_GITDIR}/libvorbis.so
#			${PL_RUNTIME_BIN_GITDIR}/libvorbisfile.so
#		)
#
#		# Set COPY_DEST to a directory to which the files in COPY_SOURCE should be copied into (this variable is used by the "copy.cmake"-script)
#		set(COPY_DEST ${PL_RUNTIME_BIN_DIR})
#
#		# Create the "copy_to_runtime.cmake"-file in the build directory of the current target (the configure file replaces the variable references with the value of the variable)
#		configure_file("${CMAKETOOLS_DIR}/Scripts/copy.cmake" ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/copy_to_runtime.cmake)
#
#		# Add custom command to call the created "copy_to_runtime.cmake"-file after the target was built
#		add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET} COMMAND ${CMAKE_COMMAND} -P "copy_to_runtime.cmake")
#	endif()
#endif()

# External dependencies, but only when not using system-libs
#if(NOT CMAKETOOLS_USE_SYSTEM_LIBS)
#	if(WIN32)
#		install(FILES ${PL_RUNTIME_BIN_DIR}/OpenAL32.dll
##					  ${PL_RUNTIME_BIN_DIR}/wrap_oal.dll
#			DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
#		)
#	elseif(LINUX)
#		install(FILES ${PL_RUNTIME_BIN_DIR}/libopenal.so
#					  ${PL_RUNTIME_BIN_DIR}/libopenal.so.1
#					  ${PL_RUNTIME_BIN_DIR}/libopenal.so.1.13.0
#					  ${PL_RUNTIME_BIN_DIR}/libogg.so
#					  ${PL_RUNTIME_BIN_DIR}/libogg.so.0
#					  ${PL_RUNTIME_BIN_DIR}/libogg.so.0.7.1
#					  ${PL_RUNTIME_BIN_DIR}/libvorbis.so
#					  ${PL_RUNTIME_BIN_DIR}/libvorbis.so.0
#					  ${PL_RUNTIME_BIN_DIR}/libvorbis.so.0.4.5
#					  ${PL_RUNTIME_BIN_DIR}/libvorbisfile.so
#					  ${PL_RUNTIME_BIN_DIR}/libvorbisfile.so.3
#					  ${PL_RUNTIME_BIN_DIR}/libvorbisfile.so.3.3.4
#			DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
#		)
#	endif()
#endif()
