#*********************************************************#
#*  File: Base.cmake                                     *
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
## CMakeTools external base macros
##
## These macros can be used in any external
## scripts (Check, Fetch, Configure, Build etc.)
##################################################


##################################################
## MACRO: external_append_log
##
## Append log file
##################################################
macro(external_append_log log lastlog)
	file(READ ${lastlog} text)
	file(APPEND ${log} ${text})
endmacro(external_append_log log lastlog)

##################################################
## MACRO: external_message
##
## Output log message
##################################################
macro(external_message type message)
	file(APPEND ${CMAKETOOLS_CURRENT_LOG} "${message}\n")
	message(${type} ${message})
endmacro(external_message type message)

##################################################
## MACRO: external_makedir
##
## Create directory
##################################################
macro(external_makedir dir)
	# Message
	get_filename_component(filename ${dir} NAME)
	external_message(STATUS "Creating directory '${filename}'")

	# Create directory
	file(MAKE_DIRECTORY ${dir})
endmacro(external_makedir dir)

##################################################
## MACRO: external_removedir
##
## Delete directory
##################################################
macro(external_removedir dir)
	# Message
	get_filename_component(filename ${dir} NAME)
	external_message(STATUS "Removing directory '${filename}'")

	# Remove directory
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_removedir dir)

##################################################
## MACRO: external_remove
##
## Delete file
##################################################
macro(external_remove file)
	# Message
	get_filename_component(filename ${file} NAME)
	external_message(STATUS "Removing file '${filename}'")

	# Remove file
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E remove ${file}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_remove file)

##################################################
## MACRO: external_copy
##
## Copy file
##################################################
macro(external_copy src dest)
	# Message
	get_filename_component(filename ${src} NAME)
	external_message(STATUS "Copying '${filename}'")

	# Copy file
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy ${src} ${dest}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_copy src dest)

##################################################
## MACRO: external_copy_dir
##
## Copy directory
##################################################
macro(external_copy_dir src dest)
	# Message
	get_filename_component(filename ${src} NAME)
	external_message(STATUS "Copying directory '${filename}''")

	# Copy directory
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${src} ${dest}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_copy_dir src dst)

##################################################
## MACRO: external_rename_dir
##
## Rename directory
##################################################
macro(external_rename_dir src dest)
	# Message
	get_filename_component(src_filename  ${src}  NAME)
	get_filename_component(dest_filename ${dest} NAME)
	external_message(STATUS "Renaming directory '${src_filename}' to '${dest_filename}'")

	# Copy directory
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${src} ${dest}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})

	# Remove directory
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${src}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_rename_dir src dest)

##################################################
## MACRO: external_convert_lineendings
##
## Convert line endings (CRLF <-> LF)
##################################################
macro(external_convert_lineendings file cmd)
	# Message
	get_filename_component(filename ${file} NAME)
	external_message(STATUS "Converting line endings for '${filename}'")

	# Copy file
	execute_process(
		COMMAND ${SFK_EXECUTABLE} ${cmd} ${file}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_convert_lineendings file cmd)

##################################################
## MACRO: external_patch
##
## Apply a patch
##################################################
macro(external_patch file dir)
	# Message
	get_filename_component(filename ${file} NAME)
	external_message(STATUS "Applying patch '${filename}'")

	# Copy directory
	execute_process(
		COMMAND ${PATCH_EXECUTABLE} -p1 -i ${file}
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_patch file dir)

##################################################
## MACRO: external_generate_md5
##
## Generate MD5 checksum of a file
##################################################
macro(external_generate_md5 file var)
	# Create md5 checksum
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E md5sum ${file}
		OUTPUT_VARIABLE md5
	)

	# Not sure, if this is always correct. Using a regex might be better...
	string(SUBSTRING ${md5} 0 32 md5sum)

	# Save checksum
	file(WRITE ${file}.md5 ${md5sum})
	set(${var} ${md5sum})
endmacro(external_generate_md5 file)

##################################################
## MACRO: external_fetch_http
##
## Download a file per http
##################################################
macro(external_fetch_http url md5 destdir)
	# Get filename and compose destination
	get_filename_component(filename ${url} NAME)
	set(dest "${destdir}${filename}")

	# Check if file exists
	if(NOT EXISTS ${dest})
		# Download file
		file(DOWNLOAD ${url} ${dest})
		external_message(STATUS "File '${filename}' downloaded")
	else()
		# File already downloaded
		external_message(STATUS "File '${filename}' already downloaded")
	endif()

	# Check md5 sum
	external_generate_md5(${dest} md5_found)
	if(NOT "${md5}" STREQUAL "")
		if("${md5}" STREQUAL ${md5_found})
			external_message(STATUS "File '${filename}' MD5 sum is correct.")
		else()
			# In case of an error, give the user some hints for possible solutions
			external_message(SEND_ERROR "File '${filename}' MD5 sum is incorrect! Should be '${md5}' but is '${md5_found}'! The external package on the server might have been updated, is your source code copy up-to-date? Is the downloaded file corrupt due to a blocked or cancelled download?")
		endif()
	endif()
endmacro(external_fetch_http url md5 destdir)

##################################################
## MACRO: external_extract_tar
##
## Extract a tar archive
##################################################
macro(external_extract_tar format file dest_dir)
	# Get filename and compose destination
	get_filename_component(filename ${file} NAME)

	# Message
	external_message(STATUS "Extracting '${filename}'")

	# Check type of archive
	set(opts "xf")
	if(${format} STREQUAL "TARGZ")
		set(opts "xzf")
	endif()

	# Create target directory
	file(MAKE_DIRECTORY ${dest_dir})

	external_message(STATUS "extract ${format} ${file} to ${dest_dir}")
	# Extract archive
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar ${opts} ${file}
		WORKING_DIRECTORY ${dest_dir}
		OUTPUT_FILE ${TEMP_LOG}
		RESULT_VARIABLE ResultVar
	)
	external_message(STATUS "extract ${file} to ${dest_dir} done: ${ResultVar}")

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_extract_tar type file dest_dir)

##################################################
## MACRO: external_extract_tar2
##
## Extract a tar archive by using tar directly (not under Windows!)
##################################################
macro(external_extract_tar2 format file dest_dir)
	# Get filename and compose destination
	get_filename_component(filename ${file} NAME)

	# Message
	external_message(STATUS "Extracting '${filename}'")

	# Check type of archive
	set(opts "-xf")
	if(${format} STREQUAL "TARGZ")
		set(opts "-xzf")
	elseif(${format} STREQUAL "TARBZ2")
		set(opts "-xjf")
	endif()

	# Create target directory
	file(MAKE_DIRECTORY ${dest_dir})

	# Extract archive
	execute_process(
		COMMAND "tar" ${opts} ${file}
		WORKING_DIRECTORY ${dest_dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_extract_tar2 type file dest_dir)

##################################################
## MACRO: external_pack_tar
##
## Pack a tar archive
##################################################
macro(external_pack_tar format file source_dir)
	# Get filename and compose destination
	get_filename_component(filename ${file} NAME_WE)

	# Message
	external_message(STATUS "Packing '${filename}'")

	# Pack archive (without absolute path within the archive!)
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar -zcvf ${file} ${filename}
		WORKING_DIRECTORY "${source_dir}/.."
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_pack_tar type file source_dir)

##################################################
## MACRO: external_run_configure
##
## Run configure script
##################################################
macro(external_run_configure dir)
	# Message
	external_message(STATUS "Running configure")

	# Set additional options
	set(options ${ARGV})
	list(REMOVE_AT options 0)

	# Add cross-compile options
	if(CONFIGURE_PREFIX)
		set(options "--prefix=${CONFIGURE_PREFIX}" ${options})
	endif()
	if(CONFIGURE_HOST)
		set(options "--host=${CONFIGURE_HOST}" ${options})
	endif()

	# Execute 'configure'
	set(ENV{CC}  ${CMAKE_C_COMPILER})
	set(ENV{CXX} ${CMAKE_CXX_COMPILER})
	set(ENV{PATH} "$ENV{PATH}:${CONFIGURE_PATH}")
	execute_process(
		COMMAND "${dir}/configure" "CC=${CMAKE_C_COMPILER}" "CXX=${CMAKE_CXX_COMPILER}" ${options}
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_run_configure dir)

macro(external_run_configure2 dir)
	# Message
	external_message(STATUS "Running configure")

	# Set additional options
	set(options ${ARGV})
	list(REMOVE_AT options 0)

	# Add cross-compile options
	if(CONFIGURE_PREFIX)
		set(options "--prefix=${CONFIGURE_PREFIX}" ${options})
	endif()
	if(CONFIGURE_HOST)
		set(options "--host=${CONFIGURE_HOST}" ${options})
	endif()

	# Execute 'configure'
	set(ENV{CC}  ${CMAKE_C_COMPILER})
	set(ENV{CXX} ${CMAKE_CXX_COMPILER})
	set(ENV{PATH} "$ENV{PATH}:${CONFIGURE_PATH}")
	execute_process(
		COMMAND "${dir}/configure" "CC=${CMAKE_C_COMPILER}" "CXX=${CMAKE_CXX_COMPILER}" ${options}
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_run_configure2 dir)

##################################################
## MACRO: external_run_cmake
##
## Use CMake to configure a project
##################################################
macro(external_run_cmake dir)
	# Message
	external_message(STATUS "Running CMake")

	# Set generator
	if(CMAKETOOLS_TARGET_SYSTEM STREQUAL "Windows")
		set(generator "NMake Makefiles")
	else()
		set(generator "Unix Makefiles")
	endif()

	# Set toolchain
	set(toolchain "")
	if(CMAKE_TOOLCHAIN_FILE)
		set(toolchain -DCMAKE_TOOLCHAIN_FILE:STRING=${CMAKE_TOOLCHAIN_FILE})
	endif()

	# Set additional options
	set(options ${ARGV})
	list(REMOVE_AT options 0)

	# Execute 'cmake'
	execute_process(
		COMMAND ${CMAKE_COMMAND} . -G "${generator}" -DCMAKE_BUILD_TYPE:STRING=RELEASE ${toolchain} ${options}
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_run_cmake dir)

##################################################
## MACRO: external_run_make
##
## Run make to build a project
##################################################
macro(external_run_make dir)
	# Message
	external_message(STATUS "Running make")

	# Execute 'make'
	set(ENV{CC}  ${CMAKE_C_COMPILER})
	set(ENV{CXX} ${CMAKE_CXX_COMPILER})
	set(ENV{PATH} "$ENV{PATH}:${CONFIGURE_PATH}")
	execute_process(
		COMMAND "make"
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_run_make dir)

macro(external_run_make2 dir)
	# Message
	external_message(STATUS "Running make")

	# Execute 'make'
	set(ENV{CC}  ${CMAKE_C_COMPILER})
	set(ENV{CXX} ${CMAKE_CXX_COMPILER})
	set(ENV{PATH} "$ENV{PATH}:${CONFIGURE_PATH}")
	execute_process(
		COMMAND "make" "CC=${CMAKE_C_COMPILER}" "CXX=${CMAKE_CXX_COMPILER}"
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_run_make2 dir)

##################################################
## MACRO: external_run_nmake
##
## Run nmake to build a project
##################################################
macro(external_run_nmake file dir target)
	# Message
	external_message(STATUS "Running nmake")

	# Execute 'configure'
	execute_process(
		COMMAND "${NMAKE_EXECUTABLE}" ${file} ${target}
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
		ERROR_FILE ${TEMP_LOG}_2
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG}_2)
endmacro(external_run_nmake file dir target)

macro(external_run_nmake2 dir)
	# Message
	external_message(STATUS "Running nmake")

	# Execute 'configure'
	execute_process(
		COMMAND "${NMAKE_EXECUTABLE}"
		WORKING_DIRECTORY ${dir}
		OUTPUT_FILE ${TEMP_LOG}
		ERROR_FILE ${TEMP_LOG}_2
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG}_2)
endmacro(external_run_nmake2 dir)

##################################################
## MACRO: external_vcproj_upgrade
##
## Update a .vcproj file to the used version of Visual Studio
##################################################
macro(external_vcproj_upgrade file)
	# Message
	get_filename_component(filename ${file} NAME)
	get_filename_component(path     ${file} PATH)
	external_message(STATUS "Upgrading VC project '${filename}'")

	# Execute 'configure'
	execute_process(
		COMMAND "${VCBUILD_EXECUTABLE}" /UPGRADE ${filename}
		WORKING_DIRECTORY "${path}"
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_vcproj_upgrade file)

##################################################
## MACRO: external_vcproj_build
##
## Build a .vcproj project
##################################################
macro(external_vcproj_build file config)
	# Message
	get_filename_component(filename ${file} NAME)
	get_filename_component(path ${file} PATH)
	external_message(STATUS "Building VC project '${filename}'")

	# Execute 'configure'
	execute_process(
		COMMAND "${VCBUILD_EXECUTABLE}" ${filename} ${config}
		WORKING_DIRECTORY "${path}"
		OUTPUT_FILE ${TEMP_LOG}
	)

	# Update Log
	external_append_log(${CMAKETOOLS_CURRENT_LOG} ${TEMP_LOG})
endmacro(external_vcproj_build file config)


##################################################
## Default actions
##################################################

# Create log directory
file(MAKE_DIRECTORY ${CMAKETOOLS_CURRENT_BUILD_DIR}/Log)

# Set temp log file
set(TEMP_LOG "${CMAKETOOLS_CURRENT_BUILD_DIR}/LastLog.txt")


##################################################
## MSVC hack - late search for VC tools
##################################################

# Try to find nmake
if(NOT NMAKE_FOUND)
	# Search for nmake
	find_program(NMAKE_EXECUTABLE NAMES nmake.exe)
	if(NMAKE_EXECUTABLE)
		set(NMAKE_FOUND ON)
	else()
		set(NMAKE_FOUND OFF)
	endif()
endif()

# Try to find vcbuild
if(NOT VCBUILD_FOUND)
	# Get path hint based on location of nmake
	set(path_hint)
	if(NMAKE_FOUND)
		get_filename_component(path1 ${NMAKE_EXECUTABLE} PATH)
		get_filename_component(path2 ${path1} PATH)
		set(path_hint "${path2}/vcpackages")
	endif()

	# Search for vcbuild
	find_program(VCBUILD_EXECUTABLE NAMES vcbuild.exe PATHS ${path_hint})
	if(VCBUILD_EXECUTABLE)
		set(VCBUILD_FOUND ON)
	else()
		set(VCBUILD_FOUND OFF)
	endif()
endif()
