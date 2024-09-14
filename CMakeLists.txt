cmake_minimum_required(VERSION 3.11)
project(PoolGame)

set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions("DEBUG" "BUILD_SHARED_LIBS=ON")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT SHARED)
  message(FATAL_ERROR "Raylib must be built as a dynamic library, pass -DSHARED=ON option to cmake")
endif()

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
file(GLOB ENTRYPOINT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/entrypoint/*.c)

# Add executable and game logic library
add_library(${PROJECT_NAME}_Game SHARED ${SOURCES})
add_executable(${PROJECT_NAME} ${ENTRYPOINT_SOURCES})

# Link Raylib
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/raylib-src/src)
target_link_libraries(${PROJECT_NAME}_Game PRIVATE raylib)