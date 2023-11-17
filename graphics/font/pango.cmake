add_executable(pango_basic ${CMAKE_CURRENT_LIST_DIR}/pango_basic.cpp)
target_link_libraries(pango_basic PkgConfig::pangocairo)

add_executable(pango_text_wrapping ${CMAKE_CURRENT_LIST_DIR}/pango_text_wrapping.cpp)
target_link_libraries(pango_text_wrapping PkgConfig::pangocairo)

add_executable(pango_text_attr ${CMAKE_CURRENT_LIST_DIR}/pango_text_attr.cpp)
target_link_libraries(pango_text_attr PkgConfig::pangocairo)