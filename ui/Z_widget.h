#pragma once

#include <vector>
#include <SDL.h>
#include <cairo/cairo.h>

struct Z_widget {

    cairo_surface_t* surface;
    cairo_t* cr;

    Z_widget* parent;
    std::vector<Z_widget*>* children;

    // position relateive to parent
    int relative_x;
    int relative_y;

    int width;
    int height;

    Z_widget(Z_widget* parent, int rel_x, int rel_y, int w, int h);

    void add_child(Z_widget * o);

    virtual void draw();

    void* pixels();
    int stride();

    ~Z_widget();
};
