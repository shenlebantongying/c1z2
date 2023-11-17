add_library(sdl_base STATIC ${CMAKE_CURRENT_LIST_DIR}/SDL_window.cpp)
target_link_libraries(sdl_base PkgConfig::cairo PkgConfig::sdl2)

add_executable(sdl_sinWave ${CMAKE_CURRENT_LIST_DIR}/SDL_sinWave.cpp)
target_link_libraries(sdl_sinWave PkgConfig::sdl2 PkgConfig::cairo sdl_base)

add_executable(sdl_drawText ${CMAKE_CURRENT_LIST_DIR}/SDL_drawText.cpp)
target_link_libraries(sdl_drawText PkgConfig::sdl2 PkgConfig::pangocairo sdl_base)

add_executable(sdl_keyboard_ev ${CMAKE_CURRENT_LIST_DIR}/SDL_keyboards_ev.cpp)
target_link_libraries(sdl_keyboard_ev PkgConfig::sdl2 PkgConfig::pangocairo sdl_base)

add_executable(SDL_drag_resize_rect ${CMAKE_CURRENT_LIST_DIR}/SDL_drag_resize_rect.cpp)
target_link_libraries(SDL_drag_resize_rect PkgConfig::sdl2 PkgConfig::pangocairo sdl_base)

