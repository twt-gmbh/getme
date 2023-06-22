# Upgrade to version >= 3.24 if possible to make all features available
cmake_minimum_required(VERSION 3.22)

project(
  GETMe
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
  #DOWNLOAD_EXTRACT_TIMESTAMP NEW # uncomment in the case of timestamp warnings
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(CTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

add_subdirectory(Examples)
add_subdirectory(Mathematics)
add_subdirectory(Mesh)
add_subdirectory(Smoothing)
add_subdirectory(Testdata)
add_subdirectory(Utility)
