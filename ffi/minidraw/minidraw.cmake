# mini draw racket ffi
add_library(minidraw SHARED ${CMAKE_CURRENT_LIST_DIR}/minidraw.cpp)
target_link_libraries(minidraw PkgConfig::cairo)

set_target_properties(minidraw PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
