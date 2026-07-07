cmake_minimum_required(VERSION 3.13)
project(TabModMenu)

find_package(Geode REQUIRED)

# This explicitly tells the builder to bypass the index for the loader
setup_geode_mod(TabModMenu EXTERNALS geode.loader:3.8.0)
