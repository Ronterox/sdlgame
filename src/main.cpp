#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.hpp"
#include "Logger.h"

int main(int argv, char *args[])
{
    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        LOG("SDL_Init HAS FAILED!!! " << SDL_GetError());
    if (!(IMG_Init(IMG_INIT_PNG)))
        LOG("IMG_Init HAS FAILED!!! " << SDL_GetError());

    LOG("It Runs!");

    RenderWindows windows("GAME v0.1", 800, 640);

    bool gameRunning = true;

    SDL_Event event;

    while (gameRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) gameRunning = false;
        }
    }

    SDL_Quit();

    return 0;
}