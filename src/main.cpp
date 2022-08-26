#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <memory>

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

        if (spr->texture == nullptr) LOG("Failed to Load texture. Error: " << SDL_GetError());
    }
};

char MAP_CHAR[][7] = {
        { 'x', 'x', 'x', 'x', 'x', 'x', 'x' },
        { 'x', ' ', 'x', ' ', 'x', ' ', 'x' },
        { 'x', ' ', ' ', 'p', ' ', ' ', 'x' },
        { 'x', ' ', ' ', ' ', ' ', ' ', 'x' },
        { 'x', ' ', ' ', ' ', ' ', ' ', 'x' },
};

struct Node
{
    int data;
    Node* left;
    Node* right;

    explicit Node(int data)
            : data(data), left(nullptr), right(nullptr)
    {
    }

    ~Node()
    {
        LOG("Destruction! of " << data);
    }
};

std::ostream& operator<<(std::ostream& str, Node const& v)
{
    return str << "Node: " << v.data;
}

Node* AddNode(Node* root, int data)
{
    if (!root) root = new Node(data);
    else if (data <= root->data) root->left = AddNode(root->left, data);
    else root->right = AddNode(root->right, data);

    return root;
}

Node* SearchNode(Node* root, int data)
{
    if (!root || root->data == data) return root;
    return SearchNode(data < root->data ? root->left : root->right, data);
}

Node* GetMin(Node* root)
{
    return root->left ? GetMin(root->left) : root;
}

Node* RemoveNode(Node* root, int data)
{
    if (!root) return nullptr;

    if (root->data != data)
    {
        root->right = RemoveNode(root->right, data);
        root->left = RemoveNode(root->left, data);
    }
    else if (root->right)
    {
        if (!root->left)
        {
            Node* temp = root;
            root = root->right;
            delete temp;
        }
        else
        {
            // TODO: Understand this better
            Node* min = GetMin(root->right);
            root->data = min->data;
            root->right = RemoveNode(root->right, min->data);
        }
    }
    else if (root->left)
    {
        Node* temp = root;
        root = root->left;
        delete temp;
    }
    else
    {
        Node* temp = root;
        delete temp;
    }

    return root;
}

void FreeNodes(Node* root)
{
    if (root->left) FreeNodes(root->left);
    if (root->right) FreeNodes(root->right);

    delete root;
}

int main()
{
    auto show = [](Node* node) { if (node) LOG(*node); };

    Node* root = nullptr;

    root = AddNode(root, 5);
    root = AddNode(root, 7);
    root = AddNode(root, 10);
    root = AddNode(root, 4);

    show(SearchNode(root, 5));
    show(SearchNode(root, 0));
    show(SearchNode(root, 10));
    show(SearchNode(root, 4));

    // TODO: Fix errors I guess
    root = RemoveNode(root, 10);
    show(SearchNode(root, 10));

    FreeNodes(root);

    return 0;
    // Initialize libraries, for safety
    if (SDL_Init(SDL_INIT_VIDEO) > 0) LOG("SDL_Init HAS FAILED!!!. Error: " << SDL_GetError());
    if (!(IMG_Init(IMG_INIT_PNG))) LOG("IMG_Init HAS FAILED!!!. Error: " << SDL_GetError());

    // Create window
    SDL_Window* window = SDL_CreateWindow("Game v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) LOG("Window Failed to Init. Error: " << SDL_GetError());
    else
    {
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        Sprite tileSprite = {
                IMG_LoadTexture(renderer, "res/gfx/mikopi_square.png"),
                968, 758
        };

        Sprite sakuyaSprite = {
                IMG_LoadTexture(renderer, "res/gfx/sakuya.png"),
                510, 830
        };

        const int SPR_SIZE = 128;

        auto CreateEntity = [](const int x, const int y, const int w, const int h, Sprite* sprite) -> std::shared_ptr<Entity> { return std::make_shared<Entity>(x, y, w, h, sprite); };

        std::vector<std::shared_ptr<Entity>> entities;

        std::shared_ptr<Entity> sakuya;

        int mapSize = sizeof(MAP_CHAR);
        int rowSize = sizeof(MAP_CHAR[0]), charSize = sizeof(MAP_CHAR[0][0]);

        for (int i = 0; i < mapSize / rowSize; i++)
        {
            for (int j = 0; j < rowSize / charSize; j++)
            {
                char obj = MAP_CHAR[i][j];
                if (obj == 'x')
                    entities.push_back(CreateEntity(SPR_SIZE * j, SPR_SIZE * i, SPR_SIZE, SPR_SIZE, &tileSprite));
                else if (obj == 'p')
                {
                    sakuya = CreateEntity(SPR_SIZE * j, SPR_SIZE * i, SPR_SIZE, SPR_SIZE * 1.5, &sakuyaSprite);
                    entities.push_back(sakuya);
                }
            }
        }

        entities.push_back(sakuya);

        SDL_Event event;

        auto TimePassed = []() -> double { return SDL_GetTicks() * 0.001; };

        const float TIME_STEP = 1.f / FPS_LIMIT;
        double accumulator = 0.f, lastTime = TimePassed();

        int spd = 10;

        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

        while (SDL_PollEvent(&event) || event.type != SDL_QUIT)
        {
            // Update FPS
            double currentTime = TimePassed();

            accumulator += currentTime - lastTime;
            lastTime = currentTime;

            if (accumulator < TIME_STEP) continue;

            accumulator -= TIME_STEP;

            // Update Input
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            {
                if (keyboardState[SDL_SCANCODE_ESCAPE]) event.type = SDL_QUIT;

                if (keyboardState[SDL_SCANCODE_LEFT]) sakuya->dest.x -= spd;
                if (keyboardState[SDL_SCANCODE_RIGHT]) sakuya->dest.x += spd;
                if (keyboardState[SDL_SCANCODE_UP]) sakuya->dest.y -= spd;
                if (keyboardState[SDL_SCANCODE_DOWN]) sakuya->dest.y += spd;
            }

            // Update & Draw Screen
            SDL_RenderClear(renderer);

            for (const std::shared_ptr<Entity>& entity : entities)
                SDL_RenderCopy(renderer, entity->spr->texture, &entity->spr->src, &entity->dest);

            SDL_RenderPresent(renderer);
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