add_library(sdl_base STATIC ${CMAKE_CURRENT_LIST_DIR}/SDL_window.cpp)
target_link_libraries(sdl_base PkgConfig::cairo SDL3::SDL3)

add_executable(sdl_sinWave ${CMAKE_CURRENT_LIST_DIR}/SDL_sinWave.cpp)
target_link_libraries(sdl_sinWave  PkgConfig::cairo SDL3::SDL3 sdl_base)

add_executable(sdl_scrolling ${CMAKE_CURRENT_LIST_DIR}/SDL_scrolling.cpp)
target_link_libraries(sdl_scrolling  PkgConfig::cairo SDL3::SDL3 sdl_base)

add_executable(sdl_drawText ${CMAKE_CURRENT_LIST_DIR}/SDL_drawText.cpp)
target_link_libraries(sdl_drawText PkgConfig::pangocairo SDL3::SDL3 sdl_base)

add_executable(sdl_keyboard_ev ${CMAKE_CURRENT_LIST_DIR}/SDL_keyboards_ev.cpp)
target_link_libraries(sdl_keyboard_ev PkgConfig::pangocairo  SDL3::SDL3 sdl_base)

add_executable(sdl_drag_resize_rect ${CMAKE_CURRENT_LIST_DIR}/SDL_drag_resize_rect.cpp)
target_link_libraries(sdl_drag_resize_rect PkgConfig::pangocairo SDL3::SDL3 sdl_base)

add_executable(sdl_quarter ${CMAKE_CURRENT_LIST_DIR}/SDL_quarter.cpp)
target_link_libraries(sdl_quarter  PkgConfig::cairo SDL3::SDL3 sdl_base)
