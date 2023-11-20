#include "Z_widget.h"
#include <algorithm>

Z_widget::Z_widget(Z_widget* parent, int rel_x, int rel_y, int w, int h)
{
    relative_x = rel_x;
    relative_y = rel_y;
    width = w;
    height = h;

    this->parent = parent;

    children = {};

    if (this->parent != nullptr) {
        this->parent->add_child(this);
    }

    surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24,
        width, height);
    cr = cairo_create(surface);
}

void Z_widget::draw()
{
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);
}

void* Z_widget::pixels()
{
    return cairo_image_surface_get_data(surface);
}

int Z_widget::stride()
{
    return cairo_image_surface_get_stride(surface);
}

void Z_widget::add_child(Z_widget* o)
{
    if (std::ranges::find(children.cbegin(), children.cend(), o) == children.end()) {
        children.push_back(o);
    }
}

Z_widget::~Z_widget()
{
    cairo_surface_destroy(surface);
    for (auto* o : children) {
        delete o;
    }
}

void Z_widget::resize(int w, int h)
{

    this->width = w;
    this->height = h;

    cairo_surface_destroy(surface);
    surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24,
        width, height);
    cr = cairo_create(surface);
}

void Z_widget::repos(int rel_x, int rel_y)
{
    relative_x = rel_x;
    relative_y = rel_y;
}

void Z_widget::relayout()
{
    switch (layout) {
    case Z_layout::NO:
        break;
    case Z_layout::H_BOX: {
        int n = children.size();
        int c_size = width / n;
        for (int i = 0; i < n; ++i) {
            children[i]->resize(c_size, height);
            children[i]->repos(i * c_size, 0);
        }
        break;
    }
    case Z_layout::V_BOX: {
        int n = children.size();
        int c_size = height / n;
        for (int i = 0; i < n; ++i) {
            children[i]->resize(width, c_size);
            children[i]->repos(0, i * c_size);
        }
        break;
    } break;
    }
}
