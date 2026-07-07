cmake_minimum_required(VERSION 3.21)
project(TabModMenu VERSION 1.0.0)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to it, or clone https://github.com/geode-sdk/geode next to this project.")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} $ENV{GEODE_SDK}/build)

setup_geode_mod(${PROJECT_NAME})
