#include "SDL_window.h"
#include <fmt/format.h>
#include <pango/pangocairo.h>
#include <string>

// see utf-8 code point def
inline int find_previous_utf8(std::string& s, int cur_pos)
{
    for (auto i = cur_pos - 1; i >= 0; --i) {
        if ((s[i] & 0b11'00'00'00) != 0b10'00'00'00) {
            return i;
        }
    }
    return 0;
}

class textEdit : public SDL_window {

    std::string text = R"""(Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.)""";
    PangoLayout* pgLayout;
    PangoContext* pgContext;

    PangoRectangle strongPos {};

    int textBytePos = 2;

public:
    textEdit(int w, int h)
        : SDL_window(w, h)
    {
    }

    ~textEdit()
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

        pango_layout_get_cursor_pos(pgLayout, textBytePos, &strongPos, nullptr);

        auto x = pango_units_to_double(strongPos.x);
        auto y = pango_units_to_double(strongPos.y);
        auto h = pango_units_to_double(strongPos.height);

        cairo_set_source_rgba(cr, 1, 0, 0, 0.8);

        cairo_rectangle(cr, x - 1, y - 3, 2, h + 6);

        cairo_fill(cr);
    }

    void updateStates() override
    {
    }

    void onEvent(SDL_Event& ev) override
    {

        switch (ev.type) {

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            pango_layout_xy_to_index(pgLayout,
                pango_units_from_double(ev.button.x),
                pango_units_from_double(ev.button.y),
                &textBytePos,
                nullptr);
            requestRedraw();
            break;
        case SDL_EVENT_TEXT_EDITING: {
            Sint32 cursor = ev.edit.start;
            Sint32 selection_len = ev.edit.length;
            std::string composition = ev.edit.text;

            fmt::println("{} {} {}", cursor, selection_len, composition);
            break;
        }

        case SDL_EVENT_KEY_DOWN:
            fmt::println("KeyDown -> {}", SDL_GetScancodeName(ev.key.keysym.scancode));
            if (ev.key.keysym.sym == SDLK_BACKSPACE) {
                auto lpos = find_previous_utf8(text, textBytePos);
                text.erase(lpos, textBytePos - lpos);

                fmt::println("Erasing lpos {} textBytePos {}", lpos, textBytePos);

                pango_layout_set_text(pgLayout, text.data(), (int)text.length());
                textBytePos = lpos;

                requestRedraw();
            }
            break;
        case SDL_EVENT_TEXT_INPUT: {
            char* new_text = ev.text.text; // owned by ev, no need to free
            fmt::println("Added -> {}", new_text);

            text.insert(textBytePos, new_text);

            pango_layout_set_text(pgLayout, text.data(), (int)text.length());
            textBytePos += strlen(new_text);

            requestRedraw();
        } break;
        }
    }
};

int main()
{
    textEdit app { 800, 600 };
    return app.run();
}
