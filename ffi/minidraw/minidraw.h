#pragma once

#include <cairo.h>
#include <cmath>
#include <cstdint>

struct drawBoard {
    cairo_surface_t* surface;
    cairo_t* cr;

    drawBoard() = default;
    bool init(int64_t width, int64_t height);

    void setColor(int x, int y, int z);
    void lineTo(double x, double y);
    void moveTo(double x, double y);
    void stroke();

    void savePng(char* name);
};

extern "C" {
const char* version(); // for test;
void initDrawBoard(int x, int y);
void setColor(int x, int y, int z); //  0~255
void lineTo(int x, int y);
void moveTo(int x, int y);
void stroke(void);
void savePng(char* name);
}
