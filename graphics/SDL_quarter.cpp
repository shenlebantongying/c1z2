#include "SDL_window.h"
#include <SDL3/SDL.h>
#include <cairo.h>

class quarter : public SDL_window {

public:
    quarter(int w, int h)
        : SDL_window(w, h)
    {
    }

    void draw() override
    {
        cairo_set_source_rgb(cr, 0, 1, 0);
        cairo_rectangle(cr, 0, 0, width / 2, height / 2);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0, 1, 0);
        cairo_rectangle(cr, width / 2, height / 2, width / 4, height / 4);
        cairo_fill(cr);
    }

    void updateStates() override
    {
        if (SDL_GetTicks() >= lastUpdateTime + 100) {
            lastUpdateTime = SDL_GetTicks();
            requestRedraw();
        }
    }
};

int main()
{
    quarter app { 800, 600 };
    return app.run();
}
