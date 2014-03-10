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
define_project(PLCore)

##################################################
## Source files
##################################################
add_sources(
	src/PLCore.cpp
	src/Runtime.cpp
	src/Container/Functions.cpp
	src/Container/Bitset.cpp
	src/Container/BitsetIterator.cpp
	src/Core/AbstractContext.cpp
	src/Core/AbstractLifecycle.cpp
	src/File/Directory.cpp
	src/File/File.cpp
	src/File/FileAccess.cpp
	src/File/FileHttp.cpp
	src/File/FileImpl.cpp
	src/File/FileStdStream.cpp
	src/File/FileObject.cpp
	src/File/FileSearch.cpp
	src/File/FileSearchImpl.cpp
	src/File/FileSearchHttp.cpp
	src/File/HttpHandle.cpp
	src/File/SearchFilter.cpp
	src/File/SearchFilterRegEx.cpp
	src/File/SearchFilterWildcard.cpp
	src/File/Url.cpp
	src/Network/Network.cpp
	src/Network/Socket.cpp
	src/Network/SocketAddress.cpp
	src/Network/Host.cpp
	src/Network/Client.cpp
	src/Network/Server.cpp
	src/Network/Connection.cpp
	src/Network/Buffer.cpp
	src/Network/Http/Http.cpp
	src/Network/Http/HttpClient.cpp
	src/Network/Http/HttpClientConnection.cpp
	src/Network/Http/HttpServer.cpp
	src/Network/Http/HttpServerConnection.cpp
	src/Network/Http/HttpHeader.cpp
	src/Network/Http/MimeTypeManager.cpp
	src/Registry/Registry.cpp
	src/Registry/RegistryImpl.cpp
	src/String/BufferedReaderFile.cpp
	src/String/BufferedReaderString.cpp
	src/String/RegEx.cpp
	src/String/String.cpp
	src/String/StringBuffer.cpp
	src/String/StringBufferASCII.cpp
	src/String/StringBufferUnicode.cpp
	src/String/StringBufferUTF8.cpp
	src/String/StringBufferManager.cpp
	src/String/Tokenizer.cpp
	src/String/UTF8Tools.cpp
	src/String/ParseTools.cpp
	src/System/DynLib.cpp
	src/System/DynLibImpl.cpp
	src/System/Mutex.cpp
	src/System/MutexImpl.cpp
	src/System/Semaphore.cpp
	src/System/SemaphoreImpl.cpp
	src/System/System.cpp
	src/System/SystemImpl.cpp
	src/System/Console.cpp
	src/System/Thread.cpp
	src/System/ThreadImpl.cpp
	src/System/ThreadFunction.cpp
	src/System/Pipe.cpp
	src/System/Process.cpp
	src/Xml/XmlBase.cpp
	src/Xml/XmlElement.cpp
	src/Xml/XmlNode.cpp
	src/Xml/XmlText.cpp
	src/Xml/XmlAttribute.cpp
	src/Xml/XmlComment.cpp
	src/Xml/XmlUnknown.cpp
	src/Xml/XmlDocument.cpp
	src/Xml/XmlDeclaration.cpp
	src/Xml/XmlParsingData.cpp
	src/Log/Log.cpp
	src/Log/LogFormatter.cpp
	src/Log/LogFormatterXml.cpp
	src/Log/LogFormatterHtml.cpp
	src/Log/LogFormatterText.cpp
	src/Base/Class.cpp
	src/Base/ClassImpl.cpp
	src/Base/ClassReal.cpp
	src/Base/ClassDummy.cpp
	src/Base/ClassManager.cpp
	src/Base/MemberDesc.cpp
	src/Base/Module.cpp
	src/Base/Object.cpp
	src/Base/Event/DynEvent.cpp
	src/Base/Event/DynEventHandler.cpp
	src/Base/Event/EventDesc.cpp
	src/Base/Event/EventHandlerDesc.cpp
	src/Base/Func/ConstructorDesc.cpp
	src/Base/Func/DynFunc.cpp
	src/Base/Func/DynParams.cpp
	src/Base/Func/FuncDesc.cpp
	src/Base/Func/DynSignature.cpp
	src/Base/Tools/ParamsParser.cpp
	src/Base/Tools/ParamsParserXml.cpp
	src/Base/TypeInfo/DynTypeInfo.cpp
	src/Base/Var/DynVar.cpp
	src/Base/Var/VarDesc.cpp
	src/Config/Config.cpp
	src/Config/ConfigLoader.cpp
	src/Config/ConfigLoaderPL.cpp
	src/Config/CoreConfigGroup.cpp
	src/Tools/Profiling.cpp
	src/Tools/QuickSort.cpp
	src/Tools/Stopwatch.cpp
	src/Tools/Time.cpp
	src/Tools/Timing.cpp
	src/Tools/Version.cpp
	src/Tools/Wrapper.cpp
	src/Tools/CommandLine.cpp
	src/Tools/HTMLParser.cpp
	src/Tools/Checksum.cpp
	src/Tools/ChecksumCRC32.cpp
	src/Tools/ChecksumMD5.cpp
	src/Tools/ChecksumSHA1.cpp
	src/Tools/Loadable.cpp
	src/Tools/LoadableManager.cpp
	src/Tools/LoadableType.cpp
	src/Tools/Loader.cpp
	src/Tools/LoaderImpl.cpp
	src/Tools/Chunk.cpp
	src/Tools/ChunkLoader.cpp
	src/Tools/ChunkLoaderPL.cpp
	src/Tools/ChunkLoaderPLXml.cpp
	src/Tools/Localization.cpp
	src/Tools/LocalizationGroup.cpp
	src/Tools/LocalizationText.cpp
	src/Tools/LocalizationLoader.cpp
	src/Tools/LocalizationLoaderPL.cpp
	src/Application/ApplicationContext.cpp
	src/Application/CoreApplication.cpp
	src/Script/Script.cpp
	src/Script/ScriptManager.cpp
	src/Script/ScriptBinding.cpp
	src/Frontend/AbstractFrontend.cpp
	src/Frontend/Frontend.cpp
	src/Frontend/FrontendImpl.cpp
	src/Frontend/FrontendContext.cpp
	src/Frontend/FrontendPixelLight.cpp
	# src/Frontend/FrontendOpenGL.cpp # Just a test, not added in here
	src/Frontend/FrontendApplication.cpp
)
if(PL_CORE_ZIP)
	# It's highly recommended to enable ZIP support
	add_sources(
		src/File/FileZip.cpp
		src/File/ZipHandle.cpp
		src/File/FileSearchZip.cpp
	)
endif()
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_sources(
		src/File/FileWindows.cpp
		src/File/FileSearchWindows.cpp
		src/Registry/RegistryWindows.cpp
		src/System/SystemWindows.cpp
		src/System/ConsoleWindows.cpp
		src/System/DynLibWindows.cpp
		src/System/ThreadWindows.cpp
		src/System/MutexWindows_Mutex.cpp
		src/System/MutexWindows_CriticalSection.cpp
		src/System/SemaphoreWindows.cpp
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_sources(
		src/PLCoreLinux.cpp
		src/File/FileLinux.cpp
		src/File/FileSearchLinux.cpp
		src/System/SystemLinux.cpp
		src/System/ConsoleLinux.cpp
		src/System/DynLibLinux.cpp
		src/System/ThreadLinux.cpp
		src/System/MutexLinux.cpp
		src/System/SemaphoreLinux.cpp
	)

	# The Android port is reusing the Linux port whenever possible
	if(ANDROID)
		##################################################
		## Android
		##################################################
		add_sources(
			src/File/FileAndroid.cpp
			src/File/FileSearchAndroid.cpp
			src/System/SystemAndroid.cpp
			src/System/ConsoleAndroid.cpp
		)

	# The Mac OS X port is reusing the Linux port whenever possible
	elseif(APPLE)
		##################################################
		## Mac OS X
		##################################################
		add_sources(
			src/System/SystemMacOSX.cpp
		)
	endif()
endif()

##################################################
## Include directories
##################################################
add_include_directories(
	include
	${PCRE_INCLUDE_DIR}
	${CMAKE_BINARY_DIR}	# For "PixelLight.h"
)
if(PL_CORE_ZIP)
	# It's highly recommended to enable ZIP support
	add_include_directories(
		${ZLIB_INCLUDE_DIR}
	)
endif()

##################################################
## Library directories
##################################################
add_link_directories(
	${PL_LIB_DIR}
)

##################################################
## Additional libraries
##################################################
add_libs(
	${PCRE_LIBRARIES}
)
if(PL_CORE_ZIP)
	# It's highly recommended to enable ZIP support
	add_libs(
		${ZLIB_LIBRARIES}
	)
endif()
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_libs(
		${WIN32_STANDARD_LIBS}
		${WIN32_WSOCK32_LIB}
		${WIN32_USER32_LIB}
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_libs(
		dl
	)

	# There's no native curses library on Android, and compiling one
	# just for this usually never used function would be overkill
	# (used within "PLCore::ConsoleLinux::GetCursorPosition()" & "PLCore::ConsoleLinux::SetCursorPosition()")
	# No need to link "pthread", it's already within Androids c library
	if(ANDROID)
		add_libs(
			log
			android
		)
	else()
		add_libs(
			pthread
			ncurses
		)
	endif()
endif()

##################################################
## Preprocessor definitions
##################################################
add_compile_defs(
	PLCORE_EXPORTS
)
if(NOT PL_CORE_ZIP)
	# Altough not recommended, it's possible to life without ZIP support for minimal builds
	add_compile_defs(
		DISABLE_ZIP_SUPPORT
	)
endif()
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_compile_defs(
		${WIN32_COMPILE_DEFS}
		PCRE_STATIC
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_compile_defs(
		${LINUX_COMPILE_DEFS}
	)
endif()

##################################################
## Compiler flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_compile_flags(
		${WIN32_COMPILE_FLAGS}
	)
elseif(LINUX)
	##################################################
	## GCC Compiler
	##################################################
	add_compile_flags(
		${LINUX_COMPILE_FLAGS}
	)
endif()

##################################################
## Linker flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_linker_flags(
		${WIN32_LINKER_FLAGS}
	)
elseif(LINUX)
	##################################################
	## GCC Compiler
	##################################################
	add_linker_flags(
		${LINUX_LINKER_FLAGS}
	)
endif()

##################################################
## Build
##################################################
build_library(${CMAKETOOLS_CURRENT_TARGET} SHARED)

##################################################
## Dependencies
##################################################
add_dependencies(${CMAKETOOLS_CURRENT_TARGET}		External-libpcre)
if(PL_CORE_ZIP)
	# It's highly recommended to enable ZIP support
	add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	External-zlib)
endif()
add_dependencies(Base								${CMAKETOOLS_CURRENT_TARGET})

##################################################
## Post-Build
##################################################

# Dynamic library
add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_SHARED_LIB} ${PL_RUNTIME_BIN_DIR}
)
if(LINUX)
	# Create relative symbolic links?
	if(CMAKETOOLS_DETAILED_VERSION_INFORMATION)
		add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
			COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKETOOLS_CURRENT_SHARED_LIB} ${CMAKETOOLS_CURRENT_SHARED_LIB_VERSION} WORKING_DIRECTORY ${PL_RUNTIME_BIN_DIR}
			COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKETOOLS_CURRENT_SHARED_LIB} ${CMAKETOOLS_CURRENT_SHARED_LIB_SO_VERSION} WORKING_DIRECTORY ${PL_RUNTIME_BIN_DIR}
		)
	endif()

	# Debug helper for gdb (pretty printer for PLCore::String)
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKETOOLS_CURRENT_TARGET}-gdb.py" "${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_SHARED_LIB_SO_VERSION}-gdb.py"
	)
endif()

# Import library
if(WIN32)
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_STATIC_LIB} ${PL_LIB_DIR}
	)
endif()

##################################################
## Install
##################################################

# Library
install(TARGETS ${CMAKETOOLS_CURRENT_TARGET}
	RUNTIME DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
	LIBRARY DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
	ARCHIVE DESTINATION ${PL_INSTALL_LIB}			COMPONENT SDK
)

# Debug helper for gdb (pretty printer for PLCore::String)
if(LINUX)
	install(FILES									"${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_SHARED_LIB_SO_VERSION}-gdb.py"
			DESTINATION ${PL_INSTALL_RUNTIME_BIN}	COMPONENT Runtime
	)
endif()

# Include files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${CMAKETOOLS_CURRENT_PROJECT}
	DESTINATION ${PL_INSTALL_INCLUDE} 				COMPONENT SDK		REGEX ${PL_EXCLUDE_FILE_EXTENSIONS} EXCLUDE
)

##################################################
## Build static library
##################################################
if(NOT LINUX)
	remove_compile_defs(PLCORE_EXPORTS)
endif()
add_compile_defs(PLCORE_STATIC)
build_library(${CMAKETOOLS_CURRENT_TARGET}Stat STATIC)

##################################################
## Dependencies
##################################################
add_dependencies(${CMAKETOOLS_CURRENT_PROJECT}Stat	External-zlib External-libpcre)
add_dependencies(Base                       		${CMAKETOOLS_CURRENT_TARGET})

##################################################
## Post-Build
##################################################

# Static library
add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_STATIC_LIB} ${PL_LIB_DIR}
)
