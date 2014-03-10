## This is the CMake script used to build Newton by our externals build pipeline


cmake_minimum_required(VERSION 2.8.3)


##################################################
## Project
##################################################
set(CURRENT_TARGET_NAME "Newton")
project(${CURRENT_TARGET_NAME})


##################################################
## Includes
##################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/physics)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/newton)


##################################################
## Source codes
##################################################
set(CURRENT_SRC
	# dg low level library
	core/dg.cpp
	core/dgRef.cpp
	core/dgCRC.cpp
	core/dgDebug.cpp
	core/dgTree.cpp
	core/dgNode.cpp
	core/dgTypes.cpp
	core/dgMatrix.cpp
	core/dgGoogol.cpp
	core/dgMemory.cpp
	core/dgSphere.cpp
	core/dgRandom.cpp
	core/dgThreads.cpp
	core/dgPolyhedra.cpp
	core/dgQuaternion.cpp
	core/dgConvexHull3d.cpp
	core/dgConvexHull4d.cpp
	core/dgIntersections.cpp
	core/dgAABBPolygonSoup.cpp
	core/dgSmallDeterminant.cpp
	core/dgPolygonSoupBuilder.cpp
	core/dgPolyhedraMassProperties.cpp
	core/dgDelaunayTetrahedralization.cpp
	# Physics engine files
	physics/dgWorld.cpp
	physics/dgBody.cpp
	physics/dgMeshEffect.cpp
	physics/dgMeshEffect2.cpp
	physics/dgCollisionConvex.cpp
	physics/dgBallConstraint.cpp
	physics/dgBilateralConstraint.cpp
	physics/dgCorkscrewConstraint.cpp
	physics/dgHingeConstraint.cpp
	physics/dgNarrowPhaseCollision.cpp
	physics/dgBroadPhaseCollision.cpp
	physics/dgMinkowskiConv.cpp
	physics/dgCollisionBox.cpp
	physics/dgCollisionCapsule.cpp
	physics/dgCollisionBVH.cpp
	physics/dgCollisionMesh.cpp
	physics/dgCollisionUserMesh.cpp
	physics/dgCollisionChamferCylinder.cpp
	physics/dgPointToCurveConstraint.cpp
	physics/dgCollisionCone.cpp
	physics/dgCollisionConvexHull.cpp
	physics/dgSlidingConstraint.cpp
	physics/dgCollisionConvexModifier.cpp
	physics/dgUniversalConstraint.cpp
	physics/dgCollision.cpp
	physics/dgUpVectorConstraint.cpp
	physics/dgCollisionCylinder.cpp
	physics/dgUserConstraint.cpp
	physics/dgCollisionEllipse.cpp
	physics/dgCollisionNull.cpp
	physics/dgCollisionScene.cpp
	physics/dgCollisionCompoundBreakable.cpp
	physics/dgCollisionHeightField.cpp
	physics/dgWorldDynamicUpdate.cpp
	physics/dgCollisionSphere.cpp
	physics/dgCollisionCompound.cpp
	physics/dgConnectorConstraint.cpp
	physics/dgConstraint.cpp
	physics/dgBodyMasterList.cpp
	physics/dgContact.cpp
	# Newton engine files
	newton/Newton.cpp
	newton/NewtonClass.cpp
)


##################################################
## Shared libraries
##################################################
set(CURRENT_SHARED_LIBRARIES
	# Not required in this project
)

##################################################
## Library
##################################################
add_library(${CURRENT_TARGET_NAME} SHARED ${CURRENT_SRC})
target_link_libraries(${CURRENT_TARGET_NAME} ${CURRENT_SHARED_LIBRARIES})

##################################################
## Build
##################################################
if(MSVC)
	if(CMAKE_GENERATOR MATCHES "Visual Studio .. Win64")
		set_target_properties(${CURRENT_TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS "__STDC_INT64__;_SCALAR_ARITHMETIC_ONLY;_WIN_64_VER;_NEWTON_BUILD_DLL")
	else()
		set_target_properties(${CURRENT_TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS "__STDC_INT64__;_SCALAR_ARITHMETIC_ONLY;_WIN_32_VER;_NEWTON_BUILD_DLL")
	endif()
	
	#set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1" CACHE INTERNAL "")
	#set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG" CACHE INTERNAL "")
	#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG" CACHE INTERNAL "")
	#set(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG" CACHE INTERNAL "")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -ffor-scope -fno-rtti -fno-exceptions -pipe -ffunction-sections -fdata-sections -ffast-math -Wnon-virtual-dtor -Wreorder -Wsign-promo -Wstrict-null-sentinel -Os -funroll-all-loops -fpeel-loops -ftree-vectorize -fpermissive")
	set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,--as-needed -Wl,--gc-sections -Wl,--no-undefined -Wl,--strip-all")
	
	if(ANDROID)
		set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-rpath-link=${ANDROID_NDK_SYSROOT}/usr/lib/ -L${ANDROID_NDK_SYSROOT}/usr/lib/")
	endif()
	
	set_target_properties(${CURRENT_TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS "__STDC_INT64__;_LINUX_VER;_LINUX_VER_64;_SCALAR_ARITHMETIC_ONLY")
endif()

##################################################
## Install
##################################################
install(TARGETS ${CURRENT_TARGET_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/newton/*.h")
install(FILES ${files} DESTINATION include/Newton)
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/physics/*.h")
install(FILES ${files} DESTINATION include/Pysics)
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/core/*.h")
install(FILES ${files} DESTINATION include/Core)
