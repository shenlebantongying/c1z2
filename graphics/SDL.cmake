add_library(SDL_window STATIC "${CMAKE_CURRENT_LIST_DIR}/SDL_window.cpp")
target_link_libraries(SDL_window PkgConfig::cairo SDL3::SDL3)

macro(SDL_add_p name)
        add_executable("${name}" "${CMAKE_CURRENT_LIST_DIR}/${name}.cpp")
        target_link_libraries("${name}" PkgConfig::cairo  PkgConfig::pangocairo SDL3::SDL3 SDL_window)
endmacro()

SDL_add_p(SDL_sinWave)
SDL_add_p(SDL_scrolling)
SDL_add_p(SDL_drawText)
SDL_add_p(SDL_keyboards_ev)
SDL_add_p(SDL_drag_resize_rect)
SDL_add_p(SDL_quarter)
SDL_add_p(SDL_text_cursor_move)
