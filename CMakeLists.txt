# CMakeLists.txt for cuddly-tribble
cmake_minimum_required(VERSION 3.10)
project(cuddly_tribble)

set(CMAKE_CXX_STANDARD 17)

# Ensure debug symbols are always included
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(cuddly_tribble ${SOURCE_FILES})

# Use CMake's OpenGL module
find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)

# Link to libraries by name only
# Nix will provide the correct environment

target_link_libraries(cuddly_tribble PRIVATE glfw GL)

add_custom_command(
    TARGET cuddly_tribble POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/preferences
            ${CMAKE_BINARY_DIR}/preferences
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/config.ini
            ${CMAKE_BINARY_DIR}/config.ini
)