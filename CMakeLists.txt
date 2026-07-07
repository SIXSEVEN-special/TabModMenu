cmake_minimum_required(VERSION 3.13)

# Force compatibility fallback for older Geode/TulipHook sub-packages
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(TabModMenu)

# This macro handles Geode's automatic environment paths behind the scenes
include(FetchContent)

find_package(Geode REQUIRED)

# Explicitly bypasses the online index lookups to prevent the geode.loader crash
setup_geode_mod(TabModMenu EXTERNALS geode.loader:3.8.0)

# Automatically finds and links ALL .cpp source files inside your src/ folder
file(GLOB_RECURSE SOURCES src/*.cpp)
target_sources(TabModMenu PRIVATE \)
