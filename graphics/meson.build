sdl_window_lib = static_library('sdl_window',
                         ['SDL_window.cpp'],
                         gnu_symbol_visibility:'default',
                         dependencies : [SDL3_dep, cairo_dep])

executable('sdl_sinWave', 'SDL_sinWave.cpp',
           link_with : sdl_window_lib, dependencies : [SDL3_dep, cairo_dep, fmt_dep])