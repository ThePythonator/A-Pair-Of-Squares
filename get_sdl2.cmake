# defines SDL2::SDL2, SDL2::SDL2main and SDL2::image targets

if(EMSCRIPTEN)
    # Emscripten-specific magic
    add_library(SDL2::SDL2 INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        INTERFACE_COMPILE_OPTIONS "SHELL:-s USE_SDL=2"
        INTERFACE_LINK_LIBRARIES "-s USE_SDL=2"
    )

    add_library(SDL2::SDL2main INTERFACE IMPORTED)

    add_library(SDL2::image INTERFACE IMPORTED)
    set_target_properties(SDL2::image PROPERTIES
        INTERFACE_COMPILE_OPTIONS "SHELL:-s USE_SDL_IMAGE=2"
        INTERFACE_LINK_LIBRARIES "-s USE_SDL_IMAGE=2"
    )

    add_library(SDL2::mixer INTERFACE IMPORTED)
    set_target_properties(SDL2::mixer PROPERTIES
        INTERFACE_COMPILE_OPTIONS "SHELL:-s USE_SDL_MIXER=2"
        INTERFACE_LINK_LIBRARIES "-s USE_SDL_MIXER=2"
    )
else()
    # Check for system SDL2
    find_package(SDL2 QUIET NO_SYSTEM_ENVIRONMENT_PATH)
    find_package(SDL2_image QUIET) # will probably fail
    find_package(SDL2_mixer QUIET)
endif()

# Didn't find it, build from source
include(FetchContent)

if(NOT TARGET SDL2::SDL2)
    message("Did not find SDL2, building from source.")

    FetchContent_Populate(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        release-2.0.16
    )
    add_subdirectory(${sdl2_SOURCE_DIR} SDL2 EXCLUDE_FROM_ALL)
endif()

if(NOT TARGET SDL2::image)
    # Get SDL2_image (no released version has CMake support)
    FetchContent_Populate(SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG        main
    )
    add_subdirectory(${sdl2_image_SOURCE_DIR} SDL2_image EXCLUDE_FROM_ALL)
    set_property(TARGET jpeg PROPERTY POSITION_INDEPENDENT_CODE ON) # build fix
endif()


#[[
if(NOT TARGET SDL2::mixer)
    # Get SDL2_mixer
    FetchContent_Populate(SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
        GIT_TAG        master
    )
    FetchContent_Populate(SDL2_mixer
        GIT_REPOSITORY https://github.com/Daft-Freak/SDL_mixer
        GIT_TAG        patch-1
    )

    add_subdirectory(${SDL2_mixer_SOURCE_DIR} SDL2_mixer EXCLUDE_FROM_ALL)
endif()
]]

if(NOT TARGET SDL2::mixer)
    # Not sure what I'm doing here
    set(SUPPORT_MP3_MPG123 ON)
    set(SUPPORT_OGG ON)

    FetchContent_Populate(SDL2_mixer
        GIT_REPOSITORY https://github.com/Daft-Freak/SDL_mixer
        GIT_TAG        patch-1
    )
    add_definitions(-DMUSIC_MP3 -DMUSIC_OGG) # VS compile errors if no music formats //-DMUSIC_WAV
    add_subdirectory(${sdl2_mixer_SOURCE_DIR} SDL2_mixer EXCLUDE_FROM_ALL)
endif()
