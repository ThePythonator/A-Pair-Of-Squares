# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.9)

set(PROJECT_NAME APairOfSquares)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

if (EXISTS SDL2_DIR)
	message("Using existing SDL2_DIR: ${SDL2_DIR}")
else()
	set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2)
	message("Using existing SDL2_DIR: ${SDL2_DIR}")
endif()

if (EXISTS SDL2_IMAGE_DIR)
	message("Using existing SDL2_IMAGE_DIR: ${SDL2_IMAGE_DIR}")
else()
	set(SDL2_IMAGE_DIR ${SDL2_DIR})
	message("Using SDL2_IMAGE_DIR: ${SDL2_IMAGE_DIR}")
endif()

# Add your sources here (adding headers is optional, but helps some CMake generators)
set(PROJECT_SOURCE "APairOfSquares.cpp" "Square.cpp" "Entity.cpp" "Spritesheet.cpp" "InputManager.cpp" "Assets.cpp" "LevelManager.cpp" "Player.cpp" "ParticleManager.cpp" "TimerManager.cpp" "Bezier.cpp" "TextManager.cpp" "Game.cpp" "Utilities.cpp" "AudioManager.cpp" "Orb.cpp" "Spike.cpp" "Colour.cpp" "Constants.cpp" "Spring.cpp" "TransitionManager.cpp" "Button.cpp" "Door.cpp")

# ... and any other files you want in the release here
set(PROJECT_DISTRIBS LICENSE README.md)
set(PROJECT_ASSETS assets)

set(ASSET_OUTPUTS "Assets.hpp" "Assets.cpp")
set(ASSET_DEPENDS "Assets.yml")

# Build configuration; approach this with caution!
if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion")
endif()


add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

find_package(PythonInterp 3.6 REQUIRED)

if(EMSCRIPTEN)
	target_compile_options(${PROJECT_NAME} PRIVATE "-s;USE_SDL=2;-s ;USE_SDL_IMAGE=2")

	set(SDL2_LIBRARIES "-s USE_SDL=2")
	set(SDL2_IMAGE_LIBRARY "-s USE_SDL_IMAGE=2")

	target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

	set_target_properties(${PROJECT_NAME} PROPERTIES
		SUFFIX ".html"
		LINK_FLAGS "-s ENVIRONMENT=web -s SDL2_IMAGE_FORMATS=['png']"
	)

	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.js ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.wasm
		DESTINATION bin
	)
else()
	find_package(SDL2 REQUIRED)
	find_package(SDL2_IMAGE REQUIRED)

	target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Build assets.hpp/cpp files
add_custom_command(
    OUTPUT ${ASSET_OUTPUTS}
    COMMAND ${PYTHON_EXECUTABLE} -m ttblit pack --config ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET_DEPENDS} --output ${CMAKE_CURRENT_BINARY_DIR} --force
    DEPENDS ${ASSET_DEPENDS}
)



target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY})

# Windows DLLs
#if(SDL2_DLL)
#    configure_file(${SDL2_DLL} ${CMAKE_CURRENT_BINARY_DIR}/SDL2.dll COPYONLY)
#endif()

#if(SDL2_IMAGE_DLL)
#    configure_file(${SDL2_IMAGE_DLL} ${CMAKE_CURRENT_BINARY_DIR}/SDL2_image.dll COPYONLY)
#endif()

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

if(APPLE)
	install(FILES "sdl_setup_mac.sh"
		DESTINATION .
	)
endif()

# Copy SDL2 dlls for Windows users
if(WIN32)
	if(SDL2_DLL)
		install(FILES ${SDL2_DLL}
			DESTINATION bin
		)
		#set(SDL2_DLL ${SDL2_DLL} PARENT_SCOPE)
	endif()

	if(SDL2_IMAGE_DLL)
		install(FILES ${SDL2_IMAGE_DLL}
			DESTINATION bin
		)
	endif()

	if(LIBJPEG_DLL)
		install(FILES ${LIBJPEG_DLL}
			DESTINATION bin
		)
	endif()

	if(ZLIB_DLL)
		install(FILES ${ZLIB_DLL}
			DESTINATION bin
		)
	endif()

	if(LIBPNG_DLL)
		install(FILES ${LIBPNG_DLL}
			DESTINATION bin
		)
	endif()
endif()

# install(DIRECTORY ${PROJECT_ASSETS} DESTINATION .)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)