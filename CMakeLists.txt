cmake_minimum_required(VERSION 3.11)
project(PoolGame)

set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions("DEBUG")

include(FetchContent)

# Fetch Raylib
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 3.5.0 # Adjust this to the version you want
)

FetchContent_GetProperties(raylib)
if(NOT raylib_POPULATED)
  FetchContent_Populate(raylib)
  add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
endif()

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Raylib
target_link_libraries(${PROJECT_NAME} raylib)