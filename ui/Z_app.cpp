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

    window = SDL_CreateWindow("Drawing board",
        width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        printf("SDL_CreateWindow failed: %s", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
    sdl_texture = SDL_CreateTexture(renderer,
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
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {

            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                resize(event.window.data1, event.window.data2);
                updateTexture();
                rec_updateLayout(mainWidget);
            } else {
                // onEvent(event);
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
    SDL_Rect target { base_x + widget->relative_x, base_y + widget->relative_y, widget->width, widget->height };
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
