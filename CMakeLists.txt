# Basic parameters; check that these match your project / environment
cmake_minimum_required(VERSION 3.9)

project(APairOfSquares)

# Add your sources here (adding headers is optional, but helps some CMake generators)
set(PROJECT_SOURCE APairOfSquares.cpp)

# ... and any other files you want in the release here
set(PROJECT_DISTRIBS LICENSE README.md)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(APairOfSquares ${PROJECT_SOURCE})
target_link_libraries(APairOfSquares ${SDL2_LIBRARIES})

# setup release packages
#install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
#set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
#set (CPACK_GENERATOR "ZIP" "TGZ")
#include (CPack)