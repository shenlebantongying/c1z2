#pragma once

#include <SDL3/SDL.h>
#include <cairo.h>

class SDL_window {

public:
    SDL_window(int init_width, int init_height);

    virtual void draw() = 0;

    virtual bool recreateSurfaces();
    virtual void updateStates() = 0;
    virtual bool additionalInit() { return true; };
    virtual void onEvent(SDL_Event& ev);

    void requestRedraw() { needRedraw = true; };

    Uint32 lastUpdateTime = 0;

    int run();

    cairo_t* cr;

    int width;
    int height;

protected:
    virtual ~SDL_window();

private:
    float scalingFactor;

    bool init();
    void blit() const;

    bool needRedraw = false;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* sdl_surface;
    SDL_Texture* sdl_texture;
    cairo_surface_t* cr_surface;
};