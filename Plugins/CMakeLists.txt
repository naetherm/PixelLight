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
project(Plugins)

##################################################
## Target
##################################################
add_custom_target(Plugins)

##################################################
## Projects
##################################################
if(PL_PLUGIN_ENGINE_COMPOSITING)
	add_subdirectory(PLCompositing)
endif()
if(PL_PLUGIN_ENGINE_SCRIPTBINDINGS)
	add_subdirectory(PLScriptBindings)
endif()
if(PL_PLUGIN_NETWORK_IRC)
	add_subdirectory(PLIRC)
endif()
if(PL_PLUGIN_NETWORK_JABBER)
	add_subdirectory(PLJabber)
endif()
if(PL_PLGUI AND PL_PLUGIN_GUI_XMLTEXT)
	add_subdirectory(PLGuiXmlText)
endif()
if(PL_PLUGIN_ENGINE_ASSIMP)
	add_subdirectory(PLAssimp)
endif()
if(PL_PLUGIN_ENGINE_IMAGEEXR)
	add_subdirectory(PLImageLoaderEXR)
endif()
if(PL_PLUGIN_ENGINE_PARTICLEGROUPS)
	add_subdirectory(PLParticleGroups)
endif()
if(PL_PLUGIN_ENGINE_SPARK)
	add_subdirectory(SPARK_PL)
endif()
if(PL_PLUGIN_ENGINE_LIBROCKET)
	add_subdirectory(PLlibRocket)
endif()
if(PL_PLUGIN_ENGINE_POSTPROCESS)
	add_subdirectory(PLPostProcessEffects)
endif()
if(PL_PLUGIN_ENGINE_NATURE)
	add_subdirectory(PLNature)
endif()
# Script backends
if(PL_PLUGIN_SCRIPT_NULL)
	add_subdirectory(PLScriptNull)
endif()
if(PL_PLUGIN_SCRIPT_LUA)
	add_subdirectory(PLScriptLua)
endif()
if(PL_PLUGIN_SCRIPT_V8)
	add_subdirectory(PLScriptV8)
endif()
if(PL_PLUGIN_SCRIPT_PYTHON)
	add_subdirectory(PLScriptPython)
endif()
if(PL_PLUGIN_SCRIPT_ANGELSCRIPT)
	add_subdirectory(PLScriptAngelScript)
endif()
# Frontend backends
if(PL_PLUGIN_FRONTEND_NULL)
	add_subdirectory(PLFrontendNull)
endif()
if(PL_PLUGIN_FRONTEND_OS)
	add_subdirectory(PLFrontendOS)
endif()
if(PL_PLGUI AND PL_PLUGIN_FRONTEND_PLGUI)
	add_subdirectory(PLFrontendPLGui)
endif()
if(PL_PLUGIN_FRONTEND_QT)
	add_subdirectory(PLFrontendQt)
endif()
if(PL_PLUGIN_FRONTEND_ACTIVEX)
	add_subdirectory(PLFrontendActiveX)
endif()
if(PL_PLUGIN_FRONTEND_MOZILLA)
	add_subdirectory(PLFrontendMozilla)
endif()
# PLDatabase backends
if(PL_PLUGIN_DATABASE_NULL)
	add_subdirectory(PLDatabaseNull)
endif()
if(PL_PLUGIN_DATABASE_MYSQL)
	add_subdirectory(PLDatabaseMySQL)
endif()
if(PL_PLUGIN_DATABASE_POSTGRESQL)
	add_subdirectory(PLDatabasePostgreSQL)
endif()
if(PL_PLUGIN_DATABASE_SQLITE)
	add_subdirectory(PLDatabaseSQLite)
endif()
# PLRenderer backends
if(PL_PLUGIN_RENDERER_NULL)
	add_subdirectory(PLRendererNull)
endif()
if(PL_PLUGIN_RENDERER_OPENGL)
	add_subdirectory(PLRendererOpenGL)
	if(PL_PLUGIN_RENDERER_OPENGLCG)
		add_subdirectory(PLRendererOpenGLCg)
	endif()
endif()
if(PL_PLUGIN_RENDERER_OPENGLES2)
	add_subdirectory(PLRendererOpenGLES2)
endif()
if(PL_PLUGIN_RENDERER_D3D9)
	add_subdirectory(PLRendererD3D9)
endif()
if(PL_PLUGIN_RENDERER_D3D11)
	add_subdirectory(PLRendererD3D11)
endif()
# PLPhysics backends
if(PL_PLUGIN_PHYSICS_NULL)
	add_subdirectory(PLPhysicsNull)
endif()
if(PL_PLUGIN_PHYSICS_NEWTON)
	add_subdirectory(PLPhysicsNewton)
endif()
if(PL_PLUGIN_PHYSICS_ODE)
	add_subdirectory(PLPhysicsODE)
endif()
if(PL_PLUGIN_PHYSICS_PHYSX)
	add_subdirectory(PLPhysicsPhysX)
endif()
if(PL_PLUGIN_PHYSICS_BULLET)
	add_subdirectory(PLPhysicsBullet)
endif()
# PLSound backends
if(PL_PLUGIN_SOUND_NULL)
	add_subdirectory(PLSoundNull)
endif()
if(PL_PLUGIN_SOUND_OPENAL)
	add_subdirectory(PLSoundOpenAL)
endif()
if(PL_PLUGIN_SOUND_OPENSLES)
	add_subdirectory(PLSoundOpenSLES)
endif()
if(PL_PLUGIN_SOUND_FMOD)
	add_subdirectory(PLSoundFMOD)
endif()
if(PL_PLUGIN_SOUND_FMODEX)
	add_subdirectory(PLSoundFMODEx)
endif()
# PLVolume
if(PL_PLUGIN_VOLUME)
	add_subdirectory(PLVolume)
	if(PL_PLUGIN_VOLUME_RENDERER)
		add_subdirectory(PLVolumeRenderer)
	endif()
	if(PL_PLUGIN_VOLUME_GUI AND PL_PLUGIN_FRONTEND_QT)
		add_subdirectory(PLVolumeGui)
	endif()
	if(PL_PLUGIN_VOLUME_LOADER_PVM)
		add_subdirectory(PLVolumeLoaderPVM)
	endif()
	if(PL_PLUGIN_VOLUME_LOADER_DICOM_GDCM)
		add_subdirectory(PLVolumeLoaderDICOM_GDCM)
	endif()
endif()

##################################################
## Dependencies
##################################################
add_dependencies(PixelLight Plugins)
