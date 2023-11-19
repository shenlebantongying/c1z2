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
    scalingFactor = SDL_GetWindowDisplayScale(window);

    renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("SDL_CreateRenderer failed %s\n", SDL_GetError());
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Ensure window shown before event polling
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    adjustTexture();
}

bool Z_app::adjustTexture()
{
    SDL_DestroyTexture(sdl_texture);
    sdl_texture = SDL_CreateTexture(renderer,
        SDL_GetPixelFormatEnumForMasks(32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0),
        SDL_TEXTUREACCESS_STREAMING, width, height);

    return true;
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
                printf("Resized -> w %d h %d\n", width, height);
                adjustTexture();
                blit();
            } else {
                // onEvent(event);
            }
        }

        blit();
    }
    return 0;
}

void Z_app::blit() const
{
    mainWidget->draw();
    SDL_UpdateTexture(sdl_texture, nullptr, mainWidget->pixels(), mainWidget->stride());

    // here
    for (Z_widget* w : *mainWidget->children) {
        w->draw();
        SDL_Rect target { w->relative_x, w->relative_y, w->width, w->height };
        SDL_UpdateTexture(sdl_texture, &target, w->pixels(), w->stride());
    }

    auto target = SDL_FRect { 0, 0, float(width * scalingFactor), float(height * scalingFactor) };
    SDL_RenderTexture(renderer, sdl_texture, nullptr, &target);
    SDL_RenderPresent(renderer);
}
