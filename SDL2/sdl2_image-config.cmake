# Note: seems to not use SDL2_DIR, causing lots of issues
# SDL_DIR is never set

find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
	HINTS ${SDL2_IMAGE_DIR}
	PATH_SUFFIXES include/SDL2 include
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(VC_LIB_PATH_SUFFIX lib/x64)
else()
	set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

find_library(SDL2_IMAGE_LIBRARY
	NAMES SDL2_image
	HINTS ${SDL2_IMAGE_DIR}
	PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
)

#[[
# Find dlls
find_file(SDL2_IMAGE_DLL SDL2_image.dll
	HINTS ${SDL2_IMAGE_DIR}
	PATH_SUFFIXES bin ${VC_LIB_PATH_SUFFIX}
)

find_file(LIBJPEG_DLL libjpeg-9.dll
	HINTS ${SDL2_IMAGE_DIR}
	PATH_SUFFIXES bin ${VC_LIB_PATH_SUFFIX}
)

find_file(ZLIB_DLL zlib1.dll
	HINTS ${SDL2_IMAGE_DIR}
	PATH_SUFFIXES bin ${VC_LIB_PATH_SUFFIX}
)

find_file(LIBPNG_DLL libpng16-16.dll
	HINTS ${SDL2_IMAGE_DIR}
	PATH_SUFFIXES bin ${VC_LIB_PATH_SUFFIX}
)
#]]