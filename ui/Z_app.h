#pragma once
#include "Z_widget.h"
#include <SDL.h>

struct Z_app {

    Z_app(int init_width, int init_height);

    int run();


    void resize(int w, int h);
    /// renew texture
    void updateTexture();

    Z_widget* getMainWidget();

    int width;
    int height;

    Z_widget* mainWidget = nullptr;

    void static rec_updateLayout(Z_widget* widget);

private:
    ~Z_app();

    void rec_updateTexture(Z_widget* widget, int base_x, int base_y);
    void blit();

    int width_pixel;
    int height_pixel;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* sdl_texture;
};