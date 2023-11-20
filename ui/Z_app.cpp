#include "Z_app.h"

#include <cstdio>
#include <cstdlib>

Z_app::~Z_app()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Z_app::Z_app(int init_width, int init_height)
    : width(init_width)
    , height(init_height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init failed: %s", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("Drawing board", width, height,
        SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        printf("SDL_CreateWindow failed: %s", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(
        window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("SDL_CreateRenderer failed %s\n", SDL_GetError());
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Ensure window shown before event polling
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    updateTexture();
}

void Z_app::updateTexture()
{
    SDL_DestroyTexture(sdl_texture);
    sdl_texture = SDL_CreateTexture(
        renderer,
        SDL_GetPixelFormatEnumForMasks(32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0),
        SDL_TEXTUREACCESS_STREAMING, width, height);
}

Z_widget* Z_app::getMainWidget()
{
    if (mainWidget == nullptr) {
        mainWidget = new Z_widget(nullptr, 0, 0, width, height);
    }
    return mainWidget;
}

int Z_app::run()
{
    if (mainWidget == nullptr) {
        return 1;
    }

    rec_updateLayout(mainWidget);
    blit();

    bool done = false;
    while (!done) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            switch (ev.type) {
            case SDL_EVENT_QUIT:
                done = true;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                resize(ev.window.data1, ev.window.data2);
                updateTexture();
                rec_updateLayout(mainWidget);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                Z_widget* hitted = hit_testing(mainWidget, ev.button.x, ev.button.y);
                if (hitted != nullptr) {
                    printf("Hit -> %s %p\n", hitted->toStr(), hitted);
                    hitted->responseToHitEvent();
                }
            } break;
            default:
            }
        }

        blit();
    }
    return 0;
}

void Z_app::blit()
{
    mainWidget->resize(width, height);
    rec_updateTexture(mainWidget, 0, 0);

    SDL_RenderTexture(renderer, sdl_texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Z_app::rec_updateTexture(Z_widget* widget, int base_x, int base_y)
{
    widget->draw();
    SDL_Rect target { base_x + widget->relative_x, base_y + widget->relative_y,
        widget->width, widget->height };
    SDL_UpdateTexture(sdl_texture, &target, widget->pixels(), widget->stride());

    for (auto* c : widget->children) {
        rec_updateTexture(c, target.x, target.y);
    }
}

void Z_app::rec_updateLayout(Z_widget* widget)
{
    widget->relayout();
    for (auto* w : widget->children) {
        rec_updateLayout(w);
    }
}

void Z_app::resize(int w, int h)
{
    width = w;
    height = h;
    mainWidget->width = w;
    mainWidget->height = h;
}

Z_widget* Z_app::hit_testing(Z_widget* w, int x, int y)
{
    if (x > w->width || y > w->height) {
        return nullptr;
    }

    if (w->children.empty()) {
        return w;
    }

    auto c = SDL_Point { x, y };

    for (auto* o : w->children) {
        auto t = SDL_Rect { o->relative_x, o->relative_y, o->width, o->height };
        if (SDL_PointInRect(&c, &t)) {
            return hit_testing(o, x - o->relative_x, y - o->relative_y);
        }
    }

    return nullptr;
}
