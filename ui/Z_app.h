#pragma once
#include "Z_widget.h"
#include <SDL.h>

struct Z_app {

    Z_app(int init_width, int init_height);

    int run();

    bool adjustTexture();

    Z_widget * getMainWidget();

    int width;
    int height;

    Z_widget* mainWidget = nullptr;

private:
    ~Z_app();

    float scalingFactor;

    void blit() const;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* sdl_texture;
};