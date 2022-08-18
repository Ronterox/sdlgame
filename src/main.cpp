#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "../include/Logger.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define FPS_LIMIT 60

struct Sprite
{
    SDL_Texture* texture;
    SDL_Rect src;

    Sprite(SDL_Texture* tex, const int w, const int h)
            : texture(tex), src({ 0, 0, w, h })
    {
    }
};

struct Entity
{
    SDL_Rect dest;
    Sprite* spr;

    Entity(const int x, const int y, const int w, const int h, Sprite* sprite)
            : dest({ x, y, w, h }), spr(sprite)
    {
    }
};

double timePassed()
{
    return SDL_GetTicks() * 0.001;
}


int main()
{
    // Initialize libraries, for safety
    if (SDL_Init(SDL_INIT_VIDEO) > 0) LOG("SDL_Init HAS FAILED!!!. Error: " << SDL_GetError());
    if (!(IMG_Init(IMG_INIT_PNG))) LOG("IMG_Init HAS FAILED!!!. Error: " << SDL_GetError());

    // Create window
    SDL_Window* window = SDL_CreateWindow("Game v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) LOG("Window Failed to Init. Error: " << SDL_GetError());
    else
    {
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        Sprite sprite = {
                IMG_LoadTexture(renderer, "res/gfx/mikopi_square.png"),
                968, 758
        };

        if (sprite.texture == nullptr) LOG("Failed to Load texture. Error: " << SDL_GetError());

        const int SPR_SIZE = 128, Y_POS = WIN_HEIGHT - SPR_SIZE;

        std::vector<Entity> entities = {
                { 0,       Y_POS, SPR_SIZE, SPR_SIZE, &sprite },
                { 128,     Y_POS, SPR_SIZE, SPR_SIZE, &sprite },
                { 128 * 2, Y_POS, SPR_SIZE, SPR_SIZE, &sprite },
                { 128 * 3, Y_POS, SPR_SIZE, SPR_SIZE, &sprite },
                { 128 * 4, Y_POS, SPR_SIZE, SPR_SIZE, &sprite },
                { 128 * 5, Y_POS, SPR_SIZE, SPR_SIZE, &sprite },
                { 128 * 6, Y_POS, SPR_SIZE, SPR_SIZE, &sprite }
        };

        SDL_Event event;

        const float TIME_STEP = 1.f / FPS_LIMIT;
        double accumulator = 0.f, lastTime = timePassed();

        while (SDL_PollEvent(&event) || event.type != SDL_QUIT)
        {
            double currentTime = timePassed();

            accumulator += currentTime - lastTime;
            lastTime = currentTime;

            if (accumulator >= TIME_STEP)
            {
                accumulator -= TIME_STEP;

                SDL_RenderClear(renderer);

                for (Entity& entity : entities)
                {
                    entity.dest.y--;
                    SDL_RenderCopy(renderer, entity.spr->texture, &entity.spr->src, &entity.dest);
                }

                SDL_RenderPresent(renderer);
            }
        }

        // TODO: Check if is necessary destroying everything
        SDL_DestroyRenderer(renderer);
    }

    SDL_DestroyWindow(window);

    LOG("Windowed destroyed!");


// Quit every sdl memory usage
    SDL_Quit();

    return 0;
}