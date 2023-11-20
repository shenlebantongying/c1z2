#include "Z_button.h"

Z_button::Z_button(Z_widget* parent, int rel_x, int rel_y, int w, int h)
    : Z_widget(parent, rel_x, rel_y, w, h)
{
}

void Z_button::draw()
{
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_line_width(cr, 5);

    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_move_to(cr, 0, 0);
    cairo_line_to(cr, width, height);
    cairo_stroke(cr);

    cairo_rectangle(cr, 0, 0, width, height);
}