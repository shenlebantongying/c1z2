#include <pango/pangocairo.h>
#include <string>

enum {
    WIDTH = 800,
    HEIGHT = 1300
};

PangoAttrList* alternating_color_size_attr(guint begin, guint end)
{
    guint gap = 2;
    PangoAttrList* attr_list = pango_attr_list_new();
    for (guint i = begin; i < end - gap; i += gap) {

        auto* sizeAttr = pango_attr_size_new_absolute((i % 50) * PANGO_SCALE);
        auto* colorAttr = pango_attr_foreground_new((i % 10) * 6553, 0, 0);

        sizeAttr->start_index = i;
        sizeAttr->end_index = i + gap;

        colorAttr->start_index = i;
        colorAttr->end_index = i + gap;

        pango_attr_list_insert(attr_list, sizeAttr);
        pango_attr_list_insert(attr_list, colorAttr);
    }

    return attr_list;
}

void draw_text(cairo_t* cr)
{

    std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
                       "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
                       "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    PangoLayout* layout;

    cairo_translate(cr, 0, 0);

    layout = pango_cairo_create_layout(cr);

    // Text Attributes

    pango_layout_set_width(layout, WIDTH * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD);

    pango_layout_set_text(layout, text.c_str(), text.length());

    pango_layout_set_attributes(layout, alternating_color_size_attr(0, text.length()));

    // set font, not that the desc dies very soon.
    PangoFontDescription* desc;
    desc = pango_font_description_from_string("Sans 14");
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    cairo_set_source_rgb(cr, 0, 0, 0);

    pango_cairo_update_layout(cr, layout);

    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
}

int main()
{
    cairo_t* cr;
    const char* filename = "pango_text_attr.png";
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
