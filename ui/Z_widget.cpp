#include "Z_widget.h"
#include <algorithm>

Z_widget::Z_widget(Z_widget* parent, int rel_x, int rel_y, int w, int h)
{
    relative_x = rel_x;
    relative_y = rel_y;
    width = w;
    height = h;

    this->parent = parent;

    children = new std::vector<Z_widget*>();

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
    if (std::ranges::find(children->cbegin(), children->cend(), o) == children->end()) {
        children->push_back(o);
    }
}

Z_widget::~Z_widget()
{
    cairo_surface_destroy(surface);
}
