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
## Configuration
##################################################
if(WIN32)
	# For Windows, there is no binary distribution save the official installer. We don't go through
	# the hassle of re-packing the binaries ourselves (who knows what the licensing implication are)
	# and just softly fail if the package is not installed. Our official SDK installer should offer
	# the Cg Toolkit for installation anyway.
	
	# Set this value first, it will be used by the script (and that one does not do it properly it seems)
	if(CMAKETOOLS_TARGET_BITSIZE MATCHES "64")
		set(NV_SYSTEM_PROCESSOR "x86_64" CACHE INTERNAL "")
	else()
		set(NV_SYSTEM_PROCESSOR "x86" CACHE INTERNAL "")
	endif()
	
	find_package(Cg)
	
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
	#[TODO] For Linux, we can download and unpack the binary distribution from NVIDIA's site
endif()
