cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME APairOfSquares)

project(${PROJECT_NAME})

# Add your sources here (adding headers is optional, but helps some CMake generators)
set(PROJECT_SOURCE "APairOfSquares.cpp" "Square.cpp" "Entity.cpp" "Spritesheet.cpp" "InputManager.cpp" "Assets.cpp" "LevelManager.cpp" "Player.cpp" "ParticleManager.cpp" "TimerManager.cpp" "Bezier.cpp" "TextManager.cpp" "Game.cpp" "Utilities.cpp" "AudioManager.cpp" "Orb.cpp" "Spike.cpp" "Colour.cpp" "Constants.cpp" "Spring.cpp" "TransitionManager.cpp" "Button.cpp" "Door.cpp")

# ... and any other files you want in the release here
set(PROJECT_DISTRIBS LICENSE README.md)
set(PROJECT_ASSETS assets)

set(ASSET_OUTPUTS "Assets.hpp" "Assets.cpp")
set(ASSET_DEPENDS "Assets.yml")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

find_package(PythonInterp 3.6 REQUIRED)

# Build Assets.hpp/cpp files
add_custom_command(
    OUTPUT ${ASSET_OUTPUTS}
    COMMAND ${PYTHON_EXECUTABLE} -m ttblit pack --config ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_DEPENDS} --output ${CMAKE_CURRENT_BINARY_DIR} --force
    DEPENDS ${ASSET_DEPENDS}
)

include(get_sdl2.cmake)

# Link
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2 SDL2::image)

# Setup release packages
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	BUNDLE DESTINATION bin
)

install(FILES ${PROJECT_DISTRIBS}
	DESTINATION .
)

install(DIRECTORY ${PROJECT_ASSETS}
    DESTINATION .
    FILES_MATCHING
    PATTERN "*.png"
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)