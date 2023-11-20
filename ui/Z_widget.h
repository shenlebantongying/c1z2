#pragma once

#include "Z_ns.h"
#include <SDL.h>
#include <cairo.h>
#include <string_view>
#include <vector>

struct Z_widget {

    Z_widget(Z_widget* parent, int rel_x, int rel_y, int w, int h);

    Z_widget* parent;
    std::vector<Z_widget*> children;

    // position relative to parent
    int relative_x;
    int relative_y;

    int width;
    int height;

    cairo_surface_t* surface;
    cairo_t* cr;

    Z_layout layout = Z_layout::NO;

    void add_child(Z_widget* o);
    void relayout();

    virtual void draw();

    void resize(int w, int h);
    void repos(int x, int y);

    void* pixels();
    int stride();

    virtual const char* toStr();

    virtual void responseToHitEvent();
    virtual ~Z_widget();
};
