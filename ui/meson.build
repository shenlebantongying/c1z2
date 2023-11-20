Z_ui_inc = include_directories('.')
Z_ui_lib = static_library('z_lib',
                          ['Z_app.cpp',
                           'Z_button.cpp',
                           'Z_widget.cpp'],
                          dependencies : [SDL3_dep, cairo_dep, pangocairo_dep])

Z_ui_dep = declare_dependency(link_with : Z_ui_lib,
                              include_directories : Z_ui_inc)

subdir('tests')