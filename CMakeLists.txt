cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Uncomment these to put the dlls in the same directory as the executable (useful if you don't have SDL2 installed and you're building from source)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Uncomment if you need to do some debugging :)
# set(CMAKE_FIND_DEBUG_MODE TRUE)

# Comment out if you want the console window (on Windows)
set(NO_CONSOLE)

set(PROJECT_NAME APairOfSquares)

project(${PROJECT_NAME})

# Add your sources here (adding headers is optional, but helps some CMake generators)
set(PROJECT_SOURCES "APairOfSquares.cpp" "Square.cpp" "Entity.cpp" "Spritesheet.cpp" "InputManager.cpp" "Assets.cpp" "LevelManager.cpp" "Player.cpp" "ParticleManager.cpp" "TimerManager.cpp" "Bezier.cpp" "TextManager.cpp" "Game.cpp" "Utilities.cpp" "AudioManager.cpp" "Orb.cpp" "Spike.cpp" "Colour.cpp" "Constants.cpp" "Spring.cpp" "TransitionManager.cpp" "Button.cpp" "Door.cpp")

# Or let GLOB find all the .cpp files in the current directory... (CMake won't detect when files are added)
#file(GLOB PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "*.cpp")
#list(APPEND PROJECT_SOURCES "Assets.cpp")

# ... and any other files you want in the release here
set(PROJECT_DISTRIBS LICENSE README.md)
set(PROJECT_ASSETS assets)

set(ASSET_OUTPUTS "Assets.hpp" "Assets.cpp")
set(ASSET_DEPENDS "Assets.yml")

if(APPLE)
    # Need to put the asssets inside the bundle (this is where SDL_GetBasePath points to)
    set(ASSETS_DEST bin/$<TARGET_FILE_NAME:${PROJECT_NAME}>.app/Contents/Resources)
else()
    set(ASSETS_DEST .)
endif()

if (NO_CONSOLE)
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE WIN32 ${PROJECT_SOURCES})
else()
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_SOURCES})
endif()

find_package(PythonInterp 3.6 REQUIRED)

# Build Assets.hpp/cpp files
add_custom_command(
    OUTPUT ${ASSET_OUTPUTS}
    COMMAND ${PYTHON_EXECUTABLE} -m ttblit pack --config ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_DEPENDS} --output ${CMAKE_CURRENT_BINARY_DIR} --force
    DEPENDS ${ASSET_DEPENDS}
)

include(get_sdl2.cmake)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Link
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2 SDL2::image SDL2::mixer)


# Setup release packages
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	BUNDLE DESTINATION bin
)

install(FILES ${PROJECT_DISTRIBS}
	DESTINATION .
)

install(DIRECTORY ${PROJECT_ASSETS}
    DESTINATION ${ASSETS_DEST}
    FILES_MATCHING
    PATTERN "*.png"
    PATTERN "*.mp3"
    PATTERN "*.ogg"
)

if (UNIX AND NOT APPLE)
    install(FILES APoS_Launcher_Linux.sh
	    DESTINATION .
    )
endif()

if (NOT EMSCRIPTEN)
    # Install dependencies
    set(DEP_SEARCH_DIRS)
    # We built these, so we're installing them
    if(TARGET SDL2)
        list(APPEND DEP_SEARCH_DIRS $<TARGET_FILE_DIR:SDL2>)
    endif()

    if(TARGET SDL2_image)
        list(APPEND DEP_SEARCH_DIRS $<TARGET_FILE_DIR:SDL2_image> $<TARGET_FILE_DIR:png> $<TARGET_FILE_DIR:zlib>)
    endif()

    if(TARGET SDL2_mixer)
        list(APPEND DEP_SEARCH_DIRS $<TARGET_FILE_DIR:SDL2_mixer>) # missing ogg and mp3 support
    endif()

    set(EXE_SUFFIX)
    if(APPLE)
        set(EXE_SUFFIX ".app")
    endif()

    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/bin/$<TARGET_FILE_NAME:${PROJECT_NAME}>${EXE_SUFFIX}\" \"\" \"${DEP_SEARCH_DIRS}\")
    ")

endif()

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)