# HiDPI sizing relationships

* https://github.com/libsdl-org/SDL/blob/main/docs/README-highdpi.md
* cairo_surface_set_device_scale -> https://www.cairographics.org/manual/cairo-cairo-surface-t.html#cairo-surface-set-device-scale

`SDL pixel density` = `window pixel size` / `window size`

`window pixel size` will be the size of drawing buffer `SDL_Surface/SDL_Texture`

We set `cairo_surface_set_device_scale` to `SDL pixel density`, 
so that we draw on coordinates of `window size` without considering the underlying buffer

# Drawing buffer

`SDL_Surface` and `cairo_surface_t` shares the same data buffer.

Note that cairo surface's RGB values are pre-multiplied by alpha.