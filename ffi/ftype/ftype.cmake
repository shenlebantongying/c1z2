# mini draw racket ffi
add_library(pointer-types SHARED ${CMAKE_CURRENT_LIST_DIR}/pointer-types.cpp)
set_target_properties(pointer-types PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
