cmake_minimum_required(VERSION 3.21)

project(SparkFlowFX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/App)
add_subdirectory(src/Core)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT App)