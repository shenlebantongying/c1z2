# mini draw racket ffi
add_library(minidraw SHARED ffi/minidraw/minidraw.cpp)
target_link_libraries(minidraw PkgConfig::cairo)

set_target_properties(minidraw PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/ffi/minidraw)
