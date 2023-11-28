cmake_minimum_required(VERSION 3.27)
project(CasEditor)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(C_STANDARD_REQUIRED ON)
set(CXX_STANDARD_REQUIRED ON)

add_subdirectory(../SDL EXCLUDE_FROM_ALL SDL3)

find_package(PkgConfig REQUIRED)

pkg_check_modules(cairo IMPORTED_TARGET cairo)
pkg_check_modules(pangocairo IMPORTED_TARGET pangocairo)
pkg_check_modules(gtest IMPORTED_TARGET gtest)
pkg_check_modules(gtest_main IMPORTED_TARGET gtest_main)
pkg_check_modules(fmt IMPORTED_TARGET fmt)

add_executable(cuno cuno.cpp cuno.h)
target_link_libraries(cuno PRIVATE SDL3::SDL3 PkgConfig::cairo PkgConfig::pangocairo PkgConfig::fmt)
