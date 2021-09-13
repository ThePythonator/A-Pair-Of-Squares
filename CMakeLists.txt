# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.9)

project(APairOfSquares)

# Add your sources here (adding headers is optional, but helps some CMake generators)
set(PROJECT_SOURCE "APairOfSquares.cpp" "Square.cpp" "Entity.cpp" "Spritesheet.cpp" "InputManager.cpp" "Assets.cpp" "LevelManager.cpp" "Player.cpp" "ParticleManager.cpp" "TimerManager.cpp" "Bezier.cpp" "TextManager.cpp" "Game.cpp" "Utilities.cpp" "AudioManager.cpp" "Orb.cpp" "Spike.cpp" "Colour.cpp" "Constants.cpp" "Spring.cpp" "TransitionManager.cpp" "Button.cpp")

# ... and any other files you want in the release here
set(PROJECT_DISTRIBS LICENSE README.md)

set(ASSET_OUTPUTS "Assets.hpp" "Assets.cpp")
set(ASSET_DEPENDS "assets.yml")

# Build configuration; approach this with caution!
if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion")
endif()

find_package(PythonInterp 3.6 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


#############

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

# Build assets.hpp/cpp files
add_custom_command(
    OUTPUT ${ASSET_OUTPUTS}
    COMMAND ${PYTHON_EXECUTABLE} -m ttblit pack --config ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_DEPENDS} --output ${CMAKE_CURRENT_BINARY_DIR} --force
    DEPENDS ${ASSET_DEPENDS}
)


add_executable(APairOfSquares ${PROJECT_SOURCE})

target_link_libraries(APairOfSquares ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# setup release packages
#install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
#set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
#set (CPACK_GENERATOR "ZIP" "TGZ")
#include (CPack)