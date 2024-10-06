#pragma once
#include "Z_widget.h"
#include <SDL3/SDL.h>

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

    static void rec_updateLayout(Z_widget* widget);

    /// get what's under cursor
    /// Note that a widget only knows relative pos to its parent
    /// x,y passed here is relative too widget w
    static Z_widget* hit_testing(Z_widget* w, int x, int y);

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