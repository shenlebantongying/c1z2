#include <SDL.h>
#include <cairo.h>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <pango/pangocairo.h>
#include <string>
#include <string_view>
#include <vector>

#define EXIT std::exit(1);

inline int find_previous_utf8(std::string &s, int cur_pos) {
  for (auto i = cur_pos - 1; i >= 0; --i) {
    if ((s[i] & 0b11'00'00'00) != 0b10'00'00'00) {
      return i;
    }
  }
  return 0;
}

struct cursor_pos_t {
  int line_n;
  int byte_n;
};

struct cuno_line {
  explicit cuno_line(PangoContext *pangoContext) {
    layout = pango_layout_new(pangoContext);
  }
  PangoLayout *layout;

  ~cuno_line() { g_object_unref(this->layout); }
};

struct cuno {

  cairo_t *cr;

  float scalingFactor;

  std::vector<cuno_line *> lines{};

  std::string get_line_text(int n) {
    return {pango_layout_get_text(lines[n]->layout)};
  }

  void set_line_text(int n, std::string_view s) {
    pango_layout_set_text(lines[n]->layout, s.data(), s.length());
  }

  bool needRedraw = false;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *sdl_surface{nullptr};
  SDL_Texture *sdl_texture{nullptr};
  cairo_surface_t *cr_surface{nullptr};

  SDL_Rect IME_pos{0, 0, 2, line_height_for_now()};

  int vertical_offset = 0;

  int width;
  int height;

  int line_height_for_now() { return 20; };

  const static int scroll_speed = 10;

  PangoContext *pgContext;
  PangoFontDescription *desc;

  cursor_pos_t cursor{0, 0};

  PangoRectangle cursor_rect{};

  cuno(int init_width, int init_height) {
    width = init_width;
    height = init_height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
      printf("SDL_Init failed: %s", SDL_GetError());
      EXIT
    }

    window =
        SDL_CreateWindow("Drawing board", width, height,
                         SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
      printf("SDL_CreateWindow failed: %s", SDL_GetError());
      EXIT
    }

    scalingFactor = SDL_GetWindowDisplayScale(window);

    renderer = SDL_CreateRenderer(
        window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
      printf("SDL_CreateRenderer failed %s\n", SDL_GetError());
      EXIT
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Ensure window shown before event polling
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    recreate_surfaces();

    pgContext = pango_cairo_create_context(cr);
    desc = pango_font_description_from_string("Sans 12");
  }

  ~cuno() {
    cairo_destroy(cr);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
  };

  void draw() {
    cairo_set_source_rgba(cr, 1, 1, 1, 1.0);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    cairo_save(cr);
    cairo_translate(cr, 0, vertical_offset);

    cairo_set_source_rgb(cr, 0, 0, 1);

    for (int i = 0; i < lines.size(); ++i) {
      cairo_save(cr);
      cairo_translate(cr, 0, line_height_for_now() * i);
      pango_cairo_update_layout(cr, lines[i]->layout);
      pango_cairo_show_layout(cr, lines[i]->layout);
      cairo_restore(cr);
    }

    auto x = pango_units_to_double(cursor_rect.x);
    auto h = pango_units_to_double(cursor_rect.height);

    cairo_set_source_rgba(cr, 1, 0, 0, 0.8);
    cairo_rectangle(cr, x - 1, line_height_for_now() * cursor.line_n - 3, 2,
                    h + 6);

    cairo_fill(cr);

    cairo_restore(cr);
  }

  bool recreate_surfaces() {
    SDL_DestroySurface(sdl_surface);
    if (cr_surface) {
      cairo_surface_destroy(cr_surface);
    }

    sdl_surface = SDL_CreateSurface(
        width * scalingFactor, height * scalingFactor,
        SDL_GetPixelFormatEnumForMasks(32, 0x00ff0000, 0x0000ff00, 0x000000ff,
                                       0xff000000));
    cr_surface = cairo_image_surface_create_for_data(
        static_cast<unsigned char *>(sdl_surface->pixels), CAIRO_FORMAT_ARGB32,
        sdl_surface->w, sdl_surface->h, sdl_surface->pitch);

    cairo_surface_set_device_scale(cr_surface, scalingFactor, scalingFactor);

    cr = cairo_create(cr_surface);

    sdl_texture = SDL_CreateTextureFromSurface(renderer, sdl_surface);

    return true;
  };

  void request_redraw() { needRedraw = true; };

  int run() {

    draw();
    blit();

    bool done = false;
    while (!done) {

      SDL_Event event;

      while (SDL_PollEvent(&event) != 0) {

        if (event.type == SDL_EVENT_QUIT) {
          done = true;
        } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
          width = event.window.data1;
          height = event.window.data2;
          recreate_surfaces();
          draw();
          blit();
        } else {
          onEvent(event);
        }
      }

      updateStates();

      if (needRedraw) {
        needRedraw = false;
        draw();
        blit();
      }
    }
    return 0;
  };

  void updateStates(){};

  void blit() const {
    cairo_surface_flush(cr_surface);
    SDL_UpdateTexture(sdl_texture, nullptr, sdl_surface->pixels,
                      int(sdl_surface->pitch));

    auto target = SDL_FRect{0, 0, float(width * scalingFactor),
                            float(height * scalingFactor)};
    SDL_RenderTexture(renderer, sdl_texture, nullptr, &target);
    SDL_RenderPresent(renderer);
  };

  void update_cursor_pos() {
    // update cursor pos;
    pango_layout_get_cursor_pos(lines[cursor.line_n]->layout, cursor.byte_n,
                                &cursor_rect, nullptr);

    // change IME position
    IME_pos = {int(pango_units_to_double(cursor_rect.x)),
               line_height_for_now() * cursor.line_n, 2, line_height_for_now()};

    SDL_SetTextInputRect(&IME_pos);
  }

  void onEvent(SDL_Event &ev) {

    switch (ev.type) {
    // scrolling
    case SDL_EVENT_MOUSE_WHEEL: {
      if (ev.wheel.y > 0) {
        if (vertical_offset < 0) {
          vertical_offset += scroll_speed;
        }
      } else {
        if (vertical_offset >
            -int(height / 3 + lines.size() * line_height_for_now())) {
          vertical_offset -= scroll_speed;
        }
      }
      request_redraw();
    } break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN: {
      int possible_cursor_line_n =
          (-vertical_offset + int(ev.button.y)) / line_height_for_now();

      if (possible_cursor_line_n >= lines.size()) {
        break;
      }

      cursor.line_n = possible_cursor_line_n;

      gboolean inside = pango_layout_xy_to_index(
          lines[cursor.line_n]->layout, pango_units_from_double(ev.button.x),
          pango_units_from_double(0), &cursor.byte_n, nullptr);
      if (!inside) {
        cursor.byte_n = get_line_text(cursor.line_n).size();
      }

      update_cursor_pos();
      request_redraw();
    } break;
    case SDL_EVENT_TEXT_EDITING: {
      Sint32 cursor = ev.edit.start;
      Sint32 selection_len = ev.edit.length;
      std::string composition = ev.edit.text;
      // TODO
    } break;
    case SDL_EVENT_KEY_DOWN: {

      // Ctrl hold
      if ((SDL_GetModState() & SDL_KMOD_LCTRL) == SDL_KMOD_LCTRL) {
        printf("Pressed ->%s\n", SDL_GetKeyName(ev.key.keysym.sym));

        bool cursor_moved = false;
        switch (ev.key.keysym.sym) {
        case SDLK_w:
          if (cursor.line_n >= 1) {
            cursor.line_n -= 1;
            cursor_moved = true;

            auto lineSize = get_line_text(cursor.line_n).size();
            if (cursor.byte_n > lineSize) {
              cursor.byte_n = lineSize;
            }
          }
          break;
        case SDLK_s:
          if (cursor.line_n <= lines.size() - 1 - 1) {
            cursor.line_n += 1;
            cursor_moved = true;

            auto lineSize = get_line_text(cursor.line_n).size();
            if (cursor.byte_n > lineSize) {
              cursor.byte_n = lineSize;
            }
          }
          break;
        case SDLK_a:
          if (cursor.byte_n >= 1) {
            cursor.byte_n -= 1;
            cursor_moved = true;
          }
          break;
        case SDLK_d:
          if (cursor.byte_n <= get_line_text(cursor.line_n).size() - 1) {
            cursor.byte_n += 1;
            cursor_moved = true;
          }
          break;
        }
        if (cursor_moved) {
          update_cursor_pos();
          request_redraw();
          break;
        }
      } else if (ev.key.keysym.sym == SDLK_BACKSPACE) {
        std::string t = pango_layout_get_text(lines[cursor.line_n]->layout);

        if (cursor.byte_n != 0) {

          auto lpos = find_previous_utf8(t, cursor.byte_n);

          t.erase(lpos, cursor.byte_n - lpos);

          pango_layout_set_text(lines[cursor.line_n]->layout, t.data(),
                                (int)t.length());

          cursor.byte_n = lpos;
        } else if (cursor.byte_n == 0) {
          if (cursor.line_n >= 1) {
            cursor.line_n -= 1;

            // merge current line with next line
            std::string cur = get_line_text(cursor.line_n);
            std::string next = get_line_text(cursor.line_n + 1);

            cursor.byte_n = cur.length();

            cur.append(next);

            set_line_text(cursor.line_n, cur);

            // TODO: shifting everything here
            for (int i = cursor.line_n + 1; i < lines.size() - 1; ++i) {
              std::string s = get_line_text(i + 1);
              set_line_text(i, s);
            }
            lines.pop_back();
          }
        } else {
          std::exit(1);
        }

        update_cursor_pos();
        request_redraw();
      } else if (ev.key.keysym.sym == SDLK_RETURN) {
        std::string t = pango_layout_get_text(lines[cursor.line_n]->layout);
        std::string car = t.substr(0, cursor.byte_n);
        std::string cdr = t.substr(cursor.byte_n, t.length() - cursor.byte_n);

        pango_layout_set_text(lines[cursor.line_n]->layout, car.data(),
                              car.length());

        auto *l = new cuno_line(pgContext);
        pango_layout_set_text(l->layout, cdr.data(), cdr.length());
        lines.insert(lines.begin() + cursor.line_n + 1, l);

        cursor.line_n += 1;
        cursor.byte_n = 0;

        update_cursor_pos();
        request_redraw();
      }
    } break;
    case SDL_EVENT_TEXT_INPUT: {
      std::string new_text = ev.text.text; // owned by ev, no need to free
      // fmt::println("Added -> {} at {} {}", new_text,
      // cursor_pos.line_n, cursor_pos.byte_n);
      std::string current_line_text =
          pango_layout_get_text(lines[cursor.line_n]->layout);
      current_line_text.insert(cursor.byte_n, new_text);
      pango_layout_set_text(lines[cursor.line_n]->layout,
                            current_line_text.data(), current_line_text.size());

      cursor.byte_n += new_text.size();

      update_cursor_pos();

      request_redraw();
    } break;
    }
  }

  void set_text(std::string_view s) {

    for (int i = 0, j = 0; i < s.length(); ++i) {
      if (s[i] == '\n') {
        lines.emplace_back(new cuno_line(pgContext));
        auto *l = lines.back();
        pango_layout_set_text(l->layout, s.substr(j, i - j).data(), i - j);
        fmt::println("{} {} {}", i, j, s.substr(j, i - j));

        j = i + 1;
      } else if (i == s.length() - 1) {
        lines.emplace_back(new cuno_line(pgContext));
        auto *l = lines.back();
        pango_layout_set_text(l->layout, s.substr(j, i - j).data(), i - j);
        fmt::println("{} {} {}", i, j, s.substr(j, i - j));
      }
    }
  }
};

bool load_file(cuno &c, std::string_view path) {
  std::fstream f{path.data(), std::ios_base::in};
  if (!f.is_open()) {
    return false;
  }
  std::string s{std::istreambuf_iterator<char>(f), {}};
  c.set_text(s);
  return true;
}

int main(int argc, char **argv) {
  cuno app{800, 600};

  if (argc == 2) {
    if (!load_file(app, argv[1])) {
      EXIT
    }
  } else {
    app.set_text("Cuno [file]\n to open a file.");
  }

  return app.run();
}
