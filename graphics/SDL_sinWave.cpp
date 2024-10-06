#include "SDL_window.h"
#include <SDL3/SDL.h>
#include <cairo.h>
#include <math.h>

class sinWave : public SDL_window {

    int delta = 0;

public:
    sinWave(int w, int h)
        : SDL_window(w, h)
    {
    }

    void draw() override
    {
        cairo_set_source_rgb(cr, 0, double(delta * 10 % 255) / 255.0, 0);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 1, 1, 1);

        for (int i = 0; i <= width; i++) {
            double x = i;
            double y = height / 2.0 - sin(x / (M_PI * 10) + delta) * height / 4;
            if (i == 0) {
                cairo_move_to(cr, x, y);
            } else {
                cairo_line_to(cr, x, y);
            }
        }

        cairo_stroke(cr);
    }

    void updateStates() override
    {
        if (SDL_GetTicks() >= lastUpdateTime + 100) {
            delta += 1;
            lastUpdateTime = SDL_GetTicks();
            requestRedraw();
        }
    }
};

int main()
{
    sinWave app { 800, 600 };
    return app.run();
}
