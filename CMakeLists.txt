cmake_minimum_required(VERSION 3.13)
project(TabModMenu)

find_package(Geode REQUIRED)

setup_geode_mod(TabModMenu EXTERNALS geode.loader)

file(GLOB_RECURSE SOURCES src/*.cpp)
target_sources(TabModMenu PRIVATE ${SOURCES})
