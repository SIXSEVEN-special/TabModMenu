cmake_minimum_required(VERSION 3.13)

project(TabModMenu)

find_package(Geode REQUIRED)

# This is the exact community standard way to bypass the index check for the loader
setup_geode_mod(TabModMenu EXTERNALS geode.loader:3.8.0)

file(GLOB_RECURSE SOURCES src/*.cpp)
target_sources(TabModMenu PRIVATE ${SOURCES})
