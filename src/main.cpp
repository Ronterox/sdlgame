#include <SDL2/SDL.h>

#include "Logger.h"

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0) LOG("SDL_Init HAS FAILED!!! " << SDL_GetError());
//    if (!(IMG_Init(IMG_INIT_PNG))) LOG("IMG_Init HAS FAILED!!! " << SDL_GetError());

    SDL_Window *window = SDL_CreateWindow("Game v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_SHOWN);

    if (window == NULL) LOG("Window Failed to Init " << SDL_GetError());

    SDL_Event event;

    while (SDL_PollEvent(&event) || event.type != SDL_QUIT)
    {
    }

    SDL_DestroyWindow(window);

    LOG("Windowed destroyed!");

    SDL_Quit();

    return 0;
}