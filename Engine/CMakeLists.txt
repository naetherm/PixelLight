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


# Set the package name
pl_set_current_package(Engine)

# We require an exact order of packages for successful build
pl_add_subdirectory(PLCore)
pl_add_subdirectory(PLMath)
pl_add_subdirectory(PLDatabase)
pl_add_subdirectory(PLInput)
pl_add_subdirectory(PLGraphics)
pl_add_subdirectory(PLRenderer)
pl_add_subdirectory(PLMesh)
pl_add_subdirectory(PLScene)
pl_add_subdirectory(PLPhysics)
pl_add_subdirectory(PLSound)
pl_add_subdirectory(PLEngine)
#if(PL_PLGUI)
	pl_add_subdirectory(PLGui)
#endif()