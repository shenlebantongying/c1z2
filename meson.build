project('c1z2',
        ['cpp'],
        version : '0.1',
        default_options : ['warning_level=3','cpp_std=c++23'])

cairo_dep = dependency('cairo')
pangocairo_dep = dependency('pangocairo')
fmt_dep = dependency('fmt')

cc = meson.get_compiler('cpp')

SDL3_dep = declare_dependency(
  dependencies : cc.find_library('SDL3', dirs : [meson.current_source_dir()+'/SDL/build/']),
  include_directories : include_directories(['./SDL/include/SDL3','./SDL/include/']),
)

subdir('ffi')
subdir('graphics')
subdir('ui')