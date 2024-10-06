#include "SDL_window.h"
#include <SDL3/SDL.h>
#include <cairo.h>
#include <fmt/format.h>

// hold shift to scroll vertically.

class scrollingCube : public SDL_window {

    int delta = 0;
    float radianOffset = 0;
    float verticalOffset = 0;

public:
    scrollingCube(int w, int h)
        : SDL_window(w, h)
    {
    }

    const float rectW = 200;

    void draw() override
    {
        fmt::print("{}\n", radianOffset);
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0, 0, 1);

        cairo_move_to(cr, width / 2, height / 2);
        cairo_line_to(cr, width / 2 + rectW, height / 2 + rectW);
        cairo_rectangle(cr, width / 2.0 - rectW / 2, height / 2.0 - rectW / 2, rectW, rectW);

        cairo_translate(cr, width / 2.0, height / 2.0);
        fmt::println("Rad -> {}", radianOffset / 1000);
        cairo_rotate(cr, radianOffset / 1000);
        radianOffset = 0;
        cairo_translate(cr, -width / 2.0, -height / 2.0);

        cairo_translate(cr, 0, verticalOffset);
        verticalOffset = 0;

        cairo_stroke(cr);
    }

    void updateStates() override
    {
    }

    void onEvent(SDL_Event& ev) override
    {

        if (ev.type == SDL_EVENT_MOUSE_WHEEL) {
            if (((SDL_KMOD_CTRL | SDL_KMOD_SHIFT) & SDL_GetModState()) != SDL_KMOD_NONE) {
                printf("!Key Holding......\n");
                if (ev.wheel.y > 0) {
                    verticalOffset += 10;
                } else {
                    verticalOffset -= 10;
                }

            } else {
                if (ev.wheel.y > 0) {
                    radianOffset += 10;
                } else {
                    radianOffset -= 10;
                }
            }

            requestRedraw();
        }
    }
};

int main()
{
    scrollingCube app { 800, 600 };
    return app.run();
}
