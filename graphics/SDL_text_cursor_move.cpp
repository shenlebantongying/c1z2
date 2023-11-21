#include "SDL_window.h"
#include <pango/pango.h>
#include <pango/pangocairo.h>
#include <string>

class textCursor : public SDL_window {

    std::string text = R"""( 💜 你好世界 Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.)""";
    PangoLayout* pgLayout;
    PangoContext* pgContext;

    PangoRectangle strongPos {};

    int lineTrailing = 0;

    float verticalOffset = 0;

    int textPos = 2;

public:
    textCursor(int w, int h)
        : SDL_window(w, h)
    {
    }

    ~textCursor()
    {
        g_object_unref(pgLayout);
    };

    bool additionalInit() override
    {
        for (int i = 0; i < 10; ++i) {
            text.append(text);
        }

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
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);

        pango_layout_set_width(pgLayout, pango_units_from_double(width));
        pango_cairo_update_layout(cr, pgLayout);

        cairo_translate(cr, 0, verticalOffset);
        pango_cairo_show_layout(cr, pgLayout);
        cairo_translate(cr, 0, -verticalOffset);

        pango_layout_get_cursor_pos(pgLayout, textPos, &strongPos, nullptr);

        auto x = pango_units_to_double(strongPos.x);
        auto y = pango_units_to_double(strongPos.y);
        auto h = pango_units_to_double(strongPos.height);

        cairo_set_source_rgba(cr, 1, 0, 0, 0.8);

        cairo_rectangle(cr, x, y - 3 + verticalOffset, 2, h + 6);

        cairo_fill(cr);
    }

    void updateStates() override
    {
    }
    void onEvent(SDL_Event& ev) override
    {
        if (ev.type == SDL_EVENT_KEY_DOWN) {
            switch (ev.key.keysym.scancode) {
            case SDL_SCANCODE_LEFT:
                pango_layout_move_cursor_visually(pgLayout, true, textPos, lineTrailing, -1, &textPos, &lineTrailing);
                break;
            case SDL_SCANCODE_RIGHT:
                pango_layout_move_cursor_visually(pgLayout, true, textPos, lineTrailing, 1, &textPos, &lineTrailing);
                break;
            default:
                break;
            }
            printf("textBytePos -> %d\n", textPos);
            requestRedraw();
        }

        if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            pango_layout_xy_to_index(pgLayout,
                pango_units_from_double(ev.button.x),
                pango_units_from_double(ev.button.y - verticalOffset),
                &textPos,
                nullptr);
            requestRedraw();
        }

        if (ev.type == SDL_EVENT_MOUSE_WHEEL) {
            verticalOffset += ev.wheel.y * 3;
            if (verticalOffset > 10) {
                verticalOffset = 10;
            }
            requestRedraw();
        }
    }
};

int main()
{
    textCursor app { 600, 600 };
    return app.run();
}
