#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindows
{
public:
    RenderWindows(const char *p_title, int p_w, int p_h);

    ~RenderWindows();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};