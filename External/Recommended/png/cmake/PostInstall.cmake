# Here we check if this build generated a debug libraries. If yes, we will rename them manually so that
# their names match their release counterparts. This is required for the rest of the system to work properly

if(WIN32)
	# libpng16_staticd.lib
	if(EXISTS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libpng16_staticd.lib)
		# The file is there, meaning the current build is a debug one - let's rename it
		file(RENAME ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libpng16_staticd.lib ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/libpng16_static.lib)
	endif()
endif()