#include "SDL_window.h"

#include <fmt/format.h>

SDL_window::~SDL_window()
{
    cairo_destroy(cr);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_window::SDL_window(int init_width, int init_height)
    : width(init_width)
    , height(init_height)
    , cr_surface(nullptr)
    , sdl_surface(nullptr)
{
}

bool SDL_window::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Drawing board",
        width, height, SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        printf("SDL_CreateWindow failed: %s", SDL_GetError());
        return false;
    }
    scalingFactor = SDL_GetWindowDisplayScale(window);

    renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        printf("SDL_CreateRenderer failed %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Ensure window shown before event polling
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    recreateSurfaces();

    return additionalInit();
}

bool SDL_window::recreateSurfaces()
{
    SDL_DestroySurface(sdl_surface);
    if (cr_surface) {
        cairo_surface_destroy(cr_surface);
    }

    sdl_surface = SDL_CreateSurface(width * scalingFactor, height * scalingFactor,
        SDL_GetPixelFormatEnumForMasks(32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));
    cr_surface = cairo_image_surface_create_for_data(static_cast<unsigned char*>(sdl_surface->pixels), CAIRO_FORMAT_ARGB32,
        sdl_surface->w, sdl_surface->h, sdl_surface->pitch);

    cairo_surface_set_device_scale(cr_surface, scalingFactor, scalingFactor);

    cr = cairo_create(cr_surface);

    sdl_texture = SDL_CreateTextureFromSurface(renderer, sdl_surface);

    return true;
}

int SDL_window::run()
{
    if (!init()) {
        printf("Failed to init\n");
    }

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
                recreateSurfaces();
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
}

void SDL_window::blit() const
{
    cairo_surface_flush(cr_surface);
    SDL_UpdateTexture(sdl_texture, nullptr, sdl_surface->pixels, int(sdl_surface->pitch));

    auto target = SDL_FRect { 0, 0, float(width * scalingFactor), float(height * scalingFactor) };
    SDL_RenderTexture(renderer, sdl_texture, nullptr, &target);
    SDL_RenderPresent(renderer);
}

void SDL_window::onEvent(SDL_Event& ev)
{
}
