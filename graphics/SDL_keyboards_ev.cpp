#include "SDL_window.h"
#include <pango/pangocairo.h>
#include <string>

struct demo_key_ev : public SDL_window {

    PangoLayout* layout;
    bool newKeyArrival = false;
    std::string text {};

    demo_key_ev(int width, int height)
        : SDL_window(width, height) {

        };
    ~demo_key_ev() = default;

    void draw() override
    {
        cairo_set_source_rgba(cr, 1, 1, 1, 1.0);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);

        pango_layout_set_text(layout, text.data(), (int)text.length());

        cairo_set_source_rgb(cr, 0, 0, 0);

        pango_cairo_update_layout(cr, layout);

        pango_cairo_show_layout(cr, layout);
    };

    void updateStates() override
    {
        if (newKeyArrival) {
            newKeyArrival = false;
            requestRedraw();
        }
    };

    bool additionalInit() override
    {

        layout = pango_cairo_create_layout(cr);

        pango_layout_set_width(layout, width * PANGO_SCALE);
        pango_layout_set_wrap(layout, PANGO_WRAP_WORD);

        PangoFontDescription* desc;

        desc = pango_font_description_from_string("Sans 12");
        pango_layout_set_font_description(layout, desc);
        pango_font_description_free(desc);
        return true;
    };

    void onEvent(SDL_Event& ev) override
    {

        if (ev.type == SDL_EVENT_KEY_DOWN) {
            switch (ev.key.scancode) {
            case SDL_SCANCODE_W:
                text = "w pressed\n";
                newKeyArrival = true;
                break;
            default:
                text.clear();
                text.append("keyPressed -> ");

                auto mod = SDL_GetModState();

                switch (mod) {
                case SDL_KMOD_LCTRL:
                    text.append("LCtrl + ");
                    break;
                case SDL_KMOD_LSHIFT:
                    text.append("LShift + ");
                    break;
                default:
                    break;
                }

                text.append(SDL_GetScancodeName(ev.key.scancode));
                newKeyArrival = true;
                break;
            }
        }
    };
};

int main()
{
    demo_key_ev app { 800, 600 };
    return app.run();
}
