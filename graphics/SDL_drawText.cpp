#include "SDL_window.h"
#include <pango/pangocairo.h>
#include <string_view>

class drawText : public SDL_window {

    std::string_view text = R"""(Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.)""";
    PangoLayout* pgLayout;
    PangoContext* pgContext;

public:
    drawText(int w, int h)
        : SDL_window(w, h)
    {
    }

    ~drawText()
    {
        g_object_unref(pgLayout);
    };

    bool additionalInit() override
    {
        pgContext = pango_cairo_create_context(cr);
        pgLayout = pango_layout_new(pgContext);

        pango_layout_set_wrap(pgLayout, PANGO_WRAP_WORD);

        pango_layout_set_text(pgLayout, text.data(), (int)text.length());

        PangoFontDescription* desc;

        desc = pango_font_description_from_string("Sans 12");
        pango_layout_set_font_description(pgLayout, desc);
        pango_font_description_free(desc);
        return true;
    }

    void draw() override
    {
        cairo_set_source_rgba(cr, 1, 1, 1, 1.0);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0, 0, 1);

        pango_layout_set_width(pgLayout, width * PANGO_SCALE);

        pango_cairo_update_layout(cr, pgLayout);

        pango_cairo_show_layout(cr, pgLayout);
    }

    void updateStates() override
    {
        if (SDL_GetTicks() >= lastUpdateTime + 10) {
            lastUpdateTime = SDL_GetTicks();
            requestRedraw();
        }
    }
};

int main()
{
    drawText app { 800, 600 };
    return app.run();
}
