#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 400;
const Uint32 SCREEN_FORMAT = SDL_PIXELFORMAT_RGBA8888;

void putPixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawPoint(renderer, x, y);
}


void mainLoop()
{

}

void prepareScene(SDL_Renderer* renderer, SDL_Texture* texture)
{
	SDL_SetRenderDrawColor(renderer, 10, 12, 25, 255);
	SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void presentScene(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    srand(static_cast<unsigned int>(time(NULL))); // seed the random number generator

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Game Loop", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SCREEN_FORMAT, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    Uint32* pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

    bool isRunning = true;
    SDL_Event event;

    while (isRunning)
    {
        prepareScene(renderer, texture);

        // handle events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) // window closed
            {
                isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN) // key pressed
            {
                if (event.key.keysym.sym == SDLK_ESCAPE) // escape key
                {
                    isRunning = false;
                }
            }
        }

        

        // generate a random pixel color and location
        Uint8 r = 255;//rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;
        int x = rand() % SCREEN_WIDTH;
        int y = rand() % SCREEN_HEIGHT;

        // clear the screen
        memset(pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

        // put the random pixel on the screen
        putPixel(renderer, x, y, r, g, b, 255);
        putPixel(renderer, x+1, y, r, g, b, 255);
        putPixel(renderer, x, y+1, r, g, b, 255);
        putPixel(renderer, x+1, y+1, r, g, b, 255);

        // copy the pixels to the texture
        SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

        // render the texture
        

        presentScene(renderer);
        SDL_Delay(33);
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);

    // App Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
