#include "RenderWindow.hpp"

#include "Logger.h"

RenderWindows::RenderWindows(const char *p_title, int p_w, int p_h)
        : window(NULL), renderer(NULL)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        LOG("Window Failed to Init " << SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

RenderWindows::~RenderWindows()
{
    SDL_DestroyWindow(window);
    LOG("Destroyed The Window!");
}
