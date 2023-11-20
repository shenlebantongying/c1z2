cmake_minimum_required(VERSION 3.27)
project(CasEditor)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(C_STANDARD_REQUIRED ON)
set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(SDL EXCLUDE_FROM_ALL)

find_package(PkgConfig REQUIRED)

pkg_check_modules(cairo IMPORTED_TARGET cairo)
pkg_check_modules(pangocairo IMPORTED_TARGET pangocairo)
pkg_check_modules(gtest IMPORTED_TARGET gtest)
pkg_check_modules(gtest_main IMPORTED_TARGET gtest_main)

pkg_check_modules(FOR_EVERY_TARGETS IMPORTED_TARGET fmt)
link_libraries(PkgConfig::FOR_EVERY_TARGETS) # All targets

include(graphics/font/pango.cmake)

include(graphics/SDL.cmake)

# Sexpr

add_library(sexpr_rec STATIC sexpr/sexpr_rec.cpp)

add_executable(sexpr_test sexpr/sexpr_rec_tests.cpp)
target_link_libraries(sexpr_test sexpr_rec PkgConfig::gtest_main)

# ffi

include(ffi/minidraw/minidraw.cmake)
include(ffi/ftype/ftype.cmake)

# IPC methods

add_executable(randomSender ipc/pipe/echoRandoms/randomSender.cpp)
set_target_properties(randomSender PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/ipc/pipe/echoRandoms/)

# GUI library

add_library(ui SHARED
        ui/Z.h
        ui/Z_app.h
        ui/Z_app.cpp
        ui/Z_widget.cpp
        ui/Z_widget.h
        ui/Z_button.cpp
        ui/Z_button.cpp
        ui/Z_button.h
        ui/Z_ns.h)
target_link_libraries(ui SDL3::SDL3 PkgConfig::cairo)

add_executable(ui_test_nest_layout ui/tests/nest_layout.cpp)
target_link_libraries(ui_test_nest_layout PRIVATE ui)
target_include_directories(ui_test_nest_layout PUBLIC ui)

# Doxygen
find_package(Doxygen
        REQUIRED dot
        OPTIONAL_COMPONENTS mscgen)

if (NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen not found - Documentation will not be generated!")
else ()
    # output -> build/html
    set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doxygen")

    doxygen_add_docs(doxygenDoc
            ui/
    )
endif ()