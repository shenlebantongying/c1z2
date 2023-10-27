#include <pango/pangocairo.h>
#include <string>

#define WIDTH 800
#define HEIGHT 600

static void
draw_text(cairo_t* cr)
{
    std::string text = R"""(Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.)""";

    int layout_width, layout_height;

    PangoLayout* layout;
    PangoFontDescription* desc;

    cairo_translate(cr, 0, 0);

    layout = pango_cairo_create_layout(cr);

    pango_layout_set_text(layout, text.c_str(), text.length());
    desc = pango_font_description_from_string("Sans 14");
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    for (int i = 0; i < 5; i++) {

        cairo_save(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);

        pango_cairo_update_layout(cr, layout);

        pango_layout_get_size(layout, &layout_width, &layout_height);
        cairo_move_to(cr, 0, 40 * i);
        pango_cairo_show_layout(cr, layout);

        cairo_restore(cr);
    }

    g_object_unref(layout);
}

int main()
{
    cairo_t* cr;
    const char* filename = "pango_basic.png";
    cairo_status_t status;
    cairo_surface_t* surface;

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
        WIDTH, HEIGHT);
    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    draw_text(cr);
    cairo_destroy(cr);

    status = cairo_surface_write_to_png(surface, filename);
    cairo_surface_destroy(surface);

    if (status != CAIRO_STATUS_SUCCESS) {
        g_printerr("Could not save png to '%s'\n", filename);
        return 1;
    }

    return 0;
}
