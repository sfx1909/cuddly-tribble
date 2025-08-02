# CMakeLists.txt for cuddly-tribble
cmake_minimum_required(VERSION 3.10)
project(cuddly_tribble)

set(CMAKE_CXX_STANDARD 17)

add_executable(cuddly_tribble src/main.cpp src/config.cpp)

add_custom_command(
    TARGET cuddly_tribble POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_BINARY_DIR}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/config.ini
            ${CMAKE_BINARY_DIR}/config.ini
)