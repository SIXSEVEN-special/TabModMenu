cmake_minimum_required(VERSION 3.13)

# This policy allows newer CMake versions to support older Geode structures
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(TabModMenu)

find_package(Geode REQUIRED)

# Sets up your mod and explicitly bypasses the online index error
setup_geode_mod(TabModMenu EXTERNALS geode.loader:3.8.0)

# Links all of your actual C++ source files inside the src folder
target_sources(TabModMenu PRIVATE src/main.cpp)
