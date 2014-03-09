## The CMake script used to build libvorbis by our externals build pipeline

cmake_minimum_required(VERSION 2.8)

project(vorbis C)

if(NOT MSVC AND NOT CMAKE_BUILD_TYPE)
    #SET(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_BUILD_TYPE "Release")
    message("No CMAKE_BUILD_TYPE specified, defaulting to ${CMAKE_BUILD_TYPE}") 
endif()

set(libvorbis_SRCS
    lib/analysis.c
    lib/bitrate.c
    lib/block.c
    lib/codebook.c
    lib/envelope.c
    lib/floor0.c
    lib/floor1.c
    lib/info.c
    lib/lookup.c
    lib/lpc.c
    lib/lsp.c
    lib/mapping0.c
    lib/mdct.c
    lib/psy.c
    lib/registry.c
    lib/res0.c
    lib/sharedbook.c
    lib/smallft.c
    lib/synthesis.c
    lib/vorbisenc.c
    lib/window.c
)

set(libvorbis_HDRS 
    lib/backends.h
    lib/bitrate.h
    lib/codebook.h
    lib/codec_internal.h
    lib/envelope.h
    lib/modes/floor_all.h
    lib/books/floor/floor_books.h
    lib/highlevel.h
    lib/lookup.h
    lib/lookup_data.h
    lib/lpc.h
    lib/lsp.h
    lib/masking.h
    lib/mdct.h
    lib/misc.h
    lib/os.h
    lib/psy.h
    lib/modes/psych_11.h
    lib/modes/psych_16.h
    lib/modes/psych_44.h
    lib/modes/psych_8.h
    lib/registry.h
    lib/books/coupled/res_books_stereo.h
    lib/books/uncoupled/res_books_uncoupled.h
    lib/modes/residue_16.h
    lib/modes/residue_44.h
    lib/modes/residue_44u.h
    lib/modes/residue_8.h
    lib/scales.h
    lib/modes/setup_11.h
    lib/modes/setup_16.h
    lib/modes/setup_22.h
    lib/modes/setup_32.h
    lib/modes/setup_44.h
    lib/modes/setup_44u.h
    lib/modes/setup_8.h
    lib/modes/setup_X.h
    lib/smallft.h
    lib/window.h
)

set(vorbis_public_HDRS
    include/vorbis/codec.h
    include/vorbis/vorbisenc.h
    include/vorbis/vorbisfile.h)

include_directories(${OGG_INCLUDE_DIR} include lib)

if(MSVC)
    add_definitions(/D_UNICODE /DUNICODE)
    list(APPEND libvorbis_SRCS win32/vorbis.def)
endif()

add_library(vorbis SHARED ${libvorbis_SRCS} ${libvorbis_HDRS} ${vorbis_public_HDRS})
add_library(vorbisfile SHARED lib/vorbisfile.c include/vorbis/vorbisfile.h)

#target_link_libraries(libvorbisfile_dynamic libvorbis_dynamic ${OGG_LIBRARY})

install(
	TARGETS vorbis vorbisfile
    	RUNTIME DESTINATION bin
    	ARCHIVE DESTINATION lib
    	LIBRARY DESTINATION lib
)

install(FILES ${vorbis_public_HDRS} DESTINATION include/vorbis)
