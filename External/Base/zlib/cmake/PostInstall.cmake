# Here we check if this build generated a debug libraries. If yes, we will rename them manually so that
# their names match their release counterparts. This is required for the rest of the system to work properly

if(WIN32)
	# zlibstaticd.lib
	if(EXISTS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstaticd.lib)
		# The file is there, meaning the current build is a debug one - let's rename it
		file(RENAME ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstaticd.lib ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibstatic.lib)
	endif()
	
	# We do the same for the dynamic library
	#TODO: We should validate this actually works!
	
	# zlibd.lib
	if(EXISTS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibd.lib)
		# The file is there, meaning the current build is a debug one - let's rename it
		file(RENAME ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlibd.lib ${CMAKETOOLS_CURRENT_INSTALL_DIR}/lib/zlib.lib)
	endif()
	
	# zlibd.dll
	if(EXISTS ${CMAKETOOLS_CURRENT_INSTALL_DIR}/bin/zlibd.dll)
		# The file is there, meaning the current build is a debug one - let's rename it
		file(RENAME ${CMAKETOOLS_CURRENT_INSTALL_DIR}/bin/zlibd.dll ${CMAKETOOLS_CURRENT_INSTALL_DIR}/bin/zlib.dll)
	endif()
endif()