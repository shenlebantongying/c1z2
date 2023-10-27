#include "minidraw.h"
#include <cstdio>

drawBoard* a;

void initDrawBoard(int w, int h)
{
    if (a == nullptr) {
        a = new drawBoard();
    }
    a->init(w, h);
};

void savePng(char* name)
{
    a->savePng(name);
}

void setColor(int x, int y, int z)
{
    a->setColor(x, y, z);
}

void lineTo(int x, int y)
{
    a->lineTo(x, y);
}

void moveTo(int x, int y)
{
    a->moveTo(x, y);
}

void stroke()
{
    a->stroke();
};

bool drawBoard::init(int64_t width, int64_t height)
{
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    return true;
}

void drawBoard::moveTo(double x, double y)
{
    cairo_move_to(cr, x, y);
}

void drawBoard::lineTo(double x, double y)
{
    cairo_line_to(cr, x, y);
}

void drawBoard::stroke()
{
    cairo_stroke(cr);
}

void drawBoard::savePng(char* name)
{
    auto status = cairo_surface_write_to_png(surface, name);
    printf("%d -> %s\n", status, name);
}

void drawBoard::setColor(int x, int y, int z)
{
    cairo_set_source_rgb(cr, x / 255, y / 255, z / 255);
}
