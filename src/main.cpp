#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/Logger.h"

struct Sprite
{
    SDL_Texture *texture;
    SDL_Rect src;

    Sprite(SDL_Texture *tex, int w, int h)
            : texture(tex), src({0, 0, w, h})
    {
    }
};

struct Entity
{
    float x, y;
    SDL_Rect frame;
    Sprite *spr;

    Entity(float x, float y, int w, int h, Sprite *sprite)
            : x(x), y(y), frame({(int) x, (int) y, w, h}), spr(sprite)
    {
    }
};


int main()
{
    // Initialize libraries, for safety
    if (SDL_Init(SDL_INIT_VIDEO) > 0) LOG("SDL_Init HAS FAILED!!! " << SDL_GetError());
    if (!(IMG_Init(IMG_INIT_PNG))) LOG("IMG_Init HAS FAILED!!! " << SDL_GetError());

    // Create window
    SDL_Window *window = SDL_CreateWindow("Game v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_SHOWN);

    if (window == NULL) LOG("Window Failed to Init " << SDL_GetError());
    else
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        Sprite sprite = {
                IMG_LoadTexture(renderer, "res/gfx/mikopi_square.png"),
                968, 758
        };

        Entity entity = {0, 0, 128, 128, &sprite};

        if (entity.spr->texture == NULL) LOG("Failed to Load texture. Error: " << SDL_GetError());

        SDL_Event event;

        float spd = 0;

        while (SDL_PollEvent(&event) || event.type != SDL_QUIT)
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, entity.spr->texture, &entity.spr->src, &entity.frame);
            SDL_RenderPresent(renderer);

            spd += 0.1;

            entity.frame.x = spd;
            entity.frame.y = spd;
        }

        SDL_DestroyWindow(window);

        LOG("Windowed destroyed!");
    }

    // Quit every sdl memory usage
    SDL_Quit();

    return 0;
}