# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.1...3.28)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  dslab
  VERSION 1.0
  LANGUAGES CXX)

# If you set any CMAKE_ variables, that can go here.
# (But usually don't do this, except maybe for C++ standard)

SET(wxUSE_OPENGL 1)


# Find packages go here.

include(c:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(OpenGL REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

#
# example-zoompan.cpp
add_library(dslab STATIC dslab.cpp dslab.h xpm_mdsg.cpp xpm_viewmag1.cpp xpm_viewmagm.cpp xpm_viewmagp.cpp)
target_link_libraries(dslab PRIVATE wx::core wx::base wx::gl)
target_compile_features(dslab PRIVATE cxx_std_17)

# Link each target with other targets or add options, etc.

# Adding something we can run - Output name matches target name
add_executable(example-zoompan WIN32  example-zoompan.cpp)
target_link_libraries(example-zoompan PRIVATE dslab wx::core wx::base)
target_include_directories(example-zoompan PRIVATE  ${PROJECT_SOURCE_DIR})
