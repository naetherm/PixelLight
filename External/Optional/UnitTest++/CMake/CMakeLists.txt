cmake_minimum_required(VERSION 2.8.3)


##################################################
## Project
##################################################
set(CURRENT_TARGET_NAME "UnitTest++")
project(${CURRENT_TARGET_NAME})


##################################################
## Includes
##################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)


##################################################
## Source codes
##################################################
set(CURRENT_SRC
	src/AssertException.cpp
	src/Test.cpp
	src/Checks.cpp
	src/TestRunner.cpp
	src/TestResults.cpp
	src/TestReporter.cpp
	src/TestReporterStdout.cpp
	src/ReportAssert.cpp
	src/TestList.cpp
	src/TimeConstraint.cpp
	src/TestDetails.cpp
	src/MemoryOutStream.cpp
	src/DeferredTestReporter.cpp
	src/DeferredTestResult.cpp
	src/XmlTestReporter.cpp
	src/CurrentTest.cpp
)
if(WIN32)
	set(CURRENT_SRC
		${CURRENT_SRC}
		src/Win32/TimeHelpers.cpp
	)
else()
	set(CURRENT_SRC
		${CURRENT_SRC}
		src/Posix/SignalTranslator.cpp
		src/Posix/TimeHelpers.cpp
	)
endif()


##################################################
## Shared libraries
##################################################
set(CURRENT_SHARED_LIBRARIES
	# Not required in this project
)


##################################################
## Build
##################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffor-scope -fno-rtti -pipe -ffunction-sections -fdata-sections -ffast-math -Wnon-virtual-dtor -Wreorder -Wsign-promo -fvisibility=hidden -fvisibility-inlines-hidden -Wstrict-null-sentinel -Os -funroll-all-loops -fpeel-loops -ftree-vectorize")
set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,--as-needed -Wl,--gc-sections -Wl,--no-undefined -Wl,--strip-all")
if(ANDROID)
	set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-rpath-link=${ANDROID_NDK_SYSROOT}/usr/lib/ -L${ANDROID_NDK_SYSROOT}/usr/lib/")
endif()
add_library(${CURRENT_TARGET_NAME} STATIC ${CURRENT_SRC})
target_link_libraries(${CURRENT_TARGET_NAME} ${CURRENT_SHARED_LIBRARIES})
set_target_properties(${CURRENT_TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS "__STDC_INT64__")
