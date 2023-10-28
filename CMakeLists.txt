cmake_minimum_required(VERSION 3.27)
project(CasEditor)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(C_STANDARD_REQUIRED ON)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(PkgConfig REQUIRED)

pkg_check_modules(cairo IMPORTED_TARGET cairo)
pkg_check_modules(sdl2 IMPORTED_TARGET sdl2)
pkg_check_modules(pangocairo IMPORTED_TARGET pangocairo)
pkg_check_modules(gtest IMPORTED_TARGET gtest)
pkg_check_modules(gtest_main IMPORTED_TARGET gtest_main)

pkg_check_modules(FOR_EVERY_TARGETS IMPORTED_TARGET fmt)
link_libraries(PkgConfig::FOR_EVERY_TARGETS) # All targets

add_executable(pango_basic graphics/font/pango_basic.cpp)
target_link_libraries(pango_basic PkgConfig::pangocairo)

add_executable(pango_text_wrapping graphics/font/pango_text_wrapping.cpp)
target_link_libraries(pango_text_wrapping PkgConfig::pangocairo)

add_executable(sdl_sinWave graphics/SDL_sinWave.cpp graphics/SDL_window.cpp)
target_link_libraries(sdl_sinWave PkgConfig::sdl2 PkgConfig::cairo)

add_executable(sdl_drawText graphics/SDL_drawText.cpp graphics/SDL_window.cpp)
target_link_libraries(sdl_drawText PkgConfig::sdl2 PkgConfig::pangocairo)

add_executable(sdl_keyboard_ev graphics/SDL_window.cpp graphics/SDL_keyboards_ev.cpp)
target_link_libraries(sdl_keyboard_ev PkgConfig::sdl2 PkgConfig::pangocairo)

# Sexpr

add_library(sexpr_rec STATIC sexpr/sexpr_rec.cpp)

add_executable(sexpr_test sexpr/sexpr_rec_tests.cpp)
target_link_libraries(sexpr_test sexpr_rec PkgConfig::gtest_main)

include(ffi/minidraw/minidraw.cmake)


# IPC methods

add_executable(randomSender ipc/pipe/echoRandoms/randomSender.cpp)
set_target_properties(randomSender PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/ipc/pipe/echoRandoms/)