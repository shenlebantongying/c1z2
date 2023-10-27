#include <pango/pangocairo.h>
#include <string>

enum {
    WIDTH = 600,
    HEIGHT = 600
};

void printLayoutSizes(PangoLayout* layout)
{
    int layout_width = 0;
    int layout_height = 0;

    pango_layout_get_size(layout, &layout_width, &layout_height);
    printf("Layout Size -> w %d h %d\n", layout_width, layout_height);

    PangoRectangle ink_rect, logical_rect;
    pango_layout_get_extents(layout, &ink_rect, &logical_rect);
    printf("Extents-> ink w %d h %d ; logical w %d h %d\n",
        ink_rect.width, ink_rect.height, logical_rect.width, logical_rect.height);

    pango_layout_get_pixel_extents(layout, &ink_rect, &logical_rect);
    printf("Pixel Extents-> ink w %d h %d ; logical w %d h %d\n",
        ink_rect.width, ink_rect.height, logical_rect.width, logical_rect.height);
}

void draw_text(cairo_t* cr)
{

    std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    PangoLayout* layout;

    cairo_translate(cr, 0, 0);

    layout = pango_cairo_create_layout(cr);

    pango_layout_set_width(layout, WIDTH * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD);

    pango_layout_set_text(layout, text.c_str(), text.length());

    // set font, not that the desc dies very soon.
    PangoFontDescription* desc;
    desc = pango_font_description_from_string("Sans 14");
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    cairo_set_source_rgb(cr, 0, 0, 0);

    pango_cairo_update_layout(cr, layout);

    pango_cairo_show_layout(cr, layout);

    printLayoutSizes(layout);

    g_object_unref(layout);
}

int main()
{
    cairo_t* cr;
    const char* filename = "pango_text_wrapping.png";
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
